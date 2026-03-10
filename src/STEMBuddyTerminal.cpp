/**
 * STEMBuddyTerminal.cpp
 *
 * Sends terminal print/clear commands to the STEM Buddy app.
 * Text is sent in 18-byte chunks over BLE.
 */

#include "STEMBuddyTerminal.h"
#include "STEMBuddy.h"

void STEMBuddyTerminal::print(const char* text) {
    if (!_parent || !text) return;
    _parent->sendChunked(SBCmd::TERMINAL_PRINT, text);
}

void STEMBuddyTerminal::println(const char* text) {
    print(text);
    print("\n");
}

void STEMBuddyTerminal::clear() {
    if (!_parent) return;
    _parent->sendCommand(SBCmd::TERMINAL_CLEAR);
}
