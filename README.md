<!-- 
Available in multiple languages:
- English: README.md
- Français: README.fr.md
-->
# ESP32 Complete Diagnostic v2.7.1

[🇫🇷 Version française](README.fr.md) | 🇬🇧 English Version

> **Note:** This is the English version. Pour la version française, [cliquez ici](README.fr.md).

---

## 📋 Description

Comprehensive **multilingual** diagnostic tool for ESP32 microcontrollers, accessible via web interface. Automatically tests all hardware components, analyzes memory, scans peripherals and generates detailed reports.

**What's new in v2.7.1**: The Bluetooth diagnostic now auto-detects toolchain support and gracefully keeps inline guidance when the ESP-IDF headers are missing, avoiding compilation failures on targets sans pile Bluetooth.

## ✨ Features

### 🌍 Multilingual Interface (NEW in v2.4)
- **French** (default language)
- **English**
- **Dynamic switching**: FR/EN buttons in interface
- **Real-time update**: No reload required
- **Complete translations**: All texts, labels, messages and status

### Hardware Tests
- **Built-in LED** - Configuration and testing with patterns (blink, fade)
- **NeoPixel/WS2812B** - Multi-LED support with RGB effects
- **OLED 0.96" I2C Screen** - 10 display tests including animations + manual step triggers
- **GPIO** - Automatic testing of all available GPIO
- **ADC** - Reading all analog channels
- **Touch Pads** - Capacitive touch sensor testing
- **PWM/LEDC** - Pulse width modulation testing

### System Analysis
- **Detailed Memory** - Flash, PSRAM (OPI/QSPI), SRAM with integrity tests
- **WiFi Scanner** - Network scan with RSSI, channel, encryption
- **I2C Scan** - Automatic peripheral detection (0x01-0x7F)
- **SPI Scan** - Available SPI bus information
- **Flash Partitions** - Complete partition list
- **Benchmarks** - CPU and memory performance
- **Stress Test** - Memory allocation limit testing

### Web Interface
- **8 tabs** - Intuitive navigation (Overview, LEDs, Screens, Advanced Tests, GPIO, WiFi, Performance, Export)
- **Language selector** - FR/EN buttons in top right
- **Dynamic configuration** - OLED I2C pins modifiable via web
- **Real-time** - Data refresh without reload
- **Responsive** - Mobile/tablet/desktop compatible
- **Complete exports** - TXT, JSON, CSV, printable PDF version

### Network Access
- **mDNS** - Access via http://ESP32-Diagnostic.local
- **Multi-WiFi** - Automatic connection with failover
- **REST API** - JSON endpoints for integration

## 🎯 Compatible Boards

Tested and optimized for:
- ✅ **ESP32** (original)
- ✅ **ESP32-S2**
- ✅ **ESP32-S3** (recommended - with OPI PSRAM)
- ✅ **ESP32-C3**
- ⚠️ **ESP32-C6, ESP32-H2** (not tested but should work)

## 📦 Requirements

### Hardware
- ESP32 board (any model)
- USB cable
- Optional: NeoPixel LED, OLED screen

### Software
- **Arduino IDE** 2.x or higher
- **ESP32 Arduino Core** 3.1.3 or higher

### Required Arduino Libraries

**Mandatory (via Library Manager):**
```
1. Adafruit BusIO
2. Adafruit GFX Library
3. Adafruit SSD1306
4. Adafruit NeoPixel
```

**Included with ESP32 Core:**
```
WiFi, WebServer, ESPmDNS, Wire, SPI
esp_chip_info, esp_mac, esp_flash, esp_heap_caps, esp_partition
```

## 🚀 Installation

### 1. Download

Download or clone this project to your Arduino folder.

### 2. Required Files

**Project structure:**
```
ESP32-Diagnostic/
├── ESP32-Diagnostic.ino              (main file)
├── languages.h                       (NEW - translations)
├── config.h.example                  (configuration template)
├── config.h                          (your credentials - to create)
└── README.md
```

### 3. WiFi Configuration

**IMPORTANT:** Create a `config.h` file in the same folder as the `.ino`

**Option A - Rename the example file:**
Rename the file `example-config.h` to `config.h`
Edit the file and add your WiFi network(s) by filling in the values

```cpp
const char* WIFI_SSID_1 = "SSID1";
const char* WIFI_PASS_1 = "Password1";

// Add as many networks as needed
// const char* WIFI_SSID_2 = "SSID2";
// const char* WIFI_PASS_2 = "Password2";

// const char* WIFI_SSID_3 = "SSID3";
// const char* WIFI_PASS_3 = "Password3";
```

**Option B - Create `config.h` manually:**
```cpp
#ifndef CONFIG_H
#define CONFIG_H

// ========== WIFI CONFIGURATION ==========
const char* WIFI_SSID_1 = "SSID1";
const char* WIFI_PASS_1 = "Password1";

// Add as many networks as needed
// const char* WIFI_SSID_2 = "SSID2";
// const char* WIFI_PASS_2 = "Password2";

// const char* WIFI_SSID_3 = "SSID3";
// const char* WIFI_PASS_3 = "Password3";

#endif
```

Replace `YourSSID` and `YourPassword` with your actual WiFi credentials.

### 4. Copy languages.h

Copy the provided `languages.h` file to the same folder as the `.ino`.

### 5. Hardware Configuration

**PSRAM (if available):**
- ESP32-S3: `Tools → PSRAM → OPI PSRAM`
- ESP32: `Tools → PSRAM → Enabled`

**Flash Size:**
```
Tools → Flash Size → [Your board's actual size]
```

### 6. Compile and Upload

```
Sketch → Upload
```

### 7. Accessing the Interface

- **http://ESP32-Diagnostic.local** (mDNS)
- **http://[DISPLAYED_IP]** (direct IP)

## 🌐 Multilingual Usage

### Language Switching

**In the web interface:**
1. Click the **FR** or **EN** button in the top right
2. The interface updates **instantly**
3. No reload required

**Default:** French interface

**Via API:**
```bash
# Switch to English
curl http://ESP32-Diagnostic.local/api/set-language?lang=en

# Switch to French
curl http://ESP32-Diagnostic.local/api/set-language?lang=fr

# Get current translations
curl http://ESP32-Diagnostic.local/api/get-translations
```

### Translated Texts

**All elements are translated:**
- Section titles
- Labels and descriptions
- Status messages (OK, FAIL, Testing...)
- Action buttons
- Error messages
- Signal qualities (Excellent, Good, Weak...)
- Reset reasons

## 🔧 Pin Configuration

### Default Pins

**I2C (OLED) - Modifiable via web:**
| Model    | SDA | SCL |
|----------|-----|-----|
| ESP32    | 21  | 22  |
| ESP32-S2 | 8   | 9   |
| ESP32-S3 | 8   | 9   |
| ESP32-C3 | 5   | 6   |

**Built-in LED (auto-detected):**
- ESP32-S3: GPIO 2
- ESP32-C3: GPIO 8
- ESP32: GPIO 2

**NeoPixel (auto-detected):**
- ESP32-S3: GPIO 48
- ESP32-C3: GPIO 8

## 📖 Usage

### Language Selection

**FR/EN buttons** in the top right of the header allow instant language switching.

### Available Tabs

1. **Overview** - Complete system information
2. **LEDs** - Built-in LED and NeoPixel tests
3. **Screens** - OLED tests with manual step controls
4. **Advanced Tests** - ADC, Touch, PWM, SPI, Partitions, Stress Test
5. **GPIO** - Automatic all GPIO testing
6. **WiFi** - WiFi network scanner
7. **Performance** - CPU/Memory benchmarks
8. **Export** - TXT, JSON, CSV, Printable version

## 🛠️ Customization

### Change Default Language

In `languages.h`:
```cpp
Language currentLanguage = LANG_EN; // For English by default
```

### Add a New Language

1. Define the enum in `languages.h`:
```cpp
enum Language {
  LANG_FR = 0,
  LANG_EN = 1,
  LANG_ES = 2  // Spanish
};
```

2. Create the translation structure:
```cpp
const Translations LANG_ES_TEXTS = {
  "Diagnóstico ESP32 Completo",
  "v",
  // ... all translations
};
```

3. Modify the `T()` function:
```cpp
const Translations& T() {
  switch(currentLanguage) {
    case LANG_FR: return LANG_FR_TEXTS;
    case LANG_EN: return LANG_EN_TEXTS;
    case LANG_ES: return LANG_ES_TEXTS;
    default: return LANG_FR_TEXTS;
  }
}
```

## 📊 REST API

**New routes in v2.4:**
```
GET /api/set-language?lang=fr    → Change language (fr/en)
GET /api/get-translations        → Get JSON translations
```

**Existing routes:**
```
GET /                            → Web interface
GET /api/test-gpio               → GPIO test
GET /api/wifi-scan               → WiFi scanner
GET /api/benchmark               → Benchmarks
GET /export/txt                  → TXT export
GET /export/json                 → JSON export
GET /export/csv                  → CSV export
```

## 🐛 Troubleshooting

### Language switching doesn't work

1. Verify that `languages.h` is in the folder
2. Recompile completely (Clean then Upload)
3. Clear browser cache (Ctrl+F5)

### Untranslated texts

If some texts remain in French:
- Check the `Translations` structure in `languages.h`
- Ensure all keys are defined
- Reload the page

### Compilation error "T() not found"

- Verify that `#include "languages.h"` is present
- The `languages.h` file must be in the same folder as the `.ino`

## 📈 Performance

- **Startup time**: ~5s
- **Web page generation**: ~200ms
- **Language switching**: <100ms (no reload)
- **Heap memory used**: ~250KB
- **Multilingual overhead**: ~15KB Flash

## 🔒 Security

⚠️ Local/development use only.

**Never share `config.h` with your WiFi credentials.**

## 📁 Project Structure v2.6.3

```
ESP32-Diagnostic/
├── ESP32-Diagnostic.ino              (main code)
├── languages.h                       (translation system - NEW)
├── config.h.example                  (template)
├── config.h                          (your credentials - gitignore)
├── README.md                         (this file)
├── README.fr.md                      (French version of README)
└── .gitignore                        (excludes config.h)
```

## 🔄 Changelog

### v2.6.3 (2025) - CHANGEMENT DE LANGUE IMMÉDIAT
- Replaced blocking popups with inline status banners on every test panel
- Reserved fixed-height feedback areas to prevent layout jumps during updates
- Highlighted that a ❌ on GPIO tests can signal a reserved or unconnected pin

### v2.5.1 (2025) - PSRAM GUIDANCE
- ✅ **Clearer PSRAM hardware status** when the board supports external RAM but it is disabled in the Arduino IDE
- 📊 Web overview, printable export, TXT/CSV/JSON outputs and API now surface PSRAM support and activation hints

### v2.5 (2025) - MULTILINGUAL
- ✨ **Exporting files in multilingual** (FR/EN)

### v2.4 (2025) - MULTILINGUAL
- ✨ **Complete multilingual system** (FR/EN)
- ✨ New `languages.h` file with translations
- ✨ FR/EN buttons in interface
- ✨ REST API for language switching
- ✨ All v2.3 features preserved
- 📚 Updated README with multilingual guide

### v2.3 (2025)
- OLED 0.96" test with 10 animations
- Dynamic I2C pin configuration
- TXT/JSON/CSV/Print exports
- `server.sendContent()` support
- WiFi externalized to `config.h`

## 📝 License

Free to use, modify and distribute.

## 🤝 Contributing

**To add a language:**
1. Fork the project
2. Add your language in `languages.h`
3. Test all screens
4. Pull Request with screenshots

## 🎓 Credits

Developed for the ESP32 community.

**v2.6.3 - Changement de langue immédiat**
**v2.5.1 - Clearer PSRAM status guidance**
**v2.5 - Translation of export files**
**v2.4 - Multilingual system**
**v2.3 - Comprehensive base**

---

**Current version**: 2.6.3 Multilingual + instant language switching
**Last update**: October 2025  
**Available languages**: French (default), English  
**Support**: ESP32 Arduino Core 3.1.3+

🌐 **Access**: http://ESP32-Diagnostic.local  
🇫🇷🇬🇧 **Language switching**: FR/EN buttons in interface
```