# Contribuer (FR)

Les contributions qui améliorent la fiabilité du diagnostic et l'expérience utilisateur sont les bienvenues.

## Contributions de code
1. **Cadrage** – concentrez-vous sur les corrections de bugs, la précision des diagnostics et les améliorations UI/UX.
2. **Branches** – créez une branche dédiée à partir de `dev/v3` pour toute évolution de code.
3. **Style** – respectez les conventions existantes dans `ESP32-Diagnostic.ino` et les en-têtes associés. N'encapsulez jamais les imports dans des blocs `try/catch`.
4. **Traductions** – mettez à jour les chaînes FR et EN dans `languages.h` pour toute modification visible.
5. **Tests** – fournissez des logs série, captures d'écran ou toute preuve matérielle du bon fonctionnement.

## Mises à jour de documentation
- Gardez la documentation anglaise et française alignée.
- Mentionnez la version du firmware vérifiée lors de la mise à jour d'un guide.
- Utilisez un ton technique et concis sans promettre de fonctionnalités non implémentées.

## Signalement d'anomalies
Incluez dans chaque ticket :
- Modèle et révision de la carte.
- Version de l'Arduino IDE et de l'Arduino Core ESP32.
- Étapes de reproduction avec comportement attendu vs observé.
- Logs série ou captures d'écran pertinents.

## Pull requests
- Référencez l'issue concernée si elle existe.
- Détaillez les tests réalisés (manuels ou automatisés).
- Décrivez toute évolution UI et ajoutez des captures pour les changements visuels.

Merci de soutenir la communauté ESP32 !
