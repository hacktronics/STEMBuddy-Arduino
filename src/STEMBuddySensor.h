/**
 * STEMBuddySensor.h
 *
 * Virtual Sensors — the phone SENDS sensor data TO your ESP32.
 * The phone already has accelerometer, gyroscope, magnetometer, light sensor, etc.
 * This class lets your ESP32 code read them as if they were wired sensors.
 *
 * Usage:
 *   float temp = buddy.sensor.temperature();  // Read virtual temperature
 *   uint16_t lux = buddy.sensor.light();       // Read virtual light level
 *   float ax   = buddy.sensor.accelX();       // Phone's accelerometer X
 *   float head = buddy.sensor.heading();      // Compass heading (degrees)
 *   float dB   = buddy.sensor.soundLevel();   // Ambient sound level
 *
 * Check for new data:
 *   if (buddy.sensor.updated()) {
 *     // At least one sensor value changed since last check
 *   }
 *
 * The values are updated automatically when the app sends BLE notifications.
 * Call buddy.update() in loop() to keep them fresh.
 */

#ifndef STEMBUDDY_SENSOR_H
#define STEMBUDDY_SENSOR_H

#include <Arduino.h>
#include <math.h>

class STEMBuddy;

class STEMBuddySensor {
    friend class STEMBuddy; // STEMBuddy calls _on* methods

public:
    STEMBuddySensor() : _parent(nullptr),
        _temperature(0), _light(0), _humidity(0), _pressure(0),
        _accelX(0), _accelY(0), _accelZ(0),
        _gyroX(0), _gyroY(0), _gyroZ(0),
        _magX(0), _magY(0), _magZ(0),
        _proximity(0), _sound(0), _steps(0),
        _gpsLat(0), _gpsLng(0), _gpsAlt(0), _gpsSpeed(0),
        _colorR(0), _colorG(0), _colorB(0),
        _gridSize(0),
        _updated(false) {
        memset(_gridR, 0, sizeof(_gridR));
        memset(_gridG, 0, sizeof(_gridG));
        memset(_gridB, 0, sizeof(_gridB));
    }
    void begin(STEMBuddy* parent) { _parent = parent; }

    // ─── Environment Sensors ─────────────────────────────────────

    /** Temperature in degrees Celsius. */
    float temperature() { return _temperature / 100.0f; }

    /** Light level in lux. */
    uint16_t light() { return _light; }

    /** Relative humidity in %. */
    float humidity() { return _humidity / 100.0f; }

    /** Barometric pressure in hPa. */
    float pressure() { return _pressure / 10.0f; }

    /** Ambient sound level in dB. */
    float soundLevel() { return _sound / 10.0f; }

    /** Proximity distance in cm (0 = nothing detected). */
    uint16_t proximity() { return _proximity; }

    // ─── Motion Sensors ──────────────────────────────────────────

    /** Accelerometer X axis (m/s²). */
    float accelX() { return _accelX / 100.0f; }

    /** Accelerometer Y axis (m/s²). */
    float accelY() { return _accelY / 100.0f; }

    /** Accelerometer Z axis (m/s²). */
    float accelZ() { return _accelZ / 100.0f; }

    /** Gyroscope X axis (deg/s). */
    float gyroX() { return _gyroX / 100.0f; }

    /** Gyroscope Y axis (deg/s). */
    float gyroY() { return _gyroY / 100.0f; }

    /** Gyroscope Z axis (deg/s). */
    float gyroZ() { return _gyroZ / 100.0f; }

    /** Magnetometer X axis (µT). */
    float magX() { return _magX / 10.0f; }

    /** Magnetometer Y axis (µT). */
    float magY() { return _magY / 10.0f; }

    /** Magnetometer Z axis (µT). */
    float magZ() { return _magZ / 10.0f; }

    // ─── Activity Sensors ────────────────────────────────────────

    /** Step count since connection started. */
    uint16_t steps() { return _steps; }

    // ─── GPS Sensors ──────────────────────────────────────────────

    /** GPS latitude in degrees. */
    float gpsLatitude() { return _gpsLat / 10000.0f; }

    /** GPS longitude in degrees. */
    float gpsLongitude() { return _gpsLng / 10000.0f; }

    /** GPS altitude in meters. */
    uint16_t gpsAltitude() { return _gpsAlt; }

    /** GPS speed in km/h. */
    float gpsSpeed() { return _gpsSpeed / 10.0f; }

    // ─── Color Sensor ─────────────────────────────────────────────

    /** Color sensor red component (0-255). */
    uint8_t colorR() { return _colorR; }

    /** Color sensor green component (0-255). */
    uint8_t colorG() { return _colorG; }

    /** Color sensor blue component (0-255). */
    uint8_t colorB() { return _colorB; }

    /** Color grid size (1, 3, or 5). */
    uint8_t colorGridSize() { return _gridSize; }

    /** Get color at a grid cell. Returns RGB via out parameters. */
    void colorGridCell(uint8_t cellIndex, uint8_t &r, uint8_t &g, uint8_t &b) {
        if (cellIndex < 25) {
            r = _gridR[cellIndex]; g = _gridG[cellIndex]; b = _gridB[cellIndex];
        } else {
            r = 0; g = 0; b = 0;
        }
    }

    // ─── Convenience Methods ─────────────────────────────────────

    /** Compass heading in degrees (0-360, 0=North). Uses magnetometer X/Y. */
    float heading() {
        float mx = _magX / 10.0f;
        float my = _magY / 10.0f;
        float h = atan2(mx, my) * 180.0f / M_PI;
        if (h < 0) h += 360.0f;
        return h;
    }

    /** Tilt angle from vertical in degrees (0=flat, 90=upright). Uses accelerometer. */
    float tilt() {
        float ax = _accelX / 100.0f;
        float ay = _accelY / 100.0f;
        float az = _accelZ / 100.0f;
        float total = sqrtf(ax * ax + ay * ay + az * az);
        if (total < 0.01f) return 0;
        float ratio = az / total;
        // Clamp to [-1, 1] to prevent NaN from float precision
        if (ratio > 1.0f) ratio = 1.0f;
        if (ratio < -1.0f) ratio = -1.0f;
        return acosf(ratio) * 180.0f / M_PI;
    }

    /**
     * Check if any sensor value was updated since the last call.
     * Returns true once, then resets until new data arrives.
     */
    bool updated() {
        bool u = _updated;
        _updated = false;
        return u;
    }

private:
    STEMBuddy* _parent;

    // Raw values (scaled integers from BLE)
    int16_t  _temperature;  // x100 (2550 = 25.50°C)
    uint16_t _light;        // lux
    int16_t  _humidity;     // x100 (5500 = 55.00%)
    uint16_t _pressure;     // x10  (10130 = 1013.0 hPa)
    int16_t  _accelX, _accelY, _accelZ; // x100
    int16_t  _gyroX, _gyroY, _gyroZ;    // x100
    int16_t  _magX, _magY, _magZ;       // x10 (µT)
    uint16_t _proximity;    // cm
    uint16_t _sound;        // x10 (dB)
    uint16_t _steps;        // count
    int32_t  _gpsLat;       // x10000 (degrees)
    int32_t  _gpsLng;       // x10000 (degrees)
    uint16_t _gpsAlt;       // meters
    uint16_t _gpsSpeed;     // x10 (km/h)
    uint8_t  _colorR, _colorG, _colorB; // 0-255
    uint8_t  _gridSize;                // 1, 3, or 5
    uint8_t  _gridR[25], _gridG[25], _gridB[25]; // max 5x5 grid

    bool _updated;

    // Called by STEMBuddy when BLE data arrives from app
    void _onTemperature(int16_t raw) { _temperature = raw; _updated = true; }
    void _onLight(uint16_t raw)      { _light = raw; _updated = true; }
    void _onHumidity(int16_t raw)    { _humidity = raw; _updated = true; }
    void _onPressure(uint16_t raw)   { _pressure = raw; _updated = true; }
    void _onAccel(uint8_t axis, int16_t raw) {
        if (axis == 0) _accelX = raw;
        else if (axis == 1) _accelY = raw;
        else _accelZ = raw;
        _updated = true;
    }
    void _onGyro(uint8_t axis, int16_t raw) {
        if (axis == 0) _gyroX = raw;
        else if (axis == 1) _gyroY = raw;
        else _gyroZ = raw;
        _updated = true;
    }
    void _onMag(uint8_t axis, int16_t raw) {
        if (axis == 0) _magX = raw;
        else if (axis == 1) _magY = raw;
        else _magZ = raw;
        _updated = true;
    }
    void _onProximity(uint16_t raw) { _proximity = raw; _updated = true; }
    void _onSound(uint16_t raw)     { _sound = raw; _updated = true; }
    void _onSteps(uint16_t raw)     { _steps = raw; _updated = true; }
    void _onGpsLat(int32_t raw)     { _gpsLat = raw; _updated = true; }
    void _onGpsLng(int32_t raw)     { _gpsLng = raw; _updated = true; }
    void _onGpsAlt(uint16_t raw)    { _gpsAlt = raw; _updated = true; }
    void _onGpsSpeed(uint16_t raw)  { _gpsSpeed = raw; _updated = true; }
    void _onColorRGB(uint8_t r, uint8_t g, uint8_t b) {
        _colorR = r; _colorG = g; _colorB = b; _updated = true;
    }
    void _onColorGrid(uint8_t gridSize, uint8_t cellIndex, uint8_t r, uint8_t g, uint8_t b) {
        _gridSize = gridSize;
        if (cellIndex < 25) {
            _gridR[cellIndex] = r; _gridG[cellIndex] = g; _gridB[cellIndex] = b;
        }
        _updated = true;
    }
};

#endif
