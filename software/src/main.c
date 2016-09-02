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

const uint32_t device_identifier __attribute__ ((section(".device_identifier"))) = BOOTLOADER_DEVICE_IDENTIFIER;
const uint32_t firmware_version __attribute__ ((section(".firmware_version"))) = (FIRMWARE_VERSION_MAJOR << 16) | (FIRMWARE_VERSION_MINOR << 8) | (FIRMWARE_VERSION_REVISION << 0);

const firmware_entry_func_t firmware_entry =  BOOTLOADER_FIRMWARE_ENTRY_FUNC;

BootloaderStatus bootloader_status;
BootloaderFunctions bootloader_functions;
int main(void) {
//	logging_init();

	bootloader_status.boot_mode = BOOT_MODE_FIRMWARE;
	bootloader_status.status_led_config = 1;
	bootloader_status.st.descriptor_section = tinydma_get_descriptor_section();
	bootloader_status.st.write_back_section = tinydma_get_write_back_section();

	firmware_entry(&bootloader_functions, &bootloader_status);

	while(true) {
		bootloader_functions.spitfp_tick(&bootloader_status);
//		puts("x\n\r");
	}

}
