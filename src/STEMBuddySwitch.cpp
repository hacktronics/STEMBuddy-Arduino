#include "STEMBuddy.h"

void STEMBuddySwitch::set(uint8_t id, bool on) {
    if (!_parent) return;
    uint8_t data[2] = { (uint8_t)(id & 0x07), (uint8_t)(on ? 1 : 0) };
    _parent->sendCommand(SBCmd::SWITCH_SET, data, 2);
}

void STEMBuddySwitch::toggle(uint8_t id) {
    if (!_parent) return;
    uint8_t data[1] = { (uint8_t)(id & 0x07) };
    _parent->sendCommand(SBCmd::SWITCH_TOGGLE, data, 1);
}
