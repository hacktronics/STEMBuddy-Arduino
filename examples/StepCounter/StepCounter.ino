/**
 * STEM Buddy — Step Counter Pedometer
 *
 * Turns your ESP32 + phone into a pedometer! The phone's step counter
 * sensor tracks your steps, and the count is displayed on a virtual
 * 7-segment display. The bar graph shows progress toward a daily goal
 * (default: 100 steps for a quick demo).
 *
 * Hit your goal and the LED turns green with a celebration buzzer!
 *
 * What you'll learn:
 *   - Reading the step counter sensor
 *   - Displaying numbers on a 7-segment display
 *   - Using a bar graph for progress tracking
 *   - Goal-based conditional logic
 *
 * Sensors used:
 *   - Step counter → buddy.sensor.steps()
 *
 * Hardware needed: ESP32 only
 *
 * Steps:
 *   1. Upload this sketch to your ESP32
 *   2. Open STEM Buddy and connect via Bluetooth
 *   3. Put your phone in your pocket and walk!
 *   4. The 7-segment display shows your step count
 *   5. Fill the bar graph to reach your goal
 *
 * Created by CodeSkool — https://codeskool.cc
 */

#include <STEMBuddy.h>

STEMBuddy buddy;

unsigned long lastUpdate = 0;
const int STEP_GOAL = 100;  // Set your step goal here
bool goalReached = false;

void setup() {
    Serial.begin(115200);
    buddy.begin("Step-Counter");

    buddy.segment7.setColor(0, 255, 100); // Green digits
    buddy.barGraph.setColor(STEMBuddyBarGraph::GREEN); // Green bars
}

void loop() {
    buddy.update();

    if (!buddy.isConnected()) return;

    if (millis() - lastUpdate < 500) return;
    lastUpdate = millis();

    uint16_t steps = buddy.sensor.steps();

    // --- 7-Segment Display: show step count ---
    buddy.segment7.showNumber(steps);

    // --- Bar Graph: progress toward goal ---
    int progress = map(steps, 0, STEP_GOAL, 0, 10);
    if (progress > 10) progress = 10;

    uint8_t bars[10];
    for (int i = 0; i < 10; i++) {
        bars[i] = (i < progress) ? 255 : 0;
    }
    buddy.barGraph.setAll(bars, 10);

    // --- LCD: show steps and goal ---
    char line0[17], line1[17];
    snprintf(line0, sizeof(line0), "Steps: %-5d    ", steps);
    snprintf(line1, sizeof(line1), "Goal:  %-5d    ", STEP_GOAL);
    buddy.lcd.print(0, 0, line0);
    buddy.lcd.print(1, 0, line1);

    // --- Goal celebration ---
    if (steps >= STEP_GOAL && !goalReached) {
        goalReached = true;
        buddy.led.setColor(0, 255, 0);
        buddy.led.on();
        buddy.segment7.setColor(255, 215, 0); // Gold!
        buddy.barGraph.setColor(STEMBuddyBarGraph::GREEN); // Green

        // Victory buzzer
        buddy.buzzer.tone(1000, 150);
        delay(200);
        buddy.buzzer.tone(1200, 150);
        delay(200);
        buddy.buzzer.tone(1500, 300);
    } else if (steps < STEP_GOAL) {
        goalReached = false;
        buddy.led.setColor(0, 100, 255); // Blue while in progress
        buddy.led.on();
    }
}
