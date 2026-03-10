/**
 * STEMBuddyTerminal.h
 *
 * Bidirectional serial terminal between ESP32 and the app.
 * ESP32 can print text to the app terminal, and read user input.
 *
 * Usage:
 *   buddy.terminal.print("Hello from ESP32!");
 *   buddy.terminal.println("Line 2");
 *   buddy.terminal.clear();
 *
 *   if (buddy.terminal.available()) {
 *     String input = buddy.terminal.read();
 *   }
 */

#ifndef STEMBUDDY_TERMINAL_H
#define STEMBUDDY_TERMINAL_H

#include <Arduino.h>

class STEMBuddy;

class STEMBuddyTerminal {
    friend class STEMBuddy;

public:
    STEMBuddyTerminal() : _parent(nullptr), _bufLen(0) {}
    void begin(STEMBuddy* parent) { _parent = parent; }

    /** Print text to the app terminal. Sent in 18-byte chunks over BLE. */
    void print(const char* text);
    void print(const String& text) { print(text.c_str()); }
    void println(const char* text);
    void println(const String& text) { println(text.c_str()); }

    /** Clear the app terminal. */
    void clear();

    /** Check if user typed something in the app terminal. */
    bool available() { return _bufLen > 0; }

    /** Read the last user input. Clears the buffer after reading. */
    String read() {
        String s(_inputBuf, _bufLen);
        _bufLen = 0;
        return s;
    }

private:
    STEMBuddy* _parent;
    char _inputBuf[64];
    uint8_t _bufLen;

    void _onInput(const uint8_t* data, uint8_t len) {
        uint8_t copyLen = (len > 63) ? 63 : len;
        memcpy(_inputBuf, data, copyLen);
        _inputBuf[copyLen] = 0;
        _bufLen = copyLen;
    }
};

#endif
