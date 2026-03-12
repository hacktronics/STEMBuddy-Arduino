/**
 * STEM Buddy — NFC/RFID Access Control
 *
 * Simulates an NFC-based access control system using your phone's
 * built-in NFC reader. Tap an NFC tag against your phone — the ESP32
 * reads the tag's unique ID (UID) and checks it against a whitelist
 * of authorized tags.
 *
 * Authorized tag → green LED + unlock sound + "Access Granted"
 * Unknown tag    → red LED + alarm sound + "Access Denied"
 *
 * The tag UID and any NDEF text data are displayed on the LCD.
 *
 * What you'll learn:
 *   - Reading NFC tag UIDs
 *   - Reading NDEF text records from NFC tags
 *   - Comparing UIDs against a whitelist
 *   - Building an access control system
 *
 * Data flow:
 *   App → ESP32: NFC tag UID, type, NDEF data
 *   ESP32 → App: LED, LCD, buzzer, notification
 *
 * Hardware needed: ESP32 only (phone must have NFC hardware)
 *
 * Steps:
 *   1. Upload this sketch to your ESP32
 *   2. Open STEM Buddy and connect via Bluetooth
 *   3. Tap an NFC tag or card against your phone
 *   4. The LCD shows the UID — first unknown tag gets "enrolled"
 *   5. Tap the same tag again → Access Granted!
 *
 * Created by CodeSkool — https://codeskool.cc
 */

#include <STEMBuddy.h>

STEMBuddy buddy;

// Store up to 5 authorized tag UIDs
String authorizedTags[5];
int tagCount = 0;
bool lastTagPresent = false;

void grantAccess(const char* uid) {
    buddy.led.setColor(0, 255, 0);
    buddy.led.on();
    buddy.lcd.print(1, 0, "ACCESS GRANTED! ");
    buddy.lcd.setBacklightColor(0, 255, 0);
    buddy.buzzer.tone(1000, 100);
    delay(200);
    buddy.buzzer.tone(1500, 200);
    buddy.notify.send("Access Granted", uid, 1); // SUCCESS icon
}

void denyAccess(const char* uid) {
    buddy.led.setColor(255, 0, 0);
    buddy.led.on();
    buddy.lcd.print(1, 0, "ACCESS DENIED!  ");
    buddy.lcd.setBacklightColor(255, 0, 0);
    buddy.buzzer.tone(300, 500);
    buddy.notify.send("Access Denied", uid, 3); // ERROR icon
}

void enrollTag(const String& uid) {
    if (tagCount < 5) {
        authorizedTags[tagCount++] = uid;
        buddy.led.setColor(0, 100, 255);
        buddy.led.on();
        buddy.lcd.print(1, 0, "TAG ENROLLED!   ");
        buddy.lcd.setBacklightColor(0, 100, 255);
        buddy.buzzer.tone(800, 100);
        delay(100);
        buddy.buzzer.tone(1200, 100);
    }
}

bool isAuthorized(const String& uid) {
    for (int i = 0; i < tagCount; i++) {
        if (authorizedTags[i] == uid) return true;
    }
    return false;
}

void setup() {
    Serial.begin(115200);
    buddy.begin("NFC-Access");

    buddy.lcd.print(0, 0, "Tap NFC tag...  ");
    buddy.lcd.print(1, 0, "Waiting...      ");
}

void loop() {
    buddy.update();

    if (!buddy.isConnected()) return;

    bool tagNow = buddy.nfc.tagPresent();

    // Detect new tag arrival (rising edge)
    if (tagNow && !lastTagPresent) {
        String uid = buddy.nfc.getUIDString();
        String ndefText = buddy.nfc.getNdefText();

        // Show UID on LCD
        char line0[17];
        snprintf(line0, sizeof(line0), "%-16s", uid.substring(0, 16).c_str());
        buddy.lcd.print(0, 0, line0);

        Serial.println("NFC Tag: " + uid);
        if (ndefText.length() > 0) {
            Serial.println("NDEF: " + ndefText);
        }

        // Check against whitelist
        if (isAuthorized(uid)) {
            grantAccess(uid.c_str());
        } else if (tagCount == 0) {
            // First tag ever scanned — auto-enroll as master
            enrollTag(uid);
        } else {
            denyAccess(uid.c_str());
        }

        delay(2000);
        buddy.lcd.setBacklightColor(255, 255, 255);
        buddy.lcd.print(0, 0, "Tap NFC tag...  ");
        buddy.lcd.print(1, 0, "Waiting...      ");
        buddy.led.off();
    }

    lastTagPresent = tagNow;
    delay(50);
}
