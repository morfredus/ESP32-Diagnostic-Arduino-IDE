<!-- 
Available in multiple languages:
- English: README.md
- Français: README.fr.md
-->
# Outil de Diagnostic Complet ESP32 v3.0.3-dev

[![Version](https://img.shields.io/badge/version-3.0.3--dev-blue.svg)](https://github.com/yourusername/esp32-diagnostic)
[![License](https://img.shields.io/badge/license-MIT-green.svg)](LICENSE)
[![Platform](https://img.shields.io/badge/platform-ESP32-orange.svg)](https://www.espressif.com/)
[![Arduino](https://img.shields.io/badge/Arduino-3.1.3-teal.svg)](https://github.com/espressif/arduino-esp32)

Un outil de diagnostic complet basé sur une interface web pour microcontrôleurs ESP32 avec surveillance temps réel, tests matériels et interface dynamique.

[🇬🇧 English version](README.md) | [📋 Changelog](CHANGELOG.md)

![Interface ESP32 Diagnostic](docs/screenshot.png)

## 🌟 Fonctionnalités Principales

### 🔄 Surveillance Temps Réel (v3.0.3-dev)
- **Interface web dynamique** avec chargement asynchrone
- **Rafraîchissement automatique toutes les 5 secondes** pour les données en direct
- **Architecture API REST** pour une communication efficace
- **Indicateur d'état de connexion** (en ligne/hors ligne)
- **Animations et transitions fluides**

### 🔧 Informations Système
- Identification complète de la puce (ESP32, S2, S3, C3, C6, H2)
- Cœurs CPU et fréquence
- Adresse MAC
- Versions SDK et ESP-IDF
- Capteur de température interne (si disponible)
- Suivi du temps de fonctionnement
- Détection de la raison du dernier redémarrage

### 💾 Analyse Mémoire Détaillée
- **Mémoire Flash** : Taille réelle vs taille configurée
- **PSRAM** : Total, libre, utilisé (support OPI/QSPI)
- **SRAM** : Statistiques mémoire interne
- **Analyse de fragmentation** avec statut
- Test de stress mémoire
- Graphiques mémoire en temps réel

### 📡 Gestion WiFi
- Support multi-réseaux (WiFiMulti)
- Scanner de réseaux avec détails :
  - SSID, BSSID, Canal
  - Force du signal (RSSI)
  - Type de chiffrement
  - Indicateur de qualité du signal
- Informations de connexion (IP, Masque, Passerelle, DNS)
- Support mDNS (esp32-diagnostic.local)

### 🔌 Tests GPIO (v3.0.3-dev)
- Test GPIO individuel (E/S numériques)
- Cartographie complète des broches pour chaque variante ESP32
- Indicateurs visuels OK/ÉCHEC
- Tests ADC (Convertisseur Analogique-Numérique)
- Tests capteurs tactiles (ESP32/S3)
- Tests PWM (Modulation de Largeur d'Impulsion)
- Rappel : un échec de test peut simplement signifier qu'un périphérique utilise déjà la broche, sans remettre en cause l'intégrité de la carte

### 💡 Contrôle LED (v3.0.3-dev)
- **LED Intégrée** :
  - Broche GPIO configurable
  - Motifs clignotement et fondu
  - Contrôle individuel
  - Prête dès la première ouverture de l'onglet grâce à un rafraîchissement garanti
- **NeoPixel/WS2812** :
  - GPIO et nombre de LEDs configurables
  - Effet arc-en-ciel
  - Couleurs personnalisées avec sélecteur
  - Animations clignotement et fondu
  - Retour d'information central et non bloquant après les diagnostics

### 📺 Support Écrans
- **Écran TFT SPI** :
  - Test couleurs complètes
  - Motif damier
  - Détection résolution personnalisée
- **OLED 0.96" I2C** :
  - Suite complète de tests (10 tests)
  - Affichage texte (tailles multiples)
  - Formes géométriques
  - Motifs de lignes
  - Animations (carré mobile, texte défilant)
  - Barre de progression
  - Affichage message personnalisé

### 🧪 Tests Avancés
- Scanner périphériques I2C
- Détection bus SPI
- Liste des partitions Flash
- Test de stress mémoire
- Benchmarks CPU et mémoire (MFLOPS, MB/s)

### 🌐 Interface Multilingue
- **Français** (par défaut)
- **Anglais**
- Changement de langue dynamique sans rechargement
- Notification toast confirmant le changement sans bloquer l'interface
- Exports traduits

### 📤 Export de Données
- **TXT** : Rapport lisible par humain
- **JSON** : Format structuré pour scripts
- **CSV** : Compatible Excel/tableurs
- **Print** : Page formatée prête pour PDF

## 📋 Matériel Requis

### Minimum
- ESP32 (n'importe quelle variante)
- Câble USB pour programmation
- Réseau WiFi

### Plateformes Testées
- ✅ ESP32 (original)
- ✅ ESP32-S2
- ✅ ESP32-S3 (avec PSRAM OPI)
- ✅ ESP32-C3
- ⚠️ ESP32-C6, H2 (non complètement testés)

### Matériel Optionnel
- Bande LED NeoPixel/WS2812
- Écran TFT SPI (ST7789/ILI9341)
- Écran OLED 0.96" I2C (SSD1306)
- Capteurs tactiles
- Périphériques externes (I2C/SPI)

## 🚀 Démarrage Rapide

### 1. Installation

#### Arduino IDE
1. Installer [Arduino IDE](https://www.arduino.cc/en/software) 1.8.19 ou ultérieur
2. Ajouter le support carte ESP32 :
   - Fichier → Préférences
   - URL de gestionnaire de cartes supplémentaires : `https://espressif.github.io/arduino-esp32/package_esp32_index.json`
   - Outils → Type de carte → Gestionnaire de cartes → Rechercher "ESP32" → Installer
3. Installer les bibliothèques requises :
   - Adafruit NeoPixel
   - TFT_eSPI
   - Adafruit GFX
   - Adafruit SSD1306

#### PlatformIO
```ini
[env:esp32dev]
platform = espressif32
board = esp32-s3-devkitc-1
framework = arduino
lib_deps =
    adafruit/Adafruit NeoPixel@^1.12.0
    bodmer/TFT_eSPI@^2.5.0
    adafruit/Adafruit GFX Library@^1.11.0
    adafruit/Adafruit SSD1306@^2.5.0
```

### 2. Configuration

Créer `config.h` :
```cpp
#ifndef CONFIG_H
#define CONFIG_H

// Réseaux WiFi (support multiple)
#define WIFI_SSID_1 "NomDeVotreReseau"
#define WIFI_PASS_1 "VotreMotDePasse"

#define WIFI_SSID_2 "ReseauSecondaire"
#define WIFI_PASS_2 "MotDePasseSecondaire"

#endif
```

### 3. Téléversement

1. Connecter votre ESP32
2. Sélectionner votre carte :
   - Outils → Type de carte → ESP32 Arduino → [Votre modèle ESP32]
3. Sélectionner le port : Outils → Port → [Votre port COM]
4. **Important pour PSRAM** : Outils → PSRAM → OPI PSRAM (ou QSPI PSRAM)
5. Téléverser : Croquis → Téléverser

### 4. Accès

Après téléversement, ouvrir le Moniteur Série (115200 baud) pour voir :
```
http://esp32-diagnostic.local
ou
http://192.168.x.x
```

Ouvrir votre navigateur et accéder à l'URL affichée.

## 📖 Guide d'Utilisation

### Onglets de l'Interface Web

#### 🏠 Vue d'ensemble
- Statut système complet
- Informations puce
- Détails mémoire avec barres de progression
- Détails connexion WiFi
- Résumé GPIO et périphériques

#### 💡 LEDs
- Configuration et test LED intégrée
- Configuration et motifs NeoPixel
- Contrôle temps réel

#### 📺 Écrans
- Tests écran TFT
- Tests écran OLED
- Affichage message personnalisé

#### 🧪 Tests
- Tests canaux ADC
- Tests touch pads
- Tests PWM
- Scan bus SPI
- Liste partitions Flash
- Test stress mémoire

#### 🔌 GPIO
- Tests GPIO complets
- Statut visuel des broches

#### 📡 WiFi
- Scanner de réseaux
- Scanner périphériques I2C

#### ⚡ Performance
- Benchmark CPU
- Benchmark mémoire
- Métriques de performance

#### 📤 Export
- Télécharger rapports diagnostic
- Options multiples formats

### Points de Terminaison API (v3.0.3-dev)

L'outil fournit une API REST complète :

#### Données Temps Réel
- `GET /api/status` - Statut système (refresh 5s)
- `GET /api/system-info` - Info puce et réseau
- `GET /api/overview` - Vue d'ensemble complète
- `GET /api/memory` - Détails mémoire
- `GET /api/wifi-info` - Informations WiFi
- `GET /api/peripherals` - Périphériques I2C/SPI

#### Informations Matériel
- `GET /api/leds-info` - Statut LED
- `GET /api/screens-info` - Statut écrans

#### Tests
- `GET /api/test-gpio` - Tester tous les GPIO
- `GET /api/wifi-scan` - Scanner réseaux WiFi
- `GET /api/i2c-scan` - Scanner périphériques I2C
- `GET /api/adc-test` - Tester canaux ADC
- `GET /api/touch-test` - Tester touch pads
- `GET /api/pwm-test` - Tester PWM
- `GET /api/benchmark` - Lancer benchmarks

#### Contrôle
- `GET /api/builtin-led-config?gpio=X` - Configurer LED
- `GET /api/builtin-led-test` - Tester LED
- `GET /api/builtin-led-control?action=[blink|fade|off]` - Contrôler LED
- `GET /api/neopixel-config?gpio=X&count=Y` - Configurer NeoPixel
- `GET /api/neopixel-pattern?pattern=[rainbow|blink|fade|off]` - Motif NeoPixel
- `GET /api/neopixel-color?r=X&g=Y&b=Z` - Définir couleur NeoPixel

## 🔧 Configuration Avancée

### Configuration Broches Personnalisées

Éditer dans le fichier `.ino` principal :

```cpp
// I2C pour OLED (modifiable via interface web)
int I2C_SDA = 8;
int I2C_SCL = 9;

// TFT SPI (fixe)
#define TFT_MOSI  45
#define TFT_SCLK   3
#define TFT_CS    14
#define TFT_DC    47
#define TFT_RST   21
#define TFT_MISO  46

// NeoPixel (modifiable via interface web)
#define CUSTOM_LED_PIN -1  // -1 pour auto-détection
#define CUSTOM_LED_COUNT 1
```

### Configuration TFT_eSPI

Éditer `User_Setup.h` dans la bibliothèque TFT_eSPI :

```cpp
#define ST7789_DRIVER
#define TFT_WIDTH  240
#define TFT_HEIGHT 320
#define TFT_MOSI 45
#define TFT_SCLK 3
#define TFT_CS   14
#define TFT_DC   47
#define TFT_RST  21
```

### Nom d'hôte mDNS

Modifier dans `.ino` :
```cpp
#define MDNS_HOSTNAME "esp32-diagnostic"
```

### Intervalle Rafraîchissement Auto

Éditer dans `web_interface.h` (section JavaScript) :
```javascript
const UPDATE_INTERVAL = 5000; // millisecondes (5 secondes)
```

## 🐛 Dépannage

### Problèmes de Connexion WiFi
- Vérifier identifiants dans `config.h`
- Vérifier réseau 2.4GHz (ESP32 ne supporte pas 5GHz)
- Essayer de se rapprocher du routeur
- Vérifier adresse IP dans le Moniteur Série

### PSRAM Non Détectée
1. Activer dans Arduino IDE : Outils → PSRAM → OPI PSRAM (ou QSPI)
2. Vérifier spécifications de votre carte
3. Voir diagnostic dans Moniteur Série au démarrage

### OLED Non Détecté
- Vérifier broches I2C (défaut : SDA=8, SCL=9)
- Vérifier adresse I2C (défaut : 0x3C)
- Utiliser interface web pour changer broches
- Lancer scan I2C pour détecter adresse

### TFT Ne Fonctionne Pas
- Vérifier broches SPI dans TFT_eSPI User_Setup.h
- Vérifier pilote écran (ST7789/ILI9341)
- Lancer test depuis interface web

### Erreurs de Compilation
- Mettre à jour core ESP32 vers 3.1.3 ou ultérieur
- Installer toutes bibliothèques requises
- Vérifier sélection carte dans Arduino IDE

## 📊 Performance

### Utilisation Mémoire
- **Flash** : ~800KB (programme)
- **SRAM** : ~50KB (exécution)
- **PSRAM** : Optionnelle (8MB recommandés)

### Interface Web
- Chargement initial : ~100KB (HTML+CSS+JS)
- Appels API : ~1-5KB par requête
- Rafraîchissement auto : Surcharge minimale

### Benchmarks (ESP32-S3 @ 240MHz)
- CPU : ~1200µs (83 MFLOPS)
- Mémoire : ~800µs
- Réponse web : <100ms

## 🤝 Contribution

Les contributions sont bienvenues ! Merci de :

1. Forker le dépôt
2. Créer une branche fonctionnalité
3. Faire vos modifications
4. Tester minutieusement
5. Soumettre une pull request

### Lignes Directrices Développement
- Suivre le style de code existant
- Commenter les sections complexes
- Mettre à jour la documentation
- Tester sur plusieurs variantes ESP32

## 📝 Licence

Licence MIT - voir fichier [LICENSE](LICENSE)

## 👤 Auteur

**morfredus**

## 🙏 Remerciements

- Espressif Systems pour la plateforme ESP32
- Communauté Arduino
- Adafruit pour d'excellentes bibliothèques
- Bodmer pour la bibliothèque TFT_eSPI
- Tous les contributeurs et testeurs

## 📚 Documentation

- [ESP32 Docs Officielles](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/)
- [Arduino ESP32 Core](https://github.com/espressif/arduino-esp32)
- [TFT_eSPI Wiki](https://github.com/Bodmer/TFT_eSPI)
- [Adafruit Learning](https://learn.adafruit.com/)

## 🔗 Projets Connexes

- [ESP32-Web-Server](https://github.com/espressif/arduino-esp32/tree/master/libraries/WebServer)
- [ESPAsyncWebServer](https://github.com/me-no-dev/ESPAsyncWebServer)
- [ESP32-PSRAM-Analyzer](https://github.com/espressif/esp-idf)

## ⭐ Montrer Votre Soutien

Si ce projet vous a aidé, merci de lui donner une ⭐ !

---

**Version** : 3.0.3-dev
**Dernière Mise à Jour** : 10 octobre 2025
**Statut** : Développement Actif
```
