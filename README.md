<!-- 
Available in multiple languages:
- English: README.md
- Français: README.fr.md
-->
# ESP32 Complete Diagnostic v2.8.15

[🇫🇷 Version française](README.fr.md) | 🇬🇧 English Version

> **Note:** This is the English version. Pour la version française, [cliquez ici](README.fr.md).

---

## 📋 Description

Comprehensive **multilingual** diagnostic tool for ESP32 microcontrollers, accessible via web interface. Automatically tests all hardware components, analyzes memory, scans peripherals and generates detailed reports.

**What's new in v2.8.15**: Replaces the WiFi timeout macro with a single `constexpr` declared in `config.h`, eliminating macro/constant conflicts during Arduino compilation while keeping the sequential WiFi loop.

## ✨ Features

### 🌍 Multilingual Interface (NEW in v2.4)
- **French** (default language)
- **English**
- **Dynamic switching**: FR/EN buttons in interface
- **Automatic refresh**: Immediate reload triggered after switching language
- **Complete translations**: All texts, labels, messages and status

### Hardware Tests
- **Built-in LED** - Configuration and testing with patterns (auto-applies the selected GPIO on test launch, blink, fade)
- **NeoPixel/WS2812B** - Multi-LED support with RGB effects (auto-applies GPIO/count before tests or animations)
- **OLED 0.96" I2C Screen** - 10 display tests including animations + manual step triggers (auto reapplies SDA/SCL before tests/messages)
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
- **Wireless status banner** (since v2.8.6, refined in v2.8.10) - Slim fixed header with live WiFi/Bluetooth indicators, always-visible IP address, STA/AP awareness, compile-time Bluetooth hints, safe driver detection and inline reminders for LED/NeoPixel/OLED tests
- **Inline status feedback** (since v2.8.7, refined in v2.8.9) - Contextual ⏳/✅/❌ messages for each action (LED effects, NeoPixel animations, OLED steps/messages, ADC, Touch, PWM, SPI, GPIO, WiFi scan, Bluetooth) with automatic config acknowledgements and accurate completion states

### Network Access
- **mDNS** - Access via http://ESP32-Diagnostic.local
- **Multi-WiFi** - Automatic connection with failover
- **REST API** - JSON endpoints for integration

## 📚 Documentation

- [`README.fr.md`](README.fr.md) – guide complet en français avec les mêmes sections que la version anglaise.
- [`USER_GUIDE.md`](USER_GUIDE.md) – setup & compilation checklist (EN) for Arduino IDE 2.x.
- [`USER_GUIDE.fr.md`](USER_GUIDE.fr.md) – mode d'emploi détaillé en français (pré-requis, bibliothèques, compilation Arduino IDE).
- [`CONFIG_REFERENCE.md`](CONFIG_REFERENCE.md) – English reference for every configurable parameter (`config.h`, `wifi-config.h`).
- [`CONFIG_REFERENCE.fr.md`](CONFIG_REFERENCE.fr.md) – référence française des paramètres personnalisables.

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

**Project structure (short view):**
```
ESP32-Diagnostic/
├── ESP32-Diagnostic.ino              (main firmware)
├── config.h                          (user-tunable hardware defaults – versioned)
├── wifi-config.example.h             (WiFi credentials template)
├── app_script.h                      (client-side JS generator)
├── languages.h                       (translations)
├── README.md / README.fr.md          (project overview)
├── USER_GUIDE.md / USER_GUIDE.fr.md
├── CONFIG_REFERENCE.md / CONFIG_REFERENCE.fr.md
└── .gitignore                        (ignores wifi-config.h)
```

### 3. Configure the project

1. **WiFi credentials (`wifi-config.h`)**
   - Copy `wifi-config.example.h` to `wifi-config.h` (kept out of Git).
   - Edit the list of networks:

```cpp
static const WiFiCredential WIFI_NETWORKS[] = {
  {"YourNetwork", "YourPassword"},
  {"BackupNetwork", "BackupPassword"}
};
```

   - You can add or remove entries; empty SSIDs are ignored automatically at runtime.
   - ✅ Keep the trailing comma at the end of each line (including the last one) so that uncommenting or duplicating entries never breaks the compilation.

2. **Hardware defaults (`config.h`)**
   - Review pin assignments (`CUSTOM_LED_PIN`, `DEFAULT_I2C_SDA/SCL`), NeoPixel count, `MDNS_HOSTNAME`, Bluetooth auto-test flag, and the wireless refresh interval `WIRELESS_STATUS_REFRESH_MS`.
   - Adjust the values to match your board before compiling.

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
- **Language switching**: <1s (auto reload to apply translations)
- **Heap memory used**: ~250KB
- **Multilingual overhead**: ~15KB Flash

## 🔒 Security

⚠️ Local/development use only.

**Never share `wifi-config.h` with your WiFi credentials.**

## 📁 Project Structure v2.8.15

```
ESP32-Diagnostic/
├── ESP32-Diagnostic.ino              (main code)
├── config.h                          (hardware defaults & tunable options)
├── wifi-config.example.h             (WiFi template → copy to wifi-config.h)
├── app_script.h                      (client-side JS generator – wireless banner, auto-config helpers)
├── languages.h                       (translation system)
├── README.md / README.fr.md          (project overview)
├── USER_GUIDE.md / USER_GUIDE.fr.md
├── CONFIG_REFERENCE.md / CONFIG_REFERENCE.fr.md
└── .gitignore                        (excludes wifi-config.h)
```

## 🔄 Changelog

### v2.8.15 (2025-10-20) - WIFI TIMEOUT CONSTEXPR CONSOLIDATION

- Swaps the `WIFI_CONNECT_TIMEOUT_MS` macro for a single `constexpr` inside `config.h`, removing duplicate-definition errors during Arduino builds.
- Updates the full documentation set (README, guides, configuration references, web banner) to reflect the v2.8.15 consolidation.

### v2.8.14 (2025-10-20) - WIFI TIMEOUT CONFIG SIMPLIFICATION

- Centralises the WiFi connection timeout in `config.h`, preventing duplicate constant declarations while preserving the sequential credential loop.
- Refreshes the bilingual documentation set (README, guides, configuration references) to describe the new configurable timeout.

### v2.8.12 (2025-10-20) - WIFI CREDENTIALS LOOP FIX

- Restores the WiFi connection loop so every entry defined in `wifi-config.h` is attempted in order (the firmware no longer reports “no WiFi network” when credentials exist).
- Keeps the bilingual documentation (README, guides, configuration references) aligned with version 2.8.12 and the updated setup instructions.

### v2.8.10 (2025-10-20) - STICKY NAVIGATION & IP BANNER

### v2.8.9 (2025-10-20) - CONTEXTUAL STATUS & RELIABLE BANNER

- Contextual status messages for every hardware action (LED/NeoPixel/OLED/ADC/Touch/PWM/SPI/GPIO/Bluetooth) and an explicit “Scanning…” message during WiFi surveys.
- Wireless banner fixes: STA/AP recognition, Bluetooth disabled-at-build detection and no more inverted indicators.
- `/api/wireless-info` now returns normalized states consumed by the web UI and documentation (README, guides) has been updated accordingly.

### v2.8.8 (2025-10-20) - WIRELESS INDICATORS & SAFE STARTUP
- Hardened WiFi boot sequence with optional SoftAP fallback to avoid queue assertions when no credentials are provided.
- Macro-driven `wifi-config.example.h` template simplifies adding multiple networks without syntax errors.
- Wireless banner retains STA/AP awareness, marks disabled Bluetooth stacks as “Indisponible” and clears stale metrics automatically.
- Updated bilingual documentation (README + user guides) reflecting the 20 Oct 2025 release.

### v2.8.7 (2025) - INLINE STATUS CLARITY
- Added consistent ⏳/✅/❌ status formatting for LED, NeoPixel, OLED, ADC, GPIO, WiFi scan and Bluetooth actions.
- Ensured "Test en cours..." messages persist until completion and reconfiguration feedback is aligned across automatic Config flows.

### v2.8.6 (2025) - WIRELESS STATUS & AUTO CONFIG
- Added a fixed header banner that displays WiFi/Bluetooth connectivity without relying on modal alerts.
- LED, NeoPixel and OLED tests now auto-apply the entered configuration before their first run while keeping the Config buttons for manual adjustments.

### v2.8.5 (2025) - CLIENT SCRIPT REWRITE
- Rebuilt the generated `/js/app.js` template to export all handlers, fix the FR/EN toggle and restore working tab navigation.
- Updated documentation banners and UI headers to advertise version 2.8.5 and the restored UI behaviour.

### v2.8.3 (2025) - API RESTORATION & BLUETOOTH FEEDBACK
- Fixed the escaped JavaScript bundle and reinstated all REST handlers (WiFi, LEDs, NeoPixel, OLED, tests, exports).
- Wireless diagnostics JSON now exposes Bluetooth availability/status with contextual hints in both languages.

### v2.8.1 (2025) - ESP32 ARDUINO CORE 3.3.2 & WIFI AVANCÉ
- Compatibilité validée avec l'Arduino Core ESP32 3.3.2 et affichage de la version du core dans l'interface, les exports et l'API.
- Nouvelles métriques WiFi (mode, veille, bande, plan de bande, puissance TX, hostname) visibles sur l'onglet Sans fil, la vue d'ensemble et tous les formats d'export.

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
- WiFi externalized to `wifi-config.h`

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

**Current version**: 2.8.15 ESP32 Arduino Core 3.3.2 + WiFi timeout `constexpr` consolidation, multi-network WiFi loop and sticky wireless banner
**Last update**: 20 October 2025
**Available languages**: French (default), English  
**Support**: ESP32 Arduino Core 3.3.2+

🌐 **Access**: http://ESP32-Diagnostic.local  
🇫🇷🇬🇧 **Language switching**: FR/EN buttons in interface
```