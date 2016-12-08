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

#include "bricklib2/hal/uartbb/uartbb.h"
#include "bricklib2/hal/system_timer/system_timer.h"
#include "bricklib2/utility/communication_callback.h"
#include "bricklib2/protocols/tfp/tfp.h"

#include "configs/config_firefly_x1.h"
#include "firefly_x1.h"

static uint32_t callback_period_coordinates = 0;
static uint32_t callback_period_status = 0;
static uint32_t callback_period_altitude = 0;
static uint32_t callback_period_motion = 0;
static uint32_t callback_period_date_time = 0;

extern FireFlyX1 firefly_x1;

BootloaderHandleMessageResponse handle_message(const void *message, void *response) {
	switch(tfp_get_fid_from_message(message)) {
		case FID_GET_COORDINATES: return get_coordinates(message, response);
		case FID_GET_STATUS: return get_status(message, response);
		case FID_GET_ALTITUDE: return get_altitude(message, response);
		case FID_GET_MOTION: return get_motion(message, response);
		case FID_GET_DATE_TIME: return get_date_time(message, response);
		case FID_RESTART: return restart(message);
		case FID_GET_SATELLITE_SYSTEM_STATUS: return get_satellite_system_status(message, response);
		case FID_GET_SATELLITE_STATUS: return get_satellite_status(message, response);
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

	uint32_t ulat  = firefly_x1.mixed.latitude.value  > 0 ? firefly_x1.mixed.latitude.value  : -firefly_x1.mixed.latitude.value;
	uint32_t ulong = firefly_x1.mixed.longitude.value > 0 ? firefly_x1.mixed.longitude.value : -firefly_x1.mixed.longitude.value;

	// Convert from DD.MMMMMM to DD.DDDDDD
	ulat  *= (10000/firefly_x1.mixed.latitude.scale);
	ulong *= (10000/firefly_x1.mixed.longitude.scale);

	uint32_t ulat_mins  = ulat  % 1000000;
	uint32_t ulong_mins = ulong % 1000000;
	ulat  -= ulat_mins;
	ulong -= ulong_mins;

	ulat  += ulat_mins*100  / 60;
	ulong += ulong_mins*100 / 60;

	response->latitude  = ulat;
	response->ns        = firefly_x1.mixed.latitude.value > 0 ? 'N' : 'S';
	response->longitude = ulong;
	response->ew        = firefly_x1.mixed.longitude.value > 0 ? 'E' : 'W';

	return HANDLE_MESSAGE_RESPONSE_NEW_MESSAGE;
}

BootloaderHandleMessageResponse get_status(const GetStatus *data, GetStatusResponse *response) {
	response->header.length = sizeof(GetStatusResponse);

	response->has_fix         = firefly_x1.mixed.fix_quality != 0;
	response->satellites_view = firefly_x1.mixed.satellites_tracked;

	return HANDLE_MESSAGE_RESPONSE_NEW_MESSAGE;
}

BootloaderHandleMessageResponse get_altitude(const GetAltitude *data, GetAltitudeResponse *response) {
	response->header.length = sizeof(GetAltitudeResponse);
	response->altitude           = firefly_x1.mixed.altitude.value * (10/firefly_x1.mixed.altitude.scale);
	response->geoidal_separation = firefly_x1.mixed.height.value   * (10/firefly_x1.mixed.height.scale);

	return HANDLE_MESSAGE_RESPONSE_NEW_MESSAGE;
}

BootloaderHandleMessageResponse get_motion(const GetMotion *data, GetMotionResponse *response) {
	response->header.length = sizeof(GetMotionResponse);
	response->course = firefly_x1.mixed.course.value * (100/firefly_x1.mixed.course.scale);
	response->speed  = firefly_x1.mixed.speed.value  * (100/firefly_x1.mixed.speed.scale);

	return HANDLE_MESSAGE_RESPONSE_NEW_MESSAGE;
}

BootloaderHandleMessageResponse get_date_time(const GetDateTime *data, GetDateTimeResponse *response) {
	response->header.length = sizeof(GetDateTimeResponse);
	response->date = firefly_x1.mixed.date.day*100*100 + firefly_x1.mixed.date.month*100 + firefly_x1.mixed.date.year;
	response->time = firefly_x1.mixed.time.hours*1000*100*100 + firefly_x1.mixed.time.minutes*1000*100 + firefly_x1.mixed.time.seconds*1000 + firefly_x1.mixed.time.microseconds;

	return HANDLE_MESSAGE_RESPONSE_NEW_MESSAGE;
}

BootloaderHandleMessageResponse restart(const Restart *data) {
	switch(data->restart_type) {
		case GPS_V2_RESTART_TYPE_HOT_START:     firefly_x1.restart |= FIREFLY_X1_RESTART_HOT;     break;
		case GPS_V2_RESTART_TYPE_WARM_START:    firefly_x1.restart |= FIREFLY_X1_RESTART_WARM;    break;
		case GPS_V2_RESTART_TYPE_COLD_START:    firefly_x1.restart |= FIREFLY_X1_RESTART_COLD;    break;
		case GPS_V2_RESTART_TYPE_FACTORY_RESET: firefly_x1.restart |= FIREFLY_X1_RESTART_FACTORY; break;
		default: return HANDLE_MESSAGE_RESPONSE_INVALID_PARAMETER;
	}

	return HANDLE_MESSAGE_RESPONSE_EMPTY;
}

BootloaderHandleMessageResponse get_satellite_system_status(const GetSatelliteSystemStatus *data, GetSatelliteSystemStatusResponse *response) {
	response->header.length = sizeof(GetSatelliteSystemStatusResponse);

	FireFlyX1DataSingle *single;
	if(data->satellite_system == GPS_V2_SATELLITE_SYSTEM_GPS) {
		single = &firefly_x1.gps;
	} else if(data->satellite_system == GPS_V2_SATELLITE_SYSTEM_GLONASS) {
		single = &firefly_x1.glonass;
	} else {
		return HANDLE_MESSAGE_RESPONSE_INVALID_PARAMETER;
	}

	for(uint8_t i = 0; i < 12; i++) {
		response->satellites[i] = single->sats[i];
	}

	response->fix  = single->fix_type;
	response->hdop = single->hdop.value * (100/single->hdop.scale);
	response->vdop = single->vdop.value * (100/single->hdop.scale);
	response->pdop = single->pdop.value * (100/single->hdop.scale);

	return HANDLE_MESSAGE_RESPONSE_NEW_MESSAGE;
}

BootloaderHandleMessageResponse get_satellite_status(const GetSatelliteStatus *data, GetSatelliteStatusResponse *response) {
	response->header.length = sizeof(GetSatelliteStatusResponse);

	FireFlyX1DataSingle *single;
	if(data->satellite_system == GPS_V2_SATELLITE_SYSTEM_GPS) {
		single = &firefly_x1.gps;
	} else if(data->satellite_system == GPS_V2_SATELLITE_SYSTEM_GLONASS) {
		single = &firefly_x1.glonass;
	} else {
		return HANDLE_MESSAGE_RESPONSE_INVALID_PARAMETER;
	}

	if(data->satellite_number < 1 || data->satellite_number > 32) {
		return HANDLE_MESSAGE_RESPONSE_INVALID_PARAMETER;
	}

	response->azimuth   = single->sat_info[data->satellite_number-1].azimuth;
	response->elevation = single->sat_info[data->satellite_number-1].elevation;
	response->snr       = single->sat_info[data->satellite_number-1].snr;

	return HANDLE_MESSAGE_RESPONSE_NEW_MESSAGE;
}

BootloaderHandleMessageResponse set_coordinates_callback_period(const SetCoordinatesCallbackPeriod *data) {
	callback_period_coordinates = data->period;

	uartbb_puts("set_coordinates_callback_period: ");
	uartbb_puti(callback_period_coordinates);
	uartbb_putnl();

	return HANDLE_MESSAGE_RESPONSE_EMPTY;
}

BootloaderHandleMessageResponse get_coordinates_callback_period(const GetCoordinatesCallbackPeriod *data, GetCoordinatesCallbackPeriodResponse *response) {
	response->header.length = sizeof(GetCoordinatesCallbackPeriodResponse);
	response->period = callback_period_coordinates;

	return HANDLE_MESSAGE_RESPONSE_NEW_MESSAGE;
}

BootloaderHandleMessageResponse set_status_callback_period(const SetStatusCallbackPeriod *data) {
	callback_period_status = data->period;

	return HANDLE_MESSAGE_RESPONSE_EMPTY;
}

BootloaderHandleMessageResponse get_status_callback_period(const GetStatusCallbackPeriod *data, GetStatusCallbackPeriodResponse *response) {
	response->header.length = sizeof(GetStatusCallbackPeriodResponse);
	response->period = callback_period_status;

	return HANDLE_MESSAGE_RESPONSE_NEW_MESSAGE;
}

BootloaderHandleMessageResponse set_altitude_callback_period(const SetAltitudeCallbackPeriod *data) {
	callback_period_altitude = data->period;

	return HANDLE_MESSAGE_RESPONSE_EMPTY;
}

BootloaderHandleMessageResponse get_altitude_callback_period(const GetAltitudeCallbackPeriod *data, GetAltitudeCallbackPeriodResponse *response) {
	response->header.length = sizeof(GetAltitudeCallbackPeriodResponse);
	response->period = callback_period_altitude;

	return HANDLE_MESSAGE_RESPONSE_NEW_MESSAGE;
}

BootloaderHandleMessageResponse set_motion_callback_period(const SetMotionCallbackPeriod *data) {
	callback_period_motion = data->period;

	return HANDLE_MESSAGE_RESPONSE_EMPTY;
}

BootloaderHandleMessageResponse get_motion_callback_period(const GetMotionCallbackPeriod *data, GetMotionCallbackPeriodResponse *response) {
	response->header.length = sizeof(GetMotionCallbackPeriodResponse);
	response->period = callback_period_motion;

	return HANDLE_MESSAGE_RESPONSE_NEW_MESSAGE;
}

BootloaderHandleMessageResponse set_date_time_callback_period(const SetDateTimeCallbackPeriod *data) {
	callback_period_date_time = data->period;

	return HANDLE_MESSAGE_RESPONSE_EMPTY;
}

BootloaderHandleMessageResponse get_date_time_callback_period(const GetDateTimeCallbackPeriod *data, GetDateTimeCallbackPeriodResponse *response) {
	response->header.length = sizeof(GetDateTimeCallbackPeriodResponse);
	response->period = callback_period_date_time;

	return HANDLE_MESSAGE_RESPONSE_NEW_MESSAGE;
}


bool handle_pulse_per_second_callback(void) {
	static bool last_pps_high = false;
	static bool is_buffered = false;
	static PulsePerSecondCallback cb;

	if(!is_buffered) {
		if(XMC_GPIO_GetInput(FIREFLY_X1_PPS_PIN)) {
			if(!last_pps_high) {
				tfp_make_default_header(&cb.header, bootloader_get_uid(), sizeof(PulsePerSecondCallback), FID_CALLBACK_PULSE_PER_SECOND);
				last_pps_high = true;
			} else {
				return false;
			}
		} else {
			last_pps_high = false;
			return false;
		}
	}

	if(bootloader_spitfp_is_send_possible(&bootloader_status.st)) {
		bootloader_spitfp_send_ack_and_message(&bootloader_status, (uint8_t*)&cb, sizeof(PulsePerSecondCallback));
		is_buffered = false;
		return true;
	} else {
		is_buffered = true;
	}

	return false;
}

bool handle_coordinates_callback(void) {
	static bool is_buffered = false;
	static CoordinatesCallback cb;
	static uint32_t last_callback = 0;

	if(!is_buffered) {
		if(callback_period_coordinates != 0 && firefly_x1.new_coordinates && system_timer_is_time_elapsed_ms(last_callback, callback_period_coordinates)) {
			firefly_x1.new_coordinates = false;
			last_callback = system_timer_get_ms();
			tfp_make_default_header(&cb.header, bootloader_get_uid(), sizeof(CoordinatesCallback), FID_CALLBACK_COORDINATES);
			get_coordinates(NULL, (GetCoordinatesResponse *) &cb);
		} else {
			return false;
		}
	}

	if(bootloader_spitfp_is_send_possible(&bootloader_status.st)) {
		bootloader_spitfp_send_ack_and_message(&bootloader_status, (uint8_t*)&cb, sizeof(CoordinatesCallback));
		is_buffered = false;
		return true;
	} else {
		is_buffered = true;
	}

	return false;
}

bool handle_status_callback(void) {
	static bool is_buffered = false;
	static StatusCallback cb;
	static uint32_t last_callback = 0;

	if(!is_buffered) {
		if(callback_period_status != 0 && firefly_x1.new_status && system_timer_is_time_elapsed_ms(last_callback, callback_period_status)) {
			firefly_x1.new_status = false;
			last_callback = system_timer_get_ms();
			tfp_make_default_header(&cb.header, bootloader_get_uid(), sizeof(StatusCallback), FID_CALLBACK_STATUS);
			get_status(NULL, (GetStatusResponse *) &cb);
		} else {
			return false;
		}
	}

	if(bootloader_spitfp_is_send_possible(&bootloader_status.st)) {
		bootloader_spitfp_send_ack_and_message(&bootloader_status, (uint8_t*)&cb, sizeof(StatusCallback));
		is_buffered = false;
		return true;
	} else {
		is_buffered = true;
	}

	return false;
}

bool handle_altitude_callback(void) {
	static bool is_buffered = false;
	static AltitudeCallback cb;
	static uint32_t last_callback = 0;

	if(!is_buffered) {
		if(callback_period_altitude != 0 && firefly_x1.new_altitude && system_timer_is_time_elapsed_ms(last_callback, callback_period_altitude)) {
			firefly_x1.new_altitude = false;
			last_callback = system_timer_get_ms();
			tfp_make_default_header(&cb.header, bootloader_get_uid(), sizeof(AltitudeCallback), FID_CALLBACK_ALTITUDE);
			get_altitude(NULL, (GetAltitudeResponse *) &cb);
		} else {
			return false;
		}
	}

	if(bootloader_spitfp_is_send_possible(&bootloader_status.st)) {
		bootloader_spitfp_send_ack_and_message(&bootloader_status, (uint8_t*)&cb, sizeof(AltitudeCallback));
		is_buffered = false;
		return true;
	} else {
		is_buffered = true;
	}

	return false;
}

bool handle_motion_callback(void) {
	static bool is_buffered = false;
	static MotionCallback cb;
	static uint32_t last_callback = 0;

	if(!is_buffered) {
		if(callback_period_motion != 0 && firefly_x1.new_motion && system_timer_is_time_elapsed_ms(last_callback, callback_period_motion)) {
			firefly_x1.new_motion = false;
			last_callback = system_timer_get_ms();
			tfp_make_default_header(&cb.header, bootloader_get_uid(), sizeof(MotionCallback), FID_CALLBACK_MOTION);
			get_motion(NULL, (GetMotionResponse *) &cb);
		} else {
			return false;
		}
	}

	if(bootloader_spitfp_is_send_possible(&bootloader_status.st)) {
		bootloader_spitfp_send_ack_and_message(&bootloader_status, (uint8_t*)&cb, sizeof(MotionCallback));
		is_buffered = false;
		return true;
	} else {
		is_buffered = true;
	}

	return false;
}

bool handle_date_time_callback(void) {
	static bool is_buffered = false;
	static DateTimeCallback cb;
	static uint32_t last_callback = 0;

	if(!is_buffered) {
		if(callback_period_date_time != 0 && firefly_x1.new_date_time && system_timer_is_time_elapsed_ms(last_callback, callback_period_date_time)) {
			firefly_x1.new_date_time = false;
			last_callback = system_timer_get_ms();
			tfp_make_default_header(&cb.header, bootloader_get_uid(), sizeof(DateTimeCallback), FID_CALLBACK_DATE_TIME);
			get_date_time(NULL, (GetDateTimeResponse *) &cb);
		} else {
			return false;
		}
	}

	if(bootloader_spitfp_is_send_possible(&bootloader_status.st)) {
		bootloader_spitfp_send_ack_and_message(&bootloader_status, (uint8_t*)&cb, sizeof(DateTimeCallback));
		is_buffered = false;
		return true;
	} else {
		is_buffered = true;
	}

	return false;
}

void communication_tick(void) {
/*	handle_pulse_per_second_callback();
	handle_coordinates_callback();
	handle_status_callback();
	handle_motion_callback();
	handle_altitude_callback();
	handle_date_time_callback();*/
	communication_callback_tick();
}

void communication_init(void) {
	communication_callback_init();
}
