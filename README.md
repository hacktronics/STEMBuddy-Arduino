# STEMBuddy Arduino Library

Turn your phone into virtual hardware for ESP32 projects.

STEMBuddy lets your ESP32 control virtual LEDs, LCDs, servos, motors, buzzers, and oscilloscopes on the [STEM Buddy Android app](https://play.google.com/store/apps/details?id=cc.codeskool.stembuddy) over BLE. The app can also send virtual sensor data and button inputs back to your ESP32 — no physical components needed.

## How It Works

```
ESP32 (your code)  ──BLE──>  Phone (STEM Buddy app)
    buddy.led.on()           Virtual LED lights up
    buddy.lcd.print("Hi")   Virtual LCD shows text
    buddy.servo.setAngle(90) Virtual servo animates

Phone (sensors/inputs) ──BLE──>  ESP32
    Accelerometer, light      buddy.sensor.temperature()
    Buttons, sliders, pots    buddy.input.potValue(0)
```

Your phone becomes the component. Students don't need to buy LEDs, LCDs, or sensors — just an ESP32 and the free app.

## Installation

### Arduino Library Manager (recommended)
1. Open Arduino IDE
2. Go to **Sketch > Include Library > Manage Libraries...**
3. Search for **STEMBuddy**
4. Click **Install**

### Manual Install
1. Download this repository as a ZIP
2. In Arduino IDE, go to **Sketch > Include Library > Add .ZIP Library...**
3. Select the downloaded ZIP file

### PlatformIO
Add to your `platformio.ini`:
```ini
lib_deps = hacktronics/STEMBuddy-Arduino
```

## Quick Start

```cpp
#include <STEMBuddy.h>

STEMBuddy buddy;

void setup() {
    buddy.begin("My Project");   // Start BLE with device name
}

void loop() {
    buddy.update();              // Process BLE messages

    if (buddy.isConnected()) {
        buddy.led.setColor(255, 0, 0);  // Red
        buddy.led.on();
        delay(1000);
        buddy.led.off();
        delay(1000);
    }
}
```

## Virtual Components

### Outputs (ESP32 → Phone)

| Component | Object | Example |
|-----------|--------|---------|
| LED / RGB LED | `buddy.led` | `buddy.led.setColor(r, g, b)` |
| LCD Display | `buddy.lcd` | `buddy.lcd.print(0, 0, "Hello!")` |
| Servo Motor | `buddy.servo` | `buddy.servo.setAngle(90)` |
| DC Motor | `buddy.motor` | `buddy.motor.setSpeed(200)` |
| Buzzer | `buddy.buzzer` | `buddy.buzzer.tone(440)` |
| Oscilloscope | `buddy.scope` | `buddy.scope.plot(value)` |
| Text-to-Speech | `buddy.tts` | `buddy.tts.speak("Hello")` |
| Serial Terminal | `buddy.terminal` | `buddy.terminal.send("debug info")` |
| Notifications | `buddy.notify` | `buddy.notify.send("Alert!")` |
| Piano | `buddy.piano` | `buddy.piano.play(60)` |
| Audio | `buddy.audio` | `buddy.audio.play()` |

### Inputs (Phone → ESP32)

| Component | Object | Example |
|-----------|--------|---------|
| Virtual Sensors | `buddy.sensor` | `buddy.sensor.temperature()` |
| Buttons / Pots / Sliders | `buddy.input` | `buddy.input.potValue(0)` |
| Camera | `buddy.camera` | `buddy.camera.capture()` |
| NFC | `buddy.nfc` | `buddy.nfc.read()` |

## Examples

The library includes ready-to-use examples:

- **LEDBlink** — Blink a virtual LED with color changes
- **LCDHello** — Display text on a virtual LCD
- **ServoSweep** — Sweep a virtual servo back and forth
- **MotorControl** — Control virtual DC motor speed
- **BuzzerMelody** — Play a melody on the virtual buzzer
- **SensorRead** — Read virtual sensor data from the phone
- **MotionDetect** — React to phone motion sensor
- **Compass** — Use phone magnetometer as a compass

Open them in Arduino IDE via **File > Examples > STEMBuddy**.

## Requirements

- **Board:** ESP32 (any variant with BLE support)
- **Arduino Core:** [arduino-esp32](https://github.com/espressif/arduino-esp32) v2.0+
- **App:** [STEM Buddy](https://play.google.com/store/apps/details?id=cc.codeskool.stembuddy) on Android

## Protocol

See [PROTOCOL.md](docs/PROTOCOL.md) for the full BLE command protocol specification.

## License

MIT License — see [LICENSE](LICENSE) for details.

## Links

- [STEM Buddy App](https://play.google.com/store/apps/details?id=cc.codeskool.stembuddy)
- [CodeSkool](https://codeskool.cc)
- [Hacktronics](https://github.com/hacktronics)
