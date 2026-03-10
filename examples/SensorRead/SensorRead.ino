/**
 * STEM Buddy — Virtual Sensor Read
 *
 * Reads virtual sensor data FROM the STEM Buddy app.
 * The phone sends its real sensor values (or simulated ones) to your ESP32.
 * No physical sensors needed — your phone already has them!
 *
 * This example reads temperature, light, sound, and proximity,
 * then displays them on the virtual LCD and oscilloscope.
 */

#include <STEMBuddy.h>

STEMBuddy buddy;
unsigned long lastUpdate = 0;

void setup() {
    Serial.begin(115200);
    buddy.begin("Sensor-Reader");
    buddy.scope.setLabel("Temperature");
    buddy.scope.setRange(1500, 3500); // 15.00 - 35.00 °C (x100)
}

void loop() {
    buddy.update();

    if (!buddy.isConnected()) return;

    if (millis() - lastUpdate > 500) {
        lastUpdate = millis();

        // Read environment sensors
        float temp = buddy.sensor.temperature();
        uint16_t lux = buddy.sensor.light();
        float dB = buddy.sensor.soundLevel();
        uint16_t cm = buddy.sensor.proximity();

        // Display on virtual LCD (padded to avoid leftover chars)
        char line0[17], line1[17];
        snprintf(line0, sizeof(line0), "T:%-5.1f L:%-5d", temp, lux);
        snprintf(line1, sizeof(line1), "S:%-4.0fdB P:%-3dcm", dB, cm);
        buddy.lcd.print(0, 0, line0);
        buddy.lcd.print(1, 0, line1);

        // Plot temperature on oscilloscope
        buddy.scope.plot((int16_t)(temp * 100));

        Serial.printf("T:%.1fC L:%dlux S:%.0fdB P:%dcm\n",
                       temp, lux, dB, cm);
    }
}
