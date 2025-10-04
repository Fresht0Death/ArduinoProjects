# Potentiometer LEDs

Description: Reads input from button and turns on and off blinking lights depending on the state of the button. Circuit is set up in series, parallel, and neither.

Features:

Press button → voltage let through → if button is not pressed two altnerating red LEDs go on and off back and forth for status "NULL" → if button is pressed a green LED is lit for status "GO" and red LEDs are paused:

Arduino Uno (or similar) 1 BUTTON + 3 LEDs + 4 220Ω resistors + Breadboard + jumper wires 

Wiring:

ARDUINO | | - 5V | - GND | | - pin#2 - 220 Ohm Res - Button | | - pin#3 - 220 Ohm Res - GREEN LED | | - pin#4 - 220 Ohm Res - RED LED | | - pin#5 - 220 Ohm Res - RED LED - GND - 5V - 
