# ESP32 Diagnostic Suite – Build & Deploy (v3.4.0)

This document describes the supported toolchains and the recommended release checklist for firmware version 3.4.0.

## Toolchains
### Arduino IDE 2.x
1. Install the ESP32 boards package **version 3.3.2** from the Boards Manager.
2. Clone or download this repository and open `ESP32-Diagnostic.ino`.
3. Select the correct board profile (e.g., *ESP32 Dev Module*, *ESP32-S3 DevKit*).
4. Configure `Tools → PSRAM` and `USB CDC` options based on the hardware.
5. Flash the firmware (`Ctrl+U`). Monitor the serial console at 115200 baud.

### Arduino CLI
```bash
arduino-cli core install esp32:esp32@3.3.2
arduino-cli lib install "Adafruit GFX Library" "Adafruit SSD1306" "Adafruit NeoPixel" "Adafruit BusIO"
arduino-cli compile --fqbn esp32:esp32:esp32s3 ESP32-Diagnostic.ino
arduino-cli upload --fqbn esp32:esp32:esp32s3 -p /dev/ttyUSB0 ESP32-Diagnostic.ino
```

### PlatformIO
`platformio.ini` (sample extract):
```ini
[env:esp32-s3-devkit]
platform = espressif32@6.5.0
board = esp32-s3-devkitc-1
framework = arduino
monitor_speed = 115200
lib_deps =
  adafruit/Adafruit GFX Library@^1.11.9
  adafruit/Adafruit NeoPixel@^1.11.0
  adafruit/Adafruit SSD1306@^2.5.7
  adafruit/Adafruit BusIO@^1.15.0
```
Run `pio run --target upload` to build and flash.

## Pre-deployment checklist
- [ ] Update `wifi-config.h` with production SSID/passwords and optional enterprise credentials.
- [ ] Confirm `DIAGNOSTIC_VERSION` matches `3.4.0` in both firmware and documentation.
- [ ] Verify multilingual assets compile without warnings (`languages.h`).
- [ ] Run a full diagnostic cycle on a reference board and export JSON/CSV reports.
- [ ] Capture screenshots or printouts of the dashboard for release notes if required.

## Acceptance tests
| Test | Procedure | Expected result |
|------|-----------|-----------------|
| Connectivity smoke test | Connect to lab Wi-Fi, run quick diagnostics. | PASS with RSSI above -70 dBm, ping < 40 ms. |
| Memory soak | Run full diagnostics 5 times consecutively. | No heap exhaustion, PSRAM stable. |
| Peripheral validation | Attach OLED + NeoPixel chain, enable modules. | PASS with consistent frame rate and LED animation. |
| REST API sanity | Trigger `/api/run` then `/api/status`. | Receives queued status and JSON payload. |

## Release packaging
1. Tag the repository with `v3.4.0` (Git annotated tag).
2. Attach compiled binaries (`.bin`) for each target board if distributing via GitHub Releases.
3. Publish release notes referencing [CHANGELOG.md](../CHANGELOG.md).
4. Share links to the updated wiki (`docs/home.md`) for support teams.

## OTA considerations
- The firmware is primarily distributed via USB flashing. For OTA, integrate with your preferred bootloader and ensure the
  diagnostic binary remains under the OTA partition size.
- Always validate signature or checksum after OTA transfer to prevent corrupted diagnostics.

## Rollback strategy
- Keep a known-good image (e.g., v3.2.21-maint) ready for emergency downgrades.
- Document the rollback steps in the internal SOP, referencing this guide for re-flashing instructions.

## Related resources
- [Installation guide](INSTALL.md)
- [Configuration guide](CONFIG.md)
- [Release notes](../CHANGELOG.md)
