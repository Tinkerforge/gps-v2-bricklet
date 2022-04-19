// This example is not self-contained.
// It requres usage of the example driver specific to your platform.
// See the HAL documentation.

#include "bindings/hal_common.h"
#include "bindings/bricklet_gps_v2.h"

#define UID "XYZ" // Change XYZ to the UID of your GPS Bricklet 2.0

void check(int rc, const char* msg);

void example_setup(TF_HAL *hal);
void example_loop(TF_HAL *hal);


static TF_GPSV2 gps;

void example_setup(TF_HAL *hal) {
	// Create device object
	check(tf_gps_v2_create(&gps, UID, hal), "create device object");

	// Get current coordinates
	uint32_t latitude, longitude; char ns, ew;
	check(tf_gps_v2_get_coordinates(&gps, &latitude, &ns, &longitude,
	                                &ew), "get coordinates");

	tf_hal_printf("Latitude: %d 1/%d °\n", latitude, 1000000);
	tf_hal_printf("N/S: %c\n", ns);
	tf_hal_printf("Longitude: %d 1/%d °\n", longitude, 1000000);
	tf_hal_printf("E/W: %c\n", ew);
}

void example_loop(TF_HAL *hal) {
	// Poll for callbacks
	tf_hal_callback_tick(hal, 0);
}
