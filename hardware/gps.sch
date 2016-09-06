EESchema Schematic File Version 2
LIBS:tinkerforge
LIBS:power
LIBS:device
LIBS:transistors
LIBS:conn
LIBS:linear
LIBS:regul
LIBS:74xx
LIBS:cmos4000
LIBS:adc-dac
LIBS:memory
LIBS:xilinx
LIBS:microcontrollers
LIBS:dsp
LIBS:microchip
LIBS:analog_switches
LIBS:motorola
LIBS:texas
LIBS:intel
LIBS:audio
LIBS:interface
LIBS:digital-audio
LIBS:philips
LIBS:display
LIBS:cypress
LIBS:siliconi
LIBS:opto
LIBS:atmel
LIBS:contrib
LIBS:valves
LIBS:gps-cache
EELAYER 25 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title "GPS Bricklet"
Date "2016-01-28"
Rev "2.0"
Comp "Tinkerforge GmbH"
Comment1 "Licensed under CERN OHL v.1.1"
Comment2 "Copyright (©) 2016, B.Nordmeyer <bastian@tinkerforge.com>"
Comment3 ""
Comment4 ""
$EndDescr
Wire Wire Line
	5400 1350 5700 1350
Wire Wire Line
	2100 1700 2100 2000
Wire Wire Line
	2100 2000 2000 2000
Wire Wire Line
	3650 2000 3650 1900
Wire Wire Line
	3650 2200 4100 2200
Wire Wire Line
	2000 2200 2350 2200
Connection ~ 2750 2300
Wire Wire Line
	2750 2100 2850 2100
Wire Wire Line
	2850 2200 2650 2200
Wire Wire Line
	2000 2300 2650 2300
Wire Wire Line
	1550 2950 1550 2850
Wire Wire Line
	2850 2300 2750 2300
Wire Wire Line
	2750 2400 2750 2000
Wire Wire Line
	2750 2000 2850 2000
Connection ~ 2750 2100
Wire Wire Line
	2000 2100 2350 2100
Wire Wire Line
	3650 2300 4100 2300
Wire Wire Line
	3650 1900 3700 1900
Connection ~ 3650 1900
Wire Wire Line
	2000 1900 2250 1900
Wire Wire Line
	6150 1500 6150 1350
Wire Wire Line
	5600 4200 6700 4200
Wire Wire Line
	6700 4500 6700 4400
$Comp
L GND #PWR01
U 1 1 507EAA92
P 6700 4500
F 0 "#PWR01" H 6700 4500 30  0001 C CNN
F 1 "GND" H 6700 4430 30  0001 C CNN
F 2 "" H 6700 4500 60  0001 C CNN
F 3 "" H 6700 4500 60  0001 C CNN
	1    6700 4500
	1    0    0    -1  
$EndComp
$Comp
L CONN_2 P2
U 1 1 506C19FC
P 7050 4300
F 0 "P2" V 7000 4300 40  0000 C CNN
F 1 "U.FL" V 7100 4300 40  0000 C CNN
F 2 "kicad-libraries:U.FL-CON" H 7050 4300 60  0001 C CNN
F 3 "" H 7050 4300 60  0001 C CNN
	1    7050 4300
	1    0    0    -1  
$EndComp
$Comp
L R R6
U 1 1 5045D394
P 3650 4350
F 0 "R6" V 3700 4150 50  0000 C CNN
F 1 "TBD" V 3650 4350 50  0000 C CNN
F 2 "kicad-libraries:0402E" H 3650 4350 60  0001 C CNN
F 3 "" H 3650 4350 60  0001 C CNN
	1    3650 4350
	0    -1   -1   0   
$EndComp
$Comp
L +BATT #PWR02
U 1 1 5045D38B
P 3500 2600
F 0 "#PWR02" H 3500 2550 20  0001 C CNN
F 1 "+BATT" H 3500 2750 30  0000 C CNN
F 2 "" H 3500 2600 60  0001 C CNN
F 3 "" H 3500 2600 60  0001 C CNN
	1    3500 2600
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR04
U 1 1 5045D281
P 6150 1500
F 0 "#PWR04" H 6150 1500 30  0001 C CNN
F 1 "GND" H 6150 1430 30  0001 C CNN
F 2 "" H 6150 1500 60  0001 C CNN
F 3 "" H 6150 1500 60  0001 C CNN
	1    6150 1500
	1    0    0    -1  
$EndComp
$Comp
L +BATT #PWR05
U 1 1 5045D242
P 5400 1350
F 0 "#PWR05" H 5400 1300 20  0001 C CNN
F 1 "+BATT" H 5400 1450 30  0000 C CNN
F 2 "" H 5400 1350 60  0001 C CNN
F 3 "" H 5400 1350 60  0001 C CNN
	1    5400 1350
	1    0    0    -1  
$EndComp
$Comp
L C C2
U 1 1 4F018CB9
P 5000 2950
F 0 "C2" V 5050 3050 50  0000 L CNN
F 1 "100nF" V 4850 2850 50  0000 L CNN
F 2 "kicad-libraries:0603E" H 5000 2950 60  0001 C CNN
F 3 "" H 5000 2950 60  0001 C CNN
	1    5000 2950
	-1   0    0    1   
$EndComp
$Comp
L VCC #PWR06
U 1 1 4F018CB5
P 6400 2650
F 0 "#PWR06" H 6400 2750 30  0001 C CNN
F 1 "VCC" H 6400 2750 30  0000 C CNN
F 2 "" H 6400 2650 60  0001 C CNN
F 3 "" H 6400 2650 60  0001 C CNN
	1    6400 2650
	1    0    0    -1  
$EndComp
Text Notes 550  7200 0    40   ~ 0
Copyright Tinkerforge GmbH 2016.\nThis documentation describes Open Hardware and is licensed under the\nCERN OHL v. 1.1.\nYou may redistribute and modify this documentation under the terms of the\nCERN OHL v.1.1. (http://ohwr.org/cernohl). This documentation is distributed\nWITHOUT ANY EXPRESS OR IMPLIED WARRANTY, INCLUDING OF\nMERCHANTABILITY, SATISFACTORY QUALITY AND FITNESS FOR A\nPARTICULAR PURPOSE. Please see the CERN OHL v.1.1 for applicable\nconditions\n
NoConn ~ 2000 1800
$Comp
L GND #PWR07
U 1 1 4CE29748
P 4200 1950
F 0 "#PWR07" H 4200 1950 30  0001 C CNN
F 1 "GND" H 4200 1880 30  0001 C CNN
F 2 "" H 4200 1950 60  0001 C CNN
F 3 "" H 4200 1950 60  0001 C CNN
	1    4200 1950
	1    0    0    -1  
$EndComp
NoConn ~ 3650 2100
$Comp
L DRILL U3
U 1 1 4C6050A5
P 10650 6150
F 0 "U3" H 10700 6200 60  0001 C CNN
F 1 "DRILL" H 10650 6150 60  0000 C CNN
F 2 "kicad-libraries:DRILL_NP" H 10650 6150 60  0001 C CNN
F 3 "" H 10650 6150 60  0001 C CNN
	1    10650 6150
	1    0    0    -1  
$EndComp
$Comp
L DRILL U4
U 1 1 4C6050A2
P 10650 6350
F 0 "U4" H 10700 6400 60  0001 C CNN
F 1 "DRILL" H 10650 6350 60  0000 C CNN
F 2 "kicad-libraries:DRILL_NP" H 10650 6350 60  0001 C CNN
F 3 "" H 10650 6350 60  0001 C CNN
	1    10650 6350
	1    0    0    -1  
$EndComp
$Comp
L DRILL U6
U 1 1 4C60509F
P 11000 6350
F 0 "U6" H 11050 6400 60  0001 C CNN
F 1 "DRILL" H 11000 6350 60  0000 C CNN
F 2 "kicad-libraries:DRILL_NP" H 11000 6350 60  0001 C CNN
F 3 "" H 11000 6350 60  0001 C CNN
	1    11000 6350
	1    0    0    -1  
$EndComp
$Comp
L DRILL U5
U 1 1 4C605099
P 11000 6150
F 0 "U5" H 11050 6200 60  0001 C CNN
F 1 "DRILL" H 11000 6150 60  0000 C CNN
F 2 "kicad-libraries:DRILL_NP" H 11000 6150 60  0001 C CNN
F 3 "" H 11000 6150 60  0001 C CNN
	1    11000 6150
	1    0    0    -1  
$EndComp
$Comp
L C C1
U 1 1 4C5FD6ED
P 3900 1900
F 0 "C1" V 4050 1900 50  0000 L CNN
F 1 "100nF" V 3750 1750 50  0000 L CNN
F 2 "kicad-libraries:0603E" H 3900 1900 60  0001 C CNN
F 3 "" H 3900 1900 60  0001 C CNN
	1    3900 1900
	0    -1   -1   0   
$EndComp
Text GLabel 4100 2300 2    60   Input ~ 0
SDA
Text GLabel 4100 2200 2    60   Input ~ 0
SCL
$Comp
L VCC #PWR08
U 1 1 4C5FD35E
P 3650 1900
F 0 "#PWR08" H 3650 2000 30  0001 C CNN
F 1 "VCC" H 3650 2000 30  0000 C CNN
F 2 "" H 3650 1900 60  0001 C CNN
F 3 "" H 3650 1900 60  0001 C CNN
	1    3650 1900
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR09
U 1 1 4C5FD34E
P 2750 2400
F 0 "#PWR09" H 2750 2400 30  0001 C CNN
F 1 "GND" H 2750 2330 30  0001 C CNN
F 2 "" H 2750 2400 60  0001 C CNN
F 3 "" H 2750 2400 60  0001 C CNN
	1    2750 2400
	1    0    0    -1  
$EndComp
$Comp
L CAT24C U1
U 1 1 4C5FD337
P 3250 2400
F 0 "U1" H 3100 2900 60  0000 C CNN
F 1 "M24C64" H 3250 2400 60  0000 C CNN
F 2 "kicad-libraries:SOIC8" H 3250 2400 60  0001 C CNN
F 3 "" H 3250 2400 60  0001 C CNN
	1    3250 2400
	1    0    0    -1  
$EndComp
$Comp
L VCC #PWR010
U 1 1 4C5FCFB4
P 2100 1700
F 0 "#PWR010" H 2100 1800 30  0001 C CNN
F 1 "VCC" H 2100 1800 30  0000 C CNN
F 2 "" H 2100 1700 60  0001 C CNN
F 3 "" H 2100 1700 60  0001 C CNN
	1    2100 1700
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR011
U 1 1 4C5FCF5E
P 2250 1900
F 0 "#PWR011" H 2250 1900 30  0001 C CNN
F 1 "GND" H 2250 1830 30  0001 C CNN
F 2 "" H 2250 1900 60  0001 C CNN
F 3 "" H 2250 1900 60  0001 C CNN
	1    2250 1900
	0    -1   -1   0   
$EndComp
$Comp
L GND #PWR012
U 1 1 4C5FCF4F
P 1550 2950
F 0 "#PWR012" H 1550 2950 30  0001 C CNN
F 1 "GND" H 1550 2880 30  0001 C CNN
F 2 "" H 1550 2950 60  0001 C CNN
F 3 "" H 1550 2950 60  0001 C CNN
	1    1550 2950
	1    0    0    -1  
$EndComp
$Comp
L CON-SENSOR P1
U 1 1 4C5FCF27
P 1550 2250
F 0 "P1" H 1300 2800 60  0000 C CNN
F 1 "CON-SENSOR" V 1700 2250 60  0000 C CNN
F 2 "kicad-libraries:CON-SENSOR" H 1550 2250 60  0001 C CNN
F 3 "" H 1550 2250 60  0001 C CNN
	1    1550 2250
	-1   0    0    -1  
$EndComp
Wire Wire Line
	4100 1900 4200 1900
Wire Wire Line
	4200 1900 4200 1950
Text GLabel 2350 2100 2    60   Output ~ 0
SCL
Text GLabel 2350 2200 2    60   Output ~ 0
SDA
Wire Wire Line
	2650 2300 2650 2200
$Comp
L FireFlyX1 U2
U 1 1 56A9E6CD
P 4950 4650
F 0 "U2" H 4450 5400 60  0000 C CNN
F 1 "FireFlyX1" H 5500 3900 60  0000 C CNN
F 2 "kicad-libraries:FireFlyX1" H 4950 5100 60  0001 C CNN
F 3 "" H 4950 5100 60  0000 C CNN
	1    4950 4650
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR013
U 1 1 56A9EBCE
P 4950 5500
F 0 "#PWR013" H 4950 5500 30  0001 C CNN
F 1 "GND" H 4950 5430 30  0001 C CNN
F 2 "" H 4950 5500 60  0001 C CNN
F 3 "" H 4950 5500 60  0001 C CNN
	1    4950 5500
	1    0    0    -1  
$EndComp
Wire Wire Line
	4700 5450 5200 5450
Wire Wire Line
	4950 5450 4950 5500
Connection ~ 4950 5450
Connection ~ 5000 5450
Connection ~ 5100 5450
Connection ~ 4800 5450
Connection ~ 4900 5450
$Comp
L R_PACK4 RP1
U 1 1 56A9EDD1
P 3700 3750
F 0 "RP1" H 3700 4200 50  0000 C CNN
F 1 "TBD" H 3700 3700 50  0000 C CNN
F 2 "kicad-libraries:4X0603" H 3700 3750 50  0001 C CNN
F 3 "" H 3700 3750 50  0000 C CNN
	1    3700 3750
	0    1    1    0   
$EndComp
$Comp
L VCC #PWR014
U 1 1 56A9F018
P 3900 3450
F 0 "#PWR014" H 3900 3550 30  0001 C CNN
F 1 "VCC" H 3900 3550 30  0000 C CNN
F 2 "" H 3900 3450 60  0001 C CNN
F 3 "" H 3900 3450 60  0001 C CNN
	1    3900 3450
	1    0    0    -1  
$EndComp
Wire Wire Line
	3750 3550 4050 3550
Wire Wire Line
	3900 3550 3900 3450
Connection ~ 3900 3550
Connection ~ 3950 3550
Connection ~ 3850 3550
Wire Wire Line
	2000 2500 2550 2500
Wire Wire Line
	2550 2500 2550 4100
Wire Wire Line
	2450 4200 2450 2600
Wire Wire Line
	2450 2600 2000 2600
Wire Wire Line
	3750 3950 3750 4100
Connection ~ 3750 4100
Wire Wire Line
	4050 3950 4050 4200
Connection ~ 4050 4200
NoConn ~ 3850 3950
NoConn ~ 3950 3950
$Comp
L R R8
U 1 1 56A9F900
P 3900 4450
F 0 "R8" V 3850 4250 50  0000 C CNN
F 1 "TBD" V 3900 4450 50  0000 C CNN
F 2 "kicad-libraries:0402E" H 3900 4450 60  0001 C CNN
F 3 "" H 3900 4450 60  0001 C CNN
	1    3900 4450
	0    -1   -1   0   
$EndComp
Wire Wire Line
	4300 4350 3900 4350
Wire Wire Line
	4300 4450 4150 4450
Wire Wire Line
	2350 2700 2350 4550
Wire Wire Line
	2350 2700 2000 2700
Connection ~ 2350 4350
$Comp
L R R9
U 1 1 56A9FD27
P 3900 4600
F 0 "R9" V 3850 4400 50  0000 C CNN
F 1 "10k" V 3900 4600 50  0000 C CNN
F 2 "kicad-libraries:0603E" H 3900 4600 60  0001 C CNN
F 3 "" H 3900 4600 60  0001 C CNN
	1    3900 4600
	0    -1   -1   0   
$EndComp
$Comp
L VCC #PWR015
U 1 1 56A9FDC5
P 3600 4600
F 0 "#PWR015" H 3600 4700 30  0001 C CNN
F 1 "VCC" H 3600 4750 30  0000 C CNN
F 2 "" H 3600 4600 60  0001 C CNN
F 3 "" H 3600 4600 60  0001 C CNN
	1    3600 4600
	0    -1   -1   0   
$EndComp
Wire Wire Line
	4150 4600 4200 4600
Wire Wire Line
	4200 4600 4200 4450
Connection ~ 4200 4450
NoConn ~ 4300 4600
NoConn ~ 4300 4700
NoConn ~ 4300 4850
$Comp
L R R2
U 1 1 56AA07EC
P 3300 4100
F 0 "R2" V 3350 3900 50  0000 C CNN
F 1 "TBD" V 3300 4100 50  0000 C CNN
F 2 "kicad-libraries:0402E" H 3300 4100 60  0001 C CNN
F 3 "" H 3300 4100 60  0001 C CNN
	1    3300 4100
	0    -1   -1   0   
$EndComp
$Comp
L R R4
U 1 1 56AA087D
P 3650 4200
F 0 "R4" V 3700 4000 50  0000 C CNN
F 1 "TBD" V 3650 4200 50  0000 C CNN
F 2 "kicad-libraries:0402E" H 3650 4200 60  0001 C CNN
F 3 "" H 3650 4200 60  0001 C CNN
	1    3650 4200
	0    -1   -1   0   
$EndComp
Wire Wire Line
	3400 4200 2450 4200
$Comp
L R R10
U 1 1 56AA0BD3
P 4000 5000
F 0 "R10" V 4100 4800 50  0000 C CNN
F 1 "TBD" V 4000 5000 50  0000 C CNN
F 2 "kicad-libraries:0402E" H 4000 5000 60  0001 C CNN
F 3 "" H 4000 5000 60  0001 C CNN
	1    4000 5000
	0    -1   -1   0   
$EndComp
$Comp
L R R7
U 1 1 56AA0CD9
P 3800 5100
F 0 "R7" V 3750 4900 50  0000 C CNN
F 1 "TBD" V 3800 5100 50  0000 C CNN
F 2 "kicad-libraries:0402E" H 3800 5100 60  0001 C CNN
F 3 "" H 3800 5100 60  0001 C CNN
	1    3800 5100
	0    -1   -1   0   
$EndComp
$Comp
L R R5
U 1 1 56AA0D3B
P 3550 5200
F 0 "R5" V 3500 5000 50  0000 C CNN
F 1 "TBD" V 3550 5200 50  0000 C CNN
F 2 "kicad-libraries:0402E" H 3550 5200 60  0001 C CNN
F 3 "" H 3550 5200 60  0001 C CNN
	1    3550 5200
	0    -1   -1   0   
$EndComp
$Comp
L R R3
U 1 1 56AA0DF9
P 3300 5300
F 0 "R3" V 3250 5100 50  0000 C CNN
F 1 "TBD" V 3300 5300 50  0000 C CNN
F 2 "kicad-libraries:0402E" H 3300 5300 60  0001 C CNN
F 3 "" H 3300 5300 60  0001 C CNN
	1    3300 5300
	0    -1   -1   0   
$EndComp
Wire Wire Line
	2250 5000 2250 2400
Connection ~ 2250 2400
Wire Wire Line
	3000 5100 3000 4100
Connection ~ 3000 4100
Wire Wire Line
	2850 5200 2850 4200
Connection ~ 2850 4200
Wire Wire Line
	2350 4550 2750 4550
Wire Wire Line
	2750 4550 2750 5300
Connection ~ 2350 4450
$Comp
L R R1
U 1 1 56AA19D1
P 3200 2600
F 0 "R1" V 3280 2600 50  0000 C CNN
F 1 "TBD" V 3200 2600 50  0000 C CNN
F 2 "kicad-libraries:0603E" H 3200 2600 60  0001 C CNN
F 3 "" H 3200 2600 60  0001 C CNN
	1    3200 2600
	0    -1   -1   0   
$EndComp
$Comp
L +BATT #PWR016
U 1 1 56AA1FBE
P 5100 3400
F 0 "#PWR016" H 5100 3350 20  0001 C CNN
F 1 "+BATT" H 5100 3500 30  0000 C CNN
F 2 "" H 5100 3400 60  0001 C CNN
F 3 "" H 5100 3400 60  0001 C CNN
	1    5100 3400
	1    0    0    -1  
$EndComp
Wire Wire Line
	5100 3400 5100 3850
$Comp
L C C4
U 1 1 56AA228C
P 5300 3600
F 0 "C4" V 5350 3700 50  0000 L CNN
F 1 "1µF" V 5150 3500 50  0000 L CNN
F 2 "kicad-libraries:0603E" H 5300 3600 60  0001 C CNN
F 3 "" H 5300 3600 60  0001 C CNN
	1    5300 3600
	-1   0    0    1   
$EndComp
$Comp
L GND #PWR017
U 1 1 56AA25E0
P 5300 3850
F 0 "#PWR017" H 5300 3850 30  0001 C CNN
F 1 "GND" H 5300 3780 30  0001 C CNN
F 2 "" H 5300 3850 60  0001 C CNN
F 3 "" H 5300 3850 60  0001 C CNN
	1    5300 3850
	1    0    0    -1  
$EndComp
Wire Wire Line
	5100 3400 5300 3400
Connection ~ 5100 3400
Wire Wire Line
	4800 2750 4800 3850
$Comp
L GND #PWR018
U 1 1 56AA308C
P 5000 3200
F 0 "#PWR018" H 5000 3200 30  0001 C CNN
F 1 "GND" H 5000 3130 30  0001 C CNN
F 2 "" H 5000 3200 60  0001 C CNN
F 3 "" H 5000 3200 60  0001 C CNN
	1    5000 3200
	1    0    0    -1  
$EndComp
$Comp
L C C3
U 1 1 56AA33DD
P 5300 2950
F 0 "C3" V 5350 3050 50  0000 L CNN
F 1 "1uF" V 5150 2850 50  0000 L CNN
F 2 "kicad-libraries:0603E" H 5300 2950 60  0001 C CNN
F 3 "" H 5300 2950 60  0001 C CNN
	1    5300 2950
	-1   0    0    1   
$EndComp
$Comp
L GND #PWR019
U 1 1 56AA359B
P 5300 3200
F 0 "#PWR019" H 5300 3200 30  0001 C CNN
F 1 "GND" H 5300 3130 30  0001 C CNN
F 2 "" H 5300 3200 60  0001 C CNN
F 3 "" H 5300 3200 60  0001 C CNN
	1    5300 3200
	1    0    0    -1  
$EndComp
$Comp
L FILTER FB1
U 1 1 56AA368E
P 5900 2750
F 0 "FB1" H 5900 2900 60  0000 C CNN
F 1 "FB" H 5900 2650 60  0000 C CNN
F 2 "kicad-libraries:0603E" H 5900 2750 60  0001 C CNN
F 3 "" H 5900 2750 60  0001 C CNN
	1    5900 2750
	1    0    0    -1  
$EndComp
$Comp
L C C7
U 1 1 56AA387E
P 6350 2950
F 0 "C7" V 6400 3050 50  0000 L CNN
F 1 "1uF" V 6200 2850 50  0000 L CNN
F 2 "kicad-libraries:0603E" H 6350 2950 60  0001 C CNN
F 3 "" H 6350 2950 60  0001 C CNN
	1    6350 2950
	-1   0    0    1   
$EndComp
$Comp
L GND #PWR020
U 1 1 56AA3911
P 6350 3200
F 0 "#PWR020" H 6350 3200 30  0001 C CNN
F 1 "GND" H 6350 3130 30  0001 C CNN
F 2 "" H 6350 3200 60  0001 C CNN
F 3 "" H 6350 3200 60  0001 C CNN
	1    6350 3200
	1    0    0    -1  
$EndComp
Wire Wire Line
	5550 2750 4800 2750
Wire Wire Line
	6250 2750 6400 2750
Wire Wire Line
	6400 2750 6400 2650
Connection ~ 6350 2750
Connection ~ 5300 2750
Connection ~ 5000 2750
$Comp
L L_Small L1
U 1 1 56AA4A18
P 6250 4000
F 0 "L1" H 6280 4040 50  0000 L CNN
F 1 "33nF" H 6280 3960 50  0000 L CNN
F 2 "kicad-libraries:0603E" H 6250 4000 50  0001 C CNN
F 3 "" H 6250 4000 50  0000 C CNN
	1    6250 4000
	1    0    0    -1  
$EndComp
Text Notes 6300 3850 0    60   ~ 0
for active antennas
$Comp
L C C6
U 1 1 56AA4D38
P 6050 3850
F 0 "C6" V 6100 3950 50  0000 L CNN
F 1 "1µF" V 5900 3750 50  0000 L CNN
F 2 "kicad-libraries:0603E" H 6050 3850 60  0001 C CNN
F 3 "" H 6050 3850 60  0001 C CNN
	1    6050 3850
	0    -1   -1   0   
$EndComp
$Comp
L GND #PWR021
U 1 1 56AA4E55
P 5850 3900
F 0 "#PWR021" H 5850 3900 30  0001 C CNN
F 1 "GND" H 5850 3830 30  0001 C CNN
F 2 "" H 5850 3900 60  0001 C CNN
F 3 "" H 5850 3900 60  0001 C CNN
	1    5850 3900
	1    0    0    -1  
$EndComp
Wire Wire Line
	6250 4100 6250 4200
Connection ~ 6250 4200
Wire Wire Line
	6250 3800 6250 3900
$Comp
L VCC #PWR022
U 1 1 56AA520B
P 6250 3800
F 0 "#PWR022" H 6250 3900 30  0001 C CNN
F 1 "VCC" H 6250 3900 30  0000 C CNN
F 2 "" H 6250 3800 60  0001 C CNN
F 3 "" H 6250 3800 60  0001 C CNN
	1    6250 3800
	1    0    0    -1  
$EndComp
Connection ~ 6250 3850
Text Notes 6250 4300 0    60   ~ 0
50 Ohm
$Comp
L Battery BT1
U 1 1 56C49817
P 5850 1350
F 0 "BT1" H 5950 1400 50  0000 L CNN
F 1 "Battery" H 5950 1300 50  0000 L CNN
F 2 "kicad-libraries:BATTERY_HOLDER_2998" V 5850 1390 50  0001 C CNN
F 3 "" V 5850 1390 50  0000 C CNN
	1    5850 1350
	0    -1   -1   0   
$EndComp
Wire Wire Line
	6150 1350 6000 1350
Wire Wire Line
	5850 3900 5850 3850
Wire Wire Line
	3450 2600 3500 2600
Wire Wire Line
	6350 3200 6350 3150
Wire Wire Line
	5300 3200 5300 3150
Wire Wire Line
	5000 3200 5000 3150
Wire Wire Line
	5300 3850 5300 3800
Wire Wire Line
	3600 4600 3650 4600
Wire Wire Line
	3650 4450 2350 4450
Wire Wire Line
	3400 4350 2350 4350
Wire Wire Line
	3900 4200 4300 4200
Wire Wire Line
	3550 4100 4300 4100
Wire Wire Line
	2550 4100 3050 4100
Wire Wire Line
	4250 5000 4300 5000
Wire Wire Line
	3750 5000 2250 5000
Wire Wire Line
	4050 5100 4300 5100
Wire Wire Line
	3550 5100 3000 5100
Wire Wire Line
	2850 5200 3300 5200
Wire Wire Line
	3800 5200 4300 5200
Wire Wire Line
	2750 5300 3050 5300
Wire Wire Line
	3550 5300 4300 5300
Wire Wire Line
	2950 2600 2650 2600
Wire Wire Line
	2650 2600 2650 2400
Wire Wire Line
	2650 2400 2000 2400
$EndSCHEMATC