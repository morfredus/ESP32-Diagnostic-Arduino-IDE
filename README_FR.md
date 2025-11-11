# ESP32 Diagnostic Suite (v3.8.0)

---

## ⚠️ AVIS IMPORTANT / NOTICE

**🇫🇷 Français :**
Ce dépôt **ESP32-Diagnostic-Arduino-IDE** (https://github.com/morfredus/ESP32-Diagnostic-Arduino-IDE) est **archivé mais reste accessible**. Le développement a migré d'Arduino IDE vers PlatformIO sous Visual Studio Code pour une meilleure gestion des dépendances, une cohérence de compilation et un flux de travail de développement professionnel.

**➡️ Veuillez visiter le nouveau dépôt :** [ESP32-Diagnostic](https://github.com/morfredus/ESP32-Diagnostic)

**⚠️ Note :** La fonctionnalité Bluetooth n'est **pas prise en charge** dans la nouvelle version PlatformIO en raison de limitations de la plateforme. Si vous devez tester les fonctionnalités Bluetooth, veuillez utiliser cette version Arduino IDE archivée.

**🇬🇧 English:**
This repository **ESP32-Diagnostic-Arduino-IDE** (https://github.com/morfredus/ESP32-Diagnostic-Arduino-IDE) is **archived but remains accessible**. Development has been migrated from Arduino IDE to PlatformIO under Visual Studio Code for better dependency management, build consistency, and professional development workflow.

**➡️ Please visit the new repository:** [ESP32-Diagnostic](https://github.com/morfredus/ESP32-Diagnostic)

**⚠️ Note:** Bluetooth functionality is **not supported** in the new PlatformIO version due to platform limitations. If you need to test Bluetooth features, please use this archived Arduino IDE version.

---

Micrologiciel de diagnostic complet pour microcontrôleurs ESP32 avec tableau de bord web interactif, tests matériels automatisés et contenus bilingues (FR/EN). Le firmware cible l'Arduino Core 3.3.3 et prend en charge les familles ESP32, ESP32-S2, ESP32-S3, ESP32-C3, ESP32-C6 et ESP32-H2.

## Points clés de la version 3.8.0
- Ajout d'un écran de démarrage OLED qui suit les étapes de connexion Wi-Fi (association, DHCP, réussite) afin d'afficher instantanément l'état sur les cartes équipées.
- Correction des régressions NimBLE en protégeant les descripteurs optionnels, en alignant les callbacks et en validant les résultats de scan sur ESP32/ESP32-S3 avant utilisation.
- Amélioration de la gestion du scan BLE grâce à la réutilisation sûre des tampons NimBLE, à la reprise de la diffusion après échec et à des réponses HTTP cohérentes lorsque le scan est indisponible.

## Structure du projet
- `ESP32-Diagnostic.ino` – point d'entrée du firmware et ordonnanceur des diagnostics.
- `languages.h` – catalogue de traductions et sélecteur de langue à chaud.
- `web_interface.h` – modèles HTML/CSS/JS fournis par le firmware.
- `wifi-config-example.h` – modèle pour stocker les identifiants Wi-Fi.
- `docs/` – wiki complet regroupant installation, utilisation, API et guides de déploiement (FR/EN).

## Documentation
| Sujet | Anglais | Français |
|-------|---------|----------|
| Wiki (accueil) | [home.md](docs/home.md) | [home_FR.md](docs/home_FR.md) |
| Vue d'ensemble | [OVERVIEW.md](docs/OVERVIEW.md) | [OVERVIEW_FR.md](docs/OVERVIEW_FR.md) |
| Matrice des fonctionnalités | [FEATURE_MATRIX.md](docs/FEATURE_MATRIX.md) | [FEATURE_MATRIX_FR.md](docs/FEATURE_MATRIX_FR.md) |
| Modules de diagnostic | [DIAGNOSTIC_MODULES.md](docs/DIAGNOSTIC_MODULES.md) | [DIAGNOSTIC_MODULES_FR.md](docs/DIAGNOSTIC_MODULES_FR.md) |
| Interface web | [WEB_INTERFACE.md](docs/WEB_INTERFACE.md) | [WEB_INTERFACE_FR.md](docs/WEB_INTERFACE_FR.md) |
| REST API | [API_REFERENCE.md](docs/API_REFERENCE.md) | [API_REFERENCE_FR.md](docs/API_REFERENCE_FR.md) |
| Compilation & déploiement | [BUILD_AND_DEPLOY.md](docs/BUILD_AND_DEPLOY.md) | [BUILD_AND_DEPLOY_FR.md](docs/BUILD_AND_DEPLOY_FR.md) |
| Sécurité | [SECURITY.md](docs/SECURITY.md) | [SECURITY_FR.md](docs/SECURITY_FR.md) |
| FAQ | [FAQ.md](docs/FAQ.md) | [FAQ_FR.md](docs/FAQ_FR.md) |
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
- **Arduino IDE :** 2.x ou supérieur avec l'Arduino Core ESP32 3.3.3.
- **Bibliothèques :** Adafruit BusIO, Adafruit GFX Library, Adafruit SSD1306, Adafruit NeoPixel (installées via Library Manager).

## Support
- Licence : [MIT](LICENSE)
- Support & remontées : [ouvrir un ticket GitHub](https://github.com/morfredus/ESP32-Diagnostic-Arduino-IDE/issues/new/choose) avec étapes de reproduction et détails carte.
- Contribution : [forkez le dépôt](https://github.com/morfredus/ESP32-Diagnostic-Arduino-IDE/fork) puis consultez [CONTRIBUTING_FR.md](CONTRIBUTING_FR.md).
