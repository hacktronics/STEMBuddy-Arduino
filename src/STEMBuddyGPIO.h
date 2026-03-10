/**
 * STEMBuddyGPIO.h
 *
 * Virtual GPIO Monitor — report pin states to the STEM Buddy app.
 *
 * Usage:
 *   buddy.gpio.reportDigital(13, HIGH, OUTPUT);  // Report pin 13 state
 *   buddy.gpio.reportAnalog(34, analogRead(34));  // Report ADC value
 */

#ifndef STEMBUDDY_GPIO_H
#define STEMBUDDY_GPIO_H

#include <Arduino.h>

class STEMBuddy;

class STEMBuddyGPIO {
    friend class STEMBuddy;

public:
    STEMBuddyGPIO() : _parent(nullptr) {}
    void begin(STEMBuddy* parent) { _parent = parent; }

    /**
     * Report a digital pin state to the app.
     * @param pin    GPIO pin number
     * @param state  0 or 1 (LOW/HIGH)
     * @param mode   Pin mode (INPUT=0, OUTPUT=1, INPUT_PULLUP=2)
     */
    void reportDigital(uint8_t pin, uint8_t state, uint8_t mode);

    /**
     * Report an analog pin value to the app.
     * @param pin    GPIO pin number
     * @param value  ADC reading (0-4095 for ESP32)
     */
    void reportAnalog(uint8_t pin, uint16_t value);

private:
    STEMBuddy* _parent;
};

#endif
