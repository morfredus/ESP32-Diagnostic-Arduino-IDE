/*
 * ============================================================
 * ESP32 Diagnostic System - WiFi Configuration
 * Version: 3.2.0
 * ============================================================
 */

#ifndef WIFI_CONFIG_H
#define WIFI_CONFIG_H

// ============================================================
// CONFIGURATION WIFI MULTI-SSID
// ============================================================

struct WiFiCredentials {
  const char* ssid;
  const char* password;
};

// Liste des réseaux WiFi (ordre de priorité)
const WiFiCredentials wifiNetworks[] = {
  // Réseau principal
  {"fredmomo", "@0squn1frgrvhx4td"},
  
  // Réseau secondaire
  {"leweekendbordeaux", "0880F649410484F946633F826F"},
};

// Nombre de réseaux configurés
const int WIFI_NETWORKS_COUNT = sizeof(wifiNetworks) / sizeof(WiFiCredentials);

// ============================================================
// CONFIGURATION POINT D'ACCÈS (Mode Secours)
// ============================================================

#define AP_SSID "ESP32-Diagnostic-Setup"
#define AP_PASSWORD "diagnostic123"
#define AP_CHANNEL 1
#define AP_MAX_CONNECTIONS 4
#define AP_HIDDEN false

// ============================================================
// PARAMÈTRES DE CONNEXION
// ============================================================

#define WIFI_CONNECT_TIMEOUT 10
#define WIFI_MAX_RETRIES 2
#define WIFI_RETRY_DELAY 500
#define WIFI_POWER_SAVE false

// ============================================================
// CONFIGURATION MDNS
// ============================================================

#define MDNS_HOSTNAME "esp32-diagnostic"

#endif // WIFI_CONFIG_H