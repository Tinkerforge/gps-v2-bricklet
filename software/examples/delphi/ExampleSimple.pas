program ExampleSimple;

{$ifdef MSWINDOWS}{$apptype CONSOLE}{$endif}
{$ifdef FPC}{$mode OBJFPC}{$H+}{$endif}

uses
  SysUtils, IPConnection, BrickletGPSV2;

type
  TExample = class
  private
    ipcon: TIPConnection;
    gps: TBrickletGPSV2;
  public
    procedure Execute;
  end;

const
  HOST = 'localhost';
  PORT = 4223;
  UID = 'XYZ'; { Change XYZ to the UID of your GPS Bricklet 2.0 }

var
  e: TExample;

procedure TExample.Execute;
var latitude: longword; ns: char; longitude: longword; ew: char;
begin
  { Create IP connection }
  ipcon := TIPConnection.Create;

  { Create device object }
  gps := TBrickletGPSV2.Create(UID, ipcon);

  { Connect to brickd }
  ipcon.Connect(HOST, PORT);
  { Don't use device before ipcon is connected }

  { Get current coordinates }
  gps.GetCoordinates(latitude, ns, longitude, ew);

  WriteLn(Format('Latitude: %.6f° %s', [latitude/1000000.0, ns]));
  WriteLn(Format('Longitude: %.6f° %s', [longitude/1000000.0, ew]));

  WriteLn('Press key to exit');
  ReadLn;
  ipcon.Destroy; { Calls ipcon.Disconnect internally }
end;

begin
  e := TExample.Create;
  e.Execute;
  e.Destroy;
end.
