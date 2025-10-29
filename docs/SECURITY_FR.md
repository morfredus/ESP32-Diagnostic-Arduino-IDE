# ESP32 Diagnostic Suite – Directives de sécurité (v3.3.11)

Même si la suite est majoritairement utilisée en laboratoire, ces recommandations limitent l'exposition lors des tests de
matériel pré-production.

## Renforcement réseau
- Préférer un réseau Wi-Fi QA dédié avec chiffrement WPA2-PSK ou WPA2-Enterprise.
- Modifier le SSID/mot de passe hotspot par défaut dans `wifi-config.h` avant les essais terrain.
- Désactiver complètement le fallback hotspot via `ENABLE_AP_FALLBACK = 0` lorsque non requis.
- Restreindre l'accès à la REST API via pare-feu ou authentification proxy sur les réseaux partagés.

## Gestion des identifiants
- Stocker les identifiants Wi-Fi dans `wifi-config.h` et exclure le fichier du contrôle de source (`.gitignore`).
- Faire tourner les mots de passe régulièrement et mettre à jour toutes les unités flashées avant de révoquer les anciens accès.
- En WPA2/EAP entreprise, utiliser des certificats spécifiques à chaque appareil et les révoquer si l'unité est perdue.

## Intégrité firmware
- Vérifier le hash SHA-256 des binaires avant les flashs de masse.
- Maintenir l'environnement de build à jour et reproductible (cf. [BUILD_AND_DEPLOY_FR.md](BUILD_AND_DEPLOY_FR.md)).
- Taguer les releases avec des tags Git annotés (ex. `v3.3.11`) pour assurer la traçabilité code ↔ firmware.

## Bonnes pratiques interface web
- Ne pas exposer directement le tableau de bord sur Internet.
- Utiliser des navigateurs à jour et un proxy HTTPS avec bibliothèques TLS récentes.
- Activer la déconnexion automatique côté proxy pour les laboratoires partagés.
- Consulter [WEB_INTERFACE_FR.md](WEB_INTERFACE_FR.md) pour les considérations ARIA utiles en mode kiosque.

## Conservation des données
- Les exports JSON/CSV/TXT contiennent des identifiants carte et notes opérateur ; traiter ces fichiers comme sensibles.
- Stocker les exports sur un support sécurisé avec sauvegardes conformément aux exigences de conformité internes.
- Purger le cache de l'appareil après extraction via `POST /api/stop` puis un cycle d'alimentation.

## Réponse à incident
- En cas de suspicion, effectuer un factory reset : `esptool.py erase_flash`, puis reflasher 3.3.11.
- Révoquer les identifiants Wi-Fi associés et mettre à jour les journaux d'audit avec les exports concernés.
- Ouvrir un ticket d'incident en mentionnant l'identifiant de rapport (`run_id`) pour la traçabilité.

## Ressources associées
- [Guide de configuration](CONFIG_FR.md)
- [Référence REST API](API_REFERENCE_FR.md)
- [Compilation & déploiement](BUILD_AND_DEPLOY_FR.md)
