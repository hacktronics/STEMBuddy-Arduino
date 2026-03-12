/**
 * STEM Buddy — Internet HTTP Requests
 *
 * Your ESP32 doesn't need WiFi to access the internet! STEM Buddy
 * uses the phone's internet connection as a gateway. This example
 * sends HTTP GET and POST requests through the app, demonstrating
 * how to fetch data from web APIs and send data to cloud services.
 *
 * Press Button 0 to fetch random joke data from an API, and Button 1
 * to send (simulated) sensor data via HTTP POST.
 *
 * What you'll learn:
 *   - Sending HTTP GET requests through the phone
 *   - Sending HTTP POST requests with JSON body
 *   - Handling asynchronous HTTP responses
 *   - Using the phone as a BLE-to-internet bridge
 *
 * Data flow:
 *   ESP32 → App → Internet: HTTP request
 *   Internet → App → ESP32: HTTP response (status + body)
 *
 * Hardware needed: ESP32 only (phone needs internet access)
 *
 * Steps:
 *   1. Upload this sketch to your ESP32
 *   2. Make sure your phone has internet connectivity
 *   3. Open STEM Buddy and connect via Bluetooth
 *   4. Press Button 0 to fetch data from an API
 *   5. Press Button 1 to send a POST request
 *
 * Created by CodeSkool — https://codeskool.cc
 */

#include <STEMBuddy.h>

STEMBuddy buddy;

bool lastBtn0 = false;
bool lastBtn1 = false;

void onGetResponse(int status, String body) {
    // Called when the GET request completes
    Serial.printf("GET response: %d\n", status);
    Serial.println(body);

    char line0[17];
    snprintf(line0, sizeof(line0), "HTTP %d        ", status);
    buddy.lcd.print(0, 0, line0);

    // Show first 16 chars of response body
    if (body.length() > 0) {
        char line1[17];
        snprintf(line1, sizeof(line1), "%-16s", body.substring(0, 16).c_str());
        buddy.lcd.print(1, 0, line1);
    }

    if (status == 200) {
        buddy.led.setColor(0, 255, 0); // Green = success
    } else {
        buddy.led.setColor(255, 0, 0); // Red = error
    }
    buddy.led.on();
}

void onPostResponse(int status, String body) {
    Serial.printf("POST response: %d\n", status);

    char line0[17];
    snprintf(line0, sizeof(line0), "POST: %d        ", status);
    buddy.lcd.print(0, 0, line0);

    if (status >= 200 && status < 300) {
        buddy.lcd.print(1, 0, "Data sent OK!   ");
        buddy.led.setColor(0, 255, 0);
        buddy.notify.send("Data Sent", "POST request successful", 1);
    } else {
        buddy.lcd.print(1, 0, "Send failed!    ");
        buddy.led.setColor(255, 0, 0);
    }
    buddy.led.on();
}

void setup() {
    Serial.begin(115200);
    buddy.begin("Internet-Demo");

    buddy.lcd.print(0, 0, "Internet Gateway");
    buddy.lcd.print(1, 0, "Btn0:GET Btn1:POST");
}

void loop() {
    buddy.update();

    if (!buddy.isConnected()) return;

    // --- Button 0: HTTP GET request ---
    bool btn0 = buddy.input.buttonPressed(0);
    if (btn0 && !lastBtn0 && !buddy.internet.isBusy()) {
        buddy.lcd.print(0, 0, "Fetching...     ");
        buddy.lcd.print(1, 0, "                ");

        buddy.internet.get(
            "http://httpbin.org/get",
            onGetResponse
        );
    }
    lastBtn0 = btn0;

    // --- Button 1: HTTP POST request ---
    bool btn1 = buddy.input.buttonPressed(1);
    if (btn1 && !lastBtn1 && !buddy.internet.isBusy()) {
        buddy.lcd.print(0, 0, "Sending...      ");
        buddy.lcd.print(1, 0, "                ");

        // Send simulated sensor data as JSON
        float temp = buddy.sensor.temperature();
        char jsonBody[64];
        snprintf(jsonBody, sizeof(jsonBody),
                 "{\"temp\":%.1f,\"device\":\"ESP32\"}", temp);

        buddy.internet.post(
            "http://httpbin.org/post",
            "Content-Type: application/json\n",
            jsonBody,
            onPostResponse
        );
    }
    lastBtn1 = btn1;

    delay(50);
}
