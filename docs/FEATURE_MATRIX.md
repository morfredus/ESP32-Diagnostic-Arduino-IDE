# ESP32 Diagnostic Suite – Feature Matrix (v3.3.0)

This matrix summarises the diagnostic coverage for the most common Espressif boards supported by version 3.3.0. Use it to plan
validation campaigns and to verify whether optional peripherals require additional wiring.

## Legend
- ✅ – Supported out of the box by the firmware.
- ⚙️ – Supported with optional configuration or hardware tweaks (see notes column).
- ⛔ – Not available for the target board or requires custom firmware extensions.

## Core board capabilities
| Board family | Wi-Fi scan | BLE advert scan | PSRAM probe | Flash integrity | NeoPixel test | OLED 0.96" test | Notes |
|--------------|-----------|-----------------|-------------|-----------------|---------------|-----------------|-------|
| ESP32 (WROOM/DevKitC) | ✅ | ✅ | ⚙️ | ✅ | ✅ | ✅ | PSRAM detection available on WROVER variants with PSRAM soldered. |
| ESP32-S2 | ✅ | ⛔ | ✅ | ✅ | ✅ | ✅ | Native USB supported; BLE hardware absent. |
| ESP32-S3 | ✅ | ✅ | ✅ | ✅ | ✅ | ✅ | Dual USB/Serial; ensure Arduino Core 3.3.2 for full coverage. |
| ESP32-C3 | ✅ | ✅ | ⛔ | ✅ | ⚙️ | ⚙️ | Requires pin remapping for NeoPixel; OLED via I2C only. |
| ESP32-C6 | ✅ | ✅ | ⛔ | ✅ | ⚙️ | ⚙️ | Thread and Matter radios unused; configure pins for LED/OLED. |
| ESP32-H2 | ⛔ | ✅ | ⛔ | ✅ | ⛔ | ⚙️ | Wi-Fi absent; focus on BLE diagnostics and GPIO matrix. |

## Peripheral bus coverage
| Bus / Peripheral | Default pins | Supported boards | Notes |
|------------------|--------------|------------------|-------|
| I2C primary bus | SDA 21, SCL 22 (overridable) | All (except remap on ESP32-C3/C6/H2) | Used for OLED, sensor packs, EEPROM. |
| I2C secondary bus | Disabled by default | ESP32, ESP32-S3 | Enable via `ENABLE_SECONDARY_I2C` flag in `config.h`. |
| SPI test bus | MOSI 23, MISO 19, CLK 18, CS 5 | ESP32, ESP32-S3 | Validates external flash/SD adaptors. |
| UART loopback | TX0/RX0 & optional UART1 | All | Requires jumper wire TX↔RX on chosen UART. |
| CAN (TWAI) probe | GPIO 4 / 5 | ESP32, ESP32-S3 | Needs external transceiver (e.g. SN65HVD230). |
| OneWire temperature | GPIO 15 | ESP32, ESP32-S3 | Dallas DS18B20 detection when `ENABLE_ONEWIRE` is set. |

## Optional module checklist
| Module | Firmware flag | Default state | Description |
|--------|---------------|---------------|-------------|
| OLED diagnostics | `ENABLE_OLED_TESTS` | Enabled when OLED wiring detected | Runs pixel sweep, contrast, I2C recovery. |
| NeoPixel animator | `ENABLE_NEOPIXEL_TESTS` | Enabled | Exercises colour wheel and stress test at 800 kHz. |
| MQTT reporter | `ENABLE_MQTT_BRIDGE` | Disabled | Publishes summary payloads; requires broker credentials in `config.h`. |
| REST automation | `ENABLE_REST_API` | Enabled | Provides JSON endpoints for CI/CD or fleet tools. |
| Power monitor | `ENABLE_POWER_MONITOR` | Disabled | Reads INA219/INA3221 sensors over I2C. |

## Environmental assumptions
- ESP32 Arduino Core **3.3.2** (PlatformIO `espressif32@6.5.x` or Arduino Boards Manager 3.0.2+).
- USB power supply delivering at least 500 mA during OLED/NeoPixel stress tests.
- Stable Wi-Fi network (2.4 GHz) for throughput benchmarks; captive portals are not supported.

## Related documents
- [Configuration guide](CONFIG.md) – enabling or disabling optional modules.
- [Diagnostic modules deep dive](DIAGNOSTIC_MODULES.md) – execution order and metrics captured for each check.
- [Troubleshooting](TROUBLESHOOTING.md) – steps to follow when a module is reported as unavailable.
