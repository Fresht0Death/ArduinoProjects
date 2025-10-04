# Mood Lighting
## Demo
![Mood Lighting Demo](## Demo
![Mood Lighting Demo](Mood_Lighting.gif)


Description: Reads input from 3 phototransisitors with red, green, and blue gels on top of phototransistors to only allow for certain light waves to be observed. Once light is oberved under the spectrum allowed for each phototransistor a cathod RGB LED changes depending on what dominating light spectrum is being input.


Features:

Shine light over phototransistor → voltage let through → analog input is recieved and stored → print to Serial Monitor what each level is for Red, Green, Blue light being observed → turn this input into the 0-255 range for analog writing to Cathode RGB LED:

Arduino Uno (or similar) 3 Phototransistors + 3 GELS(Red, Green, Blue) + 6 220Ω resistors + Breadboard + jumper wires + 1 Cathode RGB

Wiring:

ARDUINO | | - 5V | - GND | | - A0 - 220 Ohm Res - Blue Phototransistor | | - A1 - 220 Ohm Res - Green Phototransistor | | - A3 - 220 Ohm Res - Red Phototransistor | | - pin#9,10,11 - 220 Ohm Res - Cathode RGB LED - GND - 5V - 
