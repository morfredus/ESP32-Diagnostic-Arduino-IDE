# ESP32 Diagnostic Suite – Accueil (v3.2.0)

Bienvenue sur le wiki officiel de la version 3.2.0 du firmware **ESP32 Diagnostic Suite**. Vous y trouverez un aperçu rapide des
capacités de la plateforme, des nouveautés de cette itération et des liens directs vers les sections d'approfondissement.

## 🚀 Objectif du projet
- Fournir un tableau de bord web intégré pour inspecter et diagnostiquer les microcontrôleurs ESP32 (ESP32, ESP32-S2, ESP32-S3,
  ESP32-C3, ESP32-C6, ESP32-H2).
- Simplifier les campagnes de validation matérielle grâce à des tests automatisés couvrant les bus de communication, la mémoire,
  le réseau et les périphériques optionnels.
- Offrir une expérience multilingue (FR/EN) avec une documentation synchronisée entre firmware, wiki et guides PDF.

## 🆕 Points forts de la version 3.2.0
- Documentation enrichie de l'inspecteur mémoire (`/api/memory-details`) et suivi des alertes de fragmentation observées en QA.
- Checklist Wi-Fi/Bluetooth® intégrée aux guides d'utilisation et de dépannage, directement issue des retours terrain.
- Harmonisation complète du bandeau d'interface, du changelog et des exports pour garantir la cohérence firmware / documentation.

## 🌐 Interface web & API
- Bandeau principal indiquant l'état du Wi-Fi, l'adresse IP, le nom mDNS (`http://ESP32-Diagnostic.local`) et la version active.
- Lancement manuel des diagnostics via boutons dédiés et surveillance en temps réel depuis le navigateur.
- Exports disponibles en TXT, JSON, CSV et vue imprimable pour consigner les rapports de test.
- Endpoints REST documentés permettant l'automatisation (ex. `/api/run-diagnostics`, `/api/memory-details`).

## 🔍 Modules de diagnostic inclus
- **Connectivité :** scan Wi-Fi, état BLE (si pris en charge), vérification des identifiants multiples.
- **Mémoire & stockage :** PSRAM, flash SPI, allocations dynamiques, fragmentation et usage heap.
- **E/S & capteurs :** GPIO, ADC, pads tactiles, PWM, bus I2C & SPI avec profils configurables.
- **Afficheurs & LED :** séquences OLED SSD1306 0,96" et animations NeoPixel/WS2812B.

## 🧰 Pré-requis et configuration
- Arduino IDE 2.x (ou Arduino CLI) avec l'ESP32 Arduino Core **3.3.2**.
- Bibliothèques recommandées : Adafruit GFX, Adafruit SSD1306, Adafruit NeoPixel, Adafruit BusIO.
- Fichier `wifi-config.h` (ou `wifi-config-example.h`) à personnaliser pour stocker vos identifiants réseau.

## 📚 Ressources complémentaires
- [Guide d'installation](INSTALL.md)
- [Configuration avancée](CONFIG.md)
- [Guide d'utilisation](USAGE.md)
- [Dépannage](TROUBLESHOOTING.md)
- [Architecture interne](ARCHITECTURE.md)
- [Contribuer au projet](../CONTRIBUTING.md)
- [Journal des modifications complet](../CHANGELOG.md)

## 🤝 Support & contributions
- Licence : [MIT](../LICENSE)
- Suivi des anomalies : [Issues GitHub](https://github.com/ESP32-Diagnostic/ESP32-Diagnostic/issues)
- Propositions de correctifs : fork du dépôt, création de branche puis Pull Request en suivant le guide de contribution.

Bonne exploration et diagnostics productifs !
