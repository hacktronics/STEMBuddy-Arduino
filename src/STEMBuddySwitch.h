/**
 * STEMBuddySwitch.h
 *
 * Virtual Home Automation Switches — control switches on the STEM Buddy app
 * and read user-toggled switch state.
 *
 * Usage:
 *   buddy.sw.set(0, true);    // Turn on switch 0
 *   buddy.sw.toggle(1);       // Toggle switch 1
 *
 *   // Read user input (from app):
 *   bool state = buddy.input.switchState(0);
 */

#ifndef STEMBUDDY_SWITCH_H
#define STEMBUDDY_SWITCH_H

#include <Arduino.h>

class STEMBuddy;

class STEMBuddySwitch {
    friend class STEMBuddy;

public:
    STEMBuddySwitch() : _parent(nullptr) {}
    void begin(STEMBuddy* parent) { _parent = parent; }

    /** Set a switch state (ESP32 → App). id: 0-7. */
    void set(uint8_t id, bool on);

    /** Toggle a switch (ESP32 → App). id: 0-7. */
    void toggle(uint8_t id);

private:
    STEMBuddy* _parent;
};

#endif
