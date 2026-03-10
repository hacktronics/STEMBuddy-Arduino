/**
 * STEMBuddyBarGraph.h
 *
 * Virtual LED Bar Graph — display bar levels on the STEM Buddy app.
 *
 * Usage:
 *   buddy.barGraph.set(0, 200);             // Set bar 0 to value 200
 *   buddy.barGraph.setAll(values, 10);      // Set all 10 bars
 *   buddy.barGraph.setColor(1);             // 0=green, 1=red, 2=yellow, 3=blue
 *   buddy.barGraph.clear();                 // Turn all off
 */

#ifndef STEMBUDDY_BARGRAPH_H
#define STEMBUDDY_BARGRAPH_H

#include <Arduino.h>

class STEMBuddy;

class STEMBuddyBarGraph {
    friend class STEMBuddy;

public:
    STEMBuddyBarGraph() : _parent(nullptr) {}
    void begin(STEMBuddy* parent) { _parent = parent; }

    /** Set a single bar value (index 0-9, value 0-255). */
    void set(uint8_t index, uint8_t value);

    /** Set all 10 bars at once. */
    void setAll(const uint8_t* values, uint8_t count = 10);

    /** Set color palette: 0=green, 1=red, 2=yellow, 3=blue. */
    void setColor(uint8_t colorIndex);

    /** Clear all bars. */
    void clear();

private:
    STEMBuddy* _parent;
};

#endif
