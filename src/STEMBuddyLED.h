/**
 * STEMBuddyLED.h
 *
 * Virtual LED / RGB LED Strip controlled from ESP32.
 * Commands are sent over BLE to the STEM Buddy app which renders the LED.
 *
 * Usage:
 *   buddy.led.setColor(255, 0, 0);   // Red
 *   buddy.led.setBrightness(128);     // 50%
 *   buddy.led.on();
 *   buddy.led.off();
 *
 *   // LED Strip mode
 *   buddy.led.setStripCount(8);
 *   buddy.led.setStripPixel(0, 0, 255, 0);  // Pixel 0 = green
 *   buddy.led.fillStrip(0, 0, 255);          // All blue
 */

#ifndef STEMBUDDY_LED_H
#define STEMBUDDY_LED_H

#include <Arduino.h>

class STEMBuddy; // forward declaration

class STEMBuddyLED {
public:
    STEMBuddyLED() : _parent(nullptr) {}
    void begin(STEMBuddy* parent) { _parent = parent; }

    /** Turn the virtual LED on. */
    void on();

    /** Turn the virtual LED off. */
    void off();

    /** Set RGB color (0-255 each). */
    void setColor(uint8_t r, uint8_t g, uint8_t b);

    /** Set brightness (0-255). */
    void setBrightness(uint8_t brightness);

    // ─── LED Strip ─────────────────────────────────────────
    /** Set number of LEDs in strip mode (1-16). */
    void setStripCount(uint8_t count);

    /** Set a single pixel color by index. */
    void setStripPixel(uint8_t index, uint8_t r, uint8_t g, uint8_t b);

    /** Fill entire strip with one color. */
    void fillStrip(uint8_t r, uint8_t g, uint8_t b);

private:
    STEMBuddy* _parent;
};

#endif
