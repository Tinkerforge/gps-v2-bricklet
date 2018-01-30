function matlab_example_simple()
    import com.tinkerforge.IPConnection;
    import com.tinkerforge.BrickletGPSV2;

    HOST = 'localhost';
    PORT = 4223;
    UID = 'XYZ'; % Change XYZ to the UID of your GPS Bricklet 2.0

    ipcon = IPConnection(); % Create IP connection
    gps = handle(BrickletGPSV2(UID, ipcon), 'CallbackProperties'); % Create device object

    ipcon.connect(HOST, PORT); % Connect to brickd
    % Don't use device before ipcon is connected

    % Get current coordinates
    coordinates = gps.getCoordinates();

    fprintf('Latitude: %g °\n', coordinates.latitude/1000000.0);
    fprintf('N/S: %s\n', coordinates.ns);
    fprintf('Longitude: %g °\n', coordinates.longitude/1000000.0);
    fprintf('E/W: %s\n', coordinates.ew);

    input('Press key to exit\n', 's');
    ipcon.disconnect();
end
