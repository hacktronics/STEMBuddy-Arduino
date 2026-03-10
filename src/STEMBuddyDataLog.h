/**
 * STEMBuddyDataLog.h
 *
 * Data Logger — log named columns of float/string data to the app.
 * The app displays a live table and can export to CSV.
 *
 * Usage:
 *   buddy.dataLog.start("SensorData");
 *   buddy.dataLog.addColumn("Temperature");
 *   buddy.dataLog.addColumn("Humidity");
 *
 *   // In loop:
 *   buddy.dataLog.log("Temperature", 25.3);
 *   buddy.dataLog.log("Humidity", 62.1);
 *   buddy.dataLog.newRow();
 *
 *   // When done:
 *   buddy.dataLog.stop();
 */

#ifndef STEMBUDDY_DATALOG_H
#define STEMBUDDY_DATALOG_H

#include <Arduino.h>

#define DATALOG_MAX_COLUMNS 16

class STEMBuddy;

class STEMBuddyDataLog {
    friend class STEMBuddy;

public:
    STEMBuddyDataLog() : _parent(nullptr), _colCount(0) {}
    void begin(STEMBuddy* parent) { _parent = parent; }

    /**
     * Start a new logging session with a file/session name.
     * Clears any previous column definitions.
     */
    void start(const char* fileName);
    void start(const String& fileName) { start(fileName.c_str()); }

    /**
     * Define a column header. Call after start(), before logging data.
     * Returns the column index (0-based).
     */
    uint8_t addColumn(const char* name);
    uint8_t addColumn(const String& name) { return addColumn(name.c_str()); }

    /**
     * Log a float value to a named column.
     */
    void log(const char* columnName, float value);
    void log(const String& columnName, float value) { log(columnName.c_str(), value); }

    /**
     * Log a string value to a named column.
     */
    void log(const char* columnName, const char* value);
    void log(const String& columnName, const String& value) { log(columnName.c_str(), value.c_str()); }

    /**
     * Commit the current row and start a new one.
     * Call this after setting all column values for one data point.
     */
    void newRow();

    /**
     * End the logging session.
     */
    void stop();

private:
    STEMBuddy* _parent;
    String _colNames[DATALOG_MAX_COLUMNS];
    uint8_t _colCount;

    int8_t _findColumn(const char* name);
};

#endif
