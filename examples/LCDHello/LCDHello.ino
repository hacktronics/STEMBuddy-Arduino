/**
 * STEM Buddy — LCD Hello World
 *
 * Displays text on a virtual 16×2 LCD on your phone screen. No physical
 * LCD module needed — your phone renders a pixel-perfect LCD display
 * controlled by your ESP32 code.
 *
 * This example shows a welcome message on line 1 and a seconds counter
 * on line 2. It also demonstrates LCD features like cursor positioning,
 * backlight color, and scrolling text.
 *
 * What you'll learn:
 *   - Printing text to specific LCD positions (row, column)
 *   - Clearing the display
 *   - Changing the backlight color
 *   - Scrolling text
 *
 * Hardware needed: ESP32 only
 *
 * Steps:
 *   1. Upload this sketch to your ESP32
 *   2. Open STEM Buddy and connect via Bluetooth
 *   3. Watch the virtual LCD update every second
 *
 * Created by CodeSkool — https://codeskool.cc
 */

#include <STEMBuddy.h>

STEMBuddy buddy;

unsigned long lastUpdate = 0;
int seconds = 0;

void setup() {
    Serial.begin(115200);
    buddy.begin("LCD-Hello");
}

void loop() {
    buddy.update();

    if (!buddy.isConnected()) return;

    if (millis() - lastUpdate >= 1000) {
        lastUpdate = millis();

        // Line 1: static greeting
        buddy.lcd.print(0, 0, "Hello, World!   ");

        // Line 2: live counter showing uptime
        buddy.lcd.print(1, 0, "Uptime: ");
        buddy.lcd.setCursor(1, 8);
        buddy.lcd.print(seconds);
        buddy.lcd.print(" sec    ");

        // Change backlight color every 10 seconds
        switch ((seconds / 10) % 4) {
            case 0: buddy.lcd.setBacklightColor(255, 255, 255); break; // White
            case 1: buddy.lcd.setBacklightColor(  0, 200, 255); break; // Cyan
            case 2: buddy.lcd.setBacklightColor(255, 200,   0); break; // Warm
            case 3: buddy.lcd.setBacklightColor(100, 255, 100); break; // Green
        }

        // Scroll the text left every 30 seconds as a visual demo
        if (seconds > 0 && seconds % 30 == 0) {
            for (int i = 0; i < 16; i++) {
                buddy.lcd.scrollLeft();
                delay(200);
            }
            buddy.lcd.print(0, 0, "Hello, World!   ");
        }

        seconds++;
    }
}
