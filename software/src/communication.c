/* gps-v2-bricklet
 * Copyright (C) 2016 Olaf Lüke <olaf@tinkerforge.com>
 *
 * communication.c: TFP protocol message handling
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

#include "communication.h"

#include "bricklib2/protocols/tfp/tfp.h"

BootloaderHandleMessageResponse handle_message(const void *message, void *response) {
	switch(tfp_get_fid_from_message(message)) {
		case FID_GET_COORDINATES: return get_coordinates(message, response);
		case FID_GET_STATUS: return get_status(message, response);
		case FID_GET_ALTITUDE: return get_altitude(message, response);
		case FID_GET_MOTION: return get_motion(message, response);
		case FID_GET_DATE_TIME: return get_date_time(message, response);
		case FID_RESTART: return restart(message);
		case FID_SET_COORDINATES_CALLBACK_PERIOD: return set_coordinates_callback_period(message);
		case FID_GET_COORDINATES_CALLBACK_PERIOD: return get_coordinates_callback_period(message, response);
		case FID_SET_STATUS_CALLBACK_PERIOD: return set_status_callback_period(message);
		case FID_GET_STATUS_CALLBACK_PERIOD: return get_status_callback_period(message, response);
		case FID_SET_ALTITUDE_CALLBACK_PERIOD: return set_altitude_callback_period(message);
		case FID_GET_ALTITUDE_CALLBACK_PERIOD: return get_altitude_callback_period(message, response);
		case FID_SET_MOTION_CALLBACK_PERIOD: return set_motion_callback_period(message);
		case FID_GET_MOTION_CALLBACK_PERIOD: return get_motion_callback_period(message, response);
		case FID_SET_DATE_TIME_CALLBACK_PERIOD: return set_date_time_callback_period(message);
		case FID_GET_DATE_TIME_CALLBACK_PERIOD: return get_date_time_callback_period(message, response);
		default: return HANDLE_MESSAGE_RESPONSE_NOT_SUPPORTED;
	}
}


BootloaderHandleMessageResponse get_coordinates(const GetCoordinates *data, GetCoordinatesResponse *response) {
	response->header.length = sizeof(GetCoordinatesResponse);

	return HANDLE_MESSAGE_RESPONSE_NEW_MESSAGE;
}

BootloaderHandleMessageResponse get_status(const GetStatus *data, GetStatusResponse *response) {
	response->header.length = sizeof(GetStatusResponse);

	return HANDLE_MESSAGE_RESPONSE_NEW_MESSAGE;
}

BootloaderHandleMessageResponse get_altitude(const GetAltitude *data, GetAltitudeResponse *response) {
	response->header.length = sizeof(GetAltitudeResponse);

	return HANDLE_MESSAGE_RESPONSE_NEW_MESSAGE;
}

BootloaderHandleMessageResponse get_motion(const GetMotion *data, GetMotionResponse *response) {
	response->header.length = sizeof(GetMotionResponse);

	return HANDLE_MESSAGE_RESPONSE_NEW_MESSAGE;
}

BootloaderHandleMessageResponse get_date_time(const GetDateTime *data, GetDateTimeResponse *response) {
	response->header.length = sizeof(GetDateTimeResponse);

	return HANDLE_MESSAGE_RESPONSE_NEW_MESSAGE;
}

BootloaderHandleMessageResponse restart(const Restart *data) {

	return HANDLE_MESSAGE_RESPONSE_EMPTY;
}

BootloaderHandleMessageResponse set_coordinates_callback_period(const SetCoordinatesCallbackPeriod *data) {

	return HANDLE_MESSAGE_RESPONSE_EMPTY;
}

BootloaderHandleMessageResponse get_coordinates_callback_period(const GetCoordinatesCallbackPeriod *data, GetCoordinatesCallbackPeriodResponse *response) {
	response->header.length = sizeof(GetCoordinatesCallbackPeriodResponse);

	return HANDLE_MESSAGE_RESPONSE_NEW_MESSAGE;
}

BootloaderHandleMessageResponse set_status_callback_period(const SetStatusCallbackPeriod *data) {

	return HANDLE_MESSAGE_RESPONSE_EMPTY;
}

BootloaderHandleMessageResponse get_status_callback_period(const GetStatusCallbackPeriod *data, GetStatusCallbackPeriodResponse *response) {
	response->header.length = sizeof(GetStatusCallbackPeriodResponse);

	return HANDLE_MESSAGE_RESPONSE_NEW_MESSAGE;
}

BootloaderHandleMessageResponse set_altitude_callback_period(const SetAltitudeCallbackPeriod *data) {

	return HANDLE_MESSAGE_RESPONSE_EMPTY;
}

BootloaderHandleMessageResponse get_altitude_callback_period(const GetAltitudeCallbackPeriod *data, GetAltitudeCallbackPeriodResponse *response) {
	response->header.length = sizeof(GetAltitudeCallbackPeriodResponse);

	return HANDLE_MESSAGE_RESPONSE_NEW_MESSAGE;
}

BootloaderHandleMessageResponse set_motion_callback_period(const SetMotionCallbackPeriod *data) {

	return HANDLE_MESSAGE_RESPONSE_EMPTY;
}

BootloaderHandleMessageResponse get_motion_callback_period(const GetMotionCallbackPeriod *data, GetMotionCallbackPeriodResponse *response) {
	response->header.length = sizeof(GetMotionCallbackPeriodResponse);

	return HANDLE_MESSAGE_RESPONSE_NEW_MESSAGE;
}

BootloaderHandleMessageResponse set_date_time_callback_period(const SetDateTimeCallbackPeriod *data) {

	return HANDLE_MESSAGE_RESPONSE_EMPTY;
}

BootloaderHandleMessageResponse get_date_time_callback_period(const GetDateTimeCallbackPeriod *data, GetDateTimeCallbackPeriodResponse *response) {
	response->header.length = sizeof(GetDateTimeCallbackPeriodResponse);

	return HANDLE_MESSAGE_RESPONSE_NEW_MESSAGE;
}

