EESchema Schematic File Version 2
LIBS:power
LIBS:device
LIBS:switches
LIBS:relays
LIBS:motors
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
LIBS:rekicad
LIBS:megawin
LIBS:mg32f02a064-cache
EELAYER 25 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title ""
Date "2022-08-13"
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L +3.3V #PWR?
U 1 1 62BEC670
P 3800 1900
F 0 "#PWR?" H 3800 1750 50  0001 C CNN
F 1 "+3.3V" H 3800 2040 50  0000 C CNN
F 2 "" H 3800 1900 50  0001 C CNN
F 3 "" H 3800 1900 50  0001 C CNN
	1    3800 1900
	1    0    0    -1  
$EndComp
$Comp
L C_P C1
U 1 1 62BEC68B
P 3600 2100
F 0 "C1" H 3550 1950 60  0000 C CNN
F 1 "10,0" H 3600 1850 60  0000 C CNN
F 2 "" H 3600 2100 60  0000 C CNN
F 3 "" H 3600 2100 60  0000 C CNN
	1    3600 2100
	1    0    0    -1  
$EndComp
$Comp
L C_ C2
U 1 1 62BEC6C8
P 3900 2100
F 0 "C2" H 3950 1950 60  0000 C CNN
F 1 "0,1" H 3950 1850 60  0000 C CNN
F 2 "" H 3900 2100 60  0000 C CNN
F 3 "" H 3900 2100 60  0000 C CNN
	1    3900 2100
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR?
U 1 1 62BEC728
P 4100 2400
F 0 "#PWR?" H 4100 2150 50  0001 C CNN
F 1 "GND" H 4100 2250 50  0000 C CNN
F 2 "" H 4100 2400 50  0001 C CNN
F 3 "" H 4100 2400 50  0001 C CNN
	1    4100 2400
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR?
U 1 1 62BEC743
P 3800 2400
F 0 "#PWR?" H 3800 2150 50  0001 C CNN
F 1 "GND" H 3800 2250 50  0000 C CNN
F 2 "" H 3800 2400 50  0001 C CNN
F 3 "" H 3800 2400 50  0001 C CNN
	1    3800 2400
	1    0    0    -1  
$EndComp
$Comp
L C_P C3
U 1 1 62BEC775
P 5800 5100
F 0 "C3" H 5850 4950 60  0000 C CNN
F 1 "4,7" H 5850 4850 60  0000 C CNN
F 2 "" H 5800 5100 60  0000 C CNN
F 3 "" H 5800 5100 60  0000 C CNN
	1    5800 5100
	1    0    0    -1  
$EndComp
$Comp
L C_ C4
U 1 1 62BEC7A9
P 6100 5100
F 0 "C4" H 6150 4950 60  0000 C CNN
F 1 "0,1" H 6150 4850 60  0000 C CNN
F 2 "" H 6100 5100 60  0000 C CNN
F 3 "" H 6100 5100 60  0000 C CNN
	1    6100 5100
	1    0    0    -1  
$EndComp
Wire Wire Line
	3800 1900 3800 2200
Wire Wire Line
	4100 2200 4100 2100
Connection ~ 4100 2100
Connection ~ 3800 2100
Wire Wire Line
	3800 2350 3800 2400
Wire Wire Line
	4100 2350 4100 2400
Wire Wire Line
	6300 5200 6300 5000
$Comp
L GND #PWR?
U 1 1 62BEC833
P 6000 5400
F 0 "#PWR?" H 6000 5150 50  0001 C CNN
F 1 "GND" H 6000 5250 50  0000 C CNN
F 2 "" H 6000 5400 50  0001 C CNN
F 3 "" H 6000 5400 50  0001 C CNN
	1    6000 5400
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR?
U 1 1 62BEC84D
P 6300 5400
F 0 "#PWR?" H 6300 5150 50  0001 C CNN
F 1 "GND" H 6300 5250 50  0000 C CNN
F 2 "" H 6300 5400 50  0001 C CNN
F 3 "" H 6300 5400 50  0001 C CNN
	1    6300 5400
	1    0    0    -1  
$EndComp
Wire Wire Line
	6000 5350 6000 5400
Wire Wire Line
	6300 5350 6300 5400
$Comp
L LED D1
U 1 1 62BEC926
P 3350 5100
F 0 "D1" H 3200 4950 50  0000 C CNN
F 1 "LED" H 3350 4950 50  0000 C CNN
F 2 "" H 3350 5100 50  0001 C CNN
F 3 "" H 3350 5100 50  0001 C CNN
	1    3350 5100
	1    0    0    1   
$EndComp
$Comp
L R_ R1
U 1 1 62BECA1B
P 3850 5100
F 0 "R1" V 3750 5000 60  0000 C CNN
F 1 "300" V 3750 5200 60  0000 C CNN
F 2 "" H 3650 5350 60  0000 C CNN
F 3 "" H 3650 5350 60  0000 C CNN
	1    3850 5100
	0    1    1    0   
$EndComp
$Comp
L SWITCH S3
U 1 1 62BECE28
P 6500 2800
F 0 "S3" H 6550 2650 60  0000 C CNN
F 1 "Reset" H 6350 2650 60  0000 C CNN
F 2 "" H 6500 2800 60  0000 C CNN
F 3 "" H 6500 2800 60  0000 C CNN
	1    6500 2800
	-1   0    0    -1  
$EndComp
Wire Wire Line
	6400 3100 5900 3100
$Comp
L CONN_STLINK J1
U 1 1 62BEF938
P 5400 1500
F 0 "J1" H 5150 1650 60  0000 C CNN
F 1 "ST-LINK" H 5450 1650 60  0000 C CNN
F 2 "" H 5250 1450 60  0000 C CNN
F 3 "" H 5250 1450 60  0000 C CNN
	1    5400 1500
	1    0    0    -1  
$EndComp
$Comp
L SWITCH S1
U 1 1 62BF05BE
P 6500 2400
F 0 "S1" H 6500 2250 60  0000 C CNN
F 1 "Reset" H 6500 2250 60  0001 C CNN
F 2 "" H 6500 2400 60  0000 C CNN
F 3 "" H 6500 2400 60  0000 C CNN
	1    6500 2400
	-1   0    0    -1  
$EndComp
$Comp
L LED D2
U 1 1 62E90C6A
P 3350 5400
F 0 "D2" H 3200 5250 50  0000 C CNN
F 1 "LED" H 3350 5250 50  0000 C CNN
F 2 "" H 3350 5400 50  0001 C CNN
F 3 "" H 3350 5400 50  0001 C CNN
	1    3350 5400
	1    0    0    1   
$EndComp
$Comp
L R_ R2
U 1 1 62E90C70
P 3850 5400
F 0 "R2" V 3750 5300 60  0000 C CNN
F 1 "300" V 3750 5500 60  0000 C CNN
F 2 "" H 3650 5650 60  0000 C CNN
F 3 "" H 3650 5650 60  0000 C CNN
	1    3850 5400
	0    1    1    0   
$EndComp
$Comp
L GND #PWR?
U 1 1 62E90C76
P 3000 5500
F 0 "#PWR?" H 3000 5250 50  0001 C CNN
F 1 "GND" H 3000 5350 50  0000 C CNN
F 2 "" H 3000 5500 50  0001 C CNN
F 3 "" H 3000 5500 50  0001 C CNN
	1    3000 5500
	1    0    0    -1  
$EndComp
$Comp
L Conn_01x03 J2
U 1 1 62E90E01
P 3400 4600
F 0 "J2" H 3350 4850 50  0000 C CNN
F 1 "UART (3.3V)" H 3650 4850 50  0000 C CNN
F 2 "" H 3400 4600 50  0001 C CNN
F 3 "" H 3400 4600 50  0001 C CNN
	1    3400 4600
	-1   0    0    -1  
$EndComp
Text Notes 3100 4650 0    60   ~ 0
RXD
Text Notes 3100 4550 0    60   ~ 0
TXD
Text Notes 3100 4750 0    60   ~ 0
GND
$Comp
L CRYSTAL1 Z1
U 1 1 62F10190
P 7000 3950
F 0 "Z1" H 6750 4150 60  0000 C CNN
F 1 "12 MHz" H 6800 4050 60  0000 C CNN
F 2 "" H 7000 3950 60  0000 C CNN
F 3 "" H 7000 3950 60  0000 C CNN
	1    7000 3950
	-1   0    0    -1  
$EndComp
$Comp
L C_ C6
U 1 1 62F101FA
P 6900 3900
F 0 "C6" H 7200 3500 60  0000 C CNN
F 1 "56" H 7050 3500 60  0000 C CNN
F 2 "" H 6900 3900 60  0000 C CNN
F 3 "" H 6900 3900 60  0000 C CNN
	1    6900 3900
	-1   0    0    -1  
$EndComp
$Comp
L C_ C5
U 1 1 62F10274
P 6900 3650
F 0 "C5" H 7200 3700 60  0000 C CNN
F 1 "56" H 7050 3700 60  0000 C CNN
F 2 "" H 6900 3650 60  0000 C CNN
F 3 "" H 6900 3650 60  0000 C CNN
	1    6900 3650
	-1   0    0    -1  
$EndComp
Wire Wire Line
	7000 3700 7000 3850
Wire Wire Line
	5900 3700 7000 3700
Wire Wire Line
	7000 4200 7000 4050
Wire Wire Line
	6700 3900 6700 4000
$Comp
L GND #PWR?
U 1 1 62F103E5
P 6600 3950
F 0 "#PWR?" H 6600 3700 50  0001 C CNN
F 1 "GND" H 6600 3800 50  0000 C CNN
F 2 "" H 6600 3950 50  0001 C CNN
F 3 "" H 6600 3950 50  0001 C CNN
	1    6600 3950
	0    1    -1   0   
$EndComp
Wire Wire Line
	6300 4200 7000 4200
Wire Wire Line
	6600 3950 6700 3950
Connection ~ 6700 3950
Wire Wire Line
	6700 3750 6700 3700
Connection ~ 6700 3700
Wire Wire Line
	6700 4200 6700 4150
Connection ~ 6700 4200
$Comp
L MG32F02A064_LQFP48 U1
U 1 1 62F7770C
P 5150 2400
F 0 "U1" H 5100 2600 60  0000 C CNN
F 1 "MG32F02A064_LQFP48" H 5150 2500 60  0000 C CNN
F 2 "" H 5000 1400 60  0001 C CNN
F 3 "" H 5000 1400 60  0001 C CNN
	1    5150 2400
	1    0    0    -1  
$EndComp
Wire Wire Line
	3700 5400 3500 5400
Wire Wire Line
	3200 5400 3000 5400
Wire Wire Line
	3000 5100 3000 5500
Wire Wire Line
	3700 5100 3500 5100
Wire Wire Line
	3200 5100 3000 5100
Connection ~ 3000 5400
Wire Wire Line
	4400 4900 4100 4900
Wire Wire Line
	4100 4900 4100 5100
Wire Wire Line
	4100 5100 4000 5100
Wire Wire Line
	4400 5000 4200 5000
Wire Wire Line
	4200 5000 4200 5400
Wire Wire Line
	4200 5400 4000 5400
Wire Wire Line
	4400 4500 3600 4500
Wire Wire Line
	4400 4600 3600 4600
$Comp
L GND #PWR?
U 1 1 62F78688
P 3650 4750
F 0 "#PWR?" H 3650 4500 50  0001 C CNN
F 1 "GND" H 3650 4600 50  0000 C CNN
F 2 "" H 3650 4750 50  0001 C CNN
F 3 "" H 3650 4750 50  0001 C CNN
	1    3650 4750
	1    0    0    -1  
$EndComp
Wire Wire Line
	3650 4750 3650 4700
Wire Wire Line
	3650 4700 3600 4700
Wire Wire Line
	6300 4200 6300 3800
Wire Wire Line
	6300 3800 5900 3800
Wire Wire Line
	5750 1700 6000 1700
Wire Wire Line
	6000 1700 6000 2900
Wire Wire Line
	6000 2900 5900 2900
Wire Wire Line
	5750 1500 6100 1500
Wire Wire Line
	6100 1500 6100 3000
Wire Wire Line
	6100 3000 5900 3000
Wire Wire Line
	6300 5000 5900 5000
$Comp
L GND #PWR?
U 1 1 62E90EFA
P 6600 5400
F 0 "#PWR?" H 6600 5150 50  0001 C CNN
F 1 "GND" H 6600 5250 50  0000 C CNN
F 2 "" H 6600 5400 50  0001 C CNN
F 3 "" H 6600 5400 50  0001 C CNN
	1    6600 5400
	1    0    0    -1  
$EndComp
Wire Wire Line
	5900 4900 6600 4900
Wire Wire Line
	6600 4900 6600 5400
$Comp
L GND #PWR?
U 1 1 62F79527
P 6300 1700
F 0 "#PWR?" H 6300 1450 50  0001 C CNN
F 1 "GND" H 6300 1550 50  0000 C CNN
F 2 "" H 6300 1700 50  0001 C CNN
F 3 "" H 6300 1700 50  0001 C CNN
	1    6300 1700
	1    0    0    -1  
$EndComp
Wire Wire Line
	6300 1700 6300 1600
Wire Wire Line
	6300 1600 5750 1600
Wire Wire Line
	5750 1800 5900 1800
Wire Wire Line
	5900 1800 5900 2100
Wire Wire Line
	5900 2100 2600 2100
Wire Wire Line
	4300 2500 4400 2500
Wire Wire Line
	4300 2500 4300 2100
Connection ~ 4300 2100
$Comp
L SWITCH S2
U 1 1 62F79E01
P 6500 2600
F 0 "S2" H 6500 2450 60  0000 C CNN
F 1 "Reset" H 6500 2450 60  0001 C CNN
F 2 "" H 6500 2600 60  0000 C CNN
F 3 "" H 6500 2600 60  0000 C CNN
	1    6500 2600
	-1   0    0    -1  
$EndComp
Wire Wire Line
	6400 2700 5900 2700
Wire Wire Line
	5900 2800 6300 2800
Wire Wire Line
	6300 2800 6300 2900
Wire Wire Line
	6300 2900 6400 2900
Wire Wire Line
	6600 2700 6800 2700
Wire Wire Line
	6800 2700 6800 3200
Wire Wire Line
	6600 2900 6800 2900
Connection ~ 6800 2900
Wire Wire Line
	6600 3100 6800 3100
Connection ~ 6800 3100
$Comp
L GND #PWR?
U 1 1 62F7A144
P 6800 3200
F 0 "#PWR?" H 6800 2950 50  0001 C CNN
F 1 "GND" H 6800 3050 50  0000 C CNN
F 2 "" H 6800 3200 50  0001 C CNN
F 3 "" H 6800 3200 50  0001 C CNN
	1    6800 3200
	1    0    0    -1  
$EndComp
$Comp
L ADR45XX U2
U 1 1 62F78ADB
P 3050 2500
F 0 "U2" H 2900 2650 60  0000 C CNN
F 1 "ADR4530" H 3050 2550 60  0000 C CNN
F 2 "" H 3050 2500 60  0001 C CNN
F 3 "" H 3050 2500 60  0001 C CNN
	1    3050 2500
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR?
U 1 1 62F78BFA
P 2600 2800
F 0 "#PWR?" H 2600 2550 50  0001 C CNN
F 1 "GND" H 2600 2650 50  0000 C CNN
F 2 "" H 2600 2800 50  0001 C CNN
F 3 "" H 2600 2800 50  0001 C CNN
	1    2600 2800
	1    0    0    -1  
$EndComp
Wire Wire Line
	2600 2800 2600 2700
Wire Wire Line
	2600 2700 2700 2700
Wire Wire Line
	3400 2600 4400 2600
Wire Wire Line
	2600 2100 2600 2600
Wire Wire Line
	2600 2600 2700 2600
$Comp
L C_ C7
U 1 1 62F78E1C
P 3600 2600
F 0 "C7" H 3650 2450 60  0000 C CNN
F 1 "0,1" H 3650 2350 60  0000 C CNN
F 2 "" H 3600 2600 60  0000 C CNN
F 3 "" H 3600 2600 60  0000 C CNN
	1    3600 2600
	1    0    0    -1  
$EndComp
Wire Wire Line
	3800 2700 3800 2600
Connection ~ 3800 2600
$Comp
L GND #PWR?
U 1 1 62F78F20
P 3800 2900
F 0 "#PWR?" H 3800 2650 50  0001 C CNN
F 1 "GND" H 3800 2750 50  0000 C CNN
F 2 "" H 3800 2900 50  0001 C CNN
F 3 "" H 3800 2900 50  0001 C CNN
	1    3800 2900
	1    0    0    -1  
$EndComp
Wire Wire Line
	3800 2900 3800 2850
$Comp
L Conn_01x02 J3
U 1 1 62F79229
P 3400 3200
F 0 "J3" H 3500 3300 50  0000 C CNN
F 1 "ADC" H 3300 3300 50  0000 C CNN
F 2 "" H 3400 3200 50  0001 C CNN
F 3 "" H 3400 3200 50  0001 C CNN
	1    3400 3200
	-1   0    0    -1  
$EndComp
Wire Wire Line
	4400 3200 3600 3200
$Comp
L GND #PWR?
U 1 1 62F7934A
P 3700 3400
F 0 "#PWR?" H 3700 3150 50  0001 C CNN
F 1 "GND" H 3700 3250 50  0000 C CNN
F 2 "" H 3700 3400 50  0001 C CNN
F 3 "" H 3700 3400 50  0001 C CNN
	1    3700 3400
	1    0    0    -1  
$EndComp
Wire Wire Line
	3700 3400 3700 3300
Wire Wire Line
	3700 3300 3600 3300
Wire Wire Line
	6000 5200 6000 5000
Connection ~ 6000 5000
$EndSCHEMATC
