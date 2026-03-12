/**
 * STEMBuddyStepper.h
 *
 * Virtual Stepper Motor — animate a stepper on the STEM Buddy app.
 *
 * Usage:
 *   buddy.stepper.step(200);         // 200 steps clockwise
 *   buddy.stepper.step(-200);        // 200 steps counter-clockwise
 *   buddy.stepper.setAngle(90);      // Set to 90 degrees
 *   buddy.stepper.setRPM(60);        // Set speed to 60 RPM
 *   buddy.stepper.reset();           // Reset to 0 degrees
 */

#ifndef STEMBUDDY_STEPPER_H
#define STEMBUDDY_STEPPER_H

#include <Arduino.h>

class STEMBuddy;

class STEMBuddyStepper {
    friend class STEMBuddy;

public:
    /** Direction constants — use instead of raw 0/1. */
    static const uint8_t CLOCKWISE = 0;
    static const uint8_t COUNTER_CLOCKWISE = 1;

    STEMBuddyStepper() : _parent(nullptr) {}
    void begin(STEMBuddy* parent) { _parent = parent; }

    /**
     * Move a number of steps.
     * Positive = clockwise, negative = counter-clockwise.
     */
    void step(int16_t steps);

    /** Set absolute angle (0-360). */
    void setAngle(uint16_t angle);

    /** Set speed in RPM (0-255). */
    void setRPM(uint8_t rpm);

    /** Reset angle to 0. */
    void reset();

private:
    STEMBuddy* _parent;
};

#endif
