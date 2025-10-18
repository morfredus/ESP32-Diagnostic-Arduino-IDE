<!-- 
Available in multiple languages:
- English: README.md
- Français: README.fr.md
-->
# ESP32 Complete Diagnostic v4.0.14

[🇫🇷 Version française](README.fr.md) | 🇬🇧 English Version

> **Note:** This is the English version. Pour la version française, [cliquez ici](README.fr.md).

---

## 📋 Description

Comprehensive **multilingual** diagnostic tool for ESP32 microcontrollers, accessible via web interface. Automatically tests all hardware components, analyzes memory, scans peripherals and generates detailed reports.

**What's new in v4.0.14**: The firmware now auto-selects the ESP32 BLE Arduino or NimBLE backend when available, surfaces the active (or missing) BLE library across the wireless API/UI/exports, and keeps the Bluetooth card visible with localized stack guidance even when BLE is disabled.

## ✨ Features

### 🌍 Multilingual Interface (NEW in v2.4)
- **French** (default language)
- **English**
- **Dynamic switching**: FR/EN buttons in interface
- **Real-time update**: No reload required
- **Complete translations**: All texts, labels, messages and status

### Hardware Tests
- **Built-in LED** - Configurable GPIO with automatic conflict detection when the NeoPixel shares the pin
- **NeoPixel/WS2812B** - Multi-LED support with RGB effects
- **OLED 0.96" I2C Screen** - Complete test suite (full cycle, message, patterns) available individually even before auto-detection
- **GPIO** - Automatic testing of all available GPIO
- **ADC** - Reading all analog channels
- **Touch Pads** - Capacitive touch sensor testing
- **PWM/LEDC** - Pulse width modulation testing

### System Analysis
- **Detailed Memory** - Flash, PSRAM (OPI/QSPI), SRAM with integrity tests
- **Wireless Scanners** - Wi-Fi networks (RSSI, channel, security) and BLE devices (name, MAC, RSSI)
- **I2C Scan** - Automatic peripheral detection (0x01-0x7F)
- **SPI Scan** - Available SPI bus information
- **Flash Partitions** - Complete partition list
- **Benchmarks** - CPU and memory performance
- **Stress Test** - Memory allocation limit testing

### Web Interface
- **8 tabs** - Intuitive navigation (Overview, LEDs, Screens, Advanced Tests, GPIO, Wireless, Performance, Export)
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
- **ESP32 Arduino Core** 3.3.2 or higher

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
3. **Screens** - OLED tests
4. **Advanced Tests** - ADC, Touch, PWM, SPI, Partitions, Stress Test
5. **GPIO** - Automatic all GPIO testing
6. **Wireless** - Wi-Fi network and BLE device scanners
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
GET /api/ble-scan                → BLE scanner
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

## 📁 Project Structure v4.0.14

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

### v4.0.14 (2025) - BLE BACKEND DETECTION & STACK MESSAGING
- 🔌 **Automatic BLE backend selection** between ESP32 BLE Arduino and NimBLE when native BLE headers are available.
- 📡 **Wireless diagnostics surface the active BLE library** (or explain its absence) across the UI, API, serial log, and exports.
- 🧾 **Documentation, firmware banner, and UI bumped to v4.0.14** with updated wireless guidance.

### v4.0.11 (2025) - BLUETOOTH STATUS EVERYWHERE
- 📶 **Wireless tab keeps the Bluetooth readiness card visible**, even when the firmware lacks native BLE, and spells out the reason why.
- 🧾 **Serial monitor plus TXT/JSON/CSV/print exports gain wireless summaries**, mirroring the UI with Wi-Fi/BLE capability details.
- 🛠️ **Firmware, docs, and UI bumped to v4.0.11**, disabling the BLE scan button automatically when the stack is absent.

### v4.0.9 (2025) - PSRAM STATUS & WIRELESS SUMMARY
- 🧠 **Overview & exports now surface PSRAM support/mode**, including IDE enablement hints that mirror the serial diagnostic output.
- 📶 **Wireless tab opens with a Wi-Fi/BLE readiness summary**, revealing connection details and whether BLE is compiled in before launching scans.
- 🔗 **`/api/overview` and `/api/memory-details` share PSRAM capability metadata**, enabling external tools and the dynamic UI to stay in sync.

### v4.0.4 (2025) - LED CONFLICT GUARD + LOCALIZED API
- 💡 **Built-in LED test now aborts when the NeoPixel shares the GPIO** and reports the conflict instead of flashing the strip
- 🌐 **All LED/OLED API responses are translated** (including missing pattern, invalid pins, direct messages)
- 🖥️ **Legacy web interface mirrors the OLED quick actions** introduced in the dynamic page

### v4.0.3 (2025) - LED SAFEGUARD + OLED SHORTCUTS
- 💡 **Built-in LED test now detects NeoPixel pin conflicts** and switches to a safe "basic mode" when needed
- 🖥️ **OLED quick actions** trigger each demo individually without running the 25-second full test

### v4.0.2 (2025) - TRANSLATION CATALOG FIX
- 🧰 **Fixed translation catalog export** to avoid Arduino `String::endsWith` compilation errors on Core 3.3.2
- 🌐 **Dynamic language switching preserved** with complete translation data delivered to the browser

### v4.0.1 (2025) - TFT API ALIGNMENT
- 🛠️ **TFT pattern endpoints now acknowledge the disabled hardware state** with translated responses
- 🔄 **TFT status strings refresh automatically** when changing the interface language

### v4.0.0 (2025) - TFT-FREE RELEASE
- 🧹 **Removed built-in TFT_eSPI dependency** while keeping the TFT tab/API accessible
- 📡 **Wi-Fi scanner** now reports every ESP-IDF security mode and handles scan failures gracefully
- 🧮 **CPU benchmark MFLOPS** converted to use real elapsed time and operation count
- 🌍 **Memory/I2C messages** now rely on localized strings instead of hard-coded French fallbacks

### v3.0.0 (2025) - OFFICIAL RELEASE
- ✅ **Arduino Core 3.3.2 compatibility validation**
- 📘 **Updated bilingual documentation** (FR/EN)
- ✨ Multilingual exports retained (TXT/JSON/CSV/Print)

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

**v4.0.14 - BLE backend detection and stack visibility upgrades**
**v4.0.11 - Bluetooth readiness everywhere**
**v4.0.9 - PSRAM insights with wireless readiness summary**
**v4.0.2 - Translation catalog fix + localization refresh**
**v3.0.0 - Official Core 3.3.2 support**
**v2.4 - Multilingual system**
**v2.3 - Comprehensive base**

---

**Current version**: 4.0.14 (multilingual)
**Last update**: October 2025
**Available languages**: French (default), English
**Support**: ESP32 Arduino Core 3.3.2+

🌐 **Access**: http://ESP32-Diagnostic.local  
🇫🇷🇬🇧 **Language switching**: FR/EN buttons in interface
```
