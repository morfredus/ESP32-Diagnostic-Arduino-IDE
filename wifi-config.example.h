#pragma once

// ============================================================================
// WiFi credentials template for ESP32-Diagnostic (v2.8.8)
// ----------------------------------------------------------------------------
// 1. Copy this file to `wifi-config.h`.
// 2. Uncomment one or more `WIFI_CREDENTIAL` lines below and fill in your SSID
//    and password (leave the password empty for open networks).
// 3. Keep additional lines commented until you need them—no braces or commas to
//    manage manually, which avoids compilation errors.
// ============================================================================

struct WiFiCredential {
  const char* ssid;
  const char* password;
};

#define WIFI_CREDENTIALS_LIST \
  /* WIFI_CREDENTIAL("MyNetwork", "MyPassword") */ \
  /* WIFI_CREDENTIAL("BackupNetwork", "BackupPassword") */

static const WiFiCredential WIFI_NETWORKS[] = {
#define WIFI_CREDENTIAL(ssid, password) {ssid, password},
  WIFI_CREDENTIALS_LIST
#undef WIFI_CREDENTIAL
};

static const size_t WIFI_NETWORK_COUNT = sizeof(WIFI_NETWORKS) / sizeof(WIFI_NETWORKS[0]);

