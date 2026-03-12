/**
 * STEMBuddyOLED.h
 *
 * Virtual 128x64 monochrome OLED display (SSD1306). Your ESP32 code sends
 * draw commands to the STEM Buddy app and it renders them on a virtual OLED
 * screen — just like a real Adafruit SSD1306 display.
 *
 * API mirrors the Adafruit SSD1306 / GFX library for familiarity:
 *   buddy.oled.clearDisplay();
 *   buddy.oled.setCursor(0, 0);
 *   buddy.oled.setTextSize(2);
 *   buddy.oled.print("Hello!");
 *   buddy.oled.drawRect(10, 20, 50, 30, false);
 *   buddy.oled.display();  // flush to app
 */

#ifndef STEMBUDDY_OLED_H
#define STEMBUDDY_OLED_H

#include <Arduino.h>

class STEMBuddy;

class STEMBuddyOLED {
public:
    STEMBuddyOLED() : _parent(nullptr), _cursorX(0), _cursorY(0), _textSize(1) {}
    void begin(STEMBuddy* parent) { _parent = parent; }

    /** Clear the entire display framebuffer. */
    void clearDisplay();

    /** Flush the framebuffer to the app (must call after drawing). */
    void display();

    /** Set cursor position in pixels for text rendering. */
    void setCursor(uint8_t x, uint8_t y);

    /** Set text size multiplier (1-4). */
    void setTextSize(uint8_t size);

    /** Print text at current cursor position. */
    void print(const char* text);

    /** Print an integer at current cursor position. */
    void print(int value);

    /** Print a float at current cursor position. */
    void print(float value, uint8_t decimals = 2);

    /** Draw a single pixel. color: 1=on, 0=off. */
    void drawPixel(uint8_t x, uint8_t y, uint8_t color = 1);

    /** Draw a line from (x0,y0) to (x1,y1). */
    void drawLine(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1);

    /** Draw a rectangle outline. */
    void drawRect(uint8_t x, uint8_t y, uint8_t w, uint8_t h);

    /** Draw a filled rectangle. */
    void fillRect(uint8_t x, uint8_t y, uint8_t w, uint8_t h);

    /** Draw a circle outline. */
    void drawCircle(uint8_t cx, uint8_t cy, uint8_t r);

    /** Draw a filled circle. */
    void fillCircle(uint8_t cx, uint8_t cy, uint8_t r);

    /** Invert the display colors. */
    void invertDisplay(bool invert);

private:
    STEMBuddy* _parent;
    uint8_t _cursorX;
    uint8_t _cursorY;
    uint8_t _textSize;
};

#endif
