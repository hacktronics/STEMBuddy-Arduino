/**
 * STEM Buddy — Camera Capture
 *
 * Triggers your phone's camera from ESP32 code! Press a virtual button
 * to take a photo, with configurable quality and flash settings. The
 * LCD shows a live countdown before each capture, and a notification
 * confirms when the photo is saved.
 *
 * You can also set up automatic time-lapse photography — the camera
 * captures a photo every N seconds when Button 1 enables time-lapse mode.
 *
 * What you'll learn:
 *   - Triggering the phone camera from ESP32
 *   - Setting JPEG quality and flash mode
 *   - Reading camera status (idle, capturing, captured, error)
 *   - Building a time-lapse system
 *
 * Data flow:
 *   ESP32 → App: capture command, quality/flash settings
 *   App → ESP32: camera status updates, button presses
 *
 * Hardware needed: ESP32 only
 *
 * Steps:
 *   1. Upload this sketch to your ESP32
 *   2. Open STEM Buddy and connect via Bluetooth
 *   3. Press Button 0 to take a single photo
 *   4. Press Button 1 to toggle time-lapse mode (photo every 10 seconds)
 *   5. Use Slider 0 to adjust quality (low → high)
 *
 * Created by CodeSkool — https://codeskool.cc
 */

#include <STEMBuddy.h>

STEMBuddy buddy;

bool lastBtn0 = false;
bool lastBtn1 = false;
bool timelapseActive = false;
unsigned long lastCapture = 0;
int photoCount = 0;

const unsigned long TIMELAPSE_INTERVAL = 10000; // 10 seconds between shots

// Camera status names for LCD
const char* statusName(uint8_t s) {
    switch (s) {
        case 0:  return "Ready   ";
        case 1:  return "Taking..";;
        case 2:  return "Saved!  ";
        case 3:  return "Error!  ";
        default: return "Unknown ";
    }
}

void takePhoto() {
    // Show countdown on LCD
    for (int i = 3; i > 0; i--) {
        buddy.update();
        char line[17];
        snprintf(line, sizeof(line), "Capturing in %d..", i);
        buddy.lcd.print(1, 0, line);
        buddy.buzzer.tone(800, 100);
        delay(1000);
    }

    // Flash! Take the photo
    buddy.buzzer.tone(1500, 200);
    buddy.led.setColor(255, 255, 255);
    buddy.led.on();

    buddy.camera.capture();
    photoCount++;

    delay(500);
    buddy.led.off();
}

void setup() {
    Serial.begin(115200);
    buddy.begin("Camera");

    // Set camera defaults
    buddy.camera.setQuality(85);  // Good quality JPEG
    buddy.camera.setFlash(STEMBuddyCamera::AUTO);  // Auto flash

    buddy.lcd.print(0, 0, "Camera Ready    ");
    buddy.lcd.print(1, 0, "Btn0:Snap Btn1:TL");
}

void loop() {
    buddy.update();

    if (!buddy.isConnected()) return;

    // --- Slider 0: Adjust JPEG quality ---
    uint8_t qualSlider = buddy.input.slider(0);
    if (qualSlider > 0) {
        int quality = map(qualSlider, 0, 255, 30, 100);
        buddy.camera.setQuality(quality);
    }

    // --- Button 0: Single capture ---
    bool btn0 = buddy.input.buttonPressed(0);
    if (btn0 && !lastBtn0) {
        takePhoto();
    }
    lastBtn0 = btn0;

    // --- Button 1: Toggle time-lapse mode ---
    bool btn1 = buddy.input.buttonPressed(1);
    if (btn1 && !lastBtn1) {
        timelapseActive = !timelapseActive;
        if (timelapseActive) {
            buddy.notify.send("Time-Lapse", "Started (10s interval)", 0);
            buddy.led.setColor(0, 255, 0);
            buddy.led.on();
        } else {
            buddy.notify.send("Time-Lapse", "Stopped", 0);
            buddy.led.off();
        }
    }
    lastBtn1 = btn1;

    // --- Time-lapse auto-capture ---
    if (timelapseActive && millis() - lastCapture >= TIMELAPSE_INTERVAL) {
        lastCapture = millis();
        takePhoto();
    }

    // --- LCD: show status ---
    char line0[17];
    snprintf(line0, sizeof(line0), "%s #%-3d    ",
             statusName(buddy.camera.status()), photoCount);
    buddy.lcd.print(0, 0, line0);

    if (!timelapseActive) {
        buddy.lcd.print(1, 0, "Btn0:Snap Btn1:TL");
    } else {
        unsigned long nextIn = TIMELAPSE_INTERVAL - (millis() - lastCapture);
        char line1[17];
        snprintf(line1, sizeof(line1), "TL: next in %lds ", nextIn / 1000);
        buddy.lcd.print(1, 0, line1);
    }

    delay(100);
}
