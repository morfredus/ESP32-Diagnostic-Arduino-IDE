# ESP32 Diagnostic Suite – Vue d'ensemble (v3.8.0)

La suite ESP32 Diagnostic fournit une boîte à outils prête pour la production afin de valider les cartes basées sur ESP32 avant déploiement. La version 3.8.0 met l'accent sur la visibilité Wi-Fi via l'écran OLED, la compatibilité BLE inter-piles et une documentation actualisée pour les derniers toolchains.

## Mission
- Proposer une méthodologie homogène pour qualifier les cartes ESP32, ESP32-S2, ESP32-S3, ESP32-C3, ESP32-C6 et ESP32-H2.
- Offrir une interface web dédiée aux diagnostics en laboratoire comme sur le terrain, avec un minimum de configuration.
- Maintenir l'alignement entre firmware, documentation et rapports exportés afin de faciliter la traçabilité des anomalies.

## Résumé des fonctionnalités
| Domaine | Points clés |
|---------|-------------|
| Tableau de bord web | Interface HTML réactive servie directement par l'ESP32 avec indicateurs temps réel, journal d'activité et exports. |
| Vérifications automatiques | Planificateurs couvrant la connectivité, la mémoire, les bus de périphériques, l'affichage et les LEDs. |
| Reporting | Téléchargements en TXT/JSON/CSV, vue imprimable et hooks REST API pour l'intégration CI. |
| Localisation | Bascule FR/EN à l'exécution côté firmware (sortie série) et interface web. |
| Extensibilité | Registre modulaire de tests, bus d'événements pour callbacks personnalisés et helpers OLED/NeoPixel optionnels. |

## Focus de la version 3.8.0
- Écran de démarrage Wi-Fi sur les cartes équipées d'OLED pour afficher association, DHCP et réussite sans ouvrir le port série.
- Durcissement des builds NimBLE grâce aux descripteurs protégés, callbacks alignés et gestion de scan sécurisée sur ESP32/ESP32-S3.
- Documentation enrichie couvrant le nouvel écran, les garde-fous BLE et la migration vers l'Arduino Core 3.3.3.

## Carte du wiki
Consultez cette carte pour accéder directement à la bonne rubrique :

- [Accueil](home_FR.md) – porte d'entrée, points forts et objectifs généraux.
- [Installation](INSTALL_FR.md) – préparation de l'environnement, toolchain, pilotes USB et méthodes de flash.
- [Configuration](CONFIG_FR.md) – identifiants Wi-Fi, langue par défaut, ordonnancement des diagnostics et options de journalisation.
- [Utilisation](USAGE_FR.md) – workflows opérateur, formats d'export et interprétation du tableau de bord.
- [Architecture](ARCHITECTURE_FR.md) – couches firmware, planification des tâches, structure mémoire et gabarits d'interface.
- [Dépannage](TROUBLESHOOTING_FR.md) – procédures de récupération, matrice des codes d'erreur et scénarios de reset sécurisé.
- [Contribution](CONTRIBUTING_FR.md) – conventions de code, processus de revue et attentes en matière de tests.
- [Matrice des fonctionnalités](FEATURE_MATRIX_FR.md) – couverture matérielle par famille de cartes et périphériques optionnels.
- [Modules de diagnostic](DIAGNOSTIC_MODULES_FR.md) – description détaillée de chaque test automatisé et des métriques produites.
- [Guide de l'interface web](WEB_INTERFACE_FR.md) – anatomie de l'UI, états des composants et directives d'accessibilité.
- [Référence REST API](API_REFERENCE_FR.md) – endpoints, payloads, limites de débit et scénarios d'automatisation.
- [Compilation & déploiement](BUILD_AND_DEPLOY_FR.md) – compilation firmware, contrôles qualité et checklist de publication.
- [Directives de sécurité](SECURITY_FR.md) – renforcement Wi-Fi, rotation des identifiants et sécurité OTA.
- [FAQ](FAQ_FR.md) – réponses rapides aux questions d'intégration, support et déploiement les plus fréquentes.

## Terminologie
- **Cycle de diagnostic** – exécution ordonnée de tous les modules activés avec agrégation en RAM jusqu'à l'export.
- **Rapport instantané** – copie figée du dernier cycle, exportable via l'interface web, la REST API ou la commande série.
- **Profil contrôleur** – ensemble de configuration pré-définie (type de carte, options d'interface) chargé au démarrage.
- **Version de maintenance** – livraison axée sur la documentation et la stabilité sans modification de la surface fonctionnelle.

## Ressources associées
- [CHANGELOG_FR.md](../CHANGELOG_FR.md) – suivi chronologique des corrections et améliorations.
- [Table de compatibilité ESP-IDF](https://docs.espressif.com/projects/esp-idf/fr/latest/esp32/) pour comparer avec l'Arduino Core.
- [Issues GitHub](https://github.com/ESP32-Diagnostic/ESP32-Diagnostic/issues) – suivi des anomalies et propositions.
