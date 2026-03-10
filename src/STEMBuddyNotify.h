/**
 * STEMBuddyNotify.h
 *
 * Push Notifications: ESP32 sends notifications to the phone.
 * Notifications appear as system notifications even when the app is in the background.
 * Supports rich notifications with title, body, and icon type.
 *
 * Usage:
 *   buddy.notify.send("Alert!", "Temperature exceeded 40°C");
 *   buddy.notify.send("Done", "Calibration complete", STEMBuddyNotify::SUCCESS);
 *   buddy.notify.send("Warning", "Low battery", STEMBuddyNotify::WARNING);
 *   buddy.notify.clear();  // Clear all notifications
 */

#ifndef STEMBUDDY_NOTIFY_H
#define STEMBUDDY_NOTIFY_H

#include <Arduino.h>

class STEMBuddy;

class STEMBuddyNotify {
    friend class STEMBuddy;

public:
    // Icon types for notifications
    enum Icon : uint8_t {
        INFO    = 0,
        SUCCESS = 1,
        WARNING = 2,
        ERROR   = 3
    };

    STEMBuddyNotify() : _parent(nullptr) {}
    void begin(STEMBuddy* parent) { _parent = parent; }

    /**
     * Send a notification to the phone.
     * @param title  Notification title (any length, sent in 18-byte chunks)
     * @param body   Notification body text (any length, sent in 18-byte chunks)
     * @param icon   Icon type: INFO (default), SUCCESS, WARNING, ERROR
     */
    void send(const char* title, const char* body, Icon icon = INFO);
    void send(const String& title, const String& body, Icon icon = INFO) {
        send(title.c_str(), body.c_str(), icon);
    }

    /** Clear all notifications from the phone. */
    void clear();

private:
    STEMBuddy* _parent;
};

#endif
