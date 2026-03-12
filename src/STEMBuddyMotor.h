/**
 * STEMBuddyMotor.h
 *
 * Virtual DC Motor. Control speed and direction from your ESP32.
 *
 * Usage:
 *   buddy.motor.run(200);     // forward at speed 200
 *   buddy.motor.run(-200);    // backward at speed 200
 *   buddy.motor.stop();
 *
 *   // Named constants (if you prefer separate calls):
 *   buddy.motor.setDirection(STEMBuddyMotor::CLOCKWISE);
 *   buddy.motor.setDirection(STEMBuddyMotor::COUNTER_CLOCKWISE);
 */

#ifndef STEMBUDDY_MOTOR_H
#define STEMBUDDY_MOTOR_H

#include <Arduino.h>

class STEMBuddy;

class STEMBuddyMotor {
public:
    /** Direction constants — use instead of raw 0/1. */
    static const uint8_t CLOCKWISE = 0;
    static const uint8_t COUNTER_CLOCKWISE = 1;

    STEMBuddyMotor() : _parent(nullptr) {}
    void begin(STEMBuddy* parent) { _parent = parent; }

    /**
     * Run the motor at a given speed.
     * Positive = clockwise, negative = counter-clockwise.
     * Range: -255 to 255. Passing 0 stops the motor.
     */
    void run(int16_t speed);

    /** Set motor speed (0-255 PWM). */
    void setSpeed(uint8_t speed);

    /** Set direction: CLOCKWISE (0) or COUNTER_CLOCKWISE (1). */
    void setDirection(uint8_t dir);

    /** Stop the motor (speed = 0). */
    void stop();

private:
    STEMBuddy* _parent;
};

#endif
