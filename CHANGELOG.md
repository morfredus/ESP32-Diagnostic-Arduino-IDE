# Changelog

Toutes les évolutions notables d'ESP32 Diagnostic Suite sont documentées ici. Ce projet suit [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [3.1.0] - 2025-10-24
### Points forts
- Activation automatique du service Bluetooth® Low Energy avec diffusion native sur les cibles compatibles (ESP32, S3, C3, C6, H2).
- Tableau de bord web enrichi d'une cartouche BLE détaillant l'état, le nom du périphérique et les journaux d'appairage récents.
- Jeux de documents FR/EN entièrement réécrits couvrant installation, configuration, utilisation, architecture, dépannage et contribution.

### Fonctionnalités
- Service GATT par défaut avec renommage dynamique du périphérique via l'interface web ou la console série.
- Nouveaux messages d'état Wi-Fi contextualisés (association, DHCP, authentification, portail captif) affichés dans le bandeau supérieur et l'API REST.
- Prise en charge initiale assouplie pour les révisions récentes des SoC ESP32 (S3/C3/C6/H2) sur les modules BLE et Wi-Fi.

### Interface & API
- Navigation par onglets fiabilisée via délégation d'événements JavaScript, évitant les blocages après un rafraîchissement partiel.
- Menu supérieur responsive avec regroupement monoligne des indicateurs de statut pour les écrans < 768 px.
- Export TXT/JSON/CSV et aperçu imprimable synchronisés avec les nouveaux états BLE et Wi-Fi.

### Documentation
- Regroupement des guides dans `docs/` avec renvois croisés vers les versions anglaises et françaises.
- Ajout d'un guide de contribution détaillé (workflow Git, validation, formatage) et de procédures de dépannage spécifiques BLE/Wi-Fi.

### Corrections
- Harmonisation des messages BLE entre la console série, l'API `/api/status` et l'interface web.
- Nettoyage du JSON de traduction dynamique pour éviter les clés orphelines et incohérences de casse.
- Clarification de la détection des onglets actifs dans le bundle JavaScript, supprimant les doublons d'écouteurs.

---

## [2.6.0] - 2025-10-15
### Fonctionnalités
- Boutons manuels sur l'interface web et endpoints REST associés pour piloter individuellement chaque animation de diagnostic OLED.
- Possibilité de déclencher et d'arrêter les séquences d'affichage directement depuis la console série.

### Améliorations
- Simplification du flux de reconfiguration I²C OLED : sélection des broches SDA/SCL et vitesse directement depuis l'interface.
- Actualisation du pack de traductions (FR/EN) pour tous les nouveaux libellés OLED et états d'exécution.

### Corrections
- Suppression complète de la prise en charge TFT (firmware, dépendances Arduino, fragments UI), réduisant la taille du binaire et les avertissements de compilation.

---

## [2.5.1] - 2025-10-10
### Corrections
- Statuts PSRAM clarifiés dans le tableau de bord, les exports et les rapports imprimables (valeur détectée, fréquence, mode octal).
- Ajustement de l'ordre de détection PSRAM pour éviter les faux négatifs sur ESP32-WROVER.

### Améliorations
- Mise en avant des cartes compatibles PSRAM avec rappels pour activer l'option dans l'IDE Arduino et la documentation d'installation.

---

## [2.5.0] - 2025-10-08
### Fonctionnalités
- Exports TXT/JSON/CSV intégralement traduits (FR/EN) avec inclusion de l'horodatage ISO 8601 et du numéro de build.
- Prévisualisation imprimable alignée sur la langue sélectionnée et sur les nouveaux blocs de diagnostic.

### Améliorations
- Nommage des fichiers d'export enrichi avec le numéro de version, la date et la carte détectée.
- Révision du workflow d'export REST pour réduire les temps de génération et homogénéiser les intitulés de colonnes.

### Corrections
- Harmonisation des formats de date (localisation FR/EN) dans les exports et la vue imprimable.
- Ajout des traductions manquantes sur les boutons de téléchargement et sur les libellés de sections.

---

## [2.4.0] - 2025-10-07
### Fonctionnalités
- Interface multilingue complète FR/EN avec bascule à chaud sans rechargement de page.
- Endpoints `/api/set-language` et `/api/get-translations` permettant aux clients externes de piloter la langue.
- Fichier `languages.h` regroupant plus de 150 chaînes traduites pour l'interface, les exports et la console série.

### Améliorations
- Tous les textes (web, API, exports, logs) reposent sur le catalogue de traductions centralisé.
- Ajout d'un indicateur visuel de langue active et persistance de la préférence dans le stockage local du navigateur.

---

## [2.3.0] - 2025-10-06
### Fonctionnalités
- Batterie de 10 tests OLED 0,96" I²C (bascule contrastes, inversion, scroll, trames personnalisées) avec messages explicatifs.
- Reconfiguration dynamique des broches SDA/SCL via l'interface web et l'API pour faciliter le recâblage.

### Améliorations
- Détection automatique de l'écran à l'adresse 0x3C avec relance des tests après connexion.
- Ajout d'un module de calibration de contraste pour optimiser les OLED selon la tension d'alimentation.

---

## [2.2.0] - 2025-10-05
### Fonctionnalités
- Support des écrans TFT SPI (ST7789/ILI9341) avec diagnostics dédiés (test couleurs, patterns, fréquence SPI).
- Assistant de configuration TFT (pinout, rotation, inversion des axes) accessible via l'interface et la console série.

### Notes
- Cette fonctionnalité a été retirée en 2.6.0 pour alléger le firmware et se concentrer sur les OLED.

---

## [2.0.0] - 2025-10-03
### Fonctionnalités
- Réécriture complète du diagnostic : GPIO, ADC, pads tactiles, PWM, I2C, SPI, PSRAM, partitions, stress test, benchmarks et exports.
- Support multi-Wi-Fi via `WiFiMulti`, découverte mDNS `ESP32-Diagnostic.local` et interface web modernisée.
- Ajout de l'API REST (lancement tests, récupération de rapports) et des exports TXT/JSON/CSV.

### Documentation
- Première version du guide d'utilisation détaillant la configuration réseau, les tests disponibles et la collecte des journaux.

---

## [1.0.0] - 2025-10-01
### Fonctionnalités
- Première version publique avec interface web basique, rapport mémoire, tests GPIO/ADC élémentaires et console série interactive.
- Génération d'un tableau de bord unique regroupant la configuration Wi-Fi, les mesures de capteurs et les actions rapides.

### Notes
- Base de code initiale posant les fondations du système de diagnostics évolutif.
