/**
 * STEMBuddyRelay.h
 *
 * Virtual Relay — control relay switches on the STEM Buddy app.
 * Uses the Home Automation Switch BLE commands under the hood.
 *
 * Usage:
 *   buddy.relay.on();         // Turn on relay 0
 *   buddy.relay.off();        // Turn off relay 0
 *   buddy.relay.toggle();     // Toggle relay 0
 *
 *   buddy.relay.on(2);        // Turn on relay 2
 *   buddy.relay.off(2);       // Turn off relay 2
 *   buddy.relay.toggle(2);    // Toggle relay 2
 */

#ifndef STEMBUDDY_RELAY_H
#define STEMBUDDY_RELAY_H

#include <Arduino.h>

class STEMBuddy;

class STEMBuddyRelay {
    friend class STEMBuddy;

public:
    STEMBuddyRelay() : _parent(nullptr) {}
    void begin(STEMBuddy* parent) { _parent = parent; }

    /** Turn on a relay. id: 0-7 (default 0). */
    void on(uint8_t id = 0);

    /** Turn off a relay. id: 0-7 (default 0). */
    void off(uint8_t id = 0);

    /** Toggle a relay. id: 0-7 (default 0). */
    void toggle(uint8_t id = 0);

private:
    STEMBuddy* _parent;
};

#endif
