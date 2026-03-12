/**
 * STEM Buddy — LED Blink
 *
 * The classic "Hello World" of electronics — blinking an LED — but with
 * a twist: the LED is virtual. Your phone screen IS the LED, controlled
 * by code running on your ESP32 over Bluetooth.
 *
 * This example cycles through three colors (red, green, blue) with a
 * one-second on/off rhythm, demonstrating basic digital output and
 * RGB color control.
 *
 * What you'll learn:
 *   - Turning a virtual LED on and off
 *   - Setting RGB colors (0–255 per channel)
 *   - Using delay() for timing
 *
 * Hardware needed: ESP32 (no other components!)
 *
 * Wiring: None — your phone is the LED.
 *
 * Steps:
 *   1. Upload this sketch to your ESP32
 *   2. Open the STEM Buddy app on your phone
 *   3. Tap "Connect" and select your ESP32
 *   4. Watch the virtual LED cycle through red → green → blue
 *
 * Created by CodeSkool — https://codeskool.cc
 */

#include <STEMBuddy.h>

STEMBuddy buddy;

// Define the colors we'll cycle through: {R, G, B}
const uint8_t colors[][3] = {
    {255,   0,   0},  // Red
    {  0, 255,   0},  // Green
    {  0,   0, 255},  // Blue
};
const int numColors = sizeof(colors) / sizeof(colors[0]);
int colorIndex = 0;

void setup() {
    Serial.begin(115200);
    buddy.begin("LED-Blink");
    Serial.println("Waiting for STEM Buddy app to connect...");
}

void loop() {
    buddy.update();

    if (buddy.isConnected()) {
        // Set the current color and turn the LED on
        buddy.led.setColor(
            colors[colorIndex][0],
            colors[colorIndex][1],
            colors[colorIndex][2]
        );
        buddy.led.on();
        delay(1000);

        // Turn the LED off for one second
        buddy.led.off();
        delay(1000);

        // Advance to the next color
        colorIndex = (colorIndex + 1) % numColors;
    }
}
