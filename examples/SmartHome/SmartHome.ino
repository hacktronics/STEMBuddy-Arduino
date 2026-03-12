/**
 * STEM Buddy — Smart Home Automation
 *
 * Simulates a smart home system with 4 virtual switches controlled by
 * sensor thresholds. The switches toggle automatically based on
 * conditions, but can also be overridden manually via the app.
 *
 * Automation rules:
 *   - Switch 0 (Lights):  ON when ambient light drops below 200 lux
 *   - Switch 1 (Fan):     ON when temperature exceeds 30°C
 *   - Switch 2 (Alarm):   ON when proximity < 20 cm (intruder!)
 *   - Switch 3 (Heater):  ON when temperature drops below 18°C
 *
 * The LCD shows the status of all 4 switches, and a notification is
 * sent whenever a switch changes state.
 *
 * What you'll learn:
 *   - Controlling home automation switches
 *   - Sensor-driven conditional logic
 *   - Threshold-based automation
 *   - Sending push notifications
 *
 * Sensors used:
 *   - Light (lux)      → buddy.sensor.light()
 *   - Temperature (°C)  → buddy.sensor.temperature()
 *   - Proximity (cm)    → buddy.sensor.proximity()
 *
 * Hardware needed: ESP32 only
 *
 * Steps:
 *   1. Upload this sketch to your ESP32
 *   2. Open STEM Buddy and connect via Bluetooth
 *   3. Cover the light sensor → lights turn on
 *   4. Warm the phone → fan turns on
 *   5. Move hand close → alarm triggers
 *
 * Created by CodeSkool — https://codeskool.cc
 */

#include <STEMBuddy.h>

STEMBuddy buddy;

unsigned long lastUpdate = 0;
bool prevState[4] = {false, false, false, false};

void setup() {
    Serial.begin(115200);
    buddy.begin("Smart-Home");
}

void loop() {
    buddy.update();

    if (!buddy.isConnected()) return;

    if (millis() - lastUpdate < 500) return;
    lastUpdate = millis();

    // Read sensors
    uint16_t lux = buddy.sensor.light();
    float temp   = buddy.sensor.temperature();
    uint16_t cm  = buddy.sensor.proximity();

    // --- Apply automation rules ---
    bool lights = (lux < 200);
    bool fan    = (temp > 30.0f);
    bool alarm  = (cm > 0 && cm < 20);
    bool heater = (temp < 18.0f);

    bool states[4] = {lights, fan, alarm, heater};
    const char* names[4] = {"Lights", "Fan", "Alarm", "Heater"};

    // Update switches and send notifications on state change
    for (int i = 0; i < 4; i++) {
        buddy.sw.set(i, states[i]);

        if (states[i] != prevState[i]) {
            char msg[32];
            snprintf(msg, sizeof(msg), "%s turned %s", names[i], states[i] ? "ON" : "OFF");
            buddy.notify.send("Smart Home", msg, states[i] ? 1 : 0);
            prevState[i] = states[i];
        }
    }

    // --- LCD: show switch status ---
    char line0[17], line1[17];
    snprintf(line0, sizeof(line0), "L:%s F:%s       ",
             lights ? "ON " : "OFF", fan ? "ON " : "OFF");
    snprintf(line1, sizeof(line1), "A:%s H:%s       ",
             alarm ? "ON " : "OFF", heater ? "ON " : "OFF");
    buddy.lcd.print(0, 0, line0);
    buddy.lcd.print(1, 0, line1);

    // Alarm buzzer when intruder detected
    if (alarm) {
        buddy.buzzer.tone(1500, 200);
        buddy.led.setColor(255, 0, 0);
    } else {
        buddy.led.setColor(0, 255, 0);
    }
    buddy.led.on();
}
