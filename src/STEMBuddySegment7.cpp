#include "STEMBuddy.h"

void STEMBuddySegment7::setDigits(uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3) {
    if (!_parent) return;
    uint8_t data[4] = { d0, d1, d2, d3 };
    _parent->sendCommand(SBCmd::SEG7_SET_VALUE, data, 4);
}

void STEMBuddySegment7::setRaw(const uint8_t* segments, uint8_t count) {
    if (!_parent) return;
    if (count > 8) count = 8;
    _parent->sendCommand(SBCmd::SEG7_SET_RAW, segments, count);
}

void STEMBuddySegment7::setDecimal(uint8_t mask) {
    if (!_parent) return;
    _parent->sendCommand(SBCmd::SEG7_SET_DECIMAL, &mask, 1);
}

void STEMBuddySegment7::setColon(bool on) {
    if (!_parent) return;
    uint8_t val = on ? 1 : 0;
    _parent->sendCommand(SBCmd::SEG7_SET_COLON, &val, 1);
}

void STEMBuddySegment7::setColor(uint8_t r, uint8_t g, uint8_t b) {
    if (!_parent) return;
    uint8_t data[3] = { r, g, b };
    _parent->sendCommand(SBCmd::SEG7_SET_COLOR, data, 3);
}

void STEMBuddySegment7::clear() {
    if (!_parent) return;
    _parent->sendCommand(SBCmd::SEG7_CLEAR);
}
