#include "STEMBuddyMotor.h"
#include "STEMBuddy.h"

void STEMBuddyMotor::setSpeed(uint8_t speed) {
    if (!_parent) return;
    uint8_t data[] = { speed };
    _parent->sendCommand(SBCmd::MOTOR_SET_SPEED, data, 1);
}

void STEMBuddyMotor::setDirection(uint8_t dir) {
    if (!_parent) return;
    uint8_t data[] = { dir & 0x01 };
    _parent->sendCommand(SBCmd::MOTOR_SET_DIR, data, 1);
}

void STEMBuddyMotor::stop() {
    if (!_parent) return;
    _parent->sendCommand(SBCmd::MOTOR_STOP);
}
