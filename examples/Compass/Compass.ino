/**
 * STEM Buddy — Digital Compass
 *
 * Uses the phone's magnetometer to show compass heading on the virtual LCD.
 * Also shows tilt angle from the accelerometer.
 * The LED changes color based on the cardinal direction you're facing.
 *
 * Sensors used: magnetometer (heading), accelerometer (tilt)
 */

#include <STEMBuddy.h>

STEMBuddy buddy;
unsigned long lastUpdate = 0;

// Map heading to cardinal direction name
const char* cardinalDir(float heading) {
    if (heading < 22.5 || heading >= 337.5) return "N ";
    if (heading < 67.5) return "NE";
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

    if (buddy.isConnected() && millis() - lastUpdate > 200) {
        lastUpdate = millis();

        float heading = buddy.sensor.heading();
        float tilt = buddy.sensor.tilt();

        // Show heading on LCD (padded to avoid leftover chars)
        char line0[17], line1[17];
        snprintf(line0, sizeof(line0), "Dir:%s %3d\xDF    ", cardinalDir(heading), (int)heading);
        snprintf(line1, sizeof(line1), "Tilt: %5.1f\xDF   ", tilt);
        buddy.lcd.print(0, 0, line0);
        buddy.lcd.print(1, 0, line1);

        // LED color changes with direction
        // North=blue, East=green, South=red, West=yellow
        uint8_t r = 0, g = 0, b = 0;
        if (heading < 90) {
            b = 255 - heading * 2.8;
            g = heading * 2.8;
        } else if (heading < 180) {
            g = 255 - (heading - 90) * 2.8;
            r = (heading - 90) * 2.8;
        } else if (heading < 270) {
            r = 255;
            g = (heading - 180) * 2.8;
        } else {
            r = 255 - (heading - 270) * 2.8;
            g = 255 - (heading - 270) * 2.8;
            b = (heading - 270) * 2.8;
        }
        buddy.led.setColor(r, g, b);

        // Plot heading on oscilloscope
        buddy.scope.plot((int16_t)heading);
    }
}
