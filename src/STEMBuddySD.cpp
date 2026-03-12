/**
 * STEMBuddySD.cpp
 *
 * Virtual MicroSD Card: sends file operations to the STEM Buddy app.
 * Filenames and data are sent in 18-byte BLE chunks.
 */

#include "STEMBuddySD.h"
#include "STEMBuddy.h"

// ─── Write operations ────────────────────────────────────────────

void STEMBuddySD::writeFile(const char* filename, const char* content) {
    writeFile(filename, (const uint8_t*)content, strlen(content));
}

void STEMBuddySD::writeFile(const char* filename, const uint8_t* data, size_t len) {
    if (!_parent) return;

    // Send filename
    _parent->sendChunked(SBCmd::SD_FILE_NAME, filename);

    // Send data in 18-byte chunks
    size_t offset = 0;
    while (offset < len) {
        size_t chunkLen = len - offset;
        if (chunkLen > 18) chunkLen = 18;
        _parent->sendCommand(SBCmd::SD_FILE_WRITE, data + offset, chunkLen);
        offset += chunkLen;
    }

    // Signal write complete
    _parent->sendCommand(SBCmd::SD_FILE_WRITE_END);
}

// ─── Append operations ──────────────────────────────────────────

void STEMBuddySD::appendFile(const char* filename, const char* content) {
    appendFile(filename, (const uint8_t*)content, strlen(content));
}

void STEMBuddySD::appendFile(const char* filename, const uint8_t* data, size_t len) {
    if (!_parent) return;

    // Send filename
    _parent->sendChunked(SBCmd::SD_FILE_NAME, filename);

    // Send data in 18-byte chunks
    size_t offset = 0;
    while (offset < len) {
        size_t chunkLen = len - offset;
        if (chunkLen > 18) chunkLen = 18;
        _parent->sendCommand(SBCmd::SD_FILE_APPEND, data + offset, chunkLen);
        offset += chunkLen;
    }

    // Signal append complete
    _parent->sendCommand(SBCmd::SD_FILE_APPEND_END);
}

// ─── Read operations ────────────────────────────────────────────

void STEMBuddySD::readFile(const char* filename) {
    if (!_parent) return;

    _readBuffer = "";
    _hasReadResult = false;

    // Send filename, then request read
    _parent->sendChunked(SBCmd::SD_FILE_NAME, filename);
    _parent->sendCommand(SBCmd::SD_FILE_READ);
}

// ─── Delete ─────────────────────────────────────────────────────

void STEMBuddySD::deleteFile(const char* filename) {
    if (!_parent) return;

    _parent->sendChunked(SBCmd::SD_FILE_NAME, filename);
    _parent->sendCommand(SBCmd::SD_FILE_DELETE);
}

// ─── Exists ─────────────────────────────────────────────────────

void STEMBuddySD::exists(const char* filename) {
    if (!_parent) return;

    _hasExistsResult = false;

    _parent->sendChunked(SBCmd::SD_FILE_NAME, filename);
    _parent->sendCommand(SBCmd::SD_FILE_EXISTS);
}

// ─── List files ─────────────────────────────────────────────────

void STEMBuddySD::listFiles() {
    if (!_parent) return;

    _listBuffer = "";
    _listEntryBuffer = "";
    _hasListResult = false;

    _parent->sendCommand(SBCmd::SD_FILE_LIST_REQ);
}

// ─── Async result getters ───────────────────────────────────────

String STEMBuddySD::readResult() {
    _hasReadResult = false;
    String result = _readBuffer;
    _readBuffer = "";
    return result;
}

bool STEMBuddySD::fileExists() {
    _hasExistsResult = false;
    return _fileExists;
}

String STEMBuddySD::getFileList() {
    _hasListResult = false;
    String result = _listBuffer;
    _listBuffer = "";
    return result;
}

// ─── Internal callbacks (from BLE responses) ────────────────────

void STEMBuddySD::_onFileData(const uint8_t* data, uint8_t len) {
    for (uint8_t i = 0; i < len; i++) {
        _readBuffer += (char)data[i];
    }
}

void STEMBuddySD::_onFileDataEnd() {
    _hasReadResult = true;
}

void STEMBuddySD::_onFileStatus(const uint8_t* data, uint8_t len) {
    if (len >= 4) {
        _fileExists = data[0] != 0;
        _fileSize = ((uint32_t)data[1] << 16) | ((uint32_t)data[2] << 8) | data[3];
    } else {
        _fileExists = false;
        _fileSize = 0;
    }
    _hasExistsResult = true;
}

void STEMBuddySD::_onListEntry(const uint8_t* data, uint8_t len) {
    for (uint8_t i = 0; i < len; i++) {
        _listEntryBuffer += (char)data[i];
    }
}

void STEMBuddySD::_onListEnd() {
    // Each entry was chunked — finalize by adding to list
    if (_listEntryBuffer.length() > 0) {
        if (_listBuffer.length() > 0) _listBuffer += "\n";
        _listBuffer += _listEntryBuffer;
        _listEntryBuffer = "";
    }
    _hasListResult = true;
}
