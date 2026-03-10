#include "STEMBuddyPiano.h"
#include "STEMBuddy.h"

void STEMBuddyPiano::noteOn(uint8_t note, uint8_t velocity) {
    if (!_parent) return;
    if (note > 127) note = 127;
    if (velocity > 127) velocity = 127;
    uint8_t data[] = { note, velocity };
    _parent->sendCommand(SBCmd::PIANO_NOTE_ON, data, 2);
}

void STEMBuddyPiano::noteOff(uint8_t note) {
    if (!_parent) return;
    if (note > 127) note = 127;
    uint8_t data[] = { note };
    _parent->sendCommand(SBCmd::PIANO_NOTE_OFF, data, 1);
}

void STEMBuddyPiano::allOff() {
    if (!_parent) return;
    _parent->sendCommand(SBCmd::PIANO_ALL_OFF);
}

void STEMBuddyPiano::setInstrument(uint8_t type) {
    if (!_parent) return;
    if (type > 3) type = 3;
    uint8_t data[] = { type };
    _parent->sendCommand(SBCmd::PIANO_SET_INSTRUMENT, data, 1);
}
