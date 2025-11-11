# Contribuer (FR)

Les contributions qui améliorent la fiabilité du diagnostic et l'expérience utilisateur sont les bienvenues.

## Contributions de code
1. **Fork** – commencez depuis votre fork GitHub : [morfredus/ESP32-Diagnostic-Arduino-IDE](https://github.com/morfredus/ESP32-Diagnostic-Arduino-IDE/fork).
2. **Cadrage** – concentrez-vous sur les corrections de bugs, la précision des diagnostics et les améliorations UI/UX.
3. **Branches** – créez une branche dédiée à partir de `dev/v3` pour toute évolution de code.
4. **Style** – respectez les conventions existantes dans `ESP32-Diagnostic.ino` et les en-têtes associés. N'encapsulez jamais les imports dans des blocs `try/catch`.
5. **Traductions** – mettez à jour les chaînes FR et EN dans `languages.h` pour toute modification visible.
6. **Tests** – fournissez des logs série, captures d'écran ou toute preuve matérielle du bon fonctionnement.

## Mises à jour de documentation
- Gardez la documentation anglaise et française alignée.
- Mentionnez la version du firmware vérifiée lors de la mise à jour d'un guide.
- Utilisez un ton technique et concis sans promettre de fonctionnalités non implémentées.
- Rappelez la version 3.8.0 lors des travaux de maintenance afin de garder synchronisés l'écran Wi-Fi, les garde-fous NimBLE, le duo de changelog bilingue et `DIAGNOSTIC_VERSION`.

## Signalement d'anomalies
Déclarez vos anomalies via [GitHub Issues](https://github.com/morfredus/ESP32-Diagnostic-Arduino-IDE/issues/new/choose) et incluez :
- Modèle et révision de la carte.
- Version de l'Arduino IDE et de l'Arduino Core ESP32.
- Étapes de reproduction avec comportement attendu vs observé.
- Logs série ou captures d'écran pertinents.

## Pull requests
- Référencez l'issue concernée si elle existe.
- Détaillez les tests réalisés (manuels ou automatisés).
- Décrivez toute évolution UI et ajoutez des captures pour les changements visuels.

Merci de soutenir la communauté ESP32 !
