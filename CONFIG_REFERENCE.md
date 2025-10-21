# ESP32-Diagnostic Configuration Reference (v2.8.11)

This document centralises every tunable option exposed by the firmware. Review it before compiling a customised build.

## 1. `config.h` – Hardware defaults (versioned)

`config.h` ships with the repository and is intended to be edited. Each value can be tailored to your test bench.

| Symbol | Default | Description |
| --- | --- | --- |
| `MDNS_HOSTNAME` | `"esp32-diagnostic"` | Hostname advertised on the local network (`http://<hostname>.local`). |
| `CUSTOM_LED_PIN` | `-1` | Forces the GPIO used for the on-board LED. Leave `-1` to auto-detect. |
| `CUSTOM_LED_COUNT` | `1` | Default number of LEDs for the NeoPixel strip. |
| `ENABLE_I2C_SCAN` | `true` | Enables the automatic I2C bus scan during startup. |
| `DEFAULT_I2C_SDA` | `21` | SDA pin applied at boot and whenever the OLED test is reset. |
| `DEFAULT_I2C_SCL` | `20` | SCL pin applied at boot and whenever the OLED test is reset. |
| `SCREEN_WIDTH` | `128` | Logical width of the OLED display (pixels). |
| `SCREEN_HEIGHT` | `64` | Logical height of the OLED display (pixels). |
| `OLED_RESET` | `-1` | Reset pin for the OLED controller (`-1` when not wired). |
| `SCREEN_ADDRESS` | `0x3C` | Default I2C address for the OLED screen. |
| `ENABLE_BLUETOOTH_AUTOTEST` | `true` | Displays the Bluetooth self-test button when the stack is enabled in the build. |
| `WIRELESS_STATUS_REFRESH_MS` | `3000` | Interval (ms) between automatic refreshes of the wireless banner. |

> Tip: `config.h` is versioned. Keep your custom values in place; they will be preserved when pulling updates.

## 2. `wifi-config.h` – Credentials (ignored by Git)

WiFi credentials are stored separately to avoid leaking secrets:

1. Copy `wifi-config.example.h` to `wifi-config.h` (same folder as the sketch).
2. Inside the file, uncomment or duplicate the `WIFI_CREDENTIAL` lines within `WIFI_CREDENTIALS_LIST`. Empty SSIDs are ignored.

```cpp
#define WIFI_CREDENTIALS_LIST \
  WIFI_CREDENTIAL("MyNetwork", "SuperSecret") \
  /* WIFI_CREDENTIAL("LabBackup", "FallbackPass") */
```

> Comment out unused lines—the macro-based template manages braces and commas for you.

> `wifi-config.h` is listed in `.gitignore`. Never commit this file.

## 3. Checklist before compiling

- [ ] `wifi-config.h` exists and `WIFI_CREDENTIALS_LIST` exposes at least one `WIFI_CREDENTIAL` line.
- [ ] `config.h` values match your wiring (SDA/SCL, custom LED, NeoPixel count).
- [ ] Optional: disable `ENABLE_BLUETOOTH_AUTOTEST` if the Bluetooth stack is not compiled.
- [ ] Adjust `WIRELESS_STATUS_REFRESH_MS` if you need faster or slower banner updates.

Need more guidance? See [`USER_GUIDE.md`](USER_GUIDE.md) / [`USER_GUIDE.fr.md`](USER_GUIDE.fr.md) for a full step-by-step setup.
