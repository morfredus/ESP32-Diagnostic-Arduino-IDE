# ESP32 Diagnostic Suite (v3.2.0)

Micrologiciel de diagnostic complet pour microcontrôleurs ESP32 avec tableau de bord web interactif, tests matériels automatisés et contenus bilingues (FR/EN). Le firmware cible l'Arduino Core 3.3.2 et prend en charge les familles ESP32, ESP32-S2, ESP32-S3, ESP32-C3, ESP32-C6 et ESP32-H2.

## Points clés de la version 3.2.0
- Documentation de l'inspecteur de mémoire (`/api/memory-details`) et des alertes de fragmentation observées lors de la dernière campagne QA.
- Intégration, dans les guides d'utilisation et de dépannage, de la checklist Bluetooth®/Wi-Fi validée durant le débogage post-release.
- Synchronisation des guides FR/EN avec la bannière et le changelog 3.2.0 pour maintenir l'alignement entre firmware, documentation et notes de version.

## Structure du projet
- `ESP32-Diagnostic.ino` – point d'entrée du firmware et ordonnanceur des diagnostics.
- `languages.h` – catalogue de traductions et sélecteur de langue à chaud.
- `web_interface.h` – modèles HTML/CSS/JS fournis par le firmware.
- `wifi-config-example.h` – modèle pour stocker les identifiants Wi-Fi.
- `docs/` – guides détaillés d'installation, configuration, utilisation et dépannage (FR/EN).

## Documentation
| Sujet | Anglais | Français |
|-------|---------|----------|
| Installation | [INSTALL.md](docs/INSTALL.md) | [INSTALL_FR.md](docs/INSTALL_FR.md) |
| Configuration | [CONFIG.md](docs/CONFIG.md) | [CONFIG_FR.md](docs/CONFIG_FR.md) |
| Guide d'utilisation | [USAGE.md](docs/USAGE.md) | [USAGE_FR.md](docs/USAGE_FR.md) |
| Architecture du firmware | [ARCHITECTURE.md](docs/ARCHITECTURE.md) | [ARCHITECTURE_FR.md](docs/ARCHITECTURE_FR.md) |
| Dépannage | [TROUBLESHOOTING.md](docs/TROUBLESHOOTING.md) | [TROUBLESHOOTING_FR.md](docs/TROUBLESHOOTING_FR.md) |
| Contribuer | [CONTRIBUTING.md](CONTRIBUTING.md) | [CONTRIBUTING_FR.md](CONTRIBUTING_FR.md) |
| Journal des modifications | [CHANGELOG.md](CHANGELOG.md) | [CHANGELOG_FR.md](CHANGELOG_FR.md) |

## Capacités principales
- Interface multilingue avec bascule FR/EN en temps réel sans rechargement.
- Diagnostics matériels automatisés couvrant GPIO, ADC, pads tactiles, PWM, I2C, SPI, PSRAM, flash et scan Wi-Fi.
- Batterie de tests OLED 0,96" I2C avec déclenchement pas-à-pas et animations prévisualisées.
- Endpoints REST pour lancer les diagnostics et récupérer les rapports (TXT/JSON/CSV/vue imprimable).
- Support multi-AP Wi-Fi, découverte mDNS via `http://ESP32-Diagnostic.local` et exports complets.
- Motifs NeoPixel/WS2812B optionnels et outils de benchmark CPU/mémoire.

## Compatibilité & prérequis
- **Cartes :** ESP32, ESP32-S2, ESP32-S3 (recommandée), ESP32-C3, ESP32-C6, ESP32-H2.
- **Arduino IDE :** 2.x ou supérieur avec l'Arduino Core ESP32 3.3.2.
- **Bibliothèques :** Adafruit BusIO, Adafruit GFX Library, Adafruit SSD1306, Adafruit NeoPixel (installées via Library Manager).

## Support
- Licence : [MIT](LICENSE)
- Support & remontées : [ouvrir un ticket GitHub](https://github.com/ESP32-Diagnostic/ESP32-Diagnostic/issues/new/choose) avec étapes de reproduction et détails carte.
- Contribution : [forkez le dépôt](https://github.com/ESP32-Diagnostic/ESP32-Diagnostic/fork) puis consultez [CONTRIBUTING_FR.md](CONTRIBUTING_FR.md).
