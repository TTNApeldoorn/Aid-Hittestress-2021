EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title "Hitts stress 2021 node"
Date "13-1-2021"
Rev "001"
Comp "TTN-Apeldoorn (RFsee)"
Comment1 "Editor: Remko Welling (remko@rfsee.nl)"
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L HittStress21:IRM-03 PSU2
U 1 1 5FFF7A4A
P 3100 2000
F 0 "PSU2" H 3100 2365 50  0000 C CNN
F 1 "IRM-03" H 3100 2274 50  0000 C CNN
F 2 "hitteStress21-pcb:IRM-03" H 3100 2050 50  0001 C CNN
F 3 "" H 3100 2050 50  0001 C CNN
	1    3100 2000
	1    0    0    -1  
$EndComp
$Comp
L HittStress21:MINIDCDCBOOST5V PSU1
U 1 1 5FFF80A3
P 1950 5250
F 0 "PSU1" H 1950 5665 50  0000 C CNN
F 1 "MINIDCDCBOOST5V" H 1950 5574 50  0000 C CNN
F 2 "hitteStress21-pcb:MINIDCBOOST5V" H 1950 5250 50  0001 C CNN
F 3 "" H 1950 5250 50  0001 C CNN
	1    1950 5250
	1    0    0    -1  
$EndComp
$Comp
L Connector:Conn_01x02_Female J1
U 1 1 5FFF8703
P 1500 2050
F 0 "J1" H 1392 1725 50  0000 C CNN
F 1 "Conn_01x02_Female" H 1392 1816 50  0000 C CNN
F 2 "Connector_Phoenix_MC_HighVoltage:PhoenixContact_MCV_1,5_2-G-5.08_1x02_P5.08mm_Vertical" H 1500 2050 50  0001 C CNN
F 3 "~" H 1500 2050 50  0001 C CNN
	1    1500 2050
	-1   0    0    1   
$EndComp
$Comp
L SparkFun-PowerSymbols:GND #GND04
U 1 1 5FFFEB8C
P 5350 3100
F 0 "#GND04" H 5400 3050 45  0001 L BNN
F 1 "GND" H 5350 2930 45  0000 C CNN
F 2 "" H 5350 3000 60  0001 C CNN
F 3 "" H 5350 3000 60  0001 C CNN
	1    5350 3100
	1    0    0    -1  
$EndComp
$Comp
L SparkFun-PowerSymbols:GND #GND06
U 1 1 5FFFF63E
P 7550 3100
F 0 "#GND06" H 7600 3050 45  0001 L BNN
F 1 "GND" H 7550 2930 45  0000 C CNN
F 2 "" H 7550 3000 60  0001 C CNN
F 3 "" H 7550 3000 60  0001 C CNN
	1    7550 3100
	1    0    0    -1  
$EndComp
Wire Wire Line
	6950 2850 7550 2850
Wire Wire Line
	7550 2850 7550 3100
Wire Wire Line
	5950 2850 5350 2850
Wire Wire Line
	5350 2850 5350 3100
Wire Wire Line
	5350 2850 5350 2450
Wire Wire Line
	5350 2450 5950 2450
Connection ~ 5350 2850
Wire Wire Line
	5950 2650 5450 2650
Wire Wire Line
	1550 5250 1100 5250
Text Label 1150 5250 0    50   ~ 0
3V3
$Comp
L SparkFun-PowerSymbols:GND #GND01
U 1 1 60000A77
P 1950 5800
F 0 "#GND01" H 2000 5750 45  0001 L BNN
F 1 "GND" H 1950 5630 45  0000 C CNN
F 2 "" H 1950 5700 60  0001 C CNN
F 3 "" H 1950 5700 60  0001 C CNN
	1    1950 5800
	1    0    0    -1  
$EndComp
Wire Wire Line
	1950 5600 1950 5800
Wire Wire Line
	2350 5250 2800 5250
Text Label 2550 5250 0    50   ~ 0
5V_EXT
$Comp
L Device:Thermistor TH1
U 1 1 60001704
P 2300 1900
F 0 "TH1" V 2058 1900 50  0000 C CNN
F 1 "Thermistor" V 2149 1900 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P7.62mm_Horizontal" H 2300 1900 50  0001 C CNN
F 3 "~" H 2300 1900 50  0001 C CNN
	1    2300 1900
	0    1    1    0   
$EndComp
Wire Wire Line
	1700 1950 1850 1950
Wire Wire Line
	1850 1950 1850 1900
Wire Wire Line
	1850 1900 2100 1900
Wire Wire Line
	2500 1900 2750 1900
Wire Wire Line
	2750 2100 1850 2100
Wire Wire Line
	1850 2100 1850 2050
Wire Wire Line
	1850 2050 1700 2050
Wire Wire Line
	3450 1900 3900 1900
Wire Wire Line
	3450 2100 3600 2100
Wire Wire Line
	3600 2100 3600 2200
Text Label 3750 1900 0    50   ~ 0
5V
Wire Wire Line
	6950 2750 7450 2750
Text Label 7300 2750 0    50   ~ 0
5V
$Comp
L SparkFun-PowerSymbols:GND #GND02
U 1 1 60003428
P 3600 2200
F 0 "#GND02" H 3650 2150 45  0001 L BNN
F 1 "GND" H 3600 2030 45  0000 C CNN
F 2 "" H 3600 2100 60  0001 C CNN
F 3 "" H 3600 2100 60  0001 C CNN
	1    3600 2200
	1    0    0    -1  
$EndComp
Wire Wire Line
	6950 1950 7450 1950
Wire Wire Line
	6950 2050 7450 2050
Wire Wire Line
	6950 2150 7450 2150
Text Label 7000 1950 0    50   ~ 0
AM2305_DATA
Text Label 7000 2050 0    50   ~ 0
SDS011_RXD
Text Label 7000 2150 0    50   ~ 0
SDS011_TXD
Wire Wire Line
	5950 2550 5450 2550
Wire Wire Line
	5950 2150 5450 2150
Wire Wire Line
	5950 2050 5450 2050
Text Label 5500 2050 0    50   ~ 0
GPS_RXD
Text Label 5500 2150 0    50   ~ 0
GPS_TXD
Text Label 5500 2650 0    50   ~ 0
3V3
Text Label 5500 2550 0    50   ~ 0
3V3_EXT
$Comp
L HittStress21:HTCC-AB01 U1
U 1 1 60009C11
P 6450 2400
F 0 "U1" H 6450 3265 50  0000 C CNN
F 1 "HTCC-AB01" H 6450 3174 50  0000 C CNN
F 2 "hitteStress21-pcb:HTCC-AB01" H 6450 2450 50  0001 C CNN
F 3 "" H 6450 2450 50  0001 C CNN
	1    6450 2400
	1    0    0    -1  
$EndComp
Wire Wire Line
	5950 2750 5450 2750
Text Label 5500 2750 0    50   ~ 0
PV
$Comp
L Connector_Generic:Conn_01x05 J2
U 1 1 60011326
P 4750 5250
F 0 "J2" H 4830 5292 50  0000 L CNN
F 1 "Conn_01x05" H 4830 5201 50  0000 L CNN
F 2 "hitteStress21-pcb:JST_XH_S5B-XH-A_1x05_P2.50mm_Horizontal" H 4750 5250 50  0001 C CNN
F 3 "~" H 4750 5250 50  0001 C CNN
	1    4750 5250
	1    0    0    -1  
$EndComp
Wire Wire Line
	4550 5250 4400 5250
Wire Wire Line
	4400 5250 4400 5550
$Comp
L SparkFun-PowerSymbols:GND #GND03
U 1 1 60011F1B
P 4400 5550
F 0 "#GND03" H 4450 5500 45  0001 L BNN
F 1 "GND" H 4400 5380 45  0000 C CNN
F 2 "" H 4400 5450 60  0001 C CNN
F 3 "" H 4400 5450 60  0001 C CNN
	1    4400 5550
	1    0    0    -1  
$EndComp
Wire Wire Line
	4550 5350 4250 5350
Wire Wire Line
	4550 5150 4250 5150
Wire Wire Line
	4550 5050 4250 5050
$Comp
L Device:R R1
U 1 1 60014E2F
P 4100 5050
F 0 "R1" V 4000 4950 50  0000 C CNN
F 1 "1k" V 4000 5100 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P7.62mm_Horizontal" V 4030 5050 50  0001 C CNN
F 3 "~" H 4100 5050 50  0001 C CNN
	1    4100 5050
	0    1    1    0   
$EndComp
$Comp
L Device:R R2
U 1 1 6001542A
P 4100 5150
F 0 "R2" V 4200 5050 50  0000 C CNN
F 1 "1k" V 4200 5200 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P7.62mm_Horizontal" V 4030 5150 50  0001 C CNN
F 3 "~" H 4100 5150 50  0001 C CNN
	1    4100 5150
	0    1    1    0   
$EndComp
Wire Wire Line
	3950 5050 3400 5050
Wire Wire Line
	3950 5150 3400 5150
Text Label 4300 5050 0    50   ~ 0
PM_RX
Text Label 4300 5150 0    50   ~ 0
PM_TX
Text Label 3450 5350 0    50   ~ 0
5V_EXT
Text Label 3450 5050 0    50   ~ 0
SDS011_TXD
Text Label 3450 5150 0    50   ~ 0
SDS011_RXD
Text Notes 3700 4750 0    50   ~ 0
Particlesensor SDS011
$Comp
L Connector_Generic:Conn_01x03 J3
U 1 1 6002173D
P 7450 5150
F 0 "J3" H 7530 5192 50  0000 L CNN
F 1 "Conn_01x03" H 7530 5101 50  0000 L CNN
F 2 "hitteStress21-pcb:Wurth_2109_p2.54" H 7450 5150 50  0001 C CNN
F 3 "~" H 7450 5150 50  0001 C CNN
	1    7450 5150
	1    0    0    -1  
$EndComp
$Comp
L SparkFun-PowerSymbols:GND #GND05
U 1 1 60021D16
P 7150 5600
F 0 "#GND05" H 7200 5550 45  0001 L BNN
F 1 "GND" H 7150 5430 45  0000 C CNN
F 2 "" H 7150 5500 60  0001 C CNN
F 3 "" H 7150 5500 60  0001 C CNN
	1    7150 5600
	1    0    0    -1  
$EndComp
Wire Wire Line
	7250 5250 7150 5250
Wire Wire Line
	7150 5250 7150 5550
$Comp
L Device:R R4
U 1 1 60023777
P 6550 5250
F 0 "R4" V 6450 5150 50  0000 C CNN
F 1 "5k1" V 6450 5300 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P7.62mm_Horizontal" V 6480 5250 50  0001 C CNN
F 3 "~" H 6550 5250 50  0001 C CNN
	1    6550 5250
	0    -1   -1   0   
$EndComp
Wire Wire Line
	7250 5150 6300 5150
Wire Wire Line
	6300 5150 6300 5250
Wire Wire Line
	6300 5250 6400 5250
Wire Wire Line
	6300 5250 5700 5250
Connection ~ 6300 5250
Wire Wire Line
	7250 5050 6950 5050
Wire Wire Line
	6700 5250 6800 5250
Wire Wire Line
	6800 5250 6800 5050
Wire Wire Line
	6800 5050 5700 5050
Text Label 5750 5050 0    50   ~ 0
3V3
Text Label 5750 5250 0    50   ~ 0
AM2305_DATA
Text Notes 5750 4750 0    50   ~ 0
Temperature/Humidity sensor AM2305
Connection ~ 6800 5050
$Comp
L Device:C C1
U 1 1 60030A4D
P 6950 5350
F 0 "C1" H 6750 5250 50  0000 L CNN
F 1 "10u" H 6750 5150 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P5.08mm_Horizontal" H 6988 5200 50  0001 C CNN
F 3 "~" H 6950 5350 50  0001 C CNN
	1    6950 5350
	1    0    0    -1  
$EndComp
Wire Wire Line
	6950 5200 6950 5050
Connection ~ 6950 5050
Wire Wire Line
	6950 5050 6800 5050
Wire Wire Line
	6950 5500 6950 5550
Wire Wire Line
	6950 5550 7150 5550
Connection ~ 7150 5550
Wire Wire Line
	7150 5550 7150 5600
$Comp
L HittStress21:GPS U2
U 1 1 6003B0A4
P 9800 5350
F 0 "U2" H 10128 5396 50  0000 L CNN
F 1 "GPS" H 10128 5305 50  0000 L CNN
F 2 "hitteStress21-pcb:GPS" H 9800 5350 50  0001 C CNN
F 3 "" H 9800 5350 50  0001 C CNN
	1    9800 5350
	1    0    0    -1  
$EndComp
Wire Wire Line
	9250 5150 8500 5150
Wire Wire Line
	9250 5350 8500 5350
Wire Wire Line
	9250 5450 8500 5450
Wire Wire Line
	9250 5250 9100 5250
Wire Wire Line
	9100 5250 9100 5800
$Comp
L SparkFun-PowerSymbols:GND #GND07
U 1 1 6004350F
P 9100 5800
F 0 "#GND07" H 9150 5750 45  0001 L BNN
F 1 "GND" H 9100 5630 45  0000 C CNN
F 2 "" H 9100 5700 60  0001 C CNN
F 3 "" H 9100 5700 60  0001 C CNN
	1    9100 5800
	1    0    0    -1  
$EndComp
Text Label 8550 5150 0    50   ~ 0
3V3_EXT
Text Label 8550 5350 0    50   ~ 0
GPS_TXD
Text Label 8550 5450 0    50   ~ 0
GPS_RXD
Text Notes 9250 4750 0    50   ~ 0
GPS Receiver
$Comp
L Device:R R3
U 1 1 6004B202
P 4100 5350
F 0 "R3" V 4200 5250 50  0000 C CNN
F 1 "4R7" V 4200 5400 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P7.62mm_Horizontal" V 4030 5350 50  0001 C CNN
F 3 "~" H 4100 5350 50  0001 C CNN
	1    4100 5350
	0    1    1    0   
$EndComp
Wire Wire Line
	3950 5350 3400 5350
$Comp
L HittStress21:LiPo_3V7 B1
U 1 1 6003A431
P 9600 2450
F 0 "B1" H 9978 2496 50  0000 L CNN
F 1 "LiPo_3V7" H 9978 2405 50  0000 L CNN
F 2 "hitteStress21-pcb:LiPo_3V7_2000mAh_JST-PH" H 9600 2450 50  0001 C CNN
F 3 "" H 9600 2450 50  0001 C CNN
	1    9600 2450
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x02 J4
U 1 1 600437D4
P 4750 6450
F 0 "J4" H 4830 6442 50  0000 L CNN
F 1 "Conn_01x02" H 4830 6351 50  0000 L CNN
F 2 "hitteStress21-pcb:Wurth_2109_P2.54_2x1" H 4750 6450 50  0001 C CNN
F 3 "~" H 4750 6450 50  0001 C CNN
	1    4750 6450
	1    0    0    -1  
$EndComp
Wire Wire Line
	4550 6450 3400 6450
Wire Wire Line
	4550 6550 4400 6550
Wire Wire Line
	4400 6550 4400 6700
$Comp
L SparkFun-PowerSymbols:GND #GND0101
U 1 1 60046B99
P 4400 6700
F 0 "#GND0101" H 4450 6650 45  0001 L BNN
F 1 "GND" H 4400 6530 45  0000 C CNN
F 2 "" H 4400 6600 60  0001 C CNN
F 3 "" H 4400 6600 60  0001 C CNN
	1    4400 6700
	1    0    0    -1  
$EndComp
Text Label 3450 6450 0    50   ~ 0
PV
Text Notes 4000 6250 0    50   ~ 0
PV panel
$EndSCHEMATC
