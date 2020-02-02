EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr C 22000 17000
encoding utf-8
Sheet 1 1
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L quad_copter_pcb-rescue:PIC32MX775F512L-X_PT-PIC32 U4
U 1 1 5BF9DBC4
P 6650 5050
F 0 "U4" H 11450 5543 60  0000 C CNN
F 1 "PIC32MX795F512L-X_PT" H 11450 5437 60  0000 C CNN
F 2 "Package_QFP:TQFP-100_12x12mm_P0.4mm" H 6650 5050 60  0001 C CNN
F 3 "https://www.microchip.com/wwwproducts/en/PIC32MX795F512L" H 11450 5331 60  0000 C CNN
	1    6650 5050
	1    0    0    -1  
$EndComp
Text GLabel 6050 5150 0    50   Input ~ 0
VDD3.3
$Comp
L Regulator_Linear:LT1085-3.3 U1
U 1 1 5BFACA18
P 1800 1850
F 0 "U1" H 1800 2092 50  0000 C CNN
F 1 "LT1085-3.3" H 1800 2001 50  0000 C CNN
F 2 "Package_TO_SOT_SMD:TO-263-3_TabPin2" H 1800 2100 50  0001 C CIN
F 3 "http://cds.linear.com/docs/en/datasheet/1083ffe.pdf" H 1800 1850 50  0001 C CNN
	1    1800 1850
	1    0    0    -1  
$EndComp
Text GLabel 2300 1850 2    50   Input ~ 0
VDD3.3
Text GLabel 2000 2750 3    50   Input ~ 0
GND
Text GLabel 2800 1300 3    50   Input ~ 0
GND
$Comp
L Device:C C1
U 1 1 5BFAD563
P 2800 1000
F 0 "C1" H 2915 1046 50  0000 L CNN
F 1 "C_10uF" H 2915 955 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric_Pad1.15x1.40mm_HandSolder" H 2838 850 50  0001 C CNN
F 3 "https://www.digikey.com/product-detail/en/tdk-corporation/C2012JB1V106M085AC/445-11518-6-ND/3957614" H 2800 1000 50  0001 C CNN
	1    2800 1000
	1    0    0    -1  
$EndComp
$Comp
L Device:C C2
U 1 1 5BFAD5CA
P 2200 2000
F 0 "C2" H 2315 2046 50  0000 L CNN
F 1 "C_22uF" H 2315 1955 50  0000 L CNN
F 2 "Capacitor_SMD:C_1206_3216Metric_Pad1.42x1.75mm_HandSolder" H 2238 1850 50  0001 C CNN
F 3 "https://www.digikey.com/product-detail/en/avx-corporation/TAJA226M010RNJ/478-5798-1-ND/2136389" H 2200 2000 50  0001 C CNN
	1    2200 2000
	1    0    0    -1  
$EndComp
Wire Wire Line
	2800 1150 2800 1300
Wire Wire Line
	2100 1850 2200 1850
Wire Wire Line
	2300 1850 2200 1850
Connection ~ 2200 1850
Text GLabel 5300 6550 0    50   Input ~ 0
VDD3.3
Wire Wire Line
	2800 750  2800 850 
Wire Wire Line
	2800 850  1500 850 
Wire Wire Line
	1500 850  1500 1850
Connection ~ 2800 850 
Wire Wire Line
	1800 2150 1800 2750
Wire Wire Line
	2200 2150 2200 2750
Text GLabel 5350 7950 0    50   Input ~ 0
VDD3.3
Text GLabel 5350 8850 0    50   Input ~ 0
VDD3.3
Text GLabel 5350 9550 0    50   Input ~ 0
VDD3.3
Text GLabel 6650 6450 0    50   Input ~ 0
GND
Text GLabel 6650 8050 0    50   Input ~ 0
GND
Text GLabel 6650 8550 0    50   Input ~ 0
GND
Text GLabel 6650 9450 0    50   Input ~ 0
GND
Text GLabel 17200 6450 2    50   Input ~ 0
VDD3.3
Text GLabel 17000 8850 2    50   Input ~ 0
VDD3.3
Text GLabel 16250 7550 2    50   Input ~ 0
GND
Text GLabel 16250 8550 2    50   Input ~ 0
GND
$Comp
L quad_copter_pcb-rescue:XBEE-SparkFun-RF JP2
U 1 1 5BFB1552
P 3850 12000
F 0 "JP2" H 3850 12760 45  0000 C CNN
F 1 "XBEE-LEAVE EMPTY" H 3850 12676 45  0000 C CNN
F 2 "RF:XBEE" H 3850 12600 20  0001 C CNN
F 3 "" H 3850 12000 50  0001 C CNN
F 4 "S6B" H 3850 12581 60  0000 C CNN "Field4"
	1    3850 12000
	1    0    0    -1  
$EndComp
$Comp
L quad_copter_pcb-rescue:accel_gyro_conn-accel_gyro U3
U 1 1 5BFB1E23
P 18900 12050
F 0 "U3" H 19427 11546 50  0000 L CNN
F 1 "accel_gyro_conn" H 19427 11455 50  0000 L CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_1x08_P2.54mm_Vertical" H 18900 12050 50  0001 C CNN
F 3 "Leave Empty" H 18900 12050 50  0001 C CNN
	1    18900 12050
	1    0    0    -1  
$EndComp
$Comp
L Connector:Conn_01x03_Male J4
U 1 1 5BFB2248
P 19250 9050
F 0 "J4" H 19400 9300 50  0000 R CNN
F 1 "Conn_01x03_Male" H 19750 9200 50  0000 R CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x03_P2.54mm_Vertical" H 19250 9050 50  0001 C CNN
F 3 "~" H 19250 9050 50  0001 C CNN
	1    19250 9050
	-1   0    0    1   
$EndComp
$Comp
L Connector:Conn_01x03_Male J3
U 1 1 5BFB232D
P 19250 8450
F 0 "J3" H 19450 8700 50  0000 R CNN
F 1 "Conn_01x03_Male" H 19700 8600 50  0000 R CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x03_P2.54mm_Vertical" H 19250 8450 50  0001 C CNN
F 3 "~" H 19250 8450 50  0001 C CNN
	1    19250 8450
	-1   0    0    1   
$EndComp
$Comp
L Connector:Conn_01x03_Male J2
U 1 1 5BFB24A5
P 19250 7900
F 0 "J2" H 19400 8200 50  0000 R CNN
F 1 "Conn_01x03_Male" H 19700 8100 50  0000 R CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x03_P2.54mm_Vertical" H 19250 7900 50  0001 C CNN
F 3 "~" H 19250 7900 50  0001 C CNN
	1    19250 7900
	-1   0    0    1   
$EndComp
$Comp
L Connector:Conn_01x03_Male J1
U 1 1 5BFB2634
P 19250 7350
F 0 "J1" H 19356 7628 50  0000 C CNN
F 1 "Conn_01x03_Male" H 19356 7537 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x03_P2.54mm_Vertical" H 19250 7350 50  0001 C CNN
F 3 "~" H 19250 7350 50  0001 C CNN
	1    19250 7350
	-1   0    0    1   
$EndComp
Text GLabel 19050 7250 0    50   Input ~ 0
GND
Text GLabel 19050 7800 0    50   Input ~ 0
GND
Text GLabel 19050 8350 0    50   Input ~ 0
GND
Text GLabel 19050 8950 0    50   Input ~ 0
GND
Text GLabel 19050 9050 0    50   Input ~ 0
VDD3.3
Text GLabel 19050 8450 0    50   Input ~ 0
VDD3.3
Text GLabel 19050 7900 0    50   Input ~ 0
VDD3.3
Text GLabel 19050 7350 0    50   Input ~ 0
VDD3.3
Wire Notes Line
	18600 7000 18600 9450
Wire Notes Line
	18600 9450 19550 9450
Wire Notes Line
	19550 9450 19550 7000
Wire Notes Line
	19550 7000 18600 7000
Text Notes 18650 6900 0    50   ~ 0
Motor PWM Connectors
Text GLabel 18600 12250 0    50   Input ~ 0
VDD3.3
Text GLabel 18600 12350 0    50   Input ~ 0
GND
Text GLabel 18600 12650 0    50   Input ~ 0
GND
Text GLabel 18600 12750 0    50   Input ~ 0
GND
Text GLabel 18600 12850 0    50   Input ~ 0
GND
Text GLabel 3150 11600 0    50   Input ~ 0
VDD3.3
$Comp
L quad_copter_pcb-rescue:MOMENTARY-SWITCH-SPST-2-SMD-5.2MM-SparkFun-Switches S1
U 1 1 5BFBCCDC
P 4600 3550
F 0 "S1" H 4600 3985 45  0000 C CNN
F 1 "MOMENTARY-SWITCH-SPST-2-SMD-5.2MM" H 4600 3901 45  0000 C CNN
F 2 "Switches:TACTILE_SWITCH_SMD_5.2MM" H 4600 3800 20  0001 C CNN
F 3 "https://www.digikey.com/product-detail/en/c-k/RS-187R05A2-DS-MT-RT/CKN10361TR-ND/2747186" H 4600 3550 50  0001 C CNN
F 4 "SWCH-08247" H 4600 3806 60  0000 C CNN "Field4"
	1    4600 3550
	1    0    0    -1  
$EndComp
Text GLabel 4400 3550 0    50   Input ~ 0
GND
Text GLabel 4400 3650 0    50   Input ~ 0
GND
$Comp
L Device:R R4
U 1 1 5BFBFD18
P 5450 3250
F 0 "R4" H 5520 3296 50  0000 L CNN
F 1 "R10k" H 5520 3205 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.15x1.40mm_HandSolder" V 5380 3250 50  0001 C CNN
F 3 "https://www.digikey.com/product-detail/en/susumu/RR1220P-103-D/RR12P10.0KDTR-ND/432315" H 5450 3250 50  0001 C CNN
	1    5450 3250
	1    0    0    -1  
$EndComp
Text GLabel 5450 3100 1    50   Input ~ 0
VDD3.3
Wire Wire Line
	4800 3450 4800 3550
Text GLabel 6650 6350 0    50   Input ~ 0
SS2_CSL_2
Text GLabel 6650 6050 0    50   Input ~ 0
SDI2_MISO_2
Text GLabel 6650 6150 0    50   Input ~ 0
SDO2_MOSI_2
Text GLabel 6650 5950 0    50   Input ~ 0
SCK2_SCLK_2
Text GLabel 4550 11900 2    50   Input ~ 0
SS2_CSL_2
Text GLabel 4550 11800 2    50   Input ~ 0
SCK2_SCLK_2
Text GLabel 4550 12500 2    50   Input ~ 0
SDO2_MOSI_2
Text GLabel 3150 11900 0    50   Input ~ 0
SDI2_MISO_2
Text GLabel 4550 11700 2    50   Input ~ 0
SPI_ATTN_INT_L
Text GLabel 6650 6850 0    50   Input ~ 0
SPI_ATTN_INT_L
Text GLabel 16250 9750 2    50   Input ~ 0
U1TX
Text GLabel 16250 9850 2    50   Input ~ 0
U1RX
Text GLabel 6650 9650 0    50   Input ~ 0
U1CTS
Text GLabel 6650 9750 0    50   Input ~ 0
U1RTS
Text GLabel 3150 12500 0    50   Input ~ 0
GND
Text GLabel 3150 11700 0    50   Input ~ 0
U1RX
Text GLabel 3150 11800 0    50   Input ~ 0
U1TX
Text GLabel 4550 12000 2    50   Input ~ 0
U1CTS
Text GLabel 4550 12400 2    50   Input ~ 0
U1RTS
Text GLabel 17700 8150 2    50   Input ~ 0
I2C1_SDA1
Text GLabel 17700 8400 2    50   Input ~ 0
I2C1_SCL1
Text GLabel 18600 12550 0    50   Input ~ 0
I2C1_SDA1
Text GLabel 18600 12450 0    50   Input ~ 0
I2C1_SCL1
Text GLabel 6650 6750 0    50   Input ~ 0
INT1
Text GLabel 18600 12950 0    50   Input ~ 0
INT1
$Comp
L Device:LED D3
U 1 1 5BFDB219
P 5450 12250
F 0 "D3" V 5488 12133 50  0000 R CNN
F 1 "LED" V 5397 12133 50  0000 R CNN
F 2 "LED_SMD:LED_1206_3216Metric_Pad1.42x1.75mm_HandSolder" H 5450 12250 50  0001 C CNN
F 3 "https://www.digikey.com/product-detail/en/lumex-opto-components-inc/SML-LXFT1206SGD-TR/SML-LXFT1206SGD-TR-ND/7364535" H 5450 12250 50  0001 C CNN
	1    5450 12250
	0    -1   -1   0   
$EndComp
Text GLabel 5450 12400 3    50   Input ~ 0
GND
$Comp
L Device:R R9
U 1 1 5BFDB324
P 5050 12100
F 0 "R9" V 4900 12200 50  0000 C CNN
F 1 "R_56" V 4934 12100 50  0000 C CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.15x1.40mm_HandSolder" V 4980 12100 50  0001 C CNN
F 3 "https://www.digikey.com/product-detail/en/stackpole-electronics-inc/RMCF0805JT56R0/RMCF0805JT56R0DKR-ND/2427421" H 5050 12100 50  0001 C CNN
	1    5050 12100
	0    1    1    0   
$EndComp
Wire Wire Line
	4550 12100 4900 12100
Wire Wire Line
	5200 12100 5450 12100
Text GLabel 3150 12000 0    50   Input ~ 0
XBEE_RESET
Text GLabel 6650 7150 0    50   Input ~ 0
XBEE_RESET
$Comp
L Device:R R5
U 1 1 5BFE39D2
P 5450 3850
F 0 "R5" H 5520 3896 50  0000 L CNN
F 1 "R470" H 5520 3805 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad1.05x0.95mm_HandSolder" V 5380 3850 50  0001 C CNN
F 3 "https://www.digikey.com/product-detail/en/bourns-inc/CR0603-JW-471ELF/CR0603-JW-471ELFCT-ND/3740990" H 5450 3850 50  0001 C CNN
	1    5450 3850
	1    0    0    -1  
$EndComp
Wire Wire Line
	5450 3400 5450 3450
Wire Wire Line
	5450 3450 4800 3450
Wire Wire Line
	5450 4000 5450 4150
Wire Wire Line
	5450 3700 4800 3700
Wire Wire Line
	4800 3700 4800 3650
Text GLabel 5200 4150 0    50   Input ~ 0
MCLR
Wire Wire Line
	5450 4150 5200 4150
Text GLabel 6650 6250 0    50   Input ~ 0
MCLR
$Comp
L quad_copter_pcb-rescue:RJ11-6-SparkFun-Connectors JP1
U 1 1 5BFEB677
P 5650 2000
F 0 "JP1" H 5606 2810 45  0000 C CNN
F 1 "RJ11-6" H 5606 2726 45  0000 C CNN
F 2 "Connectors:RJ11-6" H 5650 2700 20  0001 C CNN
F 3 "https://www.digikey.com/product-detail/en/te-connectivity-corcom-filters/RJ11-6N-B/CCM1394-ND/260247" H 5650 2000 50  0001 C CNN
F 4 "CONN-08590" H 5606 2631 60  0000 C CNN "Field4"
	1    5650 2000
	1    0    0    -1  
$EndComp
Text GLabel 5750 2000 2    50   Input ~ 0
MCLR
Text GLabel 5750 1900 2    50   Input ~ 0
VDD3.3
Text GLabel 5750 1800 2    50   Input ~ 0
GND
Text GLabel 5750 1700 2    50   Input ~ 0
PGD
Text GLabel 5750 1600 2    50   Input ~ 0
PGC
Text GLabel 6650 7350 0    50   Input ~ 0
PGC
Text GLabel 6650 7450 0    50   Input ~ 0
PGD
$Comp
L power:-BATT #PWR0103
U 1 1 5BFF0242
P 3550 750
F 0 "#PWR0103" H 3550 600 50  0001 C CNN
F 1 "-BATT" H 3565 923 50  0000 C CNN
F 2 "MountingHole:MountingHole_2.5mm_Pad" H 3550 750 50  0001 C CNN
F 3 "" H 3550 750 50  0001 C CNN
	1    3550 750 
	1    0    0    -1  
$EndComp
Text GLabel 3550 750  3    50   Input ~ 0
GND
$Comp
L Device:R R2
U 1 1 5BFF0734
P 7550 1350
F 0 "R2" H 7620 1396 50  0000 L CNN
F 1 "R10K" H 7620 1305 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.15x1.40mm_HandSolder" V 7480 1350 50  0001 C CNN
F 3 "https://www.digikey.com/product-detail/en/susumu/RR1220P-103-D/RR12P10.0KDTR-ND/432315" H 7550 1350 50  0001 C CNN
	1    7550 1350
	1    0    0    -1  
$EndComp
$Comp
L Device:R R3
U 1 1 5BFF084B
P 7550 1900
F 0 "R3" H 7620 1946 50  0000 L CNN
F 1 "R2.8k" H 7620 1855 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.15x1.40mm_HandSolder" V 7480 1900 50  0001 C CNN
F 3 "https://www.digikey.com/product-detail/en/yageo/RC0805FR-072K8L/311-2.80KCRTR-ND/727711" H 7550 1900 50  0001 C CNN
	1    7550 1900
	1    0    0    -1  
$EndComp
Text GLabel 7550 2050 3    50   Input ~ 0
GND
Wire Wire Line
	7550 1500 7550 1600
Text GLabel 2250 850  3    50   Input ~ 0
VBATT
Text GLabel 7550 1200 1    50   Input ~ 0
VBATT
Text GLabel 8050 1600 2    50   Input ~ 0
VBATT_SCALED
Wire Wire Line
	8050 1600 7550 1600
Connection ~ 7550 1600
Wire Wire Line
	7550 1600 7550 1750
Text GLabel 6650 7650 0    50   Input ~ 0
VBATT_SCALED
$Comp
L Device:R R8
U 1 1 5BFF6C32
P 2400 12100
F 0 "R8" V 2200 12100 50  0000 C CNN
F 1 "R_56" V 2284 12100 50  0000 C CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.15x1.40mm_HandSolder" V 2330 12100 50  0001 C CNN
F 3 "https://www.digikey.com/product-detail/en/stackpole-electronics-inc/RMCF0805JT56R0/RMCF0805JT56R0DKR-ND/2427421" H 2400 12100 50  0001 C CNN
	1    2400 12100
	0    1    1    0   
$EndComp
$Comp
L Device:LED D2
U 1 1 5BFF6CDA
P 2050 12400
F 0 "D2" V 2088 12283 50  0000 R CNN
F 1 "LED" V 1997 12283 50  0000 R CNN
F 2 "LED_SMD:LED_1206_3216Metric_Pad1.42x1.75mm_HandSolder" H 2050 12400 50  0001 C CNN
F 3 "https://www.digikey.com/product-detail/en/lumex-opto-components-inc/SML-LXFT1206SGD-TR/SML-LXFT1206SGD-TR-ND/7364535" H 2050 12400 50  0001 C CNN
	1    2050 12400
	0    -1   -1   0   
$EndComp
Text GLabel 2050 12700 3    50   Input ~ 0
GND
Wire Wire Line
	3150 12100 2550 12100
Wire Wire Line
	2250 12100 2050 12100
Wire Wire Line
	2050 12100 2050 12250
Wire Wire Line
	2050 12550 2050 12700
$Comp
L Device:R R1
U 1 1 5BFF93F4
P 900 2050
F 0 "R1" H 830 2004 50  0000 R CNN
F 1 "R_56" H 830 2095 50  0000 R CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.15x1.40mm_HandSolder" V 830 2050 50  0001 C CNN
F 3 "https://www.digikey.com/product-detail/en/stackpole-electronics-inc/RMCF0805JT56R0/RMCF0805JT56R0DKR-ND/2427421" H 900 2050 50  0001 C CNN
	1    900  2050
	-1   0    0    1   
$EndComp
$Comp
L power:GND #PWR0104
U 1 1 5BFF9620
P 900 2800
F 0 "#PWR0104" H 900 2550 50  0001 C CNN
F 1 "GND" H 905 2627 50  0000 C CNN
F 2 "" H 900 2800 50  0001 C CNN
F 3 "" H 900 2800 50  0001 C CNN
	1    900  2800
	1    0    0    -1  
$EndComp
$Comp
L Device:LED D1
U 1 1 5BFF96CD
P 900 2500
F 0 "D1" V 938 2383 50  0000 R CNN
F 1 "LED" V 847 2383 50  0000 R CNN
F 2 "LED_SMD:LED_1206_3216Metric_Pad1.42x1.75mm_HandSolder" H 900 2500 50  0001 C CNN
F 3 "https://www.digikey.com/product-detail/en/lumex-opto-components-inc/SML-LXFT1206SGD-TR/SML-LXFT1206SGD-TR-ND/7364535" H 900 2500 50  0001 C CNN
	1    900  2500
	0    -1   -1   0   
$EndComp
Text GLabel 900  1900 1    50   Input ~ 0
VDD3.3
Wire Wire Line
	900  2200 900  2350
Wire Wire Line
	900  2650 900  2800
$Comp
L Device:C C5
U 1 1 5BFFC7EE
P 12000 11400
F 0 "C5" H 12115 11446 50  0000 L CNN
F 1 "C26pF" H 12115 11355 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric_Pad1.05x0.95mm_HandSolder" H 12038 11250 50  0001 C CNN
F 3 "https://www.digikey.com/product-detail/en/avx-corporation/06031A250GAT2A/478-10197-2-ND/1598435" H 12000 11400 50  0001 C CNN
	1    12000 11400
	1    0    0    -1  
$EndComp
Text GLabel 12450 11150 2    50   Input ~ 0
GND
$Comp
L Device:C C4
U 1 1 5BFFC786
P 12000 10950
F 0 "C4" H 12115 10996 50  0000 L CNN
F 1 "C26pF" H 12115 10905 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric_Pad1.05x0.95mm_HandSolder" H 12038 10800 50  0001 C CNN
F 3 "https://www.digikey.com/product-detail/en/avx-corporation/06031A250GAT2A/478-10197-2-ND/1598435" H 12000 10950 50  0001 C CNN
	1    12000 10950
	1    0    0    -1  
$EndComp
Wire Wire Line
	10950 10800 10950 11000
Wire Wire Line
	10950 10800 12000 10800
Wire Wire Line
	12000 11550 10950 11550
Wire Wire Line
	10950 11550 10950 11300
Wire Wire Line
	12000 11250 12000 11150
Wire Wire Line
	12450 11150 12000 11150
Connection ~ 12000 11150
Wire Wire Line
	12000 11150 12000 11100
Text GLabel 16250 8650 2    50   Input ~ 0
OSC2_
Text GLabel 16250 8750 2    50   Input ~ 0
OSC1_
Text GLabel 10000 10800 0    50   Input ~ 0
OSC2_
Text GLabel 10000 11550 0    50   Input ~ 0
OSC1_
$Comp
L Mechanical:MountingHole H11
U 1 1 5C023C9A
P 10300 1350
F 0 "H11" H 10400 1396 50  0000 L CNN
F 1 "MountingHole" H 10400 1305 50  0000 L CNN
F 2 "MountingHole:MountingHole_3.2mm_M3" H 10300 1350 50  0001 C CNN
F 3 "~" H 10300 1350 50  0001 C CNN
	1    10300 1350
	1    0    0    -1  
$EndComp
$Comp
L Mechanical:MountingHole H13
U 1 1 5C023DDB
P 11450 1350
F 0 "H13" H 11550 1396 50  0000 L CNN
F 1 "MountingHole" H 11550 1305 50  0000 L CNN
F 2 "MountingHole:MountingHole_3.2mm_M3" H 11450 1350 50  0001 C CNN
F 3 "~" H 11450 1350 50  0001 C CNN
	1    11450 1350
	1    0    0    -1  
$EndComp
$Comp
L Mechanical:MountingHole H12
U 1 1 5C023E69
P 10300 1700
F 0 "H12" H 10400 1746 50  0000 L CNN
F 1 "MountingHole" H 10400 1655 50  0000 L CNN
F 2 "MountingHole:MountingHole_3.2mm_M3" H 10300 1700 50  0001 C CNN
F 3 "~" H 10300 1700 50  0001 C CNN
	1    10300 1700
	1    0    0    -1  
$EndComp
$Comp
L Mechanical:MountingHole H14
U 1 1 5C023EEB
P 11450 1700
F 0 "H14" H 11550 1746 50  0000 L CNN
F 1 "MountingHole" H 11550 1655 50  0000 L CNN
F 2 "MountingHole:MountingHole_3.2mm_M3" H 11450 1700 50  0001 C CNN
F 3 "~" H 11450 1700 50  0001 C CNN
	1    11450 1700
	1    0    0    -1  
$EndComp
Text GLabel 5750 1500 2    50   Input ~ 0
GND
$Comp
L power:+15V #PWR0101
U 1 1 5C04953B
P 2800 750
F 0 "#PWR0101" H 2800 600 50  0001 C CNN
F 1 "+15V" H 2815 923 50  0000 C CNN
F 2 "MountingHole:MountingHole_2.5mm_Pad" H 2800 750 50  0001 C CNN
F 3 "" H 2800 750 50  0001 C CNN
	1    2800 750 
	1    0    0    -1  
$EndComp
Text GLabel 16250 5650 2    50   Input ~ 0
RE1
Text GLabel 16250 5250 2    50   Input ~ 0
RE2
Text GLabel 16250 5150 2    50   Input ~ 0
RE3
Text GLabel 16250 5050 2    50   Input ~ 0
RE4
Text GLabel 16250 5750 2    50   Input ~ 0
RE0
Text GLabel 6650 5250 0    50   Input ~ 0
RE5
Text GLabel 6650 5350 0    50   Input ~ 0
RE6
Text GLabel 6650 5450 0    50   Input ~ 0
RE7
Text GLabel 1800 5450 2    50   Input ~ 0
SCK2_SCLK_2
Text GLabel 1800 5350 2    50   Input ~ 0
SDI2_MISO_2
Text GLabel 1250 7750 0    50   Input ~ 0
RE1
Text GLabel 1250 7850 0    50   Input ~ 0
RE3
Text GLabel 1250 7950 0    50   Input ~ 0
RE5
Text GLabel 1250 8050 0    50   Input ~ 0
RE7
Text GLabel 1750 7750 2    50   Input ~ 0
RE0
Text GLabel 1750 7850 2    50   Input ~ 0
RE2
Text GLabel 1750 7950 2    50   Input ~ 0
RE4
Text GLabel 1750 8050 2    50   Input ~ 0
RE6
Text GLabel 1800 5250 2    50   Input ~ 0
SDO2_MOSI_2
Text GLabel 1800 5150 2    50   Input ~ 0
SS2_CSL_2
Text GLabel 1800 5050 2    50   Input ~ 0
SPI_ATTN_INT_L
Text GLabel 1800 6250 2    50   Input ~ 0
I2C1_SDA1
Text GLabel 1300 6350 0    50   Input ~ 0
I2C1_SCL1
Text GLabel 16250 7850 2    50   Input ~ 0
MOTOR1_PWM
Text GLabel 19050 7450 0    50   Input ~ 0
MOTOR1_PWM
Text GLabel 16250 7450 2    50   Input ~ 0
MOTOR2_PWM
Text GLabel 19050 8000 0    50   Input ~ 0
MOTOR2_PWM
Text GLabel 19050 8550 0    50   Input ~ 0
MOTOR3_PWM
Text GLabel 16250 7350 2    50   Input ~ 0
MOTOR3_PWM
Text GLabel 16250 7250 2    50   Input ~ 0
MOTOR4_PWM
Text GLabel 19050 9150 0    50   Input ~ 0
MOTOR4_PWM
Text GLabel 3450 6850 0    50   Input ~ 0
VBATT_SCALED
Text GLabel 2500 4550 0    50   Input ~ 0
U1CTS
Text GLabel 3000 4650 2    50   Input ~ 0
U1TX
Text GLabel 3000 4550 2    50   Input ~ 0
U1RX
Text GLabel 2500 4650 0    50   Input ~ 0
U1RTS
$Comp
L Connector_Generic:Conn_02x05_Odd_Even J5
U 1 1 5C057C1B
P 1500 5250
F 0 "J5" H 1550 5667 50  0000 C CNN
F 1 "Conn_02x05_Odd_Even" H 1550 5576 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_2x05_P2.54mm_Vertical" H 1500 5250 50  0001 C CNN
F 3 "~" H 1500 5250 50  0001 C CNN
	1    1500 5250
	1    0    0    -1  
$EndComp
Text GLabel 1300 5050 0    50   Input ~ 0
SPI_ATTN_INT_L
Text GLabel 1300 5150 0    50   Input ~ 0
SS2_CSL_2
Text GLabel 1300 5250 0    50   Input ~ 0
SDO2_MOSI_2
Text GLabel 1300 5350 0    50   Input ~ 0
SDI2_MISO_2
Text GLabel 1300 5450 0    50   Input ~ 0
SCK2_SCLK_2
$Comp
L Connector_Generic:Conn_02x02_Odd_Even J6
U 1 1 5C0586A5
P 1500 6250
F 0 "J6" H 1550 6467 50  0000 C CNN
F 1 "Conn_02x02_Odd_Even" H 1550 6376 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_2x02_P2.54mm_Vertical" H 1500 6250 50  0001 C CNN
F 3 "~" H 1500 6250 50  0001 C CNN
	1    1500 6250
	1    0    0    -1  
$EndComp
Text GLabel 1300 6250 0    50   Input ~ 0
I2C1_SDA1
Text GLabel 1800 6350 2    50   Input ~ 0
I2C1_SCL1
$Comp
L Connector_Generic:Conn_02x04_Odd_Even J8
U 1 1 5C059B28
P 1450 7850
F 0 "J8" H 1500 8167 50  0000 C CNN
F 1 "Conn_02x04_Odd_Even" H 1500 8076 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_2x04_P2.54mm_Vertical" H 1450 7850 50  0001 C CNN
F 3 "~" H 1450 7850 50  0001 C CNN
	1    1450 7850
	1    0    0    -1  
$EndComp
Text GLabel 2500 4750 0    50   Input ~ 0
XBEE_RESET
$Comp
L Connector_Generic:Conn_01x05 J11
U 1 1 5C065D8F
P 3650 7050
F 0 "J11" H 3730 7092 50  0000 L CNN
F 1 "Conn_01x05" H 3730 7001 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x05_P2.54mm_Vertical" H 3650 7050 50  0001 C CNN
F 3 "~" H 3650 7050 50  0001 C CNN
	1    3650 7050
	1    0    0    -1  
$EndComp
Text GLabel 6650 9050 0    50   Input ~ 0
AN12
Text GLabel 6650 9150 0    50   Input ~ 0
AN13
Text GLabel 6650 9250 0    50   Input ~ 0
AN14
Text GLabel 6650 9350 0    50   Input ~ 0
AN15
Text GLabel 3450 6950 0    50   Input ~ 0
AN12
Text GLabel 3450 7050 0    50   Input ~ 0
AN13
Text GLabel 3450 7150 0    50   Input ~ 0
AN14
Text GLabel 3450 7250 0    50   Input ~ 0
AN15
$Comp
L Device:C C9
U 1 1 5C066ECD
P 6250 5000
F 0 "C9" H 6365 5046 50  0000 L CNN
F 1 "C_.1uF" H 6365 4955 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric_Pad1.05x0.95mm_HandSolder" H 6288 4850 50  0001 C CNN
F 3 "https://www.digikey.com/product-detail/en/samsung-electro-mechanics/CL10F104ZO8NNNC/1276-1258-6-ND/3891772" H 6250 5000 50  0001 C CNN
	1    6250 5000
	-1   0    0    1   
$EndComp
Wire Wire Line
	6050 5150 6250 5150
$Comp
L Device:C C6
U 1 1 5C069BC0
P 17000 6750
F 0 "C6" H 17115 6796 50  0000 L CNN
F 1 "C10uF" H 17115 6705 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric_Pad1.05x0.95mm_HandSolder" H 17038 6600 50  0001 C CNN
F 3 "https://www.digikey.com/product-detail/en/tdk-corporation/C1608X5R0J106M080AB/445-4112-2-ND/1975459" H 17000 6750 50  0001 C CNN
	1    17000 6750
	-1   0    0    1   
$EndComp
$Comp
L Device:C C8
U 1 1 5C069F9C
P 16850 6300
F 0 "C8" H 16965 6346 50  0000 L CNN
F 1 "C.1uF" H 16965 6255 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric_Pad1.05x0.95mm_HandSolder" H 16888 6150 50  0001 C CNN
F 3 "https://www.digikey.com/product-detail/en/samsung-electro-mechanics/CL10F104ZO8NNNC/1276-1258-6-ND/3891772" H 16850 6300 50  0001 C CNN
	1    16850 6300
	-1   0    0    1   
$EndComp
Text GLabel 17000 6900 3    50   Input ~ 0
GND
Text GLabel 16850 6150 1    50   Input ~ 0
GND
Wire Wire Line
	16250 6550 17000 6550
Wire Wire Line
	17000 6550 17000 6600
Wire Wire Line
	17200 6450 16850 6450
Wire Wire Line
	16850 6450 16250 6450
Connection ~ 16850 6450
$Comp
L Device:C C7
U 1 1 5C06DD01
P 16750 9000
F 0 "C7" H 16865 9046 50  0000 L CNN
F 1 "C.1uF" H 16865 8955 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric_Pad1.05x0.95mm_HandSolder" H 16788 8850 50  0001 C CNN
F 3 "https://www.digikey.com/product-detail/en/samsung-electro-mechanics/CL10F104ZO8NNNC/1276-1258-6-ND/3891772" H 16750 9000 50  0001 C CNN
	1    16750 9000
	-1   0    0    1   
$EndComp
Text GLabel 16750 9150 3    50   Input ~ 0
GND
Text GLabel 6250 4850 1    50   Input ~ 0
GND
Connection ~ 6250 5150
Wire Wire Line
	6250 5150 6650 5150
Text GLabel 6650 5050 0    50   Input ~ 0
GND
Text GLabel 6650 5550 0    50   Input ~ 0
GND
Text GLabel 6650 5650 0    50   Input ~ 0
GND
Text GLabel 6650 5750 0    50   Input ~ 0
GND
Text GLabel 6650 5850 0    50   Input ~ 0
GND
Text GLabel 6650 6650 0    50   Input ~ 0
GND
Text GLabel 6650 6950 0    50   Input ~ 0
GND
Text GLabel 6650 7050 0    50   Input ~ 0
GND
Text GLabel 6650 7250 0    50   Input ~ 0
GND
Text GLabel 6650 7550 0    50   Input ~ 0
GND
Text GLabel 6650 7750 0    50   Input ~ 0
GND
Text GLabel 6650 7850 0    50   Input ~ 0
GND
Text GLabel 6650 8150 0    50   Input ~ 0
GND
Text GLabel 6650 8250 0    50   Input ~ 0
GND
Text GLabel 6650 8350 0    50   Input ~ 0
GND
Text GLabel 6650 8450 0    50   Input ~ 0
GND
Text GLabel 6650 8750 0    50   Input ~ 0
GND
Text GLabel 6650 8850 0    50   Input ~ 0
GND
Text GLabel 6650 8950 0    50   Input ~ 0
GND
Text GLabel 6650 9850 0    50   Input ~ 0
GND
Text GLabel 6650 9950 0    50   Input ~ 0
GND
Text GLabel 16250 5350 2    50   Input ~ 0
GND
Text GLabel 16250 5450 2    50   Input ~ 0
GND
Text GLabel 16250 5550 2    50   Input ~ 0
GND
Text GLabel 16250 5850 2    50   Input ~ 0
GND
Text GLabel 16250 5950 2    50   Input ~ 0
GND
Text GLabel 16250 6050 2    50   Input ~ 0
GND
Text GLabel 16250 6150 2    50   Input ~ 0
GND
Text GLabel 16250 6250 2    50   Input ~ 0
GND
Text GLabel 16250 6350 2    50   Input ~ 0
GND
Text GLabel 16250 6650 2    50   Input ~ 0
GND
Text GLabel 16250 6750 2    50   Input ~ 0
GND
Text GLabel 16250 6850 2    50   Input ~ 0
GND
Text GLabel 16250 6950 2    50   Input ~ 0
GND
Text GLabel 16250 7050 2    50   Input ~ 0
GND
Text GLabel 16250 7150 2    50   Input ~ 0
GND
Text GLabel 16250 7650 2    50   Input ~ 0
SOSCO
Text GLabel 16250 7750 2    50   Input ~ 0
SOSCI
Text GLabel 16250 7950 2    50   Input ~ 0
GND
Text GLabel 16250 8050 2    50   Input ~ 0
GND
Text GLabel 16250 8150 2    50   Input ~ 0
GND
Text GLabel 16250 8250 2    50   Input ~ 0
GND
Text GLabel 16250 8950 2    50   Input ~ 0
GND
Wire Wire Line
	16250 8850 16750 8850
Wire Wire Line
	17000 8850 16750 8850
Connection ~ 16750 8850
Text GLabel 16250 9050 2    50   Input ~ 0
GND
Text GLabel 16250 9150 2    50   Input ~ 0
GND
Text GLabel 16250 9250 2    50   Input ~ 0
GND
$Comp
L Connector:USB_B_Micro J12
U 1 1 5C085DFE
P 18850 10100
F 0 "J12" H 18905 10567 50  0000 C CNN
F 1 "USB_B_Micro" H 18905 10476 50  0000 C CNN
F 2 "Connector_USB:USB_Micro-B_Amphenol_10103594-0001LF_Horizontal" H 19000 10050 50  0001 C CNN
F 3 "https://www.digikey.com/product-detail/en/amphenol-fci/10103594-0001LF/609-4050-1-ND/2350357" H 19000 10050 50  0001 C CNN
	1    18850 10100
	1    0    0    -1  
$EndComp
Text GLabel 16250 9350 2    50   Input ~ 0
USB_D+
Text GLabel 19150 10100 2    50   Input ~ 0
USB_D+
Text GLabel 19150 10200 2    50   Input ~ 0
USB_D-
Text GLabel 16250 9450 2    50   Input ~ 0
USB_D-
Text GLabel 19150 9900 2    50   Input ~ 0
USB_BUS
Text GLabel 16250 9650 2    50   Input ~ 0
USB_BUS
Text GLabel 19150 10300 2    50   Input ~ 0
USB_ID
Text GLabel 16250 9950 2    50   Input ~ 0
USB_ID
Text GLabel 18850 10500 3    50   Input ~ 0
GND
Text GLabel 18750 10500 3    50   Input ~ 0
GND
Text GLabel 16250 9550 2    50   Input ~ 0
VDD3.3
$Comp
L Connector_Generic:Conn_02x03_Odd_Even J10
U 1 1 5C08B6FC
P 2700 4650
F 0 "J10" H 2750 4967 50  0000 C CNN
F 1 "Conn_02x03_Odd_Even" H 2750 4876 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_2x03_P2.54mm_Vertical" H 2700 4650 50  0001 C CNN
F 3 "~" H 2700 4650 50  0001 C CNN
	1    2700 4650
	1    0    0    -1  
$EndComp
Text GLabel 3150 12400 0    50   Input ~ 0
RE0
Text GLabel 3150 12300 0    50   Input ~ 0
RE1
Text GLabel 3150 12200 0    50   Input ~ 0
RE2
Text GLabel 4550 11600 2    50   Input ~ 0
RE3
Text GLabel 4550 12200 2    50   Input ~ 0
RE4
Text GLabel 4550 12300 2    50   Input ~ 0
RE5
NoConn ~ 3000 4750
$Comp
L Device:R R12
U 1 1 5C096199
P 17250 7800
F 0 "R12" H 17180 7754 50  0000 R CNN
F 1 "R2.4k" H 17180 7845 50  0000 R CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.15x1.40mm_HandSolder" V 17180 7800 50  0001 C CNN
F 3 "https://www.digikey.com/product-detail/en/susumu/RR1220P-242-D/RR12P2.4KDTR-ND/432300" H 17250 7800 50  0001 C CNN
	1    17250 7800
	-1   0    0    1   
$EndComp
$Comp
L Device:R R13
U 1 1 5C096644
P 17550 8750
F 0 "R13" H 17480 8704 50  0000 R CNN
F 1 "R2.4k" H 17480 8795 50  0000 R CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.15x1.40mm_HandSolder" V 17480 8750 50  0001 C CNN
F 3 "https://www.digikey.com/product-detail/en/susumu/RR1220P-242-D/RR12P2.4KDTR-ND/432300" H 17550 8750 50  0001 C CNN
	1    17550 8750
	-1   0    0    1   
$EndComp
Wire Wire Line
	17250 8350 17250 8150
Wire Wire Line
	17250 8150 17700 8150
Wire Wire Line
	17250 7950 17250 8150
Connection ~ 17250 8150
Wire Wire Line
	16250 8350 17250 8350
Wire Wire Line
	16250 8450 17450 8450
Wire Wire Line
	17450 8450 17450 8400
Wire Wire Line
	17450 8400 17550 8400
Wire Wire Line
	17550 8600 17550 8400
Connection ~ 17550 8400
Wire Wire Line
	17550 8400 17700 8400
Text GLabel 17250 7650 1    50   Input ~ 0
VDD3.3
Text GLabel 17550 8900 3    50   Input ~ 0
VDD3.3
$Comp
L Device:C C10
U 1 1 5C042B3D
P 5600 6750
F 0 "C10" H 5715 6796 50  0000 L CNN
F 1 "C_.1uF" H 5715 6705 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric_Pad1.05x0.95mm_HandSolder" H 5638 6600 50  0001 C CNN
F 3 "https://www.digikey.com/product-detail/en/samsung-electro-mechanics/CL10F104ZO8NNNC/1276-1258-6-ND/3891772" H 5600 6750 50  0001 C CNN
	1    5600 6750
	-1   0    0    1   
$EndComp
Text GLabel 5600 6900 3    50   Input ~ 0
GND
Wire Wire Line
	5600 6550 5600 6600
Wire Wire Line
	5300 6550 5600 6550
Connection ~ 5600 6550
Wire Wire Line
	5600 6550 6650 6550
Wire Wire Line
	6650 7950 6000 7950
$Comp
L Device:C C11
U 1 1 5C04A259
P 5600 8250
F 0 "C11" H 5715 8296 50  0000 L CNN
F 1 "C.1uF" H 5715 8205 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric_Pad1.05x0.95mm_HandSolder" H 5638 8100 50  0001 C CNN
F 3 "https://www.digikey.com/product-detail/en/samsung-electro-mechanics/CL10F104ZO8NNNC/1276-1258-6-ND/3891772" H 5600 8250 50  0001 C CNN
	1    5600 8250
	-1   0    0    1   
$EndComp
Text GLabel 5600 8400 3    50   Input ~ 0
GND
Wire Wire Line
	5600 8100 5600 7950
Connection ~ 5600 7950
Wire Wire Line
	5600 7950 5350 7950
Wire Wire Line
	5350 9550 5600 9550
$Comp
L Device:C C13
U 1 1 5C04DC7E
P 5600 9800
F 0 "C13" H 5715 9846 50  0000 L CNN
F 1 "C.1uF" H 5715 9755 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric_Pad1.05x0.95mm_HandSolder" H 5638 9650 50  0001 C CNN
F 3 "https://www.digikey.com/product-detail/en/samsung-electro-mechanics/CL10F104ZO8NNNC/1276-1258-6-ND/3891772" H 5600 9800 50  0001 C CNN
	1    5600 9800
	-1   0    0    1   
$EndComp
Text GLabel 5600 10150 3    50   Input ~ 0
GND
$Comp
L Device:C C12
U 1 1 5C04DE81
P 5600 9050
F 0 "C12" H 5715 9096 50  0000 L CNN
F 1 "C.1uF" H 5715 9005 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric_Pad1.05x0.95mm_HandSolder" H 5638 8900 50  0001 C CNN
F 3 "https://www.digikey.com/product-detail/en/samsung-electro-mechanics/CL10F104ZO8NNNC/1276-1258-6-ND/3891772" H 5600 9050 50  0001 C CNN
	1    5600 9050
	-1   0    0    1   
$EndComp
Text GLabel 5600 9200 3    50   Input ~ 0
GND
Wire Wire Line
	5350 8850 5600 8850
Wire Wire Line
	6300 8850 6300 8650
Wire Wire Line
	6300 8650 6650 8650
Wire Wire Line
	5600 8900 5600 8850
Connection ~ 5600 8850
Wire Wire Line
	5600 8850 6000 8850
Wire Wire Line
	5600 9650 5600 9550
Connection ~ 5600 9550
Wire Wire Line
	5600 9550 6650 9550
Wire Wire Line
	5600 10150 5600 9950
$Comp
L Device:R R14
U 1 1 5C056B14
P 6000 8400
F 0 "R14" H 6070 8446 50  0000 L CNN
F 1 "R10" H 6070 8355 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.15x1.40mm_HandSolder" V 5930 8400 50  0001 C CNN
F 3 "https://www.digikey.com/product-detail/en/stackpole-electronics-inc/RNCS0805BKE10R0/RNCS0805BKE10R0TR-ND/1800064" H 6000 8400 50  0001 C CNN
	1    6000 8400
	1    0    0    -1  
$EndComp
Wire Wire Line
	6000 8250 6000 7950
Connection ~ 6000 7950
Wire Wire Line
	6000 7950 5600 7950
Wire Wire Line
	6000 8550 6000 8850
Connection ~ 6000 8850
Wire Wire Line
	6000 8850 6300 8850
Wire Wire Line
	1800 2750 2200 2750
Wire Wire Line
	10000 10800 10950 10800
Connection ~ 10950 10800
Wire Wire Line
	10000 11550 10950 11550
Connection ~ 10950 11550
$Comp
L Device:Crystal Y1
U 1 1 5C08CF7F
P 10950 11150
F 0 "Y1" V 10904 11281 50  0000 L CNN
F 1 "Crystal_8MHz" V 10995 11281 50  0000 L CNN
F 2 "Clocks:HC49UP" H 10950 11150 50  0001 C CNN
F 3 "https://www.digikey.com/product-detail/en/txc-corporation/9C-8.000MEEJ-T/887-1263-2-ND/2207683" H 10950 11150 50  0001 C CNN
	1    10950 11150
	0    1    1    0   
$EndComp
$Comp
L Mechanical:MountingHole H16
U 1 1 5E215BFB
P 12850 1700
F 0 "H16" H 12950 1746 50  0000 L CNN
F 1 "MountingHole" H 12950 1655 50  0000 L CNN
F 2 "MountingHole:MountingHole_3.2mm_M3" H 12850 1700 50  0001 C CNN
F 3 "~" H 12850 1700 50  0001 C CNN
	1    12850 1700
	1    0    0    -1  
$EndComp
Text GLabel 12600 1350 0    50   Input ~ 0
VBATT
Text GLabel 12600 1700 0    50   Input ~ 0
GND
Wire Wire Line
	12850 1350 12600 1350
Wire Wire Line
	12850 1700 12600 1700
$Comp
L Mechanical:MountingHole H15
U 1 1 5E215697
P 12850 1350
F 0 "H15" H 12950 1396 50  0000 L CNN
F 1 "MountingHole" H 12950 1305 50  0000 L CNN
F 2 "MountingHole:MountingHole_3.2mm_M3" H 12850 1350 50  0001 C CNN
F 3 "~" H 12850 1350 50  0001 C CNN
	1    12850 1350
	1    0    0    -1  
$EndComp
$EndSCHEMATC
