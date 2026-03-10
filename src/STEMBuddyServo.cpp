#include "STEMBuddyServo.h"
#include "STEMBuddy.h"

void STEMBuddyServo::setAngle(uint8_t angle) {
    if (!_parent) return;
    if (angle > 180) angle = 180;
    uint8_t data[] = { angle };
    _parent->sendCommand(SBCmd::SERVO_SET_ANGLE, data, 1);
}

void STEMBuddyServo::detach() {
    if (!_parent) return;
    _parent->sendCommand(SBCmd::SERVO_DETACH);
}
