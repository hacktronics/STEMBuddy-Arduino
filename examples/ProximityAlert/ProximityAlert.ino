/**
 * STEM Buddy — Proximity Alert System
 *
 * Uses the phone's proximity sensor to simulate an ultrasonic distance
 * sensor (like the HC-SR04). When an object gets close, the system
 * responds with increasingly urgent buzzer beeps — just like a car
 * parking sensor.
 *
 * The LCD shows the distance reading, and the LED changes color from
 * green (far) to red (close) as a visual warning.
 *
 * How it works:
 *   App → ESP32: proximity sensor data (distance in cm)
 *   ESP32 → App: LCD text, LED color, buzzer tones
 *
 * Try it:
 *   1. Upload this sketch to your ESP32
 *   2. Open the STEM Buddy app and connect via Bluetooth
 *   3. Move your hand toward and away from the phone's proximity sensor
 *   4. Listen to the beep rate change with distance!
 *
 * Concepts: proximity sensing, frequency mapping, conditional alerts
 *
 * Created by CodeSkool — https://codeskool.cc
 */

#include <STEMBuddy.h>

STEMBuddy buddy;

unsigned long lastBeepTime = 0;

void setup() {
    Serial.begin(115200);
    buddy.begin("Proximity-Alert");

    buddy.scope.setLabel("Distance cm");
    buddy.scope.setRange(0, 200);
}

void loop() {
    buddy.update();

    if (buddy.isConnected()) {
        // Read proximity (distance in cm)
        int distance = buddy.sensor.proximity;

        // Clamp to sensor range
        distance = constrain(distance, 0, 200);

        // --- LCD Display ---
        char line0[17], line1[17];
        snprintf(line0, sizeof(line0), "Dist: %-3d cm    ", distance);
        buddy.lcd.print(0, 0, line0);

        // Determine zone and set LED color + beep interval
        int beepInterval;
        if (distance < 10) {
            // DANGER — very close
            buddy.led.setColor(255, 0, 0);
            snprintf(line1, sizeof(line1), "!! TOO CLOSE !! ");
            beepInterval = 100; // Rapid beeping
        } else if (distance < 30) {
            // WARNING — getting close
            buddy.led.setColor(255, 150, 0);
            snprintf(line1, sizeof(line1), "Warning: Close  ");
            beepInterval = 300;
        } else if (distance < 80) {
            // CAUTION — moderate distance
            buddy.led.setColor(255, 255, 0);
            snprintf(line1, sizeof(line1), "Caution         ");
            beepInterval = 600;
        } else {
            // SAFE — far away
            buddy.led.setColor(0, 255, 0);
            snprintf(line1, sizeof(line1), "Clear           ");
            beepInterval = 0; // No beeping
        }

        buddy.led.on();
        buddy.lcd.print(1, 0, line1);

        // --- Buzzer: beep at rate proportional to distance ---
        unsigned long now = millis();
        if (beepInterval > 0 && (now - lastBeepTime >= (unsigned long)beepInterval)) {
            // Short beep — higher pitch when closer
            int freq = map(distance, 0, 80, 2000, 500);
            buddy.buzzer.tone(freq, 80);
            lastBeepTime = now;
        } else if (beepInterval == 0) {
            buddy.buzzer.noTone();
        }

        // --- Oscilloscope: track distance over time ---
        buddy.scope.plot(distance);

        delay(50);
    }
}
