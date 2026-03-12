/**
 * STEMBuddyProtocol.h
 *
 * STEM Buddy BLE Communication Protocol
 * Shared command definitions between ESP32 firmware and the STEM Buddy app.
 *
 * Packet Format:
 *   [CMD_HI] [CMD_LO] [DATA_0] [DATA_1] ... [DATA_N]
 *
 * - Command: 16-bit ID (2 bytes, big-endian)
 * - Data: variable length payload (0-18 bytes, BLE MTU safe)
 * - Max packet size: 20 bytes (BLE default MTU)
 *
 * All commands are numbered linearly starting from 0xC200.
 *
 * Direction:
 *   ESP32 → App:  Virtual output commands (LED, LCD, Servo, Motor, Buzzer, Scope)
 *   App → ESP32:  Virtual input data (Buttons, Sensors, Sliders, Touchpad)
 */

#ifndef STEMBUDDY_PROTOCOL_H
#define STEMBUDDY_PROTOCOL_H

#include <stdint.h>

// ─── BLE Service & Characteristics ─────────────────────────────────
#define STEMBUDDY_SERVICE_UUID          "cde0a001-b5a3-f393-e0a9-e50e24dcca9e"
#define STEMBUDDY_TX_CHARACTERISTIC     "cde1b002-c6b4-0484-f1ba-f61f35eddba0"  // ESP32 writes → App reads
#define STEMBUDDY_RX_CHARACTERISTIC     "cde1b002-c6b4-0484-f1ba-f61f35eddbaf"  // App writes → ESP32 reads

namespace SBCmd {

// ─── System Commands ────────────────────────────────────────────────
constexpr uint16_t PING             = 0xC200;  // Heartbeat: no data. App responds with PONG.
constexpr uint16_t PONG             = 0xC201;  // Response to PING.
constexpr uint16_t RESET            = 0xC202;  // Reset all virtual components to defaults.
constexpr uint16_t VERSION          = 0xC203;  // Request protocol version. Response: [major] [minor]
constexpr uint16_t REGISTER         = 0xC204;  // Register component. Data: [component_type]

// ─── LED Commands ─ ESP32 → App ────────────────────────────────────
constexpr uint16_t LED_SET_COLOR    = 0xC205;  // Data: [R] [G] [B]
constexpr uint16_t LED_SET_BRIGHTNESS = 0xC206; // Data: [brightness 0-255]
constexpr uint16_t LED_ON           = 0xC207;  // No data
constexpr uint16_t LED_OFF          = 0xC208;  // No data
constexpr uint16_t LED_STRIP_SET    = 0xC209;  // Data: [index] [R] [G] [B]
constexpr uint16_t LED_STRIP_FILL   = 0xC20A;  // Data: [R] [G] [B] — fill all LEDs
constexpr uint16_t LED_STRIP_COUNT  = 0xC20B;  // Data: [count] — set strip length (1-16)
constexpr uint16_t LED_MATRIX_SIZE  = 0xC20C;  // Data: [rows] [cols] — set matrix dimensions

// ─── LCD Commands ─ ESP32 → App ────────────────────────────────────
constexpr uint16_t LCD_CLEAR        = 0xC20D;  // Clear display. No data.
constexpr uint16_t LCD_PRINT        = 0xC20E;  // Data: [row] [col] [char0] [char1] ... [charN]
constexpr uint16_t LCD_SET_CURSOR   = 0xC20F;  // Data: [row] [col]
constexpr uint16_t LCD_BACKLIGHT_ON = 0xC210;  // No data
constexpr uint16_t LCD_BACKLIGHT_OFF = 0xC211; // No data
constexpr uint16_t LCD_SET_BG_COLOR = 0xC212;  // Data: [R] [G] [B]
constexpr uint16_t LCD_SCROLL_LEFT  = 0xC213;  // No data
constexpr uint16_t LCD_SCROLL_RIGHT = 0xC214;  // No data
constexpr uint16_t LCD_CREATE_CHAR  = 0xC215;  // Data: [slot 0-7] [row0] ... [row7] (custom char)

// ─── Servo Commands ─ ESP32 → App ──────────────────────────────────
constexpr uint16_t SERVO_SET_ANGLE  = 0xC216;  // Data: [angle 0-180]
constexpr uint16_t SERVO_DETACH     = 0xC217;  // No data — stop holding position

// ─── Motor Commands ─ ESP32 → App ──────────────────────────────────
constexpr uint16_t MOTOR_SET_SPEED  = 0xC218;  // Data: [speed 0-255] (PWM value)
constexpr uint16_t MOTOR_SET_DIR    = 0xC219;  // Data: [direction] 0=CW, 1=CCW
constexpr uint16_t MOTOR_STOP       = 0xC21A;  // No data — speed=0

// ─── Buzzer Commands ─ ESP32 → App ─────────────────────────────────
constexpr uint16_t BUZZER_TONE      = 0xC21B;  // Data: [freq_hi] [freq_lo] [duration_hi] [duration_lo]
constexpr uint16_t BUZZER_NO_TONE   = 0xC21C;  // No data — stop sound
constexpr uint16_t BUZZER_SET_VOLUME = 0xC21D; // Data: [volume 0-100]

// ─── Oscilloscope Commands ─ ESP32 → App ───────────────────────────
constexpr uint16_t SCOPE_PLOT       = 0xC21E;  // Data: [value_hi] [value_lo] — signed 16-bit
constexpr uint16_t SCOPE_CLEAR      = 0xC21F;  // Clear the trace. No data.
constexpr uint16_t SCOPE_SET_RANGE  = 0xC220;  // Data: [min_hi] [min_lo] [max_hi] [max_lo]
constexpr uint16_t SCOPE_SET_LABEL  = 0xC221;  // Data: [char0] ... [charN] — channel label (max 16)

// ─── Virtual Sensor Data ─ App → ESP32 ─────────────────────────────
constexpr uint16_t SENSOR_TEMPERATURE = 0xC222; // Data: [value_hi] [value_lo] (x100)
constexpr uint16_t SENSOR_LIGHT     = 0xC223;  // Data: [value_hi] [value_lo] (lux)
constexpr uint16_t SENSOR_HUMIDITY  = 0xC224;  // Data: [value_hi] [value_lo] (x100)
constexpr uint16_t SENSOR_PRESSURE  = 0xC225;  // Data: [value_hi] [value_lo] (x10)
constexpr uint16_t SENSOR_ACCEL_X   = 0xC226;  // Data: [value_hi] [value_lo] (x100, signed)
constexpr uint16_t SENSOR_ACCEL_Y   = 0xC227;  // Data: [value_hi] [value_lo] (x100, signed)
constexpr uint16_t SENSOR_ACCEL_Z   = 0xC228;  // Data: [value_hi] [value_lo] (x100, signed)
constexpr uint16_t SENSOR_GYRO_X    = 0xC229;  // Data: [value_hi] [value_lo] (x100, signed)
constexpr uint16_t SENSOR_GYRO_Y    = 0xC22A;  // Data: [value_hi] [value_lo] (x100, signed)
constexpr uint16_t SENSOR_GYRO_Z    = 0xC22B;  // Data: [value_hi] [value_lo] (x100, signed)
constexpr uint16_t SENSOR_MAG_X     = 0xC22C;  // Data: [value_hi] [value_lo] (x10, signed, µT)
constexpr uint16_t SENSOR_MAG_Y     = 0xC22D;  // Data: [value_hi] [value_lo] (x10, signed, µT)
constexpr uint16_t SENSOR_MAG_Z     = 0xC22E;  // Data: [value_hi] [value_lo] (x10, signed, µT)
constexpr uint16_t SENSOR_PROXIMITY = 0xC22F;  // Data: [value_hi] [value_lo] (cm)
constexpr uint16_t SENSOR_SOUND     = 0xC230;  // Data: [value_hi] [value_lo] (dB x10)
constexpr uint16_t SENSOR_STEPS     = 0xC231;  // Data: [value_hi] [value_lo] (step count)
constexpr uint16_t SENSOR_GPS_LAT   = 0xC232;  // Data: [b3, b2, b1, b0] — latitude × 10000 as signed 32-bit
constexpr uint16_t SENSOR_GPS_LNG   = 0xC233;  // Data: [b3, b2, b1, b0] — longitude × 10000 as signed 32-bit
constexpr uint16_t SENSOR_GPS_ALT   = 0xC234;  // Data: [hi, lo] — altitude in meters
constexpr uint16_t SENSOR_GPS_SPEED = 0xC235;  // Data: [hi, lo] — speed in km/h × 10
constexpr uint16_t SENSOR_COLOR_RGB = 0xC236;  // Data: [R, G, B] — camera color detection
constexpr uint16_t SENSOR_COLOR_GRID = 0xC237; // Data: [gridSize, cellIndex, R, G, B]

// ─── Virtual Button/Input ─ App → ESP32 ────────────────────────────
constexpr uint16_t BUTTON_STATE     = 0xC238;  // Data: [button_id 0-7] [state] 0=released, 1=pressed
constexpr uint16_t SLIDER_VALUE     = 0xC239;  // Data: [slider_id 0-3] [value 0-255]
constexpr uint16_t TOUCHPAD_XY      = 0xC23A;  // Data: [x_hi] [x_lo] [y_hi] [y_lo] (0-1000)
constexpr uint16_t DPAD_STATE       = 0xC23B;  // Data: [direction] 0=none,1=up,2=right,3=down,4=left
constexpr uint16_t KEYPAD_STATE     = 0xC23C;  // Data: [key_char] ASCII code (0=released)

// ─── TTS / Speech ─ ESP32 → App ───────────────────────────────────
constexpr uint16_t TTS_SPEAK        = 0xC23D;  // Data: [char0] ... [charN] (max 18 chars)
constexpr uint16_t TTS_STOP         = 0xC23E;  // No data
constexpr uint16_t TTS_SET_RATE     = 0xC23F;  // Data: [rate] 0-200 (100 = normal)
constexpr uint16_t TTS_SET_PITCH    = 0xC240;  // Data: [pitch] 0-200 (100 = normal)

// ─── Terminal / Serial ─ Bidirectional ─────────────────────────────
constexpr uint16_t TERMINAL_PRINT   = 0xC241;  // ESP32 → App: [char0] ... [charN] (max 18)
constexpr uint16_t TERMINAL_CLEAR   = 0xC242;  // ESP32 → App: Clear terminal. No data.
constexpr uint16_t TERMINAL_INPUT   = 0xC243;  // App → ESP32: [char0] ... [charN]

// ─── GPIO Monitor ─ ESP32 → App ───────────────────────────────────
constexpr uint16_t GPIO_PIN_STATE   = 0xC244;  // Data: [pin_number] [state 0/1] [mode]
constexpr uint16_t GPIO_ANALOG_VALUE = 0xC245; // Data: [pin_number] [value_hi] [value_lo]

// ─── Home Automation / Switch ──────────────────────────────────────
constexpr uint16_t SWITCH_SET       = 0xC246;  // ESP32 → App: [switch_id 0-7] [state 0/1]
constexpr uint16_t SWITCH_TOGGLE    = 0xC247;  // ESP32 → App: [switch_id 0-7]
constexpr uint16_t SWITCH_CONTROL   = 0xC248;  // App → ESP32: [switch_id 0-7] [state 0/1]

// ─── 7-Segment Display ─ ESP32 → App ──────────────────────────────
constexpr uint16_t SEG7_SET_VALUE   = 0xC249;  // Data: [d0, d1, d2, d3] (0-9, 0x0A=minus, 0xFF=blank)
constexpr uint16_t SEG7_SET_RAW     = 0xC24A;  // Data: [seg0..seg7] raw segment bits
constexpr uint16_t SEG7_SET_DECIMAL = 0xC24B;  // Data: [bitmask]
constexpr uint16_t SEG7_SET_COLON   = 0xC24C;  // Data: [0|1]
constexpr uint16_t SEG7_SET_COLOR   = 0xC24D;  // Data: [R, G, B]
constexpr uint16_t SEG7_CLEAR       = 0xC24E;  // No data

// ─── LED Bar Graph ─ ESP32 → App ──────────────────────────────────
constexpr uint16_t BAR_SET          = 0xC24F;  // Data: [index, value]
constexpr uint16_t BAR_SET_ALL      = 0xC250;  // Data: [v0..v9]
constexpr uint16_t BAR_CLEAR        = 0xC251;  // No data
constexpr uint16_t BAR_SET_COLOR    = 0xC252;  // Data: [colorIndex]

// ─── Stepper Motor ─ ESP32 → App ──────────────────────────────────
constexpr uint16_t STEPPER_STEP     = 0xC253;  // Data: [steps_hi, steps_lo, dir]
constexpr uint16_t STEPPER_SET_ANGLE = 0xC254; // Data: [angle_hi, angle_lo]
constexpr uint16_t STEPPER_SET_RPM  = 0xC255;  // Data: [rpm] 0-255
constexpr uint16_t STEPPER_RESET    = 0xC256;  // No data

// ─── MAX7219 Dot Matrix ─ ESP32 → App ─────────────────────────────
constexpr uint16_t MAX7219_SET_ROW  = 0xC257;  // Data: [module, row, bits]
constexpr uint16_t MAX7219_SET_ALL  = 0xC258;  // Data: [b0..b7]
constexpr uint16_t MAX7219_CLEAR    = 0xC259;  // No data
constexpr uint16_t MAX7219_SET_INTENSITY = 0xC25A; // Data: [level] 0-15
constexpr uint16_t MAX7219_SET_CHAIN = 0xC25B; // Data: [count] 1-4
constexpr uint16_t MAX7219_SET_COLOR = 0xC25C; // Data: [R, G, B]

// ─── RFID / MFRC522 ─ App → ESP32 ─────────────────────────────────
constexpr uint16_t RFID_CARD_UID    = 0xC25D;  // Data: [uid0, uid1, uid2, uid3]
constexpr uint16_t RFID_CARD_REMOVED = 0xC25E; // No data

// ─── Speaker ─ ESP32 → App ────────────────────────────────────────
constexpr uint16_t SPEAKER_NOTE     = 0xC25F;  // Data: [freq_hi, freq_lo, dur_hi, dur_lo, waveform]
constexpr uint16_t SPEAKER_STOP     = 0xC260;  // No data
constexpr uint16_t SPEAKER_SET_VOLUME = 0xC261; // Data: [volume] 0-100
constexpr uint16_t SPEAKER_SET_WAVEFORM = 0xC262; // Data: [type] 0=sine, 1=square, 2=sawtooth, 3=triangle

// ─── Camera ─ ESP32 → App / App → ESP32 ───────────────────────────
constexpr uint16_t CAMERA_CAPTURE   = 0xC263;  // ESP32 → App: trigger capture
constexpr uint16_t CAMERA_SET_QUALITY = 0xC264; // ESP32 → App: [quality 10-100]
constexpr uint16_t CAMERA_SET_FLASH = 0xC265;  // ESP32 → App: [mode] OFF=0, ON=1, AUTO=2
constexpr uint16_t CAMERA_STATUS    = 0xC266;  // App → ESP32: [status]

// ─── Notifications ─ ESP32 → App ──────────────────────────────────
constexpr uint16_t NOTIFY_TITLE     = 0xC267;  // Data: [char0...charN] (chunked, max 18)
constexpr uint16_t NOTIFY_BODY      = 0xC268;  // Data: [char0...charN] (chunked, max 18)
constexpr uint16_t NOTIFY_SHOW      = 0xC269;  // Data: [icon_type]
constexpr uint16_t NOTIFY_CLEAR     = 0xC26A;  // No data

// ─── SMS ─ ESP32 → App ────────────────────────────────────────────
constexpr uint16_t SMS_NUMBER       = 0xC26B;  // Data: [char0...charN] (chunked, max 18)
constexpr uint16_t SMS_BODY         = 0xC26C;  // Data: [char0...charN] (chunked, max 18)
constexpr uint16_t SMS_SEND         = 0xC26D;  // No data

// ─── Piano ─ ESP32 → App / App → ESP32 ────────────────────────────
constexpr uint16_t PIANO_NOTE_ON    = 0xC26E;  // ESP32 → App: [note, velocity]
constexpr uint16_t PIANO_NOTE_OFF   = 0xC26F;  // ESP32 → App: [note]
constexpr uint16_t PIANO_ALL_OFF    = 0xC270;  // ESP32 → App: no data
constexpr uint16_t PIANO_SET_INSTRUMENT = 0xC271; // ESP32 → App: [type]
constexpr uint16_t PIANO_KEY_PRESS  = 0xC272;  // App → ESP32: [note, velocity]
constexpr uint16_t PIANO_KEY_RELEASE = 0xC273; // App → ESP32: [note]

// ─── Data Logger ─ ESP32 → App ────────────────────────────────────
constexpr uint16_t DATALOG_FILE_NAME = 0xC274; // Data: [char0...charN] (chunked, max 18)
constexpr uint16_t DATALOG_ADD_COLUMN = 0xC275; // Data: [col_index, char0...charN]
constexpr uint16_t DATALOG_FLOAT    = 0xC276;  // Data: [col_index, b3, b2, b1, b0] IEEE 754
constexpr uint16_t DATALOG_TEXT     = 0xC277;  // Data: [col_index, char0...charN]
constexpr uint16_t DATALOG_NEW_ROW  = 0xC278;  // No data
constexpr uint16_t DATALOG_STOP     = 0xC279;  // No data

// ─── Internet Gateway ─ ESP32 → App / App → ESP32 ─────────────────
constexpr uint16_t INET_REQUEST_START = 0xC27A; // ESP32 → App: [method, requestId]
constexpr uint16_t INET_REQUEST_URL = 0xC27B;  // ESP32 → App: [char0...charN] URL chunk
constexpr uint16_t INET_REQUEST_HEADER = 0xC27C; // ESP32 → App: [char0...charN] header chunk
constexpr uint16_t INET_REQUEST_BODY = 0xC27D; // ESP32 → App: [char0...charN] body chunk
constexpr uint16_t INET_REQUEST_SEND = 0xC27E; // ESP32 → App: [requestId]
constexpr uint16_t INET_RESPONSE_STATUS = 0xC27F; // App → ESP32: [requestId, status_hi, status_lo]
constexpr uint16_t INET_RESPONSE_BODY = 0xC280; // App → ESP32: [char0...charN]
constexpr uint16_t INET_RESPONSE_END = 0xC281; // App → ESP32: [requestId]
constexpr uint16_t INET_RESPONSE_ERROR = 0xC282; // App → ESP32: [requestId, error_code]

// ─── Audio Player ─ ESP32 → App ───────────────────────────────────
constexpr uint16_t AUDIO_FILE_NAME  = 0xC283;  // Data: [char0...charN] filename (chunked, max 18)
constexpr uint16_t AUDIO_PLAY       = 0xC284;  // No data
constexpr uint16_t AUDIO_STOP       = 0xC285;  // No data
constexpr uint16_t AUDIO_PAUSE      = 0xC286;  // No data
constexpr uint16_t AUDIO_RESUME     = 0xC287;  // No data
constexpr uint16_t AUDIO_SET_VOLUME = 0xC288;  // Data: [volume 0-100]
constexpr uint16_t AUDIO_SET_LOOP   = 0xC289;  // Data: [0|1]

// ─── Speech Recognition / STT ─ App → ESP32 ───────────────────────
constexpr uint16_t STT_RESULT       = 0xC28A;  // Data: [char0...charN] recognized text chunk
constexpr uint16_t STT_RESULT_END   = 0xC28B;  // No data
constexpr uint16_t STT_ERROR        = 0xC28C;  // Data: [error_code]
constexpr uint16_t STT_STATUS       = 0xC28D;  // Data: [status]

// ─── NFC ─ App → ESP32 ────────────────────────────────────────────
constexpr uint16_t NFC_TAG_UID      = 0xC28E;  // Data: [uidLen, uid0..uid6]
constexpr uint16_t NFC_TAG_TYPE     = 0xC28F;  // Data: [type]
constexpr uint16_t NFC_NDEF_TEXT    = 0xC290;  // Data: [char0..charN]
constexpr uint16_t NFC_NDEF_URI     = 0xC291;  // Data: [char0..charN]
constexpr uint16_t NFC_NDEF_END     = 0xC292;  // No data
constexpr uint16_t NFC_TAG_REMOVED  = 0xC293;  // No data

// ─── SSD1306 OLED ─ ESP32 → App ──────────────────────────────────
constexpr uint16_t OLED_CLEAR       = 0xC294;  // No data — clear framebuffer
constexpr uint16_t OLED_PRINT       = 0xC295;  // Data: [char0...charN] text at cursor (chunked, max 18)
constexpr uint16_t OLED_SET_CURSOR  = 0xC296;  // Data: [x, y] pixel position
constexpr uint16_t OLED_SET_TEXT_SIZE = 0xC297; // Data: [size] 1-4
constexpr uint16_t OLED_DRAW_PIXEL  = 0xC298;  // Data: [x, y, color] color: 0=off, 1=on
constexpr uint16_t OLED_DRAW_LINE   = 0xC299;  // Data: [x0, y0, x1, y1]
constexpr uint16_t OLED_DRAW_RECT   = 0xC29A;  // Data: [x, y, w, h, fill] fill: 0=outline, 1=filled
constexpr uint16_t OLED_DRAW_CIRCLE = 0xC29B;  // Data: [cx, cy, r, fill] fill: 0=outline, 1=filled
constexpr uint16_t OLED_INVERT      = 0xC29C;  // Data: [0|1] invert display
constexpr uint16_t OLED_DISPLAY     = 0xC29D;  // No data — flush framebuffer to display

// ─── MicroSD Virtual Storage ─ ESP32 → App / App → ESP32 ─────
constexpr uint16_t SD_FILE_NAME       = 0xC29E;  // ESP32 → App: [char0...charN] filename chunk (max 18)
constexpr uint16_t SD_FILE_WRITE      = 0xC29F;  // ESP32 → App: [char0...charN] file data chunk
constexpr uint16_t SD_FILE_WRITE_END  = 0xC2A0;  // ESP32 → App: no data — flush write
constexpr uint16_t SD_FILE_READ       = 0xC2A1;  // ESP32 → App: no data — request file content
constexpr uint16_t SD_FILE_DATA       = 0xC2A2;  // App → ESP32: [char0...charN] file data chunk
constexpr uint16_t SD_FILE_DATA_END   = 0xC2A3;  // App → ESP32: no data — end of file data
constexpr uint16_t SD_FILE_DELETE     = 0xC2A4;  // ESP32 → App: no data — delete named file
constexpr uint16_t SD_FILE_EXISTS     = 0xC2A5;  // ESP32 → App: no data — check if file exists
constexpr uint16_t SD_FILE_STATUS     = 0xC2A6;  // App → ESP32: [exists, sizeHi, sizeMid, sizeLo]
constexpr uint16_t SD_FILE_LIST_REQ   = 0xC2A7;  // ESP32 → App: no data — request file listing
constexpr uint16_t SD_FILE_LIST_ENTRY = 0xC2A8;  // App → ESP32: [char0...charN] filename chunk
constexpr uint16_t SD_FILE_LIST_END   = 0xC2A9;  // App → ESP32: no data — end of listing
constexpr uint16_t SD_FILE_APPEND     = 0xC2AA;  // ESP32 → App: [char0...charN] append data chunk
constexpr uint16_t SD_FILE_APPEND_END = 0xC2AB;  // ESP32 → App: no data — flush appended data

// ─── Component Type IDs (for REGISTER command) ────────────────────
constexpr uint8_t COMPONENT_LED     = 0x01;
constexpr uint8_t COMPONENT_LCD     = 0x02;
constexpr uint8_t COMPONENT_SERVO   = 0x03;
constexpr uint8_t COMPONENT_MOTOR   = 0x04;
constexpr uint8_t COMPONENT_BUZZER  = 0x05;
constexpr uint8_t COMPONENT_SCOPE   = 0x06;
constexpr uint8_t COMPONENT_SENSOR  = 0x07;
constexpr uint8_t COMPONENT_BUTTON  = 0x08;

} // namespace SBCmd

#endif // STEMBUDDY_PROTOCOL_H
