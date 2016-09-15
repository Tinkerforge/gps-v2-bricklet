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
#include "bricklib2/logging/logging.h"
#include "bricklib2/bootloader/bootloader.h"
#include "bricklib2/bootloader/tinydma.h"
#include "bricklib2/hal/system_timer/system_timer.h"
#include "bricklib2/hal/uartbb/uartbb.h"
#include "communication.h"
#include "firefly_x1.h"

#define SYSTEM_TIMER_FREQUENCY 1000 // Use 1 kHz system timer

FireFlyX1 firefly_x1;

int main(void) {
	system_timer_init(CONF_CLOCK_DPLL_OUTPUT_FREQUENCY, SYSTEM_TIMER_FREQUENCY);
	uartbb_init();

	uartbb_puts("Start GPS Bricklet 2.0\n\r");

	bootloader_init();
	firefly_x1_init(&firefly_x1);

	while(true) {
		bootloader_tick();
		firefly_x1_tick(&firefly_x1);

		system_timer_sleep_ms(250);
		uartbb_puts("time: "); uartbb_puti(system_timer_get_ms()); uartbb_putnl();
	}

}
