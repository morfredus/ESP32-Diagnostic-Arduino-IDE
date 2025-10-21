# Changelog

All notable changes to the ESP32 Complete Diagnostic Tool will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [2.8.8] - 2025-10-20

### Changed
- Voyants WiFi/Bluetooth fiabilisés : distinction STA/AP, état « Indisponible » cohérent et purge des valeurs obsolètes dans le bandeau sans fil.
- Nouveau modèle `wifi-config.example.h` basé sur des macros `WIFI_CREDENTIAL` pour ajouter des réseaux sans se soucier des virgules ou accolades.
- Documentation FR/EN (README et guides) réalignée au 20 octobre 2025 avec rappel SoftAP optionnel.

### Fixed
- Démarrage WiFi sécurisé : séquence unique de connexion avec repli SoftAP, suppression des assertions `queue.c:1709` quand aucun réseau n'est configuré.

---

## [2.8.7] - 2025-10-19

### Changed
- Harmonisation des statuts en ligne avec préfixes ⏳/✅/❌ pour tous les tests LED, NeoPixel, OLED, ADC, GPIO, scans WiFi et Bluetooth, avec rappels de reconfiguration cohérents.

### Fixed
- Les messages des tests lumineux et OLED affichent désormais "Test en cours..." jusqu'à la fin réelle et ne marquent plus "Terminé" prématurément, y compris pour le test Bluetooth.

---

## [2.8.6] - 2025-10-18

### Added
- Bannière fixe en haut de page affichant l'état WiFi/Bluetooth en temps réel sans recourir à des pop-ups bloquantes.

### Changed
- Les tests LED intégrée, NeoPixel et OLED appliquent désormais automatiquement la configuration saisie avant leur première exécution tout en conservant le bouton **Config** pour valider un changement manuel.

---

## [2.8.5] - 2025-10-17

### Fixed
- Réécriture complète du générateur `/js/app.js` pour supprimer les erreurs JavaScript, rétablir les onglets interactifs et la bascule FR/EN sans dépendre d'un script inline.

### Changed
- Boutons de navigation et de langue convertis en attributs `data-*` avec initialisation centralisée côté client.

---

## [2.8.4] - 2025-10-16

### Fixed
- Remplacement des attributs HTML du script `/js/app.js` par des apostrophes échappées pour supprimer les erreurs `operator""info` et `exponent has no digits` rencontrées lors de la compilation Arduino Core 3.3.2.

### Changed
- Documentation (README, bannière web) actualisée vers la version 2.8.4 pour refléter le correctif de génération du script client.

---

## [2.8.3] - 2025-10-15

### Fixed
- Correction de l'échappement du script `/js/app.js` afin d'éliminer les erreurs `stray '\'` lors de la compilation sous Arduino Core 3.3.2.
- Restauration des handlers REST (WiFi, I2C, LED intégrée, NeoPixel, OLED, tests matériels, exports) supprimés par inadvertance.

### Added
- Intégration de l'état Bluetooth (capacités, dernier test, message d'aide) dans la réponse `/api/wireless-info`.
- Test Bluetooth renforcé essayant d'initialiser/activer automatiquement le contrôleur quand il est disponible.

### Changed
- Échappement systématique des réponses JSON retournées par les endpoints de test pour éviter les caractères problématiques.

---

## [2.8.2] - 2025-10-14

### Added
- Nouveau fichier `app_script.h` contenant la fonction `buildAppScript()` pour garantir la livraison du script client `/js/app.js` sur chaque build.

### Changed
- Documentation synchronisée (README et bannière) avec la version 2.8.2 et clarification sur le rechargement automatique lors d'un changement de langue.

---

## [2.8.1] - 2025-10-13

### Fixed
- Rétablissement du chargement du JavaScript client (`/js/app.js`) pour que les onglets et le changement de langue fonctionnent sans erreur sous Arduino Core 3.3.2.

### Changed
- Externalisation du script de l'interface afin d'éviter les erreurs 404 et de faciliter les mises à jour dynamiques.

---

## [2.8.0] - 2025-10-26

### Added
- Compatibilité Arduino Core ESP32 3.3.2 avec affichage de la version du core dans l'interface, l'API et les exports.
- Nouvelles informations WiFi (mode, veille, bande, plan de bande, puissance TX, hostname) disponibles dans l'onglet Sans fil et dans les exports TXT/JSON/CSV.

### Changed
- Exports TXT/JSON/CSV enrichis et échappés avec `jsonEscape` pour éviter les caractères spéciaux non désirés.
- Interface Vue d'ensemble et version imprimable mises à jour pour refléter les métriques WiFi avancées.

---

## [2.7.1] - 2025-10-25

### Fixed
- Préprocesseur renforcé pour ignorer proprement le diagnostic Bluetooth quand les en-têtes `esp_bt*.h` sont absents, supprimant l'erreur de compilation tout en conservant les indications d'activation.

---

## [2.7.0] - 2025-10-24

### Added
- Nouvel onglet **Sans fil** réunissant les informations WiFi et Bluetooth avec zones d'état réservées.
- Test automatisé du contrôleur Bluetooth avec messages contextualisés et retour inline.
- Indication claire pour activer le Bluetooth lorsque le matériel est détecté mais désactivé à la compilation.

### Changed
- Présentation enrichie du statut WiFi (SSID, RSSI, IP, passerelle, DNS) sans recours aux pop-ups bloquantes.

---

## [2.6.3] - 2025-10-22

### Fixed
- Le changement de langue se réapplique désormais immédiatement sans rechargement manuel et conserve la sélection active.
- Harmonisation des libellés de boutons lumineux en français pour éviter les restes de termes anglais.

---

## [2.6.2] - 2025-10-21

### Fixed
- Le changement de broches OLED met désormais à jour l'interface sans rechargement complet, conservant l'onglet actif et le retour inline.
- Les scans I2C actualisent les compteurs et adresses détectées sans provoquer de redirection.

### Changed
- Stabilisation visuelle des zones de message (classes `status-field`/`status-live`) et rappel GPIO directement sous la grille de résultats.

---

## [2.6.1] - 2025-10-20

### Changed
- Remplacement des fenêtres popup par des messages intégrés aux sections de test.
- Réservation d'espaces d'information stables pour éviter les décalages de mise en page.

### Added
- Indication proactive rappelant qu'un GPIO en échec peut être simplement réservé ou non câblé.

### Fixed
- Harmonisation de la remontée d'erreurs sur les actions OLED et changement de langue via des toasts non bloquants.

---

## [2.6.0] - 2025-10-15

### Removed
- Dropped TFT SPI display support, associated libraries, and UI elements.

### Added
- Manual step buttons for each OLED diagnostic animation via the web interface and API.

### Changed
- Simplified OLED I2C reconfiguration workflow and refreshed translations.

---

## [2.5.1] - 2025-10-10

### Changed
- Clarified PSRAM status messaging across the web interface, printable report, and exports when hardware support exists but PSRAM is disabled in the Arduino IDE.

### Fixed
- Highlight PSRAM-compatible boards with activation guidance instead of reporting them as missing.

---

## [2.5] - 2025-10-08

### Added
- **Translated exports** in French and English
- Export language matches selected interface language
- Consistent terminology across all export formats

### Changed
- CSV, TXT, and JSON exports now fully translated
- Print version respects language selection
- Improved export file naming with version number

### Fixed
- Missing translations in export files
- Inconsistent date formats in exports

---

## [2.4] - 2025-10-07

### 🌐 Added - Multilingual Interface

#### Features
- **Complete French and English interface**
- **Dynamic language switching** without page reload
- **Translation system** via `languages.h`
- New API endpoints:
  - `/api/set-language?lang=[fr|en]` - Change interface language
  - `/api/get-translations` - Get all translations for current language
- **Persistent language selection** during session
- Translated sections:
  - Navigation menu
  - All page titles and headers
  - Button labels
  - Status messages
  - Memory labels
  - WiFi information
  - Test results
  - Export descriptions

#### Files Added
- `languages.h` - Complete translation system with 150+ strings
- Translation structures for French (default) and English

#### Changed
- Interface defaults to French
- All user-facing strings now use translation system
- API responses include language indicator

---

## [2.3] - 2025-10-06

### Added - OLED Display Support

#### Features
- **OLED 0.96" I2C display detection and testing**
- **Complete test suite** with 10 different tests:
  1. Welcome text with system info
  2. Large text display
  3. Multiple text sizes
  4. Geometric shapes (rectangles, circles, triangles)
  5. Horizontal lines pattern
  6. Diagonal lines pattern
  7. Moving square animation
  8. Progress bar with percentage
  9. Scrolling text animation
  10. Final success message
- **Configurable I2C pins** via web interface (SDA/SCL)
- **Custom message display** feature
- **Automatic detection** at 0x3C address
- **Re-detection capability** when changing pins

#### API Endpoints
- `/api/oled-config?sda=X&scl=Y` - Configure I2C pins
- `/api/oled-test` - Run complete test suite (25 seconds)
- `/api/oled-message?message=TEXT` - Display custom message

#### Technical Details
- Support for SSD1306 128x64 displays
- I2C bus sharing with other peripherals
- Pin conflict detection and management
- Memory-efficient display operations

---

## [2.2] - 2025-10-05

### Added - TFT Display Support

#### Features
- **TFT SPI display testing** (ST7789/ILI9341)
- **Color tests** - Red, Green, Blue, White sequence
- **Checkerboard pattern** test
- **Resolution detection** (width x height)
- **Individual test patterns** accessible via web interface

#### API Endpoints
- `/api/tft-test` - Complete TFT test (15 seconds)
- `/api/tft-pattern?pattern=[colors|checkerboard|clear]` - Individual patterns

#### Configuration
- Fixed SPI pins for TFT (CS, DC, RST, MOSI, SCLK, MISO)
- Support for 240x320 and custom resolutions
- TFT_eSPI library integration

---

## [2.1] - 2025-10-04

### Added - NeoPixel Support

#### Features
- **NeoPixel/WS2812 LED strip support**
- **Auto-detection** for ESP32-S3 (GPIO 48) and ESP32-C3 (GPIO 8)
- **Configurable GPIO** and LED count via web interface
- **Multiple patterns**:
  - Rainbow effect (full HSV spectrum)
  - Blink pattern (configurable color)
  - Fade effect (smooth brightness transition)
  - Custom color with RGB picker
- **Complete test sequence** (RGB colors + rainbow)

#### API Endpoints
- `/api/neopixel-config?gpio=X&count=Y` - Configure NeoPixel
- `/api/neopixel-test` - Run complete test
- `/api/neopixel-pattern?pattern=[rainbow|blink|fade|off]` - Run pattern
- `/api/neopixel-color?r=X&g=Y&b=Z` - Set custom color

#### Technical Details
- Support for 1-100 LEDs
- NEO_GRB + NEO_KHZ800 configuration
- Gamma correction for smooth colors
- Memory-safe implementation with proper cleanup

---

## [2.0] - 2025-10-03

### Major Rewrite - Complete Diagnostic System

#### Added
- **Built-in LED testing** with configurable GPIO
- **LED control features**:
  - Blink pattern (5 times)
  - Fade effect (smooth PWM)
  - Individual on/off control
- **Detailed memory analysis**:
  - Flash memory (real size vs configured)
  - PSRAM detection and testing (OPI/QSPI)
  - SRAM statistics
  - Fragmentation analysis
- **PSRAM diagnostic** with detailed boot log
- **WiFi network scanner** with full details (RSSI, channel, encryption, BSSID)
- **I2C peripheral scanner** with address detection
- **GPIO testing** for all available pins
- **ADC testing** on all ADC channels
- **Touch pad testing** (ESP32/S3)
- **PWM testing** with duty cycle sweep
- **SPI bus detection**
- **Flash partition listing** with details
- **Memory stress test** with max allocation detection
- **CPU and Memory benchmarks** (MFLOPS, MB/s)

#### Technical Features
- **Multi-WiFi support** via WiFiMulti
- **mDNS support** (esp32-diagnostic.local)
- **Comprehensive logging** via Serial Monitor
- **Auto-detection** for chip model and features
- **Memory safety checks** throughout
- **Real-time monitoring** of temperature and uptime

#### Export Formats
- **TXT** - Human-readable report with all data
- **JSON** - Machine-readable structured data
- **CSV** - Spreadsheet-compatible format
- **Print** - PDF-ready formatted page

---

## [1.0] - 2025-10-01

### Initial Release

#### Features
- Basic chip information display
- WiFi connection
- Simple web interface
- Memory statistics (Flash and SRAM only)
- GPIO list
- Basic system information

#### Supported Boards
- ESP32 (original)
- ESP32-S2
- ESP32-S3
- ESP32-C3

---

## Development Roadmap

### [3.1] - Planned
- [ ] WebSocket support for real-time updates without polling
- [ ] Historical data graphs (memory, temperature over time)
- [ ] Data export to SD card
- [ ] Configuration save/restore
- [ ] OTA firmware update via web interface
- [ ] Advanced WiFi features (AP mode, WiFi config portal)

### [3.2] - Planned
- [ ] MQTT support for IoT integration
- [ ] REST API documentation (Swagger/OpenAPI)
- [ ] Dark mode theme
- [ ] Mobile app (PWA support)
- [ ] Multi-device dashboard

### [4.0] - Future
- [ ] Sensor support (BME280, DHT22, etc.)
- [ ] Automation rules and triggers
- [ ] Data logging and analytics
- [ ] User authentication
- [ ] Multi-language expansion (Spanish, German, Italian)

---

## Version History Summary

| Version | Date | Key Features |
|---------|------|--------------|
| 3.0-dev | 2025-10-09 | Dynamic interface, REST API, real-time updates |
| 2.5.1 | 2025-10-10 | PSRAM guidance for IDE configuration |
| 2.5 | 2025-10-08 | Translated exports |
| 2.4 | 2025-10-07 | Multilingual interface (FR/EN) |
| 2.3 | 2025-10-06 | OLED display support |
| 2.2 | 2025-10-05 | TFT display support |
| 2.1 | 2025-10-04 | NeoPixel support |
| 2.0 | 2025-10-03 | Complete rewrite with all diagnostics |
| 1.0 | 2025-10-01 | Initial release |

---

## Contributing

See [CONTRIBUTING.md](CONTRIBUTING.md) for guidelines on:
- Reporting bugs
- Suggesting features
- Submitting pull requests
- Code style guidelines

---

## License

This project is licensed under the MIT License - see [LICENSE](LICENSE) file for details.

---

**Maintained by**: morfredus
**Repository**: https://github.com/yourusername/esp32-diagnostic
**Documentation**: https://github.com/yourusername/esp32-diagnostic/wiki
