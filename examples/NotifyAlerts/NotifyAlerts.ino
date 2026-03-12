/**
 * STEM Buddy — Push Notification Alerts
 *
 * Sends push notifications to the STEM Buddy app based on sensor
 * thresholds. This demonstrates how an ESP32 can alert the user
 * about important events — like a real IoT monitoring system.
 *
 * Notification types:
 *   - INFO:    Temperature reading (every 30 seconds)
 *   - WARNING: Light level too low (below 100 lux)
 *   - ERROR:   Proximity alert (object < 10 cm)
 *   - SUCCESS: All sensors normal
 *
 * What you'll learn:
 *   - Sending push notifications with title, body, and icon type
 *   - Using notification severity levels (info, success, warning, error)
 *   - Threshold-based alerting
 *   - Rate-limiting notifications to avoid spam
 *
 * Hardware needed: ESP32 only
 *
 * Steps:
 *   1. Upload this sketch to your ESP32
 *   2. Open STEM Buddy and connect via Bluetooth
 *   3. Watch notifications appear in the app
 *   4. Cover the light sensor or bring hand close to trigger alerts
 *
 * Created by CodeSkool — https://codeskool.cc
 */

#include <STEMBuddy.h>

STEMBuddy buddy;

unsigned long lastNotify = 0;
unsigned long lastCheck = 0;
bool lowLightNotified = false;
bool proximityNotified = false;

// Notification icon types: 0=INFO, 1=SUCCESS, 2=WARNING, 3=ERROR
#define ICON_INFO    0
#define ICON_SUCCESS 1
#define ICON_WARNING 2
#define ICON_ERROR   3

void setup() {
    Serial.begin(115200);
    buddy.begin("Notify-Alerts");
}

void loop() {
    buddy.update();

    if (!buddy.isConnected()) return;

    if (millis() - lastCheck < 1000) return;
    lastCheck = millis();

    float temp    = buddy.sensor.temperature();
    uint16_t lux  = buddy.sensor.light();
    uint16_t prox = buddy.sensor.proximity();

    // --- LCD: show current readings ---
    char line0[17], line1[17];
    snprintf(line0, sizeof(line0), "T:%.1fC L:%dlux", temp, lux);
    snprintf(line1, sizeof(line1), "P:%dcm          ", prox);
    buddy.lcd.print(0, 0, line0);
    buddy.lcd.print(1, 0, line1);

    // --- Periodic temperature report (every 30 seconds) ---
    if (millis() - lastNotify >= 30000) {
        lastNotify = millis();
        char body[48];
        snprintf(body, sizeof(body), "Current: %.1f C | Light: %d lux", temp, lux);
        buddy.notify.send("Sensor Report", body, ICON_INFO);
    }

    // --- Low light warning ---
    if (lux < 100 && !lowLightNotified) {
        buddy.notify.send("Low Light", "Ambient light below 100 lux", ICON_WARNING);
        lowLightNotified = true;
    } else if (lux >= 150) {
        if (lowLightNotified) {
            buddy.notify.send("Light Normal", "Light level restored", ICON_SUCCESS);
        }
        lowLightNotified = false;
    }

    // --- Proximity alert ---
    if (prox > 0 && prox < 10 && !proximityNotified) {
        buddy.notify.send("Proximity Alert!", "Object detected < 10 cm", ICON_ERROR);
        buddy.buzzer.tone(2000, 200);
        proximityNotified = true;
    } else if (prox >= 20) {
        proximityNotified = false;
    }
}
