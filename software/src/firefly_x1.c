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
//	XMC_GPIO_SetOutputHigh(UARTBB_TX_PIN);
	while(!XMC_USIC_CH_RXFIFO_IsEmpty(FIREFLY_X1_USIC)) {
		firefly_x1.ringbuffer_recv.buffer[firefly_x1.ringbuffer_recv.end] = FIREFLY_X1_USIC->OUTR;
		firefly_x1.ringbuffer_recv.end = (firefly_x1.ringbuffer_recv.end + 1) & FIREFLY_X1_RECV_BUFFER_MASK;

		firefly_x1.buffer_recv_counter++;
	}

//	XMC_GPIO_SetOutputLow(UARTBB_TX_PIN);
}

void /*__attribute__((optimize("-O3")))*/ firefly_x1_tx_irq_handler(void) {
//	XMC_GPIO_SetOutputHigh(UARTBB_TX_PIN);
	while(!XMC_USIC_CH_TXFIFO_IsFull(FIREFLY_X1_USIC)) {
		FIREFLY_X1_USIC->IN[0] = firefly_x1.buffer_send[firefly_x1.buffer_send_index];
		firefly_x1.buffer_send_index++;
		if(firefly_x1.buffer_send_index == FIREFLY_X1_SEND_BUFFER_SIZE) {
			XMC_USIC_CH_TXFIFO_DisableEvent(FIREFLY_X1_USIC, XMC_USIC_CH_TXFIFO_EVENT_CONF_STANDARD);
//			XMC_GPIO_SetOutputLow(UARTBB_TX_PIN);
			return;
		}
	}
//	XMC_GPIO_SetOutputLow(UARTBB_TX_PIN);
}


void firefly_x1_init(FireFlyX1 *firefly_x1) {
	// Initialize struct
	memset(firefly_x1->buffer_recv, 0, FIREFLY_X1_RECV_BUFFER_SIZE);
	memset(firefly_x1->buffer_send, 0, FIREFLY_X1_SEND_BUFFER_SIZE);
	firefly_x1->buffer_send_index = 0;
	firefly_x1->state = FIREFLY_X1_STATE_WAIT_FOR_INTERRUPT;
	firefly_x1->wait_8ms_start_time = 0;
	ringbuffer_init(&firefly_x1->ringbuffer_recv, FIREFLY_X1_RECV_BUFFER_SIZE, (uint8_t*)firefly_x1->buffer_recv);

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

void firefly_x1_handle_sentence(FireFlyX1 *firefly_x1, const char *sentence) {
	enum minmea_sentence_id id = minmea_sentence_id(sentence, false);
	uartbb_puts("sentence id: "); uartbb_puti(id); uartbb_putnl();

	char talker_id[3];
	minmea_talker_id(talker_id, sentence);
	uartbb_puts("talker id: "); uartbb_puts(talker_id); uartbb_putnl();
	FireFlyX1Talker talker;

	if(talker_id[0] == 'G' && talker_id[1] == 'P') {
		talker = TALKER_GPS;
	} else if(talker_id[0] == 'G' && talker_id[1] == 'L') {
		talker = TALKER_GLONASS;
	} else if(talker_id[0] == 'G' && talker_id[1] == 'N') {
		talker = TALKER_MIXED;
	} else {
		// Unknown talker
		return;
	}


	switch(id) {
		case MINMEA_INVALID: {
			uartbb_puts("Invalid sentence\n\r");
			uartbb_puts(sentence);
			break;
		}

		case MINMEA_UNKNOWN: {
			uartbb_puts("Unknown sentence\n\r");
			uartbb_puts(sentence);
			break;
		}

		case MINMEA_SENTENCE_RMC: {
			struct minmea_sentence_rmc rmc;
			minmea_parse_rmc(&rmc, sentence);

			firefly_x1->mixed.time      = rmc.time;
			firefly_x1->mixed.valid     = rmc.valid;
			firefly_x1->mixed.latitude  = rmc.latitude;
			firefly_x1->mixed.longitude = rmc.longitude;
			firefly_x1->mixed.speed     = rmc.speed;
			firefly_x1->mixed.course    = rmc.course;
			firefly_x1->mixed.variation = rmc.variation;

			break;
		}

		case MINMEA_SENTENCE_GGA: {
			struct minmea_sentence_gga gga;
			minmea_parse_gga(&gga, sentence);

			firefly_x1->mixed.time               = gga.time;
			firefly_x1->mixed.latitude           = gga.latitude;
			firefly_x1->mixed.longitude          = gga.longitude;
			firefly_x1->mixed.fix_quality        = gga.fix_quality;
			firefly_x1->mixed.satellites_tracked = gga.satellites_tracked;
			firefly_x1->mixed.hdop               = gga.hdop;
			firefly_x1->mixed.altitude           = gga.altitude;
			firefly_x1->mixed.altitude_units     = gga.altitude_units;
			firefly_x1->mixed.height             = gga.height;
			firefly_x1->mixed.height_units       = gga.height_units;
			firefly_x1->mixed.dgps_age           = gga.dgps_age;

			break;
		}

		case MINMEA_SENTENCE_GSA: {
			struct minmea_sentence_gsa gsa;
			minmea_parse_gsa(&gsa, sentence);

			FireFlyX1DataSingle *single;
			if(talker == TALKER_GPS) {
				single = &firefly_x1->gps;
			} else if(talker == TALKER_GLONASS) {
				single = &firefly_x1->glonass;
			} else {
				return;
			}

		    single->mode = gsa.mode;
		    single->fix_type = gsa.fix_type;
		    memcpy(single->sats, gsa.sats, 12);
		    single->pdop = gsa.pdop;
		    single->hdop = gsa.hdop;
		    single->vdop = gsa.vdop;

			break;
		}

		case MINMEA_SENTENCE_GLL: {
			uartbb_puts("Got GLL sentence!\n\r");
			// Not supported by Firefly X1
			break;
		}

		case MINMEA_SENTENCE_GST: {
			uartbb_puts("Got GST sentence!\n\r");
			// Not supported Firefly X1
			break;
		}

		case MINMEA_SENTENCE_GSV: {
			struct minmea_sentence_gsv gsv;
			minmea_parse_gsv(&gsv, sentence);

			FireFlyX1DataSingle *single;
			if(talker == TALKER_GPS) {
				single = &firefly_x1->gps;
			} else if(talker == TALKER_GLONASS) {
				single = &firefly_x1->glonass;
			} else {
				return;
			}

			for(uint8_t i = 0; i < 4; i++) {
				uint8_t pnr = gsv.sats[i].nr;
				if(pnr < 1 || pnr > 32) { // pnr should always be between 1 and 32
					continue;
				}

				single->sat_info[pnr-1].nr        = gsv.sats[i].nr;
				single->sat_info[pnr-1].azimuth   = gsv.sats[i].azimuth;
				single->sat_info[pnr-1].elevation = gsv.sats[i].elevation;
				single->sat_info[pnr-1].snr       = gsv.sats[i].snr;
			}

			break;
		}

		case MINMEA_SENTENCE_VTG: {
			struct minmea_sentence_vtg vtg;
			minmea_parse_vtg(&vtg, sentence);

			firefly_x1->mixed.true_track_degrees     = vtg.true_track_degrees;
			firefly_x1->mixed.magnetic_track_degrees = vtg.magnetic_track_degrees;
			firefly_x1->mixed.speed_knots            = vtg.speed_knots;
			firefly_x1->mixed.speed_kph              = vtg.speed_kph;
			firefly_x1->mixed.faa_mode               = vtg.faa_mode;

			break;
		}
	}

	uartbb_putnl();
}

void firefly_x1_handle_state_wait_for_interrupt(FireFlyX1 *firefly_x1) {
	// Interrupt = pin LOW
	if(!XMC_GPIO_GetInput(FIREFLY_X1_INTERRUPT_PIN)) {
		memset(firefly_x1->buffer_send, 0xAA, FIREFLY_X1_SEND_BUFFER_SIZE);

		// Start transfer
		firefly_x1->buffer_recv_counter = 0;
		firefly_x1->buffer_send_index = 0;
		XMC_SPI_CH_EnableSlaveSelect(FIREFLY_X1_USIC, XMC_SPI_CH_SLAVE_SELECT_0);
		XMC_USIC_CH_TXFIFO_EnableEvent(FIREFLY_X1_USIC, XMC_USIC_CH_TXFIFO_EVENT_CONF_STANDARD);
		XMC_USIC_CH_TriggerServiceRequest(FIREFLY_X1_USIC, FIREFLY_X1_SERVICE_REQUEST_TX);

		firefly_x1->state = FIREFLY_X1_STATE_RECEIVE_IN_PROGRESS;
	}
}

void firefly_x1_handle_state_receive_in_progress(FireFlyX1 *firefly_x1) {
	if(firefly_x1->buffer_recv_counter >= FIREFLY_X1_SEND_BUFFER_SIZE) {
		XMC_SPI_CH_DisableSlaveSelect(FIREFLY_X1_USIC);

		firefly_x1->wait_8ms_start_time = system_timer_get_ms();
		firefly_x1->state = FIREFLY_X1_STATE_WAIT_8MS;
	}

}

void firefly_x1_handle_state_wait_8ms(FireFlyX1 *firefly_x1) {
	if(system_timer_is_time_elapsed_ms(firefly_x1->wait_8ms_start_time, 8)) {
		firefly_x1->state = FIREFLY_X1_STATE_WAIT_FOR_INTERRUPT;
	}
}

void firefly_x1_handle_ringbuffer(FireFlyX1 *firefly_x1) {
	static char sentence[FIREFLY_X1_MAX_SENTENCE_LENGTH+1] = {0};
	static int16_t index = 0;

	for(;index < FIREFLY_X1_MAX_SENTENCE_LENGTH; index++) {
		// TODO: Interrupt off
		if(!ringbuffer_get(&firefly_x1->ringbuffer_recv, (uint8_t*)&sentence[index])) {
			// TODO: Interrupt on
			// Ringbuffer is empty
			break;
		}
		// TODO: Interrupt on

		if(sentence[index] == '\n') {
			if(index == 0) {
				// If the first element of a sentence in "\n" it is a placeholder byte
				// and we can start again from the beginning until we get the first
				// real character of a sentence
				index = -1;
				continue;
			}

			firefly_x1_handle_sentence(firefly_x1, sentence);
//			uartbb_puts(sentence);
			memset(sentence, 0 , index+1);
			index = 0;

			// We return after each sentence, so we can return to bootloader
			// between sentences
			break;
		}
	}

	if(index == FIREFLY_X1_MAX_SENTENCE_LENGTH) {
		// Sentence was longer then FIREFLY_X1_MAX_SENTENCE_LENGTH byte, we don't support this
		memset(sentence, 0 , FIREFLY_X1_MAX_SENTENCE_LENGTH);
		index = 0;
	}
}

void firefly_x1_tick(FireFlyX1 *firefly_x1) {
	switch(firefly_x1->state) {
		case FIREFLY_X1_STATE_WAIT_FOR_INTERRUPT:   firefly_x1_handle_state_wait_for_interrupt(firefly_x1);  break;
		case FIREFLY_X1_STATE_RECEIVE_IN_PROGRESS:  firefly_x1_handle_state_receive_in_progress(firefly_x1); break;
		case FIREFLY_X1_STATE_WAIT_8MS:             firefly_x1_handle_state_wait_8ms(firefly_x1);            break;
	}

	firefly_x1_handle_ringbuffer(firefly_x1);
}
