/**
 * STEM Buddy — Virtual SD Card File Logger
 *
 * Uses the phone as a virtual MicroSD card! Your ESP32 can create,
 * write, read, and delete files stored on the phone — no physical
 * SD card module needed. This example logs sensor data to a CSV file,
 * then reads it back to verify, and lists all files on the virtual SD.
 *
 * The CSV file can be accessed and shared from the STEM Buddy app,
 * making it easy to export data for analysis in Excel or Google Sheets.
 *
 * What you'll learn:
 *   - Creating and writing files on the virtual SD
 *   - Appending data to existing files
 *   - Reading file contents back asynchronously
 *   - Checking if a file exists and getting its size
 *   - Listing all files on the virtual SD
 *   - Building a CSV data logger
 *
 * Data flow:
 *   ESP32 → App: file write/read/delete/list commands
 *   App → ESP32: file contents, existence status, file listings
 *
 * Hardware needed: ESP32 only
 *
 * Steps:
 *   1. Upload this sketch to your ESP32
 *   2. Open STEM Buddy and connect via Bluetooth
 *   3. Sensor data is automatically logged to "sensors.csv" every 5 seconds
 *   4. Press Button 0 to read back the file contents
 *   5. Press Button 1 to list all files on the virtual SD
 *   6. Press Button 2 to delete the log file and start fresh
 *
 * Created by CodeSkool — https://codeskool.cc
 */

#include <STEMBuddy.h>

STEMBuddy buddy;

const char* LOG_FILE = "sensors.csv";
const unsigned long LOG_INTERVAL = 5000; // Log every 5 seconds

unsigned long lastLog = 0;
int rowCount = 0;
bool headerWritten = false;
bool lastBtn0 = false;
bool lastBtn1 = false;
bool lastBtn2 = false;

void setup() {
    Serial.begin(115200);
    buddy.begin("SD-Logger");

    buddy.lcd.print(0, 0, "SD Card Logger  ");
    buddy.lcd.print(1, 0, "Starting...     ");
}

void loop() {
    buddy.update();

    if (!buddy.isConnected()) return;

    // --- Write CSV header on first run ---
    if (!headerWritten) {
        buddy.sd.writeFile(LOG_FILE, "Time_ms,Temp_C,Humidity,Light_lux\n");
        headerWritten = true;
        buddy.lcd.print(1, 0, "File created    ");
        delay(1000);
    }

    // --- Auto-log sensor data every 5 seconds ---
    if (millis() - lastLog >= LOG_INTERVAL) {
        lastLog = millis();
        rowCount++;

        float temp     = buddy.sensor.temperature();
        float humidity = buddy.sensor.humidity();
        uint16_t lux   = buddy.sensor.light();

        // Format as CSV row and append to file
        char row[64];
        snprintf(row, sizeof(row), "%lu,%.1f,%.1f,%d\n",
                 millis(), temp, humidity, lux);
        buddy.sd.appendFile(LOG_FILE, row);

        // Update LCD
        char line0[17], line1[17];
        snprintf(line0, sizeof(line0), "Logging #%-4d   ", rowCount);
        snprintf(line1, sizeof(line1), "T:%.1f H:%.0f%%   ", temp, humidity);
        buddy.lcd.print(0, 0, line0);
        buddy.lcd.print(1, 0, line1);

        // Green blink to show logging activity
        buddy.led.setColor(0, 255, 0);
        buddy.led.on();
        delay(100);
        buddy.led.off();

        Serial.printf("Logged row %d: T=%.1f H=%.1f L=%d\n",
                      rowCount, temp, humidity, lux);
    }

    // --- Button 0: Read back the CSV file ---
    bool btn0 = buddy.input.buttonPressed(0);
    if (btn0 && !lastBtn0) {
        buddy.lcd.print(0, 0, "Reading file... ");
        buddy.sd.readFile(LOG_FILE);
        buddy.buzzer.tone(800, 50);
    }
    lastBtn0 = btn0;

    // Check for read result
    if (buddy.sd.available()) {
        String content = buddy.sd.readResult();
        Serial.println("=== File Contents ===");
        Serial.println(content);
        Serial.println("=== End ===");

        // Show file size on LCD
        char line[17];
        snprintf(line, sizeof(line), "Read: %d bytes  ", content.length());
        buddy.lcd.print(0, 0, line);
        buddy.lcd.print(1, 0, "See Serial Mon  ");
        buddy.notify.send("File Read", "Contents printed to Serial", 0);
    }

    // --- Button 1: List all files ---
    bool btn1 = buddy.input.buttonPressed(1);
    if (btn1 && !lastBtn1) {
        buddy.lcd.print(0, 0, "Listing files...");
        buddy.sd.listFiles();
        buddy.buzzer.tone(800, 50);
    }
    lastBtn1 = btn1;

    // Check for list result
    if (buddy.sd.hasListResult()) {
        String fileList = buddy.sd.getFileList();
        Serial.println("=== Files on SD ===");
        Serial.println(fileList);
        Serial.println("=== End ===");

        buddy.lcd.print(0, 0, "Files listed    ");
        buddy.lcd.print(1, 0, "See Serial Mon  ");
    }

    // --- Button 2: Delete log file and start fresh ---
    bool btn2 = buddy.input.buttonPressed(2);
    if (btn2 && !lastBtn2) {
        buddy.sd.deleteFile(LOG_FILE);
        headerWritten = false;
        rowCount = 0;

        buddy.lcd.print(0, 0, "File deleted!   ");
        buddy.lcd.print(1, 0, "Starting fresh  ");
        buddy.led.setColor(255, 100, 0);
        buddy.led.on();
        buddy.buzzer.tone(500, 200);
        buddy.notify.send("File Deleted", LOG_FILE, 2);
        delay(2000);
        buddy.led.off();
    }
    lastBtn2 = btn2;

    delay(50);
}
