/**
 * STEM Buddy — Buzzer Melody
 *
 * Plays "Twinkle Twinkle Little Star" on a virtual buzzer through your
 * phone's speaker. Each note's frequency is displayed on the LCD as it
 * plays, and the melody loops continuously.
 *
 * This demonstrates how the ESP32 can generate sound through the phone
 * without any physical buzzer or speaker — the phone IS the buzzer.
 *
 * What you'll learn:
 *   - Playing tones at specific frequencies
 *   - Controlling tone duration
 *   - Adjusting buzzer volume
 *   - Combining audio output with LCD display
 *
 * Hardware needed: ESP32 only (make sure phone volume is up!)
 *
 * Steps:
 *   1. Upload this sketch to your ESP32
 *   2. Open STEM Buddy and connect via Bluetooth
 *   3. Turn up your phone volume
 *   4. Listen to the melody play through your phone!
 *
 * Created by CodeSkool — https://codeskool.cc
 */

#include <STEMBuddy.h>

STEMBuddy buddy;

// Standard note frequencies in Hz
#define NOTE_C4  262
#define NOTE_D4  294
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_G4  392
#define NOTE_A4  440
#define NOTE_B4  494
#define NOTE_C5  523

// Note names for LCD display (matches melody array)
const char* noteNames[] = {
    "C4", "C4", "G4", "G4", "A4", "A4", "G4",
    "F4", "F4", "E4", "E4", "D4", "D4", "C4"
};

// Twinkle Twinkle Little Star — melody and rhythm
int melody[] = {
    NOTE_C4, NOTE_C4, NOTE_G4, NOTE_G4, NOTE_A4, NOTE_A4, NOTE_G4,
    NOTE_F4, NOTE_F4, NOTE_E4, NOTE_E4, NOTE_D4, NOTE_D4, NOTE_C4
};

int durations[] = {
    400, 400, 400, 400, 400, 400, 800,
    400, 400, 400, 400, 400, 400, 800
};

const int melodyLength = sizeof(melody) / sizeof(melody[0]);

void setup() {
    Serial.begin(115200);
    buddy.begin("Buzzer-Melody");

    // Set comfortable volume (0–100)
    buddy.buzzer.setVolume(70);
}

void loop() {
    buddy.update();

    if (!buddy.isConnected()) return;

    buddy.lcd.print(0, 0, "Now Playing:    ");
    buddy.lcd.print(1, 0, "Twinkle Twinkle ");
    delay(2000);

    // Play each note in the melody
    for (int i = 0; i < melodyLength; i++) {
        // Keep BLE connection alive during playback
        buddy.update();
        if (!buddy.isConnected()) break;

        // Play the note
        buddy.buzzer.tone(melody[i], durations[i]);

        // Show which note is playing
        char line0[17], line1[17];
        snprintf(line0, sizeof(line0), "Note: %-4s       ", noteNames[i]);
        snprintf(line1, sizeof(line1), "Freq: %-4d Hz   ", melody[i]);
        buddy.lcd.print(0, 0, line0);
        buddy.lcd.print(1, 0, line1);

        // Wait for the note to finish, plus a small gap between notes
        delay(durations[i] + 50);
    }

    // Silence between repetitions
    buddy.buzzer.noTone();
    buddy.lcd.clear();
    buddy.lcd.print(0, 0, "Melody complete!");
    buddy.lcd.print(1, 0, "Restarting...   ");
    delay(3000);
}
