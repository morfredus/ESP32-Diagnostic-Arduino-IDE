<!-- 
Available in multiple languages:
- English: README.md
- Français: README.fr.md
-->
# Diagnostic ESP32 Complet v4.1.0

🇫🇷 Version française | [🇬🇧 English Version](README.md)

> **Note :** Ceci est la version française. For the English version, [click here](README.md).

---

## 📋 Description

Outil de diagnostic exhaustif **multilingue** pour microcontrôleurs ESP32, accessible via interface web. Teste automatiquement tous les composants matériels, analyse la mémoire, scanne les périphériques et génère des rapports détaillés.

**Nouveautés v4.1.0** : Refonte complète et responsive de l'interface web tout en conservant chaque outil de diagnostic.

## ✨ Fonctionnalités

### 🌍 Interface Multilingue (NOUVEAU v2.4)
- **Français** (langue par défaut)
- **Anglais**
- **Changement dynamique** : Boutons FR/EN dans l'interface
- **Mise à jour en temps réel** : Aucun rechargement nécessaire
- **Traductions complètes** : Tous les textes, labels, messages et statuts

### Tests Matériels
- **LED intégrée** - GPIO configurable avec détection automatique de conflit quand la NeoPixel partage la broche
- **NeoPixel/WS2812B** - Support multi-LED avec effets RGB
- **Écran OLED 0.96" I2C** - Suite complète (test global, message, motifs) accessible individuellement même avant auto-détection
- **GPIO** - Test automatique de tous les GPIO disponibles
- **ADC** - Lecture de tous les canaux analogiques
- **Touch Pads** - Test capteurs tactiles capacitifs
- **PWM/LEDC** - Test modulation largeur d'impulsion

### Analyse Système
- **Mémoire détaillée** - Flash, PSRAM (OPI/QSPI), SRAM avec tests d'intégrité
- **Scanners sans fil** - Réseaux Wi-Fi (RSSI, canal, sécurité) et périphériques BLE (nom, MAC, RSSI)
- **Scan I2C** - Détection automatique périphériques (0x01-0x7F)
- **Scan SPI** - Informations bus SPI disponibles
- **Partitions Flash** - Liste complète des partitions
- **Benchmarks** - Performance CPU et mémoire
- **Stress Test** - Test limite allocation mémoire

### Interface Web
- **8 onglets** - Navigation intuitive (Vue d'ensemble, LEDs, Écrans, Tests avancés, GPIO, Sans fil, Performance, Export)
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
- Optionnel : LED NeoPixel, écran OLED

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
3. **Écrans** - Tests OLED
4. **Tests Avancés** - ADC, Touch, PWM, SPI, Partitions, Stress Test
5. **GPIO** - Test automatique tous GPIO
6. **Sans fil** - Scanners Wi-Fi et périphériques BLE
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
GET /api/ble-scan                → Scanner BLE
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

## 📁 Structure du Projet v4.1.0

```
ESP32-Diagnostic/
├── ESP32-Diagnostic.ino              (code principal)
├── languages.h                       (système traduction - NOUVEAU)
├── config.h.example                  (template)
├── config.h                          (vos identifiants - gitignore)
├── README.md                         (version Anglaise de README)
├── README.fr.md                      (ce fichier)
└── .gitignore                        (exclut config.h)
```

## 🔄 Changelog

### v4.1.0 (2025) - INTERFACE RESPONSIVE MODERNE
- 🧭 **Nouvelle interface web totalement revisitée** avec une mise en page fluide, une typographie modernisée et des cartes translucides tout en conservant chaque fonction de test.
- 📱 **Lisibilité accrue sur ordinateur comme sur mobile**, grâce à une navigation adaptative, des indicateurs de statut retravaillés et des panneaux contextuels réorganisés.

### v4.0.18 (2025) - BOUTONS WEB RÉACTIVÉS
- 🔁 **Réparation du script de scan Wi-Fi** : tous les boutons de l'interface (dont le changement de langue et la mise à jour BLE) fonctionnent à nouveau.
- 🧾 **Bannière firmware, documentation et interface affichent désormais la v4.0.18** suite à ce correctif.

### v4.0.17 (2025) - RÉSUMÉ SANS FIL TOUJOURS PRÉSENT
- ♾️ **Les cartes Wi-Fi et Bluetooth s'affichent désormais systématiquement**, même lorsque le firmware ne contient pas la pile BLE.
- 📴 **Le bouton de scan BLE se désactive automatiquement** en reprenant les mêmes messages d'indisponibilité que le moniteur série et les exports.
- 🧾 **Firmware, interface et documentation passent en v4.0.17** pour refléter ce panneau sans fil permanent.

### v4.0.16 (2025) - CARTE BLUETOOTH FIABLE & NIMBLE PAR DÉFAUT
- ♻️ **Correction du gabarit JavaScript Sans fil** afin que la carte Bluetooth s'affiche toujours sous le statut Wi-Fi.
- 📡 **Distribution systématique du script mis à jour** pour garantir que l'onglet Sans fil charge la dernière logique.
- 🧭 **Préférence automatique pour la bibliothèque NimBLE-Arduino** quand elle est disponible, tout en affichant la pile active dans les diagnostics.
- 🧾 **Firmware, documentation et interface passent en v4.0.16** avec ces améliorations Bluetooth.

### v4.0.11 (2025) - STATUT BLUETOOTH PARTOUT
- 📶 **La carte Sans fil reste visible même sans pile BLE**, avec un message clair expliquant pourquoi le Bluetooth est indisponible.
- 🧾 **Résumé Sans fil ajouté au moniteur série et aux exports TXT/JSON/CSV/impression**, reflétant les capacités Wi-Fi/BLE comme dans l'interface.
- 🛠️ **Firmware, documentation et interface passent en v4.0.11**, en désactivant automatiquement le bouton de scan BLE si la pile native n'est pas présente.

### v4.0.9 (2025) - STATUT PSRAM & RÉSUMÉ SANS FIL
- 🧠 **La vue d'ensemble et les exports détaillent désormais le support/mode PSRAM**, avec un rappel d'activation IDE identique au diagnostic série.
- 📶 **L'onglet Sans fil démarre par un résumé Wi-Fi/BLE**, indiquant la connexion active et si le BLE est compilé avant tout scan.
- 🔗 **Les endpoints `/api/overview` et `/api/memory-details` exposent les métadonnées PSRAM**, pour garder outils externes et interface dynamique synchronisés.

### v4.0.4 (2025) - GARDE CONFLIT LED + API LOCALISÉE
- 💡 **Le test de LED intégrée s'interrompt si la NeoPixel partage le GPIO** et signale le conflit sans clignoter la bande
- 🌐 **Toutes les réponses API LED/OLED sont traduites** (motif manquant, broches invalides, message personnalisé, etc.)
- 🖥️ **L'ancienne interface web bénéficie des mêmes raccourcis OLED** que la page dynamique

### v4.0.3 (2025) - PROTECTION LED + RACCOURCIS OLED
- 💡 **Le test de LED intégrée détecte les conflits NeoPixel** et bascule en "mode simple" lorsque la broche est partagée
- 🖥️ **Actions rapides OLED** pour lancer chaque démo sans relancer le test complet de 25 secondes

### v4.0.2 (2025) - CORRECTION CATALOGUE DE TRADUCTIONS
- 🧰 **Correction de l'export du catalogue de traductions** pour éviter l'erreur `String::endsWith` avec Arduino Core 3.3.2
- 🌐 **Maintien du changement de langue instantané** grâce à la distribution complète des traductions au navigateur

### v4.0.1 (2025) - ALIGNEMENT API TFT
- 🛠️ **Les endpoints TFT renvoient désormais un message traduit "fonction désactivée"** pour chaque motif demandé
- 🔄 **Actualisation automatique des statuts TFT** lors d'un changement de langue

### v4.0.0 (2025) - VERSION SANS TFT
- 🧹 **Suppression de la dépendance TFT_eSPI** tout en conservant l'onglet/API TFT
- 📡 **Scanner Wi-Fi** : toutes les méthodes d'authentification IDF et gestion des erreurs de scan
- 🧮 **Benchmark CPU** : MFLOPS calculés à partir du temps réel et du nombre d'opérations
- 🌍 **Messages mémoire/I2C** : réutilisation complète des traductions existantes

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

**v4.1.0 - Interface web moderne et responsive**
**v4.0.18 - Boutons web rétablis & version mise à jour**
**v4.0.17 - Résumé Sans fil permanent & scan BLE protégé**
**v4.0.16 - Carte Bluetooth Sans fil + priorité NimBLE**
**v4.0.11 - Statut Bluetooth partout**
**v4.0.9 - Statut PSRAM et résumé Sans fil**
**v4.0.2 - Correctif catalogue de traductions + localisation**
**v3.0.0 - Support officiel du core 3.3.2**
**v2.4 - Système multilingue**
**v2.3 - Base exhaustive**

---

**Version actuelle** : 4.1.0 Multilingue
**Dernière mise à jour** : Octobre 2025
**Langues disponibles** : Français (défaut), Anglais
**Support** : ESP32 Arduino Core 3.3.2+

🌐 **Accès** : http://ESP32-Diagnostic.local  
🇫🇷🇬🇧 **Changement de langue** : Boutons FR/EN dans l'interface
```
