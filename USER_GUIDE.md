# ESP32 Diagnostic – Setup & Compilation Guide (EN)

This checklist describes everything required to build and run **ESP32-Diagnostic v2.8.14** with the Arduino IDE 2.x toolchain.

## 1. Prerequisites

### Hardware
- ESP32 development board (ESP32, ESP32-S2, ESP32-S3 or ESP32-C3 tested)
- USB data cable
- Optional peripherals:
  - WS2812/NeoPixel LED strip
  - 0.96" I2C OLED display (SSD1306)
  - External I2C/SPI sensors to exercise the scanners

### Software
- **Arduino IDE** 2.0 or newer
- **ESP32 Arduino Core** 3.3.2 (install via *Boards Manager* → “esp32” by Espressif Systems)

### Required Arduino Libraries
Install the following through *Library Manager*:
1. **Adafruit BusIO**
2. **Adafruit GFX Library**
3. **Adafruit SSD1306**
4. **Adafruit NeoPixel**

The ESP32 core already ships with `WiFi`, `WebServer`, `ESPmDNS`, `Wire`, `SPI`, `esp_partition`, etc.

## 2. Project Files & WiFi credentials
1. Copy the folder `ESP32-Diagnostic/` into your Arduino sketches directory.
2. Duplicate `wifi-config.example.h` to `wifi-config.h` (ignored by Git) and uncomment/add `WIFI_CREDENTIAL` lines inside `WIFI_CREDENTIALS_LIST`:
   ```cpp
   #define WIFI_CREDENTIALS_LIST \
     WIFI_CREDENTIAL("MyNetwork", "Password") \
     /* WIFI_CREDENTIAL("Backup", "SparePassword") */
   ```
   Comment out lines you do not need; the template handles braces/commas for you.
3. Review `config.h` (versioned) and adjust the defaults for your hardware: `CUSTOM_LED_PIN`, `CUSTOM_LED_COUNT`, `DEFAULT_I2C_SDA/SCL`, `MDNS_HOSTNAME`, `ENABLE_BLUETOOTH_AUTOTEST`, `WIRELESS_STATUS_REFRESH_MS`, etc.

## 3. IDE configuration
1. Open **ESP32-Diagnostic.ino** inside Arduino IDE.
2. Select your board via *Tools → Board → esp32* (e.g. ESP32S3 Dev Module).
3. Pick the correct serial port under *Tools → Port*.
4. (Optional) Adjust PSRAM/Flash speed to match your hardware.
5. To run Bluetooth diagnostics, ensure the Bluetooth stack is enabled: *Tools → Bluetooth* = **Enabled** (available on boards that support BT).

## 4. Build & upload
1. Click **Verify** to compile. Compilation should succeed with Arduino core 3.3.2 and the libraries above.
2. Click **Upload** to flash the firmware.
3. Open the **Serial Monitor** at 115200 baud to follow boot logs and confirm the assigned IP address.

## 5. First launch
1. Connect the ESP32 to WiFi (or its SoftAP) – the Serial Monitor prints the IP/mDNS hostname.
2. Open a browser and visit `http://<board-ip>/` or `http://esp32-diagnostic.local/`.
3. The home page displays the wireless banner (green/orange/red). If the Bluetooth dot stays orange with a hint, enable BT in the compiler options.

## 6. Optional hardware tests
- **NeoPixel**: wire DIN to the selected GPIO and configure the pin/count before launching a pattern.
- **OLED**: connect SDA/SCL to the configured pins (default 21/20 on ESP32-S3). Use “Configurer” before running the full test.
- **GPIO/ADC**: reference the on-board pinout; failing GPIO tests may simply mean the pin is reserved (see on-screen reminder).

## 7. Troubleshooting
| Issue | Suggested fix |
| --- | --- |
| `esp_bt_main.h: No such file or directory` | Enable Bluetooth in *Tools → Bluetooth* or ignore the test (banner will show “Indisponible”). |
| Wireless banner shows the wrong dot colour | Reload the page or click “Sans fil”; v2.8.14 keeps WiFi/Bluetooth indicators in sync, retries every configured SSID sequentially and surfaces the IP badge plus Bluetooth status hints. |
| SoftAP only but WiFi dot stays red | Verify SSID/password in `wifi-config.h` or rely on SoftAP (orange dot indicates AP-only mode). |
| Translations missing | Ensure `languages.h` is present beside the `.ino` and re-upload. |

## 8. Related documentation
- [`README.md`](README.md) – detailed English documentation & changelog.
- [`README.fr.md`](README.fr.md) – French documentation.
- [`USER_GUIDE.fr.md`](USER_GUIDE.fr.md) – French edition of this setup guide.
- [`CONFIG_REFERENCE.md`](CONFIG_REFERENCE.md) / [`CONFIG_REFERENCE.fr.md`](CONFIG_REFERENCE.fr.md) – exhaustive list of configurable parameters.

Happy debugging! ✅
