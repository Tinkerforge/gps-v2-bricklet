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

#define FIREFLY_X1_SPI_SPEED              400000 // TODO: What speed do we want here?
#define FIREFLY_X1_SPI_MODULE             SERCOM1
#define FIREFLY_X1_SPI_SIGNALMUX_SETTING  SPI_SIGNAL_MUX_SETTING_D // DOPO=0, DIPO=3
#define FIREFLY_X1_SPI_SS_PIN             PIN_PA24
#define FIREFLY_X1_PINMUX_PAD0            PINMUX_PA22C_SERCOM1_PAD0 // PA22, MOSI
#define FIREFLY_X1_PINMUX_PAD1            PINMUX_PA23C_SERCOM1_PAD1 // PA23, CLK
#define FIREFLY_X1_PINMUX_PAD2            PINMUX_PA24C_SERCOM1_PAD2 // PA24, SS
#define FIREFLY_X1_PINMUX_PAD3            PINMUX_PA25C_SERCOM1_PAD3 // PA25, MISO
#define FIREFLY_X1_PERIPHERAL_TRIGGER_TX  SERCOM1_DMAC_ID_TX
#define FIREFLY_X1_PERIPHERAL_TRIGGER_RX  SERCOM1_DMAC_ID_RX

#define FIREFLY_X1_RX_INDEX               2
#define FIREFLY_X1_TX_INDEX               3

#define FIREFLY_X1_PIN_INTERRUPT          PIN_PA15
#define FIREFLY_X1_PIN_NRESET             PIN_PA14

#endif
