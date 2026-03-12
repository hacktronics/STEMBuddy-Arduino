/**
 * STEM Buddy — Temperature Gauge
 *
 * Reads the phone's ambient temperature sensor and displays the value
 * on a virtual 7-segment display, a bar graph (0–50 °C range), and
 * an oscilloscope for tracking changes over time.
 *
 * The bar graph acts as a thermometer — each of the 10 bars represents
 * a 5 °C band. A buzzer alarm sounds when the temperature exceeds a
 * configurable threshold.
 *
 * How it works:
 *   App → ESP32: temperature sensor data
 *   ESP32 → App: 7-segment digits, bar graph levels, scope plot, buzzer
 *
 * Try it:
 *   1. Upload this sketch to your ESP32
 *   2. Open the STEM Buddy app and connect via Bluetooth
 *   3. Move your phone between warm and cool areas
 *   4. Watch the temperature update in real time!
 *
 * Concepts: sensor reading, threshold alerts, multi-display output
 *
 * Created by CodeSkool — https://codeskool.cc
 */

#include <STEMBuddy.h>

STEMBuddy buddy;

const int TEMP_MIN = 0;    // Minimum displayed temperature (°C)
const int TEMP_MAX = 50;   // Maximum displayed temperature (°C)
const int ALARM_TEMP = 40; // Buzzer alarm threshold (°C)

bool alarmActive = false;

void setup() {
    Serial.begin(115200);
    buddy.begin("Temp-Gauge");

    buddy.scope.setLabel("Temp C");
    buddy.scope.setRange(TEMP_MIN, TEMP_MAX);
    buddy.segment7.setColor(0, 200, 255); // Cyan digits
    buddy.barGraph.setColor(STEMBuddyBarGraph::RED); // Red bar graph
}

void loop() {
    buddy.update();

    if (buddy.isConnected()) {
        // Read temperature from the phone's environment sensor
        int temp = buddy.sensor.temperature;

        // Clamp to display range
        int displayTemp = constrain(temp, TEMP_MIN, TEMP_MAX);

        // --- 7-Segment Display: show temperature as 4 digits ---
        // Format: "  27" for 27°C (leading spaces for alignment)
        int d0 = (displayTemp / 10) ? (displayTemp / 10) : 0x0F; // blank if zero
        int d1 = displayTemp % 10;
        // Show "°C" hint: last two digits show degree symbol approximation
        buddy.segment7.setDigits(0x0F, d0, d1, 0x0C);

        // --- Bar Graph: 10 bars, each = 5°C band ---
        uint8_t bars[10];
        for (int i = 0; i < 10; i++) {
            int barThreshold = TEMP_MIN + (i + 1) * 5;
            if (displayTemp >= barThreshold) {
                bars[i] = 255; // Fully lit
            } else if (displayTemp >= barThreshold - 5) {
                // Partially lit — proportional brightness
                bars[i] = map(displayTemp, barThreshold - 5, barThreshold, 0, 255);
            } else {
                bars[i] = 0;
            }
        }
        buddy.barGraph.setAll(bars, 10);

        // Change bar color based on temperature zone
        if (displayTemp < 20) {
            buddy.barGraph.setColor(STEMBuddyBarGraph::BLUE);   // Blue = cool
        } else if (displayTemp < 35) {
            buddy.barGraph.setColor(STEMBuddyBarGraph::GREEN);  // Green = comfortable
        } else {
            buddy.barGraph.setColor(STEMBuddyBarGraph::RED);    // Red = hot
        }

        // --- Oscilloscope: plot temperature over time ---
        buddy.scope.plot(displayTemp);

        // --- LCD: show numeric reading ---
        char line0[17];
        snprintf(line0, sizeof(line0), "Temp: %d C      ", displayTemp);
        buddy.lcd.print(0, 0, line0);

        // --- Buzzer alarm for high temperature ---
        if (displayTemp >= ALARM_TEMP && !alarmActive) {
            buddy.buzzer.tone(1000, 500);
            buddy.lcd.print(1, 0, "!! OVERTEMP !!  ");
            alarmActive = true;
        } else if (displayTemp < ALARM_TEMP - 2) {
            // 2°C hysteresis to prevent alarm flickering
            alarmActive = false;
            buddy.lcd.print(1, 0, "Status: Normal  ");
        }

        delay(500); // Update twice per second
    }
}
