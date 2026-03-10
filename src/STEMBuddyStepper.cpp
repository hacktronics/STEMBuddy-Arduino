#include "STEMBuddy.h"

void STEMBuddyStepper::step(uint16_t steps, uint8_t direction) {
    if (!_parent) return;
    uint8_t data[3] = {
        (uint8_t)(steps >> 8), (uint8_t)(steps & 0xFF),
        (uint8_t)(direction & 0x01)
    };
    _parent->sendCommand(SBCmd::STEPPER_STEP, data, 3);
}

void STEMBuddyStepper::setAngle(uint16_t angle) {
    if (!_parent) return;
    if (angle > 360) angle = 360;
    uint8_t data[2] = { (uint8_t)(angle >> 8), (uint8_t)(angle & 0xFF) };
    _parent->sendCommand(SBCmd::STEPPER_SET_ANGLE, data, 2);
}

void STEMBuddyStepper::setRPM(uint8_t rpm) {
    if (!_parent) return;
    _parent->sendCommand(SBCmd::STEPPER_SET_RPM, &rpm, 1);
}

void STEMBuddyStepper::reset() {
    if (!_parent) return;
    _parent->sendCommand(SBCmd::STEPPER_RESET);
}
