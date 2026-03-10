/**
 * STEMBuddyRFID.h
 *
 * Virtual RFID / MFRC522 — the phone sends card UIDs to your ESP32.
 * The app scans NFC tags or simulates RFID cards.
 *
 * Usage:
 *   if (buddy.rfid.cardPresent()) {
 *     uint8_t uid[4];
 *     buddy.rfid.getUID(uid);
 *     // Process card UID...
 *   }
 */

#ifndef STEMBUDDY_RFID_H
#define STEMBUDDY_RFID_H

#include <Arduino.h>

class STEMBuddy;

class STEMBuddyRFID {
    friend class STEMBuddy;

public:
    STEMBuddyRFID() : _parent(nullptr), _present(false) {
        memset(_uid, 0, 4);
    }
    void begin(STEMBuddy* parent) { _parent = parent; }

    /** Check if a card is present. */
    bool cardPresent() const { return _present; }

    /** Copy the 4-byte card UID into the provided buffer. */
    void getUID(uint8_t uid[4]) const { memcpy(uid, _uid, 4); }

private:
    STEMBuddy* _parent;
    uint8_t _uid[4];
    bool _present;

    void _onCardUID(const uint8_t* payload, uint8_t len) {
        if (len >= 4) memcpy(_uid, payload, 4);
        _present = true;
    }
    void _onCardRemoved() {
        _present = false;
        memset(_uid, 0, 4);
    }
};

#endif
