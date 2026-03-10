/**
 * STEMBuddySpeaker.h
 *
 * Virtual Speaker — play tones with different waveforms on the STEM Buddy app.
 *
 * Usage:
 *   buddy.speaker.note(440, 500);         // Play 440Hz for 500ms (default sine)
 *   buddy.speaker.note(440, 500, 1);      // Play 440Hz for 500ms, square wave
 *   buddy.speaker.setVolume(80);          // Set volume to 80%
 *   buddy.speaker.setWaveform(2);         // 0=sine, 1=square, 2=sawtooth, 3=triangle
 *   buddy.speaker.stop();                 // Stop playback
 */

#ifndef STEMBUDDY_SPEAKER_H
#define STEMBUDDY_SPEAKER_H

#include <Arduino.h>

class STEMBuddy;

class STEMBuddySpeaker {
    friend class STEMBuddy;

public:
    STEMBuddySpeaker() : _parent(nullptr), _waveform(0) {}
    void begin(STEMBuddy* parent) { _parent = parent; }

    /**
     * Play a tone.
     * @param frequency  Frequency in Hz (0-65535)
     * @param duration   Duration in ms (0=continuous, 0-65535)
     * @param waveform   0=sine, 1=square, 2=sawtooth, 3=triangle (default: current)
     */
    void note(uint16_t frequency, uint16_t duration = 0, int8_t waveform = -1);

    /** Stop playback. */
    void stop();

    /** Set volume (0-100). */
    void setVolume(uint8_t volume);

    /** Set waveform type: 0=sine, 1=square, 2=sawtooth, 3=triangle. */
    void setWaveform(uint8_t type);

private:
    STEMBuddy* _parent;
    uint8_t _waveform;
};

#endif
