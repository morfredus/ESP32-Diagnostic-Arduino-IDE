# Firmware Architecture (EN)

## High-level overview
The firmware is a single Arduino sketch (`ESP32-Diagnostic.ino`) that orchestrates diagnostics, serves a web interface, and exposes REST endpoints. Supporting headers encapsulate translations, HTML templates, and configuration helpers.

```
ESP32-Diagnostic/
├── ESP32-Diagnostic.ino      // main loop, task scheduler, HTTP handlers
├── languages.h               // translation tables and helpers
├── web_interface.h           // HTML/CSS/JS assets stored as PROGMEM strings
├── wifi-config-example.h     // Wi-Fi credential template
└── docs/...                  // documentation (EN/FR)
```

## Runtime flow
1. **Boot** – initialise Serial, load Wi-Fi credentials, start WiFiMulti connection.
2. **Services** – mount mDNS, start the HTTP server, and enable BLE advertising/service when supported.
3. **Diagnostics** – expose REST actions that trigger tests (GPIO sweep, Wi-Fi scan, OLED routines, benchmarks, exports).
4. **Web UI** – serve static HTML/JS from `web_interface.h`; dynamic data is injected via REST calls returning JSON.
5. **Translations** – `languages.h` stores both FR and EN strings. The client fetches translations via `/api/get-translations` and swaps text dynamically.

## Key modules
- **Wi-Fi stack** – uses `WiFiMulti` to iterate over the configured networks until one connects.
- **BLE manager** – detects chip capabilities, configures the GATT service, and exposes status updates to the web UI.
- **Diagnostics engine** – performs hardware tests (GPIO, ADC, touch, OLED, benchmarks) and formats results for the dashboard and exports.
- **Export subsystem** – assembles TXT/JSON/CSV payloads for download and prints.
- Release 3.2.0 keeps the reinforced `/api/set-language` handler, maintains the preallocated `jsonEscape` buffer, and documents the bilingual changelog split for maintainers.

## Front-end structure
- HTML templates define each tab with semantic sections.
- A lightweight router handles tab switching via delegated click events.
- Responsive design ensures single-line headers and collapsible navigation on small screens.

## Data flow
- Client fetches `/api/get-translations` on load, then requests `/api/overview` (embedded in HTML) for initial stats.
- Test buttons trigger dedicated endpoints; responses update the UI using JavaScript.
- Exports stream from the microcontroller; no external storage is required.

## Extensibility guidelines
- Add new diagnostics by extending the scheduler in `ESP32-Diagnostic.ino` and exposing minimal REST endpoints.
- Keep translation keys consistent between FR and EN entries.
- Avoid blocking calls in the main loop; prefer asynchronous scheduling to maintain web responsiveness.
