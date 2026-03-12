/**
 * STEM Buddy — NeoPixel Sound Meter (VU Meter)
 *
 * Turns your phone into a real-time sound level meter using the phone's
 * microphone. The sound intensity is visualized as an 8-pixel NeoPixel
 * strip — like a classic VU meter — with colors shifting from green
 * (quiet) through yellow (moderate) to red (loud).
 *
 * The oscilloscope plots the raw sound level over time so you can see
 * patterns, and the LCD shows the current dB reading.
 *
 * How it works:
 *   App → ESP32: microphone sound level (dB)
 *   ESP32 → App: NeoPixel colors, LCD text, oscilloscope data
 *
 * Try it:
 *   1. Upload this sketch to your ESP32
 *   2. Open the STEM Buddy app and connect via Bluetooth
 *   3. Clap, talk, or play music near your phone
 *   4. Watch the NeoPixel bar react to sound!
 *
 * Concepts: analog sensor reading, mapping values, color gradients
 *
 * Created by CodeSkool — https://codeskool.cc
 */

#include <STEMBuddy.h>

STEMBuddy buddy;

// Map a sound level (0–100) to a color gradient: green → yellow → red
void soundToColor(uint8_t level, uint8_t &r, uint8_t &g, uint8_t &b) {
    if (level < 50) {
        // Green → Yellow
        r = map(level, 0, 49, 0, 255);
        g = 255;
    } else {
        // Yellow → Red
        r = 255;
        g = map(level, 50, 100, 255, 0);
    }
    b = 0;
}

void setup() {
    Serial.begin(115200);
    buddy.begin("Sound-Meter");

    // Configure 8-pixel NeoPixel strip
    buddy.led.setStripCount(8);

    // Label the oscilloscope channel
    buddy.scope.setLabel("Sound dB");
    buddy.scope.setRange(0, 100);
}

void loop() {
    buddy.update();

    if (buddy.isConnected()) {
        // Read the phone microphone level (0–100 dB scale)
        int soundLevel = buddy.sensor.soundLevel;

        // Clamp to valid range
        if (soundLevel < 0) soundLevel = 0;
        if (soundLevel > 100) soundLevel = 100;

        // Calculate how many pixels to light up (0–8)
        int litPixels = map(soundLevel, 0, 100, 0, 8);

        // Update each pixel
        for (int i = 0; i < 8; i++) {
            if (i < litPixels) {
                // Pixel intensity based on its position in the strip
                uint8_t pixelLevel = map(i, 0, 7, 0, 100);
                uint8_t r, g, b;
                soundToColor(pixelLevel, r, g, b);
                buddy.led.setStripPixel(i, r, g, b);
            } else {
                // Turn off pixels above the current level
                buddy.led.setStripPixel(i, 0, 0, 0);
            }
        }

        // Display dB reading on the LCD
        char line0[17];
        snprintf(line0, sizeof(line0), "Sound: %-3d dB   ", soundLevel);
        buddy.lcd.print(0, 0, line0);

        // Show a text bar on the second line
        char bar[17] = "                ";
        int barLen = map(soundLevel, 0, 100, 0, 16);
        for (int i = 0; i < barLen; i++) bar[i] = '#';
        buddy.lcd.print(1, 0, bar);

        // Plot on oscilloscope for time-series view
        buddy.scope.plot(soundLevel);

        delay(50); // ~20 Hz refresh
    }
}
