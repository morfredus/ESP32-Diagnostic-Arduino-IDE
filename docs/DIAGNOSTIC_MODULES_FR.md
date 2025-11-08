# ESP32 Diagnostic Suite – Modules de diagnostic (v3.7.30)

Ce guide détaille chaque module de diagnostic automatisé livré avec la version 3.7.30. Il décrit l'ordre d'exécution, les métriques
collectées et les clés d'exportation à surveiller lors de l'analyse des rapports.

## Pipeline d'exécution
Le planificateur traite les modules dans l'ordre suivant. Chaque module publie des événements de statut consommés par le tableau de
bord web, la REST API et le logger série.

1. **Pré-vérifications** – contrôle la version du firmware, les flags de compilation et la mémoire libre.
2. **Connectivité** – lance des scans Wi-Fi, un scan BLE optionnel et vérifie la diffusion mDNS.
3. **Débit réseau** – exécute des benchmarks ping et HTTP lorsque les identifiants Wi-Fi sont renseignés.
4. **Mémoire & stockage** – mesure la PSRAM, la fragmentation du tas et l'intégrité lecture/écriture de la flash.
5. **Balayage GPIO** – alterne les sorties numériques, lit les entrées analogiques et sorties PWM (routine touchpad toujours retirée).
6. **Bus périphériques** – valide I2C (primaire/secondaire), boucle SPI, boucle UART et capteurs OneWire.
7. **Afficheur & LEDs** – lance les séquences OLED, animations NeoPixel et vérifie les timings de rafraîchissement.
8. **Reporting** – assemble les résultats, met à jour le journal d'activité et publie les données pour export/API.

## Référence des modules
| Module | Description | Métriques clés | Clés d'export |
|--------|-------------|----------------|--------------|
| Pré-vérifications | Confirme les métadonnées build, uptime, raison du reset, fréquence CPU. | Version firmware, raison reset, heap libre, PSRAM détectée. | `env.version`, `env.reset_reason`, `env.heap_free`. |
| Connectivité | Recense les points d'accès, alimente l'écran OLED de démarrage, vérifie l'association Wi-Fi et teste la publicité BLE (si supportée). | Nombre d'AP, RSSI par AP, temps d'association, fenêtre de scan BLE. | `wifi.networks[]`, `wifi.association_ms`, `ble.found_devices`. |
| Débit réseau | Mesure latence et taille de réponse HTTP selon les cibles configurées. | Ping min/moy/max, statut HTTP, débit (kB/s). | `net.ping`, `net.http.status`, `net.http.kbps`. |
| Mémoire & stockage | Effectue stress tests du tas et validation CRC de la flash. | Marque haute du tas, taille PSRAM, statut CRC flash. | `memory.heap_max`, `memory.psram`, `storage.flash_crc`. |
| Balayage GPIO | Pulse les sorties digitales, échantillonne les entrées analogiques, valide les pull-ups (routine tactile retirée). | Liste de broches ok/erreur, moyenne/variance ADC, synthèse PWM via les tests matériels. | `gpio.list`, `hardware_tests.pwm`. |
| Bus périphériques | Confirme l'initialisation des bus et la détection optionnelle (OLED, capteurs, INA219...). | IDs I2C détectés, statut boucle SPI, checksum UART, présence OneWire. | `bus.i2c.devices[]`, `bus.spi.status`, `bus.uart.loopback`. |
| Afficheur & LEDs | Exerce la sortie graphique et les routines LED. | Checksum trame OLED, FPS, saturation & gamma NeoPixel. | `display.oled.frames`, `display.oled.fps`, `led.neopixel.status`. |
| Reporting | Sérialise les résultats en JSON, CSV, TXT et gère le cache REST. | Taille par format, horodatage, notes opérateur. | `report.generated_at`, `report.formats`, `report.operator`. |

## Niveaux de statut
- **PASS** – module exécuté sans avertissement, métriques dans les seuils attendus.
- **WARN** – module exécuté avec incohérences (ex. latence élevée). Vérifier avant validation matérielle.
- **FAIL** – module interrompu ou erreur critique. Mettre le matériel en quarantaine pour investigation.

## Modules personnalisés
Des diagnostics personnalisés peuvent être ajoutés via `registerCustomDiagnostics()` dans `ESP32-Diagnostic.ino`. Chaque callback reçoit :

- Les helpers `logInfo`, `logWarn`, `logError` pour les journaux série et web.
- L'accès au générateur de rapport JSON afin d'injecter des payloads structurés.
- Les horodatages et métadonnées carte pour la traçabilité.

Veiller à respecter le contrat PASS/WARN/FAIL et à publier des clés d'export uniques pour éviter les collisions.

## Conseils de dépannage
- Si un module retourne **FAIL** sans logs série, activer le logging verbeux via `#define DIAGNOSTIC_DEBUG 1` dans `config.h`.
- Les modules dépendant de matériel optionnel (OLED, NeoPixel, INA219) ne s'activent que si le périphérique est détecté lors des pré-vérifications.
- Utiliser l'endpoint REST `/api/diagnostics/{module}` pour récupérer un résultat isolé.
- Pour reproduire un problème, exporter le rapport JSON et l'attacher au template d'issue GitHub.

## Ressources associées
- [Matrice des fonctionnalités](FEATURE_MATRIX_FR.md) – compatibilité matérielle et périphériques optionnels.
- [Référence REST API](API_REFERENCE_FR.md) – schémas de payload pour automatiser l'exécution.
- [Dépannage](TROUBLESHOOTING_FR.md) – étapes de récupération lorsqu'un module reste indisponible.
