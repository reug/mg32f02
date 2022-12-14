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
LIBS:mg32f02_spi-cache
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
L SWITCH S2
U 1 1 62BECE28
P 6150 2800
F 0 "S2" H 6200 2650 60  0000 C CNN
F 1 "Reset" H 5950 2650 60  0000 C CNN
F 2 "" H 6150 2800 60  0000 C CNN
F 3 "" H 6150 2800 60  0000 C CNN
	1    6150 2800
	-1   0    0    -1  
$EndComp
$Comp
L CONN_STLINK J1
U 1 1 62BEF938
P 5350 1500
F 0 "J1" H 4900 1650 60  0000 C CNN
F 1 "J-LINK/ST-LINK" H 5400 1650 60  0000 C CNN
F 2 "" H 5200 1450 60  0000 C CNN
F 3 "" H 5200 1450 60  0000 C CNN
	1    5350 1500
	1    0    0    -1  
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
F 0 "Z1" H 7050 4350 60  0000 C CNN
F 1 "12 MHz" H 7000 4250 60  0000 C CNN
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
$Comp
L GND #PWR?
U 1 1 62F79527
P 5800 1600
F 0 "#PWR?" H 5800 1350 50  0001 C CNN
F 1 "GND" H 5800 1450 50  0000 C CNN
F 2 "" H 5800 1600 50  0001 C CNN
F 3 "" H 5800 1600 50  0001 C CNN
	1    5800 1600
	0    -1   -1   0   
$EndComp
$Comp
L GND #PWR?
U 1 1 62F7A144
P 6450 3100
F 0 "#PWR?" H 6450 2850 50  0001 C CNN
F 1 "GND" H 6450 2950 50  0000 C CNN
F 2 "" H 6450 3100 50  0001 C CNN
F 3 "" H 6450 3100 50  0001 C CNN
	1    6450 3100
	0    -1   -1   0   
$EndComp
$Comp
L MG32F02A032_TSSOP20 U2
U 1 1 634D001C
P 8950 2300
F 0 "U2" H 8900 2500 60  0000 C CNN
F 1 "MG32F02A032_TSSOP20" H 8950 2400 60  0000 C CNN
F 2 "" H 8800 2000 60  0001 C CNN
F 3 "" H 8800 2000 60  0001 C CNN
	1    8950 2300
	1    0    0    -1  
$EndComp
$Comp
L CRYSTAL1 Z2
U 1 1 634D07DE
P 7500 2250
F 0 "Z2" H 7250 2450 60  0000 C CNN
F 1 "12 MHz" H 7300 2350 60  0000 C CNN
F 2 "" H 7500 2250 60  0000 C CNN
F 3 "" H 7500 2250 60  0000 C CNN
	1    7500 2250
	1    0    0    -1  
$EndComp
$Comp
L C_ C8
U 1 1 634D07E4
P 7600 2200
F 0 "C8" H 7600 1800 60  0000 C CNN
F 1 "56" H 7750 1800 60  0000 C CNN
F 2 "" H 7600 2200 60  0000 C CNN
F 3 "" H 7600 2200 60  0000 C CNN
	1    7600 2200
	1    0    0    -1  
$EndComp
$Comp
L C_ C7
U 1 1 634D07EA
P 7600 1950
F 0 "C7" H 7700 2000 60  0000 C CNN
F 1 "51" H 7850 2000 60  0000 C CNN
F 2 "" H 7600 1950 60  0000 C CNN
F 3 "" H 7600 1950 60  0000 C CNN
	1    7600 1950
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR?
U 1 1 634D07F6
P 7900 2250
F 0 "#PWR?" H 7900 2000 50  0001 C CNN
F 1 "GND" H 7900 2100 50  0000 C CNN
F 2 "" H 7900 2250 50  0001 C CNN
F 3 "" H 7900 2250 50  0001 C CNN
	1    7900 2250
	0    -1   -1   0   
$EndComp
$Comp
L GND #PWR?
U 1 1 634D0DF7
P 8100 2600
F 0 "#PWR?" H 8100 2350 50  0001 C CNN
F 1 "GND" H 8100 2450 50  0000 C CNN
F 2 "" H 8100 2600 50  0001 C CNN
F 3 "" H 8100 2600 50  0001 C CNN
	1    8100 2600
	0    1    -1   0   
$EndComp
$Comp
L SWITCH S3
U 1 1 634D0FE2
P 10100 2100
F 0 "S3" H 10150 1950 60  0000 C CNN
F 1 "Reset" H 9950 1950 60  0000 C CNN
F 2 "" H 10100 2100 60  0000 C CNN
F 3 "" H 10100 2100 60  0000 C CNN
	1    10100 2100
	-1   0    0    -1  
$EndComp
$Comp
L GND #PWR?
U 1 1 634D0FEB
P 10300 2400
F 0 "#PWR?" H 10300 2150 50  0001 C CNN
F 1 "GND" H 10300 2250 50  0000 C CNN
F 2 "" H 10300 2400 50  0001 C CNN
F 3 "" H 10300 2400 50  0001 C CNN
	1    10300 2400
	0    -1   -1   0   
$EndComp
$Comp
L SWITCH_1CH S1.1
U 1 1 634D1534
P 6350 1300
F 0 "S1.1" H 6500 1200 60  0000 C CNN
F 1 " " H 5950 1300 60  0000 C CNN
F 2 "" H 6350 1300 60  0000 C CNN
F 3 "" H 6350 1300 60  0000 C CNN
	1    6350 1300
	-1   0    0    -1  
$EndComp
$Comp
L SWITCH_1CH S1.2
U 1 1 634D17BE
P 6350 1600
F 0 "S1.2" H 6500 1300 60  0000 C CNN
F 1 " " H 5950 1600 60  0000 C CNN
F 2 "" H 6350 1600 60  0000 C CNN
F 3 "" H 6350 1600 60  0000 C CNN
	1    6350 1600
	-1   0    0    -1  
$EndComp
$Comp
L +3.3V #PWR?
U 1 1 634D1E78
P 8100 2800
F 0 "#PWR?" H 8100 2650 50  0001 C CNN
F 1 "+3.3V" H 8100 2940 50  0000 C CNN
F 2 "" H 8100 2800 50  0001 C CNN
F 3 "" H 8100 2800 50  0001 C CNN
	1    8100 2800
	0    -1   -1   0   
$EndComp
$Comp
L C_P C9
U 1 1 634D23DC
P 7300 2700
F 0 "C9" H 7350 2550 60  0000 C CNN
F 1 "4,7" H 7350 2450 60  0000 C CNN
F 2 "" H 7300 2700 60  0000 C CNN
F 3 "" H 7300 2700 60  0000 C CNN
	1    7300 2700
	1    0    0    -1  
$EndComp
$Comp
L C_ C10
U 1 1 634D23E2
P 7600 2700
F 0 "C10" H 7650 2550 60  0000 C CNN
F 1 "0,1" H 7650 2450 60  0000 C CNN
F 2 "" H 7600 2700 60  0000 C CNN
F 3 "" H 7600 2700 60  0000 C CNN
	1    7600 2700
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR?
U 1 1 634D23E9
P 7500 3000
F 0 "#PWR?" H 7500 2750 50  0001 C CNN
F 1 "GND" H 7500 2850 50  0000 C CNN
F 2 "" H 7500 3000 50  0001 C CNN
F 3 "" H 7500 3000 50  0001 C CNN
	1    7500 3000
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR?
U 1 1 634D23EF
P 7800 3000
F 0 "#PWR?" H 7800 2750 50  0001 C CNN
F 1 "GND" H 7800 2850 50  0000 C CNN
F 2 "" H 7800 3000 50  0001 C CNN
F 3 "" H 7800 3000 50  0001 C CNN
	1    7800 3000
	1    0    0    -1  
$EndComp
Text Notes 10650 2750 2    60   ~ 0
RXD
Text Notes 10650 2650 2    60   ~ 0
TXD
Text Notes 10650 2850 2    60   ~ 0
GND
$Comp
L GND #PWR?
U 1 1 634D4CEC
P 10150 2800
F 0 "#PWR?" H 10150 2550 50  0001 C CNN
F 1 "GND" V 10200 2850 50  0000 C CNN
F 2 "" H 10150 2800 50  0001 C CNN
F 3 "" H 10150 2800 50  0001 C CNN
	1    10150 2800
	0    1    1    0   
$EndComp
$Comp
L Conn_01x03 J3
U 1 1 634D3429
P 10400 2700
F 0 "J3" H 10350 2400 50  0000 C CNN
F 1 "UART (3.3V)" H 10400 2300 50  0000 C CNN
F 2 "" H 10400 2700 50  0001 C CNN
F 3 "" H 10400 2700 50  0001 C CNN
	1    10400 2700
	1    0    0    -1  
$EndComp
Text Notes 5350 5250 2    60   ~ 0
(SPI Master)
Text Notes 9150 3550 2    60   ~ 0
(SPI Slave)
$Comp
L CONN_7_B J4
U 1 1 635D3586
P 3550 3550
F 0 "J4" H 3800 4250 60  0000 C CNN
F 1 "ENC28J60" H 3650 4150 60  0000 C CNN
F 2 "" H 3500 3550 60  0000 C CNN
F 3 "" H 3500 3550 60  0000 C CNN
	1    3550 3550
	-1   0    0    -1  
$EndComp
Text Notes 3600 3250 2    60   ~ 0
NSS
Text Notes 3600 3350 2    60   ~ 0
MISO
Text Notes 3600 3550 2    60   ~ 0
MOSI
Text Notes 3600 3450 2    60   ~ 0
SCK
Text Notes 3600 3650 2    60   ~ 0
INT
Text Notes 3600 3750 2    60   ~ 0
VCC
Text Notes 3600 3850 2    60   ~ 0
GND
$Comp
L GND #PWR?
U 1 1 635D4805
P 3900 3900
F 0 "#PWR?" H 3900 3650 50  0001 C CNN
F 1 "GND" H 3900 3750 50  0000 C CNN
F 2 "" H 3900 3900 50  0001 C CNN
F 3 "" H 3900 3900 50  0001 C CNN
	1    3900 3900
	1    0    0    -1  
$EndComp
$Comp
L +3.3V #PWR?
U 1 1 635D4904
P 4050 3700
F 0 "#PWR?" H 4050 3550 50  0001 C CNN
F 1 "+3.3V" V 3950 3700 50  0000 C CNN
F 2 "" H 4050 3700 50  0001 C CNN
F 3 "" H 4050 3700 50  0001 C CNN
	1    4050 3700
	0    1    -1   0   
$EndComp
$Comp
L CONN_7_B J5
U 1 1 636537E0
P 7550 4250
F 0 "J5" H 7800 4750 60  0000 C CNN
F 1 "SPI Slave" H 7450 4750 60  0000 C CNN
F 2 "" H 7500 4250 60  0000 C CNN
F 3 "" H 7500 4250 60  0000 C CNN
	1    7550 4250
	-1   0    0    -1  
$EndComp
Text Notes 7600 3950 2    60   ~ 0
NSS
Text Notes 7600 4050 2    60   ~ 0
MISO
Text Notes 7600 4250 2    60   ~ 0
MOSI
Text Notes 7600 4150 2    60   ~ 0
SCK
Text Notes 7600 4350 2    60   ~ 0
INT
Text Notes 7600 4450 2    60   ~ 0
VCC
Text Notes 7600 4550 2    60   ~ 0
GND
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
	6300 5000 6300 5200
Wire Wire Line
	6000 5350 6000 5400
Wire Wire Line
	6300 5350 6300 5400
Wire Wire Line
	6050 3100 5900 3100
Wire Wire Line
	7000 3700 7000 3850
Wire Wire Line
	5900 3700 7000 3700
Wire Wire Line
	7000 4200 7000 4050
Wire Wire Line
	6700 3900 6700 4000
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
Wire Wire Line
	3650 4750 3650 4700
Wire Wire Line
	3650 4700 3600 4700
Wire Wire Line
	6300 4200 6300 3800
Wire Wire Line
	6300 3800 5900 3800
Wire Wire Line
	6500 1900 6500 2900
Wire Wire Line
	6500 2900 5900 2900
Wire Wire Line
	6600 1600 6600 3000
Wire Wire Line
	6600 3000 5900 3000
Wire Wire Line
	5900 5000 6300 5000
Wire Wire Line
	5900 4900 6600 4900
Wire Wire Line
	6600 4900 6600 5400
Wire Wire Line
	4300 2500 4400 2500
Wire Wire Line
	4300 2100 4300 2500
Wire Wire Line
	6250 3100 6450 3100
Wire Wire Line
	6000 5200 6000 5000
Connection ~ 6000 5000
Wire Wire Line
	7500 2000 7500 2150
Wire Wire Line
	7500 2000 8100 2000
Wire Wire Line
	7500 2500 7500 2350
Wire Wire Line
	7800 2200 7800 2300
Wire Wire Line
	7500 2500 8200 2500
Wire Wire Line
	7900 2250 7800 2250
Connection ~ 7800 2250
Wire Wire Line
	7800 2050 7800 2000
Wire Wire Line
	7800 2500 7800 2450
Connection ~ 7800 2500
Wire Wire Line
	8200 2400 8100 2400
Wire Wire Line
	8100 2400 8100 2000
Connection ~ 7800 2000
Wire Wire Line
	8200 2600 8100 2600
Wire Wire Line
	10000 2400 9700 2400
Wire Wire Line
	10200 2400 10300 2400
Wire Wire Line
	5800 1600 5700 1600
Wire Wire Line
	5700 1500 6250 1500
Wire Wire Line
	6500 1900 6450 1900
Wire Wire Line
	6600 1600 6450 1600
Wire Wire Line
	9700 2500 9800 2500
Wire Wire Line
	9800 2500 9800 1400
Wire Wire Line
	9800 1400 6450 1400
Wire Wire Line
	9700 2600 9900 2600
Wire Wire Line
	9900 2600 9900 1700
Wire Wire Line
	9900 1700 6450 1700
Wire Wire Line
	5700 1700 6100 1700
Wire Wire Line
	6100 1700 6100 1800
Wire Wire Line
	6100 1800 6250 1800
Wire Wire Line
	7500 2950 7500 3000
Wire Wire Line
	7800 2950 7800 3000
Wire Wire Line
	7500 2800 7500 2700
Wire Wire Line
	7500 2700 8200 2700
Wire Wire Line
	7800 2800 7800 2700
Connection ~ 7800 2700
Wire Wire Line
	8200 2800 8100 2800
Wire Wire Line
	9700 2700 10200 2700
Wire Wire Line
	9700 2800 10000 2800
Wire Wire Line
	10200 2800 10150 2800
Wire Wire Line
	10200 2600 10000 2600
Wire Wire Line
	10000 2600 10000 2800
Wire Wire Line
	4400 3200 3800 3200
Wire Wire Line
	4400 3300 3800 3300
Wire Wire Line
	4400 3400 3800 3400
Wire Wire Line
	3800 3500 4400 3500
Wire Wire Line
	4400 3600 3800 3600
Wire Wire Line
	3900 3900 3900 3800
Wire Wire Line
	3900 3800 3800 3800
Wire Wire Line
	4050 3700 3800 3700
Wire Wire Line
	3800 2100 4300 2100
Wire Wire Line
	7800 4200 9800 4200
Wire Wire Line
	9800 4200 9800 3300
Wire Wire Line
	9800 3300 9700 3300
Wire Wire Line
	8200 3100 7900 3100
Wire Wire Line
	7900 3100 7900 3900
Wire Wire Line
	7900 3900 7800 3900
Wire Wire Line
	8200 3200 8000 3200
Wire Wire Line
	8000 3200 8000 4000
Wire Wire Line
	8000 4000 7800 4000
Wire Wire Line
	8200 3300 8100 3300
Wire Wire Line
	8100 3300 8100 4100
Wire Wire Line
	8100 4100 7800 4100
$EndSCHEMATC
