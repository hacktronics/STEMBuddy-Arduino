/**
 * STEM Buddy — Voice-Controlled LED
 *
 * Control your virtual LED with voice commands! The phone's speech
 * recognition engine listens for spoken words and sends the recognized
 * text to your ESP32. This example responds to color commands like
 * "red", "green", "blue", "off", and "rainbow".
 *
 * It's like building a simple voice assistant — your ESP32 understands
 * spoken commands and responds with actions!
 *
 * What you'll learn:
 *   - Reading speech recognition results
 *   - String matching for command parsing
 *   - Voice-controlled hardware interaction
 *   - Handling speech recognition errors
 *
 * Data flow:
 *   App → ESP32: recognized speech text
 *   ESP32 → App: LED color, LCD text, TTS response
 *
 * Hardware needed: ESP32 only
 *
 * Supported voice commands:
 *   "red"     → LED turns red
 *   "green"   → LED turns green
 *   "blue"    → LED turns blue
 *   "white"   → LED turns white
 *   "off"     → LED turns off
 *   "rainbow" → LED cycles through colors
 *
 * Steps:
 *   1. Upload this sketch to your ESP32
 *   2. Open STEM Buddy and connect via Bluetooth
 *   3. Tap the microphone icon in the app
 *   4. Say a color name — watch the LED change!
 *
 * Created by CodeSkool — https://codeskool.cc
 */

#include <STEMBuddy.h>

STEMBuddy buddy;

bool rainbowMode = false;
int rainbowHue = 0;

// Convert a string to lowercase in-place
void toLower(String &s) {
    for (unsigned int i = 0; i < s.length(); i++) {
        s[i] = tolower(s[i]);
    }
}

void processVoiceCommand(String text) {
    toLower(text);
    rainbowMode = false;

    if (text.indexOf("red") >= 0) {
        buddy.led.setColor(255, 0, 0);
        buddy.led.on();
        buddy.lcd.print(1, 0, "LED: Red        ");
        buddy.tts.speak("Red");

    } else if (text.indexOf("green") >= 0) {
        buddy.led.setColor(0, 255, 0);
        buddy.led.on();
        buddy.lcd.print(1, 0, "LED: Green      ");
        buddy.tts.speak("Green");

    } else if (text.indexOf("blue") >= 0) {
        buddy.led.setColor(0, 0, 255);
        buddy.led.on();
        buddy.lcd.print(1, 0, "LED: Blue       ");
        buddy.tts.speak("Blue");

    } else if (text.indexOf("white") >= 0) {
        buddy.led.setColor(255, 255, 255);
        buddy.led.on();
        buddy.lcd.print(1, 0, "LED: White      ");
        buddy.tts.speak("White");

    } else if (text.indexOf("off") >= 0) {
        buddy.led.off();
        buddy.lcd.print(1, 0, "LED: Off        ");
        buddy.tts.speak("Off");

    } else if (text.indexOf("rainbow") >= 0) {
        rainbowMode = true;
        buddy.lcd.print(1, 0, "LED: Rainbow!   ");
        buddy.tts.speak("Rainbow mode!");

    } else {
        buddy.lcd.print(1, 0, "Unknown command ");
        buddy.tts.speak("I didn't understand that.");
    }
}

void setup() {
    Serial.begin(115200);
    buddy.begin("Voice-LED");

    buddy.lcd.print(0, 0, "Say a color:    ");
    buddy.lcd.print(1, 0, "Listening...    ");
}

void loop() {
    buddy.update();

    if (!buddy.isConnected()) return;

    // Check for new speech recognition result
    if (buddy.speech.available()) {
        String text = buddy.speech.lastResult();
        Serial.println("Heard: " + text);

        // Show what was heard
        char line0[17];
        snprintf(line0, sizeof(line0), "\"%-14s\"", text.substring(0, 14).c_str());
        buddy.lcd.print(0, 0, line0);

        processVoiceCommand(text);
    }

    // Handle speech recognition errors
    if (buddy.speech.hasError()) {
        uint8_t err = buddy.speech.lastError();
        const char* errMsg[] = {"No speech", "No match", "Not allowed", "Aborted"};
        if (err < 4) {
            buddy.lcd.print(1, 0, errMsg[err]);
        }
    }

    // Rainbow mode animation
    if (rainbowMode) {
        rainbowHue = (rainbowHue + 3) % 360;
        // Simple HSV-to-RGB (hue only, full saturation/value)
        int h = rainbowHue / 60;
        int f = rainbowHue % 60;
        uint8_t r, g, b;
        switch (h) {
            case 0: r = 255; g = f * 255 / 60;     b = 0;                   break;
            case 1: r = (60 - f) * 255 / 60; g = 255; b = 0;               break;
            case 2: r = 0;   g = 255;               b = f * 255 / 60;       break;
            case 3: r = 0;   g = (60 - f) * 255 / 60; b = 255;             break;
            case 4: r = f * 255 / 60; g = 0;        b = 255;               break;
            default: r = 255; g = 0;                 b = (60 - f) * 255 / 60; break;
        }
        buddy.led.setColor(r, g, b);
        buddy.led.on();
    }

    delay(30);
}
