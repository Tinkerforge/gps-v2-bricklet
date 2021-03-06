<?php

require_once('Tinkerforge/IPConnection.php');
require_once('Tinkerforge/BrickletGPSV2.php');

use Tinkerforge\IPConnection;
use Tinkerforge\BrickletGPSV2;

const HOST = 'localhost';
const PORT = 4223;
const UID = 'XYZ'; // Change XYZ to the UID of your GPS Bricklet 2.0

$ipcon = new IPConnection(); // Create IP connection
$gps = new BrickletGPSV2(UID, $ipcon); // Create device object

$ipcon->connect(HOST, PORT); // Connect to brickd
// Don't use device before ipcon is connected

// Get current coordinates
$coordinates = $gps->getCoordinates();

echo "Latitude: " . $coordinates['latitude']/1000000.0 . " °\n";
echo "N/S: " . $coordinates['ns'] . "\n";
echo "Longitude: " . $coordinates['longitude']/1000000.0 . " °\n";
echo "E/W: " . $coordinates['ew'] . "\n";

echo "Press key to exit\n";
fgetc(fopen('php://stdin', 'r'));
$ipcon->disconnect();

?>
