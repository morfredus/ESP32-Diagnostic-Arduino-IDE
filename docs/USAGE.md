# Usage Guide (EN)

## 1. First boot
1. Open the Serial Monitor at 115200 baud immediately after flashing.
2. Wait for the firmware to connect to Wi-Fi. Successful connection prints the assigned IP and the URL `http://ESP32-Diagnostic.local`.
3. If Wi-Fi fails, the console displays error hints (wrong password, AP not found, etc.). Update `wifi-config.h` accordingly.

## 2. Accessing the dashboard
- Navigate to the printed IP address or to `http://ESP32-Diagnostic.local` (mDNS) from a device on the same network.
- The interface defaults to French. Use the FR/EN toggle in the top-right corner to switch language without reloading the page.

## 3. Web interface overview
The dashboard is organised into tabs:
- **Overview** – hardware summary, memory usage, firmware version, Bluetooth® status.
- **LEDs** – onboard LED controls plus NeoPixel/WS2812B patterns.
- **Screens** – OLED diagnostics with step-by-step animations and manual trigger buttons.
- **Advanced Tests** – CPU/memory benchmarks, stress tests and flash partition listing.
- **GPIO** – interactive pin tester with live status.
- **Wi-Fi** – detailed scan results (RSSI, channel, security, PHY, bandwidth).
- **Performance** – benchmark history and runtime metrics.
- **Export** – download TXT/JSON/CSV reports or open the printable report.
- Release 3.4.0 centralises JSON helper exports, restores translation-safe `String` handling, and removes the legacy touchpad diagnostic from the automated run.

The navigation bar collapses gracefully on mobile devices and preserves active tab state.

## 4. Bluetooth® Low Energy tools
- BLE advertising starts automatically when the firmware detects a supported board.
- A dedicated card in the Overview tab shows device name, connection state, and allows renaming.
- Invalid names are rejected with HTTP 400 responses to keep the GATT service consistent.

## 5. REST API
All endpoints return JSON unless stated otherwise:
- `GET /api/test-gpio` – run the GPIO sweep.
- `GET /api/wifi-scan` – launch a Wi-Fi scan.
- `GET /api/benchmark` – execute CPU and memory benchmarks.
- `GET /api/set-language?lang=fr|en` – switch UI language.
- `GET /api/get-translations` – download current translation catalog.
- `GET /api/oled-config?sda=<pin>&scl=<pin>` – reconfigure OLED I2C pins.
- `GET /api/oled-test` – run the complete OLED test suite (takes ~25 seconds).
- `GET /api/oled-message?message=TEXT` – display a custom message on the OLED.
- `GET /api/memory-details` – return flash/PSRAM/SRAM metrics plus fragmentation percentage for post-run analysis.
- `GET /export/txt`, `/export/json`, `/export/csv` – download reports in various formats.

## 6. Reports and logging
- Serial output mirrors key actions (Wi-Fi status, BLE state, test results).
- Exported reports include board information, memory breakdown, benchmark metrics, Wi-Fi scan, GPIO results, and OLED status.
- Keep JSON exports for machine parsing and TXT/CSV for manual analysis.
- Use the `/api/memory-details` endpoint when reports warn about fragmentation; the JSON response mirrors the guidance maintained through 3.4.0.

## 7. Best practices
- Re-run Wi-Fi scans after moving the device to a new location to refresh RSSI values.
- Stop NeoPixel animations before powering down external LEDs to avoid ghosting.
- When changing OLED pins, run `/api/oled-config` followed by `/api/oled-test` to validate wiring.

## 8. Post-release debugging checklist (3.4.0)
- Trigger `/api/memory-details` to verify the unified JSON helpers return the expected fragmentation and PSRAM flags.
- Switch languages via `/api/set-language` and the UI toggle to confirm the harmonised FR/EN labels remain consistent.
- Review the default diagnostic cycle to ensure the removed touchpad routine no longer appears in serial logs or exports.
