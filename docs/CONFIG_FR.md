# Configuration (FR)

## Identifiants Wi-Fi
Le firmware lit les identifiants dans `wifi-config.h` placé à côté de `ESP32-Diagnostic.ino`.

```cpp
// wifi-config.h (exemple)
#pragma once
#include <vector>
#include <WiFiMulti.h>

static WiFiMulti wifiMulti;

static const std::vector<std::pair<const char*, const char*>> WIFI_NETWORKS = {
    {"MonSSID", "MonMotDePasse"},
    {"SecoursSSID", "SecoursMotDePasse"}
};
```

- Déclarez au moins un couple SSID/mot de passe.
- Le firmware parcourt la liste jusqu'à réussir la connexion.
- Évitez de versionner des identifiants sensibles.

## Paramètres de langue
- Le français est la langue par défaut.
- Utilisez `/api/set-language?lang=en` ou le sélecteur FR/EN dans l'interface pour basculer en anglais.
- Les chaînes de traduction résident dans `languages.h` au sein de la structure `Translations`. Ajoutez une langue en étendant cette structure et en l'exposant dans l'interface.
- La version 3.2.22-dev conserve l'intitulé accentué « Non testé », confirme l'historique consolidé et réaffirme les réponses HTTP 200/400 explicites de `/api/set-language` pour rester cohérent avec le changelog bilingue.

## Configuration OLED
- Broches par défaut : SDA=21, SCL=22 (standard ESP32).
- Modifiez le câblage à chaud via `/api/oled-config?sda=<pin>&scl=<pin>`.
- Après modification, l'écran est réinitialisé automatiquement et la cartographie reste en RAM.

## Règles de nommage Bluetooth®
- Les noms doivent comporter 3 à 31 caractères ASCII alphanumériques (+ tiret ou underscore).
- Une saisie invalide renvoie HTTP 400 sans relancer la diffusion.
- Un renommage valide persiste jusqu'au redémarrage.

## Options avancées
- Activez la PSRAM dans le menu carte de l'Arduino IDE pour les ESP32-S3 afin d'accéder aux diagnostics mémoire étendus.
- Le firmware détecte automatiquement les variantes et active le support BLE sur les cibles compatibles (ESP32, ESP32-S3, ESP32-C3, ESP32-C6, ESP32-H2).
- Pour le debug, suivez le moniteur série : retraits Wi-Fi, états BLE et progression des tests y sont journalisés, puis interrogez `/api/memory-details` pour obtenir les métriques de fragmentation détaillées.
