<!-- 
Available in multiple languages:
- English: README.md
- Français: README.fr.md
-->
# ESP32 Complete Diagnostic Tool v3.0-dev

[![Version](https://img.shields.io/badge/version-3.0--dev-blue.svg)](https://github.com/yourusername/esp32-diagnostic)
[![License](https://img.shields.io/badge/license-MIT-green.svg)](LICENSE)
[![Platform](https://img.shields.io/badge/platform-ESP32-orange.svg)](https://www.espressif.com/)
[![Arduino](https://img.shields.io/badge/Arduino-3.1.3-teal.svg)](https://github.com/espressif/arduino-esp32)

A comprehensive web-based diagnostic tool for ESP32 microcontrollers with real-time monitoring, hardware testing, and dynamic interface.

[🇫🇷 Version française](README.fr.md) | [📋 Changelog](CHANGELOG.md)

![ESP32 Diagnostic Interface](docs/screenshot.png)

## 🌟 Key Features

### 🔄 Real-Time Monitoring (v3.0-dev)
- **Dynamic web interface** with asynchronous loading
- **Auto-refresh every 5 seconds** for live data
- **REST API architecture** for efficient communication
- **Connection status indicator** (online/offline)
- **Smooth animations and transitions**

### 🔧 System Information
- Complete chip identification (ESP32, S2, S3, C3, C6, H2)
- CPU cores and frequency
- MAC address
- SDK and ESP-IDF versions
- Internal temperature sensor (if available)
- Uptime tracking
- Reset reason detection

### 💾 Detailed Memory Analysis
- **Flash Memory**: Real size vs configured size
- **PSRAM**: Total, free, used (OPI/QSPI support)
- **SRAM**: Internal memory statistics
- **Fragmentation analysis** with status
- Memory stress testing
- Real-time memory graphs

### 📡 WiFi Management
- Multi-network support (WiFiMulti)
- Network scanner with details:
  - SSID, BSSID, Channel
  - Signal strength (RSSI)
  - Encryption type
  - Signal quality indicator
- Connection information (IP, Subnet, Gateway, DNS)
- mDNS support (esp32-diagnostic.local)

### 🔌 GPIO Testing
- Individual GPIO testing (Digital I/O)
- Complete pin mapping for each ESP32 variant
- Visual OK/FAIL indicators
- ADC testing (Analog to Digital Converter)
- Touch sensor testing (ESP32/S3)
- PWM testing (Pulse Width Modulation)

### 💡 LED Control
- **Built-in LED**:
  - Configurable GPIO pin
  - Blink and fade patterns
  - Individual control
- **NeoPixel/WS2812**:
  - Configurable GPIO and LED count
  - Rainbow effect
  - Custom colors with color picker
  - Blink and fade animations

### 📺 Screen Support
- **TFT SPI Display**:
  - Full color test
  - Checkerboard pattern
  - Custom resolution detection
- **OLED 0.96" I2C**:
  - Complete test suite (10 tests)
  - Text display (multiple sizes)
  - Geometric shapes
  - Line patterns
  - Animations (moving square, scrolling text)
  - Progress bar
  - Custom message display

### 🧪 Advanced Testing
- I2C peripheral scanner
- SPI bus detection
- Flash partition listing
- Memory stress testing
- CPU and memory benchmarks (MFLOPS, MB/s)

### 🌐 Multilingual Interface
- **French** (default)
- **English**
- Dynamic language switching without reload
- Translated exports

### 📤 Data Export
- **TXT**: Human-readable report
- **JSON**: Structured format for scripts
- **CSV**: Excel/spreadsheet compatible
- **Print**: PDF-ready formatted page

## 📋 Hardware Requirements

### Minimum
- ESP32 (any variant)
- USB cable for programming
- WiFi network

### Tested Platforms
- ✅ ESP32 (original)
- ✅ ESP32-S2
- ✅ ESP32-S3 (with PSRAM OPI)
- ✅ ESP32-C3
- ⚠️ ESP32-C6, H2 (not fully tested)

### Optional Hardware
- NeoPixel/WS2812 LED strip
- TFT SPI display (ST7789/ILI9341)
- OLED 0.96" I2C display (SSD1306)
- Touch sensors
- External peripherals (I2C/SPI)

## 🚀 Quick Start

### 1. Installation

#### Arduino IDE
1. Install [Arduino IDE](https://www.arduino.cc/en/software) 1.8.19 or later
2. Add ESP32 board support:
   - File → Preferences
   - Additional Board Manager URLs: `https://espressif.github.io/arduino-esp32/package_esp32_index.json`
   - Tools → Board → Boards Manager → Search "ESP32" → Install
3. Install required libraries:
   - Adafruit NeoPixel
   - TFT_eSPI
   - Adafruit GFX
   - Adafruit SSD1306

#### PlatformIO
```ini
[env:esp32dev]
platform = espressif32
board = esp32-s3-devkitc-1
framework = arduino
lib_deps =
    adafruit/Adafruit NeoPixel@^1.12.0
    bodmer/TFT_eSPI@^2.5.0
    adafruit/Adafruit GFX Library@^1.11.0
    adafruit/Adafruit SSD1306@^2.5.0
```

### 2. Configuration

Create `config.h`:
```cpp
#ifndef CONFIG_H
#define CONFIG_H

// WiFi Networks (supports multiple)
#define WIFI_SSID_1 "YourNetworkName"
#define WIFI_PASS_1 "YourPassword"

#define WIFI_SSID_2 "SecondaryNetwork"
#define WIFI_PASS_2 "SecondaryPassword"

#endif
```

### 3. Upload

1. Connect your ESP32
2. Select your board:
   - Tools → Board → ESP32 Arduino → [Your ESP32 model]
3. Select port: Tools → Port → [Your COM port]
4. **Important for PSRAM**: Tools → PSRAM → OPI PSRAM (or QSPI PSRAM)
5. Upload: Sketch → Upload

### 4. Access

After upload, open Serial Monitor (115200 baud) to see:
```
http://esp32-diagnostic.local
or
http://192.168.x.x
```

Open your browser and navigate to the displayed URL.

## 📖 Usage Guide

### Web Interface Tabs

#### 🏠 Overview
- Complete system status
- Chip information
- Memory breakdown with progress bars
- WiFi connection details
- GPIO and peripherals summary

#### 💡 LEDs
- Built-in LED configuration and testing
- NeoPixel setup and patterns
- Real-time control

#### 📺 Screens
- TFT display testing
- OLED display testing
- Custom message display

#### 🧪 Tests
- ADC channel testing
- Touch pad testing
- PWM testing
- SPI bus scanning
- Flash partition listing
- Memory stress testing

#### 🔌 GPIO
- Complete GPIO testing
- Visual pin status

#### 📡 WiFi
- Network scanner
- I2C peripheral scanner

#### ⚡ Performance
- CPU benchmark
- Memory benchmark
- Performance metrics

#### 📤 Export
- Download diagnostic reports
- Multiple format options

### API Endpoints (v3.0-dev)

The tool provides a complete REST API:

#### Real-Time Data
- `GET /api/status` - System status (5s refresh)
- `GET /api/system-info` - Chip and network info
- `GET /api/overview` - Complete overview
- `GET /api/memory` - Memory details
- `GET /api/wifi-info` - WiFi information
- `GET /api/peripherals` - I2C/SPI devices

#### Hardware Info
- `GET /api/leds-info` - LED status
- `GET /api/screens-info` - Screen status

#### Testing
- `GET /api/test-gpio` - Test all GPIO
- `GET /api/wifi-scan` - Scan WiFi networks
- `GET /api/i2c-scan` - Scan I2C devices
- `GET /api/adc-test` - Test ADC channels
- `GET /api/touch-test` - Test touch pads
- `GET /api/pwm-test` - Test PWM
- `GET /api/benchmark` - Run benchmarks

#### Control
- `GET /api/builtin-led-config?gpio=X` - Configure LED
- `GET /api/builtin-led-test` - Test LED
- `GET /api/builtin-led-control?action=[blink|fade|off]` - Control LED
- `GET /api/neopixel-config?gpio=X&count=Y` - Configure NeoPixel
- `GET /api/neopixel-pattern?pattern=[rainbow|blink|fade|off]` - NeoPixel pattern
- `GET /api/neopixel-color?r=X&g=Y&b=Z` - Set NeoPixel color

## 🔧 Advanced Configuration

### Custom Pin Configuration

Edit in the main `.ino` file:

```cpp
// I2C for OLED (can be changed via web interface)
int I2C_SDA = 8;
int I2C_SCL = 9;

// TFT SPI (fixed)
#define TFT_MOSI  45
#define TFT_SCLK   3
#define TFT_CS    14
#define TFT_DC    47
#define TFT_RST   21
#define TFT_MISO  46

// NeoPixel (can be changed via web interface)
#define CUSTOM_LED_PIN -1  // -1 for auto-detect
#define CUSTOM_LED_COUNT 1
```

### TFT_eSPI Configuration

Edit `User_Setup.h` in TFT_eSPI library:

```cpp
#define ST7789_DRIVER
#define TFT_WIDTH  240
#define TFT_HEIGHT 320
#define TFT_MOSI 45
#define TFT_SCLK 3
#define TFT_CS   14
#define TFT_DC   47
#define TFT_RST  21
```

### mDNS Hostname

Change in `.ino`:
```cpp
#define MDNS_HOSTNAME "esp32-diagnostic"
```

### Auto-Refresh Interval

Edit in `web_interface.h` (JavaScript section):
```javascript
const UPDATE_INTERVAL = 5000; // milliseconds (5 seconds)
```

## 🐛 Troubleshooting

### WiFi Connection Issues
- Check credentials in `config.h`
- Verify 2.4GHz network (ESP32 doesn't support 5GHz)
- Try moving closer to router
- Check Serial Monitor for IP address

### PSRAM Not Detected
1. Enable in Arduino IDE: Tools → PSRAM → OPI PSRAM (or QSPI)
2. Check your board specifications
3. View diagnostic in Serial Monitor at startup

### OLED Not Detected
- Verify I2C pins (default: SDA=8, SCL=9)
- Check I2C address (default: 0x3C)
- Use web interface to change pins
- Run I2C scan to detect address

### TFT Not Working
- Check SPI pins in TFT_eSPI User_Setup.h
- Verify display driver (ST7789/ILI9341)
- Run test from web interface

### Compilation Errors
- Update ESP32 core to 3.1.3 or later
- Install all required libraries
- Check Arduino IDE board selection

## 📊 Performance

### Memory Usage
- **Flash**: ~800KB (program)
- **SRAM**: ~50KB (runtime)
- **PSRAM**: Optional (8MB recommended)

### Web Interface
- Initial load: ~100KB (HTML+CSS+JS)
- API calls: ~1-5KB per request
- Auto-refresh: Minimal overhead

### Benchmarks (ESP32-S3 @ 240MHz)
- CPU: ~1200µs (83 MFLOPS)
- Memory: ~800µs
- Web response: <100ms

## 🤝 Contributing

Contributions are welcome! Please:

1. Fork the repository
2. Create a feature branch
3. Make your changes
4. Test thoroughly
5. Submit a pull request

### Development Guidelines
- Follow existing code style
- Comment complex sections
- Update documentation
- Test on multiple ESP32 variants

## 📝 License

MIT License - see [LICENSE](LICENSE) file

## 👤 Author

**morfredus**

## 🙏 Acknowledgments

- Espressif Systems for ESP32 platform
- Arduino community
- Adafruit for excellent libraries
- Bodmer for TFT_eSPI library
- All contributors and testers

## 📚 Documentation

- [ESP32 Official Docs](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/)
- [Arduino ESP32 Core](https://github.com/espressif/arduino-esp32)
- [TFT_eSPI Wiki](https://github.com/Bodmer/TFT_eSPI)
- [Adafruit Learning](https://learn.adafruit.com/)

## 🔗 Related Projects

- [ESP32-Web-Server](https://github.com/espressif/arduino-esp32/tree/master/libraries/WebServer)
- [ESPAsyncWebServer](https://github.com/me-no-dev/ESPAsyncWebServer)
- [ESP32-PSRAM-Analyzer](https://github.com/espressif/esp-idf)

## ⭐ Show Your Support

If this project helped you, please give it a ⭐!

---

**Version**: 3.0-dev
**Last Updated**: 10/09/2025
**Status**: Active Development
```
