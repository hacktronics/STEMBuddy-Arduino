/**
 * STEM Buddy — Digital Compass
 *
 * Turns your phone into a compass for your ESP32! The phone's built-in
 * magnetometer calculates compass heading (0°–360°), and the
 * accelerometer measures tilt angle.
 *
 * The heading is shown on the LCD with a cardinal direction label
 * (N, NE, E, SE, S, SW, W, NW), and the LED changes color to match
 * the direction — blue for North, green for East, red for South,
 * yellow for West.
 *
 * The oscilloscope plots heading over time, so you can see how
 * steadily you're holding the phone.
 *
 * What you'll learn:
 *   - Reading magnetometer data (heading)
 *   - Reading accelerometer data (tilt)
 *   - Mapping angular values to LED colors
 *   - Cardinal direction calculation
 *
 * Sensors used:
 *   - Magnetometer X/Y  → buddy.sensor.heading()
 *   - Accelerometer      → buddy.sensor.tilt()
 *
 * Hardware needed: ESP32 only
 *
 * Steps:
 *   1. Upload this sketch to your ESP32
 *   2. Open STEM Buddy and connect via Bluetooth
 *   3. Rotate your phone and watch the compass heading change
 *   4. Tilt your phone to see the tilt angle update
 *
 * Created by CodeSkool — https://codeskool.cc
 */

#include <STEMBuddy.h>

STEMBuddy buddy;

unsigned long lastUpdate = 0;

// Convert heading angle to cardinal direction string
const char* toCardinal(float heading) {
    if (heading < 22.5  || heading >= 337.5) return "N ";
    if (heading < 67.5)  return "NE";
    if (heading < 112.5) return "E ";
    if (heading < 157.5) return "SE";
    if (heading < 202.5) return "S ";
    if (heading < 247.5) return "SW";
    if (heading < 292.5) return "W ";
    return "NW";
}

void setup() {
    Serial.begin(115200);
    buddy.begin("Compass");

    buddy.scope.setLabel("Heading");
    buddy.scope.setRange(0, 360);
}

void loop() {
    buddy.update();

    if (!buddy.isConnected()) return;

    // Update at 5 Hz
    if (millis() - lastUpdate < 200) return;
    lastUpdate = millis();

    float heading = buddy.sensor.heading();  // 0–360°
    float tilt    = buddy.sensor.tilt();     // 0–180°

    // --- LCD: show heading and tilt ---
    char line0[17], line1[17];
    snprintf(line0, sizeof(line0), "Dir: %s  %3d deg", toCardinal(heading), (int)heading);
    snprintf(line1, sizeof(line1), "Tilt: %5.1f deg ", tilt);
    buddy.lcd.print(0, 0, line0);
    buddy.lcd.print(1, 0, line1);

    // --- LED color mapped to compass quadrant ---
    // Smoothly blend: N=blue, E=green, S=red, W=yellow
    uint8_t r = 0, g = 0, b = 0;
    if (heading < 90) {
        b = map((int)heading, 0, 89, 255, 0);
        g = map((int)heading, 0, 89, 0, 255);
    } else if (heading < 180) {
        g = map((int)heading, 90, 179, 255, 0);
        r = map((int)heading, 90, 179, 0, 255);
    } else if (heading < 270) {
        r = 255;
        g = map((int)heading, 180, 269, 0, 255);
    } else {
        r = map((int)heading, 270, 359, 255, 0);
        g = map((int)heading, 270, 359, 255, 0);
        b = map((int)heading, 270, 359, 0, 255);
    }
    buddy.led.setColor(r, g, b);
    buddy.led.on();

    // --- Oscilloscope: plot heading over time ---
    buddy.scope.plot((int16_t)heading);
}
