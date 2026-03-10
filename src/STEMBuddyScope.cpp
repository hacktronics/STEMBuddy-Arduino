#include "STEMBuddyScope.h"
#include "STEMBuddy.h"

void STEMBuddyScope::plot(int16_t value) {
    if (!_parent) return;
    uint8_t data[] = {
        (uint8_t)((value >> 8) & 0xFF),
        (uint8_t)(value & 0xFF)
    };
    _parent->sendCommand(SBCmd::SCOPE_PLOT, data, 2);
}

void STEMBuddyScope::clear() {
    if (!_parent) return;
    _parent->sendCommand(SBCmd::SCOPE_CLEAR);
}

void STEMBuddyScope::setRange(int16_t min, int16_t max) {
    if (!_parent) return;
    uint8_t data[] = {
        (uint8_t)((min >> 8) & 0xFF), (uint8_t)(min & 0xFF),
        (uint8_t)((max >> 8) & 0xFF), (uint8_t)(max & 0xFF)
    };
    _parent->sendCommand(SBCmd::SCOPE_SET_RANGE, data, 4);
}

void STEMBuddyScope::setLabel(const char* label) {
    if (!_parent || !label) return;
    uint8_t len = strlen(label);
    if (len > 16) len = 16;
    _parent->sendCommand(SBCmd::SCOPE_SET_LABEL, (const uint8_t*)label, len);
}
