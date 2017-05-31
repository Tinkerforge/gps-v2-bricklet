<?php

require_once('Tinkerforge/IPConnection.php');
require_once('Tinkerforge/BrickletGPSV2.php');

use Tinkerforge\IPConnection;
use Tinkerforge\BrickletGPSV2;

const HOST = 'localhost';
const PORT = 4223;
const UID = 'XYZ'; // Change XYZ to the UID of your GPS Bricklet 2.0

// Callback function for coordinates callback
function cb_coordinates($latitude, $ns, $longitude, $ew)
{
    echo "Latitude: " . $latitude/1000000.0 . "° $ns\n";
    echo "Longitude: " . $longitude/1000000.0 . " ° $ew\n";
    echo "\n";
}

$ipcon = new IPConnection(); // Create IP connection
$gps = new BrickletGPSV2(UID, $ipcon); // Create device object

$ipcon->connect(HOST, PORT); // Connect to brickd
// Don't use device before ipcon is connected

// Register coordinates callback to function cb_coordinates
$gps->registerCallback(BrickletGPSV2::CALLBACK_COORDINATES, 'cb_coordinates');

// Set period for coordinates callback to 1s (1000ms)
// Note: The coordinates callback is only called every second
//       if the coordinates has changed since the last call!
$gps->setCoordinatesCallbackPeriod(1000);

echo "Press ctrl+c to exit\n";
$ipcon->dispatchCallbacks(-1); // Dispatch callbacks forever

?>