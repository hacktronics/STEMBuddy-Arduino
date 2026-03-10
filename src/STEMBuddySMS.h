/**
 * STEMBuddySMS.h
 *
 * SMS: ESP32 triggers the phone to send a real SMS silently.
 * The phone sends the SMS in the background without any user interaction.
 *
 * Usage:
 *   buddy.sms.send("+1234567890", "Hello from ESP32!");
 *   buddy.sms.send("+1234567890", "Temperature is 42°C — alert!");
 */

#ifndef STEMBUDDY_SMS_H
#define STEMBUDDY_SMS_H

#include <Arduino.h>

class STEMBuddy;

class STEMBuddySMS {
    friend class STEMBuddy;

public:
    STEMBuddySMS() : _parent(nullptr) {}
    void begin(STEMBuddy* parent) { _parent = parent; }

    /**
     * Send an SMS silently from the phone.
     * @param number  Phone number (e.g. "+1234567890")
     * @param message Message body (any length, sent in 18-byte chunks)
     */
    void send(const char* number, const char* message);
    void send(const String& number, const String& message) {
        send(number.c_str(), message.c_str());
    }

private:
    STEMBuddy* _parent;
};

#endif
