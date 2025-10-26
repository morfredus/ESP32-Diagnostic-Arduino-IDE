# Dépannage (FR)

## La diffusion BLE est invisible
- Vérifiez la compatibilité BLE de votre carte (ESP32, ESP32-S3, ESP32-C3, ESP32-C6, ESP32-H2).
- Consultez le moniteur série pour détecter une erreur d'initialisation BLE.
- Assurez-vous qu'aucune autre application n'occupe la pile BLE (redémarrez la carte).

## Le changement de langue n'a aucun effet
- Assurez-vous d'exécuter le firmware 3.1.20 afin que `/api/set-language` conserve les succès explicites (FR/EN) et un HTTP 400 pour les codes non pris en charge tout en respectant le changelog bilingue.
- Vérifiez que `languages.h` est présent à côté du sketch et compilé correctement.
- Videz le cache du navigateur (Ctrl+F5) pour recharger les traductions.
- Contrôlez que `/api/get-translations` renvoie HTTP 200 via la console ou un client REST.

## Les diagnostics additionnels affichent « Non teste » sans accent
- Passez en version 3.1.20 (ou toute maintenance 3.x ultérieure) pour conserver l'intitulé « Non testé » et les traductions du bandeau sticky alignées sur la documentation bilingue.
- Rafraîchissez le tableau de bord (Ctrl+F5) afin de recharger les traductions mises à jour.

## L'écran OLED reste noir
- Relancez `/api/oled-config?sda=<pin>&scl=<pin>` avec le câblage correct.
- Contrôlez l'adresse I2C (0x3C par défaut). Remplacez l'écran s'il ne répond pas sur le bus.
- Assurez une alimentation stable (3,3 V, masse commune avec l'ESP32).

## La connexion Wi-Fi échoue
- Revérifiez les couples SSID/mot de passe dans `wifi-config.h`.
- Surveillez le moniteur série pour les codes `WL_NO_SSID_AVAIL` ou `WL_CONNECT_FAILED`.
- Rapprochez la carte du point d'accès ou testez un SSID 2,4 GHz.

## Alertes de fragmentation mémoire dans les exports
- Appelez `/api/memory-details` et analysez le pourcentage de `fragmentation` ainsi que les drapeaux PSRAM décrits pour la 3.1.20.
- Activez la PSRAM sur les cartes ESP32-S3 afin de garder le drapeau `psramAvailable` cohérent avec la documentation.
- Si la fragmentation reste au-dessus de 20 %, redémarrez la carte puis relancez les diagnostics pour purger l'usage temporaire de la heap.

## Les tests GPIO détectent des conflits
- Libérez la broche de tout périphérique (I2C, SPI, UART) avant le balayage.
- Certaines broches sont entrée uniquement ou réservées (ex. GPIO34-39). Consultez le schéma de votre carte.

## Les exports sont vides
- Attendez la fin des diagnostics avant de déclencher un export.
- Autorisez les téléchargements depuis l'adresse IP de l'appareil dans votre navigateur.
- Vérifiez les logs série pour détecter une alerte mémoire pouvant interrompre le streaming.
