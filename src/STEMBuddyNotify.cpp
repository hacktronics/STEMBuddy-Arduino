/**
 * STEMBuddyNotify.cpp
 *
 * Sends notification commands to the STEM Buddy app.
 * Title and body are sent in 18-byte chunks, then NOTIFY_SHOW triggers display.
 */

#include "STEMBuddyNotify.h"
#include "STEMBuddy.h"

void STEMBuddyNotify::send(const char* title, const char* body, Icon icon) {
    if (!_parent) return;

    // Send title in chunks
    _parent->sendChunked(SBCmd::NOTIFY_TITLE, title);

    // Send body in chunks
    _parent->sendChunked(SBCmd::NOTIFY_BODY, body);

    // Trigger the notification display with icon type
    uint8_t data[] = { (uint8_t)icon };
    _parent->sendCommand(SBCmd::NOTIFY_SHOW, data, 1);
}

void STEMBuddyNotify::clear() {
    if (!_parent) return;
    _parent->sendCommand(SBCmd::NOTIFY_CLEAR);
}
