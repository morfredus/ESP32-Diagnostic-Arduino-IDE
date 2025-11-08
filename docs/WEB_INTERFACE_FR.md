# ESP32 Diagnostic Suite – Guide de l'interface web (v3.7.30)

Le tableau de bord web embarqué constitue la surface de contrôle principale pour les techniciens. Ce guide couvre la disposition,
les interactions et les points d'extension disponibles en version 3.7.30.

## Vue d'ensemble de la mise en page
```
┌──────────────────────────────────────────┐
│ Bandeau : identité carte & réseau        │
├──────────────────────────────────────────┤
│ Vignettes d'état (Wi-Fi, BLE, version…)  │
├──────────────────────────────────────────┤
│ Commandes de diagnostic & log en direct  │
├──────────────────────────────────────────┤
│ Tableau des résultats & exports          │
└──────────────────────────────────────────┘
```

### Bandeau
- Affiche le nom d'hôte de la carte, la version active du firmware, le SSID, l'adresse IP et l'URL mDNS.
- Présente des indicateurs colorés (vert/orange/rouge) selon le dernier statut de diagnostic.
- Propose des boutons d'accès rapide pour relancer les diagnostics et changer la langue.

### Vignettes d'état
- **Wi-Fi** – force du signal, canal, dernière mesure de latence, reflétés sur l'écran OLED au démarrage.
- **BLE** – actif sur les cartes équipées ; indique le nombre de publicités détectées.
- **Version** – reflète `DIAGNOSTIC_VERSION` et l'horodatage de compilation.
- **Stockage** – indique la mémoire libre et la PSRAM lorsqu'elle est détectée.

### Commandes de diagnostic
- `Run full diagnostics` – exécute le pipeline complet.
- `Run quick check` – se limite aux modules connectivité, mémoire et reporting.
- `Stop` – interrompt l'exécution en cours tout en conservant les résultats.
- Le log en direct diffuse les messages horodatés avec badges de sévérité.

### Tableau des résultats
- Affiche des badges PASS/WARN/FAIL par module avec les métriques résumées.
- Un clic sur une ligne déploie les données détaillées (aperçu JSON, comptages bruts).
- Les boutons d'export génèrent à la demande les rapports TXT, JSON, CSV ou l'impression.

## Accessibilité & localisation
- Toutes les actions disposent d'étiquettes ARIA et de raccourcis clavier (`Espace/Entrée`).
- Le basculement de langue FR/EN s'effectue sans rechargement grâce aux ressources définies dans `languages.h`.
- La version 3.7.30 conserve le rendu synchronisé tout en gérant l'écran Wi-Fi et les garde-fous NimBLE en arrière-plan.
- Un thème haut contraste s'active automatiquement lorsque le navigateur annonce un mode sombre.

## Personnalisation graphique
Pour adapter l'interface à votre charte :

1. Modifier le bloc CSS dans `web_interface.h` sous les commentaires `/* THEME */`.
2. Remplacer l'espace réservé du logo dans le template HTML (chercher `<!-- LOGO -->`).
3. Ajuster les couleurs tout en garantissant un contraste suffisant (recommandation WCAG AA+).

## Extensions
- Utiliser le placeholder `<div id="custom-widgets">` pour injecter des panneaux supplémentaires à l'exécution.
- S'abonner à l'événement JavaScript `diagnosticUpdate` pour suivre les résultats module par module.
- Ajouter des exports personnalisés en étendant `window.DiagnosticExports` avec de nouvelles fonctions.

## Utilisation hors ligne
- Les assets sont embarqués et mis en cache ; une fois chargée, l'interface reste opérationnelle même en cas de coupure réseau.
- En mode point d'accès, l'interface est disponible via `http://192.168.4.1/` avec les mêmes fonctionnalités.

## Ressources associées
- [Guide d'utilisation](USAGE_FR.md) – workflows opérateur et automatisation des exports.
- [Référence REST API](API_REFERENCE_FR.md) – déclencheurs programmatiques équivalents aux actions UI.
- [Directives de sécurité](SECURITY_FR.md) – recommandations pour protéger l'interface dans un environnement partagé.
