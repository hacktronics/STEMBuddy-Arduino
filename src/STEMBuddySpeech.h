/**
 * STEMBuddySpeech.h
 *
 * Speech Recognition: The phone listens via microphone and sends
 * recognized text to ESP32 over BLE.
 *
 * Usage:
 *   void loop() {
 *     buddy.update();
 *     if (buddy.speech.available()) {
 *       String text = buddy.speech.lastResult();
 *       Serial.println("You said: " + text);
 *     }
 *   }
 */

#ifndef STEMBUDDY_SPEECH_H
#define STEMBUDDY_SPEECH_H

#include <Arduino.h>

class STEMBuddy;

class STEMBuddySpeech {
    friend class STEMBuddy;

public:
    STEMBuddySpeech() : _parent(nullptr), _available(false), _listening(false), _error(0), _hasError(false) {}
    void begin(STEMBuddy* parent) { _parent = parent; }

    /** Returns true if a new speech result is ready (clears the flag). */
    bool available() {
        if (_available) {
            _available = false;
            return true;
        }
        return false;
    }

    /** Get the last recognized text. */
    String lastResult() const { return _lastResult; }

    /** Check if speech recognition is currently listening. */
    bool isListening() const { return _listening; }

    /** Check if there was an error. */
    bool hasError() const { return _hasError; }

    /** Get last error code: 0=no-speech, 1=no-match, 2=not-allowed, 3=aborted */
    uint8_t lastError() {
        _hasError = false;
        return _error;
    }

private:
    STEMBuddy* _parent;
    String _lastResult;
    String _resultBuffer;   // Accumulates chunks
    bool _available;
    bool _listening;
    uint8_t _error;
    bool _hasError;

    // Called by STEMBuddy::_processMessage when STT data arrives
    void _onResultChunk(const uint8_t* data, uint8_t len) {
        for (uint8_t i = 0; i < len; i++) {
            _resultBuffer += (char)data[i];
        }
    }

    void _onResultEnd() {
        _lastResult = _resultBuffer;
        _resultBuffer = "";
        _available = true;
    }

    void _onError(uint8_t code) {
        _error = code;
        _hasError = true;
        _listening = false;
    }

    void _onStatus(uint8_t status) {
        _listening = (status == 1);
    }
};

#endif
