/**
 * STEMBuddyScope.h
 *
 * Virtual Oscilloscope. Plot real data from your ESP32 on the phone screen.
 * Perfect for debugging analog signals, sensor readings, or visualizing algorithms.
 *
 * Usage:
 *   int reading = analogRead(A0);
 *   buddy.scope.plot(reading);         // Plot a data point
 *   buddy.scope.setRange(-1000, 1000); // Set Y-axis range
 *   buddy.scope.setLabel("Voltage");   // Label the channel
 *   buddy.scope.clear();               // Clear the trace
 */

#ifndef STEMBUDDY_SCOPE_H
#define STEMBUDDY_SCOPE_H

#include <Arduino.h>

class STEMBuddy;

class STEMBuddyScope {
public:
    STEMBuddyScope() : _parent(nullptr) {}
    void begin(STEMBuddy* parent) { _parent = parent; }

    /** Plot a single data point (signed 16-bit: -32768 to 32767). */
    void plot(int16_t value);

    /** Clear the oscilloscope trace. */
    void clear();

    /** Set the Y-axis range. */
    void setRange(int16_t min, int16_t max);

    /** Set a label for the channel (max 16 chars). */
    void setLabel(const char* label);

private:
    STEMBuddy* _parent;
};

#endif
