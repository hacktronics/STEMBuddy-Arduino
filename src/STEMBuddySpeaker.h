/**
 * STEMBuddySpeaker.h
 *
 * Virtual Speaker — play tones with different waveforms on the STEM Buddy app.
 *
 * Usage:
 *   buddy.speaker.note(440, 500);                              // Play 440Hz for 500ms (default sine)
 *   buddy.speaker.note(440, 500, STEMBuddySpeaker::SQUARE);   // Play 440Hz for 500ms, square wave
 *   buddy.speaker.setVolume(80);                               // Set volume to 80%
 *   buddy.speaker.setWaveform(STEMBuddySpeaker::SAWTOOTH);    // Set default waveform
 *   buddy.speaker.stop();                                      // Stop playback
 */

#ifndef STEMBUDDY_SPEAKER_H
#define STEMBUDDY_SPEAKER_H

#include <Arduino.h>

class STEMBuddy;

class STEMBuddySpeaker {
    friend class STEMBuddy;

public:
    // Waveform types for audio synthesis
    enum Waveform : uint8_t {
        SINE     = 0,
        SQUARE   = 1,
        SAWTOOTH = 2,
        TRIANGLE = 3
    };

    STEMBuddySpeaker() : _parent(nullptr), _waveform(SINE) {}
    void begin(STEMBuddy* parent) { _parent = parent; }

    /**
     * Play a tone.
     * @param frequency  Frequency in Hz (0-65535)
     * @param duration   Duration in ms (0=continuous, 0-65535)
     * @param waveform   SINE, SQUARE, SAWTOOTH, or TRIANGLE (default: current)
     */
    void note(uint16_t frequency, uint16_t duration = 0, int8_t waveform = -1);

    /** Stop playback. */
    void stop();

    /** Set volume (0-100). */
    void setVolume(uint8_t volume);

    /** Set waveform type: SINE, SQUARE, SAWTOOTH, or TRIANGLE. */
    void setWaveform(Waveform type);

private:
    STEMBuddy* _parent;
    Waveform _waveform;
};

#endif
