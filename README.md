<!--
Available in multiple languages:
- Français : README.md
- English : README.en.md
-->
# Diagnostic ESP32 Complet v3.0.0

🇫🇷 Version française | [🇬🇧 English Version](README.en.md)

> **Note :** Ceci est la version française. For the English version, [click here](README.en.md).

---

## 📋 Description

Outil de diagnostic exhaustif **multilingue** pour microcontrôleurs ESP32, accessible via interface web. Teste automatiquement tous les composants matériels, analyse la mémoire, scanne les périphériques et génère des rapports détaillés.

**Nouveautés v3.0.0** : Version officielle validée avec le **core ESP32 Arduino 3.3.2** et documentation bilingue actualisée.

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
- **mDNS** - Accès via http://ESP32-Diagnostic.local
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
- **ESP32 Arduino Core** 3.3.2 ou supérieur

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
ESP32-Diagnostic/
├── ESP32-Diagnostic.ino              (fichier principal)
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

- **http://ESP32-Diagnostic.local** (mDNS)
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
curl http://ESP32-Diagnostic.local/api/set-language?lang=en

# Passer en français
curl http://ESP32-Diagnostic.local/api/set-language?lang=fr

# Obtenir les traductions courantes
curl http://ESP32-Diagnostic.local/api/get-translations
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

## 📁 Structure du Projet v3.0.0

```
ESP32-Diagnostic/
├── ESP32-Diagnostic.ino              (code principal)
├── languages.h                       (système traduction - NOUVEAU)
├── config.h.example                  (template)
├── config.h                          (vos identifiants - gitignore)
├── README.md                         (ce fichier)
├── README.en.md                      (version anglaise)
└── .gitignore                        (exclut config.h)
```

## 🔄 Changelog

### v3.0.0 (2025) - VERSION OFFICIELLE
- ✅ **Compatibilité validée avec le core Arduino 3.3.2**
- 📘 **Documentation bilingue actualisée** (FR/EN)
- ✨ Exports multilingues conservés (TXT/JSON/CSV/Impression)

### v2.4 (2025) - MULTILINGUE
- ✨ **Système multilingue complet** (FR/EN)
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

**v3.0.0 - Support officiel du core 3.3.2**
**v2.4 - Système multilingue**
**v2.3 - Base exhaustive**

---

**Version actuelle** : 3.0.0 Multilingue
**Dernière mise à jour** : Octobre 2025
**Langues disponibles** : Français (défaut), Anglais
**Support** : ESP32 Arduino Core 3.3.2+

🌐 **Accès** : http://ESP32-Diagnostic.local  
🇫🇷🇬🇧 **Changement de langue** : Boutons FR/EN dans l'interface
```
