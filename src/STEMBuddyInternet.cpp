/**
 * STEMBuddyInternet.cpp
 *
 * Internet Gateway — sends HTTP requests through the phone's internet connection.
 * URLs, headers, and body are sent in 18-byte BLE chunks.
 * The app assembles them, executes fetch(), and sends the response back.
 */

#include "STEMBuddyInternet.h"
#include "STEMBuddy.h"
#include "STEMBuddyProtocol.h"

STEMBuddyInternet::STEMBuddyInternet()
    : _buddy(nullptr), _nextRequestId(0), _busy(false),
      _lastStatus(0), _pendingId(0), _callback(nullptr) {}

void STEMBuddyInternet::begin(STEMBuddy* buddy) {
    _buddy = buddy;
}

// ─── Public API ─────────────────────────────────────────────────

void STEMBuddyInternet::get(const char* url, HttpCallback callback) {
    request(0, url, nullptr, nullptr, callback);
}

void STEMBuddyInternet::post(const char* url, const char* headers,
                              const char* body, HttpCallback callback) {
    request(1, url, headers, body, callback);
}

void STEMBuddyInternet::put(const char* url, const char* headers,
                             const char* body, HttpCallback callback) {
    request(2, url, headers, body, callback);
}

void STEMBuddyInternet::del(const char* url, HttpCallback callback) {
    request(3, url, nullptr, nullptr, callback);
}

void STEMBuddyInternet::request(uint8_t method, const char* url,
                                  const char* headers, const char* body,
                                  HttpCallback callback) {
    if (!_buddy) return;

    uint8_t requestId = _nextRequestId++;
    _pendingId = requestId;
    _busy = true;
    _callback = callback;
    _responseBuffer = "";

    // 1. Send REQUEST_START: [method, requestId]
    uint8_t startData[2] = { method, requestId };
    _buddy->sendCommand(SBCmd::INET_REQUEST_START, startData, 2);

    // 2. Send URL in chunks
    if (url) _buddy->sendChunked(SBCmd::INET_REQUEST_URL, url);

    // 3. Send headers in chunks (if any)
    if (headers) _buddy->sendChunked(SBCmd::INET_REQUEST_HEADER, headers);

    // 4. Send body in chunks (if any)
    if (body) _buddy->sendChunked(SBCmd::INET_REQUEST_BODY, body);

    // 5. Send REQUEST_SEND to trigger execution
    uint8_t sendData[1] = { requestId };
    _buddy->sendCommand(SBCmd::INET_REQUEST_SEND, sendData, 1);
}

bool STEMBuddyInternet::isBusy() {
    return _busy;
}

int STEMBuddyInternet::lastStatus() {
    return _lastStatus;
}

String STEMBuddyInternet::lastBody() {
    return _lastBody;
}

// ─── Response handlers (called by STEMBuddy::_processMessage) ───

void STEMBuddyInternet::_onResponseStatus(const uint8_t* data, uint8_t len) {
    if (len < 3) return;
    // [requestId, status_hi, status_lo]
    uint8_t reqId = data[0];
    if (reqId != _pendingId) return;

    _lastStatus = ((uint16_t)data[1] << 8) | data[2];
    _responseBuffer = "";
}

void STEMBuddyInternet::_onResponseBody(const uint8_t* data, uint8_t len) {
    // Append body chunk (whole chunk at once, not char-by-char)
    char buf[19]; // max 18 + null
    uint8_t copyLen = (len > 18) ? 18 : len;
    memcpy(buf, data, copyLen);
    buf[copyLen] = 0;
    _responseBuffer += buf;
}

void STEMBuddyInternet::_onResponseEnd(const uint8_t* data, uint8_t len) {
    if (len < 1) return;
    uint8_t reqId = data[0];
    if (reqId != _pendingId) return;

    _lastBody = _responseBuffer;
    _busy = false;

    // Fire callback if registered
    if (_callback) {
        _callback(_lastStatus, _lastBody);
        _callback = nullptr;
    }
}

void STEMBuddyInternet::_onResponseError(const uint8_t* data, uint8_t len) {
    if (len < 2) return;
    uint8_t reqId = data[0];
    if (reqId != _pendingId) return;

    uint8_t errorCode = data[1];
    _lastStatus = -(int)(errorCode + 1); // Negative = error (-1=network, -2=timeout, -3=dns, -4=refused)
    _lastBody = "";
    _busy = false;

    if (_callback) {
        _callback(_lastStatus, "");
        _callback = nullptr;
    }
}
