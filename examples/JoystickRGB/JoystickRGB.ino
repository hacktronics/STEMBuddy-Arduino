/**
 * STEM Buddy — Joystick RGB Color Mixer
 *
 * Uses the virtual joystick on the STEM Buddy app to mix RGB colors
 * in real time. The X-axis controls the Red ↔ Blue balance, and the
 * Y-axis controls the Green intensity. The resulting color is displayed
 * on a virtual RGB LED and a NeoPixel strip.
 *
 * A virtual button resets the color to white, and the LCD shows the
 * exact RGB values — great for learning about color mixing!
 *
 * How it works:
 *   App → ESP32: joystick X/Y position, button press
 *   ESP32 → App: RGB LED color, NeoPixel strip, LCD readout
 *
 * Try it:
 *   1. Upload this sketch to your ESP32
 *   2. Open the STEM Buddy app and connect via Bluetooth
 *   3. Drag the virtual joystick to mix colors
 *   4. Press Button 0 to reset to white
 *
 * Concepts: 2D input mapping, color mixing, real-time control
 *
 * Created by CodeSkool — https://codeskool.cc
 */

#include <STEMBuddy.h>

STEMBuddy buddy;

void setup() {
    Serial.begin(115200);
    buddy.begin("Joystick-RGB");

    buddy.led.setStripCount(8);
}

void loop() {
    buddy.update();

    if (buddy.isConnected()) {
        // Read joystick position (0–1000 range)
        int joyX = buddy.input.touchX(); // Left-right
        int joyY = buddy.input.touchY(); // Up-down

        // Map joystick axes to RGB values
        // X-axis: left = red, center = purple, right = blue
        uint8_t red   = map(joyX, 0, 1000, 255, 0);
        uint8_t blue  = map(joyX, 0, 1000, 0, 255);
        // Y-axis: bottom = dark, top = bright green
        uint8_t green = map(joyY, 0, 1000, 0, 255);

        // Reset to white on button press
        if (buddy.input.buttonPressed(0)) {
            red = 255;
            green = 255;
            blue = 255;
        }

        // Set the main RGB LED
        buddy.led.setColor(red, green, blue);
        buddy.led.on();

        // Fill the NeoPixel strip with the same color
        buddy.led.fillStrip(red, green, blue);

        // Display RGB values on LCD
        char line0[17], line1[17];
        snprintf(line0, sizeof(line0), "R:%-3d G:%-3d    ", red, green);
        snprintf(line1, sizeof(line1), "B:%-3d           ", blue);
        buddy.lcd.print(0, 0, line0);
        buddy.lcd.print(1, 0, line1);

        // Set LCD backlight to match the mixed color
        buddy.lcd.setBacklightColor(red, green, blue);

        delay(50);
    }
}
