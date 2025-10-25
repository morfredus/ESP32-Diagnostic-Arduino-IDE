# Changelog

Toutes les évolutions notables d'ESP32 Diagnostic Suite sont documentées ici. Ce projet suit [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Version 3.1.0] - 2025-10-20
### Ajouts
- Activation automatique du service Bluetooth® Low Energy avec diffusion native sur les cibles compatibles.
- Cartouche dédiée dans l'interface web pour visualiser l'état BLE et renommer le périphérique.
- Documentation restructurée et disponible en anglais et en français (installation, configuration, usage, architecture, dépannage, contribution).

### Corrections
- Navigation par onglets fiabilisée via délégation d'événements côté JavaScript.
- Messages de statut BLE harmonisés entre la console série et l'API.
- Nettoyage du JSON de traduction dynamique pour éviter les clés fantômes.

### Améliorations
- Menu supérieur responsive avec bandeau monoligne pour les petits écrans.
- États Wi-Fi affinés avec messages explicites lors des échecs de connexion.
- Charge BLE initiale assouplie pour les dernières révisions ESP32 (S3/C3/C6/H2).

---

## [2.6.0] - 2025-10-15
### Ajouts
- Boutons manuels pour chaque animation de diagnostic OLED via le web et l'API.

### Corrections
- Suppression complète de la prise en charge TFT (firmware, bibliothèques, UI).

### Améliorations
- Simplification du flux de reconfiguration I2C OLED et rafraîchissement des traductions.

---

## [2.5.1] - 2025-10-10
### Corrections
- Statuts PSRAM clarifiés dans l'interface, les exports et les rapports imprimables.

### Améliorations
- Mise en avant des cartes compatibles PSRAM avec conseils d'activation dans l'IDE Arduino.

---

## [2.5] - 2025-10-08
### Ajouts
- Exports TXT/JSON/CSV traduits en français et en anglais.

### Corrections
- Traductions manquantes et formats de date incohérents dans les exports.

### Améliorations
- Nommage des fichiers d'export enrichi avec le numéro de version.
- Respect de la langue sélectionnée pour l'aperçu imprimable.

---

## [2.4] - 2025-10-07
### Ajouts
- Interface multilingue complète (FR/EN) avec commutation à chaud.
- API `/api/set-language` et `/api/get-translations` pour piloter les langues.
- Fichier `languages.h` contenant plus de 150 chaînes traduites.

### Améliorations
- Tous les textes de l'interface et des exports reposent sur le système de traductions.

---

## [2.3] - 2025-10-06
### Ajouts
- Batterie de tests pour écran OLED 0,96" I2C (10 scénarios + message personnalisé).
- Reconfiguration dynamique des broches SDA/SCL via l'API.

### Améliorations
- Détection automatique de l'écran à l'adresse 0x3C et relance des tests après recâblage.

---

## [2.2] - 2025-10-05
### Ajouts
- Support des écrans TFT SPI (ST7789/ILI9341) avec diagnostics dédiés.

---

## [2.0] - 2025-10-03
### Ajouts
- Réécriture complète du diagnostic (GPIO, ADC, touch, Wi-Fi, I2C, SPI, partitions, stress test, benchmarks, exports).
- Support multi-Wi-Fi via WiFiMulti et découverte mDNS `ESP32-Diagnostic.local`.

---

## [1.0] - 2025-10-01
### Ajouts
- Première version du diagnostic ESP32 avec interface web basique et statistiques mémoire.
