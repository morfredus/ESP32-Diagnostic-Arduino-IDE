## **README.md - English Version (v2.4)**

```markdown
# ESP32 Complete Diagnostic v2.4 - Multilingual

## 📋 Description

Comprehensive **multilingual** diagnostic tool for ESP32 microcontrollers, accessible via web interface. Automatically tests all hardware components, analyzes memory, scans peripherals and generates detailed reports.

**What's new in v2.4**: Interface available in **French** and **English** with **dynamic language switching without page reload**.

## ✨ Features

### 🌍 Multilingual Interface (NEW in v2.4)
- **French** (default language)
- **English**
- **Dynamic switching**: FR/EN buttons in interface
- **Real-time update**: No reload required
- **Complete translations**: All texts, labels, messages and status

### Hardware Tests
- **Built-in LED** - Configuration and testing with patterns (blink, fade)
- **NeoPixel/WS2812B** - Multi-LED support with RGB effects
- **TFT 320x240 Screen** - Color, shape, text testing
- **OLED 0.96" I2C Screen** - 10 display tests including animations
- **GPIO** - Automatic testing of all available GPIO
- **ADC** - Reading all analog channels
- **Touch Pads** - Capacitive touch sensor testing
- **PWM/LEDC** - Pulse width modulation testing

### System Analysis
- **Detailed Memory** - Flash, PSRAM (OPI/QSPI), SRAM with integrity tests
- **WiFi Scanner** - Network scan with RSSI, channel, encryption
- **I2C Scan** - Automatic peripheral detection (0x01-0x7F)
- **SPI Scan** - Available SPI bus information
- **Flash Partitions** - Complete partition list
- **Benchmarks** - CPU and memory performance
- **Stress Test** - Memory allocation limit testing

### Web Interface
- **8 tabs** - Intuitive navigation (Overview, LEDs, Screens, Advanced Tests, GPIO, WiFi, Performance, Export)
- **Language selector** - FR/EN buttons in top right
- **Dynamic configuration** - OLED I2C pins modifiable via web
- **Real-time** - Data refresh without reload
- **Responsive** - Mobile/tablet/desktop compatible
- **Complete exports** - TXT, JSON, CSV, printable PDF version

### Network Access
- **mDNS** - Access via http://ESP32-test.local
- **Multi-WiFi** - Automatic connection with failover
- **REST API** - JSON endpoints for integration

## 🎯 Compatible Boards

Tested and optimized for:
- ✅ **ESP32** (original)
- ✅ **ESP32-S2**
- ✅ **ESP32-S3** (recommended - with OPI PSRAM)
- ✅ **ESP32-C3**
- ⚠️ **ESP32-C6, ESP32-H2** (not tested but should work)

## 📦 Requirements

### Hardware
- ESP32 board (any model)
- USB cable
- Optional: NeoPixel LED, TFT screen, OLED screen

### Software
- **Arduino IDE** 2.x or higher
- **ESP32 Arduino Core** 3.1.3 or higher

### Required Arduino Libraries

**Mandatory (via Library Manager):**
```
1. Adafruit BusIO
2. Adafruit GFX Library
3. Adafruit SSD1306
4. Adafruit NeoPixel
5. TFT_eSPI
```

**Included with ESP32 Core:**
```
WiFi, WebServer, ESPmDNS, Wire, SPI
esp_chip_info, esp_mac, esp_flash, esp_heap_caps, esp_partition
```

## 🚀 Installation

### 1. Download

Download or clone this project to your Arduino folder.

### 2. Required Files

**Project structure:**
```
Test_ESP32_UNIVERSAL_Travail/
├── Test_ESP32_UNIVERSAL_Travail.ino  (main file)
├── languages.h                       (NEW - translations)
├── config.h.example                  (configuration template)
├── config.h                          (your credentials - to create)
└── README.md
```

### 3. WiFi Configuration

**IMPORTANT:** Create a `config.h` file in the same folder as the `.ino`

**Option A - Rename the example file:**
Rename the file `example-config.h` to `config.h`
Edit the file and add your WiFi network(s) by filling in the values

```cpp
const char* WIFI_SSID_1 = "SSID1";
const char* WIFI_PASS_1 = "Password1";

// Add as many networks as needed
// const char* WIFI_SSID_2 = "SSID2";
// const char* WIFI_PASS_2 = "Password2";

// const char* WIFI_SSID_3 = "SSID3";
// const char* WIFI_PASS_3 = "Password3";
```

**Option B - Create `config.h` manually:**
```cpp
#ifndef CONFIG_H
#define CONFIG_H

// ========== WIFI CONFIGURATION ==========
const char* WIFI_SSID_1 = "SSID1";
const char* WIFI_PASS_1 = "Password1";

// Add as many networks as needed
// const char* WIFI_SSID_2 = "SSID2";
// const char* WIFI_PASS_2 = "Password2";

// const char* WIFI_SSID_3 = "SSID3";
// const char* WIFI_PASS_3 = "Password3";

#endif
```

Replace `YourSSID` and `YourPassword` with your actual WiFi credentials.

### 4. Copy languages.h

Copy the provided `languages.h` file to the same folder as the `.ino`.

### 5. Hardware Configuration

**PSRAM (if available):**
- ESP32-S3: `Tools → PSRAM → OPI PSRAM`
- ESP32: `Tools → PSRAM → Enabled`

**Flash Size:**
```
Tools → Flash Size → [Your board's actual size]
```

### 6. Compile and Upload

```
Sketch → Upload
```

### 7. Accessing the Interface

- **http://ESP32-test.local** (mDNS)
- **http://[DISPLAYED_IP]** (direct IP)

## 🌐 Multilingual Usage

### Language Switching

**In the web interface:**
1. Click the **FR** or **EN** button in the top right
2. The interface updates **instantly**
3. No reload required

**Default:** French interface

**Via API:**
```bash
# Switch to English
curl http://ESP32-test.local/api/set-language?lang=en

# Switch to French
curl http://ESP32-test.local/api/set-language?lang=fr

# Get current translations
curl http://ESP32-test.local/api/get-translations
```

### Translated Texts

**All elements are translated:**
- Section titles
- Labels and descriptions
- Status messages (OK, FAIL, Testing...)
- Action buttons
- Error messages
- Signal qualities (Excellent, Good, Weak...)
- Reset reasons

## 🔧 Pin Configuration

### Default Pins

**I2C (OLED) - Modifiable via web:**
| Model    | SDA | SCL |
|----------|-----|-----|
| ESP32    | 21  | 22  |
| ESP32-S2 | 8   | 9   |
| ESP32-S3 | 8   | 9   |
| ESP32-C3 | 5   | 6   |

**TFT SPI (fixed):**
```
MOSI: 45, SCLK: 3, CS: 14, DC: 47, RST: 21, MISO: 46
```

**Built-in LED (auto-detected):**
- ESP32-S3: GPIO 2
- ESP32-C3: GPIO 8
- ESP32: GPIO 2

**NeoPixel (auto-detected):**
- ESP32-S3: GPIO 48
- ESP32-C3: GPIO 8

## 📖 Usage

### Language Selection

**FR/EN buttons** in the top right of the header allow instant language switching.

### Available Tabs

1. **Overview** - Complete system information
2. **LEDs** - Built-in LED and NeoPixel tests
3. **Screens** - TFT and OLED tests
4. **Advanced Tests** - ADC, Touch, PWM, SPI, Partitions, Stress Test
5. **GPIO** - Automatic all GPIO testing
6. **WiFi** - WiFi network scanner
7. **Performance** - CPU/Memory benchmarks
8. **Export** - TXT, JSON, CSV, Printable version

## 🛠️ Customization

### Change Default Language

In `languages.h`:
```cpp
Language currentLanguage = LANG_EN; // For English by default
```

### Add a New Language

1. Define the enum in `languages.h`:
```cpp
enum Language {
  LANG_FR = 0,
  LANG_EN = 1,
  LANG_ES = 2  // Spanish
};
```

2. Create the translation structure:
```cpp
const Translations LANG_ES_TEXTS = {
  "Diagnóstico ESP32 Completo",
  "v",
  // ... all translations
};
```

3. Modify the `T()` function:
```cpp
const Translations& T() {
  switch(currentLanguage) {
    case LANG_FR: return LANG_FR_TEXTS;
    case LANG_EN: return LANG_EN_TEXTS;
    case LANG_ES: return LANG_ES_TEXTS;
    default: return LANG_FR_TEXTS;
  }
}
```

## 📊 REST API

**New routes in v2.4:**
```
GET /api/set-language?lang=fr    → Change language (fr/en)
GET /api/get-translations        → Get JSON translations
```

**Existing routes:**
```
GET /                            → Web interface
GET /api/test-gpio               → GPIO test
GET /api/wifi-scan               → WiFi scanner
GET /api/benchmark               → Benchmarks
GET /export/txt                  → TXT export
GET /export/json                 → JSON export
GET /export/csv                  → CSV export
```

## 🐛 Troubleshooting

### Language switching doesn't work

1. Verify that `languages.h` is in the folder
2. Recompile completely (Clean then Upload)
3. Clear browser cache (Ctrl+F5)

### Untranslated texts

If some texts remain in French:
- Check the `Translations` structure in `languages.h`
- Ensure all keys are defined
- Reload the page

### Compilation error "T() not found"

- Verify that `#include "languages.h"` is present
- The `languages.h` file must be in the same folder as the `.ino`

## 📈 Performance

- **Startup time**: ~5s
- **Web page generation**: ~200ms
- **Language switching**: <100ms (no reload)
- **Heap memory used**: ~250KB
- **Multilingual overhead**: ~15KB Flash

## 🔒 Security

⚠️ Local/development use only.

**Never share `config.h` with your WiFi credentials.**

## 📁 Project Structure v2.4

```
Test_ESP32_UNIVERSAL_Travail/
├── Test_ESP32_UNIVERSAL_Travail.ino  (main code)
├── languages.h                       (translation system - NEW)
├── config.h.example                  (template)
├── config.h                          (your credentials - gitignore)
├── README.md                         (this file)
└── .gitignore                        (excludes config.h)
```

## 🔄 Changelog

### v2.4 (2025) - MULTILINGUAL
- ✨ **Complete multilingual system** (FR/EN)
- ✨ **Dynamic language switching** without reload
- ✨ New `languages.h` file with translations
- ✨ FR/EN buttons in interface
- ✨ REST API for language switching
- ✨ All v2.3 features preserved
- 📚 Updated README with multilingual guide

### v2.3 (2025)
- OLED 0.96" test with 10 animations
- Dynamic I2C pin configuration
- TXT/JSON/CSV/Print exports
- `server.sendContent()` support
- WiFi externalized to `config.h`

## 📝 License

Free to use, modify and distribute.

## 🤝 Contributing

**To add a language:**
1. Fork the project
2. Add your language in `languages.h`
3. Test all screens
4. Pull Request with screenshots

## 🎓 Credits

Developed for the ESP32 community.

**v2.4 - Multilingual system**
**v2.3 - Comprehensive base**

---

**Current version**: 2.4 Multilingual  
**Last update**: October 2025  
**Available languages**: French (default), English  
**Support**: ESP32 Arduino Core 3.1.3+

🌐 **Access**: http://ESP32-test.local  
🇫🇷🇬🇧 **Language switching**: FR/EN buttons in interface
```

```
**Version Française**
---
## **README.md - English Version (v2.4)**
## **README.md mis à jour (version 2.4)**

```markdown
# ESP32 Diagnostic Complet v2.4 - Multilingue

## 📋 Description

Outil de diagnostic exhaustif **multilingue** pour microcontrôleurs ESP32, accessible via interface web. Teste automatiquement tous les composants matériels, analyse la mémoire, scanne les périphériques et génère des rapports détaillés.

**Nouveauté v2.4** : Interface disponible en **Français** et **Anglais** avec changement de langue **dynamique sans rechargement de page**.

## ✨ Fonctionnalités

### 🌍 Interface Multilingue (NOUVEAU v2.4)
- **Français** (langue par défaut)
- **Anglais**
- **Changement dynamique** : Boutons FR/EN dans l'interface
- **Mise à jour en temps réel** : Aucun rechargement nécessaire
- **Traductions complètes** : Tous les textes, labels, messages et statuts

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
- **Sélecteur de langue** - Boutons FR/EN en haut à droite
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

**Obligatoires (via Gestionnaire de bibliothèques) :**
```
1. Adafruit BusIO
2. Adafruit GFX Library
3. Adafruit SSD1306
4. Adafruit NeoPixel
5. TFT_eSPI
```

**Incluses avec ESP32 Core :**
```
WiFi, WebServer, ESPmDNS, Wire, SPI
esp_chip_info, esp_mac, esp_flash, esp_heap_caps, esp_partition
```

## 🚀 Installation

### 1. Téléchargement

Téléchargez ou clonez ce projet dans votre dossier Arduino.

### 2. Fichiers requis

**Structure du projet :**
```
Test_ESP32_UNIVERSAL_Travail/
├── Test_ESP32_UNIVERSAL_Travail.ino  (fichier principal)
├── languages.h                       (NOUVEAU - traductions)
├── config.h.example                  (template configuration)
├── config.h                          (vos identifiants - à créer)
└── README.md
```

### 3. Configuration WiFi

**IMPORTANT :** Créez un fichier `config.h` dans le même dossier que le `.ino`

**Option A - Renommez le fichier Exemple de fichier exemple-config.h :**
Renommez le fichier exemple-config.h en config.h
Editez le fichier et ajoutez votre ou vos réseaux wifi en renseignant les valeurs

```cpp
const char* WIFI_SSID_1 = "SSID1";
const char* WIFI_PASS_1 = "MotDePasse1";

// Ajoutez autant de réseaux que nécessaire
// const char* WIFI_SSID_2 = "SSID2";
// const char* WIFI_PASS_2 = "MotDePasse2";

// const char* WIFI_SSID_3 = "SSID3";
// const char* WIFI_PASS_3 = "MotDePasse3";
```


**Option B - Créer manuellement `config.h` :**
```cpp
#ifndef CONFIG_H
#define CONFIG_H

// ========== CONFIGURATION WIFI ==========
const char* WIFI_SSID_1 = "SSID1";
const char* WIFI_PASS_1 = "MotDePasse1";

// Ajoutez autant de réseaux que nécessaire
// const char* WIFI_SSID_2 = "SSID2";
// const char* WIFI_PASS_2 = "MotDePasse2";

// const char* WIFI_SSID_3 = "SSID3";
// const char* WIFI_PASS_3 = "MotDePasse3";

#endif
```

Remplacez `VotreSSID` et `VotreMotDePasse` par vos identifiants WiFi réels.


### 4. Copier languages.h

Copiez le fichier `languages.h` fourni dans le même dossier que le `.ino`.

### 5. Configuration matérielle

**PSRAM (si disponible) :**
- ESP32-S3 : `Outils → PSRAM → OPI PSRAM`
- ESP32 : `Outils → PSRAM → Enabled`

**Flash Size :**
```
Outils → Flash Size → [Taille réelle de votre carte]
```

### 6. Compilation et téléversement

```
Croquis → Téléverser
```

### 7. Accès à l'interface

- **http://ESP32-test.local** (mDNS)
- **http://[IP_AFFICHEE]** (IP directe)

## 🌐 Utilisation Multilingue

### Changement de langue

**Dans l'interface web :**
1. Cliquez sur le bouton **FR** ou **EN** en haut à droite
2. L'interface se met à jour **instantanément**
3. Aucun rechargement nécessaire

**Par défaut :** Interface en français

**Via API :**
```bash
# Passer en anglais
curl http://ESP32-test.local/api/set-language?lang=en

# Passer en français
curl http://ESP32-test.local/api/set-language?lang=fr

# Obtenir les traductions courantes
curl http://ESP32-test.local/api/get-translations
```

### Textes traduits

**Tous les éléments sont traduits :**
- Titres de sections
- Labels et descriptions
- Messages de statut (OK, FAIL, Test...)
- Boutons d'action
- Messages d'erreur
- Qualités de signal (Excellent, Bon, Faible...)
- Raisons de reset

## 🔧 Configuration Pins

### Pins par défaut

**I2C (OLED) - Modifiable via web :**
| Modèle   | SDA | SCL |
|----------|-----|-----|
| ESP32    | 21  | 22  |
| ESP32-S2 | 8   | 9   |
| ESP32-S3 | 8   | 9   |
| ESP32-C3 | 5   | 6   |

**TFT SPI (fixes) :**
```
MOSI: 45, SCLK: 3, CS: 14, DC: 47, RST: 21, MISO: 46
```

**LED Intégrée (auto-détectée) :**
- ESP32-S3 : GPIO 2
- ESP32-C3 : GPIO 8
- ESP32 : GPIO 2

**NeoPixel (auto-détectée) :**
- ESP32-S3 : GPIO 48
- ESP32-C3 : GPIO 8

## 📖 Utilisation

### Sélection de langue

**Boutons FR/EN** en haut à droite du header permettent de changer la langue instantanément.

### Onglets disponibles

1. **Vue d'ensemble** - Informations complètes système
2. **LEDs** - Tests LED intégrée et NeoPixel
3. **Écrans** - Tests TFT et OLED
4. **Tests Avancés** - ADC, Touch, PWM, SPI, Partitions, Stress Test
5. **GPIO** - Test automatique tous GPIO
6. **WiFi** - Scanner réseaux WiFi
7. **Performance** - Benchmarks CPU/Mémoire
8. **Export** - TXT, JSON, CSV, Version imprimable

## 🛠️ Personnalisation

### Modifier la langue par défaut

Dans `languages.h` :
```cpp
Language currentLanguage = LANG_EN; // Pour anglais par défaut
```

### Ajouter une nouvelle langue

1. Définir l'enum dans `languages.h` :
```cpp
enum Language {
  LANG_FR = 0,
  LANG_EN = 1,
  LANG_ES = 2  // Espagnol
};
```

2. Créer la structure de traductions :
```cpp
const Translations LANG_ES_TEXTS = {
  "Diagnóstico ESP32 Completo",
  "v",
  // ... toutes les traductions
};
```

3. Modifier la fonction `T()` :
```cpp
const Translations& T() {
  switch(currentLanguage) {
    case LANG_FR: return LANG_FR_TEXTS;
    case LANG_EN: return LANG_EN_TEXTS;
    case LANG_ES: return LANG_ES_TEXTS;
    default: return LANG_FR_TEXTS;
  }
}
```

## 📊 API REST

**Nouvelles routes v2.4 :**
```
GET /api/set-language?lang=fr    → Change langue (fr/en)
GET /api/get-translations        → Obtient traductions JSON
```

**Routes existantes :**
```
GET /                            → Interface web
GET /api/test-gpio               → Test GPIO
GET /api/wifi-scan               → Scanner WiFi
GET /api/benchmark               → Benchmarks
GET /export/txt                  → Export TXT
GET /export/json                 → Export JSON
GET /export/csv                  → Export CSV
```

## 🐛 Dépannage

### Le changement de langue ne fonctionne pas

1. Vérifiez que `languages.h` est bien dans le dossier
2. Recompilez complètement (Clean puis Upload)
3. Videz le cache du navigateur (Ctrl+F5)

### Textes non traduits

Si certains textes restent en français :
- Vérifiez la structure `Translations` dans `languages.h`
- Assurez-vous que toutes les clés sont définies
- Rechargez la page

### Erreur compilation "T() not found"

- Vérifiez que `#include "languages.h"` est présent
- Le fichier `languages.h` doit être dans le même dossier que le `.ino`

## 📈 Performances

- **Temps de démarrage** : ~5s
- **Génération page web** : ~200ms
- **Changement de langue** : <100ms (sans rechargement)
- **Mémoire heap utilisée** : ~250KB
- **Overhead multilingue** : ~15KB Flash

## 🔒 Sécurité

⚠️ Usage local/développement uniquement.

**Ne jamais partager `config.h` avec vos identifiants WiFi.**

## 📁 Structure du Projet v2.4

```
Test_ESP32_UNIVERSAL_Travail/
├── Test_ESP32_UNIVERSAL_Travail.ino  (code principal)
├── languages.h                       (système traduction - NOUVEAU)
├── config.h.example                  (template)
├── config.h                          (vos identifiants - gitignore)
├── README.md                         (ce fichier)
└── .gitignore                        (exclut config.h)
```

## 🔄 Changelog

### v2.4 (2025) - MULTILINGUE
- ✨ **Système multilingue complet** (FR/EN)
- ✨ **Changement de langue dynamique** sans rechargement
- ✨ Nouveau fichier `languages.h` avec traductions
- ✨ Boutons FR/EN dans l'interface
- ✨ API REST pour changement de langue
- ✨ Toutes les fonctionnalités v2.3 préservées
- 📚 README mis à jour avec guide multilingue

### v2.3 (2025)
- Test OLED 0.96" avec 10 animations
- Configuration pins I2C dynamique
- Exports TXT/JSON/CSV/Print
- Support `server.sendContent()`
- Externalisation WiFi dans `config.h`

## 📝 Licence

Code libre d'utilisation, modification et distribution.

## 🤝 Contribution

**Pour ajouter une langue :**
1. Fork le projet
2. Ajoutez votre langue dans `languages.h`
3. Testez tous les écrans
4. Pull Request avec captures d'écran

## 🎓 Crédits

Développé pour la communauté ESP32.

**v2.4 - Système multilingue**
**v2.3 - Base exhaustive**

---

**Version actuelle** : 2.4 Multilingue  
**Dernière mise à jour** : Octobre 2025  
**Langues disponibles** : Français (défaut), Anglais  
**Support** : ESP32 Arduino Core 3.1.3+

🌐 **Accès** : http://ESP32-test.local  
🇫🇷🇬🇧 **Changement de langue** : Boutons FR/EN dans l'interface
```