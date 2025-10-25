# ESP32 Diagnostic Suite (v3.1.0)

Comprehensive diagnostic firmware for ESP32 microcontrollers featuring an interactive web dashboard, automated hardware tests, and bilingual content (FR/EN). The firmware targets Arduino Core 3.3.2 and supports the ESP32, ESP32-S2, ESP32-S3, ESP32-C3, ESP32-C6, and ESP32-H2 families.

## Highlights of version 3.1.0
- Native Bluetooth® Low Energy advertisement and GATT service with automatic activation on supported boards.
- Refined Wi-Fi status reporting with clearer error paths and resilience on recent ESP32 revisions.
- Fully responsive navigation bar and wireless tab redesigned for compact displays.
- Improved tab navigation stability with delegated event handling.

## Project structure
- `ESP32-Diagnostic.ino` – firmware entry point and diagnostic scheduler.
- `languages.h` – translation catalog and runtime language switcher.
- `web_interface.h` – HTML/CSS/JS templates served by the firmware.
- `wifi-config-example.h` – template for Wi-Fi credential storage.

## Documentation
| Topic | English | Français |
|-------|---------|----------|
| Installation | [INSTALL.md](INSTALL.md) | [INSTALL_FR.md](INSTALL_FR.md) |
| Configuration | [CONFIG.md](CONFIG.md) | [CONFIG_FR.md](CONFIG_FR.md) |
| Usage guide | [USAGE.md](USAGE.md) | [USAGE_FR.md](USAGE_FR.md) |
| Firmware architecture | [ARCHITECTURE.md](ARCHITECTURE.md) | [ARCHITECTURE_FR.md](ARCHITECTURE_FR.md) |
| Troubleshooting | [TROUBLESHOOTING.md](TROUBLESHOOTING.md) | [TROUBLESHOOTING_FR.md](TROUBLESHOOTING_FR.md) |
| Contribution rules | [CONTRIBUTING.md](CONTRIBUTING.md) | [CONTRIBUTING_FR.md](CONTRIBUTING_FR.md) |

## Core capabilities
- Multilingual interface with real-time FR/EN switching without page reload.
- Automated hardware diagnostics covering GPIO, ADC, touch pads, PWM, I2C, SPI, PSRAM, flash, and Wi-Fi scanning.
- OLED 0.96" I2C screen test suite with per-step triggers and animation previews.
- REST API endpoints for triggering diagnostics and retrieving reports (TXT/JSON/CSV/printable view).
- Wi-Fi multi-AP support, mDNS discovery via `http://ESP32-Diagnostic.local`, and exportable reports.
- Optional NeoPixel/WS2812B LED test patterns and benchmarking tools for CPU/memory.

## Compatibility & requirements
- **Boards:** ESP32, ESP32-S2, ESP32-S3 (recommended), ESP32-C3, ESP32-C6, ESP32-H2.
- **Arduino IDE:** 2.x or newer with ESP32 Arduino Core 3.3.2.
- **Libraries:** Adafruit BusIO, Adafruit GFX Library, Adafruit SSD1306, Adafruit NeoPixel (install via Library Manager).

## Support
- License: [MIT](LICENSE)
- Issues & feature requests: open a ticket with reproduction steps and board details.
- Contributions: see [CONTRIBUTING.md](CONTRIBUTING.md).
