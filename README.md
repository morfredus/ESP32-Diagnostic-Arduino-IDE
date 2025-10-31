# ESP32 Diagnostic Suite (v3.5.0)

Comprehensive diagnostic firmware for ESP32 microcontrollers featuring an interactive web dashboard, automated hardware tests, and bilingual content (FR/EN). The firmware targets Arduino Core 3.3.2 and supports the ESP32, ESP32-S2, ESP32-S3, ESP32-C3, ESP32-C6, and ESP32-H2 families.

## Highlights of version 3.5.0
- Completed the multilingual refresh so every dashboard card, status chip, and export action updates instantly when switching languages.
- Reused the shared translation catalog for Wi-Fi, Bluetooth, GPIO, and export prompts to eliminate duplicated strings across the UI and API.
- Updated the documentation set to capture the live-translation maintenance workflow introduced throughout the 3.4.x cycle.

## Project structure
- `ESP32-Diagnostic.ino` – firmware entry point and diagnostic scheduler.
- `languages.h` – translation catalog and runtime language switcher.
- `web_interface.h` – HTML/CSS/JS templates served by the firmware.
- `wifi-config-example.h` – template for Wi-Fi credential storage.
- `docs/` – comprehensive wiki including installation, usage, API reference, and deployment guides (EN/FR).

## Documentation
| Topic | English | Français |
|-------|---------|----------|
| Wiki home | [home.md](docs/home.md) | [home_FR.md](docs/home_FR.md) |
| Overview | [OVERVIEW.md](docs/OVERVIEW.md) | [OVERVIEW_FR.md](docs/OVERVIEW_FR.md) |
| Feature matrix | [FEATURE_MATRIX.md](docs/FEATURE_MATRIX.md) | [FEATURE_MATRIX_FR.md](docs/FEATURE_MATRIX_FR.md) |
| Diagnostic modules | [DIAGNOSTIC_MODULES.md](docs/DIAGNOSTIC_MODULES.md) | [DIAGNOSTIC_MODULES_FR.md](docs/DIAGNOSTIC_MODULES_FR.md) |
| Web interface | [WEB_INTERFACE.md](docs/WEB_INTERFACE.md) | [WEB_INTERFACE_FR.md](docs/WEB_INTERFACE_FR.md) |
| REST API | [API_REFERENCE.md](docs/API_REFERENCE.md) | [API_REFERENCE_FR.md](docs/API_REFERENCE_FR.md) |
| Build & deploy | [BUILD_AND_DEPLOY.md](docs/BUILD_AND_DEPLOY.md) | [BUILD_AND_DEPLOY_FR.md](docs/BUILD_AND_DEPLOY_FR.md) |
| Security | [SECURITY.md](docs/SECURITY.md) | [SECURITY_FR.md](docs/SECURITY_FR.md) |
| FAQ | [FAQ.md](docs/FAQ.md) | [FAQ_FR.md](docs/FAQ_FR.md) |
| Installation | [INSTALL.md](docs/INSTALL.md) | [INSTALL_FR.md](docs/INSTALL_FR.md) |
| Configuration | [CONFIG.md](docs/CONFIG.md) | [CONFIG_FR.md](docs/CONFIG_FR.md) |
| Usage guide | [USAGE.md](docs/USAGE.md) | [USAGE_FR.md](docs/USAGE_FR.md) |
| Firmware architecture | [ARCHITECTURE.md](docs/ARCHITECTURE.md) | [ARCHITECTURE_FR.md](docs/ARCHITECTURE_FR.md) |
| Troubleshooting | [TROUBLESHOOTING.md](docs/TROUBLESHOOTING.md) | [TROUBLESHOOTING_FR.md](docs/TROUBLESHOOTING_FR.md) |
| Contribution rules | [CONTRIBUTING.md](CONTRIBUTING.md) | [CONTRIBUTING_FR.md](CONTRIBUTING_FR.md) |
| Changelog | [CHANGELOG.md](CHANGELOG.md) | [CHANGELOG_FR.md](CHANGELOG_FR.md) |

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
- Issues & feature requests: [Open a GitHub issue](https://github.com/ESP32-Diagnostic/ESP32-Diagnostic/issues/new/choose) with reproduction steps and board details.
- Contributions: [Fork the repository](https://github.com/ESP32-Diagnostic/ESP32-Diagnostic/fork) and see [CONTRIBUTING.md](CONTRIBUTING.md).
