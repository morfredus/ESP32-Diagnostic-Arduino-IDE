# Changelog

All notable changes to ESP32 Diagnostic Suite are documented here. This project follows [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Version 3.2.22-dev] - 2025-10-27
### Added
- None.

### Fixed
- Removed redundant “NEW FEATURE” banner comments to avoid confusing maintenance efforts.

### Improved
- Consolidated the legacy "Version de dev" notes into a firmware constant so internal history remains accessible without banner duplication.
- Synced the firmware header comment and `DIAGNOSTIC_VERSION` macro at 3.2.22-dev and refreshed documentation to reflect the streamlined maintenance focus.

---

## [Version 3.2.0] - 2025-10-29
### Added
- Documented how to query the `/api/memory-details` endpoint and interpret fragmentation warnings from the latest diagnostics run.
- Captured the Bluetooth® and Wi-Fi debugging checklist from the post-release validation pass directly inside the usage and troubleshooting guides.

### Fixed
- Replaced remaining 3.1.19 references across READMEs and setup guides so the banner, guides, and bilingual changelog remain synchronized at 3.2.0.
- Clarified configuration guidance around language switching to avoid stale instructions when verifying `/api/set-language` responses.

### Improved
- Expanded installation, configuration, and usage manuals with concrete verification steps for the refreshed diagnostics (memory exports, BLE state widgets, Wi-Fi scan metadata).
- Strengthened the contributing guide with reminders to keep FR/EN documentation aligned during maintenance updates.

---

## [Version 3.1.19] - 2025-10-28
### Added
- None.

### Fixed
- Updated every guide to reference the bilingual changelog pair (`CHANGELOG.md` and `CHANGELOG_FR.md`) so both languages stay aligned.

### Improved
- Split the changelog into dedicated English and French files to make maintenance updates easier for each audience.
- Refreshed the version banner, `DIAGNOSTIC_VERSION`, and release highlights to reference 3.1.19 across the documentation set.

---

## [Version 3.1.18] - 2025-10-27
### Added
- None.

### Fixed
- Validated `/api/set-language` so French/English requests immediately return HTTP 200 while unsupported codes now receive a clear HTTP 400 response.
- Synchronized serial logs and exports with the new language-switch JSON responses.

### Improved
- Preallocated the `jsonEscape` buffer to reduce allocations while generating exports.
- Explicitly included `<cstring>` to guarantee C string helpers remain available on recent Arduino toolchains.
- Aligned the FR/EN documentation set (README, installation, configuration, usage, architecture, contributing, troubleshooting guides) with the 3.1.18 maintenance pass.

---

## [Version 3.1.18] - 2025-10-27
### Ajouts
- Néant.

### Corrections
- Validation de `/api/set-language` pour renvoyer immédiatement HTTP 200 sur `fr`/`en` et HTTP 400 sur les codes non pris en charge, supprimant les réponses ambiguës.
- Synchronisation des journaux série et exports avec les nouvelles réponses JSON du changement de langue.

### Améliorations
- Réservation anticipée du tampon de `jsonEscape` afin de réduire les allocations pendant la génération d'exports.
- Inclusion explicite de `<cstring>` pour assurer la compatibilité des fonctions de chaîne C sur les toolchains Arduino récentes.
- Documentation FR/EN alignée sur la maintenance 3.1.18 (README, guides d'installation, configuration, utilisation, architecture, contribution et dépannage).

---

## [Version 3.1.16] - 2025-10-27
### Added
- Unified sticky banner showing version, Wi-Fi/Bluetooth status, and a quick-access link from the web dashboard.
- Bluetooth® commands (enable, rename, reset) exposed both in the interface and through dedicated REST endpoints.

### Fixed
- Repositioned tab navigation event delegation to restore the selection after dynamic reloads.
- Refreshed translations on tabs and dynamic elements after a language switch to remove inconsistent labels.
- Restored the “Not tested” label for additional diagnostics across the interface, API, and exports.

### Improved
- Enriched Wi-Fi scan responses (BSSID, band, channel width, PHY mode) to simplify RF analysis.
- Extended `htmlEscape` preallocation to limit reallocations during HTML streaming and export generation.
- Synchronized the header comment and `DIAGNOSTIC_VERSION` at 3.1.16 to align logs and generated files.

---

## [Version 3.1.15-maint] - 2025-10-26
### Added
- None.

### Fixed
- Standardized the “Not tested” label for additional diagnostics (ADC, touch, PWM, stress) in the interface, exports, and API.
- Synchronized the header comment and `DIAGNOSTIC_VERSION` at 3.1.15-maint so logs and exported files show the correct number.

### Improved
- Extended the `htmlEscape` preallocation to reduce reallocations while escaping generated HTML strings.
- Updated the FR/EN documentation (README, installation, configuration, usage, architecture, contributing, troubleshooting guides) to reflect the 3.1.15-maint maintenance and checks.

---

## [Version 3.1.14-maint] - 2025-10-26
### Added
- None.

### Fixed
- Restored “Not tested” status labels for the additional diagnostics to keep the French interface consistent.
- Aligned the version banner comment and `DIAGNOSTIC_VERSION` with revision 3.1.14-maint to reflect the active maintenance.

### Improved
- Increased the memory reserved by `htmlEscape` to limit reallocations while escaping generated strings.

---

## [3.1.1] - 2025-10-26
### Fixed
- Removed obsolete comments and internal markers to clarify firmware maintenance status.

### Documentation
- Updated the FR/EN guides, exports, and constants to reflect the move to version 3.1.1.

---

## [3.1.0] - 2025-10-24
### Highlights
- Automatic enablement of the Bluetooth® Low Energy service with native advertising on compatible targets (ESP32, S3, C3, C6, H2).
- Web dashboard enriched with a BLE card showing status, device name, and recent pairing logs.
- Fully rewritten FR/EN document sets covering installation, configuration, usage, architecture, troubleshooting, and contribution.

### Features
- Default GATT service with dynamic device renaming via the web interface or serial console.
- New contextual Wi-Fi status messages (association, DHCP, authentication, captive portal) displayed in the top banner and REST API.
- Relaxed initial support for the latest ESP32 SoC revisions (S3/C3/C6/H2) within the BLE and Wi-Fi modules.

### Interface & API
- Hardened tab navigation through JavaScript event delegation to avoid lockups after partial refresh.
- Responsive top menu grouping status indicators on a single line for screens below 768 px.
- TXT/JSON/CSV exports and print preview synchronized with the new BLE and Wi-Fi states.

### Documentation
- Consolidated guides inside `docs/` with cross-links to English and French versions.
- Added a detailed contributing guide (Git workflow, validation, formatting) and BLE/Wi-Fi troubleshooting procedures.

### Fixed
- Harmonized BLE messages across the serial console, `/api/status`, and the web interface.
- Cleaned the dynamic translation JSON to remove orphan keys and casing inconsistencies.
- Clarified active tab detection in the JavaScript bundle, removing duplicate listeners.

---

## [2.6.0] - 2025-10-15
### Features
- Manual buttons on the web interface plus REST endpoints to control each OLED diagnostic animation individually.
- Ability to start and stop display sequences directly from the serial console.

### Improved
- Simplified the OLED I²C reconfiguration flow: select SDA/SCL pins and speed directly from the interface.
- Refreshed the translation pack (FR/EN) for all new OLED labels and runtime states.

### Fixed
- Fully removed TFT support (firmware, Arduino dependencies, UI fragments), shrinking the binary and eliminating compilation warnings.

---

## [2.5.1] - 2025-10-10
### Fixed
- Clarified PSRAM statuses on the dashboard, exports, and printable reports (detected value, frequency, octal mode).
- Adjusted PSRAM detection order to avoid false negatives on ESP32-WROVER boards.

### Improved
- Highlighted PSRAM-compatible boards with reminders to enable the option in the Arduino IDE and installation docs.

---

## [2.5.0] - 2025-10-08
### Features
- Fully translated TXT/JSON/CSV exports (FR/EN) including ISO 8601 timestamps and build numbers.
- Printable preview aligned with the selected language and the new diagnostic blocks.

### Improved
- Enriched export filenames with version number, date, and detected board.
- Revised the REST export workflow to shorten generation times and harmonize column titles.

### Fixed
- Harmonized date formats (FR/EN localization) in exports and the printable view.
- Added missing translations on download buttons and section labels.

---

## [2.4.0] - 2025-10-07
### Features
- Full FR/EN multilingual interface with hot switching without page reload.
- `/api/set-language` and `/api/get-translations` endpoints enabling external clients to control the language.
- `languages.h` file centralizing more than 150 translated strings for the interface, exports, and serial console.

### Improved
- All texts (web, API, exports, logs) now rely on the centralized translation catalog.
- Added a visual indicator for the active language and persisted the preference in the browser's local storage.

---

## [2.3.0] - 2025-10-06
### Features
- Suite of 10 OLED 0.96" I²C tests (contrast toggles, inversion, scroll, custom frames) with explanatory messages.
- Dynamic SDA/SCL pin reconfiguration through the web interface and API to simplify rewiring.

### Improved
- Automatically detects the display at address 0x3C and reruns tests after connection.
- Added a contrast calibration module to optimise OLEDs based on supply voltage.

---
