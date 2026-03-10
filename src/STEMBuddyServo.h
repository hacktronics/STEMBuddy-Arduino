/**
 * STEMBuddyServo.h
 *
 * Virtual Servo Motor. Send angle commands from ESP32
 * and the STEM Buddy app renders a servo at that position.
 *
 * Usage:
 *   buddy.servo.setAngle(90);   // Move to 90 degrees
 *   buddy.servo.setAngle(0);    // Move to 0
 *   buddy.servo.detach();       // Release the servo
 */

#ifndef STEMBUDDY_SERVO_H
#define STEMBUDDY_SERVO_H

#include <Arduino.h>

class STEMBuddy;

class STEMBuddyServo {
public:
    STEMBuddyServo() : _parent(nullptr) {}
    void begin(STEMBuddy* parent) { _parent = parent; }

    /** Set servo angle (0-180 degrees). */
    void setAngle(uint8_t angle);

    /** Convenience: same as setAngle(). */
    void write(uint8_t angle) { setAngle(angle); }

    /** Detach servo — stop holding position. */
    void detach();

private:
    STEMBuddy* _parent;
};

#endif
