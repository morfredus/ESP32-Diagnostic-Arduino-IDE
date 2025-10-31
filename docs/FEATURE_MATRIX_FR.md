# ESP32 Diagnostic Suite – Matrice des fonctionnalités (v3.5.1)

Cette matrice résume la couverture des diagnostics pour les cartes Espressif les plus courantes prises en charge par la version
3.5.1. Elle aide à planifier les campagnes de validation et à vérifier si des périphériques optionnels nécessitent un câblage
supplémentaire.

## Légende
- ✅ – Pris en charge nativement par le firmware.
- ⚙️ – Pris en charge avec configuration supplémentaire ou adaptation matérielle (voir colonne Notes).
- ⛔ – Non disponible pour la carte cible ou nécessite une extension firmware spécifique.

## Capacités des cartes principales
| Famille de carte | Scan Wi-Fi | Scan BLE | Sonde PSRAM | Intégrité flash | Test NeoPixel | Test OLED 0,96" | Notes |
|------------------|-----------|----------|-------------|-----------------|--------------|------------------|-------|
| ESP32 (WROOM/DevKitC) | ✅ | ✅ | ⚙️ | ✅ | ✅ | ✅ | Détection PSRAM disponible sur les variantes WROVER équipées. |
| ESP32-S2 | ✅ | ⛔ | ✅ | ✅ | ✅ | ✅ | USB natif supporté ; matériel BLE absent. |
| ESP32-S3 | ✅ | ✅ | ✅ | ✅ | ✅ | ✅ | Double USB/Série ; utiliser Arduino Core 3.3.2 pour une couverture complète. |
| ESP32-C3 | ✅ | ✅ | ⛔ | ✅ | ⚙️ | ⚙️ | Requiert un remappage de broches pour NeoPixel ; OLED via I2C uniquement. |
| ESP32-C6 | ✅ | ✅ | ⛔ | ✅ | ⚙️ | ⚙️ | Radios Thread/Matter non exploitées ; configurer les broches LED/OLED. |
| ESP32-H2 | ⛔ | ✅ | ⛔ | ✅ | ⛔ | ⚙️ | Wi-Fi absent ; privilégier les diagnostics BLE et la matrice GPIO. |

## Couverture des bus et périphériques
| Bus / Périphérique | Broches par défaut | Cartes supportées | Notes |
|--------------------|--------------------|-------------------|-------|
| I2C principal | SDA 21, SCL 22 (reconfigurable) | Toutes (remappage nécessaire sur ESP32-C3/C6/H2) | Utilisé pour l'OLED, les packs capteurs, l'EEPROM. |
| I2C secondaire | Désactivé par défaut | ESP32, ESP32-S3 | Activer via le flag `ENABLE_SECONDARY_I2C` dans `config.h`. |
| Bus SPI test | MOSI 23, MISO 19, CLK 18, CS 5 | ESP32, ESP32-S3 | Valide la flash externe / adaptateurs SD. |
| Boucle UART | TX0/RX0 & UART1 optionnel | Toutes | Nécessite un cavalier TX↔RX sur l'UART choisi. |
| Sonde CAN (TWAI) | GPIO 4 / 5 | ESP32, ESP32-S3 | Demande un transceiver externe (ex. SN65HVD230). |
| Température OneWire | GPIO 15 | ESP32, ESP32-S3 | Détection DS18B20 lorsque `ENABLE_ONEWIRE` est actif. |

## Modules optionnels
| Module | Flag firmware | État par défaut | Description |
|--------|---------------|-----------------|-------------|
| Diagnostics OLED | `ENABLE_OLED_TESTS` | Activé si câblage OLED détecté | Lance balayage de pixels, contraste et récupération I2C. |
| Animateur NeoPixel | `ENABLE_NEOPIXEL_TESTS` | Activé | Exécute roue chromatique et stress test à 800 kHz. |
| Reporter MQTT | `ENABLE_MQTT_BRIDGE` | Désactivé | Publie des résumés ; nécessite les identifiants broker dans `config.h`. |
| Automatisation REST | `ENABLE_REST_API` | Activé | Fournit des endpoints JSON pour CI/CD ou outils flotte. |
| Moniteur d'énergie | `ENABLE_POWER_MONITOR` | Désactivé | Lit les capteurs INA219/INA3221 via I2C. |

## Hypothèses environnementales
- Arduino Core ESP32 **3.3.2** (PlatformIO `espressif32@6.5.x` ou Boards Manager 3.0.2+).
- Alimentation USB délivrant au moins 500 mA durant les stress tests OLED/NeoPixel.
- Réseau Wi-Fi stable (2,4 GHz) pour les mesures de débit ; les portails captifs ne sont pas gérés.

## Documents liés
- [Guide de configuration](CONFIG_FR.md) – activation ou désactivation des modules optionnels.
- [Modules de diagnostic détaillés](DIAGNOSTIC_MODULES_FR.md) – ordre d'exécution et métriques collectées.
- [Dépannage](TROUBLESHOOTING_FR.md) – étapes à suivre lorsqu'un module est signalé indisponible.
