#include "STEMBuddy.h"

void STEMBuddySpeaker::note(uint16_t frequency, uint16_t duration, int8_t waveform) {
    uint8_t wf = (waveform >= 0) ? (uint8_t)waveform : _waveform;
    if (wf > 3) wf = 0;
    uint8_t data[5] = {
        (uint8_t)(frequency >> 8), (uint8_t)(frequency & 0xFF),
        (uint8_t)(duration >> 8),  (uint8_t)(duration & 0xFF),
        wf
    };
    if (!_parent) return;
    _parent->sendCommand(SBCmd::SPEAKER_NOTE, data, 5);
}

void STEMBuddySpeaker::stop() {
    if (!_parent) return;
    _parent->sendCommand(SBCmd::SPEAKER_STOP);
}

void STEMBuddySpeaker::setVolume(uint8_t volume) {
    if (!_parent) return;
    if (volume > 100) volume = 100;
    _parent->sendCommand(SBCmd::SPEAKER_SET_VOLUME, &volume, 1);
}

void STEMBuddySpeaker::setWaveform(uint8_t type) {
    if (!_parent) return;
    if (type > 3) type = 0;
    _waveform = type;
    _parent->sendCommand(SBCmd::SPEAKER_SET_WAVEFORM, &type, 1);
}
