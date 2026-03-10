#include "STEMBuddyLED.h"
#include "STEMBuddy.h"

void STEMBuddyLED::on() {
    if (!_parent) return;
    _parent->sendCommand(SBCmd::LED_ON);
}

void STEMBuddyLED::off() {
    if (!_parent) return;
    _parent->sendCommand(SBCmd::LED_OFF);
}

void STEMBuddyLED::setColor(uint8_t r, uint8_t g, uint8_t b) {
    if (!_parent) return;
    uint8_t data[] = { r, g, b };
    _parent->sendCommand(SBCmd::LED_SET_COLOR, data, 3);
}

void STEMBuddyLED::setBrightness(uint8_t brightness) {
    if (!_parent) return;
    uint8_t data[] = { brightness };
    _parent->sendCommand(SBCmd::LED_SET_BRIGHTNESS, data, 1);
}

void STEMBuddyLED::setStripCount(uint8_t count) {
    if (!_parent) return;
    uint8_t data[] = { count };
    _parent->sendCommand(SBCmd::LED_STRIP_COUNT, data, 1);
}

void STEMBuddyLED::setStripPixel(uint8_t index, uint8_t r, uint8_t g, uint8_t b) {
    if (!_parent) return;
    uint8_t data[] = { index, r, g, b };
    _parent->sendCommand(SBCmd::LED_STRIP_SET, data, 4);
}

void STEMBuddyLED::fillStrip(uint8_t r, uint8_t g, uint8_t b) {
    if (!_parent) return;
    uint8_t data[] = { r, g, b };
    _parent->sendCommand(SBCmd::LED_STRIP_FILL, data, 3);
}
