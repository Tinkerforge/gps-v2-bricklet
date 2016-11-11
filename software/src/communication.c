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

BootloaderHandleMessageReturn handle_message(const void *message, void *return_message) {
	switch(tfp_get_fid_from_message(message)) {
		case FID_GET_COORDINATES: return get_coordinates(message, return_message);
		case FID_GET_STATUS: return get_status(message, return_message);
		case FID_GET_ALTITUDE: return get_altitude(message, return_message);
		case FID_GET_MOTION: return get_motion(message, return_message);
		case FID_GET_DATE_TIME: return get_date_time(message, return_message);
		case FID_RESTART: return restart(message);
		case FID_SET_COORDINATES_CALLBACK_PERIOD: return set_coordinates_callback_period(message);
		case FID_GET_COORDINATES_CALLBACK_PERIOD: return get_coordinates_callback_period(message, return_message);
		case FID_SET_STATUS_CALLBACK_PERIOD: return set_status_callback_period(message);
		case FID_GET_STATUS_CALLBACK_PERIOD: return get_status_callback_period(message, return_message);
		case FID_SET_ALTITUDE_CALLBACK_PERIOD: return set_altitude_callback_period(message);
		case FID_GET_ALTITUDE_CALLBACK_PERIOD: return get_altitude_callback_period(message, return_message);
		case FID_SET_MOTION_CALLBACK_PERIOD: return set_motion_callback_period(message);
		case FID_GET_MOTION_CALLBACK_PERIOD: return get_motion_callback_period(message, return_message);
		case FID_SET_DATE_TIME_CALLBACK_PERIOD: return set_date_time_callback_period(message);
		case FID_GET_DATE_TIME_CALLBACK_PERIOD: return get_date_time_callback_period(message, return_message);
		default: return HANDLE_MESSAGE_RETURN_NOT_SUPPORTED;
	}
}


BootloaderHandleMessageReturn get_coordinates(const GetCoordinates *data, GetCoordinatesReturn *data_ret) {
	data_ret->header.length = sizeof(GetCoordinatesReturn);

	return HANDLE_MESSAGE_RETURN_NEW_MESSAGE;
}

BootloaderHandleMessageReturn get_status(const GetStatus *data, GetStatusReturn *data_ret) {
	data_ret->header.length = sizeof(GetStatusReturn);

	return HANDLE_MESSAGE_RETURN_NEW_MESSAGE;
}

BootloaderHandleMessageReturn get_altitude(const GetAltitude *data, GetAltitudeReturn *data_ret) {
	data_ret->header.length = sizeof(GetAltitudeReturn);

	return HANDLE_MESSAGE_RETURN_NEW_MESSAGE;
}

BootloaderHandleMessageReturn get_motion(const GetMotion *data, GetMotionReturn *data_ret) {
	data_ret->header.length = sizeof(GetMotionReturn);

	return HANDLE_MESSAGE_RETURN_NEW_MESSAGE;
}

BootloaderHandleMessageReturn get_date_time(const GetDateTime *data, GetDateTimeReturn *data_ret) {
	data_ret->header.length = sizeof(GetDateTimeReturn);

	return HANDLE_MESSAGE_RETURN_NEW_MESSAGE;
}

BootloaderHandleMessageReturn restart(const Restart *data) {

	return HANDLE_MESSAGE_RETURN_EMPTY;
}

BootloaderHandleMessageReturn set_coordinates_callback_period(const SetCoordinatesCallbackPeriod *data) {

	return HANDLE_MESSAGE_RETURN_EMPTY;
}

BootloaderHandleMessageReturn get_coordinates_callback_period(const GetCoordinatesCallbackPeriod *data, GetCoordinatesCallbackPeriodReturn *data_ret) {
	data_ret->header.length = sizeof(GetCoordinatesCallbackPeriodReturn);

	return HANDLE_MESSAGE_RETURN_NEW_MESSAGE;
}

BootloaderHandleMessageReturn set_status_callback_period(const SetStatusCallbackPeriod *data) {

	return HANDLE_MESSAGE_RETURN_EMPTY;
}

BootloaderHandleMessageReturn get_status_callback_period(const GetStatusCallbackPeriod *data, GetStatusCallbackPeriodReturn *data_ret) {
	data_ret->header.length = sizeof(GetStatusCallbackPeriodReturn);

	return HANDLE_MESSAGE_RETURN_NEW_MESSAGE;
}

BootloaderHandleMessageReturn set_altitude_callback_period(const SetAltitudeCallbackPeriod *data) {

	return HANDLE_MESSAGE_RETURN_EMPTY;
}

BootloaderHandleMessageReturn get_altitude_callback_period(const GetAltitudeCallbackPeriod *data, GetAltitudeCallbackPeriodReturn *data_ret) {
	data_ret->header.length = sizeof(GetAltitudeCallbackPeriodReturn);

	return HANDLE_MESSAGE_RETURN_NEW_MESSAGE;
}

BootloaderHandleMessageReturn set_motion_callback_period(const SetMotionCallbackPeriod *data) {

	return HANDLE_MESSAGE_RETURN_EMPTY;
}

BootloaderHandleMessageReturn get_motion_callback_period(const GetMotionCallbackPeriod *data, GetMotionCallbackPeriodReturn *data_ret) {
	data_ret->header.length = sizeof(GetMotionCallbackPeriodReturn);

	return HANDLE_MESSAGE_RETURN_NEW_MESSAGE;
}

BootloaderHandleMessageReturn set_date_time_callback_period(const SetDateTimeCallbackPeriod *data) {

	return HANDLE_MESSAGE_RETURN_EMPTY;
}

BootloaderHandleMessageReturn get_date_time_callback_period(const GetDateTimeCallbackPeriod *data, GetDateTimeCallbackPeriodReturn *data_ret) {
	data_ret->header.length = sizeof(GetDateTimeCallbackPeriodReturn);

	return HANDLE_MESSAGE_RETURN_NEW_MESSAGE;
}
