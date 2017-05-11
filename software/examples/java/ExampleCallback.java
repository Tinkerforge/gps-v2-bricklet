import com.tinkerforge.IPConnection;
import com.tinkerforge.BrickletGPSV2;

public class ExampleCallback {
	private static final String HOST = "localhost";
	private static final int PORT = 4223;

	// Change XYZ to the UID of your GPS Bricklet 2.0
	private static final String UID = "XYZ";

	// Note: To make the example code cleaner we do not handle exceptions. Exceptions
	//       you might normally want to catch are described in the documentation
	public static void main(String args[]) throws Exception {
		IPConnection ipcon = new IPConnection(); // Create IP connection
		BrickletGPSV2 gps = new BrickletGPSV2(UID, ipcon); // Create device object

		ipcon.connect(HOST, PORT); // Connect to brickd
		// Don't use device before ipcon is connected

		// Add coordinates listener
		gps.addCoordinatesListener(new BrickletGPSV2.CoordinatesListener() {
			public void coordinates(long latitude, char ns, long longitude, char ew) {
				System.out.println("Latitude: " + latitude/1000000.0 + "° " + ns);
				System.out.println("Longitude: " + longitude/1000000.0 + "° " + ew);
				System.out.println("");
			}
		});

		// Set period for coordinates callback to 1s (1000ms)
		// Note: The coordinates callback is only called every second
		//       if the coordinates has changed since the last call!
		gps.setCoordinatesCallbackPeriod(1000);

		System.out.println("Press key to exit"); System.in.read();
		ipcon.disconnect();
	}
}
