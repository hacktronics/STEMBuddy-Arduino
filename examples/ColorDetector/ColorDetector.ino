/**
 * STEM Buddy — Color Detector
 *
 * Uses the phone's camera as a color sensor! Point your phone at any
 * colored object — the detected RGB values are sent to your ESP32,
 * which then displays the color on the virtual RGB LED and shows the
 * exact values on the LCD.
 *
 * The NeoPixel strip shows a gradient from red to the detected color,
 * making it easy to compare.
 *
 * What you'll learn:
 *   - Reading color sensor data (R, G, B channels)
 *   - Understanding RGB color representation
 *   - Mapping real-world colors to digital values
 *   - Driving multiple virtual outputs from one sensor
 *
 * Sensors used:
 *   - Color sensor R → buddy.sensor.colorR()
 *   - Color sensor G → buddy.sensor.colorG()
 *   - Color sensor B → buddy.sensor.colorB()
 *
 * Hardware needed: ESP32 only
 *
 * Steps:
 *   1. Upload this sketch to your ESP32
 *   2. Open STEM Buddy and connect via Bluetooth
 *   3. Point your phone's camera at a colored object
 *   4. The LED matches the detected color!
 *
 * Created by CodeSkool — https://codeskool.cc
 */

#include <STEMBuddy.h>

STEMBuddy buddy;

unsigned long lastUpdate = 0;

// Determine the dominant color name for LCD display
const char* colorName(uint8_t r, uint8_t g, uint8_t b) {
    if (r < 30 && g < 30 && b < 30)   return "Black  ";
    if (r > 200 && g > 200 && b > 200) return "White  ";
    if (r > g && r > b)                return "Red    ";
    if (g > r && g > b)                return "Green  ";
    if (b > r && b > g)                return "Blue   ";
    if (r > 200 && g > 200)            return "Yellow ";
    if (r > 200 && b > 200)            return "Magenta";
    if (g > 200 && b > 200)            return "Cyan   ";
    return "Mixed  ";
}

void setup() {
    Serial.begin(115200);
    buddy.begin("Color-Detect");

    buddy.led.setStripCount(8);
}

void loop() {
    buddy.update();

    if (!buddy.isConnected()) return;

    if (millis() - lastUpdate < 200) return;
    lastUpdate = millis();

    // Read detected color from the phone's camera
    uint8_t r = buddy.sensor.colorR();
    uint8_t g = buddy.sensor.colorG();
    uint8_t b = buddy.sensor.colorB();

    // Show the detected color on the main LED
    buddy.led.setColor(r, g, b);
    buddy.led.on();

    // Fill NeoPixel strip with the detected color
    buddy.led.fillStrip(r, g, b);

    // Display RGB values and color name on LCD
    char line0[17], line1[17];
    snprintf(line0, sizeof(line0), "R:%-3d G:%-3d B:%-3d", r, g, b);
    snprintf(line1, sizeof(line1), "%s         ", colorName(r, g, b));
    buddy.lcd.print(0, 0, line0);
    buddy.lcd.print(1, 0, line1);

    // Match LCD backlight to detected color
    buddy.lcd.setBacklightColor(r, g, b);
}
