# ArduinoProjects

In an attempt to better understand embedded systems and embedded C, I am building small projects that track data, interact with hardware, and eventually log results. Each project is meant to build on my knowledge of C/C++, Arduino, and embedded systems concepts.

---

## 📂 Projects

### 1. sensorTemp
**Description:**  
Reads temperature from a TMP36 sensor, tracks the **hottest and coldest values of the day (°F)**, and displays temperature ranges with **3 LEDs (°C bands)**.

**Features:**
- Converts analog readings → voltage → °C and °F
- Tracks min/max in °F
- LED indicators show temperature zones based on °C thresholds
- Prints live readings + min/max to Serial Monitor

**Hardware Requirements:**
- Arduino Uno (or similar)
- TMP36 temperature sensor
- 3 LEDs + 220Ω resistors
- Breadboard + jumper wires

**Wiring:**
