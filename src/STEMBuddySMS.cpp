/**
 * STEMBuddySMS.cpp
 *
 * Sends SMS commands to the STEM Buddy app.
 * Phone number and body are sent in 18-byte chunks, then SMS_SEND triggers sending.
 */

#include "STEMBuddySMS.h"
#include "STEMBuddy.h"

void STEMBuddySMS::send(const char* number, const char* message) {
    if (!_parent) return;

    // Send phone number in chunks
    _parent->sendChunked(SBCmd::SMS_NUMBER, number);

    // Send message body in chunks
    _parent->sendChunked(SBCmd::SMS_BODY, message);

    // Trigger the silent SMS send
    _parent->sendCommand(SBCmd::SMS_SEND);
}
