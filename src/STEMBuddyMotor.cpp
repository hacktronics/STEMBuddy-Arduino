#include "STEMBuddyMotor.h"
#include "STEMBuddy.h"

void STEMBuddyMotor::run(int16_t speed) {
    if (!_parent) return;
    if (speed == 0) {
        stop();
        return;
    }
    setDirection(speed > 0 ? CLOCKWISE : COUNTER_CLOCKWISE);
    int16_t absSpeed = abs(speed);
    setSpeed((uint8_t)(absSpeed > 255 ? 255 : absSpeed));
}

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
