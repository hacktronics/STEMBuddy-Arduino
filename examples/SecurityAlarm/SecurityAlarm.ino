/**
 * STEM Buddy — Security Alarm System
 *
 * Uses the phone's PIR motion sensor (camera-based person detection) to
 * build a security alarm. When motion is detected:
 *   1. A siren audio file plays on the phone
 *   2. An SMS alert is sent to the configured phone number
 *   3. The LED turns red and the LCD shows an alert
 *
 * This is a great example of combining multiple virtual peripherals —
 * sensor input, audio output, and SMS — into a real-world IoT project.
 *
 * What you'll learn:
 *   - Reading PIR motion sensor data (via button input)
 *   - Playing audio files as alarm sounds
 *   - Sending SMS alerts triggered by sensor events
 *   - Sending a one-time SMS alert on first detection
 *   - Building a multi-component security system
 *
 * Data flow:
 *   App → ESP32: PIR motion detection (button 0)
 *   ESP32 → App: audio play, SMS send, LED, LCD, buzzer
 *
 * Hardware needed: ESP32 only (phone needs SIM card for SMS)
 *
 * Steps:
 *   1. Open the Audio Player in STEM Buddy and import alarm.mp3
 *   2. Update ALERT_PHONE below to the number you want to alert
 *   3. Upload this sketch to your ESP32
 *   4. Open STEM Buddy and connect via Bluetooth
 *   5. Enable camera detection on the PIR Motion Sensor page
 *   6. Walk in front of the camera — the alarm triggers!
 *
 * Created by CodeSkool — https://codeskool.cc
 */

#include <STEMBuddy.h>

STEMBuddy buddy;

// *** CONFIGURATION — change these for your setup ***
const char* ALERT_PHONE = "+919876543210";  // Phone number to receive SMS alerts (in international format)
const char* SIREN_FILE  = "alarm.mp3";

// State
bool armed = true;
bool alarmActive = false;
bool lastMotion = false;
bool smsSent = false;  // SMS sent only once
unsigned long alarmStartTime = 0;

void setup() {
    Serial.begin(115200);
    buddy.begin("Security-Alarm");

    buddy.audio.setVolume(100);
    buddy.audio.setLoop(true);

    buddy.lcd.print(0, 0, "Security Alarm  ");
    buddy.lcd.print(1, 0, "Armed & Ready   ");

    // Green LED = armed and monitoring
    buddy.led.setColor(0, 255, 0);
    buddy.led.on();
}

void loop() {
    buddy.update();

    if (!buddy.isConnected()) return;

    // PIR motion sensor maps to button 0
    bool motionNow = buddy.input.buttonPressed(0);

    // --- Motion detected (rising edge) ---
    if (motionNow && !lastMotion && armed && !alarmActive) {
        alarmActive = true;
        alarmStartTime = millis();

        // 1. Play siren audio
        buddy.audio.play(SIREN_FILE);

        // 2. Red LED alert
        buddy.led.setColor(255, 0, 0);
        buddy.led.on();

        // 3. Buzzer warning burst
        buddy.buzzer.tone(2500, 300);

        // 4. LCD alert
        buddy.lcd.print(0, 0, "!! INTRUDER !!  ");
        buddy.lcd.print(1, 0, "ALARM TRIGGERED ");

        // 5. Send SMS alert (only once)
        if (!smsSent) {
            buddy.sms.send(ALERT_PHONE,
                "SECURITY ALERT!\n"
                "Motion detected by STEM Buddy.\n"
                "Alarm has been triggered.\n"
                "Check your premises immediately.");
            smsSent = true;

            buddy.notify.send("INTRUDER ALERT",
                "Motion detected! SMS sent.", STEMBuddyNotify::ERROR);

            Serial.println("Alarm triggered — SMS sent!");
        } else {
            buddy.notify.send("INTRUDER ALERT",
                "Motion detected again!", STEMBuddyNotify::WARNING);

            Serial.println("Alarm triggered — SMS already sent.");
        }
    }

    // --- Stop siren after 1 minute ---
    if (alarmActive && (millis() - alarmStartTime > 60000)) {

        alarmActive = false;

        // Stop siren
        buddy.audio.stop();

        // Back to green (armed)
        buddy.led.setColor(0, 255, 0);
        buddy.led.on();

        buddy.lcd.print(0, 0, "Security Alarm  ");
        buddy.lcd.print(1, 0, "Armed & Ready   ");

        Serial.println("Alarm reset — monitoring.");
    }

    // --- Blinking red LED while alarm is active ---
    if (alarmActive) {
        bool blink = (millis() / 300) % 2;
        if (blink) {
            buddy.led.setColor(255, 0, 0);
        } else {
            buddy.led.setColor(50, 0, 0);
        }
        buddy.led.on();
    }

    lastMotion = motionNow;
    delay(100);
}
