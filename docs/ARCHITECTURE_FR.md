# Architecture du firmware (FR)

## Vue d'ensemble
Le firmware est un unique sketch Arduino (`ESP32-Diagnostic.ino`) qui orchestre les diagnostics, sert l'interface web et expose des endpoints REST. Les en-têtes associés regroupent les traductions, les modèles HTML et les utilitaires de configuration.

```
ESP32-Diagnostic/
├── ESP32-Diagnostic.ino      // boucle principale, ordonnanceur, gestion HTTP
├── languages.h               // tables de traductions et helpers
├── web_interface.h           // ressources HTML/CSS/JS stockées en PROGMEM
├── wifi-config-example.h     // modèle pour les identifiants Wi-Fi
└── docs/...                  // documentation (EN/FR)
```

## Cycle d'exécution
1. **Boot** – initialisation du port série, chargement des identifiants Wi-Fi, connexion via WiFiMulti.
2. **Services** – activation mDNS, démarrage du serveur HTTP et du service BLE lorsque la carte le permet.
3. **Diagnostics** – exposition d'actions REST déclenchant les tests (balayage GPIO, scan Wi-Fi, routines OLED, benchmarks, exports).
4. **Interface web** – les ressources statiques proviennent de `web_interface.h`; les données dynamiques sont injectées via des réponses JSON.
5. **Traductions** – `languages.h` stocke les chaînes FR et EN. Le client récupère `/api/get-translations` puis met à jour les textes à la volée.

## Modules clés
- **Pile Wi-Fi** – utilise `WiFiMulti` pour tester les réseaux configurés jusqu'à réussite.
- **Gestionnaire BLE** – détecte les capacités du chip, configure le service GATT et expose l'état côté interface.
- **Moteur de diagnostics** – exécute les tests matériels (GPIO, ADC, tactile, OLED, benchmarks) et formate les résultats pour le tableau de bord et les exports.
- **Sous-système d'export** – assemble les charges TXT/JSON/CSV et la vue imprimable.
- La version 3.7.30 introduit l'écran Wi-Fi de démarrage et des helpers de scan BLE mutualisés, maintenant le même ordonnanceur que la pile soit Bluedroid ou NimBLE.

## Structure front-end
- Les modèles HTML définissent chaque onglet avec des sections sémantiques.
- Un routeur léger gère la navigation via délégation d'événements.
- Le design responsive garantit des en-têtes monolignes et une navigation compacte sur mobiles.

## Flux de données
- Le client appelle `/api/get-translations` au chargement, puis consomme `/api/overview` (injecté dans le HTML) pour l'état initial.
- Les boutons de test invoquent leurs endpoints dédiés ; les réponses mettent l'UI à jour côté JavaScript.
- Les exports sont générés à la volée sur le microcontrôleur sans stockage externe.

## Principes d'extension
- Ajoutez un diagnostic en étendant l'ordonnanceur dans `ESP32-Diagnostic.ino` et en exposant l'endpoint REST minimal.
- Conservez des clés de traduction cohérentes entre les entrées FR et EN.
- Évitez les appels bloquants dans la boucle principale pour préserver la réactivité web.
