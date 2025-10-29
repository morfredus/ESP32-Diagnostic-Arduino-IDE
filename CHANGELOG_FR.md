# Changelog (FR)

Toutes les évolutions notables d'ESP32 Diagnostic Suite sont documentées ici. Ce projet suit [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Version 3.4.0] - 2025-10-29
### Ajouts
- Mutualisation des prototypes des helpers JSON afin d'exposer des signatures cohérentes entre modules de diagnostic.

### Corrections
- Résolution des échecs de compilation causés par des déclarations de helpers JSON divergentes selon les modules.
- Rétablissement des conversions `String` sûres dans la chaîne de traduction après l'optimisation du catalogue.

### Améliorations
- Unification des builders de réponses JSON HTTP et des handlers périphériques pour réduire les duplications dans les exports.
- Harmonisation des libellés bilingues et retrait du diagnostic touchpad historique de la séquence par défaut.
- Mise à jour complète de la documentation FR/EN afin de refléter la stabilisation de la maintenance 3.3.x.

---

## [Version 3.3.0] - 2025-10-27
### Ajouts
- Néant.

### Corrections
- Suppression des bannières de commentaires « NEW FEATURE » devenues trompeuses pour les passages de maintenance.

### Améliorations
- Consolidation des notes historiques « Version de dev » dans une constante firmware afin de préserver la mémoire du projet sans dupliquer les bannières.
- Synchronisation du commentaire d'en-tête et de la macro `DIAGNOSTIC_VERSION` sur 3.3.0 avec une documentation mise à jour autour de cette maintenance.

---

## [Version 3.2.0] - 2025-10-29
### Ajouts
- Documentation de la requête `/api/memory-details` et des alertes de fragmentation issues de la dernière campagne de tests.
- Intégration, dans les guides d'utilisation et de dépannage, de la checklist Bluetooth® et Wi-Fi validée lors du débogage post-release.

### Corrections
- Remplacement des mentions 3.1.19 restantes dans les README et guides afin d'aligner bannière, documentation et changelog bilingue sur 3.2.0.
- Clarification des consignes de changement de langue pour éviter les instructions obsolètes lors de la vérification de `/api/set-language`.

### Améliorations
- Guides d'installation, de configuration et d'utilisation étoffés avec des étapes de vérification concrètes (exports mémoire, widget BLE, métadonnées de scan Wi-Fi).
- Guide de contribution renforcé pour rappeler la synchronisation systématique des contenus FR/EN lors des maintenances.

---

## [Version 3.1.19] - 2025-10-28
### Ajouts
- Néant.

### Corrections
- Mise à jour de l'ensemble des guides pour référencer le duo bilingue (`CHANGELOG.md` et `CHANGELOG_FR.md`) et conserver la cohérence FR/EN.

### Améliorations
- Scission du journal des modifications en fichiers dédiés anglais et français afin de faciliter les maintenances ciblées.
- Rafraîchissement du bandeau de version, de `DIAGNOSTIC_VERSION` et des encarts de version 3.1.19 dans toute la documentation.

---

## [Version 3.1.18] - 2025-10-27
### Ajouts
- Néant.

### Corrections
- Validation de `/api/set-language` pour renvoyer immédiatement HTTP 200 sur `fr`/`en` et HTTP 400 sur les codes non pris en charge, supprimant les réponses ambiguës.
- Synchronisation des journaux série et exports avec les nouvelles réponses JSON du changement de langue.

### Améliorations
- Réservation anticipée du tampon de `jsonEscape` afin de réduire les allocations pendant la génération d'exports.
- Inclusion explicite de `<cstring>` pour assurer la compatibilité des fonctions de chaîne C sur les toolchains Arduino récentes.
- Documentation FR/EN alignée sur la maintenance 3.1.18 (README, guides d'installation, configuration, utilisation, architecture, contribution et dépannage).

---

## [Version 3.1.16] - 2025-10-27
### Ajouts
- Bandeau sticky unifié affichant version, statut Wi-Fi/Bluetooth et lien d'accès direct depuis le tableau de bord web.
- Commandes Bluetooth® (activation, renommage, réinitialisation) exposées dans l'interface et via les endpoints REST dédiés.

### Corrections
- Délégation d'événements repositionnée pour la navigation par onglets afin de restaurer la sélection après rechargement dynamique.
- Actualisation des traductions sur les onglets et éléments dynamiques après un changement de langue pour éviter les libellés incohérents.
- Rétablissement du libellé « Non testé » sur les diagnostics additionnels dans l'interface, l'API et les exports.

### Améliorations
- Réponses de scan Wi-Fi enrichies (BSSID, bande, largeur de canal, mode PHY) pour faciliter l'analyse radio.
- Préallocation étendue de `htmlEscape` afin de limiter les réallocations lors du streaming HTML et des exports.
- Synchronisation du commentaire d'en-tête et de `DIAGNOSTIC_VERSION` sur 3.1.16 pour aligner journaux et fichiers générés.

---

## [Version 3.1.15-maint] - 2025-10-26
### Ajouts
- Néant.

### Corrections
- Uniformisation du libellé « Non testé » pour les diagnostics additionnels (ADC, tactile, PWM, stress) dans l'interface, les exports et l'API.
- Synchronisation du commentaire d'en-tête et de `DIAGNOSTIC_VERSION` sur 3.1.15-maint afin d'afficher le bon numéro dans les journaux et fichiers exportés.

### Améliorations
- Extension de la préallocation de `htmlEscape` pour réduire les réallocations lors de l'échappement des chaînes HTML générées.
- Documentation FR/EN actualisée (README, guides d'installation, configuration, utilisation, architecture, contribution et dépannage) pour refléter la maintenance 3.1.15-maint et les vérifications associées.

---

## [Version 3.1.14-maint] - 2025-10-26
### Ajouts
- Néant.

### Corrections
- Rétablissement des libellés "Non testé" pour les diagnostics additionnels afin d'assurer une interface cohérente en français.
- Alignement du commentaire de version et de `DIAGNOSTIC_VERSION` sur la révision 3.1.14-maint pour refléter la maintenance en cours.

### Améliorations
- Augmentation de la réserve mémoire utilisée par `htmlEscape` pour limiter les réallocations lors de l'échappement des chaînes.

---

## [3.1.1] - 2025-10-26
### Corrections
- Suppression des commentaires obsolètes et marqueurs internes pour clarifier la maintenance du firmware.

### Documentation
- Mise à jour des guides FR/EN, exports et constantes pour refléter le passage en version 3.1.1.

---

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
