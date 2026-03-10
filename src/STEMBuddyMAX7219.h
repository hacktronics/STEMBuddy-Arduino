/**
 * STEMBuddyMAX7219.h
 *
 * Virtual MAX7219 Dot Matrix — control an 8x8 LED matrix on the STEM Buddy app.
 *
 * Usage:
 *   buddy.matrix.setRow(0, 3, 0b10101010);  // Set module 0, row 3
 *   buddy.matrix.setAll(rowData);            // Set all 8 rows of module 0
 *   buddy.matrix.setChain(2);               // Use 2 chained modules
 *   buddy.matrix.setIntensity(8);           // Brightness 0-15
 *   buddy.matrix.setColor(255, 0, 0);       // Red LEDs
 *   buddy.matrix.clear();                   // Blank all modules
 */

#ifndef STEMBUDDY_MAX7219_H
#define STEMBUDDY_MAX7219_H

#include <Arduino.h>

class STEMBuddy;

class STEMBuddyMAX7219 {
    friend class STEMBuddy;

public:
    STEMBuddyMAX7219() : _parent(nullptr) {}
    void begin(STEMBuddy* parent) { _parent = parent; }

    /** Set a single row of a module. bits = 8-bit row pattern. */
    void setRow(uint8_t module, uint8_t row, uint8_t bits);

    /** Set all 8 rows of module 0 at once. */
    void setAll(const uint8_t rows[8]);

    /** Clear all modules. */
    void clear();

    /** Set brightness (0-15). */
    void setIntensity(uint8_t level);

    /** Set number of chained modules (1-4). */
    void setChain(uint8_t count);

    /** Set LED color. */
    void setColor(uint8_t r, uint8_t g, uint8_t b);

private:
    STEMBuddy* _parent;
};

#endif
