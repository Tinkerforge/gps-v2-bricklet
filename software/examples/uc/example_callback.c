// This example is not self-contained.
// It requres usage of the example driver specific to your platform.
// See the HAL documentation.

#include "bindings/hal_common.h"
#include "bindings/bricklet_gps_v2.h"

#define UID "XYZ" // Change XYZ to the UID of your GPS Bricklet 2.0

void check(int rc, const char* msg);

void example_setup(TF_HalContext *hal);
void example_loop(TF_HalContext *hal);


// Callback function for coordinates callback
static void coordinates_handler(TF_GPSV2 *device, uint32_t latitude, char ns,
                                uint32_t longitude, char ew, void *user_data) {
	(void)device; (void)user_data; // avoid unused parameter warning

	tf_hal_printf("Latitude: %d 1/%d °\n", latitude, 1000000.0);
	tf_hal_printf("N/S: %c\n", ns);
	tf_hal_printf("Longitude: %d 1/%d °\n", longitude, 1000000.0);
	tf_hal_printf("E/W: %c\n", ew);
	tf_hal_printf("\n");
}

static TF_GPSV2 gps;

void example_setup(TF_HalContext *hal) {
	// Create device object
	check(tf_gps_v2_create(&gps, UID, hal), "create device object");

	// Register coordinates callback to function coordinates_handler
	tf_gps_v2_register_coordinates_callback(&gps,
	                                        coordinates_handler,
	                                        NULL);

	// Set period for coordinates callback to 1s (1000ms)
	// Note: The coordinates callback is only called every second
	//       if the coordinates has changed since the last call!
	check(tf_gps_v2_set_coordinates_callback_period(&gps, 1000), "set coordinates callback period");
}

void example_loop(TF_HalContext *hal) {
	// Poll for callbacks
	tf_hal_callback_tick(hal, 0);
}
