# ESP32 Diagnostic Suite – Overview (v3.8.0)

The ESP32 Diagnostic Suite delivers a production-ready toolkit for validating ESP32-based boards before deployment. Version 3.8.0 emphasises Wi-Fi visibility on OLED hardware, cross-stack BLE compatibility, and refreshed documentation for the latest toolchains.

## Mission statement
- Provide a consistent methodology to qualify ESP32, ESP32-S2, ESP32-S3, ESP32-C3, ESP32-C6, and ESP32-H2 boards.
- Offer a web-first control surface for laboratory and field diagnostics with minimal setup overhead.
- Keep firmware, documentation, and exported reports aligned so QA teams can trace issues quickly.

## Feature summary
| Area | Highlights |
|------|------------|
| Web dashboard | Responsive HTML interface served directly by the ESP32 with live status badges, activity log, and export actions. |
| Automated checks | Schedulers covering connectivity, memory, peripheral buses, display, and LED pipelines. |
| Reporting | Downloadable TXT/JSON/CSV bundles, printable view, and REST API hooks for CI integration. |
| Localisation | Runtime FR/EN switching at both the firmware (serial output) and interface levels. |
| Extensibility | Modular test registry, event bus for user-defined callbacks, and optional OLED/NeoPixel helpers. |

## Release 3.8.0 focus
- Boot-time Wi-Fi splash on OLED-equipped boards to expose association, DHCP, and completion states without opening Serial.
- Hardened NimBLE builds with guarded descriptors, aligned callbacks, and safer scan handling across ESP32/ESP32-S3 devices.
- Documentation updates covering the splash workflow, BLE safeguards, and the move to Arduino Core 3.3.3.

## Wiki map
Use this map to jump directly to the right knowledge category:

- [Home](home.md) – entry point, release highlights, and high-level goals.
- [Installation](INSTALL.md) – environment preparation, toolchain requirements, USB drivers, and flashing options.
- [Configuration](CONFIG.md) – Wi-Fi credentials, localisation defaults, diagnostic scheduling, and logging preferences.
- [Usage](USAGE.md) – day-to-day operator workflows, export formats, and interpreting the dashboard.
- [Architecture](ARCHITECTURE.md) – firmware layers, task scheduling, storage layout, and interface templating.
- [Troubleshooting](TROUBLESHOOTING.md) – recovery procedures, error code matrix, and safe reset scenarios.
- [Contributing](CONTRIBUTING.md) – coding standards, review process, and test expectations.
- [Feature matrix](FEATURE_MATRIX.md) – hardware coverage per board family and optional peripherals.
- [Diagnostic modules](DIAGNOSTIC_MODULES.md) – deep dive into each automated test and the metrics it emits.
- [Web interface guide](WEB_INTERFACE.md) – UI anatomy, component states, and accessibility guidelines.
- [REST API reference](API_REFERENCE.md) – endpoints, payloads, rate limits, and automation scenarios.
- [Build & deploy](BUILD_AND_DEPLOY.md) – firmware compilation, QA gating, and release checklist.
- [Security guidelines](SECURITY.md) – Wi-Fi hardening, credential rotation, and OTA update safety.
- [FAQ](FAQ.md) – quick answers to the most common integration, support, and deployment questions.

## Terminology
- **Diagnostic cycle** – the ordered execution of all enabled modules with aggregated results stored in RAM until export.
- **Snapshot report** – a frozen copy of the latest diagnostic cycle exported via web UI, REST API, or serial command.
- **Controller profile** – pre-defined configuration bundle (board type, interface options) loaded at boot.
- **Maintenance release** – a version delivering documentation and stability improvements without altering the feature surface.

## Related resources
- [CHANGELOG.md](../CHANGELOG.md) – chronological list of fixes and enhancements.
- [ESP-IDF compatibility table](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/) for comparing Arduino Core expectations.
- [GitHub issues](https://github.com/ESP32-Diagnostic/ESP32-Diagnostic/issues) – track bugs or request enhancements.
