/**
 * STEM Buddy — SMS Alert System
 *
 * Sends real SMS messages from your phone, triggered by your ESP32!
 * This example monitors the temperature sensor and sends an SMS alert
 * when the temperature exceeds a threshold — like a real IoT alarm
 * system that texts you when something goes wrong.
 *
 * You can also send a manual SMS by pressing Button 0, which sends
 * a status report with the current sensor readings.
 *
 * IMPORTANT: The phone sends real SMS messages, so make sure you
 * update the phone number below before testing! Standard carrier
 * SMS charges may apply.
 *
 * What you'll learn:
 *   - Sending SMS messages from ESP32 through the phone
 *   - Building sensor-triggered alert systems
 *   - Formatting dynamic data into messages
 *   - Rate-limiting alerts to prevent spam
 *
 * Data flow:
 *   ESP32 → App → Carrier: SMS message
 *   App → ESP32: sensor data, button presses
 *
 * Hardware needed: ESP32 only (phone needs a SIM card for SMS)
 *
 * Steps:
 *   1. Change ALERT_PHONE_NUMBER below to your phone number
 *   2. Upload this sketch to your ESP32
 *   3. Open STEM Buddy and connect via Bluetooth
 *   4. Press Button 0 to send a manual status report SMS
 *   5. Heat the phone past 35°C to trigger an automatic alert
 *
 * Created by CodeSkool — https://codeskool.cc
 */

#include <STEMBuddy.h>

STEMBuddy buddy;

// *** CHANGE THIS to the phone number you want to alert ***
const char* ALERT_PHONE_NUMBER = "+1234567890";

const float TEMP_THRESHOLD = 35.0;   // °C — triggers alert above this
const unsigned long ALERT_COOLDOWN = 60000; // Minimum 60 seconds between auto-alerts

unsigned long lastAlert = 0;
bool alertSent = false;
bool lastBtn0 = false;

void setup() {
    Serial.begin(115200);
    buddy.begin("SMS-Alert");

    buddy.lcd.print(0, 0, "SMS Alert System");
    buddy.lcd.print(1, 0, "Monitoring...   ");
}

void loop() {
    buddy.update();

    if (!buddy.isConnected()) return;

    float temp     = buddy.sensor.temperature();
    uint16_t lux   = buddy.sensor.light();
    float humidity = buddy.sensor.humidity();

    // --- Display current readings on LCD ---
    char line0[17], line1[17];
    snprintf(line0, sizeof(line0), "T:%.1fC H:%.0f%%  ", temp, humidity);
    snprintf(line1, sizeof(line1), "L:%dlux         ", lux);
    buddy.lcd.print(0, 0, line0);
    buddy.lcd.print(1, 0, line1);

    // --- Button 0: Send manual status report SMS ---
    bool btn0 = buddy.input.buttonPressed(0);
    if (btn0 && !lastBtn0) {
        char msg[160];
        snprintf(msg, sizeof(msg),
                 "STEM Buddy Status Report:\n"
                 "Temp: %.1f C\n"
                 "Humidity: %.0f%%\n"
                 "Light: %d lux\n"
                 "All sensors normal.",
                 temp, humidity, lux);

        buddy.sms.send(ALERT_PHONE_NUMBER, msg);
        buddy.notify.send("SMS Sent", "Status report sent", 1);
        buddy.buzzer.tone(1000, 100);

        buddy.lcd.print(0, 0, "SMS Sent!       ");
        buddy.lcd.print(1, 0, "Status report   ");
        delay(2000);

        Serial.println("Manual SMS sent.");
    }
    lastBtn0 = btn0;

    // --- Auto-alert: temperature exceeds threshold ---
    if (temp > TEMP_THRESHOLD && !alertSent &&
        (millis() - lastAlert > ALERT_COOLDOWN)) {

        char msg[160];
        snprintf(msg, sizeof(msg),
                 "ALERT: High temperature detected!\n"
                 "Current: %.1f C (threshold: %.0f C)\n"
                 "Humidity: %.0f%%\n"
                 "Sent by STEM Buddy on ESP32.",
                 temp, TEMP_THRESHOLD, humidity);

        buddy.sms.send(ALERT_PHONE_NUMBER, msg);
        lastAlert = millis();
        alertSent = true;

        // Visual + audio alert
        buddy.led.setColor(255, 0, 0);
        buddy.led.on();
        buddy.buzzer.tone(2000, 500);
        buddy.notify.send("TEMP ALERT!", "SMS alert sent", 3);

        buddy.lcd.print(0, 0, "!! TEMP ALERT !!");
        buddy.lcd.print(1, 0, "SMS sent!       ");
        delay(3000);

        Serial.printf("Temperature alert SMS sent: %.1f°C\n", temp);
    }

    // Reset alert flag when temperature returns to normal (with hysteresis)
    if (temp < TEMP_THRESHOLD - 2.0) {
        if (alertSent) {
            buddy.led.setColor(0, 255, 0);
            buddy.led.on();
            buddy.notify.send("Temp Normal", "Temperature back to normal", 1);
        }
        alertSent = false;
    }

    delay(1000); // Check every second
}
