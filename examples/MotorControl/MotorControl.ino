/**
 * STEM Buddy — Motor Control with Virtual Slider
 *
 * Uses virtual buttons and slider from the STEM Buddy app to control
 * a virtual motor. The phone shows the motor spinning — speed controlled
 * by a slider on your phone screen, read by your ESP32 code.
 *
 * Two-way communication:
 *   App → ESP32: slider value (speed), button (direction toggle)
 *   ESP32 → App: motor speed + direction commands (visual feedback)
 */

#include <STEMBuddy.h>

STEMBuddy buddy;
uint8_t currentDir = 0; // 0=CW, 1=CCW
bool lastButtonState = false;

void setup() {
    Serial.begin(115200);
    buddy.begin("Motor-Control");
}

void loop() {
    buddy.update();

    if (buddy.isConnected()) {
        // Read slider 0 as motor speed
        uint8_t speed = buddy.input.slider(0);
        buddy.motor.setSpeed(speed);

        // Toggle direction with button 0
        bool pressed = buddy.input.buttonPressed(0);
        if (pressed && !lastButtonState) {
            currentDir = !currentDir;
            buddy.motor.setDirection(currentDir);
        }
        lastButtonState = pressed;

        // Show status on virtual LCD (padded to avoid leftover chars)
        char line0[17];
        snprintf(line0, sizeof(line0), "Speed: %-3d      ", speed);
        buddy.lcd.print(0, 0, line0);
        buddy.lcd.print(1, 0, currentDir ? "Dir: CCW        " : "Dir: CW         ");

        delay(50);
    }
}
