/**
 * STEMBuddyTTS.cpp
 *
 * Sends text-to-speech commands to the STEM Buddy app.
 * Text is sent in 18-byte chunks, then TTS_SPEAK triggers speech synthesis.
 */

#include "STEMBuddyTTS.h"
#include "STEMBuddy.h"

void STEMBuddyTTS::speak(const char* text) {
    if (!_parent || !text) return;
    _parent->sendChunked(SBCmd::TTS_SPEAK, text);
}

void STEMBuddyTTS::stop() {
    if (!_parent) return;
    _parent->sendCommand(SBCmd::TTS_STOP);
}

void STEMBuddyTTS::setRate(uint8_t rate) {
    if (!_parent) return;
    if (rate > 200) rate = 200;
    uint8_t data[] = { rate };
    _parent->sendCommand(SBCmd::TTS_SET_RATE, data, 1);
}

void STEMBuddyTTS::setPitch(uint8_t pitch) {
    if (!_parent) return;
    if (pitch > 200) pitch = 200;
    uint8_t data[] = { pitch };
    _parent->sendCommand(SBCmd::TTS_SET_PITCH, data, 1);
}
