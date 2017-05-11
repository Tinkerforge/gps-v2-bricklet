/* gps-v2-bricklet
 * Copyright (C) 2016 Olaf Lüke <olaf@tinkerforge.com>
 *
 * communication.h: TFP protocol message handling
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

#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include <stdint.h>
#include <stdbool.h>

#include "bricklib2/bootloader/bootloader.h"

// Default functions
BootloaderHandleMessageResponse handle_message(const void *data, void *response);
void communication_tick(void);
void communication_init(void);

// Constants
#define GPS_V2_RESTART_TYPE_HOT_START 0
#define GPS_V2_RESTART_TYPE_WARM_START 1
#define GPS_V2_RESTART_TYPE_COLD_START 2
#define GPS_V2_RESTART_TYPE_FACTORY_RESET 3

#define GPS_V2_SATELLITE_SYSTEM_GPS 0
#define GPS_V2_SATELLITE_SYSTEM_GLONASS 1
#define GPS_V2_SATELLITE_SYSTEM_GALILEO 2

#define GPS_V2_FIX_NO_FIX 1
#define GPS_V2_FIX_2D_FIX 2
#define GPS_V2_FIX_3D_FIX 3

#define GPS_V2_FIX_LED_CONFIG_OFF 0
#define GPS_V2_FIX_LED_CONFIG_ON 1
#define GPS_V2_FIX_LED_CONFIG_SHOW_HEARTBEAT 2
#define GPS_V2_FIX_LED_CONFIG_SHOW_FIX 3
#define GPS_V2_FIX_LED_CONFIG_SHOW_PPS 4

// Function and callback IDs and structs
#define FID_GET_COORDINATES 1
#define FID_GET_STATUS 2
#define FID_GET_ALTITUDE 3
#define FID_GET_MOTION 4
#define FID_GET_DATE_TIME 5
#define FID_RESTART 6
#define FID_GET_SATELLITE_SYSTEM_STATUS_LOW_LEVEL 7
#define FID_GET_SATELLITE_STATUS 8
#define FID_SET_FIX_LED_CONFIG 9
#define FID_GET_FIX_LED_CONFIG 10
#define FID_SET_COORDINATES_CALLBACK_PERIOD 11
#define FID_GET_COORDINATES_CALLBACK_PERIOD 12
#define FID_SET_STATUS_CALLBACK_PERIOD 13
#define FID_GET_STATUS_CALLBACK_PERIOD 14
#define FID_SET_ALTITUDE_CALLBACK_PERIOD 15
#define FID_GET_ALTITUDE_CALLBACK_PERIOD 16
#define FID_SET_MOTION_CALLBACK_PERIOD 17
#define FID_GET_MOTION_CALLBACK_PERIOD 18
#define FID_SET_DATE_TIME_CALLBACK_PERIOD 19
#define FID_GET_DATE_TIME_CALLBACK_PERIOD 20

#define FID_CALLBACK_PULSE_PER_SECOND 21
#define FID_CALLBACK_COORDINATES 22
#define FID_CALLBACK_STATUS 23
#define FID_CALLBACK_ALTITUDE 24
#define FID_CALLBACK_MOTION 25
#define FID_CALLBACK_DATE_TIME 26

typedef struct {
	TFPMessageHeader header;
} __attribute__((__packed__)) GetCoordinates;

typedef struct {
	TFPMessageHeader header;
	uint32_t latitude;
	char ns;
	uint32_t longitude;
	char ew;
} __attribute__((__packed__)) GetCoordinates_Response;

typedef struct {
	TFPMessageHeader header;
} __attribute__((__packed__)) GetStatus;

typedef struct {
	TFPMessageHeader header;
	bool has_fix;
	uint8_t satellites_view;
} __attribute__((__packed__)) GetStatus_Response;

typedef struct {
	TFPMessageHeader header;
} __attribute__((__packed__)) GetAltitude;

typedef struct {
	TFPMessageHeader header;
	int32_t altitude;
	int32_t geoidal_separation;
} __attribute__((__packed__)) GetAltitude_Response;

typedef struct {
	TFPMessageHeader header;
} __attribute__((__packed__)) GetMotion;

typedef struct {
	TFPMessageHeader header;
	uint32_t course;
	uint32_t speed;
} __attribute__((__packed__)) GetMotion_Response;

typedef struct {
	TFPMessageHeader header;
} __attribute__((__packed__)) GetDateTime;

typedef struct {
	TFPMessageHeader header;
	uint32_t date;
	uint32_t time;
} __attribute__((__packed__)) GetDateTime_Response;

typedef struct {
	TFPMessageHeader header;
	uint8_t restart_type;
} __attribute__((__packed__)) Restart;

typedef struct {
	TFPMessageHeader header;
	uint8_t satellite_system;
} __attribute__((__packed__)) GetSatelliteSystemStatusLowLevel;

typedef struct {
	TFPMessageHeader header;
	uint8_t satellite_numbers_length;
	uint8_t satellite_numbers_data[12];
	uint8_t fix;
	uint16_t pdop;
	uint16_t hdop;
	uint16_t vdop;
} __attribute__((__packed__)) GetSatelliteSystemStatusLowLevel_Response;

typedef struct {
	TFPMessageHeader header;
	uint8_t satellite_system;
	uint8_t satellite_number;
} __attribute__((__packed__)) GetSatelliteStatus;

typedef struct {
	TFPMessageHeader header;
	int16_t elevation;
	int16_t azimuth;
	int16_t snr;
} __attribute__((__packed__)) GetSatelliteStatus_Response;

typedef struct {
	TFPMessageHeader header;
	uint8_t config;
} __attribute__((__packed__)) SetFixLEDConfig;

typedef struct {
	TFPMessageHeader header;
} __attribute__((__packed__)) GetFixLEDConfig;

typedef struct {
	TFPMessageHeader header;
	uint8_t config;
} __attribute__((__packed__)) GetFixLEDConfig_Response;

typedef struct {
	TFPMessageHeader header;
	uint32_t period;
} __attribute__((__packed__)) SetCoordinatesCallbackPeriod;

typedef struct {
	TFPMessageHeader header;
} __attribute__((__packed__)) GetCoordinatesCallbackPeriod;

typedef struct {
	TFPMessageHeader header;
	uint32_t period;
} __attribute__((__packed__)) GetCoordinatesCallbackPeriod_Response;

typedef struct {
	TFPMessageHeader header;
	uint32_t period;
} __attribute__((__packed__)) SetStatusCallbackPeriod;

typedef struct {
	TFPMessageHeader header;
} __attribute__((__packed__)) GetStatusCallbackPeriod;

typedef struct {
	TFPMessageHeader header;
	uint32_t period;
} __attribute__((__packed__)) GetStatusCallbackPeriod_Response;

typedef struct {
	TFPMessageHeader header;
	uint32_t period;
} __attribute__((__packed__)) SetAltitudeCallbackPeriod;

typedef struct {
	TFPMessageHeader header;
} __attribute__((__packed__)) GetAltitudeCallbackPeriod;

typedef struct {
	TFPMessageHeader header;
	uint32_t period;
} __attribute__((__packed__)) GetAltitudeCallbackPeriod_Response;

typedef struct {
	TFPMessageHeader header;
	uint32_t period;
} __attribute__((__packed__)) SetMotionCallbackPeriod;

typedef struct {
	TFPMessageHeader header;
} __attribute__((__packed__)) GetMotionCallbackPeriod;

typedef struct {
	TFPMessageHeader header;
	uint32_t period;
} __attribute__((__packed__)) GetMotionCallbackPeriod_Response;

typedef struct {
	TFPMessageHeader header;
	uint32_t period;
} __attribute__((__packed__)) SetDateTimeCallbackPeriod;

typedef struct {
	TFPMessageHeader header;
} __attribute__((__packed__)) GetDateTimeCallbackPeriod;

typedef struct {
	TFPMessageHeader header;
	uint32_t period;
} __attribute__((__packed__)) GetDateTimeCallbackPeriod_Response;

typedef struct {
	TFPMessageHeader header;
} __attribute__((__packed__)) PulsePerSecond_Callback;

typedef struct {
	TFPMessageHeader header;
	uint32_t latitude;
	char ns;
	uint32_t longitude;
	char ew;
} __attribute__((__packed__)) Coordinates_Callback;

typedef struct {
	TFPMessageHeader header;
	bool has_fix;
	uint8_t satellites_view;
} __attribute__((__packed__)) Status_Callback;

typedef struct {
	TFPMessageHeader header;
	int32_t altitude;
	int32_t geoidal_separation;
} __attribute__((__packed__)) Altitude_Callback;

typedef struct {
	TFPMessageHeader header;
	uint32_t course;
	uint32_t speed;
} __attribute__((__packed__)) Motion_Callback;

typedef struct {
	TFPMessageHeader header;
	uint32_t date;
	uint32_t time;
} __attribute__((__packed__)) DateTime_Callback;


// Function prototypes
BootloaderHandleMessageResponse get_coordinates(const GetCoordinates *data, GetCoordinates_Response *response);
BootloaderHandleMessageResponse get_status(const GetStatus *data, GetStatus_Response *response);
BootloaderHandleMessageResponse get_altitude(const GetAltitude *data, GetAltitude_Response *response);
BootloaderHandleMessageResponse get_motion(const GetMotion *data, GetMotion_Response *response);
BootloaderHandleMessageResponse get_date_time(const GetDateTime *data, GetDateTime_Response *response);
BootloaderHandleMessageResponse restart(const Restart *data);
BootloaderHandleMessageResponse get_satellite_system_status_low_level(const GetSatelliteSystemStatusLowLevel *data, GetSatelliteSystemStatusLowLevel_Response *response);
BootloaderHandleMessageResponse get_satellite_status(const GetSatelliteStatus *data, GetSatelliteStatus_Response *response);
BootloaderHandleMessageResponse set_fix_led_config(const SetFixLEDConfig *data);
BootloaderHandleMessageResponse get_fix_led_config(const GetFixLEDConfig *data, GetFixLEDConfig_Response *response);
BootloaderHandleMessageResponse set_coordinates_callback_period(const SetCoordinatesCallbackPeriod *data);
BootloaderHandleMessageResponse get_coordinates_callback_period(const GetCoordinatesCallbackPeriod *data, GetCoordinatesCallbackPeriod_Response *response);
BootloaderHandleMessageResponse set_status_callback_period(const SetStatusCallbackPeriod *data);
BootloaderHandleMessageResponse get_status_callback_period(const GetStatusCallbackPeriod *data, GetStatusCallbackPeriod_Response *response);
BootloaderHandleMessageResponse set_altitude_callback_period(const SetAltitudeCallbackPeriod *data);
BootloaderHandleMessageResponse get_altitude_callback_period(const GetAltitudeCallbackPeriod *data, GetAltitudeCallbackPeriod_Response *response);
BootloaderHandleMessageResponse set_motion_callback_period(const SetMotionCallbackPeriod *data);
BootloaderHandleMessageResponse get_motion_callback_period(const GetMotionCallbackPeriod *data, GetMotionCallbackPeriod_Response *response);
BootloaderHandleMessageResponse set_date_time_callback_period(const SetDateTimeCallbackPeriod *data);
BootloaderHandleMessageResponse get_date_time_callback_period(const GetDateTimeCallbackPeriod *data, GetDateTimeCallbackPeriod_Response *response);

// Callbacks
bool handle_pulse_per_second_callback(void);
bool handle_coordinates_callback(void);
bool handle_status_callback(void);
bool handle_altitude_callback(void);
bool handle_motion_callback(void);
bool handle_date_time_callback(void);

#define COMMUNICATION_CALLBACK_TICK_WAIT_MS 1
#define COMMUNICATION_CALLBACK_HANDLER_NUM 5
#define COMMUNICATION_CALLBACK_LIST_INIT \
	handle_coordinates_callback, \
	handle_status_callback, \
	handle_altitude_callback, \
	handle_motion_callback, \
	handle_date_time_callback, \


#endif
