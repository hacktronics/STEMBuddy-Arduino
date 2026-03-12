/**
 * STEM Buddy — Motion Detector
 *
 * Detects device motion using the phone's accelerometer and gyroscope.
 * When you shake or move the phone, the NeoPixel strip lights up like
 * a signal strength meter — green for gentle motion, yellow for moderate,
 * and red for vigorous shaking.
 *
 * A sharp shake triggers a buzzer alarm, and the step counter tracks
 * how many steps you've walked. All data is displayed on the LCD and
 * plotted on the oscilloscope.
 *
 * What you'll learn:
 *   - Reading accelerometer data (X, Y, Z axes)
 *   - Calculating acceleration magnitude
 *   - Motion threshold detection
 *   - Using the step counter sensor
 *   - Driving NeoPixel strips based on sensor values
 *
 * Sensors used:
 *   - Accelerometer (X/Y/Z) → buddy.sensor.accelX/Y/Z()
 *   - Step counter           → buddy.sensor.steps()
 *
 * Hardware needed: ESP32 only
 *
 * Steps:
 *   1. Upload this sketch to your ESP32
 *   2. Open STEM Buddy and connect via Bluetooth
 *   3. Shake your phone — watch the NeoPixels react!
 *   4. Walk around to see the step counter increment
 *
 * Created by CodeSkool — https://codeskool.cc
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
    buddy.scope.setRange(0, 300);
}

void loop() {
    buddy.update();

    if (!buddy.isConnected()) return;

    // Update at 4 Hz
    if (millis() - lastUpdate < 250) return;
    lastUpdate = millis();

    // Read accelerometer (m/s²)
    float ax = buddy.sensor.accelX();
    float ay = buddy.sensor.accelY();
    float az = buddy.sensor.accelZ();

    // Total acceleration magnitude
    float magnitude = sqrtf(ax * ax + ay * ay + az * az);

    // Motion intensity = deviation from gravity (9.8 m/s²)
    // At rest, total ≈ 9.8. Motion makes it spike above or dip below.
    float motion = fabsf(magnitude - 9.8f) * 25.0f;
    if (motion > 255) motion = 255;

    // --- NeoPixel strip: motion level meter ---
    int litCount = (int)(motion / 32); // 0–8 LEDs
    for (int i = 0; i < 8; i++) {
        if (i < litCount) {
            // Color gradient: green → yellow → orange → red
            uint8_t r, g;
            if (i < 3) {
                r = 0;    g = 255;  // Green
            } else if (i < 5) {
                r = 255;  g = 255;  // Yellow
            } else if (i < 7) {
                r = 255;  g = 128;  // Orange
            } else {
                r = 255;  g = 0;    // Red
            }
            buddy.led.setStripPixel(i, r, g, 0);
        } else {
            buddy.led.setStripPixel(i, 15, 15, 15); // Dim background
        }
    }

    // --- Buzzer: beep on strong motion (shake detected) ---
    if (motion > 150) {
        buddy.buzzer.tone(1200, 100);
    }

    // --- LCD: show motion intensity and step count ---
    char line0[17], line1[17];
    snprintf(line0, sizeof(line0), "Motion: %-3d     ", (int)motion);
    snprintf(line1, sizeof(line1), "Steps:  %-5d   ", buddy.sensor.steps());
    buddy.lcd.print(0, 0, line0);
    buddy.lcd.print(1, 0, line1);

    // --- Oscilloscope: plot motion over time ---
    buddy.scope.plot((int16_t)motion);
}
