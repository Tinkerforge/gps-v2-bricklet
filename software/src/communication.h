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

#include "bricklib2/bootloader/bootloader.h"

BootloaderHandleMessageResponse handle_message(const void *data, void *response);


#define FID_GET_COORDINATES 1
#define FID_GET_STATUS 2
#define FID_GET_ALTITUDE 3
#define FID_GET_MOTION 4
#define FID_GET_DATE_TIME 5
#define FID_RESTART 6
#define FID_SET_COORDINATES_CALLBACK_PERIOD 7
#define FID_GET_COORDINATES_CALLBACK_PERIOD 8
#define FID_SET_STATUS_CALLBACK_PERIOD 9
#define FID_GET_STATUS_CALLBACK_PERIOD 10
#define FID_SET_ALTITUDE_CALLBACK_PERIOD 11
#define FID_GET_ALTITUDE_CALLBACK_PERIOD 12
#define FID_SET_MOTION_CALLBACK_PERIOD 13
#define FID_GET_MOTION_CALLBACK_PERIOD 14
#define FID_SET_DATE_TIME_CALLBACK_PERIOD 15
#define FID_GET_DATE_TIME_CALLBACK_PERIOD 16

#define FID_CALLBACK_COORDINATES 17
#define FID_CALLBACK_STATUS 18
#define FID_CALLBACK_ALTITUDE 19
#define FID_CALLBACK_MOTION 20
#define FID_CALLBACK_DATE_TIME 21


typedef struct {
	TFPMessageHeader header;
} __attribute__((__packed__)) GetCoordinates;

typedef struct {
	TFPMessageHeader header;
	uint32_t latitude;
	char ns;
	uint32_t longitude;
	char ew;
	uint16_t pdop;
	uint16_t hdop;
	uint16_t vdop;
	uint16_t epe;
} __attribute__((__packed__)) GetCoordinatesResponse;

typedef struct {
	TFPMessageHeader header;
} __attribute__((__packed__)) GetStatus;

typedef struct {
	TFPMessageHeader header;
	uint8_t fix;
	uint8_t satellites_view;
	uint8_t satellites_used;
} __attribute__((__packed__)) GetStatusResponse;

typedef struct {
	TFPMessageHeader header;
} __attribute__((__packed__)) GetAltitude;

typedef struct {
	TFPMessageHeader header;
	int32_t altitude;
	int32_t geoidal_separation;
} __attribute__((__packed__)) GetAltitudeResponse;

typedef struct {
	TFPMessageHeader header;
} __attribute__((__packed__)) GetMotion;

typedef struct {
	TFPMessageHeader header;
	uint32_t course;
	uint32_t speed;
} __attribute__((__packed__)) GetMotionResponse;

typedef struct {
	TFPMessageHeader header;
} __attribute__((__packed__)) GetDateTime;

typedef struct {
	TFPMessageHeader header;
	uint32_t date;
	uint32_t time;
} __attribute__((__packed__)) GetDateTimeResponse;

typedef struct {
	TFPMessageHeader header;
	uint8_t restart_type;
} __attribute__((__packed__)) Restart;

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
} __attribute__((__packed__)) GetCoordinatesCallbackPeriodResponse;

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
} __attribute__((__packed__)) GetStatusCallbackPeriodResponse;

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
} __attribute__((__packed__)) GetAltitudeCallbackPeriodResponse;

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
} __attribute__((__packed__)) GetMotionCallbackPeriodResponse;

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
} __attribute__((__packed__)) GetDateTimeCallbackPeriodResponse;

typedef struct {
	TFPMessageHeader header;
	uint32_t latitude;
	char ns;
	uint32_t longitude;
	char ew;
	uint16_t pdop;
	uint16_t hdop;
	uint16_t vdop;
	uint16_t epe;
} __attribute__((__packed__)) CoordinatesCallback;

typedef struct {
	TFPMessageHeader header;
	uint8_t fix;
	uint8_t satellites_view;
	uint8_t satellites_used;
} __attribute__((__packed__)) StatusCallback;

typedef struct {
	TFPMessageHeader header;
	int32_t altitude;
	int32_t geoidal_separation;
} __attribute__((__packed__)) AltitudeCallback;

typedef struct {
	TFPMessageHeader header;
	uint32_t course;
	uint32_t speed;
} __attribute__((__packed__)) MotionCallback;

typedef struct {
	TFPMessageHeader header;
	uint32_t date;
	uint32_t time;
} __attribute__((__packed__)) DateTimeCallback;



BootloaderHandleMessageResponse get_coordinates(const GetCoordinates *data, GetCoordinatesResponse *response);
BootloaderHandleMessageResponse get_status(const GetStatus *data, GetStatusResponse *response);
BootloaderHandleMessageResponse get_altitude(const GetAltitude *data, GetAltitudeResponse *response);
BootloaderHandleMessageResponse get_motion(const GetMotion *data, GetMotionResponse *response);
BootloaderHandleMessageResponse get_date_time(const GetDateTime *data, GetDateTimeResponse *response);
BootloaderHandleMessageResponse restart(const Restart *data);
BootloaderHandleMessageResponse set_coordinates_callback_period(const SetCoordinatesCallbackPeriod *data);
BootloaderHandleMessageResponse get_coordinates_callback_period(const GetCoordinatesCallbackPeriod *data, GetCoordinatesCallbackPeriodResponse *response);
BootloaderHandleMessageResponse set_status_callback_period(const SetStatusCallbackPeriod *data);
BootloaderHandleMessageResponse get_status_callback_period(const GetStatusCallbackPeriod *data, GetStatusCallbackPeriodResponse *response);
BootloaderHandleMessageResponse set_altitude_callback_period(const SetAltitudeCallbackPeriod *data);
BootloaderHandleMessageResponse get_altitude_callback_period(const GetAltitudeCallbackPeriod *data, GetAltitudeCallbackPeriodResponse *response);
BootloaderHandleMessageResponse set_motion_callback_period(const SetMotionCallbackPeriod *data);
BootloaderHandleMessageResponse get_motion_callback_period(const GetMotionCallbackPeriod *data, GetMotionCallbackPeriodResponse *response);
BootloaderHandleMessageResponse set_date_time_callback_period(const SetDateTimeCallbackPeriod *data);
BootloaderHandleMessageResponse get_date_time_callback_period(const GetDateTimeCallbackPeriod *data, GetDateTimeCallbackPeriodResponse *response);


#endif
