# Mood Cue
## Demo
![Mood Lighting Demo](ezgif.com-video-to-gif-converter.gif)


Description: Reads input from a potentiometer and depending on how far the knob is turned the servo motor pointer-finger turns to reveal what state entering a room is at. Two decoupling transsitors allow for smoother waves which allows for a more smooth experience between potentiometer and Servo motor.


Features:

Turn potentiometer → voltage let through → analog input is recieved and stored → voltage is translated to 0-170 degree range → then Servo pointer-finger reflects this with where it is pointing:

Arduino Uno (or similar) 1 Potentiometer + 2 100 micronFarrad Capacitors + jumper wires 

Wiring:

ARDUINO | | - 5V | - GND | | - A0 - 100 micronF Capacitor - Potentiometer | | - A9 - 100 micronF Capacitor - Servo Motor | |  - GND - 5V - 

