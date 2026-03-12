/**
 * STEM Buddy — GPS Data Logger
 *
 * Reads GPS data from your phone (latitude, longitude, altitude, speed)
 * and displays it on the LCD. The data is also logged to a CSV file
 * using the built-in data logger, so you can export your route later.
 *
 * Take your phone for a walk or drive — the logger records a breadcrumb
 * trail of your GPS coordinates every 2 seconds. The speed is plotted
 * on the oscilloscope in real time.
 *
 * What you'll learn:
 *   - Reading GPS sensor data (lat, lng, alt, speed)
 *   - Using the data logger to record time-series data
 *   - Exporting logged data as CSV from the app
 *   - Plotting speed on the oscilloscope
 *
 * Sensors used:
 *   - GPS latitude   → buddy.sensor.gpsLatitude()
 *   - GPS longitude  → buddy.sensor.gpsLongitude()
 *   - GPS altitude   → buddy.sensor.gpsAltitude()
 *   - GPS speed      → buddy.sensor.gpsSpeed()
 *
 * Hardware needed: ESP32 only (phone GPS must be enabled)
 *
 * Steps:
 *   1. Upload this sketch to your ESP32
 *   2. Enable GPS/Location on your phone
 *   3. Open STEM Buddy and connect via Bluetooth
 *   4. Walk or drive — GPS data is logged every 2 seconds
 *   5. Open the Data Logger in the app to export your route as CSV
 *
 * Created by CodeSkool — https://codeskool.cc
 */

#include <STEMBuddy.h>

STEMBuddy buddy;

unsigned long lastLog = 0;
int rowCount = 0;
bool logStarted = false;

void setup() {
    Serial.begin(115200);
    buddy.begin("GPS-Logger");

    buddy.scope.setLabel("Speed km/h");
    buddy.scope.setRange(0, 100);
}

void loop() {
    buddy.update();

    if (!buddy.isConnected()) return;

    // Start the data logger on first connection
    if (!logStarted) {
        buddy.dataLog.start("GPS_Route");
        buddy.dataLog.addColumn("Latitude");
        buddy.dataLog.addColumn("Longitude");
        buddy.dataLog.addColumn("Altitude");
        buddy.dataLog.addColumn("Speed");
        logStarted = true;
    }

    // Log GPS data every 2 seconds
    if (millis() - lastLog < 2000) return;
    lastLog = millis();

    float lat   = buddy.sensor.gpsLatitude();
    float lng   = buddy.sensor.gpsLongitude();
    uint16_t alt = buddy.sensor.gpsAltitude();
    float speed  = buddy.sensor.gpsSpeed();

    // Display on LCD
    char line0[17], line1[17];

    // Show lat/lng with 4 decimal places
    // dtostrf is more reliable than %f on ESP32 for small buffers
    char latStr[10], lngStr[10];
    dtostrf(lat, 8, 4, latStr);
    dtostrf(lng, 8, 4, lngStr);

    snprintf(line0, sizeof(line0), "%.8s        ", latStr);
    snprintf(line1, sizeof(line1), "%.8s        ", lngStr);
    buddy.lcd.print(0, 0, line0);
    buddy.lcd.print(1, 0, line1);

    // Log to data logger (app shows live table + CSV export)
    buddy.dataLog.log("Latitude", lat);
    buddy.dataLog.log("Longitude", lng);
    buddy.dataLog.log("Altitude", (float)alt);
    buddy.dataLog.log("Speed", speed);
    buddy.dataLog.newRow();
    rowCount++;

    // Plot speed on oscilloscope
    buddy.scope.plot((int16_t)(speed * 10));

    Serial.printf("GPS: %.4f, %.4f, %dm, %.1fkm/h (row %d)\n",
                  lat, lng, alt, speed, rowCount);
}
