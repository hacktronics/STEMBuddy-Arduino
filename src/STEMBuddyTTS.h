/**
 * STEMBuddyTTS.h
 *
 * Text-to-Speech: ESP32 sends text to the app, which speaks it
 * using the Web Speech API on the phone.
 *
 * Usage:
 *   buddy.tts.speak("Hello World");
 *   buddy.tts.setRate(120);   // Faster (100 = normal)
 *   buddy.tts.setPitch(80);   // Lower pitch
 *   buddy.tts.stop();
 */

#ifndef STEMBUDDY_TTS_H
#define STEMBUDDY_TTS_H

#include <Arduino.h>

class STEMBuddy;

class STEMBuddyTTS {
    friend class STEMBuddy;

public:
    STEMBuddyTTS() : _parent(nullptr) {}
    void begin(STEMBuddy* parent) { _parent = parent; }

    /** Speak text on the phone. Supports up to ~200 chars (sent in 18-byte chunks). */
    void speak(const char* text);
    void speak(const String& text) { speak(text.c_str()); }

    /** Stop speaking. */
    void stop();

    /** Set speech rate. 0-200 where 100 = normal speed. */
    void setRate(uint8_t rate);

    /** Set speech pitch. 0-200 where 100 = normal pitch. */
    void setPitch(uint8_t pitch);

private:
    STEMBuddy* _parent;
};

#endif
