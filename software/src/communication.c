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
		case FID_GET_SATELLITE_SYSTEM_STATUS_LOW_LEVEL: return get_satellite_system_status_low_level(message, response);
		case FID_GET_SATELLITE_STATUS: return get_satellite_status(message, response);
		case FID_SET_FIX_LED_CONFIG: return set_fix_led_config(message);
		case FID_GET_FIX_LED_CONFIG: return get_fix_led_config(message, response);
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
		case FID_SET_SBAS_CONFIG: return set_sbas_config(message);
		case FID_GET_SBAS_CONFIG: return get_sbas_config(message, response);
		default: return HANDLE_MESSAGE_RESPONSE_NOT_SUPPORTED;
	}
}

BootloaderHandleMessageResponse get_coordinates(const GetCoordinates *data, GetCoordinates_Response *response) {
	response->header.length = sizeof(GetCoordinates_Response);

	uint32_t ulat = firefly_x1.mixed.latitude.value  > 0 ? firefly_x1.mixed.latitude.value  : -firefly_x1.mixed.latitude.value;
	uint32_t ulon = firefly_x1.mixed.longitude.value > 0 ? firefly_x1.mixed.longitude.value : -firefly_x1.mixed.longitude.value;

	// Convert from DDMM.MMMM to DD.DDDDDD, assume the module report 4 or more
	// factional digits and convert to always 4 factional digits
	ulat /= firefly_x1.mixed.latitude.scale / 10000;
	ulon /= firefly_x1.mixed.longitude.scale / 10000;

	uint32_t ulat_mins = ulat % 1000000;
	uint32_t ulon_mins = ulon % 1000000;

	ulat -= ulat_mins;
	ulon -= ulon_mins;

	ulat += ulat_mins * 100 / 60;
	ulon += ulon_mins * 100 / 60;

	response->latitude  = ulat;
	response->ns        = firefly_x1.mixed.latitude.value > 0 ? 'N' : 'S';
	response->longitude = ulon;
	response->ew        = firefly_x1.mixed.longitude.value > 0 ? 'E' : 'W';

	return HANDLE_MESSAGE_RESPONSE_NEW_MESSAGE;
}

BootloaderHandleMessageResponse get_status(const GetStatus *data, GetStatus_Response *response) {
	response->header.length = sizeof(GetStatus_Response);

	response->has_fix         = firefly_x1.mixed.fix_quality != 0;
	response->satellites_view = firefly_x1.mixed.satellites_tracked;

	return HANDLE_MESSAGE_RESPONSE_NEW_MESSAGE;
}

BootloaderHandleMessageResponse get_altitude(const GetAltitude *data, GetAltitude_Response *response) {
	response->header.length = sizeof(GetAltitude_Response);

	if (firefly_x1.mixed.altitude.scale <= 100) {
		response->altitude = firefly_x1.mixed.altitude.value * (100 / firefly_x1.mixed.altitude.scale);
	} else {
		response->altitude = firefly_x1.mixed.altitude.value / (firefly_x1.mixed.altitude.scale / 100);
	}

	if (firefly_x1.mixed.height.scale <= 100) {
		response->geoidal_separation = firefly_x1.mixed.height.value * (100 / firefly_x1.mixed.height.scale);
	} else {
		response->geoidal_separation = firefly_x1.mixed.height.value / (firefly_x1.mixed.height.scale / 100);
	}

	return HANDLE_MESSAGE_RESPONSE_NEW_MESSAGE;
}

BootloaderHandleMessageResponse get_motion(const GetMotion *data, GetMotion_Response *response) {
	response->header.length = sizeof(GetMotion_Response);
	response->course = firefly_x1.mixed.course.value * (100/firefly_x1.mixed.course.scale);
	response->speed  = firefly_x1.mixed.speed.value  * 100 * 1852 / (firefly_x1.mixed.speed.scale * 1000); // knots -> km/h

	return HANDLE_MESSAGE_RESPONSE_NEW_MESSAGE;
}

BootloaderHandleMessageResponse get_date_time(const GetDateTime *data, GetDateTime_Response *response) {
	response->header.length = sizeof(GetDateTime_Response);
	response->date = firefly_x1.mixed.date.day*100*100 + firefly_x1.mixed.date.month*100 + firefly_x1.mixed.date.year;
	response->time = firefly_x1.mixed.time.hours*1000*100*100 + firefly_x1.mixed.time.minutes*1000*100 + firefly_x1.mixed.time.seconds*1000 + firefly_x1.mixed.time.microseconds/1000;

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

BootloaderHandleMessageResponse get_satellite_system_status_low_level(const GetSatelliteSystemStatusLowLevel *data, GetSatelliteSystemStatusLowLevel_Response *response) {
	response->header.length = sizeof(GetSatelliteSystemStatusLowLevel_Response);

	FireFlyX1DataSingle *single;
	if(data->satellite_system == GPS_V2_SATELLITE_SYSTEM_GPS) {
		single = &firefly_x1.gps;
	} else if(data->satellite_system == GPS_V2_SATELLITE_SYSTEM_GLONASS) {
		single = &firefly_x1.glonass;
	} else {
		return HANDLE_MESSAGE_RESPONSE_INVALID_PARAMETER;
	}

	response->satellite_numbers_length = 0;

	for(uint8_t i = 0; i < 12; i++) {
		if(single->sats[i] != 0) {
			response->satellite_numbers_data[response->satellite_numbers_length++] = single->sats[i];
		}
	}

	for(uint8_t i = response->satellite_numbers_length; i < 12; i++) {
		response->satellite_numbers_data[i] = 0;
	}

	response->fix  = single->fix_type;
	response->hdop = single->hdop.value * (100/single->hdop.scale);
	response->vdop = single->vdop.value * (100/single->hdop.scale);
	response->pdop = single->pdop.value * (100/single->hdop.scale);

	return HANDLE_MESSAGE_RESPONSE_NEW_MESSAGE;
}

BootloaderHandleMessageResponse get_satellite_status(const GetSatelliteStatus *data, GetSatelliteStatus_Response *response) {
	response->header.length = sizeof(GetSatelliteStatus_Response);

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

BootloaderHandleMessageResponse set_fix_led_config(const SetFixLEDConfig *data) {
	if(data->config > GPS_V2_FIX_LED_CONFIG_SHOW_PPS) {
		return HANDLE_MESSAGE_RESPONSE_INVALID_PARAMETER;
	}

	// The fix config and flicker config are not the same, we save the
	// "show fix" and "show pps" options as "external"
	if(data->config == GPS_V2_FIX_LED_CONFIG_SHOW_FIX || data->config == GPS_V2_FIX_LED_CONFIG_SHOW_PPS) {
		firefly_x1.fix_led_state.config = LED_FLICKER_CONFIG_EXTERNAL;
	} else {
		firefly_x1.fix_led_state.config = data->config;
	}

	firefly_x1.fix_led_config = data->config;

	// Set LED according to value
	if(firefly_x1.fix_led_state.config == LED_FLICKER_CONFIG_OFF || firefly_x1.fix_led_state.config == LED_FLICKER_CONFIG_EXTERNAL) {
		XMC_GPIO_SetOutputHigh(FIREFLY_X1_FIX_LED_PIN);
	} else {
		XMC_GPIO_SetOutputLow(FIREFLY_X1_FIX_LED_PIN);
	}

	return HANDLE_MESSAGE_RESPONSE_EMPTY;
}

BootloaderHandleMessageResponse get_fix_led_config(const GetFixLEDConfig *data, GetFixLEDConfig_Response *response) {
	response->header.length = sizeof(GetFixLEDConfig_Response);
	response->config = firefly_x1.fix_led_config;

	return HANDLE_MESSAGE_RESPONSE_NEW_MESSAGE;
}

BootloaderHandleMessageResponse set_coordinates_callback_period(const SetCoordinatesCallbackPeriod *data) {
	callback_period_coordinates = data->period;

	uartbb_puts("set_coordinates_callback_period: ");
	uartbb_puti(callback_period_coordinates);
	uartbb_putnl();

	return HANDLE_MESSAGE_RESPONSE_EMPTY;
}

BootloaderHandleMessageResponse get_coordinates_callback_period(const GetCoordinatesCallbackPeriod *data, GetCoordinatesCallbackPeriod_Response *response) {
	response->header.length = sizeof(GetCoordinatesCallbackPeriod_Response);
	response->period = callback_period_coordinates;

	return HANDLE_MESSAGE_RESPONSE_NEW_MESSAGE;
}

BootloaderHandleMessageResponse set_status_callback_period(const SetStatusCallbackPeriod *data) {
	callback_period_status = data->period;

	return HANDLE_MESSAGE_RESPONSE_EMPTY;
}

BootloaderHandleMessageResponse get_status_callback_period(const GetStatusCallbackPeriod *data, GetStatusCallbackPeriod_Response *response) {
	response->header.length = sizeof(GetStatusCallbackPeriod_Response);
	response->period = callback_period_status;

	return HANDLE_MESSAGE_RESPONSE_NEW_MESSAGE;
}

BootloaderHandleMessageResponse set_altitude_callback_period(const SetAltitudeCallbackPeriod *data) {
	callback_period_altitude = data->period;

	return HANDLE_MESSAGE_RESPONSE_EMPTY;
}

BootloaderHandleMessageResponse get_altitude_callback_period(const GetAltitudeCallbackPeriod *data, GetAltitudeCallbackPeriod_Response *response) {
	response->header.length = sizeof(GetAltitudeCallbackPeriod_Response);
	response->period = callback_period_altitude;

	return HANDLE_MESSAGE_RESPONSE_NEW_MESSAGE;
}

BootloaderHandleMessageResponse set_motion_callback_period(const SetMotionCallbackPeriod *data) {
	callback_period_motion = data->period;

	return HANDLE_MESSAGE_RESPONSE_EMPTY;
}

BootloaderHandleMessageResponse get_motion_callback_period(const GetMotionCallbackPeriod *data, GetMotionCallbackPeriod_Response *response) {
	response->header.length = sizeof(GetMotionCallbackPeriod_Response);
	response->period = callback_period_motion;

	return HANDLE_MESSAGE_RESPONSE_NEW_MESSAGE;
}

BootloaderHandleMessageResponse set_date_time_callback_period(const SetDateTimeCallbackPeriod *data) {
	callback_period_date_time = data->period;

	return HANDLE_MESSAGE_RESPONSE_EMPTY;
}

BootloaderHandleMessageResponse get_date_time_callback_period(const GetDateTimeCallbackPeriod *data, GetDateTimeCallbackPeriod_Response *response) {
	response->header.length = sizeof(GetDateTimeCallbackPeriod_Response);
	response->period = callback_period_date_time;

	return HANDLE_MESSAGE_RESPONSE_NEW_MESSAGE;
}

BootloaderHandleMessageResponse set_sbas_config(const SetSBASConfig *data) {
	if(data->sbas_config > GPS_V2_SBAS_DISABLED) {
		return HANDLE_MESSAGE_RESPONSE_INVALID_PARAMETER;
	}

	firefly_x1.sbas_enabled = data->sbas_config == GPS_V2_SBAS_ENABLED;
	firefly_x1_update_sbas(&firefly_x1);

	return HANDLE_MESSAGE_RESPONSE_EMPTY;
}

BootloaderHandleMessageResponse get_sbas_config(const GetSBASConfig *data, GetSBASConfig_Response *response) {
	response->header.length = sizeof(GetSBASConfig_Response);
	if(firefly_x1.sbas_enabled) {
		response->sbas_config = GPS_V2_SBAS_ENABLED;
	} else {
		response->sbas_config = GPS_V2_SBAS_DISABLED;
	}

	return HANDLE_MESSAGE_RESPONSE_NEW_MESSAGE;
}

bool handle_pulse_per_second_callback(void) {
	static bool last_pps_high = false;
	static bool is_buffered = false;
	static PulsePerSecond_Callback cb;

	if(!is_buffered) {
		if(XMC_GPIO_GetInput(FIREFLY_X1_PPS_PIN)) {
			if(!last_pps_high) {
				tfp_make_default_header(&cb.header, bootloader_get_uid(), sizeof(PulsePerSecond_Callback), FID_CALLBACK_PULSE_PER_SECOND);
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
		bootloader_spitfp_send_ack_and_message(&bootloader_status, (uint8_t*)&cb, sizeof(PulsePerSecond_Callback));
		is_buffered = false;
		return true;
	} else {
		is_buffered = true;
	}

	return false;
}

bool handle_coordinates_callback(void) {
	static bool is_buffered = false;
	static Coordinates_Callback cb;
	static uint32_t last_callback = 0;

	if(!is_buffered) {
		if(callback_period_coordinates != 0 && firefly_x1.new_coordinates && system_timer_is_time_elapsed_ms(last_callback, callback_period_coordinates)) {
			firefly_x1.new_coordinates = false;
			last_callback = system_timer_get_ms();
			tfp_make_default_header(&cb.header, bootloader_get_uid(), sizeof(Coordinates_Callback), FID_CALLBACK_COORDINATES);
			get_coordinates(NULL, (GetCoordinates_Response *) &cb);
		} else {
			return false;
		}
	}

	if(bootloader_spitfp_is_send_possible(&bootloader_status.st)) {
		bootloader_spitfp_send_ack_and_message(&bootloader_status, (uint8_t*)&cb, sizeof(Coordinates_Callback));
		is_buffered = false;
		return true;
	} else {
		is_buffered = true;
	}

	return false;
}

bool handle_status_callback(void) {
	static bool is_buffered = false;
	static Status_Callback cb;
	static uint32_t last_callback = 0;

	if(!is_buffered) {
		if(callback_period_status != 0 && firefly_x1.new_status && system_timer_is_time_elapsed_ms(last_callback, callback_period_status)) {
			firefly_x1.new_status = false;
			last_callback = system_timer_get_ms();
			tfp_make_default_header(&cb.header, bootloader_get_uid(), sizeof(Status_Callback), FID_CALLBACK_STATUS);
			get_status(NULL, (GetStatus_Response *) &cb);
		} else {
			return false;
		}
	}

	if(bootloader_spitfp_is_send_possible(&bootloader_status.st)) {
		bootloader_spitfp_send_ack_and_message(&bootloader_status, (uint8_t*)&cb, sizeof(Status_Callback));
		is_buffered = false;
		return true;
	} else {
		is_buffered = true;
	}

	return false;
}

bool handle_altitude_callback(void) {
	static bool is_buffered = false;
	static Altitude_Callback cb;
	static uint32_t last_callback = 0;

	if(!is_buffered) {
		if(callback_period_altitude != 0 && firefly_x1.new_altitude && system_timer_is_time_elapsed_ms(last_callback, callback_period_altitude)) {
			firefly_x1.new_altitude = false;
			last_callback = system_timer_get_ms();
			tfp_make_default_header(&cb.header, bootloader_get_uid(), sizeof(Altitude_Callback), FID_CALLBACK_ALTITUDE);
			get_altitude(NULL, (GetAltitude_Response *) &cb);
		} else {
			return false;
		}
	}

	if(bootloader_spitfp_is_send_possible(&bootloader_status.st)) {
		bootloader_spitfp_send_ack_and_message(&bootloader_status, (uint8_t*)&cb, sizeof(Altitude_Callback));
		is_buffered = false;
		return true;
	} else {
		is_buffered = true;
	}

	return false;
}

bool handle_motion_callback(void) {
	static bool is_buffered = false;
	static Motion_Callback cb;
	static uint32_t last_callback = 0;

	if(!is_buffered) {
		if(callback_period_motion != 0 && firefly_x1.new_motion && system_timer_is_time_elapsed_ms(last_callback, callback_period_motion)) {
			firefly_x1.new_motion = false;
			last_callback = system_timer_get_ms();
			tfp_make_default_header(&cb.header, bootloader_get_uid(), sizeof(Motion_Callback), FID_CALLBACK_MOTION);
			get_motion(NULL, (GetMotion_Response *) &cb);
		} else {
			return false;
		}
	}

	if(bootloader_spitfp_is_send_possible(&bootloader_status.st)) {
		bootloader_spitfp_send_ack_and_message(&bootloader_status, (uint8_t*)&cb, sizeof(Motion_Callback));
		is_buffered = false;
		return true;
	} else {
		is_buffered = true;
	}

	return false;
}

bool handle_date_time_callback(void) {
	static bool is_buffered = false;
	static DateTime_Callback cb;
	static uint32_t last_callback = 0;

	if(!is_buffered) {
		if(callback_period_date_time != 0 && firefly_x1.new_date_time && system_timer_is_time_elapsed_ms(last_callback, callback_period_date_time)) {
			firefly_x1.new_date_time = false;
			last_callback = system_timer_get_ms();
			tfp_make_default_header(&cb.header, bootloader_get_uid(), sizeof(DateTime_Callback), FID_CALLBACK_DATE_TIME);
			get_date_time(NULL, (GetDateTime_Response *) &cb);
		} else {
			return false;
		}
	}

	if(bootloader_spitfp_is_send_possible(&bootloader_status.st)) {
		bootloader_spitfp_send_ack_and_message(&bootloader_status, (uint8_t*)&cb, sizeof(DateTime_Callback));
		is_buffered = false;
		return true;
	} else {
		is_buffered = true;
	}

	return false;
}

void communication_tick(void) {
	handle_pulse_per_second_callback();
	communication_callback_tick();
}

void communication_init(void) {
	communication_callback_init();
}
