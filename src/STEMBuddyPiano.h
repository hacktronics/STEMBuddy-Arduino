/**
 * STEMBuddyPiano.h
 *
 * Virtual Piano — play notes on the STEM Buddy app from your ESP32.
 * The app renders a piano keyboard and plays real audio through the phone speaker.
 * Users can also tap keys on the app, which sends note events back to ESP32.
 *
 * Usage:
 *   buddy.piano.noteOn(60, 100);     // Play middle C (MIDI 60), velocity 100
 *   buddy.piano.noteOff(60);         // Stop middle C
 *   buddy.piano.allOff();            // Stop all notes
 *   buddy.piano.setInstrument(STEMBuddyPiano::PIANO);  // Named constants
 *
 * Reading user key presses:
 *   if (buddy.piano.available()) {
 *     uint8_t note = buddy.piano.lastNote();
 *     bool pressed = buddy.piano.lastPressed();
 *   }
 */

#ifndef STEMBUDDY_PIANO_H
#define STEMBUDDY_PIANO_H

#include <Arduino.h>

class STEMBuddy;

class STEMBuddyPiano {
    friend class STEMBuddy;

public:
    /** Instrument/sound type constants for setInstrument(). */
    enum Instrument : uint8_t {
        PIANO     = 0,
        ORGAN     = 1,
        SYNTH     = 2,
        MUSIC_BOX = 3
    };

    STEMBuddyPiano() : _parent(nullptr), _lastNote(0), _lastVelocity(0),
                        _lastPressed(false), _available(false) {}
    void begin(STEMBuddy* parent) { _parent = parent; }

    /**
     * Play a MIDI note on the app.
     * @param note     MIDI note number (0-127). 60 = middle C.
     * @param velocity Volume/intensity (0-127). 0 = silent, 127 = max.
     */
    void noteOn(uint8_t note, uint8_t velocity = 100);

    /**
     * Stop a MIDI note.
     * @param note  MIDI note number to stop.
     */
    void noteOff(uint8_t note);

    /** Stop all currently playing notes. */
    void allOff();

    /**
     * Set the instrument/sound type.
     * @param type  Instrument enum: PIANO, ORGAN, SYNTH, MUSIC_BOX
     */
    void setInstrument(Instrument type);

    // ─── Reading user key presses from app ────────────────────────

    /** Check if a new key event is available from the app. */
    bool available() {
        bool a = _available;
        _available = false;
        return a;
    }

    /** Get the last note pressed/released by the user. */
    uint8_t lastNote() { return _lastNote; }

    /** Get the velocity of the last key press (0 if released). */
    uint8_t lastVelocity() { return _lastVelocity; }

    /** True if the last event was a key press, false if release. */
    bool lastPressed() { return _lastPressed; }

private:
    STEMBuddy* _parent;
    uint8_t _lastNote;
    uint8_t _lastVelocity;
    bool _lastPressed;
    bool _available;

    // Called by STEMBuddy when BLE data arrives from app
    void _onKeyPress(uint8_t note, uint8_t velocity) {
        _lastNote = note;
        _lastVelocity = velocity;
        _lastPressed = true;
        _available = true;
    }
    void _onKeyRelease(uint8_t note) {
        _lastNote = note;
        _lastVelocity = 0;
        _lastPressed = false;
        _available = true;
    }
};

#endif
