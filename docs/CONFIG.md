# Configuration (EN)

## Wi-Fi credentials
The firmware reads credentials from `wifi-config.h` located next to `ESP32-Diagnostic.ino`.

```cpp
// wifi-config.h (example)
#pragma once
#include <vector>
#include <WiFiMulti.h>

static WiFiMulti wifiMulti;

static const std::vector<std::pair<const char*, const char*>> WIFI_NETWORKS = {
    {"MySSID", "MyPassword"},
    {"BackupSSID", "BackupPassword"}
};
```

- Declare at least one SSID/password pair.
- The firmware iterates over the list until a connection succeeds.
- Avoid storing production credentials in version control.

## Language settings
- French is the default language.
- Use `/api/set-language?lang=en` or the UI toggle to switch to English.
- Translation strings live in `languages.h` inside the `Translations` structure. Add new languages by extending this file and exposing them in the UI.
- Release 3.2.0 keeps the accented "Non testé" placeholders aligned, preserves the explicit HTTP 200/400 responses on `/api/set-language`, and updates links to the bilingual changelog pair.

## OLED configuration
- Default pins: SDA=21, SCL=22 (ESP32 standard).
- Change wiring on the fly with `/api/oled-config?sda=<pin>&scl=<pin>`.
- After updating pins, the firmware automatically reinitialises the display and stores the mapping in RAM.

## Bluetooth® naming rules
- Names must be 3–31 characters, ASCII alphanumeric plus hyphen or underscore.
- Invalid input returns HTTP 400 without restarting advertising.
- Successful renaming persists until reboot.

## Advanced options
- Enable PSRAM in the Arduino IDE board menu for ESP32-S3 boards to unlock extended memory diagnostics.
- The firmware auto-detects chip variants and enables BLE support on compatible targets (ESP32, ESP32-S3, ESP32-C3, ESP32-C6, ESP32-H2).
- For debugging, open the Serial Monitor to observe Wi-Fi retries, BLE state transitions, and diagnostic progress.
