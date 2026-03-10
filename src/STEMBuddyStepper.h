/**
 * STEMBuddyStepper.h
 *
 * Virtual Stepper Motor — animate a stepper on the STEM Buddy app.
 *
 * Usage:
 *   buddy.stepper.step(200, 0);       // 200 steps clockwise
 *   buddy.stepper.setAngle(90);       // Set to 90 degrees
 *   buddy.stepper.setRPM(60);         // Set speed to 60 RPM
 *   buddy.stepper.reset();            // Reset to 0 degrees
 */

#ifndef STEMBUDDY_STEPPER_H
#define STEMBUDDY_STEPPER_H

#include <Arduino.h>

class STEMBuddy;

class STEMBuddyStepper {
    friend class STEMBuddy;

public:
    STEMBuddyStepper() : _parent(nullptr) {}
    void begin(STEMBuddy* parent) { _parent = parent; }

    /** Move a number of steps. dir: 0=CW, 1=CCW. */
    void step(uint16_t steps, uint8_t direction = 0);

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
