Imports System
Imports Tinkerforge

Module ExampleSimple
    Const HOST As String = "localhost"
    Const PORT As Integer = 4223
    Const UID As String = "XYZ" ' Change XYZ to the UID of your GPS Bricklet 2.0

    Sub Main()
        Dim ipcon As New IPConnection() ' Create IP connection
        Dim gps As New BrickletGPSV2(UID, ipcon) ' Create device object

        ipcon.Connect(HOST, PORT) ' Connect to brickd
        ' Don't use device before ipcon is connected

        ' Get current coordinates
        Dim latitude, longitude As Long
        Dim ns, ew As Char

        gps.GetCoordinates(latitude, ns, longitude, ew)

        Console.WriteLine("Latitude: " + (latitude/1000000.0).ToString() + "° " + ns)
        Console.WriteLine("Longitude: " + (longitude/1000000.0).ToString() + "° " + ew)

        Console.WriteLine("Press key to exit")
        Console.ReadLine()
        ipcon.Disconnect()
    End Sub
End Module
