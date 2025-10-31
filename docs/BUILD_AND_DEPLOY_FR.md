# ESP32 Diagnostic Suite – Compilation & déploiement (v3.5.0)

Ce document décrit les toolchains supportées et la checklist recommandée pour livrer le firmware 3.5.0.

## Toolchains
### Arduino IDE 2.x
1. Installer le package de cartes ESP32 **version 3.3.2** via le Boards Manager.
2. Cloner ou télécharger ce dépôt puis ouvrir `ESP32-Diagnostic.ino`.
3. Sélectionner le profil adapté (ex. *ESP32 Dev Module*, *ESP32-S3 DevKit*).
4. Configurer `Tools → PSRAM` et `USB CDC` selon le matériel.
5. Flasher le firmware (`Ctrl+U`) et surveiller la console série à 115200 bauds.

### Arduino CLI
```bash
arduino-cli core install esp32:esp32@3.3.2
arduino-cli lib install "Adafruit GFX Library" "Adafruit SSD1306" "Adafruit NeoPixel" "Adafruit BusIO"
arduino-cli compile --fqbn esp32:esp32:esp32s3 ESP32-Diagnostic.ino
arduino-cli upload --fqbn esp32:esp32:esp32s3 -p /dev/ttyUSB0 ESP32-Diagnostic.ino
```

### PlatformIO
Extrait `platformio.ini` :
```ini
[env:esp32-s3-devkit]
platform = espressif32@6.5.0
board = esp32-s3-devkitc-1
framework = arduino
monitor_speed = 115200
lib_deps =
  adafruit/Adafruit GFX Library@^1.11.9
  adafruit/Adafruit NeoPixel@^1.11.0
  adafruit/Adafruit SSD1306@^2.5.7
  adafruit/Adafruit BusIO@^1.15.0
```
Lancer `pio run --target upload` pour compiler et flasher.

## Checklist pré-déploiement
- [ ] Mettre à jour `wifi-config.h` avec les identifiants production et, le cas échéant, les paramètres entreprise.
- [ ] Vérifier que `DIAGNOSTIC_VERSION` vaut `3.5.0` dans le firmware et la documentation.
- [ ] Compiler les ressources multilingues sans avertissement (`languages.h`).
- [ ] Exécuter un cycle complet sur une carte de référence et exporter les rapports JSON/CSV.
- [ ] Capturer des captures d'écran ou impressions du tableau de bord pour les notes de version si nécessaire.

## Tests d'acceptation
| Test | Procédure | Résultat attendu |
|------|-----------|------------------|
| Smoke test connectivité | Connexion au Wi-Fi labo, lancer un diagnostic rapide. | PASS avec RSSI > -70 dBm, ping < 40 ms. |
| Endurance mémoire | Lancer 5 diagnostics complets consécutifs. | Aucune saturation du tas, PSRAM stable. |
| Validation périphériques | Brancher OLED + chaîne NeoPixel, activer les modules. | PASS avec FPS stable et animation cohérente. |
| Sanity REST API | Déclencher `/api/run` puis `/api/status`. | File d'attente reçue et payload JSON valide. |

## Packaging release
1. Taguer le dépôt avec `v3.5.0` (tag annoté).
2. Joindre les binaires compilés (`.bin`) pour chaque carte si diffusion via GitHub Releases.
3. Publier les notes en se référant au [CHANGELOG_FR.md](../CHANGELOG_FR.md).
4. Partager les liens du wiki mis à jour (`docs/home.md`) avec les équipes support.

## Considérations OTA
- La diffusion principale reste le flash USB. Pour l'OTA, intégrer votre bootloader et vérifier que le binaire reste sous la taille
  de partition OTA.
- Toujours valider la signature ou le checksum après transfert OTA pour éviter un diagnostic corrompu.

## Stratégie de rollback
- Conserver une image stable (ex. v3.2.21-maint) prête pour un retour arrière d'urgence.
- Documenter les étapes de rollback dans la procédure interne en se référant à ce guide pour le reflashing.

## Ressources associées
- [Guide d'installation](INSTALL_FR.md)
- [Guide de configuration](CONFIG_FR.md)
- [Notes de version](../CHANGELOG_FR.md)
