#include "STEMBuddy.h"

void STEMBuddyMAX7219::setRow(uint8_t module, uint8_t row, uint8_t bits) {
    if (!_parent) return;
    uint8_t data[3] = { module, row, bits };
    _parent->sendCommand(SBCmd::MAX7219_SET_ROW, data, 3);
}

void STEMBuddyMAX7219::setAll(const uint8_t rows[8]) {
    if (!_parent) return;
    _parent->sendCommand(SBCmd::MAX7219_SET_ALL, rows, 8);
}

void STEMBuddyMAX7219::clear() {
    if (!_parent) return;
    _parent->sendCommand(SBCmd::MAX7219_CLEAR);
}

void STEMBuddyMAX7219::setIntensity(uint8_t level) {
    if (!_parent) return;
    if (level > 15) level = 15;
    _parent->sendCommand(SBCmd::MAX7219_SET_INTENSITY, &level, 1);
}

void STEMBuddyMAX7219::setChain(uint8_t count) {
    if (!_parent) return;
    if (count < 1) count = 1;
    if (count > 4) count = 4;
    _parent->sendCommand(SBCmd::MAX7219_SET_CHAIN, &count, 1);
}

void STEMBuddyMAX7219::setColor(uint8_t r, uint8_t g, uint8_t b) {
    if (!_parent) return;
    uint8_t data[3] = { r, g, b };
    _parent->sendCommand(SBCmd::MAX7219_SET_COLOR, data, 3);
}
