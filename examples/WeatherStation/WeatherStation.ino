/**
 * STEM Buddy — Weather Station
 *
 * A complete weather monitoring station using your phone's sensors!
 * Reads temperature, humidity, and barometric pressure, then displays
 * them on the LCD with a simple weather forecast based on pressure
 * trends (rising = improving, falling = worsening).
 *
 * Data is logged for export, and the oscilloscope shows pressure
 * changes over time — a key indicator of weather patterns.
 *
 * What you'll learn:
 *   - Reading temperature, humidity, and pressure sensors
 *   - Calculating pressure trends (rising/falling/steady)
 *   - Building a simple weather prediction algorithm
 *   - Logging data for long-term analysis
 *
 * Sensors used:
 *   - Temperature (°C) → buddy.sensor.temperature()
 *   - Humidity (%)      → buddy.sensor.humidity()
 *   - Pressure (hPa)    → buddy.sensor.pressure()
 *
 * Hardware needed: ESP32 only
 *
 * Steps:
 *   1. Upload this sketch to your ESP32
 *   2. Open STEM Buddy and connect via Bluetooth
 *   3. The LCD shows current conditions and a forecast
 *   4. Leave it running to track pressure trends
 *
 * Created by CodeSkool — https://codeskool.cc
 */

#include <STEMBuddy.h>

STEMBuddy buddy;

unsigned long lastUpdate = 0;
float pressureHistory[6];  // Last 6 readings (one per 10 seconds = 1 minute window)
int historyIndex = 0;
bool historyFull = false;
bool logStarted = false;

// Determine weather forecast from pressure trend
const char* forecast(float current, float oldest) {
    float diff = current - oldest;
    if (diff > 1.0f)       return "Improving ";
    if (diff < -1.0f)      return "Worsening ";
    return "Steady    ";
}

void setup() {
    Serial.begin(115200);
    buddy.begin("Weather-Stn");

    buddy.scope.setLabel("hPa");
    buddy.scope.setRange(9800, 10400); // 980.0 - 1040.0 hPa (x10)

    memset(pressureHistory, 0, sizeof(pressureHistory));
}

void loop() {
    buddy.update();

    if (!buddy.isConnected()) {
        logStarted = false;
        return;
    }

    if (!logStarted) {
        buddy.dataLog.start("Weather");
        buddy.dataLog.addColumn("Temp_C");
        buddy.dataLog.addColumn("Humidity");
        buddy.dataLog.addColumn("Pressure");
        logStarted = true;
    }

    if (millis() - lastUpdate < 10000) return; // Update every 10 seconds
    lastUpdate = millis();

    // Read weather sensors
    float temp     = buddy.sensor.temperature();
    float humidity = buddy.sensor.humidity();
    float pressure = buddy.sensor.pressure();

    // Store pressure history for trend analysis
    pressureHistory[historyIndex] = pressure;
    historyIndex = (historyIndex + 1) % 6;
    if (historyIndex == 0) historyFull = true;

    // Calculate trend from oldest to newest reading
    int oldestIdx = historyFull ? historyIndex : 0;
    const char* trend = forecast(pressure, pressureHistory[oldestIdx]);

    // --- LCD Display ---
    char line0[17], line1[17];
    snprintf(line0, sizeof(line0), "%.1fC %.0f%%     ", temp, humidity);
    snprintf(line1, sizeof(line1), "%.0fhPa %s", pressure, historyFull ? trend : "Wait...");
    buddy.lcd.print(0, 0, line0);
    buddy.lcd.print(1, 0, line1);

    // --- LED color based on conditions ---
    if (pressure > 1015) {
        buddy.led.setColor(0, 200, 255);  // Blue = high pressure (clear sky)
    } else if (pressure > 1005) {
        buddy.led.setColor(255, 255, 100); // Yellow = normal
    } else {
        buddy.led.setColor(150, 150, 150); // Gray = low pressure (cloudy)
    }
    buddy.led.on();

    // --- Oscilloscope: pressure over time ---
    buddy.scope.plot((int16_t)(pressure * 10));

    // --- Log data ---
    buddy.dataLog.log("Temp_C", temp);
    buddy.dataLog.log("Humidity", humidity);
    buddy.dataLog.log("Pressure", pressure);
    buddy.dataLog.newRow();
}
