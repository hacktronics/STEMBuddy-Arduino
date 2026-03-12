/**
 * STEM Buddy — NeoPixel Rainbow
 *
 * Displays a flowing rainbow animation on an 8-pixel NeoPixel strip.
 * The rainbow smoothly scrolls across the strip, creating a mesmerizing
 * color effect. Slider 0 controls the animation speed, and Slider 1
 * controls brightness.
 *
 * What you'll learn:
 *   - Setting individual NeoPixel colors
 *   - HSV-to-RGB color conversion for smooth gradients
 *   - Animation timing and frame-based updates
 *   - Interactive speed/brightness control
 *
 * Data flow:
 *   App → ESP32: slider values (speed, brightness)
 *   ESP32 → App: NeoPixel strip colors
 *
 * Hardware needed: ESP32 only
 *
 * Steps:
 *   1. Upload this sketch to your ESP32
 *   2. Open STEM Buddy and connect via Bluetooth
 *   3. Watch the rainbow flow across the NeoPixel strip
 *   4. Drag Slider 0 to change speed, Slider 1 for brightness
 *
 * Created by CodeSkool — https://codeskool.cc
 */

#include <STEMBuddy.h>

STEMBuddy buddy;

int hueOffset = 0;

// Convert HSV (hue 0–360, saturation/value 0–255) to RGB
void hsvToRgb(int h, uint8_t s, uint8_t v, uint8_t &r, uint8_t &g, uint8_t &b) {
    h = h % 360;
    uint8_t region = h / 60;
    uint8_t remainder = (h % 60) * 255 / 60;

    uint8_t p = (v * (255 - s)) / 255;
    uint8_t q = (v * (255 - (s * remainder) / 255)) / 255;
    uint8_t t = (v * (255 - (s * (255 - remainder)) / 255)) / 255;

    switch (region) {
        case 0:  r = v; g = t; b = p; break;
        case 1:  r = q; g = v; b = p; break;
        case 2:  r = p; g = v; b = t; break;
        case 3:  r = p; g = q; b = v; break;
        case 4:  r = t; g = p; b = v; break;
        default: r = v; g = p; b = q; break;
    }
}

void setup() {
    Serial.begin(115200);
    buddy.begin("NeoPixel-RGB");

    buddy.led.setStripCount(8);
}

void loop() {
    buddy.update();

    if (!buddy.isConnected()) return;

    // Speed from Slider 0 (1–10, default 3 if slider at 0)
    uint8_t speedVal = buddy.input.slider(0);
    int speed = (speedVal > 0) ? map(speedVal, 0, 255, 1, 10) : 3;

    // Brightness from Slider 1 (50–255, default 200 if slider at 0)
    uint8_t brightVal = buddy.input.slider(1);
    uint8_t brightness = (brightVal > 0) ? map(brightVal, 0, 255, 50, 255) : 200;

    // Update each pixel with a rainbow gradient
    for (int i = 0; i < 8; i++) {
        // Spread 360° of hue across 8 pixels, offset by animation frame
        int hue = (hueOffset + i * 45) % 360;  // 45° spacing = 360/8
        uint8_t r, g, b;
        hsvToRgb(hue, 255, brightness, r, g, b);
        buddy.led.setStripPixel(i, r, g, b);
    }

    // Advance the animation
    hueOffset = (hueOffset + speed) % 360;

    // Show current speed and brightness on LCD
    char line0[17], line1[17];
    snprintf(line0, sizeof(line0), "Rainbow         ");
    snprintf(line1, sizeof(line1), "Spd:%-2d Brt:%-3d  ", speed, brightness);
    buddy.lcd.print(0, 0, line0);
    buddy.lcd.print(1, 0, line1);

    delay(30);
}
