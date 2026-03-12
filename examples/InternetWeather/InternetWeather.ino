/**
 * STEM Buddy — Internet IoT Cloud Push
 *
 * A practical IoT example: your ESP32 reads sensor data and pushes it
 * to a cloud service (ThingSpeak) through the phone's internet — no
 * WiFi needed on the ESP32! The phone acts as a BLE-to-internet bridge.
 *
 * Every 30 seconds, the ESP32 sends temperature and humidity data to
 * ThingSpeak via HTTP GET. You can view live charts on the ThingSpeak
 * dashboard from any browser. This example also shows how to fetch
 * data back from the cloud using HTTP GET with a callback.
 *
 * You can easily adapt this to work with any REST API: IFTTT webhooks,
 * Adafruit IO, Blynk, or your own server.
 *
 * What you'll learn:
 *   - Pushing sensor data to a cloud API via HTTP GET
 *   - Periodic data uploads on a timer
 *   - Fetching data from a cloud API with callbacks
 *   - Using the phone as a WiFi-free internet gateway
 *   - Building a complete IoT data pipeline
 *
 * Data flow:
 *   Sensors → ESP32 → BLE → Phone → Internet → ThingSpeak
 *
 * Hardware needed: ESP32 only (phone needs internet)
 *
 * Before using:
 *   1. Create a free ThingSpeak account at https://thingspeak.com
 *   2. Create a new channel with fields: field1=Temperature, field2=Humidity
 *   3. Copy your Write API Key and paste it below
 *   4. Copy your Channel ID for reading data back
 *
 * Steps:
 *   1. Update THINGSPEAK_API_KEY below with your key
 *   2. Upload this sketch to your ESP32
 *   3. Open STEM Buddy and connect via Bluetooth
 *   4. Data uploads automatically every 30 seconds
 *   5. Press Button 0 to force an immediate upload
 *   6. Press Button 1 to read the last uploaded value back
 *   7. View your data at https://thingspeak.com/channels/YOUR_CHANNEL_ID
 *
 * Created by CodeSkool — https://codeskool.cc
 */

#include <STEMBuddy.h>

STEMBuddy buddy;

// *** REPLACE with your ThingSpeak Write API Key ***
const char* THINGSPEAK_API_KEY = "YOUR_API_KEY_HERE";

// *** REPLACE with your ThingSpeak Channel ID (for reading back) ***
const char* THINGSPEAK_CHANNEL = "YOUR_CHANNEL_ID";

const unsigned long UPLOAD_INTERVAL = 30000; // 30 seconds (ThingSpeak free tier limit: 15s)

unsigned long lastUpload = 0;
int uploadCount = 0;
bool lastBtn0 = false;
bool lastBtn1 = false;

// Callback: upload response
void onUploadDone(int status, String body) {
    if (status == 200) {
        buddy.led.setColor(0, 255, 0);
        buddy.lcd.print(1, 0, "Upload OK!      ");
        Serial.printf("ThingSpeak entry: %s\n", body.c_str());
    } else {
        buddy.led.setColor(255, 0, 0);
        char line[17];
        snprintf(line, sizeof(line), "Error: %d       ", status);
        buddy.lcd.print(1, 0, line);
        Serial.printf("Upload failed: %d\n", status);
    }
    buddy.led.on();
}

// Callback: read-back response
void onReadDone(int status, String body) {
    if (status == 200) {
        buddy.lcd.print(0, 0, "Cloud value:    ");
        char line[17];
        snprintf(line, sizeof(line), "%-16s", body.substring(0, 16).c_str());
        buddy.lcd.print(1, 0, line);
        Serial.println("Read from cloud: " + body);
    } else {
        buddy.lcd.print(1, 0, "Read failed!    ");
    }
}

void uploadSensorData() {
    float temp     = buddy.sensor.temperature();
    float humidity = buddy.sensor.humidity();

    // Build ThingSpeak update URL
    char url[200];
    snprintf(url, sizeof(url),
             "http://api.thingspeak.com/update?api_key=%s&field1=%.1f&field2=%.1f",
             THINGSPEAK_API_KEY, temp, humidity);

    buddy.internet.get(url, onUploadDone);
    uploadCount++;

    // Show status
    char line0[17];
    snprintf(line0, sizeof(line0), "Upload #%-4d    ", uploadCount);
    buddy.lcd.print(0, 0, line0);
    buddy.lcd.print(1, 0, "Sending...      ");

    Serial.printf("Uploading: T=%.1f H=%.1f (#%d)\n", temp, humidity, uploadCount);
}

void setup() {
    Serial.begin(115200);
    buddy.begin("IoT-Cloud");

    buddy.scope.setLabel("Temp C");
    buddy.scope.setRange(0, 5000);

    buddy.lcd.print(0, 0, "IoT Cloud Push  ");
    buddy.lcd.print(1, 0, "Connecting...   ");
}

void loop() {
    buddy.update();

    if (!buddy.isConnected()) return;

    // --- Auto-upload every 30 seconds ---
    if (millis() - lastUpload >= UPLOAD_INTERVAL) {
        lastUpload = millis();
        if (!buddy.internet.isBusy()) {
            uploadSensorData();
        }
    }

    // --- Button 0: Force immediate upload ---
    bool btn0 = buddy.input.buttonPressed(0);
    if (btn0 && !lastBtn0 && !buddy.internet.isBusy()) {
        uploadSensorData();
    }
    lastBtn0 = btn0;

    // --- Button 1: Read last value from cloud ---
    bool btn1 = buddy.input.buttonPressed(1);
    if (btn1 && !lastBtn1 && !buddy.internet.isBusy()) {
        char url[150];
        snprintf(url, sizeof(url),
                 "http://api.thingspeak.com/channels/%s/fields/1/last.txt",
                 THINGSPEAK_CHANNEL);
        buddy.internet.get(url, onReadDone);
        buddy.lcd.print(0, 0, "Reading cloud...");
    }
    lastBtn1 = btn1;

    // --- Oscilloscope: plot temperature ---
    float temp = buddy.sensor.temperature();
    buddy.scope.plot((int16_t)(temp * 100));

    // --- Show time until next upload ---
    unsigned long nextIn = UPLOAD_INTERVAL - (millis() - lastUpload);
    if (nextIn > UPLOAD_INTERVAL) nextIn = 0;
    static unsigned long lastLCDUpdate = 0;
    if (millis() - lastLCDUpdate > 1000 && !buddy.internet.isBusy()) {
        lastLCDUpdate = millis();
        char line1[17];
        snprintf(line1, sizeof(line1), "Next: %lds #%-4d", nextIn / 1000, uploadCount);
        buddy.lcd.print(1, 0, line1);
    }

    delay(100);
}
