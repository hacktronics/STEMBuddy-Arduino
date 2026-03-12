/**
 * STEM Buddy — Motor Control with Virtual Slider
 *
 * Demonstrates two-way communication between your ESP32 and the STEM
 * Buddy app. A virtual slider on your phone controls the motor speed,
 * and a virtual button toggles the direction — the app shows the motor
 * spinning with your chosen speed and direction.
 *
 * This is a great example of how physical controls (potentiometers,
 * switches) can be replaced with virtual ones on your phone screen.
 *
 * What you'll learn:
 *   - Reading virtual slider values (0–255)
 *   - Detecting button press events (edge detection)
 *   - Controlling motor speed and direction with run()
 *   - Two-way BLE communication (app ↔ ESP32)
 *
 * Data flow:
 *   App → ESP32: slider value (speed), button press (direction toggle)
 *   ESP32 → App: motor speed/direction commands, LCD status text
 *
 * Hardware needed: ESP32 only
 *
 * Steps:
 *   1. Upload this sketch to your ESP32
 *   2. Open STEM Buddy and connect via Bluetooth
 *   3. Drag Slider 0 to control motor speed
 *   4. Press Button 0 to toggle direction (CW ↔ CCW)
 *
 * Created by CodeSkool — https://codeskool.cc
 */

#include <STEMBuddy.h>

STEMBuddy buddy;

bool forward = true;           // true = clockwise, false = counter-clockwise
bool lastButtonState = false;  // For edge detection

void setup() {
    Serial.begin(115200);
    buddy.begin("Motor-Control");
}

void loop() {
    buddy.update();

    if (!buddy.isConnected()) return;

    // --- Read virtual slider as motor speed (0–255) ---
    uint8_t speed = buddy.input.slider(0);

    // --- Toggle direction on button press (rising edge only) ---
    bool pressed = buddy.input.buttonPressed(0);
    if (pressed && !lastButtonState) {
        forward = !forward;
    }
    lastButtonState = pressed;

    // --- Run motor: positive = CW, negative = CCW ---
    buddy.motor.run(forward ? speed : -speed);

    // --- Display status on LCD ---
    char line0[17], line1[17];
    snprintf(line0, sizeof(line0), "Speed: %-3d      ", speed);
    snprintf(line1, sizeof(line1), "Dir: %-11s", forward ? "CW" : "CCW");
    buddy.lcd.print(0, 0, line0);
    buddy.lcd.print(1, 0, line1);

    delay(50);
}
