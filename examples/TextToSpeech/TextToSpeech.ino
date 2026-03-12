/**
 * STEM Buddy — Text-to-Speech (TTS)
 *
 * Makes your phone speak out loud! Send text from your ESP32 and the
 * phone's TTS engine reads it aloud. This example reads sensor values
 * and speaks them — like a talking thermometer.
 *
 * Press Button 0 to hear the current temperature, Button 1 for light
 * level, and Button 2 for a custom greeting.
 *
 * What you'll learn:
 *   - Sending text to the phone's speech engine
 *   - Adjusting speech rate and pitch
 *   - Combining sensor data with speech output
 *   - Building voice-based feedback systems
 *
 * Data flow:
 *   App → ESP32: button presses, sensor data
 *   ESP32 → App: TTS text, LCD display
 *
 * Hardware needed: ESP32 only (turn up phone volume!)
 *
 * Steps:
 *   1. Upload this sketch to your ESP32
 *   2. Open STEM Buddy and connect via Bluetooth
 *   3. Press Button 0 to hear the temperature
 *   4. Press Button 1 to hear the light level
 *   5. Press Button 2 to hear a greeting
 *
 * Created by CodeSkool — https://codeskool.cc
 */

#include <STEMBuddy.h>

STEMBuddy buddy;

bool lastBtn[3] = {false, false, false};

void setup() {
    Serial.begin(115200);
    buddy.begin("TTS-Demo");

    // Normal speech rate and pitch
    buddy.tts.setRate(100);
    buddy.tts.setPitch(100);
}

void loop() {
    buddy.update();

    if (!buddy.isConnected()) return;

    // --- Button 0: Speak temperature ---
    bool btn0 = buddy.input.buttonPressed(0);
    if (btn0 && !lastBtn[0]) {
        float temp = buddy.sensor.temperature();
        char msg[64];
        snprintf(msg, sizeof(msg), "The temperature is %.1f degrees Celsius.", temp);
        buddy.tts.speak(msg);

        buddy.lcd.print(0, 0, "Speaking...     ");
        char line1[17];
        snprintf(line1, sizeof(line1), "Temp: %.1f C    ", temp);
        buddy.lcd.print(1, 0, line1);
    }
    lastBtn[0] = btn0;

    // --- Button 1: Speak light level ---
    bool btn1 = buddy.input.buttonPressed(1);
    if (btn1 && !lastBtn[1]) {
        uint16_t lux = buddy.sensor.light();
        char msg[64];
        snprintf(msg, sizeof(msg), "The light level is %d lux.", lux);
        buddy.tts.speak(msg);

        buddy.lcd.print(0, 0, "Speaking...     ");
        char line1[17];
        snprintf(line1, sizeof(line1), "Light: %d lux  ", lux);
        buddy.lcd.print(1, 0, line1);
    }
    lastBtn[1] = btn1;

    // --- Button 2: Custom greeting ---
    bool btn2 = buddy.input.buttonPressed(2);
    if (btn2 && !lastBtn[2]) {
        buddy.tts.speak("Hello! I am your STEM Buddy. I can read sensors and speak to you.");
        buddy.lcd.print(0, 0, "Speaking...     ");
        buddy.lcd.print(1, 0, "Hello message!  ");
    }
    lastBtn[2] = btn2;

    // Default display when not speaking
    static unsigned long lastIdle = 0;
    if (millis() - lastIdle > 5000) {
        lastIdle = millis();
        buddy.lcd.print(0, 0, "Press a button: ");
        buddy.lcd.print(1, 0, "0:Temp 1:Lux 2:Hi");
    }

    delay(50);
}
