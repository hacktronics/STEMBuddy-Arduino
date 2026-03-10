/**
 * STEM Buddy — LED Blink Example
 *
 * Blinks a virtual LED on the STEM Buddy app.
 * The LED appears on your phone screen, controlled by this ESP32.
 *
 * 1. Upload this sketch to your ESP32
 * 2. Open the STEM Buddy app
 * 3. Connect to your ESP32 via Bluetooth
 * 4. Watch the virtual LED blink!
 */

#include <STEMBuddy.h>

STEMBuddy buddy;

void setup() {
    Serial.begin(115200);
    buddy.begin("LED-Blink-Demo");
    Serial.println("Waiting for STEM Buddy app to connect...");
}

void loop() {
    buddy.update();

    if (buddy.isConnected()) {
        // Red LED ON
        buddy.led.setColor(255, 0, 0);
        buddy.led.on();
        delay(1000);

        // LED OFF
        buddy.led.off();
        delay(1000);

        // Green LED ON
        buddy.led.setColor(0, 255, 0);
        buddy.led.on();
        delay(1000);

        // LED OFF
        buddy.led.off();
        delay(1000);
    }
}
