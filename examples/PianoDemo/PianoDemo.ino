/**
 * STEM Buddy — Virtual Piano
 *
 * Turns your phone into a piano keyboard! The app shows piano keys
 * that you can tap to play notes. Your ESP32 reads which keys are
 * pressed and can respond — this example lights up the LED in a
 * different color for each note and shows the note name on the LCD.
 *
 * The ESP32 can also play notes programmatically — this example plays
 * a C major scale on startup to demonstrate.
 *
 * What you'll learn:
 *   - Reading piano key presses from the app
 *   - Playing notes programmatically from ESP32
 *   - Mapping MIDI note numbers to note names
 *   - Changing instrument sounds
 *
 * Data flow:
 *   App → ESP32: key press/release events (note, velocity)
 *   ESP32 → App: noteOn/noteOff commands, LED color, LCD text
 *
 * Hardware needed: ESP32 only (turn up phone volume!)
 *
 * Steps:
 *   1. Upload this sketch to your ESP32
 *   2. Open STEM Buddy and connect via Bluetooth
 *   3. Listen to the C major scale play automatically
 *   4. Then tap the piano keys on your phone!
 *   5. The LED changes color with each note
 *
 * Created by CodeSkool — https://codeskool.cc
 */

#include <STEMBuddy.h>

STEMBuddy buddy;

bool scalePlayedOnce = false;

// Note names for MIDI notes 60–72 (C4–C5)
const char* noteName(uint8_t note) {
    const char* names[] = {
        "C4", "C#4", "D4", "D#4", "E4", "F4",
        "F#4", "G4", "G#4", "A4", "A#4", "B4", "C5"
    };
    if (note >= 60 && note <= 72) return names[note - 60];
    return "?";
}

// Map note to a rainbow color
void noteToColor(uint8_t note, uint8_t &r, uint8_t &g, uint8_t &b) {
    // Cycle through rainbow based on note within octave
    uint8_t pos = (note % 12) * 21; // 0–252 spread
    if (pos < 85) {
        r = 255 - pos * 3; g = pos * 3; b = 0;
    } else if (pos < 170) {
        pos -= 85;
        r = 0; g = 255 - pos * 3; b = pos * 3;
    } else {
        pos -= 170;
        r = pos * 3; g = 0; b = 255 - pos * 3;
    }
}

void setup() {
    Serial.begin(115200);
    buddy.begin("Piano-Demo");

    // Set instrument sound (PIANO, ORGAN, SYNTH, MUSIC_BOX)
    buddy.piano.setInstrument(STEMBuddyPiano::PIANO);
}

void loop() {
    buddy.update();

    if (!buddy.isConnected()) return;

    // Play a C major scale once on first connection
    if (!scalePlayedOnce) {
        scalePlayedOnce = true;
        buddy.lcd.print(0, 0, "Playing C Major ");
        buddy.lcd.print(1, 0, "Scale...        ");

        // C4 to C5: MIDI notes 60, 62, 64, 65, 67, 69, 71, 72
        uint8_t scale[] = {60, 62, 64, 65, 67, 69, 71, 72};
        for (int i = 0; i < 8; i++) {
            buddy.update();
            if (!buddy.isConnected()) break;

            buddy.piano.noteOn(scale[i], 100);
            delay(400);
            buddy.piano.noteOff(scale[i]);
            delay(50);
        }
        buddy.lcd.clear();
    }

    // Listen for user piano key presses
    if (buddy.piano.available()) {
        uint8_t note = buddy.piano.lastNote();
        uint8_t vel  = buddy.piano.lastVelocity();
        bool pressed = buddy.piano.lastPressed();

        if (pressed) {
            // Light up LED with note color
            uint8_t r, g, b;
            noteToColor(note, r, g, b);
            buddy.led.setColor(r, g, b);
            buddy.led.on();

            // Show note on LCD
            char line0[17], line1[17];
            snprintf(line0, sizeof(line0), "Note: %-4s      ", noteName(note));
            snprintf(line1, sizeof(line1), "MIDI: %-3d V:%-3d ", note, vel);
            buddy.lcd.print(0, 0, line0);
            buddy.lcd.print(1, 0, line1);
        } else {
            buddy.led.off();
        }
    }

    delay(20);
}
