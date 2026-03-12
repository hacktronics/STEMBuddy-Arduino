/**
 * STEM Buddy — RGB Color Mixer with Sliders
 *
 * Use three virtual sliders to mix any color in real time! Slider 0
 * controls Red, Slider 1 controls Green, and Slider 2 controls Blue.
 * The mixed color is shown on the virtual RGB LED, and the exact
 * RGB values are displayed on the LCD.
 *
 * This is a hands-on way to learn how RGB color mixing works — try
 * making orange (R:255 G:128 B:0), purple (R:128 G:0 B:255), or
 * any color you can imagine.
 *
 * What you'll learn:
 *   - Reading multiple virtual sliders (0–255)
 *   - RGB color model (additive color mixing)
 *   - Controlling LED color with precise values
 *   - Real-time input → output feedback loop
 *
 * Data flow:
 *   App → ESP32: three slider values (R, G, B)
 *   ESP32 → App: LED color, LCD text, backlight color
 *
 * Hardware needed: ESP32 only
 *
 * Steps:
 *   1. Upload this sketch to your ESP32
 *   2. Open STEM Buddy and connect via Bluetooth
 *   3. Drag the three sliders to mix your color
 *   4. The LED and LCD backlight update instantly
 *
 * Created by CodeSkool — https://codeskool.cc
 */

#include <STEMBuddy.h>

STEMBuddy buddy;

void setup() {
    Serial.begin(115200);
    buddy.begin("Color-Mixer");
}

void loop() {
    buddy.update();

    if (!buddy.isConnected()) return;

    // Read three sliders as R, G, B (each 0–255)
    uint8_t r = buddy.input.slider(0);
    uint8_t g = buddy.input.slider(1);
    uint8_t b = buddy.input.slider(2);

    // Set the RGB LED to the mixed color
    buddy.led.setColor(r, g, b);
    buddy.led.on();

    // Also set the LCD backlight to match
    buddy.lcd.setBacklightColor(r, g, b);

    // Display the exact RGB values
    char line0[17], line1[17];
    snprintf(line0, sizeof(line0), "R:%-3d G:%-3d    ", r, g);
    snprintf(line1, sizeof(line1), "B:%-3d           ", b);
    buddy.lcd.print(0, 0, line0);
    buddy.lcd.print(1, 0, line1);

    delay(50);
}
