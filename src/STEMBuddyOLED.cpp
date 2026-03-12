#include "STEMBuddyOLED.h"
#include "STEMBuddy.h"

void STEMBuddyOLED::clearDisplay() {
    if (!_parent) return;
    _cursorX = 0;
    _cursorY = 0;
    _parent->sendCommand(SBCmd::OLED_CLEAR);
}

void STEMBuddyOLED::display() {
    if (!_parent) return;
    _parent->sendCommand(SBCmd::OLED_DISPLAY);
}

void STEMBuddyOLED::setCursor(uint8_t x, uint8_t y) {
    if (!_parent) return;
    _cursorX = x;
    _cursorY = y;
    uint8_t data[] = { x, y };
    _parent->sendCommand(SBCmd::OLED_SET_CURSOR, data, 2);
}

void STEMBuddyOLED::setTextSize(uint8_t size) {
    if (!_parent) return;
    if (size < 1) size = 1;
    if (size > 4) size = 4;
    _textSize = size;
    uint8_t data[] = { size };
    _parent->sendCommand(SBCmd::OLED_SET_TEXT_SIZE, data, 1);
}

void STEMBuddyOLED::print(const char* text) {
    if (!_parent || !text) return;
    // Send text in 18-byte BLE chunks
    _parent->sendChunked(SBCmd::OLED_PRINT, text);

    // Update local cursor position to match app
    for (const char* p = text; *p; p++) {
        if (*p == '\n') {
            _cursorX = 0;
            _cursorY += 8 * _textSize;
        } else {
            _cursorX += 6 * _textSize;
        }
    }
}

void STEMBuddyOLED::print(int value) {
    char buf[12];
    snprintf(buf, sizeof(buf), "%d", value);
    print(buf);
}

void STEMBuddyOLED::print(float value, uint8_t decimals) {
    char buf[16];
    dtostrf(value, 0, decimals, buf);
    print(buf);
}

void STEMBuddyOLED::drawPixel(uint8_t x, uint8_t y, uint8_t color) {
    if (!_parent) return;
    uint8_t data[] = { x, y, color };
    _parent->sendCommand(SBCmd::OLED_DRAW_PIXEL, data, 3);
}

void STEMBuddyOLED::drawLine(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1) {
    if (!_parent) return;
    uint8_t data[] = { x0, y0, x1, y1 };
    _parent->sendCommand(SBCmd::OLED_DRAW_LINE, data, 4);
}

void STEMBuddyOLED::drawRect(uint8_t x, uint8_t y, uint8_t w, uint8_t h) {
    if (!_parent) return;
    uint8_t data[] = { x, y, w, h, 0 };
    _parent->sendCommand(SBCmd::OLED_DRAW_RECT, data, 5);
}

void STEMBuddyOLED::fillRect(uint8_t x, uint8_t y, uint8_t w, uint8_t h) {
    if (!_parent) return;
    uint8_t data[] = { x, y, w, h, 1 };
    _parent->sendCommand(SBCmd::OLED_DRAW_RECT, data, 5);
}

void STEMBuddyOLED::drawCircle(uint8_t cx, uint8_t cy, uint8_t r) {
    if (!_parent) return;
    uint8_t data[] = { cx, cy, r, 0 };
    _parent->sendCommand(SBCmd::OLED_DRAW_CIRCLE, data, 4);
}

void STEMBuddyOLED::fillCircle(uint8_t cx, uint8_t cy, uint8_t r) {
    if (!_parent) return;
    uint8_t data[] = { cx, cy, r, 1 };
    _parent->sendCommand(SBCmd::OLED_DRAW_CIRCLE, data, 4);
}

void STEMBuddyOLED::invertDisplay(bool invert) {
    if (!_parent) return;
    uint8_t data[] = { (uint8_t)(invert ? 1 : 0) };
    _parent->sendCommand(SBCmd::OLED_INVERT, data, 1);
}
