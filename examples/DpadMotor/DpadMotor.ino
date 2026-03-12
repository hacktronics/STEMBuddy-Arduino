/**
 * STEM Buddy — D-Pad Motor & Servo Controller
 *
 * Use the virtual D-pad to control a motor and servo — like a game
 * controller for hardware! Up/Down controls motor speed (faster/slower),
 * and Left/Right rotates the servo.
 *
 * This demonstrates how the D-pad input works and how it can be mapped
 * to control multiple outputs simultaneously — a common pattern in
 * robotics and remote control.
 *
 * What you'll learn:
 *   - Reading D-pad directional input
 *   - Mapping D-pad to motor speed control
 *   - Mapping D-pad to servo angle control
 *   - Real-time feedback on LCD
 *
 * Virtual inputs used:
 *   - D-pad: use STEMBuddyInput::DPAD_UP / DOWN / LEFT / RIGHT constants
 *
 * Hardware needed: ESP32 only
 *
 * Steps:
 *   1. Upload this sketch to your ESP32
 *   2. Open STEM Buddy and connect via Bluetooth
 *   3. Press Up/Down on the D-pad → motor speeds up/slows down
 *   4. Press Left/Right → servo rotates
 *
 * Created by CodeSkool — https://codeskool.cc
 */

#include <STEMBuddy.h>

STEMBuddy buddy;

int motorSpeed = 0;   // 0–255
int servoAngle = 90;  // 0–180, starts centered

void setup() {
    Serial.begin(115200);
    buddy.begin("DPad-Control");
}

void loop() {
    buddy.update();

    if (!buddy.isConnected()) return;

    uint8_t dpad = buddy.input.dpad();

    // Up/Down → motor speed
    if (dpad == STEMBuddyInput::DPAD_UP) {
        motorSpeed += 5;
        if (motorSpeed > 255) motorSpeed = 255;
    } else if (dpad == STEMBuddyInput::DPAD_DOWN) {
        motorSpeed -= 5;
        if (motorSpeed < 0) motorSpeed = 0;
    }

    // Left/Right → servo angle
    if (dpad == STEMBuddyInput::DPAD_LEFT) {
        servoAngle -= 3;
        if (servoAngle < 0) servoAngle = 0;
    } else if (dpad == STEMBuddyInput::DPAD_RIGHT) {
        servoAngle += 3;
        if (servoAngle > 180) servoAngle = 180;
    }

    // Apply outputs
    buddy.motor.setSpeed(motorSpeed);
    buddy.servo.setAngle(servoAngle);

    // LED color reflects motor speed (dim blue → bright red)
    uint8_t r = map(motorSpeed, 0, 255, 0, 255);
    uint8_t b = map(motorSpeed, 0, 255, 100, 0);
    buddy.led.setColor(r, 0, b);
    buddy.led.on();

    // LCD display
    char line0[17], line1[17];
    snprintf(line0, sizeof(line0), "Motor: %-3d      ", motorSpeed);
    snprintf(line1, sizeof(line1), "Servo: %-3d deg  ", servoAngle);
    buddy.lcd.print(0, 0, line0);
    buddy.lcd.print(1, 0, line1);

    delay(30);
}
