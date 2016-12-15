/* gps-v2-bricklet
 * Copyright (C) 2016 Olaf Lüke <olaf@tinkerforge.com>
 *
 * config_firefly_x1.c: Configuration for FieFly X1 GPS module
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

#ifndef CONFIG_FIREFLY_X1_H
#define CONFIG_FIREFLY_X1_H

#include "xmc_gpio.h"

#define FIREFLY_X1_FIX_LED_PIN          P2_2

#define FIREFLY_X1_USIC_CHANNEL         USIC0_CH1
#define FIREFLY_X1_USIC                 XMC_SPI0_CH1

#define FIREFLY_X1_SCLK_PIN             P0_8
#define FIREFLY_X1_SCLK_PIN_AF          (XMC_GPIO_MODE_OUTPUT_PUSH_PULL_ALT7 | P0_8_AF_U0C1_SCLKOUT)

#define FIREFLY_X1_SELECT_PIN           P0_9
#define FIREFLY_X1_SELECT_PIN_AF        (XMC_GPIO_MODE_OUTPUT_PUSH_PULL_ALT7 | P0_9_AF_U0C1_SELO0)

#define FIREFLY_X1_MOSI_PIN             P0_7
#define FIREFLY_X1_MOSI_PIN_AF          (XMC_GPIO_MODE_OUTPUT_PUSH_PULL_ALT7 | P0_7_AF_U0C1_DOUT0)

#define FIREFLY_X1_MISO_PIN             P0_6
#define FIREFLY_X1_MISO_INPUT           XMC_USIC_CH_INPUT_DX0
#define FIREFLY_X1_MISO_SOURCE          0b010 // DX0C

#define FIREFLY_X1_SERVICE_REQUEST_RX   2
#define FIREFLY_X1_SERVICE_REQUEST_TX   3

#define FIREFLY_X1_INTERRUPT_PIN        P2_6
#define FIREFLY_X1_NRESET_PIN           P2_7
#define FIREFLY_X1_PPS_PIN              P2_9
#define FIREFLY_X1_RTCM_PIN             P1_2

#define FIREFLY_X1_RXD_PIN              P2_10
#define FIREFLY_X1_TXD_PIN              P2_11

#define FIREFLY_X1_IRQ_RX               11
#define FIREFLY_X1_IRQ_RX_PRIORITY      2
#define FIREFLY_X1_IRQ_TX               12
#define FIREFLY_X1_IRQ_TX_PRIORITY      3

#endif
