/**
 * STEM Buddy — Environment Sensor Reader
 *
 * Reads four environment sensors from your phone and displays them on
 * the virtual LCD: temperature (°C), ambient light (lux), sound level
 * (dB), and proximity distance (cm).
 *
 * The oscilloscope plots temperature over time so you can observe
 * changes as you move between warm and cool areas.
 *
 * Your phone already has these sensors built in — STEM Buddy streams
 * their values to your ESP32 over Bluetooth, so your code can read
 * them just like physical sensors wired to GPIO pins.
 *
 * What you'll learn:
 *   - Reading phone sensor data on the ESP32
 *   - Displaying formatted values on the LCD
 *   - Plotting data on the oscilloscope
 *   - Understanding sensor data types and units
 *
 * Sensors used:
 *   - Temperature (°C)     — buddy.sensor.temperature()
 *   - Light (lux)          — buddy.sensor.light()
 *   - Sound level (dB)     — buddy.sensor.soundLevel()
 *   - Proximity (cm)       — buddy.sensor.proximity()
 *
 * Hardware needed: ESP32 only
 *
 * Steps:
 *   1. Upload this sketch to your ESP32
 *   2. Open STEM Buddy and connect via Bluetooth
 *   3. The LCD shows live sensor readings from your phone
 *   4. Try covering the light sensor, making noise, etc.
 *
 * Created by CodeSkool — https://codeskool.cc
 */

#include <STEMBuddy.h>

STEMBuddy buddy;

unsigned long lastUpdate = 0;

void setup() {
    Serial.begin(115200);
    buddy.begin("Sensor-Reader");

    // Configure oscilloscope for temperature range
    buddy.scope.setLabel("Temp C");
    buddy.scope.setRange(0, 5000); // 0.00–50.00 °C (x100)
}

void loop() {
    buddy.update();

    if (!buddy.isConnected()) return;

    // Update display at 2 Hz (every 500ms)
    if (millis() - lastUpdate < 500) return;
    lastUpdate = millis();

    // Read all environment sensors
    float temp   = buddy.sensor.temperature();  // °C
    uint16_t lux = buddy.sensor.light();        // lux
    float dB     = buddy.sensor.soundLevel();   // dB
    uint16_t cm  = buddy.sensor.proximity();    // cm

    // Format and display on the virtual LCD
    // Line 1: Temperature and Light
    char line0[17];
    snprintf(line0, sizeof(line0), "T:%-5.1fC L:%-4d", temp, lux);
    buddy.lcd.print(0, 0, line0);

    // Line 2: Sound and Proximity
    char line1[17];
    snprintf(line1, sizeof(line1), "S:%-4.0fdB P:%-3dcm", dB, cm);
    buddy.lcd.print(1, 0, line1);

    // Plot temperature on the oscilloscope (x100 for precision)
    buddy.scope.plot((int16_t)(temp * 100));

    // Also print to serial for debugging
    Serial.printf("T:%.1f°C L:%dlux S:%.0fdB P:%dcm\n", temp, lux, dB, cm);
}
