# Guide d'utilisation (FR)

## 1. Premier démarrage
1. Ouvrez le moniteur série à 115200 bauds juste après le flash.
2. Patientez pendant la connexion Wi-Fi. En cas de succès, l'IP attribuée et l'URL `http://ESP32-Diagnostic.local` sont affichées.
3. En cas d'échec, la console fournit des indices (mot de passe erroné, réseau introuvable, etc.). Mettez `wifi-config.h` à jour.

## 2. Accéder au tableau de bord
- Rendez-vous sur l'adresse IP affichée ou sur `http://ESP32-Diagnostic.local` (mDNS) depuis un appareil du même réseau.
- L'interface s'ouvre en français. Utilisez le commutateur FR/EN en haut à droite pour changer de langue sans rechargement.

## 3. Aperçu de l'interface web
Le tableau de bord est organisé en onglets :
- **Vue d'ensemble** – résumé matériel, mémoire, version firmware, statut Bluetooth®.
- **LEDs** – contrôle de la LED embarquée et motifs NeoPixel/WS2812B.
- **Écrans** – diagnostics OLED avec animations pas-à-pas et déclenchement manuel.
- **Tests avancés** – benchmarks CPU/mémoire, stress tests et partitions flash.
- **GPIO** – test interactif des broches avec état en direct.
- **Wi-Fi** – résultats détaillés (RSSI, canal, sécurité, PHY, largeur de bande).
- **Performance** – historique des benchmarks et métriques temps réel.
- **Export** – téléchargement des rapports TXT/JSON/CSV ou ouverture de la vue imprimable.

La version 3.1.20 documente l'inspecteur `/api/memory-details`, intègre la checklist de débogage Bluetooth/Wi-Fi et maintient le bandeau sticky aligné sur le changelog bilingue.

La barre de navigation s'adapte aux mobiles et conserve l'onglet actif.

## 4. Outils Bluetooth® Low Energy
- La diffusion BLE démarre automatiquement lorsque la carte est compatible.
- Une cartouche dédiée dans l'onglet Vue d'ensemble affiche le nom, l'état de connexion et autorise le renommage.
- Les noms invalides sont refusés avec un HTTP 400 pour préserver la cohérence du service GATT.

## 5. API REST
Toutes les routes renvoient du JSON sauf mention contraire :
- `GET /api/test-gpio` – lance le balayage GPIO.
- `GET /api/wifi-scan` – effectue un scan Wi-Fi.
- `GET /api/benchmark` – exécute les benchmarks CPU et mémoire.
- `GET /api/set-language?lang=fr|en` – change la langue de l'interface.
- `GET /api/get-translations` – récupère le catalogue de traductions courant.
- `GET /api/oled-config?sda=<pin>&scl=<pin>` – reconfigure les broches I2C de l'OLED.
- `GET /api/oled-test` – lance la séquence complète de tests OLED (~25 secondes).
- `GET /api/oled-message?message=TEXTE` – affiche un message personnalisé sur l'OLED.
- `GET /api/memory-details` – renvoie les métriques flash/PSRAM/SRAM ainsi que le pourcentage de fragmentation pour l'analyse post-diagnostic.
- `GET /export/txt`, `/export/json`, `/export/csv` – télécharge les rapports dans différents formats.

## 6. Rapports et journalisation
- La sortie série reflète les actions clés (Wi-Fi, BLE, résultats de tests).
- Les exports incluent les informations carte, la mémoire, les benchmarks, le scan Wi-Fi, les GPIO et l'état OLED.
- Utilisez le JSON pour l'analyse automatisée et TXT/CSV pour la consultation manuelle.
- Exploitez l'endpoint `/api/memory-details` lorsque les exports signalent une fragmentation ; la réponse JSON est désormais documentée en 3.1.20.

## 7. Bonnes pratiques
- Relancez un scan Wi-Fi après tout déplacement pour rafraîchir les RSSI.
- Arrêtez les animations NeoPixel avant de couper l'alimentation des LED externes.
- Après changement des broches OLED, exécutez `/api/oled-config` puis `/api/oled-test` pour valider le câblage.

## 8. Checklist de débogage post-release (3.1.20)
- Déclenchez `/api/memory-details` pour vérifier que la fragmentation et la PSRAM correspondent aux indications de la documentation.
- Confirmez que la carte Bluetooth® reflète les bascules d'advertising après `/api/bluetooth/toggle` et un renommage via `/api/bluetooth/name`.
- Passez en revue l'onglet Wi-Fi et la réponse `/api/wifi-scan` pour valider les métadonnées de canal, bande, largeur et PHY collectées pendant la campagne de débogage.
