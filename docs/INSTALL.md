# Installation Guide (EN)

Follow the steps below to deploy the ESP32 Diagnostic Suite on your board.

> **Maintenance 3.1.14-maint:** The firmware banner and `DIAGNOSTIC_VERSION` constant now expose 3.1.14-maint. Verify the Serial Monitor displays this value after flashing.

## 1. Prerequisites
- ESP32, ESP32-S2, ESP32-S3, ESP32-C3, ESP32-C6, or ESP32-H2 board (S3 recommended).
- USB cable with data capability.
- Optional: NeoPixel/WS2812B LED strip and 0.96" SSD1306 OLED screen for extended tests.
- Arduino IDE 2.x or newer with the ESP32 Arduino Core 3.3.2 toolchain.

## 2. Install required libraries
Install these libraries via the Arduino Library Manager:
- **Adafruit BusIO**
- **Adafruit GFX Library**
- **Adafruit SSD1306**
- **Adafruit NeoPixel**

The following libraries are bundled with the ESP32 core and do not require manual installation: `WiFi`, `WebServer`, `ESPmDNS`, `Wire`, `SPI`, `esp_chip_info`, `esp_flash`, `esp_heap_caps`, `esp_partition`.

## 3. Get the source code
1. [Fork the repository on GitHub](https://github.com/ESP32-Diagnostic/ESP32-Diagnostic/fork) or clone it into your Arduino sketchbook directory.
2. Ensure the folder is named `ESP32-Diagnostic` and contains `ESP32-Diagnostic.ino` at its root.

```
ESP32-Diagnostic/
├── ESP32-Diagnostic.ino
├── languages.h
├── web_interface.h
├── wifi-config-example.h
└── docs...
```

## 4. Prepare Wi-Fi credentials
1. Duplicate `wifi-config-example.h` and rename the copy to `wifi-config.h`.
2. Open `wifi-config.h` and replace the placeholder SSID/PASS pairs with your own networks.
3. Multiple networks can be declared; the firmware will try them in order.

Detailed configuration notes are available in [CONFIG.md](CONFIG.md).

## 5. Compile and upload
1. Open `ESP32-Diagnostic.ino` in Arduino IDE.
2. Select your ESP32 board and the correct serial port.
3. Ensure **PSRAM** is enabled for ESP32-S3 boards to unlock advanced diagnostics.
4. Click **Verify** to compile, then **Upload** to flash the firmware.

Upon success, the serial monitor will display the firmware version and connection status.
