/**
 * STEM Buddy — LCD Hello World
 *
 * Displays text on a virtual 16x2 LCD on the STEM Buddy app.
 * No physical LCD needed — your phone IS the LCD.
 *
 * 1. Upload this sketch to your ESP32
 * 2. Open STEM Buddy → connect via Bluetooth
 * 3. The virtual LCD shows your messages!
 */

#include <STEMBuddy.h>

STEMBuddy buddy;
unsigned long lastUpdate = 0;
int counter = 0;

void setup() {
    Serial.begin(115200);
    buddy.begin("LCD-Hello");
    Serial.println("Waiting for STEM Buddy app...");
}

void loop() {
    buddy.update();

    if (buddy.isConnected() && millis() - lastUpdate > 1000) {
        lastUpdate = millis();

        buddy.lcd.clear();
        buddy.lcd.print(0, 0, "Hello World!");
        buddy.lcd.print(1, 0, "Count: ");
        buddy.lcd.print(1, 7, counter);

        counter++;
    }
}
