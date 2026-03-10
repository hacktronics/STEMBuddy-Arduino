/**
 * STEM Buddy — Motion Detector
 *
 * Uses accelerometer and gyroscope to detect movement.
 * Buzzes when the phone is shaken, shows movement intensity on LED strip.
 * Counts steps and displays them on the LCD.
 *
 * Sensors used: accelerometer, gyroscope, step counter
 */

#include <STEMBuddy.h>
#include <math.h>

STEMBuddy buddy;
unsigned long lastUpdate = 0;

void setup() {
    Serial.begin(115200);
    buddy.begin("Motion-Detect");
    buddy.led.setStripCount(8);
    buddy.scope.setLabel("Motion");
    buddy.scope.setRange(0, 2600);
}

void loop() {
    buddy.update();

    if (buddy.isConnected() && millis() - lastUpdate > 250) {
        lastUpdate = millis();

        // Calculate total acceleration magnitude
        float ax = buddy.sensor.accelX();
        float ay = buddy.sensor.accelY();
        float az = buddy.sensor.accelZ();
        float total = sqrtf(ax*ax + ay*ay + az*az);

        // Motion intensity: how far from resting (9.8 m/s²)
        float motion = fabsf(total - 9.8f) * 100;
        if (motion > 255) motion = 255;

        // Light up LED strip based on motion intensity
        int litLeds = (int)(motion / 32); // 0-8 LEDs
        for (int i = 0; i < 8; i++) {
            if (i < litLeds) {
                // Green → Yellow → Red as intensity increases
                uint8_t r = (i > 4) ? 255 : i * 50;
                uint8_t g = (i < 6) ? 255 : 255 - (i - 5) * 128;
                buddy.led.setStripPixel(i, r, g, 0);
            } else {
                buddy.led.setStripPixel(i, 10, 10, 10); // dim
            }
        }

        // Buzz if motion is strong (shake detected)
        if (motion > 150) {
            buddy.buzzer.tone(800, 100);
        }

        // Display on LCD (padded to avoid leftover chars)
        char line0[17], line1[17];
        snprintf(line0, sizeof(line0), "Motion: %-7d", (int)motion);
        snprintf(line1, sizeof(line1), "Steps:  %-7d", (int)buddy.sensor.steps());
        buddy.lcd.print(0, 0, line0);
        buddy.lcd.print(1, 0, line1);

        // Plot motion on oscilloscope
        buddy.scope.plot((int16_t)(motion * 10));
    }
}
