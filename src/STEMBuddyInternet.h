/**
 * STEMBuddyInternet.h
 *
 * Internet Gateway — send HTTP requests through the phone's internet connection.
 * The ESP32 doesn't need WiFi — it uses the STEM Buddy app as a BLE-to-internet bridge.
 *
 * Usage:
 *   buddy.internet.get("http://api.thingspeak.com/update?field1=25");
 *
 *   buddy.internet.get("http://api.openweathermap.org/data/2.5/weather?q=London&appid=KEY",
 *     [](int status, String body) {
 *       Serial.println("Weather: " + body);
 *     });
 *
 *   buddy.internet.post("http://example.com/api",
 *     "Content-Type: application/json\n",
 *     "{\"temp\":25.5}",
 *     [](int status, String body) {
 *       Serial.println("Response: " + String(status));
 *     });
 */

#ifndef STEMBUDDY_INTERNET_H
#define STEMBUDDY_INTERNET_H

#include <Arduino.h>
#include <functional>

// Forward declaration
class STEMBuddy;

// Response callback: (statusCode, responseBody)
typedef std::function<void(int statusCode, String body)> HttpCallback;

class STEMBuddyInternet {
public:
    STEMBuddyInternet();

    /**
     * Initialize with parent reference. Called by STEMBuddy::begin().
     */
    void begin(STEMBuddy* buddy);

    /**
     * Send an HTTP GET request through the phone.
     * @param url      Full URL to request
     * @param callback Optional callback for response (status, body)
     */
    void get(const char* url, HttpCallback callback = nullptr);

    /**
     * Send an HTTP POST request through the phone.
     * @param url      Full URL
     * @param headers  Headers in "Key: Value\n" format (or nullptr)
     * @param body     Request body (or nullptr)
     * @param callback Optional callback for response
     */
    void post(const char* url, const char* headers = nullptr,
              const char* body = nullptr, HttpCallback callback = nullptr);

    /**
     * Send an HTTP PUT request through the phone.
     */
    void put(const char* url, const char* headers = nullptr,
             const char* body = nullptr, HttpCallback callback = nullptr);

    /**
     * Send an HTTP DELETE request through the phone.
     */
    void del(const char* url, HttpCallback callback = nullptr);

    /**
     * Generic HTTP request.
     * @param method   0=GET, 1=POST, 2=PUT, 3=DELETE
     * @param url      Full URL
     * @param headers  Headers string (or nullptr)
     * @param body     Request body (or nullptr)
     * @param callback Optional callback
     */
    void request(uint8_t method, const char* url,
                 const char* headers = nullptr, const char* body = nullptr,
                 HttpCallback callback = nullptr);

    /**
     * Check if a request is currently pending (waiting for response).
     */
    bool isBusy();

    /**
     * Get the status code of the last completed request.
     */
    int lastStatus();

    /**
     * Get the response body of the last completed request.
     */
    String lastBody();

    // ─── Internal (called by STEMBuddy::_processMessage) ─────────
    void _onResponseStatus(const uint8_t* data, uint8_t len);
    void _onResponseBody(const uint8_t* data, uint8_t len);
    void _onResponseEnd(const uint8_t* data, uint8_t len);
    void _onResponseError(const uint8_t* data, uint8_t len);

private:
    STEMBuddy*   _buddy;
    uint8_t      _nextRequestId;
    bool         _busy;
    int          _lastStatus;
    String       _lastBody;

    // Current pending request
    uint8_t      _pendingId;
    String       _responseBuffer;
    HttpCallback _callback;
};

#endif // STEMBUDDY_INTERNET_H
