/**
 * STEMBuddy.cpp
 *
 * Core BLE communication layer.
 * Manages the BLE server, handles connections, sends/receives packets.
 */

#include "STEMBuddy.h"

STEMBuddy::STEMBuddy()
    : _server(nullptr), _txChar(nullptr), _rxChar(nullptr),
      _bleConnected(false), _appReady(false), _initialized(false) {}

void STEMBuddy::begin(const char* deviceName) {
    if (_initialized) return;

    // Initialize BLE
    BLEDevice::init(deviceName);
    _server = BLEDevice::createServer();
    _server->setCallbacks(this);

    // Create UART service
    BLEService* service = _server->createService(STEMBUDDY_SERVICE_UUID);

    // TX characteristic: ESP32 → App (notify)
    _txChar = service->createCharacteristic(
        STEMBUDDY_TX_CHARACTERISTIC,
        BLECharacteristic::PROPERTY_NOTIFY
    );
    _txChar->addDescriptor(new BLE2902());

    // RX characteristic: App → ESP32 (write)
    _rxChar = service->createCharacteristic(
        STEMBUDDY_RX_CHARACTERISTIC,
        BLECharacteristic::PROPERTY_WRITE | BLECharacteristic::PROPERTY_WRITE_NR
    );
    _rxChar->setCallbacks(this);

    service->start();

    // Start advertising
    BLEAdvertising* advertising = BLEDevice::getAdvertising();
    advertising->addServiceUUID(STEMBUDDY_SERVICE_UUID);
    advertising->setScanResponse(true);
    advertising->setMinPreferred(0x06);
    advertising->setMaxPreferred(0x12);
    BLEDevice::startAdvertising();

    // Wire up component references so they can send commands
    led.begin(this);
    lcd.begin(this);
    servo.begin(this);
    motor.begin(this);
    buzzer.begin(this);
    scope.begin(this);
    sensor.begin(this);
    input.begin(this);
    tts.begin(this);
    camera.begin(this);
    notify.begin(this);
    sms.begin(this);
    dataLog.begin(this);
    piano.begin(this);
    internet.begin(this);
    audio.begin(this);
    speech.begin(this);
    nfc.begin(this);
    speaker.begin(this);
    gpio.begin(this);
    segment7.begin(this);
    barGraph.begin(this);
    stepper.begin(this);
    matrix.begin(this);
    rfid.begin(this);
    sw.begin(this);
    relay.begin(this);
    oled.begin(this);
    sd.begin(this);

    _initialized = true;
}

void STEMBuddy::update() {
    // Nothing to poll — BLE callbacks handle incoming data.
    // This method exists for future use (e.g., periodic sensor requests)
    // and to keep the API consistent.
}

bool STEMBuddy::isConnected() {
    // Only true after app has subscribed to notifications and sent PONG handshake.
    // This prevents commands being sent before the app is ready to receive them.
    return _bleConnected && _appReady;
}

void STEMBuddy::reset() {
    sendCommand(SBCmd::RESET);
}

void STEMBuddy::sendCommand(uint16_t command, const uint8_t* data, uint8_t len) {
    if (!_bleConnected || !_txChar) return;

    uint8_t packet[20]; // BLE default MTU
    packet[0] = (command >> 8) & 0xFF;
    packet[1] = command & 0xFF;

    uint8_t copyLen = (len > 18) ? 18 : len; // Max 18 bytes of data
    if (data && copyLen > 0) {
        memcpy(&packet[2], data, copyLen);
    }

    _txChar->setValue(packet, 2 + copyLen);
    _txChar->notify();
}

void STEMBuddy::sendChunked(uint16_t command, const char* text) {
    if (!text) return;
    size_t len = strlen(text);
    size_t offset = 0;

    while (offset < len) {
        size_t chunkLen = len - offset;
        if (chunkLen > 18) chunkLen = 18;

        sendCommand(command, (const uint8_t*)(text + offset), chunkLen);
        offset += chunkLen;
    }
}

// ─── BLE Callbacks ─────────────────────────────────────────────────

void STEMBuddy::onConnect(BLEServer* pServer) {
    _bleConnected = true;
    _appReady = false;  // Wait for PONG handshake from app
}

void STEMBuddy::onDisconnect(BLEServer* pServer) {
    _bleConnected = false;
    _appReady = false;
    // Restart advertising so the app can reconnect
    BLEDevice::startAdvertising();
}

void STEMBuddy::onWrite(BLECharacteristic* pCharacteristic) {
    String value = pCharacteristic->getValue();
    if (value.length() >= 2) {
        _processMessage((const uint8_t*)value.c_str(), value.length());
    }
}

// ─── Internal ──────────────────────────────────────────────────────

void STEMBuddy::_processMessage(const uint8_t* data, size_t len) {
    uint16_t command = ((uint16_t)(data[0]) << 8) | data[1];
    const uint8_t* payload = data + 2;
    uint8_t payloadLen = len - 2;

    switch (command) {
        // System
        case SBCmd::PING:
            sendCommand(SBCmd::PONG);
            break;
        case SBCmd::PONG:
            // App has subscribed to notifications and is ready to receive data.
            // This completes the handshake — isConnected() now returns true.
            _appReady = true;
            break;

        // Virtual sensor data from app
        case SBCmd::SENSOR_TEMPERATURE:
            if (payloadLen >= 2) sensor._onTemperature(((int16_t)payload[0] << 8) | payload[1]);
            break;
        case SBCmd::SENSOR_LIGHT:
            if (payloadLen >= 2) sensor._onLight((payload[0] << 8) | payload[1]);
            break;
        case SBCmd::SENSOR_HUMIDITY:
            if (payloadLen >= 2) sensor._onHumidity(((int16_t)payload[0] << 8) | payload[1]);
            break;
        case SBCmd::SENSOR_PRESSURE:
            if (payloadLen >= 2) sensor._onPressure((payload[0] << 8) | payload[1]);
            break;
        case SBCmd::SENSOR_ACCEL_X:
            if (payloadLen >= 2) sensor._onAccel(0, ((int16_t)payload[0] << 8) | payload[1]);
            break;
        case SBCmd::SENSOR_ACCEL_Y:
            if (payloadLen >= 2) sensor._onAccel(1, ((int16_t)payload[0] << 8) | payload[1]);
            break;
        case SBCmd::SENSOR_ACCEL_Z:
            if (payloadLen >= 2) sensor._onAccel(2, ((int16_t)payload[0] << 8) | payload[1]);
            break;
        case SBCmd::SENSOR_GYRO_X:
            if (payloadLen >= 2) sensor._onGyro(0, ((int16_t)payload[0] << 8) | payload[1]);
            break;
        case SBCmd::SENSOR_GYRO_Y:
            if (payloadLen >= 2) sensor._onGyro(1, ((int16_t)payload[0] << 8) | payload[1]);
            break;
        case SBCmd::SENSOR_GYRO_Z:
            if (payloadLen >= 2) sensor._onGyro(2, ((int16_t)payload[0] << 8) | payload[1]);
            break;
        case SBCmd::SENSOR_MAG_X:
            if (payloadLen >= 2) sensor._onMag(0, ((int16_t)payload[0] << 8) | payload[1]);
            break;
        case SBCmd::SENSOR_MAG_Y:
            if (payloadLen >= 2) sensor._onMag(1, ((int16_t)payload[0] << 8) | payload[1]);
            break;
        case SBCmd::SENSOR_MAG_Z:
            if (payloadLen >= 2) sensor._onMag(2, ((int16_t)payload[0] << 8) | payload[1]);
            break;
        case SBCmd::SENSOR_PROXIMITY:
            if (payloadLen >= 2) sensor._onProximity((payload[0] << 8) | payload[1]);
            break;
        case SBCmd::SENSOR_SOUND:
            if (payloadLen >= 2) sensor._onSound((payload[0] << 8) | payload[1]);
            break;
        case SBCmd::SENSOR_STEPS:
            if (payloadLen >= 2) sensor._onSteps((payload[0] << 8) | payload[1]);
            break;
        case SBCmd::SENSOR_GPS_LAT:
            if (payloadLen >= 4) sensor._onGpsLat(((int32_t)payload[0] << 24) | ((int32_t)payload[1] << 16) | ((int32_t)payload[2] << 8) | payload[3]);
            break;
        case SBCmd::SENSOR_GPS_LNG:
            if (payloadLen >= 4) sensor._onGpsLng(((int32_t)payload[0] << 24) | ((int32_t)payload[1] << 16) | ((int32_t)payload[2] << 8) | payload[3]);
            break;
        case SBCmd::SENSOR_GPS_ALT:
            if (payloadLen >= 2) sensor._onGpsAlt((payload[0] << 8) | payload[1]);
            break;
        case SBCmd::SENSOR_GPS_SPEED:
            if (payloadLen >= 2) sensor._onGpsSpeed((payload[0] << 8) | payload[1]);
            break;
        case SBCmd::SENSOR_COLOR_RGB:
            if (payloadLen >= 3) sensor._onColorRGB(payload[0], payload[1], payload[2]);
            break;
        case SBCmd::SENSOR_COLOR_GRID:
            if (payloadLen >= 5) sensor._onColorGrid(payload[0], payload[1], payload[2], payload[3], payload[4]);
            break;

        // Virtual button/input from app
        case SBCmd::BUTTON_STATE:
            if (payloadLen >= 2) input._onButton(payload[0], payload[1]);
            break;
        case SBCmd::SLIDER_VALUE:
            if (payloadLen >= 2) input._onSlider(payload[0], payload[1]);
            break;
        case SBCmd::DPAD_STATE:
            if (payloadLen >= 1) input._onDpad(payload[0]);
            break;
        case SBCmd::TOUCHPAD_XY:
            if (payloadLen >= 4) {
                uint16_t x = (payload[0] << 8) | payload[1];
                uint16_t y = (payload[2] << 8) | payload[3];
                input._onTouchpad(x, y);
            }
            break;
        case SBCmd::KEYPAD_STATE:
            if (payloadLen >= 1) input._onKeypad(payload[0]);
            break;

        // Camera status from app
        case SBCmd::CAMERA_STATUS:
            if (payloadLen >= 1) camera._setStatus(payload[0]);
            break;

        // Piano key events from app
        case SBCmd::PIANO_KEY_PRESS:
            if (payloadLen >= 2) piano._onKeyPress(payload[0], payload[1]);
            break;
        case SBCmd::PIANO_KEY_RELEASE:
            if (payloadLen >= 1) piano._onKeyRelease(payload[0]);
            break;

        // Terminal input from app
        case SBCmd::TERMINAL_INPUT:
            if (payloadLen >= 1) terminal._onInput(payload, payloadLen);
            break;

        // Switch control from app
        case SBCmd::SWITCH_CONTROL:
            if (payloadLen >= 2) input._onSwitch(payload[0], payload[1]);
            break;

        // Internet response from app
        case SBCmd::INET_RESPONSE_STATUS:
            if (payloadLen >= 3) internet._onResponseStatus(payload, payloadLen);
            break;
        case SBCmd::INET_RESPONSE_BODY:
            if (payloadLen >= 1) internet._onResponseBody(payload, payloadLen);
            break;
        case SBCmd::INET_RESPONSE_END:
            if (payloadLen >= 1) internet._onResponseEnd(payload, payloadLen);
            break;
        case SBCmd::INET_RESPONSE_ERROR:
            if (payloadLen >= 2) internet._onResponseError(payload, payloadLen);
            break;

        // Speech recognition results from app
        case SBCmd::STT_RESULT:
            if (payloadLen >= 1) speech._onResultChunk(payload, payloadLen);
            break;
        case SBCmd::STT_RESULT_END:
            speech._onResultEnd();
            break;
        case SBCmd::STT_ERROR:
            if (payloadLen >= 1) speech._onError(payload[0]);
            break;
        case SBCmd::STT_STATUS:
            if (payloadLen >= 1) speech._onStatus(payload[0]);
            break;

        // NFC tag data from app
        case SBCmd::NFC_TAG_UID:
            if (payloadLen >= 1) nfc._onTagUID(payload, payloadLen);
            break;
        case SBCmd::NFC_TAG_TYPE:
            if (payloadLen >= 1) nfc._onTagType(payload[0]);
            break;
        case SBCmd::NFC_NDEF_TEXT:
            if (payloadLen >= 1) nfc._onNdefText(payload, payloadLen);
            break;
        case SBCmd::NFC_NDEF_URI:
            if (payloadLen >= 1) nfc._onNdefUri(payload, payloadLen);
            break;
        case SBCmd::NFC_NDEF_END:
            nfc._onNdefEnd();
            break;
        case SBCmd::NFC_TAG_REMOVED:
            nfc._onTagRemoved();
            break;

        // RFID card data from app
        case SBCmd::RFID_CARD_UID:
            if (payloadLen >= 4) rfid._onCardUID(payload, payloadLen);
            break;
        case SBCmd::RFID_CARD_REMOVED:
            rfid._onCardRemoved();
            break;

        // MicroSD file data from app (responses to readFile/exists/listFiles)
        case SBCmd::SD_FILE_DATA:
            if (payloadLen >= 1) sd._onFileData(payload, payloadLen);
            break;
        case SBCmd::SD_FILE_DATA_END:
            sd._onFileDataEnd();
            break;
        case SBCmd::SD_FILE_STATUS:
            if (payloadLen >= 4) sd._onFileStatus(payload, payloadLen);
            break;
        case SBCmd::SD_FILE_LIST_ENTRY:
            if (payloadLen >= 1) sd._onListEntry(payload, payloadLen);
            break;
        case SBCmd::SD_FILE_LIST_END:
            sd._onListEnd();
            break;
    }
}
