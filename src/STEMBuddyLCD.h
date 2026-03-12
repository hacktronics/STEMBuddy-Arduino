/**
 * STEMBuddyLCD.h
 *
 * Virtual 16x2 LCD display. Your ESP32 code sends text to the STEM Buddy app
 * and it appears on a virtual LCD screen — just like a real LiquidCrystal display.
 *
 * Usage:
 *   buddy.lcd.clear();
 *   buddy.lcd.print(0, 0, "Hello World!");  // Row 0, Col 0
 *   buddy.lcd.print(1, 0, "Temperature: ");
 *   buddy.lcd.setCursor(1, 13);
 *   buddy.lcd.print("25C");                 // Print text at cursor
 *   buddy.lcd.print(42);                    // Print int at cursor
 *   buddy.lcd.print(3.14);                  // Print float at cursor
 */

#ifndef STEMBUDDY_LCD_H
#define STEMBUDDY_LCD_H

#include <Arduino.h>

class STEMBuddy;

class STEMBuddyLCD {
public:
    STEMBuddyLCD() : _parent(nullptr), _cursorRow(0), _cursorCol(0) {}
    void begin(STEMBuddy* parent) { _parent = parent; }

    /** Clear the entire display. */
    void clear();

    /** Print text at specific row and column. */
    void print(uint8_t row, uint8_t col, const char* text);

    /** Print text at current cursor position. */
    void print(const char* text);

    /** Print a number at current cursor position. */
    void print(int value);

    /** Print a float at current cursor position. */
    void print(float value, uint8_t decimals = 2);

    /** Print a number at specific position. */
    void print(uint8_t row, uint8_t col, int value);

    /** Print a float at specific position. */
    void print(uint8_t row, uint8_t col, float value, uint8_t decimals = 2);

    /** Set cursor position for subsequent print() calls. */
    void setCursor(uint8_t row, uint8_t col);

    /** Turn backlight on. */
    void backlightOn();

    /** Turn backlight off. */
    void backlightOff();

    /** Set backlight color (RGB). */
    void setBacklightColor(uint8_t r, uint8_t g, uint8_t b);

    /** Scroll display left by one position. */
    void scrollLeft();

    /** Scroll display right by one position. */
    void scrollRight();

    /** Create a custom character (slot 0-7, 8 rows of 5 bits). */
    void createChar(uint8_t slot, const uint8_t charmap[8]);

private:
    STEMBuddy* _parent;
    uint8_t _cursorRow;
    uint8_t _cursorCol;
};

#endif
