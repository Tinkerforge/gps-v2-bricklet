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
#define FIREFLY_X1_SPI_SIGNALMUX_SETTING  SPI_SIGNAL_MUX_SETTING_I // DOPO=2, DIPO=0
#define FIREFLY_X1_PINMUX_PAD0            PINMUX_PA04D_SERCOM0_PAD0 // PA4, MOSI
#define FIREFLY_X1_PINMUX_PAD1            PINMUX_PA05D_SERCOM0_PAD1 // PA5, CLK
#define FIREFLY_X1_PINMUX_PAD2            PINMUX_PA06D_SERCOM0_PAD2 // PA6, SS
#define FIREFLY_X1_PINMUX_PAD3            PINMUX_PA07D_SERCOM0_PAD3 // PA7, MISO
#define FIREFLY_X1_PERIPHERAL_TRIGGER_TX  SERCOM1_DMAC_ID_TX
#define FIREFLY_X1_PERIPHERAL_TRIGGER_RX  SERCOM1_DMAC_ID_RX

#define FIREFLY_X1_RX_INDEX               2
#define FIREFLY_X1_TX_INDEX               3

#endif
