# Potentiometer LEDs

Description: Reads input from potentiometer and as knob is turned from start - finish indicators LEDs start coming on one by one until the max is reached where then a PWM LEDs rises and falls.

Features:

Turn potentiometer → voltage let through baed on how much the potentiometer is turned → 3 LEDs are activated sequentially as potentiometer reaches its max → once potentiometer max is reached the 3 LEDs are turned off and a BLUE LED is Pulse width modulated to create a rising and falling LED:

Arduino Uno (or similar) 1 Potentiometer + 4 LEDs + 4 220Ω resistors + Breadboard + jumper wires 

Wiring:

ARDUINO | | - 5V | - GND | | - A0 - 220 Ohm Res - Potentiometer | | - pin#2 - 220 Ohm Res - GREEN LED | | - pin#3 - 220 Ohm Res - RED LED | | - pin#4 - 220 Ohm Res - RED LED - GND - 5V - 
