#include "STEMBuddy.h"

void STEMBuddyStepper::step(int16_t steps) {
    if (!_parent) return;
    uint8_t dir = (steps < 0) ? COUNTER_CLOCKWISE : CLOCKWISE;
    uint16_t absSteps = (uint16_t)abs(steps);
    uint8_t data[3] = {
        (uint8_t)(absSteps >> 8), (uint8_t)(absSteps & 0xFF),
        dir
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
