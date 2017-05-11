Imports System
Imports Tinkerforge

Module ExampleCallback
    Const HOST As String = "localhost"
    Const PORT As Integer = 4223
    Const UID As String = "XYZ" ' Change XYZ to the UID of your GPS Bricklet 2.0

    ' Callback subroutine for coordinates callback
    Sub CoordinatesCB(ByVal sender As BrickletGPSV2, ByVal latitude As Long, _
                      ByVal ns As Char, ByVal longitude As Long, ByVal ew As Char)
        Console.WriteLine("Latitude: " + (latitude/1000000.0).ToString() + "° " + ns)
        Console.WriteLine("Longitude: " + (longitude/1000000.0).ToString() + "° " + ew)
        Console.WriteLine("")
    End Sub

    Sub Main()
        Dim ipcon As New IPConnection() ' Create IP connection
        Dim gps As New BrickletGPSV2(UID, ipcon) ' Create device object

        ipcon.Connect(HOST, PORT) ' Connect to brickd
        ' Don't use device before ipcon is connected

        ' Register coordinates callback to subroutine CoordinatesCB
        AddHandler gps.CoordinatesCallback, AddressOf CoordinatesCB

        ' Set period for coordinates callback to 1s (1000ms)
        ' Note: The coordinates callback is only called every second
        '       if the coordinates has changed since the last call!
        gps.SetCoordinatesCallbackPeriod(1000)

        Console.WriteLine("Press key to exit")
        Console.ReadLine()
        ipcon.Disconnect()
    End Sub
End Module
