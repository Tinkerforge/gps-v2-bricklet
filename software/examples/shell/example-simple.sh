#!/bin/sh
# Connects to localhost:4223 by default, use --host and --port to change this

uid=XYZ # Change XYZ to the UID of your GPS Bricklet 2.0

# Get current coordinates
tinkerforge call gps-v2-bricklet $uid get-coordinates
