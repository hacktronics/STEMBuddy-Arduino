# STEMBuddy BLE Command Protocol Reference

## Overview

STEMBuddy uses a binary BLE protocol for communication between ESP32 and the phone app. The phone acts as a **virtual peripheral** — ESP32 sends commands to render virtual components (LED, LCD, Servo, etc.), and the app sends back virtual sensor/input data.

## Packet Format

```
[CMD_HI] [CMD_LO] [DATA_0] [DATA_1] ... [DATA_N]
```

| Field | Size | Description |
|-------|------|-------------|
| CMD_HI | 1 byte | Always `0xC2` |
| CMD_LO | 1 byte | Command index `0x00`–`0x93` |
| DATA | 0–18 bytes | Variable payload |
| **Total** | **2–20 bytes** | **BLE default MTU safe** |

## BLE Configuration

| Parameter | Value |
|-----------|-------|
| Service UUID | `cde0a001-b5a3-f393-e0a9-e50e24dcca9e` |
| TX Characteristic | `cde1b002-c6b4-0484-f1ba-f61f35eddba0` (ESP32 → App, notify) |
| RX Characteristic | `cde1b002-c6b4-0484-f1ba-f61f35eddbaf` (App → ESP32, write) |

## Data Encoding Conventions

- **16-bit integers**: Big-endian `[hi, lo]`
- **Signed 16-bit**: Two's complement, big-endian
- **32-bit integers**: Big-endian `[b3, b2, b1, b0]`
- **Float values**: Transmitted as scaled integers (e.g., temperature × 100)
- **Text/strings**: Raw ASCII bytes, max 18 per packet. For longer text, send multiple packets — app concatenates until a terminating command.
- **Boolean**: `0` = off/false, `1` = on/true

---

## Command Reference

### System (0xC200–0xC204) — Bidirectional

| # | Hex | Name | Direction | Data | Description |
|---|-----|------|-----------|------|-------------|
| 0 | `C200` | `PING` | ESP32 → App | — | Heartbeat. App responds with PONG. |
| 1 | `C201` | `PONG` | App → ESP32 | — | Response to PING. |
| 2 | `C202` | `RESET` | ESP32 → App | — | Reset all virtual components to defaults. |
| 3 | `C203` | `VERSION` | Bidirectional | `[major, minor]` | Request/respond with protocol version. |
| 4 | `C204` | `REGISTER` | ESP32 → App | `[component_type]` | Register a virtual component. Types: 0x01=LED, 0x02=LCD, 0x03=Servo, 0x04=Motor, 0x05=Buzzer, 0x06=Scope, 0x07=Sensor, 0x08=Button |

### LED (0xC205–0xC20C) — ESP32 → App

| # | Hex | Name | Data | Description |
|---|-----|------|------|-------------|
| 5 | `C205` | `LED_SET_COLOR` | `[R, G, B]` | Set LED color (0–255 each) |
| 6 | `C206` | `LED_SET_BRIGHTNESS` | `[brightness]` | Set brightness (0–255) |
| 7 | `C207` | `LED_ON` | — | Turn LED on |
| 8 | `C208` | `LED_OFF` | — | Turn LED off |
| 9 | `C209` | `LED_STRIP_SET` | `[index, R, G, B]` | Set single NeoPixel color |
| 10 | `C20A` | `LED_STRIP_FILL` | `[R, G, B]` | Fill all strip LEDs with color |
| 11 | `C20B` | `LED_STRIP_COUNT` | `[count]` | Set strip length (1–16) |
| 12 | `C20C` | `LED_MATRIX_SIZE` | `[rows, cols]` | Set NeoPixel matrix dimensions |

### LCD (0xC20D–0xC215) — ESP32 → App

| # | Hex | Name | Data | Description |
|---|-----|------|------|-------------|
| 13 | `C20D` | `LCD_CLEAR` | — | Clear display |
| 14 | `C20E` | `LCD_PRINT` | `[row, col, char0...charN]` | Print text at position. row: 0–1, col: 0–15, max 16 chars |
| 15 | `C20F` | `LCD_SET_CURSOR` | `[row, col]` | Move cursor position |
| 16 | `C210` | `LCD_BACKLIGHT_ON` | — | Enable backlight |
| 17 | `C211` | `LCD_BACKLIGHT_OFF` | — | Disable backlight |
| 18 | `C212` | `LCD_SET_BG_COLOR` | `[R, G, B]` | Set background color |
| 19 | `C213` | `LCD_SCROLL_LEFT` | — | Scroll display left |
| 20 | `C214` | `LCD_SCROLL_RIGHT` | — | Scroll display right |
| 21 | `C215` | `LCD_CREATE_CHAR` | `[slot, row0...row7]` | Create custom character (slot 0–7, 8 rows of 5-bit data) |

### Servo (0xC216–0xC217) — ESP32 → App

| # | Hex | Name | Data | Description |
|---|-----|------|------|-------------|
| 22 | `C216` | `SERVO_SET_ANGLE` | `[angle]` | Set servo angle (0–180) |
| 23 | `C217` | `SERVO_DETACH` | — | Release servo (stop holding position) |

### Motor (0xC218–0xC21A) — ESP32 → App

| # | Hex | Name | Data | Description |
|---|-----|------|------|-------------|
| 24 | `C218` | `MOTOR_SET_SPEED` | `[speed]` | Set DC motor PWM speed (0–255) |
| 25 | `C219` | `MOTOR_SET_DIR` | `[dir]` | Set direction: 0=CW, 1=CCW |
| 26 | `C21A` | `MOTOR_STOP` | — | Stop motor (speed=0) |

### Buzzer (0xC21B–0xC21D) — ESP32 → App

| # | Hex | Name | Data | Description |
|---|-----|------|------|-------------|
| 27 | `C21B` | `BUZZER_TONE` | `[freq_hi, freq_lo, dur_hi, dur_lo]` | Play tone. Frequency in Hz, duration in ms (0=continuous) |
| 28 | `C21C` | `BUZZER_NO_TONE` | — | Stop buzzer |
| 29 | `C21D` | `BUZZER_SET_VOLUME` | `[volume]` | Set volume (0–100) |

### Oscilloscope (0xC21E–0xC221) — ESP32 → App

| # | Hex | Name | Data | Description |
|---|-----|------|------|-------------|
| 30 | `C21E` | `SCOPE_PLOT` | `[value_hi, value_lo]` | Plot single data point (signed 16-bit, -32768 to 32767) |
| 31 | `C21F` | `SCOPE_CLEAR` | — | Clear the trace |
| 32 | `C220` | `SCOPE_SET_RANGE` | `[min_hi, min_lo, max_hi, max_lo]` | Set Y-axis range |
| 33 | `C221` | `SCOPE_SET_LABEL` | `[char0...charN]` | Set channel label (max 16 chars) |

### Virtual Sensors (0xC222–0xC237) — App → ESP32

Phone sensors or generated values sent TO the ESP32.

| # | Hex | Name | Data | Description |
|---|-----|------|------|-------------|
| 34 | `C222` | `SENSOR_TEMPERATURE` | `[hi, lo]` | Temperature × 100 (e.g., 2550 = 25.50°C) |
| 35 | `C223` | `SENSOR_LIGHT` | `[hi, lo]` | Light level in lux |
| 36 | `C224` | `SENSOR_HUMIDITY` | `[hi, lo]` | Humidity × 100 (e.g., 5500 = 55.00%) |
| 37 | `C225` | `SENSOR_PRESSURE` | `[hi, lo]` | Pressure × 10 (e.g., 10130 = 1013.0 hPa) |
| 38 | `C226` | `SENSOR_ACCEL_X` | `[hi, lo]` | Accelerometer X × 100 (signed) |
| 39 | `C227` | `SENSOR_ACCEL_Y` | `[hi, lo]` | Accelerometer Y × 100 (signed) |
| 40 | `C228` | `SENSOR_ACCEL_Z` | `[hi, lo]` | Accelerometer Z × 100 (signed) |
| 41 | `C229` | `SENSOR_GYRO_X` | `[hi, lo]` | Gyroscope X × 100 (signed) |
| 42 | `C22A` | `SENSOR_GYRO_Y` | `[hi, lo]` | Gyroscope Y × 100 (signed) |
| 43 | `C22B` | `SENSOR_GYRO_Z` | `[hi, lo]` | Gyroscope Z × 100 (signed) |
| 44 | `C22C` | `SENSOR_MAG_X` | `[hi, lo]` | Magnetometer X × 10 (signed, µT) |
| 45 | `C22D` | `SENSOR_MAG_Y` | `[hi, lo]` | Magnetometer Y × 10 (signed, µT) |
| 46 | `C22E` | `SENSOR_MAG_Z` | `[hi, lo]` | Magnetometer Z × 10 (signed, µT) |
| 47 | `C22F` | `SENSOR_PROXIMITY` | `[hi, lo]` | Proximity in cm (0=far) |
| 48 | `C230` | `SENSOR_SOUND` | `[hi, lo]` | Sound level × 10 dB (e.g., 650 = 65.0 dB) |
| 49 | `C231` | `SENSOR_STEPS` | `[hi, lo]` | Step count (resets on connect) |
| 50 | `C232` | `SENSOR_GPS_LAT` | `[b3, b2, b1, b0]` | Latitude × 10000 (signed 32-bit) |
| 51 | `C233` | `SENSOR_GPS_LNG` | `[b3, b2, b1, b0]` | Longitude × 10000 (signed 32-bit) |
| 52 | `C234` | `SENSOR_GPS_ALT` | `[hi, lo]` | Altitude in meters (0–65535) |
| 53 | `C235` | `SENSOR_GPS_SPEED` | `[hi, lo]` | Speed in km/h × 10 |
| 54 | `C236` | `SENSOR_COLOR_RGB` | `[R, G, B]` | Detected color (camera) |
| 55 | `C237` | `SENSOR_COLOR_GRID` | `[gridSize, cellIndex, R, G, B]` | Grid cell color |

### Virtual Inputs (0xC238–0xC23C) — App → ESP32

| # | Hex | Name | Data | Description |
|---|-----|------|------|-------------|
| 56 | `C238` | `BUTTON_STATE` | `[button_id, state]` | Button 0–7, state: 0=released, 1=pressed |
| 57 | `C239` | `SLIDER_VALUE` | `[slider_id, value]` | Slider 0–3, value: 0–255 |
| 58 | `C23A` | `TOUCHPAD_XY` | `[x_hi, x_lo, y_hi, y_lo]` | Touch position (0–1000 range) |
| 59 | `C23B` | `DPAD_STATE` | `[direction]` | 0=none, 1=up, 2=right, 3=down, 4=left. Use `STEMBuddyInput::DPAD_UP` etc. |
| 60 | `C23C` | `KEYPAD_STATE` | `[key_char]` | ASCII code of pressed key (0=released). Keys: '0'–'9', '*', '#', 'A'–'D' |

### TTS / Speech (0xC23D–0xC240) — ESP32 → App

| # | Hex | Name | Data | Description |
|---|-----|------|------|-------------|
| 61 | `C23D` | `TTS_SPEAK` | `[char0...charN]` | Speak text (max 18 chars/packet, chunked) |
| 62 | `C23E` | `TTS_STOP` | — | Stop speaking |
| 63 | `C23F` | `TTS_SET_RATE` | `[rate]` | Speech rate 0–200 (100=normal, maps to 0.1–2.0) |
| 64 | `C240` | `TTS_SET_PITCH` | `[pitch]` | Pitch 0–200 (100=normal, maps to 0.1–2.0) |

### Terminal / Serial (0xC241–0xC243) — Bidirectional

| # | Hex | Name | Direction | Data | Description |
|---|-----|------|-----------|------|-------------|
| 65 | `C241` | `TERMINAL_PRINT` | ESP32 → App | `[char0...charN]` | Print to terminal (max 18 chars/packet) |
| 66 | `C242` | `TERMINAL_CLEAR` | ESP32 → App | — | Clear terminal |
| 67 | `C243` | `TERMINAL_INPUT` | App → ESP32 | `[char0...charN]` | User typed text |

### GPIO Monitor (0xC244–0xC245) — ESP32 → App

| # | Hex | Name | Data | Description |
|---|-----|------|------|-------------|
| 68 | `C244` | `GPIO_PIN_STATE` | `[pin, state, mode]` | Pin digital state. mode: 0=input, 1=output |
| 69 | `C245` | `GPIO_ANALOG_VALUE` | `[pin, value_hi, value_lo]` | Pin analog value (12-bit ADC) |

### Home Automation / Switch / Relay (0xC246–0xC248)

| # | Hex | Name | Direction | Data | Description |
|---|-----|------|-----------|------|-------------|
| 70 | `C246` | `SWITCH_SET` | ESP32 → App | `[switch_id, state]` | Set switch 0–7 state (0/1) |
| 71 | `C247` | `SWITCH_TOGGLE` | ESP32 → App | `[switch_id]` | Toggle switch 0–7 |
| 72 | `C248` | `SWITCH_CONTROL` | App → ESP32 | `[switch_id, state]` | User toggled switch |

> **Relay**: The `buddy.relay` class provides a relay-friendly API (`on()`, `off()`, `toggle()`) that uses the same SWITCH_SET/SWITCH_TOGGLE commands under the hood. Default relay ID is 0.

### 7-Segment Display (0xC249–0xC24E) — ESP32 → App

| # | Hex | Name | Data | Description |
|---|-----|------|------|-------------|
| 73 | `C249` | `SEG7_SET_VALUE` | `[d0, d1, d2, d3]` | Set digit values (0–9, 0x0A=minus, 0xFF=blank) |
| 74 | `C24A` | `SEG7_SET_RAW` | `[seg0...seg7]` | Set raw segment bits per digit |
| 75 | `C24B` | `SEG7_SET_DECIMAL` | `[bitmask]` | Which digits show decimal point (bit0=digit0) |
| 76 | `C24C` | `SEG7_SET_COLON` | `[0\|1]` | Colon on/off |
| 77 | `C24D` | `SEG7_SET_COLOR` | `[R, G, B]` | Segment color |
| 78 | `C24E` | `SEG7_CLEAR` | — | Blank all digits |

> **showNumber()**: The C++ library provides `buddy.segment7.showNumber(int)` and `showNumber(float)` convenience methods that automatically decompose numbers into right-aligned digits with leading blanks, negative signs (digit value 0x0A = segment G only), and auto decimal points. Uses `SEG7_SET_VALUE` + `SEG7_SET_DECIMAL` under the hood.

### LED Bar Graph (0xC24F–0xC252) — ESP32 → App

| # | Hex | Name | Data | Description |
|---|-----|------|------|-------------|
| 79 | `C24F` | `BAR_SET` | `[index, value]` | Set single LED (0=off, 1=on) |
| 80 | `C250` | `BAR_SET_ALL` | `[v0...v9]` | Set all 10 LEDs (0 or 1 each) |
| 81 | `C251` | `BAR_CLEAR` | — | Turn all off |
| 82 | `C252` | `BAR_SET_COLOR` | `[colorIndex]` | GREEN=0, RED=1, YELLOW=2, BLUE=3 (use `STEMBuddyBarGraph::Color` enum) |

### Stepper Motor (0xC253–0xC256) — ESP32 → App

| # | Hex | Name | Data | Description |
|---|-----|------|------|-------------|
| 83 | `C253` | `STEPPER_STEP` | `[steps_hi, steps_lo, dir]` | Step N steps. dir: 0=CW, 1=CCW |
| 84 | `C254` | `STEPPER_SET_ANGLE` | `[angle_hi, angle_lo]` | Set absolute angle (0–360) |
| 85 | `C255` | `STEPPER_SET_RPM` | `[rpm]` | Set speed in RPM (0–255) |
| 86 | `C256` | `STEPPER_RESET` | — | Reset angle to 0 |

### MAX7219 Dot Matrix (0xC257–0xC25C) — ESP32 → App

| # | Hex | Name | Data | Description |
|---|-----|------|------|-------------|
| 87 | `C257` | `MAX7219_SET_ROW` | `[module, row, bits]` | Set one row (8 bits) of one module |
| 88 | `C258` | `MAX7219_SET_ALL` | `[b0...b7]` | Set all 8 rows of module 0 |
| 89 | `C259` | `MAX7219_CLEAR` | — | Blank all modules |
| 90 | `C25A` | `MAX7219_SET_INTENSITY` | `[level]` | Brightness 0–15 |
| 91 | `C25B` | `MAX7219_SET_CHAIN` | `[count]` | Number of chained 8x8 modules (1–4) |
| 92 | `C25C` | `MAX7219_SET_COLOR` | `[R, G, B]` | LED color (visual only, default red) |

### RFID / MFRC522 (0xC25D–0xC25E) — App → ESP32

| # | Hex | Name | Data | Description |
|---|-----|------|------|-------------|
| 93 | `C25D` | `RFID_CARD_UID` | `[uid0, uid1, uid2, uid3]` | 4-byte card UID |
| 94 | `C25E` | `RFID_CARD_REMOVED` | — | Card removed from reader |

### Speaker (0xC25F–0xC262) — ESP32 → App

| # | Hex | Name | Data | Description |
|---|-----|------|------|-------------|
| 95 | `C25F` | `SPEAKER_NOTE` | `[freq_hi, freq_lo, dur_hi, dur_lo, waveform]` | Play note with waveform type (`STEMBuddySpeaker::Waveform` enum) |
| 96 | `C260` | `SPEAKER_STOP` | — | Stop playback |
| 97 | `C261` | `SPEAKER_SET_VOLUME` | `[volume]` | Volume 0–100 |
| 98 | `C262` | `SPEAKER_SET_WAVEFORM` | `[type]` | `STEMBuddySpeaker::Waveform` enum: SINE=0, SQUARE=1, SAWTOOTH=2, TRIANGLE=3 |

### Camera (0xC263–0xC266)

| # | Hex | Name | Direction | Data | Description |
|---|-----|------|-----------|------|-------------|
| 99 | `C263` | `CAMERA_CAPTURE` | ESP32 → App | — | Trigger photo capture |
| 100 | `C264` | `CAMERA_SET_QUALITY` | ESP32 → App | `[quality]` | JPEG quality 10–100 |
| 101 | `C265` | `CAMERA_SET_FLASH` | ESP32 → App | `[mode]` | OFF=0, ON=1, AUTO=2 (use `STEMBuddyCamera::OFF/ON/AUTO`) |
| 102 | `C266` | `CAMERA_STATUS` | App → ESP32 | `[status]` | 0=idle, 1=capturing, 2=captured, 3=error |

### Notifications (0xC267–0xC26A) — ESP32 → App

| # | Hex | Name | Data | Description |
|---|-----|------|------|-------------|
| 103 | `C267` | `NOTIFY_TITLE` | `[char0...charN]` | Set notification title (chunked, max 18/packet) |
| 104 | `C268` | `NOTIFY_BODY` | `[char0...charN]` | Set notification body (chunked, max 18/packet) |
| 105 | `C269` | `NOTIFY_SHOW` | `[icon_type]` | Show notification. 0=info, 1=success, 2=warning, 3=error |
| 106 | `C26A` | `NOTIFY_CLEAR` | — | Clear all notifications |

### SMS (0xC26B–0xC26D) — ESP32 → App

| # | Hex | Name | Data | Description |
|---|-----|------|------|-------------|
| 107 | `C26B` | `SMS_NUMBER` | `[char0...charN]` | Set phone number (chunked, max 18/packet) |
| 108 | `C26C` | `SMS_BODY` | `[char0...charN]` | Set message body (chunked, max 18/packet) |
| 109 | `C26D` | `SMS_SEND` | — | Send SMS silently using buffered number + body |

### Piano (0xC26E–0xC273)

| # | Hex | Name | Direction | Data | Description |
|---|-----|------|-----------|------|-------------|
| 110 | `C26E` | `PIANO_NOTE_ON` | ESP32 → App | `[note, velocity]` | Play MIDI note (0–127), velocity (0–127) |
| 111 | `C26F` | `PIANO_NOTE_OFF` | ESP32 → App | `[note]` | Stop MIDI note |
| 112 | `C270` | `PIANO_ALL_OFF` | ESP32 → App | — | Stop all notes |
| 113 | `C271` | `PIANO_SET_INSTRUMENT` | ESP32 → App | `[type]` | Instrument enum: PIANO=0, ORGAN=1, SYNTH=2, MUSIC_BOX=3 |
| 114 | `C272` | `PIANO_KEY_PRESS` | App → ESP32 | `[note, velocity]` | User pressed key |
| 115 | `C273` | `PIANO_KEY_RELEASE` | App → ESP32 | `[note]` | User released key |

### Data Logger (0xC274–0xC279) — ESP32 → App

| # | Hex | Name | Data | Description |
|---|-----|------|------|-------------|
| 116 | `C274` | `DATALOG_FILE_NAME` | `[char0...charN]` | Session/file name (chunked, max 18/packet) |
| 117 | `C275` | `DATALOG_ADD_COLUMN` | `[col_index, char0...charN]` | Define column header |
| 118 | `C276` | `DATALOG_FLOAT` | `[col_index, b3, b2, b1, b0]` | Log float32 (IEEE 754) to column |
| 119 | `C277` | `DATALOG_TEXT` | `[col_index, char0...charN]` | Log text value to column |
| 120 | `C278` | `DATALOG_NEW_ROW` | — | Commit current row, start new one |
| 121 | `C279` | `DATALOG_STOP` | — | End logging session |

### Internet Gateway (0xC27A–0xC282)

ESP32 sends HTTP requests through the phone's internet connection via BLE. No WiFi needed on ESP32.

| # | Hex | Name | Direction | Data | Description |
|---|-----|------|-----------|------|-------------|
| 122 | `C27A` | `INET_REQUEST_START` | ESP32 → App | `[method, requestId]` | Start request. method: 0=GET, 1=POST, 2=PUT, 3=DELETE |
| 123 | `C27B` | `INET_REQUEST_URL` | ESP32 → App | `[char0...charN]` | URL chunk (concatenated) |
| 124 | `C27C` | `INET_REQUEST_HEADER` | ESP32 → App | `[char0...charN]` | Header chunk ("Key: Value\n") |
| 125 | `C27D` | `INET_REQUEST_BODY` | ESP32 → App | `[char0...charN]` | Request body chunk |
| 126 | `C27E` | `INET_REQUEST_SEND` | ESP32 → App | `[requestId]` | Execute the assembled request |
| 127 | `C27F` | `INET_RESPONSE_STATUS` | App → ESP32 | `[requestId, status_hi, status_lo]` | HTTP status code |
| 128 | `C280` | `INET_RESPONSE_BODY` | App → ESP32 | `[char0...charN]` | Response body chunk |
| 129 | `C281` | `INET_RESPONSE_END` | App → ESP32 | `[requestId]` | Response complete |
| 130 | `C282` | `INET_RESPONSE_ERROR` | App → ESP32 | `[requestId, error_code]` | 0=network, 1=timeout, 2=dns, 3=refused |

### Audio Player (0xC283–0xC289) — ESP32 → App

| # | Hex | Name | Data | Description |
|---|-----|------|------|-------------|
| 131 | `C283` | `AUDIO_FILE_NAME` | `[char0...charN]` | Filename (chunked, max 18/packet) |
| 132 | `C284` | `AUDIO_PLAY` | — | Play audio using buffered filename |
| 133 | `C285` | `AUDIO_STOP` | — | Stop playback |
| 134 | `C286` | `AUDIO_PAUSE` | — | Pause playback |
| 135 | `C287` | `AUDIO_RESUME` | — | Resume playback |
| 136 | `C288` | `AUDIO_SET_VOLUME` | `[volume]` | Volume 0–100 |
| 137 | `C289` | `AUDIO_SET_LOOP` | `[0\|1]` | 0=no loop, 1=loop |

### Speech Recognition / STT (0xC28A–0xC28D) — App → ESP32

| # | Hex | Name | Data | Description |
|---|-----|------|------|-------------|
| 138 | `C28A` | `STT_RESULT` | `[char0...charN]` | Recognized text chunk (max 18 chars) |
| 139 | `C28B` | `STT_RESULT_END` | — | End of recognized text |
| 140 | `C28C` | `STT_ERROR` | `[error_code]` | 0=no-speech, 1=no-match, 2=not-allowed, 3=aborted |
| 141 | `C28D` | `STT_STATUS` | `[status]` | 0=stopped, 1=listening, 2=processing |

### NFC (0xC28E–0xC293) — App → ESP32

| # | Hex | Name | Data | Description |
|---|-----|------|------|-------------|
| 142 | `C28E` | `NFC_TAG_UID` | `[uidLen, uid0...uid6]` | Tag UID (up to 7 bytes) |
| 143 | `C28F` | `NFC_TAG_TYPE` | `[type]` | 0=unknown, 1=NfcA, 2=NfcB, 3=NfcF, 4=NfcV, 5=IsoDep, 6=Mifare |
| 144 | `C290` | `NFC_NDEF_TEXT` | `[char0...charN]` | NDEF text record chunk (max 18 chars) |
| 145 | `C291` | `NFC_NDEF_URI` | `[char0...charN]` | NDEF URI record chunk (max 18 chars) |
| 146 | `C292` | `NFC_NDEF_END` | — | End of NDEF records |
| 147 | `C293` | `NFC_TAG_REMOVED` | — | Tag removed / scan session ended |

---

## Summary

| Category | Range | Count | Direction |
|----------|-------|-------|-----------|
| System | C200–C204 | 5 | Bidirectional |
| LED | C205–C20C | 8 | ESP32 → App |
| LCD | C20D–C215 | 9 | ESP32 → App |
| Servo | C216–C217 | 2 | ESP32 → App |
| Motor | C218–C21A | 3 | ESP32 → App |
| Buzzer | C21B–C21D | 3 | ESP32 → App |
| Oscilloscope | C21E–C221 | 4 | ESP32 → App |
| Sensors | C222–C237 | 22 | App → ESP32 |
| Inputs | C238–C23C | 5 | App → ESP32 |
| TTS | C23D–C240 | 4 | ESP32 → App |
| Terminal | C241–C243 | 3 | Bidirectional |
| GPIO | C244–C245 | 2 | ESP32 → App |
| Switch | C246–C248 | 3 | Bidirectional |
| 7-Segment | C249–C24E | 6 | ESP32 → App |
| Bar Graph | C24F–C252 | 4 | ESP32 → App |
| Stepper | C253–C256 | 4 | ESP32 → App |
| MAX7219 | C257–C25C | 6 | ESP32 → App |
| RFID | C25D–C25E | 2 | App → ESP32 |
| Speaker | C25F–C262 | 4 | ESP32 → App |
| Camera | C263–C266 | 4 | Bidirectional |
| Notifications | C267–C26A | 4 | ESP32 → App |
| SMS | C26B–C26D | 3 | ESP32 → App |
| Piano | C26E–C273 | 6 | Bidirectional |
| Data Logger | C274–C279 | 6 | ESP32 → App |
| Internet | C27A–C282 | 9 | Bidirectional |
| Audio | C283–C289 | 7 | ESP32 → App |
| STT | C28A–C28D | 4 | App → ESP32 |
| NFC | C28E–C293 | 6 | App → ESP32 |
| **Total** | **C200–C293** | **148** | |
