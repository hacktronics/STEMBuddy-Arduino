/**
 * STEM Buddy — Audio File Player
 *
 * Plays audio files through the STEM Buddy app using virtual buttons
 * as a media controller. Button 0 plays/pauses, Button 1 stops, and
 * Slider 0 controls volume.
 *
 * The app can play audio files stored on the device (MP3, WAV, OGG).
 * This example plays a built-in demo sound, but you can change the
 * filename to any audio file accessible by the app.
 *
 * What you'll learn:
 *   - Playing, pausing, and stopping audio files
 *   - Controlling playback volume
 *   - Building a media player interface
 *   - Combining button/slider inputs with audio output
 *
 * Data flow:
 *   App → ESP32: button presses, slider (volume)
 *   ESP32 → App: play/pause/stop commands, LCD status
 *
 * Hardware needed: ESP32 only (turn up phone volume!)
 *
 * Steps:
 *   1. Upload this sketch to your ESP32
 *   2. Open STEM Buddy and connect via Bluetooth
 *   3. Press Button 0 to play/pause
 *   4. Press Button 1 to stop
 *   5. Drag Slider 0 to control volume
 *
 * Created by CodeSkool — https://codeskool.cc
 */

#include <STEMBuddy.h>

STEMBuddy buddy;

bool isPlaying = false;
bool isPaused = false;
bool lastBtn0 = false;
bool lastBtn1 = false;
uint8_t lastVolume = 0;

void setup() {
    Serial.begin(115200);
    buddy.begin("Audio-Player");

    buddy.audio.setVolume(70);
    buddy.audio.setLoop(true);  // Loop playback
}

void loop() {
    buddy.update();

    if (!buddy.isConnected()) return;

    // --- Button 0: Play / Pause toggle ---
    bool btn0 = buddy.input.buttonPressed(0);
    if (btn0 && !lastBtn0) {
        if (!isPlaying) {
            buddy.audio.play("demo.mp3");
            isPlaying = true;
            isPaused = false;
        } else if (!isPaused) {
            buddy.audio.pause();
            isPaused = true;
        } else {
            buddy.audio.resume();
            isPaused = false;
        }
    }
    lastBtn0 = btn0;

    // --- Button 1: Stop ---
    bool btn1 = buddy.input.buttonPressed(1);
    if (btn1 && !lastBtn1) {
        buddy.audio.stop();
        isPlaying = false;
        isPaused = false;
    }
    lastBtn1 = btn1;

    // --- Slider 0: Volume control ---
    uint8_t vol = buddy.input.slider(0);
    if (vol != lastVolume) {
        int volume = map(vol, 0, 255, 0, 100);
        buddy.audio.setVolume(volume);
        lastVolume = vol;
    }

    // --- LCD: show playback status ---
    const char* status;
    if (!isPlaying)     status = "Stopped ";
    else if (isPaused)  status = "Paused  ";
    else                status = "Playing ";

    char line0[17], line1[17];
    snprintf(line0, sizeof(line0), "%-16s", status);

    int displayVol = map(lastVolume, 0, 255, 0, 100);
    snprintf(line1, sizeof(line1), "Vol: %-3d%%       ", displayVol);
    buddy.lcd.print(0, 0, line0);
    buddy.lcd.print(1, 0, line1);

    // --- LED: green when playing, orange when paused, off when stopped ---
    if (isPlaying && !isPaused) {
        buddy.led.setColor(0, 255, 0);
        buddy.led.on();
    } else if (isPaused) {
        buddy.led.setColor(255, 150, 0);
        buddy.led.on();
    } else {
        buddy.led.off();
    }

    delay(50);
}
