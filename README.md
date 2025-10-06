# ESP32 Diagnostic Complet v2.3

## 📋 Description

Outil de diagnostic exhaustif pour microcontrôleurs ESP32, accessible via interface web. Teste automatiquement tous les composants matériels, analyse la mémoire, scanne les périphériques et génère des rapports détaillés.

## ✨ Fonctionnalités

### Tests Matériels
- **LED intégrée** - Configuration et test avec patterns (blink, fade)
- **NeoPixel/WS2812B** - Support multi-LED avec effets RGB
- **Écran TFT 320x240** - Test couleurs, formes, texte
- **Écran OLED 0.96" I2C** - 10 tests d'affichage incluant animations
- **GPIO** - Test automatique de tous les GPIO disponibles
- **ADC** - Lecture de tous les canaux analogiques
- **Touch Pads** - Test capteurs tactiles capacitifs
- **PWM/LEDC** - Test modulation largeur d'impulsion

### Analyse Système
- **Mémoire détaillée** - Flash, PSRAM (OPI/QSPI), SRAM avec tests d'intégrité
- **WiFi Scanner** - Scan réseaux avec RSSI, canal, encryption
- **Scan I2C** - Détection automatique périphériques (0x01-0x7F)
- **Scan SPI** - Informations bus SPI disponibles
- **Partitions Flash** - Liste complète des partitions
- **Benchmarks** - Performance CPU et mémoire
- **Stress Test** - Test limite allocation mémoire

### Interface Web
- **8 onglets** - Navigation intuitive (Vue d'ensemble, LEDs, Écrans, Tests avancés, GPIO, WiFi, Performance, Export)
- **Configuration dynamique** - Pins I2C OLED modifiables via web
- **Temps réel** - Actualisation données sans rechargement
- **Responsive** - Compatible mobile/tablette/desktop
- **Exports complets** - TXT, JSON, CSV, version imprimable PDF

### Accès Réseau
- **mDNS** - Accès via http://ESP32-test.local
- **Multi-WiFi** - Connexion automatique avec failover
- **API REST** - Endpoints JSON pour intégration

## 🎯 Cartes Compatibles

Testé et optimisé pour :
- ✅ **ESP32** (original)
- ✅ **ESP32-S2**
- ✅ **ESP32-S3** (recommandé - avec PSRAM OPI)
- ✅ **ESP32-C3**
- ⚠️ **ESP32-C6, ESP32-H2** (non testé mais devrait fonctionner)

## 📦 Prérequis

### Matériel
- Carte ESP32 (n'importe quel modèle)
- Câble USB
- Optionnel : LED NeoPixel, écran TFT, écran OLED

### Logiciel
- **Arduino IDE** 2.x ou supérieur
- **ESP32 Arduino Core** 3.1.3 ou supérieur

### Bibliothèques Arduino Requises

#### Via le Gestionnaire de bibliothèques
**Obligatoires (pour compilation) :**
```
1. Adafruit BusIO           (dépendance I2C/SPI)
2. Adafruit GFX Library     (dépendance graphique)
3. Adafruit SSD1306         (pour écran OLED 0.96")
4. Adafruit NeoPixel        (pour LEDs RGB WS2812B)
5. TFT_eSPI                 (pour écran TFT 320x240)
```

#### Incluses avec ESP32 Core (pas besoin d'installer)
```
- WiFi, WebServer, ESPmDNS
- Wire (I2C), SPI
- esp_chip_info, esp_mac, esp_flash, esp_heap_caps, esp_partition
```

**Installation :**
```
Arduino IDE → Croquis → Inclure une bibliothèque → Gérer les bibliothèques
```

Recherchez et installez **dans cet ordre** :
1. `Adafruit BusIO`
2. `Adafruit GFX Library`
3. `Adafruit SSD1306`
4. `Adafruit NeoPixel`
5. `TFT_eSPI`

**Versions testées compatibles :**
```
- Adafruit NeoPixel : 1.12.0+
- TFT_eSPI : 2.5.0+
- Adafruit GFX : 1.11.0+
- Adafruit SSD1306 : 2.5.0+
- Adafruit BusIO : 1.14.0+
- ESP32 Arduino Core : 3.1.3
```

## 🚀 Installation

### 1. Téléchargement

Téléchargez ou clonez ce projet dans votre dossier Arduino.

### 2. Configuration WiFi

**IMPORTANT :** Créez un fichier `config.h` dans le même dossier que le `.ino`

**Option A - Copier le template :**
```bash
cp config.h.example config.h
```

**Option B - Créer manuellement `config.h` :**
```cpp
#ifndef CONFIG_H
#define CONFIG_H

// ========== CONFIGURATION WIFI ==========
const char* WIFI_SSID_1 = "VotreSSID";
const char* WIFI_PASS_1 = "VotreMotDePasse";

const char* WIFI_SSID_2 = "SSID2"; // Optionnel
const char* WIFI_PASS_2 = "Pass2"; // Optionnel

#endif
```

Remplacez `VotreSSID` et `VotreMotDePasse` par vos identifiants WiFi réels.

**⚠️ SÉCURITÉ :** Ne partagez jamais le fichier `config.h` (ajoutez-le à `.gitignore`)

### 3. Configuration PSRAM (si disponible)

Si votre carte a de la PSRAM :

**ESP32-S3 :**
```
Outils → PSRAM → OPI PSRAM
```

**ESP32 classique :**
```
Outils → PSRAM → Enabled
```

**ESP32-S2 :**
```
Outils → PSRAM → QSPI PSRAM
```

### 4. Configuration Flash

Vérifiez la taille Flash de votre carte :
```
Outils → Flash Size → (choisir la taille réelle)
```

Exemples :
- ESP32-S3-DevKitC-1 : **16MB**
- ESP32-WROOM-32 : **4MB**
- ESP32-C3-DevKitM-1 : **4MB**

### 5. Téléversement

```
Croquis → Téléverser
```

### 6. Accès à l'interface

Ouvrez le **Moniteur Série** (115200 bauds) pour voir :
- L'adresse IP attribuée
- L'URL mDNS
- Les informations de démarrage

Accédez via :
- **http://ESP32-test.local** (si mDNS fonctionne)
- **http://[IP_AFFICHEE]** (toujours fiable)

## 🔧 Configuration Pins

### Pins par défaut

#### I2C (OLED) - Modifiable via web
| Modèle   |SDA|SCL|
|----------|---|---|
| ESP32    | 21| 2 |
| ESP32-S2 | 8 | 9 |
| ESP32-S3 | 8 | 9 |
| ESP32-C3 | 5 | 6 |

**✨ Configuration dynamique** : Modifiable directement dans l'onglet "Écrans" de l'interface web.

#### TFT SPI (fixes dans le code)
```cpp
MOSI: 45, SCLK: 3, CS: 14, DC: 47, RST: 21, MISO: 46
```

#### LED Intégrée (auto-détectée)
- ESP32-S3 : GPIO 2
- ESP32-C3 : GPIO 8
- ESP32 : GPIO 2

#### NeoPixel (auto-détectée, personnalisable)
- ESP32-S3 : GPIO 48
- ESP32-C3 : GPIO 8
- Personnalisable via `#define CUSTOM_LED_PIN`

## 📖 Utilisation

### Vue d'ensemble
Affiche toutes les informations système :
- **Chip** : Modèle, révision, fréquence, MAC, température
- **Mémoire** : Flash (réelle vs configurée), PSRAM, SRAM avec graphiques de progression
- **WiFi** : SSID, RSSI, qualité signal, IP, masque, passerelle, DNS
- **GPIO** : Liste complète des GPIO disponibles
- **I2C** : Périphériques détectés avec adresses

### Onglet LEDs
- **LED intégrée** : Configuration GPIO, Test automatique, Blink, Fade, Off
- **NeoPixel** : Config GPIO/Count, Test complet, Rainbow, Blink, Fade, Couleur personnalisée RGB

### Onglet Écrans
- **TFT 320x240** : Test complet (15s - couleurs primaires, damier, texte), Patterns individuels
- **OLED 0.96"** : 
  - Configuration pins I2C dynamique
  - Test complet (25s avec 10 animations)
  - Message personnalisé
  - Tests : Texte multi-tailles, formes, lignes, animations, barre progression, texte défilant

### Tests Avancés
- **ADC** : Test tous canaux avec valeurs brutes et tensions calculées
- **Touch Pads** : Valeurs de tous les capteurs tactiles (10-14 selon modèle)
- **PWM** : Test modulation avec progression 0-255
- **SPI** : Informations bus disponibles (HSPI/VSPI pour ESP32)
- **Partitions Flash** : Liste détaillée avec type, adresse, taille
- **Stress Test** : Allocation mémoire maximale avec rapport KB alloués

### GPIO
Test automatique de tous les GPIO disponibles avec affichage visuel OK/FAIL

### WiFi
Scanner complet avec :
- Qualité signal (🟢🟡🔴)
- RSSI en dBm
- Canal
- Type d'encryption
- BSSID

### Performance
- **Benchmarks CPU** : MFLOPS calculés
- **Benchmark Mémoire** : Vitesse lecture/écriture

### Export
- **TXT** : Rapport complet texte brut (toutes sections)
- **JSON** : Format structuré pour intégration API
- **CSV** : Compatible Excel/LibreOffice avec catégories
- **Imprimer** : Version optimisée A4 avec auto-impression

## 🛠️ Personnalisation

### Modifier le hostname mDNS
```cpp
#define MDNS_HOSTNAME "ESP32-test"
```

### Modifier la version
```cpp
#define DIAGNOSTIC_VERSION "2.3"
```

### Désactiver le scan I2C
```cpp
#define ENABLE_I2C_SCAN false
```

### Changer les pins NeoPixel par défaut
```cpp
#define CUSTOM_LED_PIN 48
#define CUSTOM_LED_COUNT 8
```

### Adresse I2C OLED
```cpp
#define SCREEN_ADDRESS 0x3C  // ou 0x3D selon le module
```

## 📊 API REST

Tous les tests sont accessibles via API JSON :

```
GET /                       → Interface web complète
GET /api/test-gpio          → Test tous GPIO
GET /api/wifi-scan          → Scanner WiFi
GET /api/i2c-scan           → Scanner I2C
GET /api/adc-test           → Test ADC
GET /api/touch-test         → Test Touch Pads
GET /api/pwm-test           → Test PWM
GET /api/spi-scan           → Info SPI
GET /api/partitions-list    → Liste partitions
GET /api/benchmark          → Benchmarks CPU/Mémoire
GET /api/memory-details     → Mémoire détaillée
GET /api/stress-test        → Stress test

GET /api/builtin-led-test   → Test LED intégrée
GET /api/neopixel-test      → Test NeoPixel
GET /api/tft-test           → Test TFT
GET /api/oled-test          → Test OLED

GET /export/txt             → Export TXT
GET /export/json            → Export JSON
GET /export/csv             → Export CSV
GET /print                  → Version imprimable
```

Exemple d'utilisation :
```bash
curl http://ESP32-test.local/api/memory-details
```

## 🐛 Dépannage

### PSRAM non détectée
1. Vérifier : `Outils → PSRAM → OPI PSRAM` (ou QSPI selon modèle)
2. Clean complet : `Croquis → Nettoyer`
3. Recompiler et téléverser
4. Consulter le diagnostic PSRAM dans le Moniteur Série

### Flash size incorrecte
Si "Taille Réelle" ≠ "Configurée IDE" dans l'interface :
1. Noter la taille réelle affichée
2. `Outils → Flash Size → [Taille réelle]`
3. Recompiler

### Écran OLED non détecté
1. Vérifier câblage physique SDA/SCL
2. Tester adresse I2C : 0x3C ou 0x3D
3. Scanner I2C dans "Vue d'ensemble"
4. Modifier les pins dans l'onglet "Écrans"

### Onglets vides dans l'interface web
- Problème résolu en v2.3 avec `server.sendContent()`
- Vérifier que vous utilisez la dernière version
- Vider le cache du navigateur (Ctrl+F5)

### Page web ne charge pas
1. Vérifier connexion WiFi dans Moniteur Série
2. Utiliser IP directe au lieu de mDNS
3. Vérifier pare-feu/antivirus
4. Essayer un autre navigateur

### Erreur compilation `ledcSetup`
API mise à jour pour ESP32 Core 3.x (utilise `ledcAttach` maintenant).
Vérifiez que vous utilisez **ESP32 Core 3.1.3+**

### WiFi ne se connecte pas
1. Vérifier que `config.h` existe et contient les bons identifiants
2. Vérifier la portée WiFi
3. Tester avec un point d'accès 2.4GHz (ESP32 ne supporte pas 5GHz)
4. Vérifier SSID/mot de passe (sensible à la casse)

## 📈 Performances

- **Temps de démarrage** : ~5s
- **Génération page web** : ~200ms (17 chunks)
- **Mémoire heap utilisée** : ~250KB
- **Scan WiFi** : 2-5s (dépend du nombre de réseaux)
- **Test GPIO complet** : ~2s pour 20-40 GPIO
- **Benchmarks** : ~1s total
- **Test OLED complet** : 25s (10 animations)
- **Test TFT complet** : 15s

## 🔒 Sécurité

⚠️ **Important** : Ce diagnostic est prévu pour usage local/développement uniquement.

**Recommandations pour usage en production :**
- Ajoutez authentification HTTP Basic
- Utilisez HTTPS avec certificats
- Limitez les accès réseau (firewall)
- Ne pas exposer sur Internet public
- Désactivez les tests destructifs (stress test)
- Utilisez des mots de passe forts dans `config.h`

**Fichier `config.h` :**
- Ne jamais commiter ce fichier dans Git
- Ajouter à `.gitignore`
- Ne jamais partager avec des identifiants réels

## 📁 Structure du Projet

```
Test_ESP32_UNIVERSAL_Travail/
├── Test_ESP32_UNIVERSAL_Travail.ino  (fichier principal)
├── config.h.example                  (template configuration)
├── config.h                          (vos identifiants - à créer)
├── README.md                         (ce fichier)
└── .gitignore                        (exclut config.h)
```

## 📝 Licence

Code libre d'utilisation, modification et distribution.
Aucune garantie fournie - utilisez à vos propres risques.

## 🤝 Contribution

Contributions bienvenues via :
- Issues GitHub
- Pull Requests
- Forum Arduino
- Suggestions d'amélioration

## 🎓 Crédits

Développé pour la communauté ESP32.

**Basé sur :**
- ESP32 Arduino Core 3.1.3
- Adafruit Libraries (NeoPixel, GFX, SSD1306)
- Bodmer TFT_eSPI

**Testé sur :**
- ESP32-S3-DevKitC-1 (16MB Flash, 8MB PSRAM OPI)

## 🔄 Changelog

### v2.3 (2025)
- ✨ Ajout test OLED 0.96" avec 10 animations
- ✨ Configuration pins I2C dynamique via web
- ✨ Exports complets (TXT/JSON/CSV/Print)
- ✨ Support `server.sendContent()` pour pages volumineuses
- ✨ Externalisation WiFi dans `config.h`
- ✨ Variable `DIAGNOSTIC_VERSION` centralisée
- 🐛 Correction onglets vides
- 🐛 Correction détection PSRAM pour Core 3.1.3
- 🐛 Correction API PWM pour Core 3.x (`ledcAttach`)
- 📚 Documentation complète README

### v2.0-2.2
- Tests de base ESP32
- Interface web initiale
- Support TFT, NeoPixel, LED

---

**Version actuelle** : 2.3  
**Dernière mise à jour** : Janvier 2025  
**Auteur** : Communauté ESP32  
**Support** : ESP32 Arduino Core 3.1.3+

🌐 **Accès après installation** : http://ESP32-test.local