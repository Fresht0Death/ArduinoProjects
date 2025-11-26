Embedded Hourglass Pomodoro Timer ⏳
A dedicated hardware productivity tool designed to visualize time passage using a digital hourglass metaphor. Built on Arduino using C++, this project implements a Finite State Machine (FSM) architecture to handle concurrent graphics rendering and touch input without blocking.

📖 Overview
Traditional digital timers induce anxiety with ticking numbers. This project solves that by using a pixel-art hourglass that allows for peripheral monitoring of time—you can see how much time is left out of the corner of your eye without breaking focus.

This is a "bare-metal" approach to productivity, replacing phone apps to eliminate digital distractions.

✨ Key Features
Visual Physics Engine: Custom logic draws "sand" falling pixel-by-pixel, updating the display in real-time.

Interrupt-Style UI: Touch inputs (Pause/Resume) are handled immediately, even while the display is updating.

State Preservation: Pausing the timer freezes the sand exactly where it is; resuming picks up instantly.

Safety Rails: "Confirm Reset" dialog prevents accidental cancellation of a work session.

Automated Intervals: Tracks completed Pomodoros (#) and automatically triggers a 5-minute break state upon completion.

⚙️ Technical Implementation
1. Finite State Machine (FSM)

The core logic avoids "spaghetti code" by using a strict enum based State Machine. This ensures the system is deterministic and easy to debug.

C++
enum HourGlassState {
  TOP_Q1,      // First 6m 15s quadrant
  TOP_Q2,      // Second quadrant
  TOP_Q3,      // ...
  TOP_Q4,      // ...
  DONE,        // Session complete
  WAIT_START   // Idle/Menu state
};
2. Non-Blocking Timing

Instead of using the standard delay() function (which halts the processor and makes the screen unresponsive to touch), this project uses millis() based comparison (via the millisDelay library).

Benefit: The user can pause or reset the device instantly at any microsecond, even mid-animation.

3. Graphics Logic

To optimize the refresh rate on the ILI9341 display driver, the code does not redraw the entire screen every frame.

It utilizes a dirty-line method: It draws a line of "sand" color at the bottom and overwrites a line of "white" color at the top of the current quadrant.

This minimizes SPI bus traffic, resulting in a smoother animation compared to full-frame buffering.

4. Hardware/Software Stack

Microcontroller: Arduino Uno / Mega

Display: 2.8" TFT LCD Shield (ILI9341 Driver)

Input: Resistive Touchscreen (4-wire)

Language: C++

Libraries: Elegoo_TFTLCD, Elegoo_GFX, TouchScreen, millisDelay

📊 Logic Flow
Code snippet
graph TD
    A[WAIT_START] -->|User Taps YES| B[TOP_Q1]
    B -->|Timer Q1 Done| C[TOP_Q2]
    C -->|Timer Q2 Done| D[TOP_Q3]
    D -->|Timer Q3 Done| E[TOP_Q4]
    E -->|Timer Q4 Done| F[DONE / BREAK]
    F -->|Break Timer End| A
    
    subgraph Interrupt Handling
    B -.->|Pause Button| P[PAUSED]
    C -.->|Pause Button| P
    D -.->|Pause Button| P
    E -.->|Pause Button| P
    P -.->|Resume| B
    end
🛠️ Setup & Wiring
This code is designed for a TFT Shield that plugs directly into an Arduino, but pins can be configured in the macros:

C++
#define LCD_CS A3
#define LCD_CD A2
#define LCD_WR A1
#define LCD_RD A0
#define LCD_RESET A4
Touch Calibration: The code includes calibration values for standard 2.8" resistive screens (TS_MINX, TS_MAXX, etc.). If touches are inaccurate, run a calibration sketch to update lines 45-50 in the source code.

🚀 Future Improvements
PCB Integration: Move from breadboard/shield to a custom PCB with a lipo battery for portability.

Deep Sleep: Implement power-saving interrupts to sleep the MCU during the "Wait" state.

Haptic Feedback: Add a vibration motor for silent alarms when the timer completes.
