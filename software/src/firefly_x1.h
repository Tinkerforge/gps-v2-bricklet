/* gps-v2-bricklet
 * Copyright (C) 2016 Olaf Lüke <olaf@tinkerforge.com>
 *
 * firefly_x1.h: SPI communication with FireFly X1 GPS module
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

#ifndef FIREFLY_X1_H
#define FIREFLY_X1_H

#include <stdint.h>

#define FIREFLY_X1_RECV_BUFFER_SIZE 256
#define FIREFLY_X1_SEND_BUFFER_SIZE 256

typedef enum {
	FIREFLY_X1_STATE_WAIT_FOR_INTERRUPT,
	FIREFLY_X1_STATE_RECEIVE_IN_PROGRESS,
	FIREFLY_X1_STATE_WAIT_8MS,
	FIREFLY_X1_STATE_NEW_DATA_RECEIVED,
} FireFlyX1State;

typedef struct {
	char buffer_recv[FIREFLY_X1_RECV_BUFFER_SIZE];
	char buffer_send[FIREFLY_X1_SEND_BUFFER_SIZE];
	uint16_t buffer_recv_index;
	uint16_t buffer_send_index;
	FireFlyX1State state;
	uint32_t wait_8ms_start_time;
} FireFlyX1;


void firefly_x1_init(FireFlyX1 *firefly_x1);
void firefly_x1_tick(FireFlyX1 *firefly_x1);

#endif
