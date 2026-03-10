/**
 * STEMBuddyBuzzer.h
 *
 * Virtual Buzzer / Tone Generator.
 * Send tone commands from ESP32 and the STEM Buddy app plays the sound.
 *
 * Usage:
 *   buddy.buzzer.tone(440, 500);   // 440Hz for 500ms
 *   buddy.buzzer.tone(880);        // 880Hz continuous
 *   buddy.buzzer.noTone();         // Stop
 *   buddy.buzzer.setVolume(50);    // 50%
 */

#ifndef STEMBUDDY_BUZZER_H
#define STEMBUDDY_BUZZER_H

#include <Arduino.h>

class STEMBuddy;

class STEMBuddyBuzzer {
public:
    STEMBuddyBuzzer() : _parent(nullptr) {}
    void begin(STEMBuddy* parent) { _parent = parent; }

    /**
     * Play a tone.
     * @param frequency  Frequency in Hz (20-20000)
     * @param duration   Duration in ms (0 = play until noTone())
     */
    void tone(uint16_t frequency, uint16_t duration = 0);

    /** Stop playing. */
    void noTone();

    /** Set volume (0-100). */
    void setVolume(uint8_t volume);

private:
    STEMBuddy* _parent;
};

#endif
