# SensorTemp

Description:
Reads temperature from a TMP36 sensor, tracks the hottest and coldest values of the day (°F), and displays temperature ranges with 3 LEDs (°C bands).

Features:

Converts analog readings → voltage → °C and °F
Tracks min/max in °F
LED indicators show temperature zones based on °C thresholds
Prints live readings + min/max to Serial Monitor
Hardware Requirements:

Arduino Uno (or similar)
TMP36 temperature sensor
3 LEDs + 220Ω resistors
Breadboard + jumper wires


Wiring:

ARDUINO | | - 5V | - GND | | - pin#2 - 220 Ohm Res - G LED | | - pin#3 - 220 Ohm Res - Y LED | | - pin#4 - 220 Ohm Res - R LED | | - A0 - GND - 5V - Temp Sensor
