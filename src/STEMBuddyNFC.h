/**
 * STEMBuddyNFC.h
 *
 * NFC Tag Reader: The phone reads real NFC tags using its built-in NFC
 * hardware and sends the tag UID, type, and NDEF data to ESP32 over BLE.
 *
 * Usage:
 *   void loop() {
 *     buddy.update();
 *     if (buddy.nfc.tagPresent()) {
 *       uint8_t uid[7];
 *       uint8_t len = buddy.nfc.getUID(uid);
 *       Serial.print("NFC UID: ");
 *       for (int i = 0; i < len; i++) {
 *         Serial.printf("%02X", uid[i]);
 *         if (i < len - 1) Serial.print(":");
 *       }
 *       Serial.println();
 *
 *       String text = buddy.nfc.getNdefText();
 *       if (text.length() > 0) {
 *         Serial.println("NDEF: " + text);
 *       }
 *     }
 *   }
 */

#ifndef STEMBUDDY_NFC_H
#define STEMBUDDY_NFC_H

#include <Arduino.h>

class STEMBuddy;

class STEMBuddyNFC {
    friend class STEMBuddy;

public:
    STEMBuddyNFC() : _parent(nullptr), _tagPresent(false), _uidLen(0), _tagType(0) {
        memset(_uid, 0, sizeof(_uid));
    }
    void begin(STEMBuddy* parent) { _parent = parent; }

    /** Returns true if an NFC tag is currently detected. */
    bool tagPresent() const { return _tagPresent; }

    /**
     * Copy the tag UID into the provided buffer.
     * @param uid  Output buffer (must be at least 7 bytes).
     * @return     Number of UID bytes (typically 4 or 7), or 0 if no tag.
     */
    uint8_t getUID(uint8_t* uid) const {
        if (!_tagPresent) return 0;
        memcpy(uid, _uid, _uidLen);
        return _uidLen;
    }

    /** Get the UID as a colon-separated hex string (e.g., "04:A3:2B:C1:00:00:00"). */
    String getUIDString() const {
        if (!_tagPresent || _uidLen == 0) return "";
        String s;
        for (uint8_t i = 0; i < _uidLen; i++) {
            if (i > 0) s += ':';
            if (_uid[i] < 0x10) s += '0';
            s += String(_uid[i], HEX);
        }
        s.toUpperCase();
        return s;
    }

    /**
     * Get the tag type ID.
     * 0=unknown, 1=NfcA, 2=NfcB, 3=NfcF, 4=NfcV, 5=IsoDep, 6=Mifare
     */
    uint8_t getType() const { return _tagType; }

    /** Get the last NDEF text record content (empty if none). */
    String getNdefText() const { return _ndefText; }

    /** Get the last NDEF URI record content (empty if none). */
    String getNdefUri() const { return _ndefUri; }

private:
    STEMBuddy* _parent;
    bool    _tagPresent;
    uint8_t _uid[7];
    uint8_t _uidLen;
    uint8_t _tagType;
    String  _ndefText;
    String  _ndefUri;
    String  _ndefTextBuffer;
    String  _ndefUriBuffer;

    // Called by STEMBuddy::_processMessage when NFC data arrives
    void _onTagUID(const uint8_t* data, uint8_t len) {
        if (len < 1) return;
        _uidLen = data[0];
        if (_uidLen > 7) _uidLen = 7;
        memset(_uid, 0, sizeof(_uid));
        for (uint8_t i = 0; i < _uidLen && (i + 1) < len; i++) {
            _uid[i] = data[i + 1];
        }
        _tagPresent = true;
        // Clear NDEF buffers for new tag
        _ndefTextBuffer = "";
        _ndefUriBuffer = "";
    }

    void _onTagType(uint8_t type) {
        _tagType = type;
    }

    void _onNdefText(const uint8_t* data, uint8_t len) {
        for (uint8_t i = 0; i < len; i++) {
            _ndefTextBuffer += (char)data[i];
        }
    }

    void _onNdefUri(const uint8_t* data, uint8_t len) {
        for (uint8_t i = 0; i < len; i++) {
            _ndefUriBuffer += (char)data[i];
        }
    }

    void _onNdefEnd() {
        _ndefText = _ndefTextBuffer;
        _ndefUri = _ndefUriBuffer;
        _ndefTextBuffer = "";
        _ndefUriBuffer = "";
    }

    void _onTagRemoved() {
        _tagPresent = false;
        _uidLen = 0;
        memset(_uid, 0, sizeof(_uid));
        _tagType = 0;
        _ndefText = "";
        _ndefUri = "";
    }
};

#endif
