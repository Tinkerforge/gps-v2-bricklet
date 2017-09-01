/* gps-v2-bricklet
 * Copyright (C) 2016 Olaf Lüke <olaf@tinkerforge.com>
 *
 * main.c: Initialization for GPS Bricklet 2.0
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

#include <stdio.h>
#include <stdbool.h>

#include "xmc_scu.h"
#include "xmc_spi.h"
#include "xmc_gpio.h"
#include "configs/config.h"

#include "bricklib2/bootloader/bootloader.h"
#include "bricklib2/hal/uartbb/uartbb.h"
#include "bricklib2/utility/communication_callback.h"
#include "communication.h"
#include "firefly_x1.h"

#define SYSTEM_TIMER_FREQUENCY 1000 // Use 1 kHz system timer

FireFlyX1 firefly_x1;

int main(void) {
	communication_callback_init();
	firefly_x1_init(&firefly_x1);

	while(true) {
		bootloader_tick();
		firefly_x1_tick(&firefly_x1);
		communication_tick();
	}

}
