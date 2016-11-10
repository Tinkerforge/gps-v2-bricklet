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

#include "xmc_spi.h"
#include "xmc_gpio.h"

#include "bricklib2/utility/ringbuffer.h"
#include "bricklib2/protocols/tfp/tfp.h"
#include "bricklib2/hal/system_timer/system_timer.h"
#include "bricklib2/hal/uartbb/uartbb.h"
#include "bricklib2/bootloader/bootloader.h"
#include "configs/config_firefly_x1.h"

#include "minmea.h"

extern FireFlyX1 firefly_x1;

#define firefly_x1_rx_irq_handler IRQ_Hdlr_11
#define firefly_x1_tx_irq_handler IRQ_Hdlr_12

void /*__attribute__((optimize("-O3")))*/ firefly_x1_rx_irq_handler(void) {
//	uartbb_puts("irq rx\n\r");

	while(!XMC_USIC_CH_RXFIFO_IsEmpty(FIREFLY_X1_USIC)) {
		firefly_x1.buffer_recv[firefly_x1.buffer_recv_index] = FIREFLY_X1_USIC->OUTR;
		firefly_x1.buffer_recv_index++;
		if(firefly_x1.buffer_recv_index == FIREFLY_X1_RECV_BUFFER_SIZE) {
			XMC_SPI_CH_DisableSlaveSelect(FIREFLY_X1_USIC);
		}
	}


}

void /*__attribute__((optimize("-O3")))*/ firefly_x1_tx_irq_handler(void) {
//	uartbb_puts("irq tx\n\r");
	while(!XMC_USIC_CH_TXFIFO_IsFull(FIREFLY_X1_USIC)) {
		FIREFLY_X1_USIC->IN[0] = firefly_x1.buffer_send[firefly_x1.buffer_send_index];
		firefly_x1.buffer_send_index++;
		if(firefly_x1.buffer_send_index == FIREFLY_X1_SEND_BUFFER_SIZE) {
			XMC_USIC_CH_TXFIFO_DisableEvent(FIREFLY_X1_USIC, XMC_USIC_CH_TXFIFO_EVENT_CONF_STANDARD);
//			uartbb_puts("buffer send\n\r");
			return;
		}
	}
//	uartbb_puts("irq tx full\n\r");
}


void firefly_x1_init(FireFlyX1 *firefly_x1) {
	// Initialize struct
	memset(firefly_x1->buffer_recv, 0, FIREFLY_X1_RECV_BUFFER_SIZE);
	memset(firefly_x1->buffer_send, 0, FIREFLY_X1_RECV_BUFFER_SIZE);
	firefly_x1->buffer_send_index = 0;
	firefly_x1->state = FIREFLY_X1_STATE_WAIT_FOR_INTERRUPT;
	firefly_x1->wait_8ms_start_time = 0;

	// USIC channel configuration
	const XMC_SPI_CH_CONFIG_t channel_config = {
		.baudrate       = 100000,
		.bus_mode       = XMC_SPI_CH_BUS_MODE_MASTER,
		.selo_inversion = XMC_SPI_CH_SLAVE_SEL_INV_TO_MSLS,
		.parity_mode    = XMC_USIC_CH_PARITY_MODE_NONE
	};

	// MISO pin configuration
	const XMC_GPIO_CONFIG_t mosi_pin_config = {
	  .mode             = FIREFLY_X1_MOSI_PIN_AF,
	  .output_level     = XMC_GPIO_OUTPUT_LEVEL_HIGH
	};

	// MOSI pin configuration
	const XMC_GPIO_CONFIG_t miso_pin_config = {
	  .mode             = XMC_GPIO_MODE_INPUT_TRISTATE,
	  .input_hysteresis = XMC_GPIO_INPUT_HYSTERESIS_STANDARD
	};

	// SCLK pin configuration
	const XMC_GPIO_CONFIG_t sclk_pin_config = {
	  .mode             = FIREFLY_X1_SCLK_PIN_AF,
	  .output_level     = XMC_GPIO_OUTPUT_LEVEL_HIGH
	};

	// SELECT pin configuration
	const XMC_GPIO_CONFIG_t select_pin_config = {
	  .mode             = FIREFLY_X1_SELECT_PIN_AF,
	  .output_level     = XMC_GPIO_OUTPUT_LEVEL_HIGH
	};

	// Interrupt pin configuration
	const XMC_GPIO_CONFIG_t interrupt_pin_config = {
	  .mode             = XMC_GPIO_MODE_INPUT_TRISTATE,
	  .output_level     = XMC_GPIO_OUTPUT_LEVEL_HIGH,
	  .input_hysteresis = XMC_GPIO_INPUT_HYSTERESIS_STANDARD
	};

	// Reset pin configuration
	const XMC_GPIO_CONFIG_t nreset_pin_config = {
	  .mode             = XMC_GPIO_MODE_OUTPUT_PUSH_PULL,
	  .output_level     = XMC_GPIO_OUTPUT_LEVEL_HIGH, // not reset
	};

	// PPS pin configuration
	const XMC_GPIO_CONFIG_t pps_pin_config = {
	  .mode             = XMC_GPIO_MODE_INPUT_TRISTATE,
	  .output_level     = XMC_GPIO_OUTPUT_LEVEL_HIGH,
	  .input_hysteresis = XMC_GPIO_INPUT_HYSTERESIS_STANDARD
	};

	// Configure GPIO pins
	XMC_GPIO_Init(FIREFLY_X1_INTERRUPT_PIN, &interrupt_pin_config);
	XMC_GPIO_Init(FIREFLY_X1_NRESET_PIN, &nreset_pin_config);
	XMC_GPIO_Init(FIREFLY_X1_PPS_PIN, &pps_pin_config);

	// Configure MISO pin
	XMC_GPIO_Init(FIREFLY_X1_MOSI_PIN, &miso_pin_config);

	// Initialize USIC channel in SPI master mode
	XMC_SPI_CH_Init(FIREFLY_X1_USIC, &channel_config);
	FIREFLY_X1_USIC->SCTR &= ~USIC_CH_SCTR_PDL_Msk; // Set passive data level to 0

	XMC_SPI_CH_SetBitOrderMsbFirst(FIREFLY_X1_USIC);

	XMC_SPI_CH_SetWordLength(FIREFLY_X1_USIC, (uint8_t)8U);
	XMC_SPI_CH_SetFrameLength(FIREFLY_X1_USIC, (uint8_t)64U);

	XMC_SPI_CH_SetTransmitMode(FIREFLY_X1_USIC, XMC_SPI_CH_MODE_STANDARD);

	// Configure the clock polarity and clock delay
	XMC_SPI_CH_ConfigureShiftClockOutput(FIREFLY_X1_USIC,
									     XMC_SPI_CH_BRG_SHIFT_CLOCK_PASSIVE_LEVEL_1_DELAY_DISABLED,
									     XMC_SPI_CH_BRG_SHIFT_CLOCK_OUTPUT_SCLK);
	// Configure Leading/Trailing delay
	XMC_SPI_CH_SetSlaveSelectDelay(FIREFLY_X1_USIC, 2);


	// Set input source path
	XMC_SPI_CH_SetInputSource(FIREFLY_X1_USIC, FIREFLY_X1_MISO_INPUT, FIREFLY_X1_MISO_SOURCE);

	// SPI Mode: CPOL=1 and CPHA=1
	FIREFLY_X1_USIC_CHANNEL->DX1CR |= USIC_CH_DX1CR_DPOL_Msk;

	// TODO: this is probably not needed
	  XMC_USIC_CH_SetInterruptNodePointer(FIREFLY_X1_USIC,
	                                      XMC_USIC_CH_INTERRUPT_NODE_POINTER_PROTOCOL,
	                                      (uint32_t)FIREFLY_X1_SERVICE_REQUEST_TX);

	// Configure transmit FIFO
	XMC_USIC_CH_TXFIFO_Configure(FIREFLY_X1_USIC, 48, XMC_USIC_CH_FIFO_SIZE_16WORDS, 8);

	// Configure receive FIFO
	XMC_USIC_CH_RXFIFO_Configure(FIREFLY_X1_USIC, 32, XMC_USIC_CH_FIFO_SIZE_16WORDS, 0);
//	FIREFLY_X1_USIC->RBCTR |= USIC_CH_RBCTR_SRBTM_Msk; // RX Interrupt for >= 0

	// Set service request for tx FIFO transmit interrupt
	XMC_USIC_CH_TXFIFO_SetInterruptNodePointer(FIREFLY_X1_USIC, XMC_USIC_CH_TXFIFO_INTERRUPT_NODE_POINTER_STANDARD, FIREFLY_X1_SERVICE_REQUEST_TX);  // IRQ FIREFLY_X1_IRQ_TX

	// Set service request for rx FIFO receive interrupt
	XMC_USIC_CH_RXFIFO_SetInterruptNodePointer(FIREFLY_X1_USIC, XMC_USIC_CH_RXFIFO_INTERRUPT_NODE_POINTER_STANDARD, FIREFLY_X1_SERVICE_REQUEST_RX);  // IRQ FIREFLY_X1_IRQ_RX
	XMC_USIC_CH_RXFIFO_SetInterruptNodePointer(FIREFLY_X1_USIC, XMC_USIC_CH_RXFIFO_INTERRUPT_NODE_POINTER_ALTERNATE, FIREFLY_X1_SERVICE_REQUEST_RX); // IRQ FIREFLY_X1_IRQ_RX

	//Set priority and enable NVIC node for transmit interrupt
	NVIC_SetPriority((IRQn_Type)FIREFLY_X1_IRQ_TX, FIREFLY_X1_IRQ_TX_PRIORITY);
	NVIC_EnableIRQ((IRQn_Type)FIREFLY_X1_IRQ_TX);

	// Set priority and enable NVIC node for receive interrupt
	NVIC_SetPriority((IRQn_Type)FIREFLY_X1_IRQ_RX, FIREFLY_X1_IRQ_RX_PRIORITY);
	NVIC_EnableIRQ((IRQn_Type)FIREFLY_X1_IRQ_RX);

	// Start SPI
	XMC_SPI_CH_Start(FIREFLY_X1_USIC);

	// Configure SCLK pin
	XMC_GPIO_Init(FIREFLY_X1_SCLK_PIN, &sclk_pin_config);

	// Configure slave select pin
	XMC_GPIO_Init(FIREFLY_X1_SELECT_PIN, &select_pin_config);

	//Configure MOSI pin
	XMC_GPIO_Init(FIREFLY_X1_MOSI_PIN, &mosi_pin_config);

	//XMC_USIC_CH_EnableEvent(FIREFLY_X1_USIC, (uint32_t)((uint32_t)XMC_USIC_CH_EVENT_STANDARD_RECEIVE | (uint32_t)XMC_USIC_CH_EVENT_ALTERNATIVE_RECEIVE));
	XMC_USIC_CH_RXFIFO_EnableEvent(FIREFLY_X1_USIC, XMC_USIC_CH_RXFIFO_EVENT_CONF_STANDARD | XMC_USIC_CH_RXFIFO_EVENT_CONF_ALTERNATE);

}

void firefly_x1_handle_sentence(const char *sentence) {
	enum minmea_sentence_id id = minmea_sentence_id(sentence, false);
	uartbb_puts("sentence id: "); uartbb_puti(id); uartbb_putnl();
}

void firefly_x1_handle_state_wait_for_interrupt(FireFlyX1 *firefly_x1) {
	// Interrupt = pin LOW
	if(!XMC_GPIO_GetInput(FIREFLY_X1_INTERRUPT_PIN)) {
		memset(firefly_x1->buffer_send, 0xAA, FIREFLY_X1_SEND_BUFFER_SIZE);

		// Start transfer
		firefly_x1->buffer_recv_index = 0;
		firefly_x1->buffer_send_index = 0;
		XMC_SPI_CH_EnableSlaveSelect(FIREFLY_X1_USIC, XMC_SPI_CH_SLAVE_SELECT_0);
		XMC_USIC_CH_TXFIFO_EnableEvent(FIREFLY_X1_USIC, XMC_USIC_CH_TXFIFO_EVENT_CONF_STANDARD);
		XMC_USIC_CH_TriggerServiceRequest(FIREFLY_X1_USIC, FIREFLY_X1_SERVICE_REQUEST_TX);

		firefly_x1->state = FIREFLY_X1_STATE_RECEIVE_IN_PROGRESS;
	}
}

void firefly_x1_handle_state_receive_in_progress(FireFlyX1 *firefly_x1) {
	if(firefly_x1->buffer_recv_index == FIREFLY_X1_RECV_BUFFER_SIZE) {
		firefly_x1->state = FIREFLY_X1_STATE_NEW_DATA_RECEIVED;
		XMC_USIC_CH_TXFIFO_DisableEvent(FIREFLY_X1_USIC, XMC_USIC_CH_TXFIFO_EVENT_CONF_STANDARD);
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
