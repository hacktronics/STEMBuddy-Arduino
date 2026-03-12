/**
 * STEM Buddy — Multi-Sensor Data Logger
 *
 * Logs temperature, humidity, light, and sound data to a CSV file
 * through the STEM Buddy app. The app shows a live data table and
 * lets you export the logged data as a CSV file for analysis in
 * Excel or Google Sheets.
 *
 * Perfect for science experiments — log environmental data over time
 * and analyze the results later!
 *
 * What you'll learn:
 *   - Setting up the data logger with named columns
 *   - Logging float and string values to columns
 *   - Creating new rows at regular intervals
 *   - Exporting data from the app as CSV
 *
 * Sensors logged:
 *   - Temperature (°C), Humidity (%), Light (lux), Sound (dB)
 *
 * Hardware needed: ESP32 only
 *
 * Steps:
 *   1. Upload this sketch to your ESP32
 *   2. Open STEM Buddy and connect via Bluetooth
 *   3. Data is logged every 5 seconds automatically
 *   4. Open the Data Logger view in the app to see the live table
 *   5. Tap "Export" in the app to download the CSV file
 *
 * Created by CodeSkool — https://codeskool.cc
 */

#include <STEMBuddy.h>

STEMBuddy buddy;

unsigned long lastLog = 0;
int rowCount = 0;
bool logStarted = false;

const unsigned long LOG_INTERVAL = 5000; // Log every 5 seconds

void setup() {
    Serial.begin(115200);
    buddy.begin("Data-Logger");
}

void loop() {
    buddy.update();

    if (!buddy.isConnected()) {
        logStarted = false;
        return;
    }

    // Initialize data logger on first connection
    if (!logStarted) {
        buddy.dataLog.start("SensorLog");
        buddy.dataLog.addColumn("Temp_C");
        buddy.dataLog.addColumn("Humidity_%");
        buddy.dataLog.addColumn("Light_lux");
        buddy.dataLog.addColumn("Sound_dB");
        logStarted = true;
        rowCount = 0;

        buddy.lcd.print(0, 0, "Data Logger     ");
        buddy.lcd.print(1, 0, "Recording...    ");
    }

    // Log sensor data at regular intervals
    if (millis() - lastLog >= LOG_INTERVAL) {
        lastLog = millis();
        rowCount++;

        // Read all sensors
        float temp     = buddy.sensor.temperature();
        float humidity = buddy.sensor.humidity();
        uint16_t lux   = buddy.sensor.light();
        float sound    = buddy.sensor.soundLevel();

        // Log to data table
        buddy.dataLog.log("Temp_C", temp);
        buddy.dataLog.log("Humidity_%", humidity);
        buddy.dataLog.log("Light_lux", (float)lux);
        buddy.dataLog.log("Sound_dB", sound);
        buddy.dataLog.newRow();

        // Update LCD with latest values
        char line0[17], line1[17];
        snprintf(line0, sizeof(line0), "T:%.1fC H:%.0f%%  ", temp, humidity);
        snprintf(line1, sizeof(line1), "Row:%-4d L:%-4d ", rowCount, lux);
        buddy.lcd.print(0, 0, line0);
        buddy.lcd.print(1, 0, line1);

        // Also plot temperature on oscilloscope
        buddy.scope.plot((int16_t)(temp * 100));

        Serial.printf("Row %d: T=%.1f H=%.1f L=%d S=%.1f\n",
                      rowCount, temp, humidity, lux, sound);
    }
}
