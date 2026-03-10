#include "STEMBuddyCamera.h"
#include "STEMBuddy.h"

void STEMBuddyCamera::capture() {
    if (!_parent) return;
    _status = 1; // capturing
    _parent->sendCommand(SBCmd::CAMERA_CAPTURE);
}

void STEMBuddyCamera::setQuality(uint8_t quality) {
    if (!_parent) return;
    if (quality < 10) quality = 10;
    if (quality > 100) quality = 100;
    uint8_t data[] = { quality };
    _parent->sendCommand(SBCmd::CAMERA_SET_QUALITY, data, 1);
}

void STEMBuddyCamera::setFlash(uint8_t mode) {
    if (!_parent) return;
    if (mode > 2) mode = 0;
    uint8_t data[] = { mode };
    _parent->sendCommand(SBCmd::CAMERA_SET_FLASH, data, 1);
}
