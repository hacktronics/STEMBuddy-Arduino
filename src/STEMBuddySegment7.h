/**
 * STEMBuddySegment7.h
 *
 * Virtual 7-Segment Display — show digits on the STEM Buddy app.
 *
 * Usage:
 *   buddy.segment7.setDigits(1, 2, 3, 4);   // Show "1234"
 *   buddy.segment7.setColon(true);            // Enable colon (clock mode)
 *   buddy.segment7.setDecimal(0b0100);        // Decimal after 3rd digit
 *   buddy.segment7.setColor(255, 0, 0);       // Red segments
 *   buddy.segment7.clear();                   // Blank display
 */

#ifndef STEMBUDDY_SEGMENT7_H
#define STEMBUDDY_SEGMENT7_H

#include <Arduino.h>

class STEMBuddy;

class STEMBuddySegment7 {
    friend class STEMBuddy;

public:
    STEMBuddySegment7() : _parent(nullptr) {}
    void begin(STEMBuddy* parent) { _parent = parent; }

    /** Set 4-digit value. Use 0xFF for blank digit. */
    void setDigits(uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3);

    /** Set raw segment bits for up to 8 digits. */
    void setRaw(const uint8_t* segments, uint8_t count);

    /** Set decimal point bitmask (bit 0 = leftmost digit). */
    void setDecimal(uint8_t mask);

    /** Enable/disable colon. */
    void setColon(bool on);

    /** Set segment color. */
    void setColor(uint8_t r, uint8_t g, uint8_t b);

    /** Clear (blank) all digits. */
    void clear();

private:
    STEMBuddy* _parent;
};

#endif
