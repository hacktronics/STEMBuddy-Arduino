/**
 * STEMBuddyDataLog.cpp
 *
 * Data Logger implementation — sends columnar data to the app over BLE.
 */

#include "STEMBuddyDataLog.h"
#include "STEMBuddy.h"

void STEMBuddyDataLog::start(const char* fileName) {
    if (!_parent) return;
    _colCount = 0;
    _parent->sendChunked(SBCmd::DATALOG_FILE_NAME, fileName);
}

uint8_t STEMBuddyDataLog::addColumn(const char* name) {
    if (!_parent) return 0;
    if (_colCount >= DATALOG_MAX_COLUMNS) return _colCount - 1;

    uint8_t idx = _colCount;
    _colNames[idx] = name;
    _colCount++;

    // Send: [col_index, char0, char1, ...]
    size_t nameLen = strlen(name);
    uint8_t sendLen = (nameLen > 17) ? 17 : (uint8_t)nameLen; // 1 byte index + max 17 chars = 18 payload
    uint8_t buf[18];
    buf[0] = idx;
    memcpy(&buf[1], name, sendLen);

    _parent->sendCommand(SBCmd::DATALOG_ADD_COLUMN, buf, 1 + sendLen);
    return idx;
}

void STEMBuddyDataLog::log(const char* columnName, float value) {
    if (!_parent) return;
    int8_t idx = _findColumn(columnName);
    if (idx < 0) return;

    // Send: [col_index, b3, b2, b1, b0] — IEEE 754 big-endian
    uint8_t buf[5];
    buf[0] = (uint8_t)idx;
    uint8_t* fp = (uint8_t*)&value;
    // ESP32 is little-endian, send as big-endian
    buf[1] = fp[3];
    buf[2] = fp[2];
    buf[3] = fp[1];
    buf[4] = fp[0];

    _parent->sendCommand(SBCmd::DATALOG_FLOAT, buf, 5);
}

void STEMBuddyDataLog::log(const char* columnName, const char* value) {
    if (!_parent) return;
    int8_t idx = _findColumn(columnName);
    if (idx < 0) return;

    size_t textLen = strlen(value);
    uint8_t sendLen = (textLen > 17) ? 17 : (uint8_t)textLen;
    uint8_t buf[18];
    buf[0] = (uint8_t)idx;
    memcpy(&buf[1], value, sendLen);

    _parent->sendCommand(SBCmd::DATALOG_TEXT, buf, 1 + sendLen);
}

void STEMBuddyDataLog::newRow() {
    if (!_parent) return;
    _parent->sendCommand(SBCmd::DATALOG_NEW_ROW);
}

void STEMBuddyDataLog::stop() {
    if (!_parent) return;
    _parent->sendCommand(SBCmd::DATALOG_STOP);
}

int8_t STEMBuddyDataLog::_findColumn(const char* name) {
    for (uint8_t i = 0; i < _colCount; i++) {
        if (_colNames[i] == name) return i;
    }
    return -1;
}
