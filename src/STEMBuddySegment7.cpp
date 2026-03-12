#include "STEMBuddy.h"

// 0x0A = minus sign digit (only segment G lit on the display)
static const uint8_t DIGIT_MINUS = 0x0A;

void STEMBuddySegment7::showNumber(int num) {
    // Clamp to displayable range
    if (num > 9999) num = 9999;
    if (num < -999) num = -999;

    bool neg = num < 0;
    int absVal = neg ? -num : num;

    uint8_t digits[4] = { 0xFF, 0xFF, 0xFF, 0xFF };

    // Fill digits right-to-left
    if (absVal == 0) {
        digits[3] = 0;
    } else {
        int pos = 3;
        while (absVal > 0 && pos >= 0) {
            digits[pos--] = absVal % 10;
            absVal /= 10;
        }
        if (neg) digits[pos] = DIGIT_MINUS;
    }

    setDigits(digits[0], digits[1], digits[2], digits[3]);
    setDecimal(0); // clear decimal for integer display
}

void STEMBuddySegment7::showNumber(float num) {
    bool neg = num < 0.0f;
    float absVal = neg ? -num : num;
    int intPart = (int)absVal;

    // If integer part alone overflows, fall back to integer display
    if (intPart > 9999 || (neg && intPart > 999)) {
        showNumber((int)(neg ? -absVal : absVal));
        return;
    }

    // Count integer digits
    int intDigits;
    if (intPart == 0) {
        intDigits = 1;
    } else {
        intDigits = 0;
        int t = intPart;
        while (t > 0) { intDigits++; t /= 10; }
    }

    int signSlots = neg ? 1 : 0;
    int decimalPlaces = 4 - intDigits - signSlots;

    if (decimalPlaces <= 0) {
        // No room for decimal — show as integer
        showNumber((int)(neg ? -absVal : absVal));
        return;
    }

    // Scale the number to fill all digit slots
    int scale = 1;
    for (int i = 0; i < decimalPlaces; i++) scale *= 10;
    int scaled = (int)(absVal * scale + 0.5f);

    // Extract digits right-to-left
    int totalDigits = intDigits + decimalPlaces;
    uint8_t digits[4] = { 0xFF, 0xFF, 0xFF, 0xFF };
    int startPos = 4 - totalDigits;
    if (neg) startPos--;

    for (int i = 3; i >= 4 - totalDigits; i--) {
        digits[i] = scaled % 10;
        scaled /= 10;
    }
    if (neg) digits[4 - totalDigits - 1] = DIGIT_MINUS;

    // Decimal point goes on the last integer digit
    int decimalPos = 4 - decimalPlaces - 1;
    uint8_t decMask = 1 << decimalPos;

    setDigits(digits[0], digits[1], digits[2], digits[3]);
    setDecimal(decMask);
}

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
