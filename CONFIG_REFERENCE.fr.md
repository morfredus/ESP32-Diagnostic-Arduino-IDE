# Référence de configuration ESP32-Diagnostic (v2.8.9)

Ce document récapitule l'ensemble des paramètres modifiables. Consultez-le avant toute compilation personnalisée.

## 1. `config.h` – Paramètres matériels (versionné)

`config.h` est livré avec le dépôt et doit être ajusté selon votre banc de test.

| Symbole | Valeur par défaut | Description |
| --- | --- | --- |
| `MDNS_HOSTNAME` | `"esp32-diagnostic"` | Nom d'hôte annoncé sur le réseau local (`http://<hostname>.local`). |
| `CUSTOM_LED_PIN` | `-1` | Force la broche de la LED intégrée. Laisser `-1` pour l'auto-détection. |
| `CUSTOM_LED_COUNT` | `1` | Nombre de LED par défaut pour la bande NeoPixel. |
| `ENABLE_I2C_SCAN` | `true` | Active le scan I2C automatique au démarrage. |
| `DEFAULT_I2C_SDA` | `21` | Broche SDA appliquée au démarrage et lors d'un reset du test OLED. |
| `DEFAULT_I2C_SCL` | `20` | Broche SCL appliquée au démarrage et lors d'un reset du test OLED. |
| `SCREEN_WIDTH` | `128` | Largeur logique de l'écran OLED (pixels). |
| `SCREEN_HEIGHT` | `64` | Hauteur logique de l'écran OLED (pixels). |
| `OLED_RESET` | `-1` | Broche de reset de l'OLED (`-1` si non câblée). |
| `SCREEN_ADDRESS` | `0x3C` | Adresse I2C par défaut de l'écran OLED. |
| `ENABLE_BLUETOOTH_AUTOTEST` | `true` | Affiche le bouton d'auto-test Bluetooth lorsque la pile est compilée. |
| `WIRELESS_STATUS_REFRESH_MS` | `3000` | Intervalle (ms) entre deux rafraîchissements du bandeau sans fil. |

> Astuce : `config.h` est versionné. Vos ajustements restent donc en place après un `git pull`.

## 2. `wifi-config.h` – Identifiants WiFi (ignoré par Git)

1. Copier `wifi-config.example.h` en `wifi-config.h` (même dossier que le sketch).
2. Renseigner le tableau `WIFI_NETWORKS`. Chaque entrée comporte un SSID et un mot de passe (les SSID vides sont ignorés).

```cpp
static const WiFiCredential WIFI_NETWORKS[] = {
  {"MonReseau", "MotDePasse"},
  {"ReseauSecours", "Mot2"}
};
```

> `wifi-config.h` figure dans `.gitignore`. Ne le publiez jamais.

## 3. Contrôle final avant compilation

- [ ] `wifi-config.h` existe avec au moins un couple SSID/mot de passe valide.
- [ ] `config.h` correspond à votre câblage (SDA/SCL, LED, nombre de NeoPixel).
- [ ] Optionnel : passer `ENABLE_BLUETOOTH_AUTOTEST` à `false` si la pile Bluetooth n'est pas incluse.
- [ ] Ajuster `WIRELESS_STATUS_REFRESH_MS` si vous souhaitez un rafraîchissement plus rapide/lent du bandeau.

Pour une procédure détaillée, consultez [`USER_GUIDE.md`](USER_GUIDE.md) ou [`GUIDE_UTILISATION.fr.md`](GUIDE_UTILISATION.fr.md).
