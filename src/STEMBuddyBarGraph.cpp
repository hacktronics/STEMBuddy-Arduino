#include "STEMBuddy.h"

void STEMBuddyBarGraph::set(uint8_t index, uint8_t value) {
    if (!_parent) return;
    uint8_t data[2] = { index, value };
    _parent->sendCommand(SBCmd::BAR_SET, data, 2);
}

void STEMBuddyBarGraph::setAll(const uint8_t* values, uint8_t count) {
    if (!_parent) return;
    if (count > 10) count = 10;
    _parent->sendCommand(SBCmd::BAR_SET_ALL, values, count);
}

void STEMBuddyBarGraph::setColor(Color color) {
    if (!_parent) return;
    uint8_t c = static_cast<uint8_t>(color);
    _parent->sendCommand(SBCmd::BAR_SET_COLOR, &c, 1);
}

void STEMBuddyBarGraph::clear() {
    if (!_parent) return;
    _parent->sendCommand(SBCmd::BAR_CLEAR);
}
