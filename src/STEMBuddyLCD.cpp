#include "STEMBuddyLCD.h"
#include "STEMBuddy.h"

void STEMBuddyLCD::clear() {
    if (!_parent) return;
    _cursorRow = 0;
    _cursorCol = 0;
    _parent->sendCommand(SBCmd::LCD_CLEAR);
}

void STEMBuddyLCD::print(uint8_t row, uint8_t col, const char* text) {
    if (!_parent || !text) return;
    uint8_t textLen = strlen(text);
    if (textLen > 16) textLen = 16;

    // Packet: [row] [col] [chars...]
    uint8_t data[18]; // 2 + max 16 chars
    data[0] = row & 0x01;
    data[1] = col & 0x0F;
    memcpy(&data[2], text, textLen);

    _parent->sendCommand(SBCmd::LCD_PRINT, data, 2 + textLen);

    _cursorRow = row;
    _cursorCol = col + textLen;
}

void STEMBuddyLCD::print(const char* text) {
    print(_cursorRow, _cursorCol, text);
}

void STEMBuddyLCD::print(uint8_t row, uint8_t col, int value) {
    char buf[17];
    snprintf(buf, sizeof(buf), "%d", value);
    print(row, col, buf);
}

void STEMBuddyLCD::print(uint8_t row, uint8_t col, float value, uint8_t decimals) {
    char buf[17];
    dtostrf(value, 0, decimals, buf);
    print(row, col, buf);
}

void STEMBuddyLCD::setCursor(uint8_t row, uint8_t col) {
    if (!_parent) return;
    _cursorRow = row;
    _cursorCol = col;
    uint8_t data[] = { (uint8_t)(row & 0x01), (uint8_t)(col & 0x0F) };
    _parent->sendCommand(SBCmd::LCD_SET_CURSOR, data, 2);
}

void STEMBuddyLCD::backlightOn() {
    if (!_parent) return;
    _parent->sendCommand(SBCmd::LCD_BACKLIGHT_ON);
}

void STEMBuddyLCD::backlightOff() {
    if (!_parent) return;
    _parent->sendCommand(SBCmd::LCD_BACKLIGHT_OFF);
}

void STEMBuddyLCD::setBacklightColor(uint8_t r, uint8_t g, uint8_t b) {
    if (!_parent) return;
    uint8_t data[] = { r, g, b };
    _parent->sendCommand(SBCmd::LCD_SET_BG_COLOR, data, 3);
}

void STEMBuddyLCD::scrollLeft() {
    if (!_parent) return;
    _parent->sendCommand(SBCmd::LCD_SCROLL_LEFT);
}

void STEMBuddyLCD::scrollRight() {
    if (!_parent) return;
    _parent->sendCommand(SBCmd::LCD_SCROLL_RIGHT);
}

void STEMBuddyLCD::createChar(uint8_t slot, const uint8_t charmap[8]) {
    if (!_parent) return;
    uint8_t data[9];
    data[0] = slot & 0x07;
    memcpy(&data[1], charmap, 8);
    _parent->sendCommand(SBCmd::LCD_CREATE_CHAR, data, 9);
}
