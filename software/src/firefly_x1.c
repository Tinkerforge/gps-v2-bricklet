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
#include "bricklib2/bootloader/bootloader.h"
#include "configs/config_firefly_x1.h"

void firefly_x1_init(FireFlyX1 *firefly_x1) {
	firefly_x1->descriptor_rx = &bootloader_status.st.descriptor_section[FIREFLY_X1_RX_INDEX];
	firefly_x1->descriptor_rx = &bootloader_status.st.descriptor_section[FIREFLY_X1_TX_INDEX];

	// Configure and enable SERCOM SPI module
	struct spi_config firefly_x1_spi_config;
	spi_get_config_defaults(&firefly_x1_spi_config);

	firefly_x1_spi_config.mode = SPI_MODE_MASTER;
	firefly_x1_spi_config.transfer_mode = SPI_TRANSFER_MODE_3;

	firefly_x1_spi_config.mode_specific.master.baudrate = FIREFLY_X1_SPI_SPEED; // TODO: What is a good value here?

	// TODO: Choose correct pins
	firefly_x1_spi_config.mux_setting = FIREFLY_X1_SPI_SIGNALMUX_SETTING; // DOPO=2, DIPO=0
	firefly_x1_spi_config.pinmux_pad0 = FIREFLY_X1_PINMUX_PAD0;  // PA4, MOSI
	firefly_x1_spi_config.pinmux_pad1 = FIREFLY_X1_PINMUX_PAD1;  // PA5, CLK
	firefly_x1_spi_config.pinmux_pad2 = FIREFLY_X1_PINMUX_PAD2;  // PA6, SS
	firefly_x1_spi_config.pinmux_pad3 = FIREFLY_X1_PINMUX_PAD3;  // PA7, MISO

	spi_init(&firefly_x1->spi_module, FIREFLY_X1_SPI_MODULE, &firefly_x1_spi_config);
	spi_enable(&firefly_x1->spi_module);

	TinyDmaChannelConfig firefly_x1_channel_config_rx;
	tinydma_get_channel_config_defaults(&firefly_x1_channel_config_rx);
	firefly_x1_channel_config_rx.peripheral_trigger = FIREFLY_X1_PERIPHERAL_TRIGGER_RX;
	firefly_x1_channel_config_rx.trigger_action = DMA_TRIGGER_ACTION_BEAT;
	tinydma_channel_init(FIREFLY_X1_RX_INDEX, &firefly_x1_channel_config_rx);

	// Configure SPI tx and rx resources
	TinyDmaChannelConfig firefly_x1_channel_config_tx;
	tinydma_get_channel_config_defaults(&firefly_x1_channel_config_tx);
	firefly_x1_channel_config_tx.peripheral_trigger = FIREFLY_X1_PERIPHERAL_TRIGGER_TX;
	firefly_x1_channel_config_tx.trigger_action = DMA_TRIGGER_ACTION_BEAT;
	tinydma_channel_init(FIREFLY_X1_TX_INDEX, &firefly_x1_channel_config_tx);

	// Configure SPI rx descriptor
	TinyDmaDescriptorConfig firefly_x1_descriptor_config_rx;
	tinydma_descriptor_get_config_defaults(&firefly_x1_descriptor_config_rx);
	firefly_x1_descriptor_config_rx.beat_size = DMA_BEAT_SIZE_BYTE;
	firefly_x1_descriptor_config_rx.src_increment_enable = false;
	firefly_x1_descriptor_config_rx.block_transfer_count = SPITFP_RECEIVE_BUFFER_SIZE;
	firefly_x1_descriptor_config_rx.destination_address = (uint32_t)(firefly_x1->buffer_recv + FIREFLY_X1_RECV_BUFFER_SIZE);
	firefly_x1_descriptor_config_rx.source_address = (uint32_t)(&firefly_x1->spi_module.hw->SPI.DATA.reg);
	tinydma_descriptor_init(firefly_x1->descriptor_rx, &firefly_x1_descriptor_config_rx);

	// Configure SPI tx descriptor
	TinyDmaDescriptorConfig firefly_x1_descriptor_config_tx;
	tinydma_descriptor_get_config_defaults(&firefly_x1_descriptor_config_tx);
	firefly_x1_descriptor_config_tx.beat_size = DMA_BEAT_SIZE_BYTE;
	firefly_x1_descriptor_config_tx.dst_increment_enable = false;
	firefly_x1_descriptor_config_tx.src_increment_enable = false;
	firefly_x1_descriptor_config_tx.block_transfer_count = 1;
	firefly_x1_descriptor_config_tx.block_action = DMA_BLOCK_ACTION_INT;
	firefly_x1_descriptor_config_tx.source_address = (uint32_t)(firefly_x1->buffer_send + FIREFLY_X1_SEND_BUFFER_SIZE);
	firefly_x1_descriptor_config_tx.destination_address = (uint32_t)(&firefly_x1->spi_module.hw->SPI.DATA.reg);
	tinydma_descriptor_init(firefly_x1->descriptor_tx, &firefly_x1_descriptor_config_tx);

	// Start dma transfer for rx/tx resource
//	tinydma_start_transfer(FIREFLY_X1_RX_INDEX);
//	tinydma_start_transfer(FIREFLY_X1_TX_INDEX);
}

void firefly_x1_tick(FireFlyX1 *firefly_x1) {

}
