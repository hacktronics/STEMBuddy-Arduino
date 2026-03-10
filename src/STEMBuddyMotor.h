/**
 * STEMBuddyMotor.h
 *
 * Virtual DC Motor. Control speed and direction from your ESP32.
 *
 * Usage:
 *   buddy.motor.setSpeed(200);     // PWM 0-255
 *   buddy.motor.setDirection(1);   // 0=CW, 1=CCW
 *   buddy.motor.stop();
 */

#ifndef STEMBUDDY_MOTOR_H
#define STEMBUDDY_MOTOR_H

#include <Arduino.h>

class STEMBuddy;

class STEMBuddyMotor {
public:
    STEMBuddyMotor() : _parent(nullptr) {}
    void begin(STEMBuddy* parent) { _parent = parent; }

    /** Set motor speed (0-255 PWM). */
    void setSpeed(uint8_t speed);

    /** Set direction: 0 = clockwise, 1 = counter-clockwise. */
    void setDirection(uint8_t dir);

    /** Stop the motor (speed = 0). */
    void stop();

private:
    STEMBuddy* _parent;
};

#endif
