/* gps-v2-bricklet
 * Copyright (C) 2016 Olaf Lüke <olaf@tinkerforge.com>
 *
 * firefly_x1.c: SPI communication with FireFly X1 GPS module
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#include "firefly_x1.h"

#include "spi.h"
#include "bricklib2/utility/ringbuffer.h"
#include "bricklib2/protocols/tfp/tfp.h"
#include "bricklib2/hal/system_timer/system_timer.h"
#include "bricklib2/hal/uartbb/uartbb.h"
#include "bricklib2/bootloader/bootloader.h"
#include "configs/config_firefly_x1.h"

#include "minmea.h"

void firefly_x1_init(FireFlyX1 *firefly_x1) {
	// Configure default state
	firefly_x1->state = FIREFLY_X1_STATE_WAIT_FOR_INTERRUPT;

	// Configure interrupt and reset pins
	struct port_config pin_conf;

	port_get_config_defaults(&pin_conf);
	port_pin_set_config(FIREFLY_X1_PIN_INTERRUPT, &pin_conf);

	pin_conf.direction = PORT_PIN_DIR_OUTPUT;
	port_pin_set_config(FIREFLY_X1_PIN_NRESET, &pin_conf);
	port_pin_set_output_level(FIREFLY_X1_PIN_NRESET, true);

	// Configure and enable SERCOM SPI module
	struct spi_slave_inst_config slave_dev_config;
	spi_slave_inst_get_config_defaults(&slave_dev_config);
	slave_dev_config.ss_pin = FIREFLY_X1_SPI_SS_PIN;
	spi_attach_slave(&firefly_x1->slave, &slave_dev_config);

	struct spi_config firefly_x1_spi_config;
	spi_get_config_defaults(&firefly_x1_spi_config);

	firefly_x1_spi_config.master_slave_select_enable = false;
	firefly_x1_spi_config.mode = SPI_MODE_MASTER;
	firefly_x1_spi_config.transfer_mode = SPI_TRANSFER_MODE_3;

	firefly_x1_spi_config.mode_specific.master.baudrate = FIREFLY_X1_SPI_SPEED;

	firefly_x1_spi_config.mux_setting = FIREFLY_X1_SPI_SIGNALMUX_SETTING; // DOPO=0, DIPO=3
	firefly_x1_spi_config.pinmux_pad0 = FIREFLY_X1_PINMUX_PAD0;  // PA22, MOSI
	firefly_x1_spi_config.pinmux_pad1 = FIREFLY_X1_PINMUX_PAD1;  // PA23, CLK

	// We don't use the hardware SS, since it selects/deselects between each byte...
	firefly_x1_spi_config.pinmux_pad2 = PINMUX_UNUSED; //FIREFLY_X1_PINMUX_PAD2;  // PA24, SS
	firefly_x1_spi_config.pinmux_pad3 = FIREFLY_X1_PINMUX_PAD3;  // PA25, MISO

	bootloader_spi_init(&firefly_x1->spi_module, FIREFLY_X1_SPI_MODULE, &firefly_x1_spi_config);
	spi_enable(&firefly_x1->spi_module);

	// Configure SPI rx channel
	TinyDmaChannelConfig firefly_x1_channel_config_rx;
	bootloader_tinydma_get_channel_config_defaults(&firefly_x1_channel_config_rx);
	firefly_x1_channel_config_rx.trigger_action = DMA_TRIGGER_ACTION_BEAT;
	firefly_x1_channel_config_rx.peripheral_trigger = SERCOM1_DMAC_ID_RX;
	bootloader_tinydma_channel_init(FIREFLY_X1_RX_INDEX, &firefly_x1_channel_config_rx);

	// Configure SPI tx channel
	TinyDmaChannelConfig firefly_x1_channel_config_tx;
	bootloader_tinydma_get_channel_config_defaults(&firefly_x1_channel_config_tx);
	firefly_x1_channel_config_tx.trigger_action = DMA_TRIGGER_ACTION_BEAT;
	firefly_x1_channel_config_tx.peripheral_trigger = SERCOM1_DMAC_ID_TX;
	bootloader_tinydma_channel_init(FIREFLY_X1_TX_INDEX, &firefly_x1_channel_config_tx);

	// Configure SPI rx descriptor
	TinyDmaDescriptorConfig firefly_x1_descriptor_config_rx;
	bootloader_tinydma_descriptor_get_config_defaults(&firefly_x1_descriptor_config_rx);
	firefly_x1_descriptor_config_rx.beat_size = DMA_BEAT_SIZE_BYTE;
	firefly_x1_descriptor_config_rx.src_increment_enable = false;
	firefly_x1_descriptor_config_rx.step_selection = DMA_STEPSEL_DST;
	firefly_x1_descriptor_config_rx.block_transfer_count = FIREFLY_X1_RECV_BUFFER_SIZE;
	firefly_x1_descriptor_config_rx.destination_address = (uint32_t)(firefly_x1->buffer_recv + FIREFLY_X1_RECV_BUFFER_SIZE);
	firefly_x1_descriptor_config_rx.source_address = (uint32_t)(&firefly_x1->spi_module.hw->SPI.DATA.reg);
	bootloader_tinydma_descriptor_init(&bootloader_status.st.descriptor_section[FIREFLY_X1_RX_INDEX], &firefly_x1_descriptor_config_rx);

	// Configure SPI tx descriptor
	TinyDmaDescriptorConfig firefly_x1_descriptor_config_tx;
	bootloader_tinydma_descriptor_get_config_defaults(&firefly_x1_descriptor_config_tx);
	firefly_x1_descriptor_config_tx.beat_size = DMA_BEAT_SIZE_BYTE;
	firefly_x1_descriptor_config_tx.dst_increment_enable = false;
	firefly_x1_descriptor_config_tx.step_selection = DMA_STEPSEL_SRC;
	firefly_x1_descriptor_config_tx.block_transfer_count = FIREFLY_X1_SEND_BUFFER_SIZE;
	firefly_x1_descriptor_config_tx.source_address = (uint32_t)(firefly_x1->buffer_send + FIREFLY_X1_SEND_BUFFER_SIZE);
	firefly_x1_descriptor_config_tx.destination_address = (uint32_t)(&firefly_x1->spi_module.hw->SPI.DATA.reg);
	bootloader_tinydma_descriptor_init(&bootloader_status.st.descriptor_section[FIREFLY_X1_TX_INDEX], &firefly_x1_descriptor_config_tx);

	memset(firefly_x1->buffer_send, 0, FIREFLY_X1_SEND_BUFFER_SIZE);
}

void firefly_x1_handle_sentence(const char *sentence) {
	enum minmea_sentence_id id = minmea_sentence_id(sentence, false);
	uartbb_puts("sentence id: "); uartbb_puti(id); uartbb_putnl();
}

void firefly_x1_handle_state_wait_for_interrupt(FireFlyX1 *firefly_x1) {
	// Interrupt = pin LOW
	if(!port_pin_get_input_level(FIREFLY_X1_PIN_INTERRUPT)) {
		spi_select_slave(&firefly_x1->spi_module, &firefly_x1->slave, true);
		bootloader_tinydma_start_transfer(FIREFLY_X1_RX_INDEX);
		bootloader_tinydma_start_transfer(FIREFLY_X1_TX_INDEX);

		firefly_x1->state = FIREFLY_X1_STATE_RECEIVE_IN_PROGRESS;
	}
}

void firefly_x1_handle_state_receive_in_progress(FireFlyX1 *firefly_x1) {
	const uint32_t count = TINYDMA_CURRENT_BUFFER_COUNT_FOR_CHANNEL(FIREFLY_X1_RX_INDEX);
	if(count == 0) {
		firefly_x1->state = FIREFLY_X1_STATE_NEW_DATA_RECEIVED;
		spi_select_slave(&firefly_x1->spi_module, &firefly_x1->slave, false);
	}
}

void firefly_x1_handle_state_new_data_received(FireFlyX1 *firefly_x1) {
	firefly_x1->wait_8ms_start_time = system_timer_get_ms();
	firefly_x1->state = FIREFLY_X1_STATE_WAIT_8MS;

	uint16_t start = 0;
	uint16_t last_valid = 0;
	for(uint16_t i = 0; i < FIREFLY_X1_RECV_BUFFER_SIZE; i++) {
		if(firefly_x1->buffer_recv[i] == '\n') {
			if(start != i) {
				last_valid = i;
				char message[257] = {0};
				memcpy(message, &firefly_x1->buffer_recv[start], last_valid - start + 1);
				firefly_x1_handle_sentence(message);
				uartbb_puts(message);
//				uartbb_putnl();

				firefly_x1->buffer_recv[i] = '\0';
			}
			start = i + 1;
		}
	}

	uartbb_puts("Last valid index: "); uartbb_puti(last_valid); uartbb_putnl();
	uartbb_putnl();
}

void firefly_x1_handle_state_wait_8ms(FireFlyX1 *firefly_x1) {
	if(system_timer_is_time_elapsed_ms(firefly_x1->wait_8ms_start_time, 8)) {
		firefly_x1->state = FIREFLY_X1_STATE_WAIT_FOR_INTERRUPT;
	}
}

void firefly_x1_tick(FireFlyX1 *firefly_x1) {
	switch(firefly_x1->state) {
		case FIREFLY_X1_STATE_WAIT_FOR_INTERRUPT:   firefly_x1_handle_state_wait_for_interrupt(firefly_x1);  break;
		case FIREFLY_X1_STATE_RECEIVE_IN_PROGRESS:  firefly_x1_handle_state_receive_in_progress(firefly_x1); break;
		case FIREFLY_X1_STATE_NEW_DATA_RECEIVED:    firefly_x1_handle_state_new_data_received(firefly_x1);   break;
		case FIREFLY_X1_STATE_WAIT_8MS:             firefly_x1_handle_state_wait_8ms(firefly_x1);            break;
	}
}
