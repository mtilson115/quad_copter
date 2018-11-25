EESchema Schematic File Version 4
EELAYER 26 0
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
L power:+BATT #PWR?
U 1 1 5BF9D457
P 2800 750
F 0 "#PWR?" H 2800 600 50  0001 C CNN
F 1 "+BATT" H 2815 923 50  0000 C CNN
F 2 "" H 2800 750 50  0001 C CNN
F 3 "" H 2800 750 50  0001 C CNN
	1    2800 750 
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 5BF9D4BC
P 2800 2800
F 0 "#PWR?" H 2800 2550 50  0001 C CNN
F 1 "GND" H 2805 2627 50  0000 C CNN
F 2 "" H 2800 2800 50  0001 C CNN
F 3 "" H 2800 2800 50  0001 C CNN
	1    2800 2800
	1    0    0    -1  
$EndComp
$Comp
L PIC32:PIC32MX775F512L-X_PT U?
U 1 1 5BF9DBC4
P 8850 3250
F 0 "U?" H 13650 3743 60  0000 C CNN
F 1 "PIC32MX795F512L-X_PT" H 13650 3637 60  0000 C CNN
F 2 "TQFP100_12x12MC" H 8850 3250 60  0001 C CNN
F 3 "http://ww1.microchip.com/downloads/en/devicedoc/60001156j.pdf" H 13650 3531 60  0000 C CNN
	1    8850 3250
	1    0    0    -1  
$EndComp
Text GLabel 8850 3350 0    50   Input ~ 0
VDD3.3
$Comp
L Regulator_Linear:LT1085-3.3 U?
U 1 1 5BFACA18
P 1800 1850
F 0 "U?" H 1800 2092 50  0000 C CNN
F 1 "LT1085-3.3" H 1800 2001 50  0000 C CNN
F 2 "" H 1800 2100 50  0001 C CIN
F 3 "http://cds.linear.com/docs/en/datasheet/1083ffe.pdf" H 1800 1850 50  0001 C CNN
	1    1800 1850
	1    0    0    -1  
$EndComp
Text GLabel 2300 1850 2    50   Input ~ 0
VDD3.3
$Comp
L Regulator_Linear:L7805 U?
U 1 1 5BFACD8D
P 3500 1850
F 0 "U?" H 3500 2092 50  0000 C CNN
F 1 "L7805" H 3500 2001 50  0000 C CNN
F 2 "" H 3525 1700 50  0001 L CIN
F 3 "http://www.st.com/content/ccc/resource/technical/document/datasheet/41/4f/b3/b0/12/d4/47/88/CD00000444.pdf/files/CD00000444.pdf/jcr:content/translations/en.CD00000444.pdf" H 3500 1800 50  0001 C CNN
	1    3500 1850
	1    0    0    -1  
$EndComp
Text GLabel 4100 1850 2    50   Input ~ 0
VDD5.0
Text GLabel 2800 2700 1    50   Input ~ 0
GND
Text GLabel 2800 1300 3    50   Input ~ 0
GND
$Comp
L Device:C C?
U 1 1 5BFAD563
P 2800 1000
F 0 "C?" H 2915 1046 50  0000 L CNN
F 1 "C" H 2915 955 50  0000 L CNN
F 2 "" H 2838 850 50  0001 C CNN
F 3 "~" H 2800 1000 50  0001 C CNN
	1    2800 1000
	1    0    0    -1  
$EndComp
$Comp
L Device:C C?
U 1 1 5BFAD5CA
P 2200 2000
F 0 "C?" H 2315 2046 50  0000 L CNN
F 1 "C" H 2315 1955 50  0000 L CNN
F 2 "" H 2238 1850 50  0001 C CNN
F 3 "~" H 2200 2000 50  0001 C CNN
	1    2200 2000
	1    0    0    -1  
$EndComp
$Comp
L Device:C C?
U 1 1 5BFAD6B3
P 3950 2000
F 0 "C?" H 4065 2046 50  0000 L CNN
F 1 "C" H 4065 1955 50  0000 L CNN
F 2 "" H 3988 1850 50  0001 C CNN
F 3 "~" H 3950 2000 50  0001 C CNN
	1    3950 2000
	1    0    0    -1  
$EndComp
Wire Wire Line
	2800 1150 2800 1300
Wire Wire Line
	2100 1850 2200 1850
Wire Wire Line
	2300 1850 2200 1850
Connection ~ 2200 1850
Text GLabel 8850 4750 0    50   Input ~ 0
VDD3.3
Wire Wire Line
	2800 2800 2800 2750
Wire Wire Line
	2800 750  2800 850 
Wire Wire Line
	2800 850  1500 850 
Wire Wire Line
	1500 850  1500 1850
Connection ~ 2800 850 
Wire Wire Line
	3200 850  2800 850 
Wire Wire Line
	3200 850  3200 1850
Wire Wire Line
	1800 2150 1800 2750
Wire Wire Line
	1800 2750 2200 2750
Connection ~ 2800 2750
Wire Wire Line
	2800 2750 2800 2700
Wire Wire Line
	3500 2150 3500 2750
Wire Wire Line
	3500 2750 2800 2750
Wire Wire Line
	2200 2150 2200 2750
Connection ~ 2200 2750
Wire Wire Line
	2200 2750 2800 2750
Wire Wire Line
	4100 1850 3950 1850
Wire Wire Line
	3800 1850 3950 1850
Connection ~ 3950 1850
Wire Wire Line
	3950 2150 3950 2750
Wire Wire Line
	3950 2750 3500 2750
Connection ~ 3500 2750
Text GLabel 8850 6150 0    50   Input ~ 0
VDD3.3
Text GLabel 8850 6850 0    50   Input ~ 0
VDD3.3
Text GLabel 8850 7750 0    50   Input ~ 0
VDD3.3
Text GLabel 8850 4650 0    50   Input ~ 0
GND
Text GLabel 8850 6250 0    50   Input ~ 0
GND
Text GLabel 8850 6750 0    50   Input ~ 0
GND
Text GLabel 8850 7650 0    50   Input ~ 0
GND
Text GLabel 18450 4650 2    50   Input ~ 0
VDD3.3
Text GLabel 18450 7050 2    50   Input ~ 0
VDD3.3
Text GLabel 18450 5750 2    50   Input ~ 0
GND
Text GLabel 18450 6750 2    50   Input ~ 0
GND
$EndSCHEMATC
