/**
 * STEMBuddyCamera.h
 *
 * Virtual Camera — trigger the phone's camera from ESP32.
 * The STEM Buddy app captures a photo when commanded and reports status.
 *
 * Usage:
 *   buddy.camera.capture();        // Trigger photo capture
 *   buddy.camera.setQuality(90);   // JPEG quality 10-100
 *   buddy.camera.setFlash(0);      // 0=off, 1=on, 2=auto
 *
 *   // Check status (updated by app via BLE)
 *   uint8_t s = buddy.camera.status();
 *   // 0=idle, 1=capturing, 2=captured, 3=error
 */

#ifndef STEMBUDDY_CAMERA_H
#define STEMBUDDY_CAMERA_H

#include <Arduino.h>

class STEMBuddy;

class STEMBuddyCamera {
    friend class STEMBuddy;

public:
    STEMBuddyCamera() : _parent(nullptr), _status(0) {}
    void begin(STEMBuddy* parent) { _parent = parent; }

    /** Trigger a photo capture on the phone. */
    void capture();

    /** Set JPEG quality (10-100). Default: 90. */
    void setQuality(uint8_t quality);

    /**
     * Set flash mode.
     * @param mode  0=off, 1=on, 2=auto
     */
    void setFlash(uint8_t mode);

    /**
     * Get the current camera status.
     * @return 0=idle, 1=capturing, 2=captured, 3=error
     */
    uint8_t status() const { return _status; }

private:
    STEMBuddy* _parent;
    uint8_t _status;

    void _setStatus(uint8_t s) { _status = s; }
};

#endif
