# Changelog

All notable changes to the ESP32 Complete Diagnostic Tool will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [4.0.1] - 2025-10-12

### Fixed
- TFT pattern API now returns localized "feature disabled" responses per requested pattern instead of triggering validation errors.
- TFT status strings refresh after language changes so the disabled message always matches the active locale.

### Changed
- Added the requested pattern identifier to the TFT API response payload for easier client-side handling.

---

## [4.0.0] - 2025-10-11

### Removed
- Dropped the TFT_eSPI dependency and all on-device TFT drawing routines while keeping the web UI tabs and API endpoints available.

### Changed
- CPU benchmark now reports MFLOPS using elapsed time in seconds and the effective floating-point operation count.
- Wi-Fi scanner now distinguishes all ESP-IDF authentication modes and surfaces scan failures instead of returning negative network counts.
- Memory fragmentation status now reuses translated warning strings instead of a hard-coded French label.
- I2C scan results fall back to the localized "none" string when no peripherals are found.

### Fixed
- TFT status messaging is now localized and aligned with the disabled hardware support state.

---

## [3.0.0] - 2025-10-10

### Added
- Exported version and mDNS host strings for re-use by the standalone web interface helper (`web_interface.h`).

### Changed
- Verified compatibility and updated documentation for ESP32 Arduino Core **3.3.2**.
- Refreshed English and French READMEs to reflect the official 3.0.0 release.
- Updated project metadata to use the semantic version identifier `3.0.0` across the firmware and web assets.

### Notes
- Consolidated the improvements from the 3.0 development cycle (dynamic multilingual web interface, REST endpoints, live updates) into the first stable v3 release.

---

## [2.5] - 2025-10-08

### Added
- **Translated exports** in French and English
- Export language matches selected interface language
- Consistent terminology across all export formats

### Changed
- CSV, TXT, and JSON exports now fully translated
- Print version respects language selection
- Improved export file naming with version number

### Fixed
- Missing translations in export files
- Inconsistent date formats in exports

---

## [2.4] - 2025-10-07

### 🌐 Added - Multilingual Interface

#### Features
- **Complete French and English interface**
- **Dynamic language switching** without page reload
- **Translation system** via `languages.h`
- New API endpoints:
  - `/api/set-language?lang=[fr|en]` - Change interface language
  - `/api/get-translations` - Get all translations for current language
- **Persistent language selection** during session
- Translated sections:
  - Navigation menu
  - All page titles and headers
  - Button labels
  - Status messages
  - Memory labels
  - WiFi information
  - Test results
  - Export descriptions

#### Files Added
- `languages.h` - Complete translation system with 150+ strings
- Translation structures for French (default) and English

#### Changed
- Interface defaults to French
- All user-facing strings now use translation system
- API responses include language indicator

---

## [2.3] - 2025-10-06

### Added - OLED Display Support

#### Features
- **OLED 0.96" I2C display detection and testing**
- **Complete test suite** with 10 different tests:
  1. Welcome text with system info
  2. Large text display
  3. Multiple text sizes
  4. Geometric shapes (rectangles, circles, triangles)
  5. Horizontal lines pattern
  6. Diagonal lines pattern
  7. Moving square animation
  8. Progress bar with percentage
  9. Scrolling text animation
  10. Final success message
- **Configurable I2C pins** via web interface (SDA/SCL)
- **Custom message display** feature
- **Automatic detection** at 0x3C address
- **Re-detection capability** when changing pins

#### API Endpoints
- `/api/oled-config?sda=X&scl=Y` - Configure I2C pins
- `/api/oled-test` - Run complete test suite (25 seconds)
- `/api/oled-message?message=TEXT` - Display custom message

#### Technical Details
- Support for SSD1306 128x64 displays
- I2C bus sharing with other peripherals
- Pin conflict detection and management
- Memory-efficient display operations

---

## [2.2] - 2025-10-05

### Added - TFT Display Support

#### Features
- **TFT SPI display testing** (ST7789/ILI9341)
- **Color tests** - Red, Green, Blue, White sequence
- **Checkerboard pattern** test
- **Resolution detection** (width x height)
- **Individual test patterns** accessible via web interface

#### API Endpoints
- `/api/tft-test` - Complete TFT test (15 seconds)
- `/api/tft-pattern?pattern=[colors|checkerboard|clear]` - Individual patterns

#### Configuration
- Fixed SPI pins for TFT (CS, DC, RST, MOSI, SCLK, MISO)
- Support for 240x320 and custom resolutions
- TFT_eSPI library integration

---

## [2.1] - 2025-10-04

### Added - NeoPixel Support

#### Features
- **NeoPixel/WS2812 LED strip support**
- **Auto-detection** for ESP32-S3 (GPIO 48) and ESP32-C3 (GPIO 8)
- **Configurable GPIO** and LED count via web interface
- **Multiple patterns**:
  - Rainbow effect (full HSV spectrum)
  - Blink pattern (configurable color)
  - Fade effect (smooth brightness transition)
  - Custom color with RGB picker
- **Complete test sequence** (RGB colors + rainbow)

#### API Endpoints
- `/api/neopixel-config?gpio=X&count=Y` - Configure NeoPixel
- `/api/neopixel-test` - Run complete test
- `/api/neopixel-pattern?pattern=[rainbow|blink|fade|off]` - Run pattern
- `/api/neopixel-color?r=X&g=Y&b=Z` - Set custom color

#### Technical Details
- Support for 1-100 LEDs
- NEO_GRB + NEO_KHZ800 configuration
- Gamma correction for smooth colors
- Memory-safe implementation with proper cleanup

---

## [2.0] - 2025-10-03

### Major Rewrite - Complete Diagnostic System

#### Added
- **Built-in LED testing** with configurable GPIO
- **LED control features**:
  - Blink pattern (5 times)
  - Fade effect (smooth PWM)
  - Individual on/off control
- **Detailed memory analysis**:
  - Flash memory (real size vs configured)
  - PSRAM detection and testing (OPI/QSPI)
  - SRAM statistics
  - Fragmentation analysis
- **PSRAM diagnostic** with detailed boot log
- **WiFi network scanner** with full details (RSSI, channel, encryption, BSSID)
- **I2C peripheral scanner** with address detection
- **GPIO testing** for all available pins
- **ADC testing** on all ADC channels
- **Touch pad testing** (ESP32/S3)
- **PWM testing** with duty cycle sweep
- **SPI bus detection**
- **Flash partition listing** with details
- **Memory stress test** with max allocation detection
- **CPU and Memory benchmarks** (MFLOPS, MB/s)

#### Technical Features
- **Multi-WiFi support** via WiFiMulti
- **mDNS support** (esp32-diagnostic.local)
- **Comprehensive logging** via Serial Monitor
- **Auto-detection** for chip model and features
- **Memory safety checks** throughout
- **Real-time monitoring** of temperature and uptime

#### Export Formats
- **TXT** - Human-readable report with all data
- **JSON** - Machine-readable structured data
- **CSV** - Spreadsheet-compatible format
- **Print** - PDF-ready formatted page

---

## [1.0] - 2025-10-01

### Initial Release

#### Features
- Basic chip information display
- WiFi connection
- Simple web interface
- Memory statistics (Flash and SRAM only)
- GPIO list
- Basic system information

#### Supported Boards
- ESP32 (original)
- ESP32-S2
- ESP32-S3
- ESP32-C3

---

## Development Roadmap

### [3.1] - Planned
- [ ] WebSocket support for real-time updates without polling
- [ ] Historical data graphs (memory, temperature over time)
- [ ] Data export to SD card
- [ ] Configuration save/restore
- [ ] OTA firmware update via web interface
- [ ] Advanced WiFi features (AP mode, WiFi config portal)

### [3.2] - Planned
- [ ] MQTT support for IoT integration
- [ ] REST API documentation (Swagger/OpenAPI)
- [ ] Dark mode theme
- [ ] Mobile app (PWA support)
- [ ] Multi-device dashboard

### [4.0] - Future
- [ ] Sensor support (BME280, DHT22, etc.)
- [ ] Automation rules and triggers
- [ ] Data logging and analytics
- [ ] User authentication
- [ ] Multi-language expansion (Spanish, German, Italian)

---

## Version History Summary

| Version | Date | Key Features |
|---------|------|--------------|
| 3.0.0 | 2025-10-10 | Official release, Core 3.3.2 support, multilingual docs |
| 2.5 | 2025-10-08 | Translated exports |
| 2.4 | 2025-10-07 | Multilingual interface (FR/EN) |
| 2.3 | 2025-10-06 | OLED display support |
| 2.2 | 2025-10-05 | TFT display support |
| 2.1 | 2025-10-04 | NeoPixel support |
| 2.0 | 2025-10-03 | Complete rewrite with all diagnostics |
| 1.0 | 2025-10-01 | Initial release |

---

## Contributing

See [CONTRIBUTING.md](CONTRIBUTING.md) for guidelines on:
- Reporting bugs
- Suggesting features
- Submitting pull requests
- Code style guidelines

---

## License

This project is licensed under the MIT License - see [LICENSE](LICENSE) file for details.

---

**Maintained by**: morfredus
**Repository**: https://github.com/yourusername/esp32-diagnostic
**Documentation**: https://github.com/yourusername/esp32-diagnostic/wiki
