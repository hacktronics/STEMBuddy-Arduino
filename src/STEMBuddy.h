/**
 * STEMBuddy.h
 *
 * Main header for the STEM Buddy Arduino library.
 * Include this in your ESP32 sketch to use virtual hardware on the STEM Buddy app.
 *
 * Usage:
 *   #include <STEMBuddy.h>
 *
 *   STEMBuddy buddy;
 *
 *   void setup() {
 *     buddy.begin("My Project");     // Start BLE with device name
 *     buddy.led.on();                // Turn on virtual LED
 *     buddy.lcd.print(0, 0, "Hi!");  // Print on virtual LCD
 *   }
 *
 *   void loop() {
 *     buddy.update();                // Process BLE messages
 *     float temp = buddy.sensor.temperature();  // Read virtual sensor
 *   }
 */

#ifndef STEMBUDDY_H
#define STEMBUDDY_H

#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

#include "STEMBuddyProtocol.h"
#include "STEMBuddyLED.h"
#include "STEMBuddyLCD.h"
#include "STEMBuddyServo.h"
#include "STEMBuddyMotor.h"
#include "STEMBuddyBuzzer.h"
#include "STEMBuddyScope.h"
#include "STEMBuddySensor.h"
#include "STEMBuddyInput.h"
#include "STEMBuddyTTS.h"
#include "STEMBuddyTerminal.h"
#include "STEMBuddyCamera.h"
#include "STEMBuddyNotify.h"
#include "STEMBuddySMS.h"
#include "STEMBuddyDataLog.h"
#include "STEMBuddyPiano.h"
#include "STEMBuddyInternet.h"
#include "STEMBuddyAudio.h"
#include "STEMBuddySpeech.h"
#include "STEMBuddyNFC.h"
#include "STEMBuddySpeaker.h"
#include "STEMBuddyGPIO.h"
#include "STEMBuddySegment7.h"
#include "STEMBuddyBarGraph.h"
#include "STEMBuddyStepper.h"
#include "STEMBuddyMAX7219.h"
#include "STEMBuddyRFID.h"
#include "STEMBuddySwitch.h"

class STEMBuddy : public BLEServerCallbacks, public BLECharacteristicCallbacks {
public:
    // Virtual peripheral instances — use these directly
    STEMBuddyLED      led;
    STEMBuddyLCD      lcd;
    STEMBuddyServo    servo;
    STEMBuddyMotor    motor;
    STEMBuddyBuzzer   buzzer;
    STEMBuddyScope    scope;
    STEMBuddySensor   sensor;
    STEMBuddyInput    input;
    STEMBuddyTTS      tts;
    STEMBuddyTerminal terminal;
    STEMBuddyCamera   camera;
    STEMBuddyNotify   notify;
    STEMBuddySMS      sms;
    STEMBuddyDataLog  dataLog;
    STEMBuddyPiano    piano;
    STEMBuddyInternet internet;
    STEMBuddyAudio    audio;
    STEMBuddySpeech   speech;
    STEMBuddyNFC      nfc;
    STEMBuddySpeaker  speaker;
    STEMBuddyGPIO     gpio;
    STEMBuddySegment7 segment7;
    STEMBuddyBarGraph barGraph;
    STEMBuddyStepper  stepper;
    STEMBuddyMAX7219  matrix;
    STEMBuddyRFID     rfid;
    STEMBuddySwitch   sw;

    STEMBuddy();

    /**
     * Initialize BLE and start advertising.
     * @param deviceName  Name shown in the STEM Buddy app scan list.
     */
    void begin(const char* deviceName = "STEMBuddy");

    /**
     * Call this in loop() to process incoming BLE messages.
     * Must be called regularly for sensor/input data to update.
     */
    void update();

    /**
     * Check if the STEM Buddy app is connected.
     */
    bool isConnected();

    /**
     * Reset all virtual components to their default state.
     */
    void reset();

    /**
     * Send a raw command to the app.
     * Prefer using the component classes (led, lcd, etc.) instead.
     */
    void sendCommand(uint16_t command, const uint8_t* data = nullptr, uint8_t len = 0);

    /**
     * Send a text string in 18-byte BLE chunks.
     * Used internally by components for long text (TTS, SMS, Notify, etc.).
     */
    void sendChunked(uint16_t command, const char* text);

    // BLE callbacks (internal)
    void onConnect(BLEServer* pServer) override;
    void onDisconnect(BLEServer* pServer) override;
    void onWrite(BLECharacteristic* pCharacteristic) override;

private:
    BLEServer*          _server;
    BLECharacteristic*  _txChar;   // ESP32 sends TO app
    BLECharacteristic*  _rxChar;   // ESP32 receives FROM app
    bool                _connected;
    bool                _initialized;

    void _processMessage(const uint8_t* data, size_t len);
};

#endif // STEMBUDDY_H
