use std::{error::Error, io};

use tinkerforge::{gps_v2_bricklet::*, ip_connection::IpConnection};

const HOST: &str = "localhost";
const PORT: u16 = 4223;
const UID: &str = "XYZ"; // Change XYZ to the UID of your GPS Bricklet 2.0.

fn main() -> Result<(), Box<dyn Error>> {
    let ipcon = IpConnection::new(); // Create IP connection.
    let gps = GpsV2Bricklet::new(UID, &ipcon); // Create device object.

    ipcon.connect((HOST, PORT)).recv()??; // Connect to brickd.
                                          // Don't use device before ipcon is connected.

    // Get current coordinates.
    let coordinates = gps.get_coordinates().recv()?;

    println!("Latitude: {} °", coordinates.latitude as f32 / 1000000.0);
    println!("N/S: {}", coordinates.ns);
    println!("Longitude: {} °", coordinates.longitude as f32 / 1000000.0);
    println!("E/W: {}", coordinates.ew);

    println!("Press enter to exit.");
    let mut _input = String::new();
    io::stdin().read_line(&mut _input)?;
    ipcon.disconnect();
    Ok(())
}
