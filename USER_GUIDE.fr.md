# ESP32 Diagnostic – Mode d'emploi & compilation (FR)

Ce guide pas-à-pas décrit la configuration nécessaire pour compiler et utiliser **ESP32-Diagnostic v2.8.10** avec l'IDE Arduino 2.x.

## 1. Pré-requis

### Matériel
- Carte de développement ESP32 (ESP32, ESP32-S2, ESP32-S3 ou ESP32-C3 testées)
- Câble USB de données
- Périphériques optionnels :
  - Bandeau WS2812/NeoPixel
  - Écran OLED I2C 0.96" (SSD1306)
  - Modules I2C/SPI supplémentaires pour les scanners

### Logiciel
- **Arduino IDE** 2.0 ou supérieur
- **Arduino-ESP32 Core** 3.3.2 (installable via *Gestionnaire de cartes* → « esp32 » par Espressif Systems)

### Bibliothèques Arduino indispensables
Installez via le *Gestionnaire de bibliothèques* :
1. **Adafruit BusIO**
2. **Adafruit GFX Library**
3. **Adafruit SSD1306**
4. **Adafruit NeoPixel**

Le core ESP32 fournit déjà `WiFi`, `WebServer`, `ESPmDNS`, `Wire`, `SPI`, `esp_partition`, etc.

## 2. Fichiers du projet & WiFi
1. Copiez le dossier `ESP32-Diagnostic/` dans votre répertoire de sketches Arduino.
2. Dupliquez `wifi-config.example.h` en `wifi-config.h` (fichier non versionné) puis décommentez/ajoutez des lignes `WIFI_CREDENTIAL` dans `WIFI_CREDENTIALS_LIST` :
   ```cpp
   #define WIFI_CREDENTIALS_LIST \
     WIFI_CREDENTIAL("MonReseau", "MotDePasse") \
     /* WIFI_CREDENTIAL("Secours", "Mot2") */
   ```
   Commentez les lignes inutiles : le gabarit gère automatiquement virgules et accolades.
3. Ajustez `config.h` (fourni) : broches (`CUSTOM_LED_PIN`, `DEFAULT_I2C_SDA/SCL`), `MDNS_HOSTNAME`, activation de l'auto-test Bluetooth (`ENABLE_BLUETOOTH_AUTOTEST`), intervalle `WIRELESS_STATUS_REFRESH_MS`, etc.

## 3. Réglages IDE
1. Ouvrez **ESP32-Diagnostic.ino** dans Arduino IDE.
2. Sélectionnez votre carte via *Outils → Type de carte → esp32* (ex. ESP32S3 Dev Module).
3. Choisissez le port série correct (*Outils → Port*).
4. (Optionnel) Ajustez PSRAM/vitesse Flash selon votre carte.
5. Pour tester le Bluetooth, activez la pile dans *Outils → Bluetooth* = **Enabled** (option visible sur les cartes compatibles BT).

## 4. Compilation & téléversement
1. Cliquez sur **Vérifier** pour compiler. Avec le core 3.3.2 et les bibliothèques ci-dessus, la compilation doit réussir.
2. Cliquez sur **Téléverser**.
3. Ouvrez le **Moniteur série** (115200 bauds) pour suivre le démarrage et récupérer l'adresse IP ou le hostname mDNS.

## 5. Premier lancement
1. L'ESP32 se connecte au WiFi (ou crée son SoftAP). L'adresse IP apparaît dans le moniteur série.
2. Dans un navigateur, ouvrez `http://<ip-de-la-carte>/` ou `http://esp32-diagnostic.local/`.
3. Le bandeau supérieur affiche les voyants WiFi/Bluetooth (vert/orange/rouge). Si le Bluetooth reste orange avec un rappel, activez-le dans les options de compilation.

## 6. Tests matériels optionnels
- **NeoPixel** : reliez DIN au GPIO choisi et définissez broche/nombre avant de lancer un motif.
- **OLED** : câbler SDA/SCL selon la configuration (par défaut 21/20 sur ESP32-S3). Utilisez « Configurer » avant le test complet.
- **GPIO/ADC** : consultez le pinout de la carte ; un ❌ peut simplement indiquer une broche réservée (message d'aide intégré).

## 7. Dépannage
| Problème | Solution recommandée |
| --- | --- |
| `esp_bt_main.h: No such file or directory` | Activez *Outils → Bluetooth*. Sinon, ignorez le test : le bandeau indiquera « Indisponible ». |
| Voyants WiFi/Bluetooth incohérents | Rechargez l'onglet « Sans fil » ou la page ; la v2.8.10 conserve la cohérence des voyants, affiche l'adresse IP et signale clairement un Bluetooth désactivé à la compilation. |
| SoftAP uniquement mais voyant WiFi rouge | Vérifiez les identifiants dans `wifi-config.h` ou utilisez le SoftAP (voyant orange = mode AP seul). |
| Textes non traduits | Assurez-vous que `languages.h` est présent à côté du `.ino`, puis re-téléversez. |

## 8. Documentation associée
- [`README.fr.md`](README.fr.md) – documentation complète en français.
- [`README.md`](README.md) – version anglaise et changelog détaillé.
- [`USER_GUIDE.md`](USER_GUIDE.md) – version anglaise de ce mode d'emploi.
- [`CONFIG_REFERENCE.md`](CONFIG_REFERENCE.md) / [`CONFIG_REFERENCE.fr.md`](CONFIG_REFERENCE.fr.md) – référence complète des paramètres.

Bon diagnostic ! ✅
