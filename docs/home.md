# ESP32 Diagnostic Suite – Wiki Home (v3.8.0)

## ⚠️ ARCHIVED REPOSITORY NOTICE

This repository **ESP32-Diagnostic-Arduino-IDE** is **archived but remains accessible**. Development has migrated to [ESP32-Diagnostic](https://github.com/morfredus/ESP32-Diagnostic) using PlatformIO under Visual Studio Code.

**Note:** Bluetooth functionality is **not supported** in the new PlatformIO version. If you need Bluetooth features, use this archived Arduino IDE version.

---

Welcome to the official wiki for firmware version **3.8.0**. This space centralises every guide, reference, and checklist required
to operate, customise, and maintain the ESP32 Diagnostic Suite across multilingual teams.

## Table of contents
- [Overview](OVERVIEW.md)
- [Feature matrix](FEATURE_MATRIX.md)
- [Diagnostic modules](DIAGNOSTIC_MODULES.md)
- [Web interface guide](WEB_INTERFACE.md)
- [REST API reference](API_REFERENCE.md)
- [Build & deploy](BUILD_AND_DEPLOY.md)
- [Security guidelines](SECURITY.md)
- [FAQ](FAQ.md)
- Core guides: [Installation](INSTALL.md), [Configuration](CONFIG.md), [Usage](USAGE.md), [Troubleshooting](TROUBLESHOOTING.md),
  [Architecture](ARCHITECTURE.md), [Contributing](../CONTRIBUTING.md)

## Release spotlight – 3.8.0
- OLED-equipped boards now display Wi-Fi association, DHCP, and completion phases at boot for immediate visibility.
- NimBLE builds compile cleanly on ESP32/ESP32-S3 targets and resume advertising gracefully after each scan.
- The documentation set highlights the Wi-Fi splash workflow, BLE safeguards, and updated toolchain requirements.

## Quick start
1. Follow the [installation guide](INSTALL.md) to set up the Arduino IDE, CLI, or PlatformIO toolchain.
2. Configure Wi-Fi credentials, localisation, and optional modules via [CONFIG.md](CONFIG.md).
3. Flash the firmware and open the embedded web dashboard at `http://esp32-diagnostic.local/`.
4. Run a quick diagnostic, review the [diagnostic modules](DIAGNOSTIC_MODULES.md), and export reports.

## Knowledge base map
| Topic | English | Français |
|-------|---------|----------|
| Wiki home | [home.md](home.md) | [home_FR.md](home_FR.md) |
| Overview | [OVERVIEW.md](OVERVIEW.md) | [OVERVIEW_FR.md](OVERVIEW_FR.md) |
| Feature matrix | [FEATURE_MATRIX.md](FEATURE_MATRIX.md) | [FEATURE_MATRIX_FR.md](FEATURE_MATRIX_FR.md) |
| Diagnostic modules | [DIAGNOSTIC_MODULES.md](DIAGNOSTIC_MODULES.md) | [DIAGNOSTIC_MODULES_FR.md](DIAGNOSTIC_MODULES_FR.md) |
| Web interface | [WEB_INTERFACE.md](WEB_INTERFACE.md) | [WEB_INTERFACE_FR.md](WEB_INTERFACE_FR.md) |
| REST API | [API_REFERENCE.md](API_REFERENCE.md) | [API_REFERENCE_FR.md](API_REFERENCE_FR.md) |
| Build & deploy | [BUILD_AND_DEPLOY.md](BUILD_AND_DEPLOY.md) | [BUILD_AND_DEPLOY_FR.md](BUILD_AND_DEPLOY_FR.md) |
| Security | [SECURITY.md](SECURITY.md) | [SECURITY_FR.md](SECURITY_FR.md) |
| FAQ | [FAQ.md](FAQ.md) | [FAQ_FR.md](FAQ_FR.md) |
| Installation | [INSTALL.md](INSTALL.md) | [INSTALL_FR.md](INSTALL_FR.md) |
| Configuration | [CONFIG.md](CONFIG.md) | [CONFIG_FR.md](CONFIG_FR.md) |
| Usage | [USAGE.md](USAGE.md) | [USAGE_FR.md](USAGE_FR.md) |
| Troubleshooting | [TROUBLESHOOTING.md](TROUBLESHOOTING.md) | [TROUBLESHOOTING_FR.md](TROUBLESHOOTING_FR.md) |
| Architecture | [ARCHITECTURE.md](ARCHITECTURE.md) | [ARCHITECTURE_FR.md](ARCHITECTURE_FR.md) |
| Contributing | [../CONTRIBUTING.md](../CONTRIBUTING.md) | [../CONTRIBUTING_FR.md](../CONTRIBUTING_FR.md) |

## Support & escalation
- License: [MIT](../LICENSE)
- Issue tracker: [GitHub Issues](https://github.com/morfredus/ESP32-Diagnostic-Arduino-IDE/issues)
- Release history: [CHANGELOG.md](../CHANGELOG.md) / [CHANGELOG_FR.md](../CHANGELOG_FR.md)
- Community contributions: Fork the repository and submit pull requests following the [contribution guide](../CONTRIBUTING.md).

Happy diagnostics! If you maintain internal SOPs, link back to this wiki to ensure future teams stay aligned with the 3.8.0 release.
