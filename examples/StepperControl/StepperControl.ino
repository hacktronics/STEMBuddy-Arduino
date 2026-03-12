/**
 * STEM Buddy — Stepper Motor Precision Control
 *
 * Controls a virtual stepper motor using the keypad for precise angle
 * positioning. Type a number (0–360) on the keypad and press '#' to
 * move the stepper to that exact angle. Press '*' to reset to 0°.
 *
 * The LCD shows the current and target angles, and the 7-segment
 * display shows the current position.
 *
 * What you'll learn:
 *   - Controlling stepper motor angle precisely
 *   - Building numeric input from keypad digits
 *   - Stepper motor direction control (CW/CCW)
 *   - Displaying feedback on multiple virtual components
 *
 * Virtual inputs used:
 *   - Keypad: '0'–'9' for angle, '#' to go, '*' to reset
 *
 * Hardware needed: ESP32 only
 *
 * Steps:
 *   1. Upload this sketch to your ESP32
 *   2. Open STEM Buddy and connect via Bluetooth
 *   3. Type an angle (e.g., 90) on the keypad
 *   4. Press '#' to move the stepper to that angle
 *   5. Press '*' to return to 0°
 *
 * Created by CodeSkool — https://codeskool.cc
 */

#include <STEMBuddy.h>

STEMBuddy buddy;

char inputBuf[5];   // Buffer for typed digits
int inputLen = 0;
int currentAngle = 0;

void showAngleOn7Seg(int angle) {
    buddy.segment7.showNumber(angle);
}

void setup() {
    Serial.begin(115200);
    buddy.begin("Stepper-Ctrl");

    buddy.stepper.setRPM(30);
    buddy.segment7.setColor(255, 100, 0); // Orange
    memset(inputBuf, 0, sizeof(inputBuf));

    buddy.lcd.print(0, 0, "Stepper Control ");
    buddy.lcd.print(1, 0, "Type angle + #  ");
}

void loop() {
    buddy.update();

    if (!buddy.isConnected()) return;

    // Check for keypad input
    if (buddy.input.keypadPressed()) {
        char key = buddy.input.keypad();

        if (key == '*') {
            // Reset to 0°
            currentAngle = 0;
            buddy.stepper.setAngle(0);
            inputLen = 0;
            memset(inputBuf, 0, sizeof(inputBuf));
            buddy.buzzer.tone(500, 50);

        } else if (key == '#') {
            // Submit — move to the entered angle
            if (inputLen > 0) {
                inputBuf[inputLen] = '\0';
                int targetAngle = atoi(inputBuf);
                targetAngle = constrain(targetAngle, 0, 360);

                buddy.stepper.setAngle(targetAngle);
                currentAngle = targetAngle;

                buddy.buzzer.tone(1000, 100);
            }
            inputLen = 0;
            memset(inputBuf, 0, sizeof(inputBuf));

        } else if (key >= '0' && key <= '9' && inputLen < 3) {
            // Add digit to input
            inputBuf[inputLen++] = key;
            buddy.buzzer.tone(800, 30);
        }
    }

    // --- 7-Segment: current angle ---
    showAngleOn7Seg(currentAngle);

    // --- LCD: show current angle and input buffer ---
    char line0[17], line1[17];
    snprintf(line0, sizeof(line0), "Angle: %-3d deg  ", currentAngle);

    if (inputLen > 0) {
        inputBuf[inputLen] = '\0';
        snprintf(line1, sizeof(line1), "Go to: %-3s #    ", inputBuf);
    } else {
        snprintf(line1, sizeof(line1), "Type angle + #  ");
    }
    buddy.lcd.print(0, 0, line0);
    buddy.lcd.print(1, 0, line1);

    delay(50);
}
