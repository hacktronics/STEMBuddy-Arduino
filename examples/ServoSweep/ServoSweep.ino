/**
 * STEM Buddy — Servo Sweep
 *
 * Smoothly sweeps a virtual servo motor back and forth between 0° and
 * 180°. The phone displays an animated servo arm moving in real time,
 * and the LCD shows the current angle.
 *
 * You can also control the servo manually using a virtual slider in the
 * STEM Buddy app — slide to set any angle from 0° to 180°.
 *
 * What you'll learn:
 *   - Controlling a servo angle programmatically
 *   - Reading a virtual slider for manual override
 *   - Displaying numeric values on the LCD
 *
 * Hardware needed: ESP32 only
 *
 * Steps:
 *   1. Upload this sketch to your ESP32
 *   2. Open STEM Buddy and connect via Bluetooth
 *   3. Watch the servo sweep automatically
 *   4. Drag Slider 0 in the app to take manual control
 *
 * Created by CodeSkool — https://codeskool.cc
 */

#include <STEMBuddy.h>

STEMBuddy buddy;

int angle = 0;
int direction = 1;           // 1 = increasing, -1 = decreasing
bool manualMode = false;

void setup() {
    Serial.begin(115200);
    buddy.begin("Servo-Sweep");
}

void loop() {
    buddy.update();

    if (!buddy.isConnected()) return;

    // Check if the user is moving the slider (manual override)
    uint8_t sliderVal = buddy.input.slider(0);

    if (sliderVal > 5) {
        // Slider is active — switch to manual mode
        manualMode = true;
        angle = map(sliderVal, 0, 255, 0, 180);
    } else if (manualMode && sliderVal <= 5) {
        // Slider released — resume auto sweep from current angle
        manualMode = false;
        direction = 1;
    }

    if (!manualMode) {
        // Auto sweep: 2° steps for smooth motion
        angle += direction * 2;
        if (angle >= 180) { angle = 180; direction = -1; }
        if (angle <= 0)   { angle = 0;   direction =  1; }
    }

    // Send angle to the virtual servo
    buddy.servo.setAngle(angle);

    // Show the angle on the LCD
    char line0[17], line1[17];
    snprintf(line0, sizeof(line0), "Angle: %-3d deg  ", angle);
    snprintf(line1, sizeof(line1), "Mode: %-10s", manualMode ? "Manual" : "Auto Sweep");
    buddy.lcd.print(0, 0, line0);
    buddy.lcd.print(1, 0, line1);

    delay(30);
}
