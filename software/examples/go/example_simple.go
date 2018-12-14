package main

import (
	"fmt"
	"tinkerforge/gps_v2_bricklet"
	"tinkerforge/ipconnection"
)

const ADDR string = "localhost:4223"
const UID string = "XYZ" // Change XYZ to the UID of your GPS Bricklet 2.0.

func main() {
	ipcon := ipconnection.New()
	defer ipcon.Close()
	gps, _ := gps_v2_bricklet.New(UID, &ipcon) // Create device object.

	ipcon.Connect(ADDR) // Connect to brickd.
	defer ipcon.Disconnect()
	// Don't use device before ipcon is connected.

	// Get current coordinates.
	latitude, ns, longitude, ew, _ := gps.GetCoordinates()

	fmt.Printf("Latitude: %f °\n", float64(latitude)/1000000.0)
	fmt.Printf("N/S: \n", ns)
	fmt.Printf("Longitude: %f °\n", float64(longitude)/1000000.0)
	fmt.Printf("E/W: \n", ew)

	fmt.Print("Press enter to exit.")
	fmt.Scanln()

}