# ESP32 Diagnostic Suite – FAQ (v3.3.11)

## Généralités
**Q : Quelles cartes sont officiellement supportées ?**
R : ESP32, ESP32-S2, ESP32-S3, ESP32-C3, ESP32-C6 et ESP32-H2. Voir [FEATURE_MATRIX_FR.md](FEATURE_MATRIX_FR.md) pour le détail des capacités.

**Q : Puis-je lancer les diagnostics sans Wi-Fi ?**
R : Oui. La suite fonctionne en mode hotspot hors ligne ou via commandes série USB. Les modules réseau sont simplement ignorés.

**Q : Existe-t-il un mécanisme OTA ?**
R : Pas en natif. Vous pouvez intégrer le binaire à votre chargeur OTA. Consultez [BUILD_AND_DEPLOY_FR.md](BUILD_AND_DEPLOY_FR.md) pour la préparation.

## Installation & configuration
**Q : Où stocker les identifiants Wi-Fi ?**
R : Copier `wifi-config-example.h` en `wifi-config.h`, renseigner vos SSID/mots de passe et exclure le fichier du dépôt.

**Q : Comment activer/désactiver les modules optionnels ?**
R : Ajuster les flags dans `config.h` ou utiliser l'endpoint REST `POST /api/modules/<id>/toggle`. Voir [DIAGNOSTIC_MODULES_FR.md](DIAGNOSTIC_MODULES_FR.md).

**Q : Puis-je changer la langue par défaut ?**
R : Oui, définir `DEFAULT_LANGUAGE` à `LANG_FR` ou `LANG_EN` dans `config.h`.

## Exécution des diagnostics
**Q : Différence entre diagnostic complet et rapide ?**
R : Le complet exécute tous les modules. Le rapide ignore les stress tests périphériques et se concentre sur connectivité, mémoire et reporting.

**Q : Durée d'un cycle complet ?**
R : Sur ESP32-S3 connecté en Wi-Fi, compter ~90 secondes incluant les benchmarks réseau et tests OLED/NeoPixel.

**Q : Où sont stockés les rapports ?**
R : En RAM jusqu'à l'export. Utiliser le tableau de bord ou `/api/report` immédiatement après la fin du cycle.

## Dépannage
**Q : Le module OLED est indisponible.**
R : Vérifier le câblage I2C (SDA/SCL), l'adresse 0x3C et l'activation `ENABLE_OLED_TESTS`.

**Q : Les tests NeoPixel échouent sur ESP32-C3/C6.**
R : Remapper la broche LED vers GPIO 8 ou 9 et mettre à jour `config.h`. Certaines cartes nécessitent une alimentation dédiée pour le ruban LED.

**Q : L'API REST renvoie 409.**
R : Un autre cycle est en cours. Attendre la fin ou appeler `POST /api/stop` avant de relancer.

## Développement
**Q : Comment ajouter un module personnalisé ?**
R : Implémenter la logique dans `registerCustomDiagnostics()` dans `ESP32-Diagnostic.ino`. Émettre PASS/WARN/FAIL et alimenter le générateur de rapport.

**Q : Des tests unitaires existent-ils ?**
R : Le projet privilégie les tests d'intégration matériels. Utiliser le tableau de bord et les exports pour valider le comportement.

**Q : Comment contribuer aux traductions ?**
R : Soumettre une pull request modifiant `languages.h` en gardant les versions EN/FR synchronisées.

## Support
**Q : Où signaler un bug ?**
R : Sur le [tracker GitHub](https://github.com/ESP32-Diagnostic/ESP32-Diagnostic/issues) en joignant le dernier rapport JSON.

**Q : Un support commercial est-il disponible ?**
R : Non officiel. Les contributions communautaires sont encouragées via GitHub discussions et pull requests.
