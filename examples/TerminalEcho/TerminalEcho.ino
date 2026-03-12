/**
 * STEM Buddy — Serial Terminal Echo
 *
 * A bidirectional serial terminal between your ESP32 and the STEM Buddy
 * app. Type a message in the app terminal, and the ESP32 echoes it back
 * with a response. Also shows how to print formatted output like sensor
 * readings and status messages.
 *
 * This is the virtual equivalent of the Arduino Serial Monitor — but
 * wireless over Bluetooth!
 *
 * What you'll learn:
 *   - Sending text from ESP32 to the app terminal
 *   - Reading user input from the app terminal
 *   - Building a command-response interface
 *   - Formatted text output (println, print)
 *
 * Data flow:
 *   App → ESP32: typed text from terminal
 *   ESP32 → App: response text, sensor readings, status
 *
 * Hardware needed: ESP32 only
 *
 * Supported commands:
 *   - "help"   → show available commands
 *   - "temp"   → read temperature sensor
 *   - "light"  → read light sensor
 *   - "led on" → turn LED on (green)
 *   - "led off" → turn LED off
 *   - anything else → echo back with a greeting
 *
 * Steps:
 *   1. Upload this sketch to your ESP32
 *   2. Open STEM Buddy and connect via Bluetooth
 *   3. Open the Serial Terminal in the app
 *   4. Type commands and see the ESP32 respond!
 *
 * Created by CodeSkool — https://codeskool.cc
 */

#include <STEMBuddy.h>

STEMBuddy buddy;

bool welcomed = false;

void processCommand(String cmd) {
    cmd.trim();
    cmd.toLowerCase();

    if (cmd == "help") {
        buddy.terminal.println("Available commands:");
        buddy.terminal.println("  help  - show this message");
        buddy.terminal.println("  temp  - read temperature");
        buddy.terminal.println("  light - read light level");
        buddy.terminal.println("  led on  - turn LED green");
        buddy.terminal.println("  led off - turn LED off");

    } else if (cmd == "temp") {
        float temp = buddy.sensor.temperature();
        char buf[32];
        snprintf(buf, sizeof(buf), "Temperature: %.1f C", temp);
        buddy.terminal.println(buf);

    } else if (cmd == "light") {
        uint16_t lux = buddy.sensor.light();
        char buf[32];
        snprintf(buf, sizeof(buf), "Light: %d lux", lux);
        buddy.terminal.println(buf);

    } else if (cmd == "led on") {
        buddy.led.setColor(0, 255, 0);
        buddy.led.on();
        buddy.terminal.println("LED: ON (green)");

    } else if (cmd == "led off") {
        buddy.led.off();
        buddy.terminal.println("LED: OFF");

    } else {
        buddy.terminal.print("You said: ");
        buddy.terminal.println(cmd.c_str());
    }

    buddy.terminal.println(""); // Blank line between responses
}

void setup() {
    Serial.begin(115200);
    buddy.begin("Terminal-Echo");
}

void loop() {
    buddy.update();

    if (!buddy.isConnected()) {
        welcomed = false;
        return;
    }

    // Send welcome message on first connection
    if (!welcomed) {
        buddy.terminal.println("=== STEM Buddy Terminal ===");
        buddy.terminal.println("Type 'help' for commands.");
        buddy.terminal.println("");
        welcomed = true;
    }

    // Check for user input
    if (buddy.terminal.available()) {
        String input = buddy.terminal.read();
        processCommand(input);

        // Also echo to hardware serial for debugging
        Serial.println("Terminal input: " + input);
    }

    delay(50);
}
