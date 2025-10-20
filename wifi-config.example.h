#pragma once

// ============================================================================
// WiFi credentials template for ESP32-Diagnostic
// ----------------------------------------------------------------------------
// Copy this file to `wifi-config.h` and edit the list below with your own
// networks. The file `wifi-config.h` is ignored by Git to protect secrets.
// ============================================================================

struct WiFiCredential {
  const char* ssid;
  const char* password;
};

static const WiFiCredential WIFI_NETWORKS[] = {
  {"YourNetwork", "YourPassword"}
  // {"BackupNetwork", "BackupPassword"},
};

static const size_t WIFI_NETWORK_COUNT = sizeof(WIFI_NETWORKS) / sizeof(WIFI_NETWORKS[0]);

