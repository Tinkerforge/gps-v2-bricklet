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

BootloaderHandleMessageReturn handle_message(const void *data, void *return_message);


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
#define FID_COORDINATES 17
#define FID_STATUS 18
#define FID_ALTITUDE 19
#define FID_MOTION 20
#define FID_DATE_TIME 21


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
} __attribute__((__packed__)) GetCoordinatesReturn;

typedef struct {
	TFPMessageHeader header;
} __attribute__((__packed__)) GetStatus;

typedef struct {
	TFPMessageHeader header;
	uint8_t fix;
	uint8_t satellites_view;
	uint8_t satellites_used;
} __attribute__((__packed__)) GetStatusReturn;

typedef struct {
	TFPMessageHeader header;
} __attribute__((__packed__)) GetAltitude;

typedef struct {
	TFPMessageHeader header;
	uint32_t altitude;
	uint32_t geoidal_separation;
} __attribute__((__packed__)) GetAltitudeReturn;

typedef struct {
	TFPMessageHeader header;
} __attribute__((__packed__)) GetMotion;

typedef struct {
	TFPMessageHeader header;
	uint32_t course;
	uint32_t speed;
} __attribute__((__packed__)) GetMotionReturn;

typedef struct {
	TFPMessageHeader header;
} __attribute__((__packed__)) GetDateTime;

typedef struct {
	TFPMessageHeader header;
	uint32_t date;
	uint32_t time;
} __attribute__((__packed__)) GetDateTimeReturn;

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
} __attribute__((__packed__)) GetCoordinatesCallbackPeriodReturn;

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
} __attribute__((__packed__)) GetStatusCallbackPeriodReturn;

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
} __attribute__((__packed__)) GetAltitudeCallbackPeriodReturn;

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
} __attribute__((__packed__)) GetMotionCallbackPeriodReturn;

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
} __attribute__((__packed__)) GetDateTimeCallbackPeriodReturn;

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
} __attribute__((__packed__)) Coordinates;

typedef struct {
	TFPMessageHeader header;
	uint8_t fix;
	uint8_t satellites_view;
	uint8_t satellites_used;
} __attribute__((__packed__)) Status;

typedef struct {
	TFPMessageHeader header;
	uint32_t altitude;
	uint32_t geoidal_separation;
} __attribute__((__packed__)) Altitude;

typedef struct {
	TFPMessageHeader header;
	uint32_t course;
	uint32_t speed;
} __attribute__((__packed__)) Motion;

typedef struct {
	TFPMessageHeader header;
	uint32_t date;
	uint32_t time;
} __attribute__((__packed__)) DateTime;

typedef struct {
	TFPMessageHeader header;
} __attribute__((__packed__)) StandardMessage;

BootloaderHandleMessageReturn get_coordinates(const GetCoordinates *data, GetCoordinatesReturn *data_ret);
BootloaderHandleMessageReturn get_status(const GetStatus *data, GetStatusReturn *data_ret);
BootloaderHandleMessageReturn get_altitude(const GetAltitude *data, GetAltitudeReturn *data_ret);
BootloaderHandleMessageReturn get_motion(const GetMotion *data, GetMotionReturn *data_ret);
BootloaderHandleMessageReturn get_date_time(const GetDateTime *data, GetDateTimeReturn *data_ret);
BootloaderHandleMessageReturn restart(const Restart *data);
BootloaderHandleMessageReturn set_coordinates_callback_period(const SetCoordinatesCallbackPeriod *data);
BootloaderHandleMessageReturn get_coordinates_callback_period(const GetCoordinatesCallbackPeriod *data, GetCoordinatesCallbackPeriodReturn *data_ret);
BootloaderHandleMessageReturn set_status_callback_period(const SetStatusCallbackPeriod *data);
BootloaderHandleMessageReturn get_status_callback_period(const GetStatusCallbackPeriod *data, GetStatusCallbackPeriodReturn *data_ret);
BootloaderHandleMessageReturn set_altitude_callback_period(const SetAltitudeCallbackPeriod *data);
BootloaderHandleMessageReturn get_altitude_callback_period(const GetAltitudeCallbackPeriod *data, GetAltitudeCallbackPeriodReturn *data_ret);
BootloaderHandleMessageReturn set_motion_callback_period(const SetMotionCallbackPeriod *data);
BootloaderHandleMessageReturn get_motion_callback_period(const GetMotionCallbackPeriod *data, GetMotionCallbackPeriodReturn *data_ret);
BootloaderHandleMessageReturn set_date_time_callback_period(const SetDateTimeCallbackPeriod *data);
BootloaderHandleMessageReturn get_date_time_callback_period(const GetDateTimeCallbackPeriod *data, GetDateTimeCallbackPeriodReturn *data_ret);


#endif
