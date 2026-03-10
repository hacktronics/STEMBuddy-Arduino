#include "STEMBuddyBuzzer.h"
#include "STEMBuddy.h"

void STEMBuddyBuzzer::tone(uint16_t frequency, uint16_t duration) {
    if (!_parent) return;
    uint8_t data[] = {
        (uint8_t)((frequency >> 8) & 0xFF),
        (uint8_t)(frequency & 0xFF),
        (uint8_t)((duration >> 8) & 0xFF),
        (uint8_t)(duration & 0xFF)
    };
    _parent->sendCommand(SBCmd::BUZZER_TONE, data, 4);
}

void STEMBuddyBuzzer::noTone() {
    if (!_parent) return;
    _parent->sendCommand(SBCmd::BUZZER_NO_TONE);
}

void STEMBuddyBuzzer::setVolume(uint8_t volume) {
    if (!_parent) return;
    if (volume > 100) volume = 100;
    uint8_t data[] = { volume };
    _parent->sendCommand(SBCmd::BUZZER_SET_VOLUME, data, 1);
}
