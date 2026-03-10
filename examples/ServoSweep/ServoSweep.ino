/**
 * STEM Buddy — Servo Sweep
 *
 * Sweeps a virtual servo back and forth on the STEM Buddy app.
 * Your phone displays the servo arm moving — controlled by your ESP32.
 */

#include <STEMBuddy.h>

STEMBuddy buddy;
int angle = 0;
int direction = 1;

void setup() {
    Serial.begin(115200);
    buddy.begin("Servo-Sweep");
}

void loop() {
    buddy.update();

    if (buddy.isConnected()) {
        buddy.servo.setAngle(angle);

        angle += direction * 3;
        if (angle >= 180) { angle = 180; direction = -1; }
        if (angle <= 0)   { angle = 0;   direction = 1;  }

        delay(30);
    }
}
