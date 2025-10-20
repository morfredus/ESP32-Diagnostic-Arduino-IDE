<!--
Disponible en plusieurs langues :
- Français : README.fr.md
- English: README.md
-->
# Diagnostic Complet ESP32 v2.8.11

[🇬🇧 English version](README.md) | 🇫🇷 Version française

> **Note :** Ce document présente la version française officielle de la documentation. La traduction anglaise est accessible via le lien ci-dessus.

---

## 📋 Description

Outil **multilingue** de diagnostic pour microcontrôleurs ESP32, accessible via une interface web moderne. Il automatise les tests matériels (LED, NeoPixel, OLED, GPIO, ADC, Touch, PWM), dresse l'inventaire mémoire, scanne les bus I2C/SPI et génère des rapports détaillés.

**Nouveautés de la v2.8.11** : le bandeau sans fil interprète désormais correctement les champs booléens renvoyés par le firmware, garantissant une correspondance exacte entre les voyants WiFi/Bluetooth et l'état réel de connectivité. Le guide d'installation français a également été renommé en `USER_GUIDE.fr.md` pour être plus facilement identifié.

## ✨ Fonctionnalités principales

### 🌍 Interface multilingue (depuis v2.4)
- **Français** (par défaut)
- **Anglais**
- **Bascule dynamique** : boutons FR/EN dans le bandeau
- **Rechargement automatique** après changement de langue
- **Traduction complète** des libellés, messages et exports

### Tests matériels
- **LED intégrée** – configuration automatique du GPIO, tests Blink/Fade/Off
- **NeoPixel / WS2812B** – gestion multi-LED avec motifs et couleurs personnalisées
- **Écran OLED I2C 0.96"** – 10 scénarios de test + étapes manuelles
- **GPIO** – campagne automatique avec rappel « ❌ ≠ panne assurée »
- **ADC / Touch / PWM** – mesures détaillées avec affichage inline

### Analyse système & réseau
- **Mémoire détaillée** – Flash, PSRAM (OPI/QSPI), SRAM, fragmentation
- **Scanner WiFi** – RSSI, canal, chiffrement, tri visuel
- **Scanner I2C & infos SPI** – adresses détectées et bus disponibles
- **Benchmarks & stress mémoire** – CPU (MFLOPS), vitesse mémoire, max allocations
- **Exports** – rapports TXT/JSON/CSV et version imprimable

### Interface web
- **8 onglets** : Vue d'ensemble, LEDs, Écrans, Tests avancés, GPIO, Sans fil, Performance, Export
- **Bandeau sans fil** (v2.8.6, affiné en v2.8.11) – indicateurs WiFi/Bluetooth temps réel, conscience STA/AP et rappels si le Bluetooth est désactivé à la compilation
- **Statuts uniformes** (v2.8.7 affiné en v2.8.8) – préfixes ⏳/✅/❌ cohérents jusqu'à la fin des actions
- **Responsive & sans pop-up** – compatible mobile/tablette/desktop
- **API REST** – endpoints JSON pour intégration externe

## 📚 Documentation

- [`README.md`](README.md) – version anglaise complète.
- [`USER_GUIDE.md`](USER_GUIDE.md) – checklist d'installation et de compilation (anglais).
- [`USER_GUIDE.fr.md`](USER_GUIDE.fr.md) – mode d'emploi détaillé en français (pré-requis, bibliothèques, compilation Arduino IDE).
- [`CONFIG_REFERENCE.md`](CONFIG_REFERENCE.md) – référence (EN) des paramètres personnalisables (`config.h`, `wifi-config.h`).
- [`CONFIG_REFERENCE.fr.md`](CONFIG_REFERENCE.fr.md) – version française de la référence de configuration.

## 🎯 Cartes compatibles

Validées sur :
- ✅ **ESP32** (WROOM)
- ✅ **ESP32-S2**
- ✅ **ESP32-S3** (recommandée avec PSRAM OPI)
- ✅ **ESP32-C3**
- ⚠️ **ESP32-C6 / H2** (non testées mais supportées par le core 3.3.2)

## 📦 Pré-requis

### Matériel
- Carte ESP32
- Câble USB
- Optionnel : bandeau NeoPixel, écran OLED I2C, modules I2C/SPI

### Logiciel
- **Arduino IDE** 2.x
- **Arduino-ESP32 Core** 3.3.2 (Gestionnaire de cartes)

### Bibliothèques Arduino indispensables
```
Adafruit BusIO
Adafruit GFX Library
Adafruit SSD1306
Adafruit NeoPixel
```
(installables via le Library Manager)

Bibliothèques incluses avec le core : `WiFi`, `WebServer`, `ESPmDNS`, `Wire`, `SPI`, `esp_*`.

## 🚀 Installation rapide

1. **Cloner ou télécharger** le dossier `ESP32-Diagnostic/` dans votre espace Arduino.
2. **Dupliquer** `wifi-config.example.h` en `wifi-config.h` (fichier ignoré par Git) puis renseigner vos réseaux WiFi :
   ```cpp
   static const WiFiCredential WIFI_NETWORKS[] = {
     {"MonRéseau", "MotDePasse"},
     {"Secours", "MotDePasse2"}
   };
   ```
   - ✅ Conservez la virgule de fin sur chaque ligne (y compris la dernière) afin que l'ajout ou le décommentage d'un réseau n'entraîne jamais d'erreur de compilation.
3. **Adapter** `config.h` : broches (`CUSTOM_LED_PIN`, `DEFAULT_I2C_SDA/SCL`), nom mDNS (`MDNS_HOSTNAME`), activation de l'auto-test Bluetooth et intervalle `WIRELESS_STATUS_REFRESH_MS`.
4. **Ouvrir** `ESP32-Diagnostic.ino` dans Arduino IDE, sélectionner votre carte ESP32 et le port série.
5. **Vérifier** dans *Outils → Bluetooth* que la pile est activée si vous souhaitez tester le Bluetooth.
6. **Téléverser** le sketch.

### Structure du projet
```
ESP32-Diagnostic/
├── ESP32-Diagnostic.ino      (programme principal)
├── app_script.h              (générateur JS – bandeau sans fil, aides auto-config)
├── languages.h               (système de traduction)
├── config.h                  (paramètres matériels personnalisables)
├── wifi-config.example.h     (modèle WiFi → copier en wifi-config.h)
├── README.md / README.fr.md  (documentation principale)
├── USER_GUIDE.md / USER_GUIDE.fr.md
├── CONFIG_REFERENCE.md / CONFIG_REFERENCE.fr.md
└── .gitignore                (ignore wifi-config.h)
```

## 🔄 Historique des versions récentes

- **v2.8.11 – 20/10/2025** : normalisation des booléens du bandeau sans fil (voyants WiFi/Bluetooth cohérents) et renommage du guide français en `USER_GUIDE.fr.md`.
- **v2.8.10 – 20/10/2025** : modèle `wifi-config.h` sécurisé (virgule finale par défaut) et détection WiFi du bandeau renforcée lorsque le Bluetooth est désactivé.
- **v2.8.9 – 20/10/2025** : refonte de la configuration (`config.h`, `wifi-config.h`), nouvelle référence FR/EN et bannière sans fil corrigée (rafraîchissement paramétrable).
- **v2.8.8 – 20/10/2025** : correction des voyants WiFi/Bluetooth (STA/AP, Bluetooth désactivé, purge des valeurs périmées) et publication des guides renommés (`USER_GUIDE` / `GUIDE_UTILISATION`).
- **v2.8.7 – 19/10/2025** : statuts inline harmonisés avec préfixes ⏳/✅/❌ et messages « Test en cours... » jusqu'à la fin réelle.
- **v2.8.6 – 18/10/2025** : bandeau sans fil fixé en haut de page et auto-application des configurations LED/NeoPixel/OLED.

L'historique complet est disponible dans [`CHANGELOG.md`](CHANGELOG.md).

## ❓ Support & contributions

- Vérifiez la section « Mode d'emploi » correspondante avant d'ouvrir un ticket.
- Rapportez les anomalies en précisant la carte, la version du core et le scénario.
- Les contributions sont les bienvenues (traductions supplémentaires, nouveaux tests, correctifs).

Bon diagnostic !
