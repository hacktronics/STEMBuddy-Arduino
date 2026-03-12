/**
 * STEM Buddy — GPIO Pin Monitor
 *
 * Reads real GPIO pins on your ESP32 and displays their state on the
 * STEM Buddy app's pin monitor view. This is like a virtual logic
 * analyzer — you can see digital HIGH/LOW states and analog values
 * for multiple pins at once.
 *
 * This example monitors:
 *   - Digital pins 2, 4, 5 (with built-in pull-up)
 *   - Analog pins 34, 35 (ADC input)
 *
 * Wire real buttons, sensors, or signals to these pins and watch
 * the states update live in the app!
 *
 * What you'll learn:
 *   - Reporting GPIO pin states to the app
 *   - Reading digital and analog pins on ESP32
 *   - Understanding pin modes (INPUT, OUTPUT, INPUT_PULLUP)
 *   - Real-time hardware monitoring
 *
 * Hardware needed:
 *   - ESP32
 *   - Optional: buttons on pins 2, 4, 5 (with pull-up)
 *   - Optional: potentiometers on pins 34, 35
 *
 * Steps:
 *   1. Upload this sketch to your ESP32
 *   2. Optionally wire buttons/pots to the monitored pins
 *   3. Open STEM Buddy and connect via Bluetooth
 *   4. The app shows live GPIO states
 *
 * Created by CodeSkool — https://codeskool.cc
 */

#include <STEMBuddy.h>

STEMBuddy buddy;

unsigned long lastUpdate = 0;

// Digital pins to monitor
const int digitalPins[] = {2, 4, 5};
const int numDigital = sizeof(digitalPins) / sizeof(digitalPins[0]);

// Analog pins to monitor
const int analogPins[] = {34, 35};
const int numAnalog = sizeof(analogPins) / sizeof(analogPins[0]);

void setup() {
    Serial.begin(115200);
    buddy.begin("Pin-Monitor");

    // Configure digital pins with internal pull-up
    for (int i = 0; i < numDigital; i++) {
        pinMode(digitalPins[i], INPUT_PULLUP);
    }

    // Analog pins are input by default on ESP32
}

void loop() {
    buddy.update();

    if (!buddy.isConnected()) return;

    // Update at 10 Hz
    if (millis() - lastUpdate < 100) return;
    lastUpdate = millis();

    // --- Report digital pin states ---
    for (int i = 0; i < numDigital; i++) {
        int state = digitalRead(digitalPins[i]);
        buddy.gpio.reportDigital(digitalPins[i], state, 2); // mode 2 = INPUT_PULLUP
    }

    // --- Report analog pin values ---
    for (int i = 0; i < numAnalog; i++) {
        int value = analogRead(analogPins[i]);
        buddy.gpio.reportAnalog(analogPins[i], value);
    }

    // --- LCD: show pin states ---
    char line0[17], line1[17];
    snprintf(line0, sizeof(line0), "D2:%d D4:%d D5:%d  ",
             digitalRead(2), digitalRead(4), digitalRead(5));
    snprintf(line1, sizeof(line1), "A34:%-4d A35:%-4d",
             analogRead(34), analogRead(35));
    buddy.lcd.print(0, 0, line0);
    buddy.lcd.print(1, 0, line1);
}
