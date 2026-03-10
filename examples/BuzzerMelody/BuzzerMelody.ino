/**
 * STEM Buddy — Buzzer Melody
 *
 * Plays a melody on the virtual buzzer in the STEM Buddy app.
 * Your phone speaker plays the tones — no physical buzzer needed.
 */

#include <STEMBuddy.h>

STEMBuddy buddy;

// Note frequencies (Hz)
#define NOTE_C4  262
#define NOTE_D4  294
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_G4  392
#define NOTE_A4  440
#define NOTE_B4  494
#define NOTE_C5  523

// Twinkle Twinkle Little Star
int melody[] = {
    NOTE_C4, NOTE_C4, NOTE_G4, NOTE_G4, NOTE_A4, NOTE_A4, NOTE_G4,
    NOTE_F4, NOTE_F4, NOTE_E4, NOTE_E4, NOTE_D4, NOTE_D4, NOTE_C4
};
int durations[] = {
    400, 400, 400, 400, 400, 400, 800,
    400, 400, 400, 400, 400, 400, 800
};
int melodyLength = sizeof(melody) / sizeof(melody[0]);

void setup() {
    Serial.begin(115200);
    buddy.begin("Buzzer-Melody");
    buddy.buzzer.setVolume(70);
}

void loop() {
    buddy.update();

    if (buddy.isConnected()) {
        // Play melody
        for (int i = 0; i < melodyLength; i++) {
            buddy.buzzer.tone(melody[i], durations[i]);

            // Show current note on LCD
            buddy.lcd.print(0, 0, "Playing note:   ");
            char buf[17];
            snprintf(buf, sizeof(buf), "%-6dHz         ", melody[i]);
            buddy.lcd.print(1, 0, buf);

            delay(durations[i] + 50); // Wait for note + small gap
        }

        buddy.buzzer.noTone();
        buddy.lcd.clear();
        buddy.lcd.print(0, 0, "Melody done!");
        delay(3000);
    }
}
