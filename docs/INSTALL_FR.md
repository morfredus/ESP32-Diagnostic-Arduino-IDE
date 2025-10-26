# Guide d'installation (FR)

Suivez ces étapes pour déployer ESP32 Diagnostic Suite sur votre carte.

> **Maintenance 3.1.14-maint :** Le bandeau de version et la constante `DIAGNOSTIC_VERSION` affichent désormais 3.1.14-maint. Vérifiez ce numéro dans le Moniteur série après téléversement.

## 1. Prérequis
- Carte ESP32, ESP32-S2, ESP32-S3, ESP32-C3, ESP32-C6 ou ESP32-H2 (S3 recommandée).
- Câble USB avec transfert de données.
- Optionnel : bandeau NeoPixel/WS2812B et écran OLED 0,96" SSD1306 pour les tests avancés.
- Arduino IDE 2.x ou supérieur avec l'Arduino Core ESP32 3.3.2.

## 2. Bibliothèques nécessaires
Installez ces bibliothèques via le Library Manager de l'Arduino IDE :
- **Adafruit BusIO**
- **Adafruit GFX Library**
- **Adafruit SSD1306**
- **Adafruit NeoPixel**

Les bibliothèques suivantes sont fournies avec l'Arduino Core ESP32 : `WiFi`, `WebServer`, `ESPmDNS`, `Wire`, `SPI`, `esp_chip_info`, `esp_flash`, `esp_heap_caps`, `esp_partition`.

## 3. Récupérer le code source
1. [Forkez le dépôt sur GitHub](https://github.com/ESP32-Diagnostic/ESP32-Diagnostic/fork), téléchargez l'archive du dépôt ou clonez-le dans votre dossier de croquis Arduino.
2. Vérifiez que le dossier se nomme `ESP32-Diagnostic` et contient `ESP32-Diagnostic.ino` à la racine.

```
ESP32-Diagnostic/
├── ESP32-Diagnostic.ino
├── languages.h
├── web_interface.h
├── wifi-config-example.h
└── docs...
```

## 4. Préparer les identifiants Wi-Fi
1. Dupliquez `wifi-config-example.h` puis renommez la copie en `wifi-config.h`.
2. Ouvrez `wifi-config.h` et remplacez les paires SSID/MOT_DE_PASSE par vos réseaux.
3. Vous pouvez déclarer plusieurs réseaux ; le firmware les testera dans l'ordre.

Les précisions de configuration sont détaillées dans [CONFIG_FR.md](CONFIG_FR.md).

## 5. Compilation et téléversement
1. Ouvrez `ESP32-Diagnostic.ino` dans l'Arduino IDE.
2. Sélectionnez votre carte ESP32 et le port série approprié.
3. Activez la **PSRAM** pour les cartes ESP32-S3 afin de profiter de tous les diagnostics.
4. Cliquez sur **Vérifier** puis sur **Téléverser** pour flasher le firmware.

En cas de succès, le moniteur série affiche la version du firmware et l'état de connexion.
