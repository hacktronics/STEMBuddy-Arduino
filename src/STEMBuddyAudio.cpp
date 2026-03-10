/**
 * STEMBuddyAudio.cpp
 *
 * Sends audio playback commands to the STEM Buddy app.
 * Filename is sent in 18-byte chunks, then AUDIO_PLAY triggers playback.
 */

#include "STEMBuddyAudio.h"
#include "STEMBuddy.h"

void STEMBuddyAudio::play(const char* filename) {
    if (!_parent) return;

    // Send filename in chunks
    _parent->sendChunked(SBCmd::AUDIO_FILE_NAME, filename);

    // Trigger playback
    _parent->sendCommand(SBCmd::AUDIO_PLAY);
}

void STEMBuddyAudio::stop() {
    if (!_parent) return;
    _parent->sendCommand(SBCmd::AUDIO_STOP);
}

void STEMBuddyAudio::pause() {
    if (!_parent) return;
    _parent->sendCommand(SBCmd::AUDIO_PAUSE);
}

void STEMBuddyAudio::resume() {
    if (!_parent) return;
    _parent->sendCommand(SBCmd::AUDIO_RESUME);
}

void STEMBuddyAudio::setVolume(uint8_t volume) {
    if (!_parent) return;
    if (volume > 100) volume = 100;
    uint8_t data[] = { volume };
    _parent->sendCommand(SBCmd::AUDIO_SET_VOLUME, data, 1);
}

void STEMBuddyAudio::setLoop(bool loop) {
    if (!_parent) return;
    uint8_t data[] = { (uint8_t)(loop ? 1 : 0) };
    _parent->sendCommand(SBCmd::AUDIO_SET_LOOP, data, 1);
}
