/**
 * STEM Buddy — MAX7219 LED Matrix Animation
 *
 * Displays animated patterns on a virtual 8×8 LED matrix. Cycles
 * through several classic animations: smiley face, heart, checkerboard,
 * and a scrolling wave effect.
 *
 * The MAX7219 is a popular LED matrix driver chip. With STEM Buddy,
 * you don't need the physical chip or matrix — the phone renders it!
 *
 * What you'll learn:
 *   - Setting individual rows of an 8×8 matrix (bitmask)
 *   - Creating bitmap patterns with binary notation
 *   - Animating frames in sequence
 *   - Controlling matrix intensity (brightness)
 *
 * Hardware needed: ESP32 only
 *
 * Steps:
 *   1. Upload this sketch to your ESP32
 *   2. Open STEM Buddy and connect via Bluetooth
 *   3. Watch the LED matrix cycle through animations
 *
 * Created by CodeSkool — https://codeskool.cc
 */

#include <STEMBuddy.h>

STEMBuddy buddy;

// --- Bitmap Patterns (8 rows, MSB = leftmost pixel) ---

const uint8_t smiley[8] = {
    0b00111100,  //   ****
    0b01000010,  //  *    *
    0b10100101,  // * *  * *
    0b10000001,  // *      *
    0b10100101,  // * *  * *
    0b10011001,  // *  **  *
    0b01000010,  //  *    *
    0b00111100   //   ****
};

const uint8_t heart[8] = {
    0b00000000,  //
    0b01100110,  //  **  **
    0b11111111,  // ********
    0b11111111,  // ********
    0b01111110,  //  ******
    0b00111100,  //   ****
    0b00011000,  //    **
    0b00000000   //
};

const uint8_t arrow[8] = {
    0b00010000,  //    *
    0b00110000,  //   **
    0b01110000,  //  ***
    0b11111111,  // ********
    0b11111111,  // ********
    0b01110000,  //  ***
    0b00110000,  //   **
    0b00010000   //    *
};

const uint8_t checkerA[8] = {
    0b10101010,
    0b01010101,
    0b10101010,
    0b01010101,
    0b10101010,
    0b01010101,
    0b10101010,
    0b01010101
};

const uint8_t checkerB[8] = {
    0b01010101,
    0b10101010,
    0b01010101,
    0b10101010,
    0b01010101,
    0b10101010,
    0b01010101,
    0b10101010
};

// Array of all patterns
const uint8_t* patterns[] = { smiley, heart, arrow, checkerA, checkerB };
const int numPatterns = sizeof(patterns) / sizeof(patterns[0]);

void displayPattern(const uint8_t* pattern) {
    for (int row = 0; row < 8; row++) {
        buddy.matrix.setRow(0, row, pattern[row]);
    }
}

void setup() {
    Serial.begin(115200);
    buddy.begin("Matrix-Anim");

    buddy.matrix.setIntensity(8); // Medium brightness (0–15)
    buddy.matrix.setColor(0, 255, 0); // Green LEDs
}

void loop() {
    buddy.update();

    if (!buddy.isConnected()) return;

    // --- Phase 1: Show static patterns (2 seconds each) ---
    for (int p = 0; p < 3; p++) { // smiley, heart, arrow
        buddy.update();
        if (!buddy.isConnected()) break;

        displayPattern(patterns[p]);
        delay(2000);
    }

    // --- Phase 2: Checkerboard animation (flashing) ---
    for (int i = 0; i < 8; i++) {
        buddy.update();
        if (!buddy.isConnected()) break;

        displayPattern((i % 2 == 0) ? checkerA : checkerB);
        delay(300);
    }

    // --- Phase 3: Scrolling wave effect ---
    for (int frame = 0; frame < 16; frame++) {
        buddy.update();
        if (!buddy.isConnected()) break;

        uint8_t wave[8];
        for (int row = 0; row < 8; row++) {
            // Create a single-column wave that moves across
            int col = (frame + row) % 8;
            wave[row] = 1 << (7 - col);
        }
        for (int row = 0; row < 8; row++) {
            buddy.matrix.setRow(0, row, wave[row]);
        }
        delay(150);
    }

    buddy.matrix.clear();
    delay(500);
}
