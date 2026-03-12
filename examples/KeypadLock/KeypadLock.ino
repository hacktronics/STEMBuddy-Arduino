/**
 * STEM Buddy — Keypad Lock (PIN Entry System)
 *
 * Simulates a 4-digit PIN lock using the virtual keypad in the STEM
 * Buddy app. Type the correct PIN to "unlock" — the LED turns green
 * and a success notification appears. Wrong PIN? The LED turns red
 * and a buzzer sounds.
 *
 * The LCD shows asterisks as you type (like a real PIN pad), and
 * pressing '#' submits the code. Press '*' to clear and start over.
 *
 * What you'll learn:
 *   - Reading keypad input character by character
 *   - String comparison for PIN validation
 *   - Building interactive input systems
 *   - Combining multiple outputs (LCD, LED, buzzer)
 *
 * Virtual inputs used:
 *   - Keypad ('0'–'9', '*' to clear, '#' to submit)
 *
 * Hardware needed: ESP32 only
 *
 * Steps:
 *   1. Upload this sketch to your ESP32
 *   2. Open STEM Buddy and connect via Bluetooth
 *   3. Use the virtual keypad to type the PIN: 1234
 *   4. Press '#' to submit — green LED = success, red = wrong PIN
 *   5. Press '*' to clear and try again
 *
 * Created by CodeSkool — https://codeskool.cc
 */

#include <STEMBuddy.h>

STEMBuddy buddy;

const char* SECRET_PIN = "1234";  // Change this to set your own PIN
char enteredPIN[9];               // Buffer for entered digits (max 8)
int pinIndex = 0;
bool locked = true;

void showPINDisplay() {
    // Show asterisks for entered digits
    char display[17] = "PIN: ____       ";
    for (int i = 0; i < pinIndex && i < 4; i++) {
        display[5 + i] = '*';
    }
    buddy.lcd.print(0, 0, display);
}

void setup() {
    Serial.begin(115200);
    buddy.begin("Keypad-Lock");

    memset(enteredPIN, 0, sizeof(enteredPIN));
}

void loop() {
    buddy.update();

    if (!buddy.isConnected()) return;

    if (locked) {
        // Show lock screen
        buddy.led.setColor(255, 100, 0); // Orange = locked
        buddy.led.on();
        showPINDisplay();
        buddy.lcd.print(1, 0, "* Clear  # Enter");

        // Check for keypad input
        if (buddy.input.keypadPressed()) {
            char key = buddy.input.keypad();

            if (key == '*') {
                // Clear — start over
                pinIndex = 0;
                memset(enteredPIN, 0, sizeof(enteredPIN));
                buddy.buzzer.tone(200, 50);

            } else if (key == '#') {
                // Submit — check the PIN
                enteredPIN[pinIndex] = '\0';

                if (strcmp(enteredPIN, SECRET_PIN) == 0) {
                    // Correct PIN — unlock!
                    locked = false;
                    buddy.led.setColor(0, 255, 0);
                    buddy.led.on();
                    buddy.lcd.clear();
                    buddy.lcd.print(0, 0, "  ACCESS GRANTED");
                    buddy.lcd.setBacklightColor(0, 255, 0);
                    buddy.buzzer.tone(1000, 100);
                    delay(200);
                    buddy.buzzer.tone(1500, 200);
                } else {
                    // Wrong PIN
                    buddy.led.setColor(255, 0, 0);
                    buddy.led.on();
                    buddy.lcd.print(0, 0, "  WRONG PIN!    ");
                    buddy.lcd.setBacklightColor(255, 0, 0);
                    buddy.buzzer.tone(300, 500);
                    delay(1500);
                    buddy.lcd.setBacklightColor(255, 255, 255);
                }

                // Reset input
                pinIndex = 0;
                memset(enteredPIN, 0, sizeof(enteredPIN));

            } else if (key >= '0' && key <= '9' && pinIndex < 8) {
                // Digit — add to PIN
                enteredPIN[pinIndex++] = key;
                buddy.buzzer.tone(800, 30); // Key click feedback
            }
        }
    } else {
        // Unlocked state — show success
        buddy.lcd.print(0, 0, "  ACCESS GRANTED");
        buddy.lcd.print(1, 0, "# to re-lock    ");

        // Press '#' to re-lock
        if (buddy.input.keypadPressed()) {
            char key = buddy.input.keypad();
            if (key == '#') {
                locked = true;
                buddy.lcd.setBacklightColor(255, 255, 255);
                buddy.lcd.clear();
            }
        }
    }

    delay(50);
}
