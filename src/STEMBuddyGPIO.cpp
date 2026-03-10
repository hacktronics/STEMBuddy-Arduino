#include "STEMBuddy.h"

void STEMBuddyGPIO::reportDigital(uint8_t pin, uint8_t state, uint8_t mode) {
    if (!_parent) return;
    uint8_t data[3] = { pin, (uint8_t)(state & 0x01), mode };
    _parent->sendCommand(SBCmd::GPIO_PIN_STATE, data, 3);
}

void STEMBuddyGPIO::reportAnalog(uint8_t pin, uint16_t value) {
    if (!_parent) return;
    uint8_t data[3] = { pin, (uint8_t)(value >> 8), (uint8_t)(value & 0xFF) };
    _parent->sendCommand(SBCmd::GPIO_ANALOG_VALUE, data, 3);
}
