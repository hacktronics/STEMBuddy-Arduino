/**
 * STEM Buddy — Automatic Light Dimmer
 *
 * Uses the phone's ambient light sensor to automatically adjust LED
 * brightness — brighter in dark rooms, dimmer in bright rooms. Just
 * like a real smart night-light!
 *
 * The bar graph shows the current light level (0–1000 lux), and the
 * oscilloscope plots light changes over time. Cover the phone's light
 * sensor to see the LED brighten instantly.
 *
 * What you'll learn:
 *   - Reading the ambient light sensor (lux)
 *   - Inverse mapping (dark → bright LED, bright → dim LED)
 *   - Using the bar graph for level visualization
 *   - Real-time sensor-driven output control
 *
 * Sensors used:
 *   - Ambient light (lux) → buddy.sensor.light()
 *
 * Hardware needed: ESP32 only
 *
 * Steps:
 *   1. Upload this sketch to your ESP32
 *   2. Open STEM Buddy and connect via Bluetooth
 *   3. Cover the phone's light sensor — the LED gets brighter
 *   4. Shine a flashlight at it — the LED dims
 *
 * Created by CodeSkool — https://codeskool.cc
 */

#include <STEMBuddy.h>

STEMBuddy buddy;

unsigned long lastUpdate = 0;

void setup() {
    Serial.begin(115200);
    buddy.begin("Light-Dimmer");

    buddy.scope.setLabel("Lux");
    buddy.scope.setRange(0, 1000);
    buddy.barGraph.setColor(STEMBuddyBarGraph::YELLOW); // Yellow bars
}

void loop() {
    buddy.update();

    if (!buddy.isConnected()) return;

    if (millis() - lastUpdate < 200) return;
    lastUpdate = millis();

    // Read ambient light level in lux
    uint16_t lux = buddy.sensor.light();

    // Cap at 1000 lux for our display range
    if (lux > 1000) lux = 1000;

    // Inverse mapping: dark room → bright LED, bright room → dim LED
    uint8_t brightness = map(lux, 0, 1000, 255, 0);
    buddy.led.setColor(255, 200, 100); // Warm white color
    buddy.led.setBrightness(brightness);
    buddy.led.on();

    // Bar graph: show ambient light level (10 bars)
    uint8_t bars[10];
    for (int i = 0; i < 10; i++) {
        int threshold = (i + 1) * 100; // Each bar = 100 lux
        if ((int)lux >= threshold) {
            bars[i] = 255;
        } else if ((int)lux >= threshold - 100) {
            bars[i] = map(lux, threshold - 100, threshold, 0, 255);
        } else {
            bars[i] = 0;
        }
    }
    buddy.barGraph.setAll(bars, 10);

    // LCD readout
    char line0[17], line1[17];
    snprintf(line0, sizeof(line0), "Light: %-4d lux ", lux);
    snprintf(line1, sizeof(line1), "LED:   %-3d%%     ", (brightness * 100) / 255);
    buddy.lcd.print(0, 0, line0);
    buddy.lcd.print(1, 0, line1);

    // Oscilloscope plot
    buddy.scope.plot((int16_t)lux);
}
