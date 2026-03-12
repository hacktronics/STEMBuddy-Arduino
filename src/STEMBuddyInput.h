/**
 * STEMBuddyInput.h
 *
 * Virtual Buttons, Sliders, D-Pad, Touchpad, and Membrane Keypad.
 * The STEM Buddy app shows these controls on screen and sends events
 * to your ESP32 when the user interacts with them.
 *
 * Usage:
 *   if (buddy.input.buttonPressed(0)) {
 *     // Button 0 was pressed on the phone!
 *     buddy.led.setColor(255, 0, 0);
 *   }
 *
 *   uint8_t speed = buddy.input.slider(0);  // Read slider 0 (0-255)
 *   uint8_t dpad = buddy.input.dpad();
 *   if (dpad == STEMBuddyInput::DPAD_UP) { ... }
 *
 *   // Membrane Keypad (4x4: 1-9, 0, *, #, A-D)
 *   if (buddy.input.keypadPressed()) {
 *     char key = buddy.input.keypad();  // Returns '0'-'9', '*', '#', 'A'-'D'
 *   }
 *
 *   // Home Automation switches (0-7)
 *   if (buddy.input.switchState(0)) { ... }
 */

#ifndef STEMBUDDY_INPUT_H
#define STEMBUDDY_INPUT_H

#include <Arduino.h>

class STEMBuddy;

class STEMBuddyInput {
    friend class STEMBuddy;

public:
    /** D-pad direction constants — use these instead of raw numbers. */
    static const uint8_t DPAD_NONE  = 0;
    static const uint8_t DPAD_UP    = 1;
    static const uint8_t DPAD_RIGHT = 2;
    static const uint8_t DPAD_DOWN  = 3;
    static const uint8_t DPAD_LEFT  = 4;

    STEMBuddyInput() : _parent(nullptr), _dpadState(0), _touchX(0), _touchY(0), _keypadKey(0) {
        memset(_buttons, 0, sizeof(_buttons));
        memset(_sliders, 0, sizeof(_sliders));
        memset(_switches, 0, sizeof(_switches));
    }
    void begin(STEMBuddy* parent) { _parent = parent; }

    /** Check if a button is currently pressed (id: 0-7). */
    bool buttonPressed(uint8_t id) {
        return (id < 8) ? _buttons[id] : false;
    }

    /** Read a slider value (id: 0-3, returns 0-255). */
    uint8_t slider(uint8_t id) {
        return (id < 4) ? _sliders[id] : 0;
    }

    /** Read D-pad state. Compare with DPAD_UP, DPAD_DOWN, DPAD_LEFT, DPAD_RIGHT, DPAD_NONE. */
    uint8_t dpad() { return _dpadState; }

    /** Touchpad X position (0-1000). */
    uint16_t touchX() { return _touchX; }

    /** Touchpad Y position (0-1000). */
    uint16_t touchY() { return _touchY; }

    /** Read switch state (id: 0-7). Returns true if ON. */
    bool switchState(uint8_t id) {
        return (id < 8) ? _switches[id] : false;
    }

    /** Reset all input state to defaults (used by HIL test harness). */
    void reset() {
        memset(_buttons, 0, sizeof(_buttons));
        memset(_switches, 0, sizeof(_switches));
        memset(_sliders, 0, sizeof(_sliders));
        _dpadState = 0;
        _touchX = 0; _touchY = 0;
        _keypadKey = 0;
    }

    /** Check if a keypad key is currently pressed. */
    bool keypadPressed() { return _keypadKey != 0; }

    /**
     * Get the last pressed keypad key.
     * Returns '0'-'9', '*', '#', 'A'-'D', or 0 if none.
     * Reading clears the key (returns 0 on next call until new press).
     */
    char keypad() {
        char k = _keypadKey;
        _keypadKey = 0;
        return k;
    }

private:
    STEMBuddy* _parent;
    bool    _buttons[8];
    bool    _switches[8];
    uint8_t _sliders[4];
    uint8_t _dpadState;
    uint16_t _touchX, _touchY;
    char _keypadKey;

    void _onButton(uint8_t id, uint8_t state) {
        if (id < 8) _buttons[id] = (state != 0);
    }
    void _onSlider(uint8_t id, uint8_t value) {
        if (id < 4) _sliders[id] = value;
    }
    void _onDpad(uint8_t dir) { _dpadState = dir; }
    void _onTouchpad(uint16_t x, uint16_t y) { _touchX = x; _touchY = y; }
    void _onKeypad(uint8_t key) { _keypadKey = (char)key; }
    void _onSwitch(uint8_t id, uint8_t state) {
        if (id < 8) _switches[id] = (state != 0);
    }
};

#endif
