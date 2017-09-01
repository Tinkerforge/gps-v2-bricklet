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

#include "minmea.h"
#include "bricklib2/utility/ringbuffer.h"
#include "bricklib2/utility/led_flicker.h"

#define FIREFLY_X1_PPS_BLINK_TIME 200 // Blink duration in ms
#define FIREFLY_X1_MAX_SENTENCE_LENGTH 512

#define FIREFLY_X1_RECV_BUFFER_SIZE 1024
#define FIREFLY_X1_SEND_BUFFER_SIZE 256

#define FIREFLY_X1_RECV_BUFFER_MASK (FIREFLY_X1_RECV_BUFFER_SIZE-1) // Use power of two size only here!

#define FIREFLY_X1_MAX_SAT_NUM 32

#define FIREFLY_X1_INTERRUPT_TIMEOUT 2000
#define FIREFLY_X1_DATA_TIMEOUT 10000
#define FIREFLY_X1_RESET_TIMEOUT 1000
#define FIREFLY_X1_TIME_BETWEEN_SENDS 100 // According to datasheet we need to wait 30ms between each message that we send

typedef struct {
    char mode;
    int fix_type;
    int sats[12];
    struct minmea_float pdop;
    struct minmea_float hdop;
    struct minmea_float vdop;

    struct minmea_sat_info sat_info[FIREFLY_X1_MAX_SAT_NUM];
} FireFlyX1DataSingle;

typedef struct {
    struct minmea_time time;
    bool valid;
    struct minmea_float latitude;
    struct minmea_float longitude;
    struct minmea_float speed; // knots
    struct minmea_float course;
    struct minmea_date date;
    struct minmea_float variation;
    int fix_quality;
    int satellites_tracked;
    struct minmea_float hdop;
    struct minmea_float altitude; char altitude_units;
    struct minmea_float height; char height_units;
    int dgps_age;
    struct minmea_float true_track_degrees;
    struct minmea_float magnetic_track_degrees;
    struct minmea_float speed_knots;
    struct minmea_float speed_kph;
    enum minmea_faa_mode faa_mode;
} FireFlyX1DataMixed;

typedef enum {
	FIREFLY_X1_RESTART_HOT     = 1,
	FIREFLY_X1_RESTART_WARM    = 2,
	FIREFLY_X1_RESTART_COLD    = 4,
	FIREFLY_X1_RESTART_FACTORY = 8,
} FireFlyX1Restart;

typedef enum {
	FIREFLY_X1_SBAS_5HZ      = 1,
	FIREFLY_X1_SBAS_ENABLE   = 2,
	FIREFLY_X1_SBAS_DISABLE  = 4,
	FIREFLY_X1_SBAS_10HZ     = 8,
	FIREFLY_X1_SBAS_BAUDRATE = 16,
} FireFlyX1SBAS;

typedef enum {
	FIREFLY_X1_STATE_WAIT_FOR_INTERRUPT,
	FIREFLY_X1_STATE_RECEIVE_IN_PROGRESS,
	FIREFLY_X1_STATE_WAIT_8MS,
	FIREFLY_X1_STATE_RESET,
} FireFlyX1State;

typedef struct {
	Ringbuffer ringbuffer_recv;
	char buffer_recv[FIREFLY_X1_RECV_BUFFER_SIZE];
	char buffer_send[FIREFLY_X1_SEND_BUFFER_SIZE];
	uint16_t buffer_recv_counter;
	uint16_t buffer_send_index;
	FireFlyX1State state;
	uint32_t wait_8ms_start_time;
	LEDFlickerState fix_led_state;
	uint8_t fix_led_config;

	uint32_t last_interrupt_time;
	uint32_t last_data_time;
	uint32_t reset_time;
	uint32_t last_send_time;

	uint8_t restart;
	uint8_t sbas;
	bool sbas_enabled;

	bool new_coordinates;
	bool new_status;
	bool new_altitude;
	bool new_motion;
	bool new_date_time;

	FireFlyX1DataMixed mixed;
	FireFlyX1DataSingle gps;
	FireFlyX1DataSingle glonass;
} FireFlyX1;

typedef enum {
	TALKER_MIXED,
	TALKER_GPS,
	TALKER_GLONASS
} FireFlyX1Talker;


void firefly_x1_update_sbas(FireFlyX1 *firefly_x1);
void firefly_x1_init(FireFlyX1 *firefly_x1);
void firefly_x1_tick(FireFlyX1 *firefly_x1);

#endif
