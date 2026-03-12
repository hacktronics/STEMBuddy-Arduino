/**
 * STEM Buddy — Virtual OLED Display (SSD1306)
 *
 * Drives a virtual 128×64 monochrome OLED display on the STEM Buddy
 * app. The API mirrors the popular Adafruit SSD1306 library, so code
 * you write here can easily be ported to a real OLED later.
 *
 * This example demonstrates all the drawing primitives: text at
 * different sizes, rectangles, circles, lines, and pixel-level
 * drawing. It then shows a live sensor dashboard with temperature,
 * humidity, and a real-time bar chart.
 *
 * What you'll learn:
 *   - Drawing text at different sizes (setTextSize)
 *   - Drawing shapes (rect, circle, line, pixel)
 *   - Building a sensor dashboard on a graphical display
 *   - Understanding the clearDisplay() → draw → display() cycle
 *   - Porting Adafruit GFX-style code to STEMBuddy
 *
 * Hardware needed: ESP32 only
 *
 * Steps:
 *   1. Upload this sketch to your ESP32
 *   2. Open STEM Buddy and connect via Bluetooth
 *   3. Watch the OLED demo cycle through shapes and text
 *   4. Then see the live sensor dashboard
 *
 * Created by CodeSkool — https://codeskool.cc
 */

#include <STEMBuddy.h>

STEMBuddy buddy;

unsigned long lastFrame = 0;
int demoPhase = 0;
unsigned long phaseStart = 0;

// --- Demo Phase 1: Text rendering at different sizes ---
void demoText() {
    buddy.oled.clearDisplay();

    buddy.oled.setTextSize(1);
    buddy.oled.setCursor(0, 0);
    buddy.oled.print("STEM Buddy OLED");

    buddy.oled.setTextSize(2);
    buddy.oled.setCursor(0, 16);
    buddy.oled.print("Hello!");

    buddy.oled.setTextSize(1);
    buddy.oled.setCursor(0, 40);
    buddy.oled.print("128x64 pixels");
    buddy.oled.setCursor(0, 52);
    buddy.oled.print("No real OLED needed");

    buddy.oled.display();
}

// --- Demo Phase 2: Drawing shapes ---
void demoShapes() {
    buddy.oled.clearDisplay();

    // Draw rectangle outlines
    buddy.oled.drawRect(0, 0, 40, 30);
    buddy.oled.drawRect(5, 5, 30, 20);

    // Draw filled rectangle
    buddy.oled.fillRect(50, 5, 25, 20);

    // Draw circles
    buddy.oled.drawCircle(100, 15, 12);
    buddy.oled.fillCircle(100, 15, 5);

    // Draw lines
    buddy.oled.drawLine(0, 40, 127, 40);
    buddy.oled.drawLine(0, 63, 127, 40);
    buddy.oled.drawLine(127, 63, 0, 40);

    // Label
    buddy.oled.setTextSize(1);
    buddy.oled.setCursor(0, 50);
    buddy.oled.print("Shapes Demo");

    buddy.oled.display();
}

// --- Demo Phase 3: Animated bouncing pixel ---
void demoBounce(int frame) {
    buddy.oled.clearDisplay();

    // Bouncing ball
    int x = 64 + (int)(50.0 * sin(frame * 0.08));
    int y = 32 + (int)(20.0 * cos(frame * 0.12));
    buddy.oled.fillCircle(x, y, 4);

    // Trail circles (smaller, outline only)
    for (int i = 1; i <= 3; i++) {
        int tx = 64 + (int)(50.0 * sin((frame - i * 4) * 0.08));
        int ty = 32 + (int)(20.0 * cos((frame - i * 4) * 0.12));
        buddy.oled.drawCircle(tx, ty, 4 - i);
    }

    // Border
    buddy.oled.drawRect(0, 0, 128, 64);

    buddy.oled.display();
}

// --- Demo Phase 4: Live sensor dashboard ---
void demoSensorDashboard() {
    buddy.oled.clearDisplay();

    float temp     = buddy.sensor.temperature();
    float humidity = buddy.sensor.humidity();
    uint16_t lux   = buddy.sensor.light();

    // Title bar
    buddy.oled.fillRect(0, 0, 128, 12);
    buddy.oled.setTextSize(1);
    // Title text drawn in inverse (over filled rect = visible)
    buddy.oled.setCursor(20, 2);
    buddy.oled.print("SENSOR DASH");

    // Temperature
    buddy.oled.setCursor(0, 16);
    buddy.oled.print("Temp:");
    char buf[16];
    snprintf(buf, sizeof(buf), "%.1f C", temp);
    buddy.oled.setCursor(60, 16);
    buddy.oled.print(buf);

    // Humidity
    buddy.oled.setCursor(0, 28);
    buddy.oled.print("Humid:");
    snprintf(buf, sizeof(buf), "%.0f %%", humidity);
    buddy.oled.setCursor(60, 28);
    buddy.oled.print(buf);

    // Light
    buddy.oled.setCursor(0, 40);
    buddy.oled.print("Light:");
    snprintf(buf, sizeof(buf), "%d lux", lux);
    buddy.oled.setCursor(60, 40);
    buddy.oled.print(buf);

    // Temperature bar graph at bottom
    int barWidth = map(constrain((int)temp, 0, 50), 0, 50, 0, 120);
    buddy.oled.drawRect(3, 54, 122, 8);
    buddy.oled.fillRect(4, 55, barWidth, 6);

    buddy.oled.display();
}

void setup() {
    Serial.begin(115200);
    buddy.begin("OLED-Display");

    phaseStart = millis();
}

void loop() {
    buddy.update();

    if (!buddy.isConnected()) return;

    unsigned long elapsed = millis() - phaseStart;

    switch (demoPhase) {
        case 0: // Text demo — show for 4 seconds
            if (elapsed < 100) demoText();
            if (elapsed > 4000) { demoPhase = 1; phaseStart = millis(); }
            break;

        case 1: // Shapes demo — show for 4 seconds
            if (elapsed < 100) demoShapes();
            if (elapsed > 4000) { demoPhase = 2; phaseStart = millis(); }
            break;

        case 2: // Bouncing ball — run for 5 seconds
            if (millis() - lastFrame > 50) {
                lastFrame = millis();
                demoBounce((int)(elapsed / 50));
            }
            if (elapsed > 5000) { demoPhase = 3; phaseStart = millis(); }
            break;

        case 3: // Sensor dashboard — runs indefinitely, updates at 2 Hz
            if (millis() - lastFrame > 500) {
                lastFrame = millis();
                demoSensorDashboard();
            }
            break;
    }

    delay(20);
}
