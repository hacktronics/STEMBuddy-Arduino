/**
 * STEM Buddy — Speaker Waveform Explorer
 *
 * Generates different audio waveforms through the virtual speaker and
 * visualizes them on the oscilloscope. Use Slider 0 to sweep the
 * frequency and Button 0–3 to switch between waveform types: sine,
 * square, sawtooth, and triangle.
 *
 * This is a great way to hear and see the difference between waveform
 * shapes — a fundamental concept in electronics and audio synthesis.
 *
 * What you'll learn:
 *   - Generating sine, square, sawtooth, and triangle waves
 *   - Controlling frequency with a slider
 *   - Understanding how waveform shape affects sound character
 *   - Using the virtual speaker vs. the virtual buzzer
 *
 * Data flow:
 *   App → ESP32: slider (frequency), buttons (waveform selection)
 *   ESP32 → App: speaker output, oscilloscope plot, LCD text
 *
 * Hardware needed: ESP32 only (turn up phone volume!)
 *
 * Steps:
 *   1. Upload this sketch to your ESP32
 *   2. Open STEM Buddy and connect via Bluetooth
 *   3. Drag Slider 0 to change frequency (100–2000 Hz)
 *   4. Press Buttons 0–3 to change waveform type
 *
 * Created by CodeSkool — https://codeskool.cc
 */

#include <STEMBuddy.h>

STEMBuddy buddy;

const char* waveNames[] = { "Sine", "Square", "Sawtooth", "Triangle" };
STEMBuddySpeaker::Waveform waveforms[] = {
    STEMBuddySpeaker::SINE, STEMBuddySpeaker::SQUARE,
    STEMBuddySpeaker::SAWTOOTH, STEMBuddySpeaker::TRIANGLE
};
int currentWave = 0;
bool lastBtn[4] = {false, false, false, false};

void setup() {
    Serial.begin(115200);
    buddy.begin("Speaker-Waves");

    buddy.speaker.setVolume(60);
    buddy.scope.setLabel("Frequency");
    buddy.scope.setRange(100, 2000);
}

void loop() {
    buddy.update();

    if (!buddy.isConnected()) return;

    // --- Waveform selection via buttons (rising edge) ---
    for (int i = 0; i < 4; i++) {
        bool pressed = buddy.input.buttonPressed(i);
        if (pressed && !lastBtn[i]) {
            currentWave = i;
            buddy.speaker.setWaveform(waveforms[currentWave]);
        }
        lastBtn[i] = pressed;
    }

    // --- Frequency control via slider (100–2000 Hz) ---
    uint8_t sliderVal = buddy.input.slider(0);
    int freq = map(sliderVal, 0, 255, 100, 2000);

    // Play the tone continuously
    buddy.speaker.note(freq, 0, waveforms[currentWave]); // duration=0 → continuous

    // --- LCD display ---
    char line0[17], line1[17];
    snprintf(line0, sizeof(line0), "Wave: %-9s", waveNames[currentWave]);
    snprintf(line1, sizeof(line1), "Freq: %-4d Hz   ", freq);
    buddy.lcd.print(0, 0, line0);
    buddy.lcd.print(1, 0, line1);

    // --- Oscilloscope ---
    buddy.scope.plot((int16_t)freq);

    delay(50);
}
