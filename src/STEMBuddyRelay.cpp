#include "STEMBuddy.h"

void STEMBuddyRelay::on(uint8_t id) {
    if (!_parent) return;
    uint8_t data[2] = { (uint8_t)(id & 0x07), 1 };
    _parent->sendCommand(SBCmd::SWITCH_SET, data, 2);
}

void STEMBuddyRelay::off(uint8_t id) {
    if (!_parent) return;
    uint8_t data[2] = { (uint8_t)(id & 0x07), 0 };
    _parent->sendCommand(SBCmd::SWITCH_SET, data, 2);
}

void STEMBuddyRelay::toggle(uint8_t id) {
    if (!_parent) return;
    uint8_t data[1] = { (uint8_t)(id & 0x07) };
    _parent->sendCommand(SBCmd::SWITCH_TOGGLE, data, 1);
}
