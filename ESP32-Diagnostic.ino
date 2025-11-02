/*
 * ESP32 Diagnostic Suite v3.6.04-dev
 * Compatible: ESP32, ESP32-S2, ESP32-S3, ESP32-C3, ESP32-C6, ESP32-H2
 * Optimisé pour ESP32 Arduino Core 3.3.2
 * Carte testée: ESP32-S3 avec PSRAM OPI
 * Auteur: morfredus
 */

#if defined(__GNUC__)
  #define DIAGNOSTIC_UNUSED __attribute__((unused))
#else
  #define DIAGNOSTIC_UNUSED
#endif

static const char* const DIAGNOSTIC_VERSION_HISTORY[] DIAGNOSTIC_UNUSED = {
  "3.6.04-dev - Remplacement des icones OLED non ASCII pour compilation",
  "3.6.03-dev - Inclusion interface web pour exposer handleJavaScript()",
  "3.6.02-dev - Correction route /js/app.js manquante pour chargement JavaScript UI",
  "3.6.01-dev - Reorganisation UI ergonomique par themes (Affichage, Capteurs, Tests materiel)",
  "3.6.00-dev - Ajout gestion capteurs (LED RGB, Buzzer, DHT11, Luminosite, Distance, Mouvement)",
  "3.5.3-dev - Correction traductions hardcodées (Message affiche, Inconnu, WiFi Open)",
  "3.5.2-dev - Ajout endpoints API manquants et correction traductions Overview",
  "3.5.1 - Restauration historique des versions",
  "3.5.0 - Publication de la version 3.5.0 pour mise en production",
  "3.4.11 - Publication documentation et harmonisation finale des traductions dynamiques",
  "3.4.11-dev - Harmonisation des traductions dynamiques",
  "3.4.10-dev - Localisation instantanée des tests avancés",
  "3.4.09-dev - Actualisation instantanée des traductions",
  "3.4.08-dev - Localisation des tests avancés",
  "3.4.07-dev - Harmonisation des traductions OLED",
  "3.4.06-dev - Localisation des statuts et actions OLED",
  "3.4.05-dev - Centralisation des traductions OLED dans languages.h",
  "3.4.04-dev - Correction des traductions des commandes écran",
  "3.4.03-dev - Traduction dynamique des actions écran OLED",
  "3.4.02-dev - Repli HTTP automatique si HTTPS indisponible",
  "3.4.01-dev - Préférence HTTPS et repli intelligent",
  "3.4.0 - Stabilisation JSON et documentation 3.4.x",
  "3.4.0-dev - Nettoyage des commentaires et harmonisation Bluetooth",
  "3.3.10-dev - Prototypes des helpers JSON mutualisés",
  "3.3.09-dev - Unification des handlers périphériques",
  "3.3.08-dev - Correctifs compilation helpers JSON",
  "3.3.07-dev - Mutualisation des réponses HTTP JSON",
  "3.3.06-dev - Corrections des retours String après optimisation de la traduction",
  "3.3.05-dev - Levée de l'ambiguïté String/const char* des traductions",
  "3.3.04-dev - Conversion String pour la table de traduction",
  "3.3.03-dev - Optimisation de la table de traduction",
  "3.3.02-dev - Harmonisation bilingue des libellés UI",
  "3.3.01-dev - Retrait du diagnostic touchpad devenu obsolète",
  "3.2.21-maint - Nettoyage des bannières et consolidation de l'historique",
  "3.2.20-dev - Correction de l'attribut data-i18n dans appendInfoItem",
  "3.2.19-dev - Correction de la collision tr() lors du changement de langue",
  "3.2.18-dev - Suppression du double échappement JS dynamique",
  "3.2.17-dev - Correction des chaînes JS des tests dynamiques",
  "3.2.16-dev - Localisation étendue des chaînes UI dynamiques",
  "3.2.15-dev - Réduction du gabarit HTML et allègement du sketch",
  "3.2.14-dev - Harmonisation des traductions UI dynamiques",
  "3.2.13-dev - Retrait final du doublon IP sur le bandeau legacy",
  "3.2.12-dev - Bandeau sans doublon IP",
  "3.2.11-dev - Bandeau d'accès compact et lien IP unique",
  "3.2.10-dev - Pré-configuration du nom d'hôte mDNS avant le WiFi",
  "3.2.09-dev - Restauration complète de mDNS sur ESP32-S3",
  "3.2.08-dev - Correction de l'initialisation WiFi pour mDNS",
  "3.2.07-dev - Compatibilité mDNS multi-environnements",
  "3.2.06-dev - Restauration mDNS fiable et lien d'accès constant",
  "3.2.05-dev - Suppression du service mDNS instable",
  "3.2.04-dev - Reconstruction du serveur mDNS",
  "3.2.02-dev - Ajustement du maintien mDNS sans appel update()",
  "3.2.01-dev - Correction de la publication mDNS",
  "3.2.0-doc - Consolidation des guides après la campagne de tests",
  "3.1.19-doc - Scission du changelog FR/EN et rafraîchissement de la documentation",
  "3.1.18-doc - Documentation 3.1.18 et durcissement du changement de langue",
  "3.1.17-maint - Nettoyage de commentaires superflus et renforcement de la sélection de langue",
  "3.1.16 - Bandeau sticky unifié et correctifs navigation/traductions",
  "3.1.15-maint - Harmonisation de la documentation et rappel des libellés par défaut",
  "3.1.14-maint - Corrections de messages d'état et amélioration de l'échappement HTML",
  "3.1.13-dev - Bandeau sticky unifié et retrait du test tactile des exports",
  "3.1.12-dev - Alignement du numéro de version dans le bandeau principal"
};

#undef DIAGNOSTIC_UNUSED

#include <WiFi.h>
#include <WiFiMulti.h>
#include <WebServer.h>

#if defined(ARDUINO_ARCH_ESP32)
  #if defined(__has_include)
    #if __has_include(<ESPmDNS.h>)
      #include <ESPmDNS.h>
      #define DIAGNOSTIC_HAS_MDNS 1
      #define DIAGNOSTIC_USES_ESPMDNS 1
    #elif __has_include(<mdns.h>)
      #include <mdns.h>
      #define DIAGNOSTIC_HAS_MDNS 1
      #define DIAGNOSTIC_USES_IDF_MDNS 1
    #else
      #define DIAGNOSTIC_HAS_MDNS 0
    #endif
  #else
    #include <ESPmDNS.h>
    #define DIAGNOSTIC_HAS_MDNS 1
    #define DIAGNOSTIC_USES_ESPMDNS 1
  #endif
#else
  #define DIAGNOSTIC_HAS_MDNS 0
#endif

#if !defined(DIAGNOSTIC_HAS_MDNS)
  #define DIAGNOSTIC_HAS_MDNS 0
#endif

#if DIAGNOSTIC_HAS_MDNS && defined(DIAGNOSTIC_USES_ESPMDNS)
  #if defined(NO_GLOBAL_MDNS) || defined(NO_GLOBAL_INSTANCES)
    static MDNSResponder diagnosticMDNSResponder;
    #define DIAGNOSTIC_MDNS_SERVER diagnosticMDNSResponder
  #else
    #define DIAGNOSTIC_MDNS_SERVER MDNS
  #endif
#endif
#include <esp_chip_info.h>
#include <esp_err.h>
#include <esp_mac.h>
#include <esp_flash.h>
#include <esp_heap_caps.h>
#include <esp_partition.h>
#include <esp_wifi.h>
#if defined(__has_include)
  #if __has_include(<esp_netif.h>)
    #include <esp_netif.h>
    #define DIAGNOSTIC_HAS_ESP_NETIF 1
  #endif
  #if __has_include(<tcpip_adapter.h>)
    #include <tcpip_adapter.h>
    #define DIAGNOSTIC_HAS_TCPIP_ADAPTER 1
  #endif
#else
  #include <esp_netif.h>
  #define DIAGNOSTIC_HAS_ESP_NETIF 1
#endif
#include <soc/soc.h>
#include <soc/rtc.h>
#if defined(__has_include)
  #if __has_include(<soc/soc_caps.h>)
    #include <soc/soc_caps.h>
  #endif
#else
  #include <soc/soc_caps.h>
#endif
#include <Wire.h>
#include <Adafruit_NeoPixel.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

#define BLE_STACK_SUPPORTED 1

#if defined(SOC_BLE_SUPPORTED)
  static const bool TARGET_BLE_SUPPORTED = (SOC_BLE_SUPPORTED);
#elif defined(CONFIG_IDF_TARGET_ESP32S3) || defined(CONFIG_IDF_TARGET_ESP32C3) || \
      defined(CONFIG_IDF_TARGET_ESP32C6) || defined(CONFIG_IDF_TARGET_ESP32H2)
  static const bool TARGET_BLE_SUPPORTED = true;
#elif defined(CONFIG_IDF_TARGET_ESP32S2)
  static const bool TARGET_BLE_SUPPORTED = false;
#elif defined(ARDUINO_ARCH_ESP32)
  #if defined(ARDUINO_ESP32S2_DEV) || defined(ARDUINO_LOLIN_S2_MINI) || defined(ARDUINO_LILYGO_T_DISPLAY_S2) || \
      defined(ARDUINO_TINYPICO_S2) || defined(ARDUINO_TINYS2) || defined(ARDUINO_QTPY_ESP32S2)
    static const bool TARGET_BLE_SUPPORTED = false;
  #else
    static const bool TARGET_BLE_SUPPORTED = true;
  #endif
#else
    static const bool TARGET_BLE_SUPPORTED = true;
#endif
#include <vector>
#include <cstring>
#include <initializer_list>
#include "json_helpers.h"

#if defined(__has_include)
  #if __has_include("wifi-config.h")
    #include "wifi-config.h"
  #elif __has_include("wifi-config-example.h")
    #include "wifi-config-example.h"
  #else
    #error "Aucun fichier de configuration WiFi disponible"
  #endif
#else
  #include "wifi-config.h"
#endif

// Système de traduction
#include "languages.h"

// --- [BUGFIX] Exposition des handlers d'interface web ---
struct DiagnosticInfo;
#include "web_interface.h"

Language currentLanguage = LANG_FR;

static String buildActionResponseJson(bool success,
                                      const String& message,
                                      std::initializer_list<JsonFieldSpec> extraFields = {});
inline void sendActionResponse(int statusCode,
                               bool success,
                               const String& message,
                               std::initializer_list<JsonFieldSpec> extraFields = {});
inline void sendOperationSuccess(const String& message,
                                 std::initializer_list<JsonFieldSpec> extraFields = {});
inline void sendOperationError(int statusCode,
                               const String& message,
                               std::initializer_list<JsonFieldSpec> extraFields = {});

#if defined(__has_include)
  #if __has_include(<esp_arduino_version.h>)
    #include <esp_arduino_version.h>
  #endif
#endif

#ifndef ESP_ARDUINO_VERSION_VAL
#define ESP_ARDUINO_VERSION_VAL(major, minor, patch) ((major << 16) | (minor << 8) | (patch))
#endif

#ifndef ESP_ARDUINO_VERSION
#if defined(ESP_ARDUINO_VERSION_MAJOR) && defined(ESP_ARDUINO_VERSION_MINOR) && defined(ESP_ARDUINO_VERSION_PATCH)
#define ESP_ARDUINO_VERSION ESP_ARDUINO_VERSION_VAL(ESP_ARDUINO_VERSION_MAJOR, ESP_ARDUINO_VERSION_MINOR, ESP_ARDUINO_VERSION_PATCH)
#else
#define ESP_ARDUINO_VERSION ESP_ARDUINO_VERSION_VAL(0, 0, 0)
#endif
#endif

// ========== CONFIGURATION ==========
#define DIAGNOSTIC_VERSION "3.6.04-dev"
#define DIAGNOSTIC_HOSTNAME "esp32-diagnostic"
#define CUSTOM_LED_PIN -1
#define CUSTOM_LED_COUNT 1
#define ENABLE_I2C_SCAN true
const char* DIAGNOSTIC_VERSION_STR = DIAGNOSTIC_VERSION;

#if !defined(DIAGNOSTIC_PREFER_SECURE)
#define DIAGNOSTIC_PREFER_SECURE 0
#endif
const char* const DIAGNOSTIC_SECURE_SCHEME = "https://";
const char* const DIAGNOSTIC_LEGACY_SCHEME = "http://";

static inline String buildAccessUrl(const String& hostOrAddress,
                                    bool preferSecure = (DIAGNOSTIC_PREFER_SECURE != 0)) {
  if (hostOrAddress.length() == 0) {
    return String();
  }
  const String secureScheme = String(DIAGNOSTIC_SECURE_SCHEME);
  const String legacyScheme = String(DIAGNOSTIC_LEGACY_SCHEME);
  return (preferSecure ? secureScheme : legacyScheme) + hostOrAddress;
}

static inline String getStableAccessHost() {
  return String(DIAGNOSTIC_HOSTNAME) + ".local";
}

String getArduinoCoreVersionString() {
#if defined(ESP_ARDUINO_VERSION_MAJOR) && defined(ESP_ARDUINO_VERSION_MINOR) && defined(ESP_ARDUINO_VERSION_PATCH)
  return String(ESP_ARDUINO_VERSION_MAJOR) + "." + String(ESP_ARDUINO_VERSION_MINOR) + "." + String(ESP_ARDUINO_VERSION_PATCH);
#else
  uint32_t value = ESP_ARDUINO_VERSION;
  int major = (value >> 16) & 0xFF;
  int minor = (value >> 8) & 0xFF;
  int patch = value & 0xFF;
  return String(major) + "." + String(minor) + "." + String(patch);
#endif
}

// Pins I2C pour OLED (modifiables via web)
int I2C_SCL = 20;
int I2C_SDA = 21;

// OLED 0.96" I2C
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C

uint8_t oledRotation = 0;

// --- [NEW FEATURE] LED RGB (pins modifiables via web) ---
int RGB_LED_PIN_R = 14;
int RGB_LED_PIN_G = 13;
int RGB_LED_PIN_B = 12;

// --- [NEW FEATURE] Buzzer (pin modifiable via web) ---
int BUZZER_PIN = 16;

// --- [NEW FEATURE] DHT11 Temperature & Humidity Sensor (pin modifiable via web) ---
int DHT11_PIN = 4;

// --- [NEW FEATURE] Light Sensor (pin modifiable via web) ---
int LIGHT_SENSOR_PIN = 17;

// --- [NEW FEATURE] Ultrasonic Distance Sensor HC-SR04 (pins modifiables via web) ---
int DISTANCE_TRIG_PIN = 7;
int DISTANCE_ECHO_PIN = 8;

// --- [NEW FEATURE] PIR Motion Sensor (pin modifiable via web) ---
int MOTION_SENSOR_PIN = 6;

// ========== OBJETS GLOBAUX ==========
WebServer server(80);
WiFiMulti wifiMulti;
#if DIAGNOSTIC_HAS_MDNS
bool mdnsServiceActive = false;
bool wifiPreviouslyConnected = false;
unsigned long lastMDNSAttempt = 0;
bool mdnsLastAttemptFailed = false;
#if defined(DIAGNOSTIC_USES_IDF_MDNS)
bool mdnsResponderInitialized = false;
#endif
#endif
#if BLE_STACK_SUPPORTED
BLEServer* bluetoothServer = nullptr;
BLEService* bluetoothService = nullptr;
BLECharacteristic* bluetoothCharacteristic = nullptr;
#endif
bool bluetoothCapable = false;
bool bluetoothEnabled = false;
bool bluetoothAdvertising = false;
String bluetoothDeviceName = "";
String defaultBluetoothName = "";
#if BLE_STACK_SUPPORTED
bool bluetoothClientConnected = false;
uint32_t bluetoothNotifyCounter = 0;
unsigned long lastBluetoothNotify = 0;

static const char* DIAG_BLE_SERVICE_UUID = "4fafc201-1fb5-459e-8fcc-c5c9c331914b";
static const char* DIAG_BLE_CHARACTERISTIC_UUID = "beb5483e-36e1-4688-b7f5-ea07361b26a8";

class DiagnosticBLECallbacks : public BLEServerCallbacks {
  void onConnect(BLEServer* server) override {
    bluetoothClientConnected = true;
    bluetoothAdvertising = false;
    Serial.println("[BLE] Client connecté.");
    bluetoothNotifyCounter = 0;
    lastBluetoothNotify = millis();
  }

  void onDisconnect(BLEServer* server) override {
    bluetoothClientConnected = false;
    Serial.println("[BLE] Client déconnecté. Reprise de la diffusion...");
    if (server) {
      server->startAdvertising();
    } else {
      BLEDevice::startAdvertising();
    }
    bluetoothNotifyCounter = 0;
    lastBluetoothNotify = millis();
    bluetoothAdvertising = true;
  }
};
#endif
Adafruit_SSD1306 oled(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// NeoPixel
int LED_PIN = CUSTOM_LED_PIN;
int LED_COUNT = CUSTOM_LED_COUNT;
Adafruit_NeoPixel *strip = nullptr;

bool neopixelTested = false;
bool neopixelAvailable = false;
bool neopixelSupported = false;
String neopixelTestResult = String(T().not_tested);

// LED intégrée
int BUILTIN_LED_PIN = -1;
bool builtinLedTested = false;
bool builtinLedAvailable = false;
String builtinLedTestResult = String(T().not_tested);

// OLED
bool oledTested = false;
bool oledAvailable = false;
String oledTestResult = String(T().not_tested);

// Tests additionnels
String adcTestResult = String(T().not_tested);
String pwmTestResult = String(T().not_tested);
String partitionsInfo = "";
String spiInfo = "";
String stressTestResult = String(T().not_tested);

// --- [NEW FEATURE] Résultats de tests des nouveaux capteurs ---
String rgbLedTestResult = String(T().not_tested);
bool rgbLedAvailable = false;

String buzzerTestResult = String(T().not_tested);
bool buzzerAvailable = false;

String dht11TestResult = String(T().not_tested);
bool dht11Available = false;
float dht11Temperature = -999.0;
float dht11Humidity = -999.0;

String lightSensorTestResult = String(T().not_tested);
bool lightSensorAvailable = false;
int lightSensorValue = -1;

String distanceSensorTestResult = String(T().not_tested);
bool distanceSensorAvailable = false;
float distanceValue = -1.0;

String motionSensorTestResult = String(T().not_tested);
bool motionSensorAvailable = false;
bool motionDetected = false;

// ========== STRUCTURES ==========
struct DiagnosticInfo {
  String chipModel;
  String chipRevision;
  int cpuCores;
  uint32_t cpuFreqMHz;
  uint32_t flashSize;
  uint32_t psramSize;
  String macAddress;
  
  uint32_t heapSize;
  uint32_t freeHeap;
  uint32_t minFreeHeap;
  uint32_t maxAllocHeap;
  
  bool hasWiFi;
  bool hasBT;
  bool hasBLE;
  String wifiSSID;
  int wifiRSSI;
  String ipAddress;
  bool mdnsAvailable;
  bool bluetoothEnabled;
  bool bluetoothAdvertising;
  String bluetoothName;
  String bluetoothAddress;

  String gpioList;
  int totalGPIO;
  
  String sdkVersion;
  String idfVersion;
  unsigned long uptime;
  float temperature;
  
  bool neopixelTested;
  bool neopixelAvailable;
  String neopixelResult;
  
  unsigned long cpuBenchmark;
  unsigned long memBenchmark;
  
  String i2cDevices;
  int i2cCount;
  
  bool oledTested;
  bool oledAvailable;
  String oledResult;
} diagnosticData;

struct DetailedMemoryInfo {
  uint32_t flashSizeReal;
  uint32_t flashSizeChip;
  
  uint32_t psramTotal;
  uint32_t psramFree;
  uint32_t psramUsed;
  uint32_t psramLargestBlock;
  bool psramAvailable;
  bool psramConfigured;
  bool psramBoardSupported;
  const char* psramType;

  uint32_t sramTotal;
  uint32_t sramFree;
  uint32_t sramUsed;
  uint32_t sramLargestBlock;
  
  bool sramTestPassed;
  bool psramTestPassed;
  float fragmentationPercent;
  String memoryStatus;
};

DetailedMemoryInfo detailedMemory;

struct GPIOTestResult {
  int pin;
  bool tested;
  bool working;
  String mode;
  String notes;
};

std::vector<GPIOTestResult> gpioResults;

struct WiFiNetwork {
  String ssid;
  int rssi;
  int channel;
  String encryption;
  String bssid;
  int freqMHz;
  String band;
  String bandwidth;
  String phyModes;
};

std::vector<WiFiNetwork> wifiNetworks;

struct ADCReading {
  int pin;
  int rawValue;
  float voltage;
};

std::vector<ADCReading> adcReadings;

#define HISTORY_SIZE 60
float heapHistory[HISTORY_SIZE];
float tempHistory[HISTORY_SIZE];
int historyIndex = 0;

String getStableAccessURL() {
  return buildAccessUrl(getStableAccessHost());
}

void configureNetworkHostname() {
  if (!WiFi.setHostname(DIAGNOSTIC_HOSTNAME)) {
    Serial.printf("[WiFi] Impossible de définir le nom d'hôte %s\r\n", DIAGNOSTIC_HOSTNAME);
  } else {
    Serial.printf("[WiFi] Nom d'hôte défini : %s.local\r\n", DIAGNOSTIC_HOSTNAME);
  }
#if defined(ARDUINO_ARCH_ESP32)
  #if defined(DIAGNOSTIC_HAS_ESP_NETIF)
    esp_netif_t* staNetif = esp_netif_get_handle_from_ifkey("WIFI_STA_DEF");
    if (staNetif != nullptr) {
      esp_err_t netifStatus = esp_netif_set_hostname(staNetif, DIAGNOSTIC_HOSTNAME);
      if (netifStatus != ESP_OK) {
        Serial.printf("[WiFi] Échec assignation hostname netif (0x%X)\r\n", netifStatus);
      }
    } else {
      Serial.println("[WiFi] Interface STA introuvable pour l'assignation du hostname");
    }
  #elif defined(DIAGNOSTIC_HAS_TCPIP_ADAPTER)
    err_t adapterStatus = tcpip_adapter_set_hostname(TCPIP_ADAPTER_IF_STA, DIAGNOSTIC_HOSTNAME);
    if (adapterStatus != ERR_OK) {
      Serial.printf("[WiFi] Échec assignation hostname TCP/IP (%d)\r\n", static_cast<int>(adapterStatus));
    }
  #endif
#endif
}

void stopMDNSService(const char* reason) {
#if DIAGNOSTIC_HAS_MDNS
  if (!mdnsServiceActive) {
    return;
  }
#if defined(DIAGNOSTIC_USES_ESPMDNS)
  DIAGNOSTIC_MDNS_SERVER.end();
#elif defined(DIAGNOSTIC_USES_IDF_MDNS)
  if (mdnsResponderInitialized) {
    mdns_free();
    mdnsResponderInitialized = false;
  }
#endif
  mdnsServiceActive = false;
  if (reason != nullptr) {
    Serial.printf("[mDNS] Service arrêté (%s)\r\n", reason);
  } else {
    Serial.println("[mDNS] Service arrêté");
  }
#else
  (void) reason;
#endif
}

bool startMDNSService(bool verbose) {
#if DIAGNOSTIC_HAS_MDNS
  if (!WiFi.isConnected()) {
    return false;
  }
  if (mdnsServiceActive) {
    return true;
  }

  unsigned long now = millis();
  if (!verbose && (now - lastMDNSAttempt < 5000)) {
    return false;
  }

  lastMDNSAttempt = now;
#if defined(DIAGNOSTIC_USES_ESPMDNS)
  if (!DIAGNOSTIC_MDNS_SERVER.begin(DIAGNOSTIC_HOSTNAME)) {
    if (verbose || !mdnsLastAttemptFailed) {
      Serial.println("[mDNS] Échec du démarrage - nouvel essai dans 5s");
    }
    mdnsLastAttemptFailed = true;
    return false;
  }

  DIAGNOSTIC_MDNS_SERVER.addService("http", "tcp", 80);
#elif defined(DIAGNOSTIC_USES_IDF_MDNS)
  esp_err_t mdnsInitStatus = mdns_init();
  if (mdnsInitStatus != ESP_OK) {
    if (verbose || !mdnsLastAttemptFailed) {
      Serial.printf("[mDNS] Initialisation IDF échouée: 0x%02X\r\n", mdnsInitStatus);
    }
    mdnsLastAttemptFailed = true;
    return false;
  }

  mdnsResponderInitialized = true;

  esp_err_t hostStatus = mdns_hostname_set(DIAGNOSTIC_HOSTNAME);
  if (hostStatus != ESP_OK) {
    if (verbose || !mdnsLastAttemptFailed) {
      Serial.printf("[mDNS] Attribution du nom d'hôte échouée: 0x%02X\r\n", hostStatus);
    }
    mdns_free();
    mdnsResponderInitialized = false;
    mdnsLastAttemptFailed = true;
    return false;
  }

  mdns_instance_name_set("ESP32 Diagnostic");
  esp_err_t serviceStatus = mdns_service_add("ESP32 Diagnostic", "_http", "_tcp", 80, nullptr, 0);
  if (serviceStatus != ESP_OK) {
    if (verbose || !mdnsLastAttemptFailed) {
      Serial.printf("[mDNS] Publication du service échouée: 0x%02X\r\n", serviceStatus);
    }
    mdns_free();
    mdnsResponderInitialized = false;
    mdnsLastAttemptFailed = true;
    return false;
  }
  mdns_service_txt_item_set("_http", "_tcp", "path", "/");
#endif
  mdnsServiceActive = true;
  mdnsLastAttemptFailed = false;
  Serial.printf("[mDNS] Service actif sur %s\r\n", getStableAccessURL().c_str());
  return true;
#else
  (void) verbose;
  return false;
#endif
}

void maintainNetworkServices() {
#if DIAGNOSTIC_HAS_MDNS
  bool wifiConnectedNow = (WiFi.status() == WL_CONNECTED);
  if (wifiConnectedNow) {
    if (!wifiPreviouslyConnected) {
      startMDNSService(true);
    } else {
      startMDNSService(false);
    }
  } else if (mdnsServiceActive) {
    stopMDNSService("WiFi déconnecté");
  }
  wifiPreviouslyConnected = wifiConnectedNow;
#endif
}

// ========== DÉTECTION MODÈLE ==========
String detectChipModel() {
  #ifdef CONFIG_IDF_TARGET_ESP32
    return "ESP32";
  #elif defined(CONFIG_IDF_TARGET_ESP32S2)
    return "ESP32-S2";
  #elif defined(CONFIG_IDF_TARGET_ESP32S3)
    return "ESP32-S3";
  #elif defined(CONFIG_IDF_TARGET_ESP32C3)
    return "ESP32-C3";
  #elif defined(CONFIG_IDF_TARGET_ESP32C6)
    return "ESP32-C6";
  #elif defined(CONFIG_IDF_TARGET_ESP32H2)
    return "ESP32-H2";
  #else
    return "ESP32";
  #endif
}

// ========== FONCTIONS UTILITAIRES ==========
String getChipFeatures() {
  esp_chip_info_t chip_info;
  esp_chip_info(&chip_info);
  String features = "";
  
  if (chip_info.features & CHIP_FEATURE_WIFI_BGN) features += "WiFi 2.4GHz, ";
  if (chip_info.features & CHIP_FEATURE_BT) features += "Bluetooth Classic, ";
  if (chip_info.features & CHIP_FEATURE_BLE) features += "Bluetooth LE, ";
  
  if (features.length() > 0) {
    features = features.substring(0, features.length() - 2);
  } else {
    features = T().none.str();
  }
  
  return features;
}

String getFlashType() {
  #ifdef CONFIG_ESPTOOLPY_FLASHMODE_QIO
    return "QIO";
  #elif defined(CONFIG_ESPTOOLPY_FLASHMODE_QOUT)
    return "QOUT";
  #elif defined(CONFIG_ESPTOOLPY_FLASHMODE_DIO)
    return "DIO";
  #elif defined(CONFIG_ESPTOOLPY_FLASHMODE_DOUT)
    return "DOUT";
  #else
    return T().unknown.str();
  #endif
}

String getFlashSpeed() {
  #ifdef CONFIG_ESPTOOLPY_FLASHFREQ_80M
    return "80 MHz";
  #elif defined(CONFIG_ESPTOOLPY_FLASHFREQ_40M)
    return "40 MHz";
  #elif defined(CONFIG_ESPTOOLPY_FLASHFREQ_26M)
    return "26 MHz";
  #elif defined(CONFIG_ESPTOOLPY_FLASHFREQ_20M)
    return "20 MHz";
  #else
    return T().unknown.str();
  #endif
}

const char* getResetReasonKey() {
  esp_reset_reason_t reason = esp_reset_reason();
  switch (reason) {
    case ESP_RST_POWERON: return "poweron";
    case ESP_RST_SW: return "software_reset";
    case ESP_RST_DEEPSLEEP: return "deepsleep_exit";
    case ESP_RST_BROWNOUT: return "brownout";
    default: return "other";
  }
}

String getResetReason() {
  esp_reset_reason_t reason = esp_reset_reason();
  switch (reason) {
    case ESP_RST_POWERON: return T().poweron.str();
    case ESP_RST_SW: return T().software_reset.str();
    case ESP_RST_DEEPSLEEP: return T().deepsleep_exit.str();
    case ESP_RST_BROWNOUT: return T().brownout.str();
    default: return T().other.str();
  }
}

String formatUptime(unsigned long days, unsigned long hours, unsigned long minutes) {
  String formatted;
  if (days > 0) {
    formatted += String(days) + " " + String(T().days);
  }
  if (hours > 0 || formatted.length() > 0) {
    if (formatted.length() > 0) {
      formatted += " ";
    }
    formatted += String(hours) + " " + String(T().hours);
  }
  if (minutes > 0 || formatted.length() == 0) {
    if (formatted.length() > 0) {
      formatted += " ";
    }
    formatted += String(minutes) + " " + String(T().minutes);
  }
  return formatted;
}

String getMemoryStatus() {
  float heapUsagePercent = ((float)(diagnosticData.heapSize - diagnosticData.freeHeap) / diagnosticData.heapSize) * 100;
  if (heapUsagePercent < 50) return T().excellent.str();
  else if (heapUsagePercent < 70) return T().good.str();
  else if (heapUsagePercent < 85) return T().warning.str();
  else return T().critical.str();
}

const char* getWiFiSignalQualityKey() {
  if (diagnosticData.wifiRSSI >= -50) return "excellent";
  else if (diagnosticData.wifiRSSI >= -60) return "very_good";
  else if (diagnosticData.wifiRSSI >= -70) return "good";
  else if (diagnosticData.wifiRSSI >= -80) return "weak";
  else return "very_weak";
}

String getWiFiSignalQuality() {
  const char* key = getWiFiSignalQualityKey();
  if (key == nullptr) {
    return T().unknown.str();
  }
  if (strcmp(key, "excellent") == 0) return T().excellent.str();
  if (strcmp(key, "very_good") == 0) return T().very_good.str();
  if (strcmp(key, "good") == 0) return T().good.str();
  if (strcmp(key, "weak") == 0) return T().weak.str();
  if (strcmp(key, "very_weak") == 0) return T().very_weak.str();
  return T().unknown.str();
}

// --- [TRANSLATION FIX] Use translation keys for WiFi auth modes ---
String wifiAuthModeToString(wifi_auth_mode_t mode) {
  switch (mode) {
    case WIFI_AUTH_OPEN: return T().wifi_open_auth.str();
#ifdef WIFI_AUTH_WEP
    case WIFI_AUTH_WEP: return "WEP";
#endif
    case WIFI_AUTH_WPA_PSK: return "WPA-PSK";
    case WIFI_AUTH_WPA2_PSK: return "WPA2-PSK";
    case WIFI_AUTH_WPA_WPA2_PSK: return "WPA/WPA2";
#ifdef WIFI_AUTH_WPA2_ENTERPRISE
    case WIFI_AUTH_WPA2_ENTERPRISE: return "WPA2-ENT";
#endif
#ifdef WIFI_AUTH_WPA3_PSK
    case WIFI_AUTH_WPA3_PSK: return "WPA3-PSK";
#endif
#ifdef WIFI_AUTH_WPA2_WPA3_PSK
    case WIFI_AUTH_WPA2_WPA3_PSK: return "WPA2/WPA3";
#endif
#ifdef WIFI_AUTH_WAPI_PSK
    case WIFI_AUTH_WAPI_PSK: return "WAPI-PSK";
#endif
#ifdef WIFI_AUTH_OWE
    case WIFI_AUTH_OWE: return "OWE";
#endif
    default: return T().unknown.str();
  }
}

#if defined(WIFI_CIPHER_TYPE_NONE)
String wifiCipherToString(wifi_cipher_type_t cipher) {
  switch (cipher) {
    case WIFI_CIPHER_TYPE_NONE: return "None";
    case WIFI_CIPHER_TYPE_WEP40: return "WEP40";
    case WIFI_CIPHER_TYPE_WEP104: return "WEP104";
    case WIFI_CIPHER_TYPE_TKIP: return "TKIP";
    case WIFI_CIPHER_TYPE_CCMP: return "CCMP";
    case WIFI_CIPHER_TYPE_TKIP_CCMP: return "TKIP/CCMP";
#ifdef WIFI_CIPHER_TYPE_GCMP
    case WIFI_CIPHER_TYPE_GCMP: return "GCMP";
#endif
#ifdef WIFI_CIPHER_TYPE_GCMP256
    case WIFI_CIPHER_TYPE_GCMP256: return "GCMP256";
#endif
#ifdef WIFI_CIPHER_TYPE_AES_CMAC128
    case WIFI_CIPHER_TYPE_AES_CMAC128: return "CMAC128";
#endif
#ifdef WIFI_CIPHER_TYPE_SMS4
    case WIFI_CIPHER_TYPE_SMS4: return "SMS4";
#endif
    default: return "-";
  }
}
#endif

String wifiBandwidthToString(wifi_second_chan_t secondary) {
  switch (secondary) {
    case WIFI_SECOND_CHAN_NONE: return "20 MHz";
    case WIFI_SECOND_CHAN_ABOVE:
    case WIFI_SECOND_CHAN_BELOW: return "40 MHz";
    default: return "Auto";
  }
}

String wifiChannelToBand(int channel) {
  if (channel <= 0) return "?";
  if (channel <= 14) return "2.4 GHz";
  if (channel >= 180) return "6 GHz";
  return "5 GHz";
}

int wifiChannelToFrequency(int channel) {
  if (channel <= 0) return 0;
  if (channel <= 14) return 2407 + channel * 5;
  if (channel >= 180) return 5950 + channel * 5;
  return 5000 + channel * 5;
}

String wifiPhyModesToString(const wifi_ap_record_t& record) {
  String modes = "";
  if (record.phy_11b) {
    if (modes.length()) modes += "/";
    modes += "11b";
  }
  if (record.phy_11g) {
    if (modes.length()) modes += "/";
    modes += "11g";
  }
  if (record.phy_11n) {
    if (modes.length()) modes += "/";
    modes += "11n";
  }
#ifdef CONFIG_ESP_WIFI_80211AC_SUPPORT
  if (record.phy_11ac) {
    if (modes.length()) modes += "/";
    modes += "11ac";
  }
#endif
#ifdef CONFIG_ESP_WIFI_80211AX_SUPPORT
  if (record.phy_11ax) {
    if (modes.length()) modes += "/";
    modes += "11ax";
  }
#endif
#ifdef CONFIG_ESP_WIFI_80211BE_SUPPORT
  if (record.phy_11be) {
    if (modes.length()) modes += "/";
    modes += "11be";
  }
#endif
  if (record.phy_lr) {
    if (modes.length()) modes += "/";
    modes += "LR";
  }
  if (modes.length() == 0) {
    modes = "-";
  }
  return modes;
}

String getGPIOList() {
  String gpioList = "";
  
  #ifdef CONFIG_IDF_TARGET_ESP32
    int gpios[] = {0,2,4,5,12,13,14,15,16,17,18,19,21,22,23,25,26,27,32,33};
    int numGPIO = 20;
  #elif defined(CONFIG_IDF_TARGET_ESP32S2)
    int gpios[] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21};
    int numGPIO = 22;
  #elif defined(CONFIG_IDF_TARGET_ESP32S3)
    int gpios[] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21};
    int numGPIO = 22;
  #elif defined(CONFIG_IDF_TARGET_ESP32C3)
    int gpios[] = {0,1,2,3,4,5,6,7,8,9,10};
    int numGPIO = 11;
  #else
    int gpios[] = {0,2,4,5};
    int numGPIO = 4;
  #endif
  
  for (int i = 0; i < numGPIO; i++) {
    gpioList += String(gpios[i]);
    if (i < numGPIO - 1) gpioList += ", ";
  }
  
  return gpioList;
}

int countGPIO() {
  #ifdef CONFIG_IDF_TARGET_ESP32
    return 26;
  #elif defined(CONFIG_IDF_TARGET_ESP32S2)
    return 37;
  #elif defined(CONFIG_IDF_TARGET_ESP32S3)
    return 39;
  #elif defined(CONFIG_IDF_TARGET_ESP32C3)
    return 15;
  #else
    return 20;
  #endif
}

// ========== MÉMOIRE DÉTAILLÉE ==========
bool testSRAMQuick() {
  const size_t testSize = 1024;
  uint8_t* testBuffer = (uint8_t*)malloc(testSize);
  if (!testBuffer) return false;
  
  memset(testBuffer, 0xAA, testSize);
  bool ok = (testBuffer[0] == 0xAA && testBuffer[testSize-1] == 0xAA);
  free(testBuffer);
  return ok;
}

bool testPSRAMQuick() {
  if (ESP.getPsramSize() == 0) return false;
  
  const size_t testSize = 1024;
  uint8_t* testBuffer = (uint8_t*)heap_caps_malloc(testSize, MALLOC_CAP_SPIRAM);
  if (!testBuffer) return false;
  
  memset(testBuffer, 0x55, testSize);
  bool ok = (testBuffer[0] == 0x55 && testBuffer[testSize-1] == 0x55);
  heap_caps_free(testBuffer);
  return ok;
}

void collectDetailedMemory() {
  uint32_t flashSizeReal;
  esp_flash_get_size(NULL, &flashSizeReal);
  
  detailedMemory.flashSizeChip = flashSizeReal;
  detailedMemory.flashSizeReal = ESP.getFlashChipSize();
  
  detailedMemory.psramTotal = ESP.getPsramSize();
  detailedMemory.psramAvailable = (detailedMemory.psramTotal > 0);

  detailedMemory.psramConfigured = false;
  detailedMemory.psramBoardSupported = false;
  detailedMemory.psramType = "PSRAM";

  #if defined(CONFIG_SPIRAM)
    detailedMemory.psramConfigured = true;
    detailedMemory.psramBoardSupported = true;
  #endif
  #if defined(CONFIG_SPIRAM_SUPPORT)
    detailedMemory.psramConfigured = true;
    detailedMemory.psramBoardSupported = true;
  #endif
  #if defined(BOARD_HAS_PSRAM)
    detailedMemory.psramConfigured = true;
    detailedMemory.psramBoardSupported = true;
  #endif
  #if defined(CONFIG_SPIRAM_MODE_OCT)
    detailedMemory.psramConfigured = true;
    detailedMemory.psramBoardSupported = true;
    detailedMemory.psramType = "OPI";
  #endif
  #if defined(CONFIG_SPIRAM_MODE_QUAD)
    detailedMemory.psramConfigured = true;
    detailedMemory.psramBoardSupported = true;
    detailedMemory.psramType = "QSPI";
  #endif

  if (detailedMemory.psramTotal == 0) {
    detailedMemory.psramConfigured = false;
  }
  
  if (detailedMemory.psramAvailable) {
    detailedMemory.psramFree = ESP.getFreePsram();
    detailedMemory.psramUsed = detailedMemory.psramTotal - detailedMemory.psramFree;
    
    multi_heap_info_t info;
    heap_caps_get_info(&info, MALLOC_CAP_SPIRAM);
    detailedMemory.psramLargestBlock = info.largest_free_block;
  } else {
    detailedMemory.psramFree = 0;
    detailedMemory.psramUsed = 0;
    detailedMemory.psramLargestBlock = 0;
  }
  
  multi_heap_info_t infoInternal;
  heap_caps_get_info(&infoInternal, MALLOC_CAP_INTERNAL);
  detailedMemory.sramTotal = infoInternal.total_free_bytes + infoInternal.total_allocated_bytes;
  detailedMemory.sramFree = infoInternal.total_free_bytes;
  detailedMemory.sramUsed = infoInternal.total_allocated_bytes;
  detailedMemory.sramLargestBlock = infoInternal.largest_free_block;
  
  size_t largestBlock = ESP.getMaxAllocHeap();
  size_t freeHeap = ESP.getFreeHeap();
  if (freeHeap > 0) {
    detailedMemory.fragmentationPercent = 100.0 - (100.0 * largestBlock / freeHeap);
  } else {
    detailedMemory.fragmentationPercent = 0;
  }
  
  detailedMemory.sramTestPassed = testSRAMQuick();
  detailedMemory.psramTestPassed = testPSRAMQuick();
  
  if (detailedMemory.fragmentationPercent < 20) {
    detailedMemory.memoryStatus = T().excellent.str();
  } else if (detailedMemory.fragmentationPercent < 40) {
    detailedMemory.memoryStatus = T().good.str();
  } else if (detailedMemory.fragmentationPercent < 60) {
    detailedMemory.memoryStatus = "Moyen"; // Pas traduit (statut technique)
  } else {
    detailedMemory.memoryStatus = T().critical.str();
  }
}

void printPSRAMDiagnostic() {
  Serial.println("\r\n=== DIAGNOSTIC PSRAM DETAILLE ===");
  Serial.printf("ESP.getPsramSize(): %u octets (%.2f MB)\r\n", 
                ESP.getPsramSize(), ESP.getPsramSize() / 1048576.0);
  
  Serial.println("\r\nFlags de compilation detectes (indication du type supporte par la carte):");
  bool anyFlag = false;
  String psramType = detailedMemory.psramType ? detailedMemory.psramType : T().unknown.str();
  
  #ifdef CONFIG_SPIRAM
    Serial.println("  ✓ CONFIG_SPIRAM");
    anyFlag = true;
  #endif
  #ifdef CONFIG_SPIRAM_SUPPORT
    Serial.println("  ✓ CONFIG_SPIRAM_SUPPORT");
    anyFlag = true;
  #endif
  #ifdef BOARD_HAS_PSRAM
    Serial.println("  ✓ BOARD_HAS_PSRAM");
    anyFlag = true;
  #endif
  #ifdef CONFIG_SPIRAM_MODE_OCT
    Serial.println("  ✓ CONFIG_SPIRAM_MODE_OCT (OPI) - Type supporte par la carte");
    psramType = "OPI";
    anyFlag = true;
  #endif
  #ifdef CONFIG_SPIRAM_MODE_QUAD
    Serial.println("  ✓ CONFIG_SPIRAM_MODE_QUAD (QSPI) - Type supporte par la carte");
    psramType = "QSPI";
    anyFlag = true;
  #endif
  
  if (!anyFlag) {
    Serial.println("  ✗ Aucun flag PSRAM detecte");
  }
  
  Serial.println("\r\nResultat logique:");
  Serial.printf("  psramTotal = %u octets\r\n", detailedMemory.psramTotal);
  Serial.printf("  psramConfigured = %s\r\n", detailedMemory.psramConfigured ? "TRUE" : "FALSE");
  Serial.printf("  psramAvailable = %s\r\n", detailedMemory.psramAvailable ? "TRUE" : "FALSE");
  
  Serial.println("\r\nConclusion:");
  if (ESP.getPsramSize() > 0) {
    Serial.printf("  → PSRAM fonctionnelle! Type: %s\r\n", psramType.c_str());
  } else if (anyFlag) {
    Serial.printf("  → Carte compatible PSRAM %s mais DESACTIVEE dans IDE\r\n", psramType.c_str());
    Serial.println("  → Pour activer: Tools → PSRAM → OPI PSRAM (ou QSPI)");
  } else {
    Serial.println("  → Carte sans support PSRAM");
  }
  Serial.println("=====================================\r\n");
}

// ========== BENCHMARKS ==========
unsigned long benchmarkCPU() {
  Serial.println("\r\n=== BENCHMARK CPU ===");
  unsigned long start = micros();
  
  volatile float result = 0;
  for(int i = 0; i < 100000; i++) {
    result += sqrt(i) * sin(i);
  }
  
  unsigned long duration = micros() - start;
  Serial.printf("CPU: %lu us (%.2f MFLOPS)\r\n", duration, 100000.0 / duration);
  return duration;
}

unsigned long benchmarkMemory() {
  Serial.println("=== BENCHMARK MEMOIRE ===");
  unsigned long start = micros();
  
  const int size = 10000;
  int* testArray = (int*)malloc(size * sizeof(int));
  if (!testArray) return 0;
  
  for(int i = 0; i < size; i++) testArray[i] = i;
  volatile int sum = 0;
  for(int i = 0; i < size; i++) sum += testArray[i];
  
  free(testArray);
  unsigned long duration = micros() - start;
  Serial.printf("Memory: %lu us\r\n", duration);
  return duration;
}

// ========== SCAN I2C ==========
void ensureI2CBusConfigured() {
  Wire.end();
#if ESP_ARDUINO_VERSION >= ESP_ARDUINO_VERSION_VAL(3, 3, 0)
  Wire.setPins(I2C_SDA, I2C_SCL);
  Wire.begin();
#else
  Wire.begin(I2C_SDA, I2C_SCL);
#endif
  Wire.setClock(400000);
}

void scanI2C() {
  if (!ENABLE_I2C_SCAN) return;

  Serial.println("\r\n=== SCAN I2C ===");
  ensureI2CBusConfigured();
  Serial.printf("I2C: SDA=%d, SCL=%d\r\n", I2C_SDA, I2C_SCL);
  
  diagnosticData.i2cDevices = "";
  diagnosticData.i2cCount = 0;
  
  for (byte address = 1; address < 127; address++) {
    Wire.beginTransmission(address);
    if (Wire.endTransmission() == 0) {
      char addr[6];
      sprintf(addr, "0x%02X", address);
      if (diagnosticData.i2cCount > 0) diagnosticData.i2cDevices += ", ";
      diagnosticData.i2cDevices += String(addr);
      diagnosticData.i2cCount++;
    }
  }
  
  if (diagnosticData.i2cCount == 0) {
    diagnosticData.i2cDevices = "Aucun";
  }
  Serial.printf("I2C: %d peripherique(s)\r\n", diagnosticData.i2cCount);
}

// ========== SCAN WIFI ==========
void scanWiFiNetworks() {
  Serial.println("\r\n=== SCAN WIFI ===");
  wifiNetworks.clear();

#if ESP_ARDUINO_VERSION >= ESP_ARDUINO_VERSION_VAL(3, 3, 0)
  int n = WiFi.scanNetworks(false, true, true);
#else
  int n = WiFi.scanNetworks();
#endif

  if (n < 0) {
    Serial.println("WiFi: scan en echec");
    return;
  }

  for (int i = 0; i < n; i++) {
    WiFiNetwork net;

#if ESP_ARDUINO_VERSION >= ESP_ARDUINO_VERSION_VAL(3, 3, 0)
    wifi_ap_record_t* info = static_cast<wifi_ap_record_t*>(WiFi.getScanInfoByIndex(i));
    if (info != nullptr) {
      net.ssid = String(reinterpret_cast<const char*>(info->ssid));
      net.rssi = info->rssi;
      net.channel = info->primary;

      char bssidStr[18];
      sprintf(bssidStr, "%02X:%02X:%02X:%02X:%02X:%02X",
              info->bssid[0], info->bssid[1], info->bssid[2], info->bssid[3], info->bssid[4], info->bssid[5]);
      net.bssid = String(bssidStr);

      net.encryption = wifiAuthModeToString(info->authmode);
#if defined(WIFI_CIPHER_TYPE_NONE)
      String pairwise = wifiCipherToString(info->pairwise_cipher);
      String group = wifiCipherToString(info->group_cipher);
      if ((pairwise != "-" && pairwise != "None") || (group != "-" && group != "None")) {
        net.encryption += " (" + pairwise + "/" + group + ")";
      }
#endif
      net.bandwidth = wifiBandwidthToString(info->second);
      net.band = wifiChannelToBand(info->primary);
      net.freqMHz = wifiChannelToFrequency(info->primary);
      net.phyModes = wifiPhyModesToString(*info);
    } else
#endif
    {
      net.ssid = WiFi.SSID(i);
      net.rssi = WiFi.RSSI(i);
      net.channel = WiFi.channel(i);

      uint8_t* bssid = WiFi.BSSID(i);
      char bssidFallback[18];
      sprintf(bssidFallback, "%02X:%02X:%02X:%02X:%02X:%02X",
              bssid[0], bssid[1], bssid[2], bssid[3], bssid[4], bssid[5]);
      net.bssid = String(bssidFallback);

      wifi_auth_mode_t auth = WiFi.encryptionType(i);
      net.encryption = wifiAuthModeToString(auth);
      net.band = wifiChannelToBand(net.channel);
      net.bandwidth = "-";
      net.freqMHz = wifiChannelToFrequency(net.channel);
      net.phyModes = "-";
    }

    if (net.bandwidth.length() == 0) net.bandwidth = "-";
    wifiNetworks.push_back(net);
  }

#if ESP_ARDUINO_VERSION >= ESP_ARDUINO_VERSION_VAL(3, 3, 0)
  WiFi.scanDelete();
#endif

  Serial.printf("WiFi: %d reseaux trouves\r\n", n);
}

// ========== TEST GPIO ==========
bool testSingleGPIO(int pin) {
  if (pin == 1 || pin == 3) return false;
  if (pin >= 6 && pin <= 11) return false;
  
  pinMode(pin, OUTPUT);
  digitalWrite(pin, HIGH);
  delay(10);
  bool highOk = digitalRead(pin) == HIGH;
  
  digitalWrite(pin, LOW);
  delay(10);
  bool lowOk = digitalRead(pin) == LOW;
  
  pinMode(pin, INPUT);
  return highOk && lowOk;
}

void testAllGPIOs() {
  Serial.println("\r\n=== TEST GPIO ===");
  gpioResults.clear();
  
  #ifdef CONFIG_IDF_TARGET_ESP32
    int gpios[] = {0,2,4,5,12,13,14,15,16,17,18,19,21,22,23,25,26,27,32,33};
    int numGPIO = 20;
  #elif defined(CONFIG_IDF_TARGET_ESP32S3)
    int gpios[] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21};
    int numGPIO = 22;
  #elif defined(CONFIG_IDF_TARGET_ESP32C3)
    int gpios[] = {0,1,2,3,4,5,6,7,8,9,10};
    int numGPIO = 11;
  #else
    int gpios[] = {0,2,4,5};
    int numGPIO = 4;
  #endif
  
  for (int i = 0; i < numGPIO; i++) {
    GPIOTestResult result;
    result.pin = gpios[i];
    result.tested = true;
    result.working = testSingleGPIO(gpios[i]);
    result.mode = "Digital I/O";
    result.notes = result.working ? String(T().ok) : String(T().fail);
    gpioResults.push_back(result);
  }
  Serial.printf("GPIO: %d testes\r\n", numGPIO);
}

// ========== LED INTÉGRÉE ==========
void detectBuiltinLED() {
  String chipModel = detectChipModel();
  
  #ifdef LED_BUILTIN
    BUILTIN_LED_PIN = LED_BUILTIN;
  #else
    if (chipModel == "ESP32-S3") BUILTIN_LED_PIN = 2;
    else if (chipModel == "ESP32-C3") BUILTIN_LED_PIN = 8;
    else if (chipModel == "ESP32-S2") BUILTIN_LED_PIN = 15;
    else BUILTIN_LED_PIN = 2;
  #endif
  
  builtinLedTestResult = String(T().gpio) + " " + String(BUILTIN_LED_PIN) + " - " + String(T().not_tested);
  Serial.printf("LED integree: GPIO %d\r\n", BUILTIN_LED_PIN);
}

void testBuiltinLED() {
  if (BUILTIN_LED_PIN == -1) return;
  
  Serial.println("\r\n=== TEST LED ===");
  pinMode(BUILTIN_LED_PIN, OUTPUT);
  
  for(int i = 0; i < 5; i++) {
    digitalWrite(BUILTIN_LED_PIN, HIGH);
    delay(200);
    digitalWrite(BUILTIN_LED_PIN, LOW);
    delay(200);
  }
  
  for(int i = 0; i <= 255; i += 5) {
    analogWrite(BUILTIN_LED_PIN, i);
    delay(10);
  }
  for(int i = 255; i >= 0; i -= 5) {
    analogWrite(BUILTIN_LED_PIN, i);
    delay(10);
  }
  
  digitalWrite(BUILTIN_LED_PIN, LOW);
  builtinLedAvailable = true;
  builtinLedTestResult = String(T().test) + " " + String(T().ok) + " - GPIO " + String(BUILTIN_LED_PIN);
  builtinLedTested = true;
  Serial.println("LED: OK");
}

void resetBuiltinLEDTest() {
  builtinLedTested = false;
  builtinLedAvailable = false;
  if (BUILTIN_LED_PIN != -1) digitalWrite(BUILTIN_LED_PIN, LOW);
}

// ========== NEOPIXEL ==========
void detectNeoPixelSupport() {
  String chipModel = detectChipModel();
  
  if (CUSTOM_LED_PIN != -1) {
    LED_PIN = CUSTOM_LED_PIN;
    neopixelSupported = true;
  }
  else if (chipModel == "ESP32-S3") {
    LED_PIN = 48;
    neopixelSupported = true;
  } 
  else if (chipModel == "ESP32-C3") {
    LED_PIN = 8;
    neopixelSupported = true;
  }
  else {
    LED_PIN = 2;
    neopixelSupported = false;
  }
  
  if (strip != nullptr) delete strip;
  strip = new Adafruit_NeoPixel(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);
  neopixelTestResult = String(T().gpio) + " " + String(LED_PIN) + " - " + String(T().not_tested);
  Serial.printf("NeoPixel: GPIO %d\r\n", LED_PIN);
}

void testNeoPixel() {
  if (!strip) return;
  
  Serial.println("\r\n=== TEST NEOPIXEL ===");
  strip->begin();
  strip->clear();
  strip->show();
  
  strip->setPixelColor(0, strip->Color(255, 0, 0));
  strip->show();
  delay(500);
  
  strip->setPixelColor(0, strip->Color(0, 255, 0));
  strip->show();
  delay(500);
  
  strip->setPixelColor(0, strip->Color(0, 0, 255));
  strip->show();
  delay(500);
  
  for(int i = 0; i < 256; i += 32) {
    strip->setPixelColor(0, strip->gamma32(strip->ColorHSV(i * 256)));
    strip->show();
    delay(50);
  }
  
  strip->clear();
  strip->show();
  
  neopixelAvailable = true;
  neopixelTestResult = String(T().test) + " " + String(T().ok) + " - GPIO " + String(LED_PIN);
  neopixelTested = true;
  Serial.println("NeoPixel: OK");
}

void resetNeoPixelTest() {
  neopixelTested = false;
  neopixelAvailable = false;
  if (strip) {
    strip->clear();
    strip->show();
  }
}

void neopixelRainbow() {
  if (!strip) return;
  for(int i = 0; i < 256; i++) {
    for(int j = 0; j < LED_COUNT; j++) {
      strip->setPixelColor(j, strip->gamma32(strip->ColorHSV(i * 256)));
    }
    strip->show();
    delay(10);
  }
}

void neopixelBlink(uint32_t color, int times) {
  if (!strip) return;
  for(int i = 0; i < times; i++) {
    strip->fill(color);
    strip->show();
    delay(300);
    strip->clear();
    strip->show();
    delay(300);
  }
}

void neopixelFade(uint32_t color) {
  if (!strip) return;
  for(int brightness = 0; brightness <= 255; brightness += 5) {
    strip->setBrightness(brightness);
    strip->fill(color);
    strip->show();
    delay(20);
  }
  for(int brightness = 255; brightness >= 0; brightness -= 5) {
    strip->setBrightness(brightness);
    strip->fill(color);
    strip->show();
    delay(20);
  }
  strip->setBrightness(255);
}

void applyOLEDOrientation() {
  oled.setRotation(oledRotation & 0x03);
}

// ========== OLED 0.96" ==========
void detectOLED() {
  Serial.println("\r\n=== DETECTION OLED ===");
  ensureI2CBusConfigured();
  Serial.printf("I2C: SDA=%d, SCL=%d\r\n", I2C_SDA, I2C_SCL);

  Wire.beginTransmission(SCREEN_ADDRESS);
  bool i2cDetected = (Wire.endTransmission() == 0);

  if(i2cDetected && oled.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    oledAvailable = true;
    applyOLEDOrientation();
    oledTestResult = String(T().detected) + " @ 0x" + String(SCREEN_ADDRESS, HEX);
    Serial.println("OLED: Detecte!\r\n");
  } else {
    oledAvailable = false;
    if (i2cDetected) {
      oledTestResult = String(T().i2c_peripherals) + " - " + String(T().fail);
    } else {
      oledTestResult = String(T().not_detected) + " (SDA:" + String(I2C_SDA) + " SCL:" + String(I2C_SCL) + ")";
    }
    Serial.println("OLED: Non detecte\r\n");
  }
}

void oledStepWelcome() {
  if (!oledAvailable) return;
  applyOLEDOrientation();
  oled.clearDisplay();
  oled.setTextSize(1);
  oled.setTextColor(SSD1306_WHITE);
  oled.setCursor(0, 0);
  oled.println("TEST OLED 0.96\"");
  oled.println();
  oled.println("128x64 pixels");
  oled.printf("I2C: 0x%02X\r\n", SCREEN_ADDRESS);
  oled.printf("SDA:%d SCL:%d", I2C_SDA, I2C_SCL);
  oled.display();
  delay(2000);
}

void oledStepBigText() {
  if (!oledAvailable) return;
  applyOLEDOrientation();
  oled.clearDisplay();
  oled.setTextSize(2);
  oled.setTextColor(SSD1306_WHITE);
  oled.setCursor(20, 20);
  oled.println("ESP32");
  oled.display();
  delay(1500);
}

void oledStepTextSizes() {
  if (!oledAvailable) return;
  applyOLEDOrientation();
  oled.clearDisplay();
  oled.setTextSize(1);
  oled.setTextColor(SSD1306_WHITE);
  oled.setCursor(0, 0);
  oled.println("Taille 1");
  oled.setTextSize(2);
  oled.println("Taille 2");
  oled.setTextSize(1);
  oled.println("Retour taille 1");
  oled.display();
  delay(2000);
}

void oledStepShapes() {
  if (!oledAvailable) return;
  applyOLEDOrientation();
  oled.clearDisplay();
  oled.drawRect(10, 10, 30, 20, SSD1306_WHITE);
  oled.fillRect(50, 10, 30, 20, SSD1306_WHITE);
  oled.drawCircle(25, 50, 10, SSD1306_WHITE);
  oled.fillCircle(65, 50, 10, SSD1306_WHITE);
  oled.drawTriangle(95, 30, 85, 10, 105, 10, SSD1306_WHITE);
  oled.display();
  delay(2000);
}

void oledStepHorizontalLines() {
  if (!oledAvailable) return;
  applyOLEDOrientation();
  oled.clearDisplay();
  for (int i = 0; i < SCREEN_HEIGHT; i += 4) {
    oled.drawLine(0, i, SCREEN_WIDTH, i, SSD1306_WHITE);
  }
  oled.display();
  delay(1500);
}

void oledStepDiagonals() {
  if (!oledAvailable) return;
  applyOLEDOrientation();
  oled.clearDisplay();
  for (int i = 0; i < SCREEN_WIDTH; i += 8) {
    oled.drawLine(0, 0, i, SCREEN_HEIGHT - 1, SSD1306_WHITE);
    oled.drawLine(SCREEN_WIDTH - 1, 0, i, SCREEN_HEIGHT - 1, SSD1306_WHITE);
  }
  oled.display();
  delay(1500);
}

void oledStepMovingSquare() {
  if (!oledAvailable) return;
  applyOLEDOrientation();
  for (int x = 0; x < SCREEN_WIDTH - 20; x += 4) {
    oled.clearDisplay();
    oled.fillRect(x, 22, 20, 20, SSD1306_WHITE);
    oled.display();
    delay(20);
  }
}

void oledStepProgressBar() {
  if (!oledAvailable) return;
  applyOLEDOrientation();
  oled.clearDisplay();
  oled.setTextSize(1);
  oled.setTextColor(SSD1306_WHITE);
  oled.setCursor(20, 10);
  oled.println(String(T().loading));
  for (int i = 0; i <= 100; i += 5) {
    oled.drawRect(10, 30, 108, 15, SSD1306_WHITE);
    oled.fillRect(12, 32, i, 11, SSD1306_WHITE);
    oled.setCursor(45, 50);
    oled.printf("%d%%", i);
    oled.display();
    delay(100);
    if (i < 100) {
      oled.fillRect(12, 32, i, 11, SSD1306_BLACK);
      oled.fillRect(45, 50, 40, 10, SSD1306_BLACK);
    }
  }
  delay(1000);
}

void oledStepScrollText() {
  if (!oledAvailable) return;
  applyOLEDOrientation();
  String scrollText = "  DIAGNOSTIC ESP32 COMPLET - OLED 0.96 pouces I2C  ";
  for (int offset = 0; offset < scrollText.length() * 6; offset += 2) {
    oled.clearDisplay();
    oled.setTextSize(1);
    oled.setTextColor(SSD1306_WHITE);
    oled.setCursor(-offset, 28);
    oled.print(scrollText);
    oled.display();
    delay(30);
  }
}

void oledStepFinalMessage() {
  if (!oledAvailable) return;
  applyOLEDOrientation();
  oled.clearDisplay();
  oled.setTextSize(1);
  oled.setTextColor(SSD1306_WHITE);
  oled.setCursor(30, 20);
  oled.println("TEST OK!");
  oled.drawRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, SSD1306_WHITE);
  oled.display();
  delay(2000);
  oled.clearDisplay();
  oled.display();
}

bool performOLEDStep(const String &stepId) {
  if (!oledAvailable) {
    return false;
  }

  if (stepId == "welcome") {
    oledStepWelcome();
  } else if (stepId == "big_text") {
    oledStepBigText();
  } else if (stepId == "text_sizes") {
    oledStepTextSizes();
  } else if (stepId == "shapes") {
    oledStepShapes();
  } else if (stepId == "horizontal_lines") {
    oledStepHorizontalLines();
  } else if (stepId == "diagonals") {
    oledStepDiagonals();
  } else if (stepId == "moving_square") {
    oledStepMovingSquare();
  } else if (stepId == "progress_bar") {
    oledStepProgressBar();
  } else if (stepId == "scroll_text") {
    oledStepScrollText();
  } else if (stepId == "final_message") {
    oledStepFinalMessage();
  } else {
    return false;
  }

  return true;
}

String getOLEDStepLabel(const String &stepId) {
  if (stepId == "welcome") return String(T().oled_step_welcome);
  if (stepId == "big_text") return String(T().oled_step_big_text);
  if (stepId == "text_sizes") return String(T().oled_step_text_sizes);
  if (stepId == "shapes") return String(T().oled_step_shapes);
  if (stepId == "horizontal_lines") return String(T().oled_step_horizontal_lines);
  if (stepId == "diagonals") return String(T().oled_step_diagonals);
  if (stepId == "moving_square") return String(T().oled_step_moving_square);
  if (stepId == "progress_bar") return String(T().oled_step_progress_bar);
  if (stepId == "scroll_text") return String(T().oled_step_scroll_text);
  if (stepId == "final_message") return String(T().oled_step_final_message);
  return stepId;
}

void testOLED() {
  if (!oledAvailable || oledTested) return;

  Serial.println("\r\n=== TEST OLED ===");

  oledStepWelcome();
  oledStepBigText();
  oledStepTextSizes();
  oledStepShapes();
  oledStepHorizontalLines();
  oledStepDiagonals();
  oledStepMovingSquare();
  oledStepProgressBar();
  oledStepScrollText();
  oledStepFinalMessage();

  oledTested = true;
  oledTestResult = String(T().test) + " " + String(T().ok) + " - 128x64";
  Serial.println("OLED: Tests complets OK\r\n");
}

void resetOLEDTest() {
  oledTested = false;
  if (oledAvailable) {
    oled.clearDisplay();
    oled.display();
  }
}

void oledShowMessage(String message) {
  if (!oledAvailable) return;
  applyOLEDOrientation();
  oled.clearDisplay();
  oled.setTextSize(1);
  oled.setTextColor(SSD1306_WHITE);
  oled.setCursor(0, 0);
  oled.println(message);
  oled.display();
}

// ========== TEST ADC ==========
void testADC() {
  Serial.println("\r\n=== TEST ADC ===");
  adcReadings.clear();
  
  #ifdef CONFIG_IDF_TARGET_ESP32
    int adcPins[] = {36, 39, 34, 35, 32, 33};
    int numADC = 6;
  #elif defined(CONFIG_IDF_TARGET_ESP32S3)
    int adcPins[] = {1, 2, 3, 4, 5, 6};
    int numADC = 6;
  #elif defined(CONFIG_IDF_TARGET_ESP32C3)
    int adcPins[] = {0, 1, 2, 3, 4};
    int numADC = 5;
  #else
    int adcPins[] = {36, 39};
    int numADC = 2;
  #endif
  
  for(int i = 0; i < numADC; i++) {
    ADCReading reading;
    reading.pin = adcPins[i];
    reading.rawValue = analogRead(adcPins[i]);
    reading.voltage = (reading.rawValue / 4095.0) * 3.3;
    adcReadings.push_back(reading);
    
    Serial.printf("GPIO%d: %d (%.2fV)\r\n", reading.pin, reading.rawValue, reading.voltage);
  }
  
  adcTestResult = String(numADC) + " " + String(T().channels) + " - " + String(T().ok);
  Serial.printf("ADC: %d canaux testes\r\n", numADC);
}

// ========== TEST PWM ==========
void testPWM() {
  Serial.println("\r\n=== TEST PWM ===");
  int testPin = 2;
  
  #ifdef CONFIG_IDF_TARGET_ESP32S3
    testPin = 48;
  #elif defined(CONFIG_IDF_TARGET_ESP32C3)
    testPin = 8;
  #endif
  
  Serial.printf("Test PWM sur GPIO%d\r\n", testPin);
  
  // Nouvelle API pour ESP32 Arduino Core 3.x
  ledcAttach(testPin, 5000, 8); // pin, freq, resolution
  
  for(int duty = 0; duty <= 255; duty += 51) {
    ledcWrite(testPin, duty);
    Serial.printf("PWM duty: %d/255\r\n", duty);
    delay(200);
  }
  
  ledcWrite(testPin, 0);
  ledcDetach(testPin);
  
  pwmTestResult = String(T().test) + " " + String(T().ok) + " - GPIO " + String(testPin);
  Serial.println("PWM: OK");
}

// ========== SCAN SPI ==========
void scanSPI() {
  Serial.println("\r\n=== SCAN SPI ===");
  spiInfo = "";
  
  #ifdef CONFIG_IDF_TARGET_ESP32
    spiInfo = "HSPI (12,13,14,15), VSPI (18,19,23,5)";
    Serial.println("SPI: HSPI + VSPI disponibles");
  #elif defined(CONFIG_IDF_TARGET_ESP32S2)
    spiInfo = "SPI2, SPI3 disponibles";
    Serial.println("SPI: SPI2, SPI3");
  #elif defined(CONFIG_IDF_TARGET_ESP32S3)
    spiInfo = "SPI2, SPI3 disponibles";
    Serial.println("SPI: SPI2, SPI3");
  #elif defined(CONFIG_IDF_TARGET_ESP32C3)
    spiInfo = "SPI2 disponible";
    Serial.println("SPI: SPI2");
  #else
    spiInfo = "Info SPI non disponible";
  #endif
}

// ========== LISTING PARTITIONS ==========
void listPartitions() {
  Serial.println("\r\n=== PARTITIONS FLASH ===");
  partitionsInfo = "";
  
  esp_partition_iterator_t it = esp_partition_find(ESP_PARTITION_TYPE_ANY, ESP_PARTITION_SUBTYPE_ANY, NULL);
  
  int count = 0;
  while (it != NULL) {
    const esp_partition_t *part = esp_partition_get(it);
    
    char line[128];
    sprintf(line, "%s - Type:0x%02x Addr:0x%08x Size:%dKB", 
            part->label, part->type, part->address, part->size / 1024);
    
    if (count > 0) partitionsInfo += "\r\n";
    partitionsInfo += String(line);
    
    Serial.println(line);
    
    it = esp_partition_next(it);
    count++;
  }
  
  esp_partition_iterator_release(it);
  Serial.printf("Total: %d partitions\r\n", count);
}

// --- [NEW FEATURE] TEST LED RGB ---
void testRGBLed() {
  Serial.println("\r\n=== TEST LED RGB ===");

  if (RGB_LED_PIN_R < 0 || RGB_LED_PIN_G < 0 || RGB_LED_PIN_B < 0) {
    rgbLedTestResult = String(T().configuration_invalid);
    rgbLedAvailable = false;
    Serial.println("LED RGB: Configuration invalide");
    return;
  }

  pinMode(RGB_LED_PIN_R, OUTPUT);
  pinMode(RGB_LED_PIN_G, OUTPUT);
  pinMode(RGB_LED_PIN_B, OUTPUT);

  Serial.printf("Test LED RGB - R:%d G:%d B:%d\r\n", RGB_LED_PIN_R, RGB_LED_PIN_G, RGB_LED_PIN_B);

  digitalWrite(RGB_LED_PIN_R, LOW);
  digitalWrite(RGB_LED_PIN_G, LOW);
  digitalWrite(RGB_LED_PIN_B, LOW);
  delay(200);

  digitalWrite(RGB_LED_PIN_R, HIGH);
  delay(300);
  digitalWrite(RGB_LED_PIN_R, LOW);

  digitalWrite(RGB_LED_PIN_G, HIGH);
  delay(300);
  digitalWrite(RGB_LED_PIN_G, LOW);

  digitalWrite(RGB_LED_PIN_B, HIGH);
  delay(300);
  digitalWrite(RGB_LED_PIN_B, LOW);

  rgbLedTestResult = String(T().ok);
  rgbLedAvailable = true;
  Serial.println("LED RGB: OK");
}

void setRGBLedColor(int r, int g, int b) {
  if (RGB_LED_PIN_R >= 0 && RGB_LED_PIN_G >= 0 && RGB_LED_PIN_B >= 0) {
    analogWrite(RGB_LED_PIN_R, r);
    analogWrite(RGB_LED_PIN_G, g);
    analogWrite(RGB_LED_PIN_B, b);
  }
}

// --- [NEW FEATURE] TEST BUZZER ---
void testBuzzer() {
  Serial.println("\r\n=== TEST BUZZER ===");

  if (BUZZER_PIN < 0) {
    buzzerTestResult = String(T().configuration_invalid);
    buzzerAvailable = false;
    Serial.println("Buzzer: Configuration invalide");
    return;
  }

  pinMode(BUZZER_PIN, OUTPUT);
  Serial.printf("Test Buzzer - Pin:%d\r\n", BUZZER_PIN);

  tone(BUZZER_PIN, 1000, 200);
  delay(300);
  tone(BUZZER_PIN, 1500, 200);
  delay(300);
  tone(BUZZER_PIN, 2000, 200);
  delay(300);
  noTone(BUZZER_PIN);

  buzzerTestResult = String(T().ok);
  buzzerAvailable = true;
  Serial.println("Buzzer: OK");
}

void playBuzzerTone(int frequency, int duration) {
  if (BUZZER_PIN >= 0) {
    tone(BUZZER_PIN, frequency, duration);
  }
}

// --- [NEW FEATURE] TEST DHT11 ---
void testDHT11() {
  Serial.println("\r\n=== TEST DHT11 ===");

  if (DHT11_PIN < 0) {
    dht11TestResult = String(T().configuration_invalid);
    dht11Available = false;
    Serial.println("DHT11: Configuration invalide");
    return;
  }

  Serial.printf("Lecture DHT11 - Pin:%d\r\n", DHT11_PIN);

  pinMode(DHT11_PIN, OUTPUT);
  digitalWrite(DHT11_PIN, LOW);
  delay(20);
  digitalWrite(DHT11_PIN, HIGH);
  delayMicroseconds(40);
  pinMode(DHT11_PIN, INPUT_PULLUP);

  uint8_t data[5] = {0};
  uint8_t bits[40] = {0};

  unsigned long timeout = millis();
  while (digitalRead(DHT11_PIN) == HIGH) {
    if (millis() - timeout > 100) {
      dht11TestResult = String(T().error_label);
      dht11Available = false;
      Serial.println("DHT11: Timeout");
      return;
    }
  }

  timeout = millis();
  while (digitalRead(DHT11_PIN) == LOW) {
    if (millis() - timeout > 100) {
      dht11TestResult = String(T().error_label);
      dht11Available = false;
      Serial.println("DHT11: Timeout");
      return;
    }
  }

  timeout = millis();
  while (digitalRead(DHT11_PIN) == HIGH) {
    if (millis() - timeout > 100) {
      dht11TestResult = String(T().error_label);
      dht11Available = false;
      Serial.println("DHT11: Timeout");
      return;
    }
  }

  for (int i = 0; i < 40; i++) {
    timeout = micros();
    while (digitalRead(DHT11_PIN) == LOW) {
      if (micros() - timeout > 100) break;
    }

    unsigned long t = micros();
    timeout = micros();
    while (digitalRead(DHT11_PIN) == HIGH) {
      if (micros() - timeout > 100) break;
    }

    if ((micros() - t) > 40) {
      bits[i] = 1;
    }
  }

  for (int i = 0; i < 5; i++) {
    data[i] = 0;
    for (int j = 0; j < 8; j++) {
      data[i] <<= 1;
      data[i] |= bits[i * 8 + j];
    }
  }

  if (data[4] == ((data[0] + data[1] + data[2] + data[3]) & 0xFF)) {
    dht11Humidity = data[0];
    dht11Temperature = data[2];
    dht11TestResult = String(T().ok);
    dht11Available = true;
    Serial.printf("DHT11: T=%.1f°C H=%.1f%%\r\n", dht11Temperature, dht11Humidity);
  } else {
    dht11TestResult = String(T().error_label);
    dht11Available = false;
    Serial.println("DHT11: Checksum error");
  }
}

// --- [NEW FEATURE] TEST LIGHT SENSOR ---
void testLightSensor() {
  Serial.println("\r\n=== TEST LIGHT SENSOR ===");

  if (LIGHT_SENSOR_PIN < 0) {
    lightSensorTestResult = String(T().configuration_invalid);
    lightSensorAvailable = false;
    Serial.println("Light Sensor: Configuration invalide");
    return;
  }

  pinMode(LIGHT_SENSOR_PIN, INPUT);
  Serial.printf("Lecture Light Sensor - Pin:%d\r\n", LIGHT_SENSOR_PIN);

  int sum = 0;
  for (int i = 0; i < 10; i++) {
    sum += analogRead(LIGHT_SENSOR_PIN);
    delay(10);
  }
  lightSensorValue = sum / 10;

  lightSensorTestResult = String(T().ok);
  lightSensorAvailable = true;
  Serial.printf("Light Sensor: %d\r\n", lightSensorValue);
}

// --- [NEW FEATURE] TEST DISTANCE SENSOR ---
void testDistanceSensor() {
  Serial.println("\r\n=== TEST DISTANCE SENSOR (HC-SR04) ===");

  if (DISTANCE_TRIG_PIN < 0 || DISTANCE_ECHO_PIN < 0) {
    distanceSensorTestResult = String(T().configuration_invalid);
    distanceSensorAvailable = false;
    Serial.println("Distance Sensor: Configuration invalide");
    return;
  }

  pinMode(DISTANCE_TRIG_PIN, OUTPUT);
  pinMode(DISTANCE_ECHO_PIN, INPUT);

  Serial.printf("Distance Sensor - Trig:%d Echo:%d\r\n", DISTANCE_TRIG_PIN, DISTANCE_ECHO_PIN);

  digitalWrite(DISTANCE_TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(DISTANCE_TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(DISTANCE_TRIG_PIN, LOW);

  long duration = pulseIn(DISTANCE_ECHO_PIN, HIGH, 30000);

  if (duration > 0) {
    distanceValue = duration * 0.034 / 2;
    distanceSensorTestResult = String(T().ok);
    distanceSensorAvailable = true;
    Serial.printf("Distance: %.2f cm\r\n", distanceValue);
  } else {
    distanceSensorTestResult = String(T().error_label);
    distanceSensorAvailable = false;
    Serial.println("Distance Sensor: No echo");
  }
}

// --- [NEW FEATURE] TEST MOTION SENSOR ---
void testMotionSensor() {
  Serial.println("\r\n=== TEST MOTION SENSOR (PIR) ===");

  if (MOTION_SENSOR_PIN < 0) {
    motionSensorTestResult = String(T().configuration_invalid);
    motionSensorAvailable = false;
    Serial.println("Motion Sensor: Configuration invalide");
    return;
  }

  pinMode(MOTION_SENSOR_PIN, INPUT);
  Serial.printf("Motion Sensor - Pin:%d\r\n", MOTION_SENSOR_PIN);

  delay(100);
  motionDetected = digitalRead(MOTION_SENSOR_PIN);

  motionSensorTestResult = String(T().ok);
  motionSensorAvailable = true;
  Serial.printf("Motion: %s\r\n", motionDetected ? "Detected" : "None");
}

// ========== TEST STRESS MÉMOIRE ==========
void memoryStressTest() {
  Serial.println("\r\n=== STRESS TEST MEMOIRE ===");
  
  const int allocSize = 1024;
  int maxAllocs = 0;
  std::vector<void*> allocations;
  
  Serial.println("Allocation progressive...");
  while(true) {
    void* ptr = malloc(allocSize);
    if (!ptr) break;
    allocations.push_back(ptr);
    maxAllocs++;
    
    if (maxAllocs % 100 == 0) {
      Serial.printf("Alloue: %d KB\r\n", maxAllocs);
    }
  }
  
  Serial.printf("Max allocations: %d blocs (%d KB)\r\n", maxAllocs, maxAllocs);
  
  Serial.println("Liberation memoire...");
  for(auto ptr : allocations) {
    free(ptr);
  }
  
  stressTestResult = String(T().max_alloc) + ": " + String(maxAllocs) + " KB";
  Serial.println("Stress test: OK");
}

// ========== COLLECTE DONNÉES ==========
void collectDiagnosticInfo() {
  esp_chip_info_t chip_info;
  esp_chip_info(&chip_info);
  
  diagnosticData.chipModel = detectChipModel();
  diagnosticData.chipRevision = String(chip_info.revision);
  diagnosticData.cpuCores = chip_info.cores;
  diagnosticData.cpuFreqMHz = ESP.getCpuFreqMHz();
  
  uint32_t flash_size;
  esp_flash_get_size(NULL, &flash_size);
  diagnosticData.flashSize = flash_size;
  diagnosticData.psramSize = ESP.getPsramSize();
  
  uint8_t mac[6];
  esp_read_mac(mac, ESP_MAC_WIFI_STA);
  char macStr[18];
  sprintf(macStr, "%02X:%02X:%02X:%02X:%02X:%02X", 
          mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
  diagnosticData.macAddress = String(macStr);
  
  diagnosticData.heapSize = ESP.getHeapSize();
  diagnosticData.freeHeap = ESP.getFreeHeap();
  diagnosticData.minFreeHeap = ESP.getMinFreeHeap();
  diagnosticData.maxAllocHeap = ESP.getMaxAllocHeap();
  
  diagnosticData.hasWiFi = (chip_info.features & CHIP_FEATURE_WIFI_BGN);
  diagnosticData.hasBT = (chip_info.features & CHIP_FEATURE_BT);
  bool featureBLE = (chip_info.features & CHIP_FEATURE_BLE);
  bool runtimeBLE = featureBLE || TARGET_BLE_SUPPORTED;
#ifdef CHIP_ESP32S2
  if (chip_info.model == CHIP_ESP32S2) {
    runtimeBLE = false;
  }
#endif
#ifdef CHIP_ESP32S3
  if (chip_info.model == CHIP_ESP32S3) {
    runtimeBLE = true;
  }
#endif
#ifdef CHIP_ESP32C3
  if (chip_info.model == CHIP_ESP32C3) {
    runtimeBLE = true;
  }
#endif
#ifdef CHIP_ESP32C6
  if (chip_info.model == CHIP_ESP32C6) {
    runtimeBLE = true;
  }
#endif
#ifdef CHIP_ESP32H2
  if (chip_info.model == CHIP_ESP32H2) {
    runtimeBLE = true;
  }
#endif
  diagnosticData.hasBLE = runtimeBLE;
  bluetoothCapable = runtimeBLE && BLE_STACK_SUPPORTED;
  
  if (WiFi.status() == WL_CONNECTED) {
    diagnosticData.wifiSSID = WiFi.SSID();
    diagnosticData.wifiRSSI = WiFi.RSSI();
    diagnosticData.ipAddress = WiFi.localIP().toString();
#if DIAGNOSTIC_HAS_MDNS
    diagnosticData.mdnsAvailable = mdnsServiceActive;
#else
    diagnosticData.mdnsAvailable = false;
#endif
  } else {
    diagnosticData.wifiSSID = "";
    diagnosticData.wifiRSSI = -127;
    diagnosticData.ipAddress = "";
    diagnosticData.mdnsAvailable = false;
  }
  
  diagnosticData.gpioList = getGPIOList();
  diagnosticData.totalGPIO = countGPIO();
  
  diagnosticData.sdkVersion = ESP.getSdkVersion();
  diagnosticData.idfVersion = esp_get_idf_version();
  diagnosticData.uptime = millis();
  
  #ifdef SOC_TEMP_SENSOR_SUPPORTED
    diagnosticData.temperature = temperatureRead();
  #else
    diagnosticData.temperature = -999;
  #endif
  
  diagnosticData.neopixelTested = neopixelTested;
  diagnosticData.neopixelAvailable = neopixelAvailable;
  diagnosticData.neopixelResult = neopixelTestResult;
  
  diagnosticData.oledTested = oledTested;
  diagnosticData.oledAvailable = oledAvailable;
  diagnosticData.oledResult = oledTestResult;

  syncBluetoothDiagnostics();

  heapHistory[historyIndex] = (float)diagnosticData.freeHeap / 1024.0;
  if (diagnosticData.temperature != -999) {
    tempHistory[historyIndex] = diagnosticData.temperature;
  }
  historyIndex = (historyIndex + 1) % HISTORY_SIZE;
}

// ========== HANDLERS API ==========
void handleTestGPIO() {
  testAllGPIOs();
  String json;
  json.reserve(gpioResults.size() * 40 + 20);  // Estimate size to avoid reallocations
  json = "{\"results\":[";
  for (size_t i = 0; i < gpioResults.size(); i++) {
    if (i > 0) json += ",";
    json += "{\"pin\":" + String(gpioResults[i].pin) + ",\"working\":" + String(gpioResults[i].working ? "true" : "false") + "}";
  }
  json += "]}";
  server.send(200, "application/json", json);
}

void handleWiFiScan() {
  scanWiFiNetworks();
  String json;
  json.reserve(wifiNetworks.size() * 150 + 20);  // Estimate size to avoid reallocations
  json = "{\"networks\":[";
  for (size_t i = 0; i < wifiNetworks.size(); i++) {
    if (i > 0) json += ",";
    json += "{\"ssid\":\"" + wifiNetworks[i].ssid + "\",\"rssi\":" + String(wifiNetworks[i].rssi) +
            ",\"channel\":" + String(wifiNetworks[i].channel) + ",\"encryption\":\"" + wifiNetworks[i].encryption +
            "\",\"bssid\":\"" + wifiNetworks[i].bssid + "\",\"band\":\"" + wifiNetworks[i].band +
            "\",\"bandwidth\":\"" + wifiNetworks[i].bandwidth + "\",\"phy\":\"" + wifiNetworks[i].phyModes +
            "\",\"freq\":" + String(wifiNetworks[i].freqMHz) + "}";
  }
  json += "]}";
  server.send(200, "application/json", json);
}

void handleI2CScan() {
  scanI2C();
  sendJsonResponse(200, {
    jsonNumberField("count", diagnosticData.i2cCount),
    jsonStringField("devices", diagnosticData.i2cDevices)
  });
}

void handleBuiltinLEDConfig() {
  if (server.hasArg("gpio")) {
    int newGPIO = server.arg("gpio").toInt();
    if (newGPIO >= 0 && newGPIO <= 48) {
      BUILTIN_LED_PIN = newGPIO;
      resetBuiltinLEDTest();
      String message = String(T().config) + " " + String(T().gpio) + " " + String(BUILTIN_LED_PIN);
      sendOperationSuccess(message);
      return;
    }
  }
  sendOperationError(400, T().gpio_invalid.str());
}

void handleBuiltinLEDTest() {
  resetBuiltinLEDTest();
  testBuiltinLED();
  sendActionResponse(200, builtinLedAvailable, builtinLedTestResult, {
    jsonStringField("result", builtinLedTestResult)
  });
}

void handleBuiltinLEDControl() {
  if (!server.hasArg("action")) {
    sendOperationError(400, T().configuration_invalid.str());
    return;
  }

  String action = server.arg("action");
  if (BUILTIN_LED_PIN == -1) {
    sendOperationError(400, T().gpio_invalid.str());
    return;
  }

  pinMode(BUILTIN_LED_PIN, OUTPUT);
  String message = "";
  
  if (action == "blink") {
    for(int i = 0; i < 5; i++) {
      digitalWrite(BUILTIN_LED_PIN, HIGH);
      delay(200);
      digitalWrite(BUILTIN_LED_PIN, LOW);
      delay(200);
    }
    message = String(T().blink) + " " + String(T().ok);
  } else if (action == "fade") {
    for(int i = 0; i <= 255; i += 5) {
      analogWrite(BUILTIN_LED_PIN, i);
      delay(10);
    }
    for(int i = 255; i >= 0; i -= 5) {
      analogWrite(BUILTIN_LED_PIN, i);
      delay(10);
    }
    digitalWrite(BUILTIN_LED_PIN, LOW);
    message = String(T().fade) + " " + String(T().ok);
  } else if (action == "off") {
    digitalWrite(BUILTIN_LED_PIN, LOW);
    builtinLedTested = false;
    message = String(T().off);
  } else {
    sendOperationError(400, T().configuration_invalid.str());
    return;
  }

  sendOperationSuccess(message);
}

void handleNeoPixelConfig() {
  if (server.hasArg("gpio") && server.hasArg("count")) {
    int newGPIO = server.arg("gpio").toInt();
    int newCount = server.arg("count").toInt();
    
    if (newGPIO >= 0 && newGPIO <= 48 && newCount > 0 && newCount <= 100) {
      LED_PIN = newGPIO;
      LED_COUNT = newCount;
      if (strip) delete strip;
      strip = new Adafruit_NeoPixel(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);
      resetNeoPixelTest();
      String message = String(T().config) + " " + String(T().gpio) + " " + String(LED_PIN);
      sendOperationSuccess(message);
      return;
    }
  }
  sendOperationError(400, T().configuration_invalid.str());
}

void handleNeoPixelTest() {
  resetNeoPixelTest();
  testNeoPixel();
  sendActionResponse(200, neopixelAvailable, neopixelTestResult, {
    jsonStringField("result", neopixelTestResult)
  });
}

void handleNeoPixelPattern() {
  if (!server.hasArg("pattern")) {
    sendOperationError(400, T().configuration_invalid.str());
    return;
  }

  String pattern = server.arg("pattern");
  if (!strip) {
    String message = String(T().neopixel) + " - " + String(T().not_detected);
    sendOperationError(400, message);
    return;
  }

  String message = "";
  if (pattern == "rainbow") {
    neopixelRainbow();
    message = String(T().rainbow) + " " + String(T().ok);
  } else if (pattern == "blink") {
    neopixelBlink(strip->Color(255, 0, 0), 5);
    message = String(T().blink) + " " + String(T().ok);
  } else if (pattern == "fade") {
    neopixelFade(strip->Color(0, 0, 255));
    message = String(T().fade) + " " + String(T().ok);
  } else if (pattern == "off") {
    strip->clear();
    strip->show();
    neopixelTested = false;
    message = String(T().off);
  } else {
    sendOperationError(400, T().configuration_invalid.str());
    return;
  }

  sendOperationSuccess(message);
}

void handleNeoPixelColor() {
  if (!server.hasArg("r") || !server.hasArg("g") || !server.hasArg("b") || !strip) {
    sendOperationError(400, T().configuration_invalid.str());
    return;
  }

  int r = server.arg("r").toInt();
  int g = server.arg("g").toInt();
  int b = server.arg("b").toInt();
  
  strip->fill(strip->Color(r, g, b));
  strip->show();
  neopixelTested = false;

  String message = "RGB(" + String(r) + "," + String(g) + "," + String(b) + ")";
  sendOperationSuccess(message);
}

void handleOLEDConfig() {
  if (server.hasArg("sda") && server.hasArg("scl") && server.hasArg("rotation")) {
    int newSDA = server.arg("sda").toInt();
    int newSCL = server.arg("scl").toInt();
    int newRotation = server.arg("rotation").toInt();

    if (newSDA >= 0 && newSDA <= 48 && newSCL >= 0 && newSCL <= 48 && newRotation >= 0 && newRotation <= 3) {
      bool pinsChanged = (I2C_SDA != newSDA) || (I2C_SCL != newSCL);
      bool rotationChanged = (oledRotation != static_cast<uint8_t>(newRotation));

      I2C_SDA = newSDA;
      I2C_SCL = newSCL;
      oledRotation = static_cast<uint8_t>(newRotation);

      if (pinsChanged || rotationChanged) {
        resetOLEDTest();
        Wire.end();
        detectOLED();
      } else if (oledAvailable) {
        applyOLEDOrientation();
      }

      String message = "I2C reconfigure: SDA:" + String(I2C_SDA) + " SCL:" + String(I2C_SCL) + " Rot:" + String(oledRotation);
      sendOperationSuccess(message, {
        jsonNumberField("sda", I2C_SDA),
        jsonNumberField("scl", I2C_SCL),
        jsonNumberField("rotation", oledRotation)
      });
      return;
    }
  }
  sendOperationError(400, T().configuration_invalid.str());
}

void handleOLEDTest() {
  resetOLEDTest();
  testOLED();
  sendActionResponse(200, oledAvailable, oledTestResult, {
    jsonStringField("result", oledTestResult)
  });
}

void handleOLEDStep() {
  if (!server.hasArg("step")) {
    sendOperationError(400, T().oled_step_unknown.str());
    return;
  }

  String stepId = server.arg("step");

  if (!oledAvailable) {
    sendActionResponse(200, false, T().oled_step_unavailable.str());
    return;
  }

  bool ok = performOLEDStep(stepId);
  if (!ok) {
    sendOperationError(400, T().oled_step_unknown.str());
    return;
  }

  String label = getOLEDStepLabel(stepId);
  String message = String(T().oled_step_executed_prefix) + " " + label;
  sendOperationSuccess(message);
}

void handleOLEDMessage() {
  if (!server.hasArg("message")) {
    sendOperationError(400, T().configuration_invalid.str());
    return;
  }

  String message = server.arg("message");
  oledShowMessage(message);
  // --- [TRANSLATION FIX] Use translation key instead of hardcoded string ---
  sendOperationSuccess(T().message_displayed.str());
}

void handleADCTest() {
  testADC();
  String json;
  json.reserve(adcReadings.size() * 60 + 50);  // Estimate size to avoid reallocations
  json = "{\"readings\":[";
  for (size_t i = 0; i < adcReadings.size(); i++) {
    if (i > 0) json += ",";
    json += "{\"pin\":" + String(adcReadings[i].pin) + ",\"raw\":" + String(adcReadings[i].rawValue) +
            ",\"voltage\":" + String(adcReadings[i].voltage, 2) + "}";
  }
  json += "],\"result\":\"" + adcTestResult + "\"}";
  server.send(200, "application/json", json);
}

void handlePWMTest() {
  testPWM();
  sendJsonResponse(200, { jsonStringField("result", pwmTestResult) });
}

void handleSPIScan() {
  scanSPI();
  sendJsonResponse(200, { jsonStringField("info", spiInfo) });
}

void handlePartitionsList() {
  listPartitions();
  sendJsonResponse(200, { jsonStringField("partitions", partitionsInfo) });
}

void handleStressTest() {
  memoryStressTest();
  sendJsonResponse(200, { jsonStringField("result", stressTestResult) });
}

// --- [NEW FEATURE] Handlers API pour les nouveaux capteurs ---
void handleRGBLedConfig() {
  if (server.hasArg("r") && server.hasArg("g") && server.hasArg("b")) {
    RGB_LED_PIN_R = server.arg("r").toInt();
    RGB_LED_PIN_G = server.arg("g").toInt();
    RGB_LED_PIN_B = server.arg("b").toInt();
    sendActionResponse(200, true, String(T().ok));
  } else {
    sendActionResponse(400, false, String(T().configuration_invalid));
  }
}

void handleRGBLedTest() {
  testRGBLed();
  sendJsonResponse(200, {
    jsonBoolField("success", rgbLedAvailable),
    jsonStringField("result", rgbLedTestResult)
  });
}

void handleRGBLedColor() {
  if (server.hasArg("r") && server.hasArg("g") && server.hasArg("b")) {
    int r = server.arg("r").toInt();
    int g = server.arg("g").toInt();
    int b = server.arg("b").toInt();
    setRGBLedColor(r, g, b);
    sendActionResponse(200, true, "RGB(" + String(r) + "," + String(g) + "," + String(b) + ")");
  } else {
    sendActionResponse(400, false, String(T().configuration_invalid));
  }
}

void handleBuzzerConfig() {
  if (server.hasArg("pin")) {
    BUZZER_PIN = server.arg("pin").toInt();
    sendActionResponse(200, true, String(T().ok));
  } else {
    sendActionResponse(400, false, String(T().configuration_invalid));
  }
}

void handleBuzzerTest() {
  testBuzzer();
  sendJsonResponse(200, {
    jsonBoolField("success", buzzerAvailable),
    jsonStringField("result", buzzerTestResult)
  });
}

void handleBuzzerTone() {
  if (server.hasArg("freq") && server.hasArg("duration")) {
    int freq = server.arg("freq").toInt();
    int duration = server.arg("duration").toInt();
    playBuzzerTone(freq, duration);
    sendActionResponse(200, true, String(freq) + "Hz");
  } else {
    sendActionResponse(400, false, String(T().configuration_invalid));
  }
}

void handleDHT11Config() {
  if (server.hasArg("pin")) {
    DHT11_PIN = server.arg("pin").toInt();
    sendActionResponse(200, true, String(T().ok));
  } else {
    sendActionResponse(400, false, String(T().configuration_invalid));
  }
}

void handleDHT11Test() {
  testDHT11();
  sendJsonResponse(200, {
    jsonBoolField("success", dht11Available),
    jsonStringField("result", dht11TestResult),
    jsonFloatField("temperature", dht11Temperature, 1),
    jsonFloatField("humidity", dht11Humidity, 1)
  });
}

void handleLightSensorConfig() {
  if (server.hasArg("pin")) {
    LIGHT_SENSOR_PIN = server.arg("pin").toInt();
    sendActionResponse(200, true, String(T().ok));
  } else {
    sendActionResponse(400, false, String(T().configuration_invalid));
  }
}

void handleLightSensorTest() {
  testLightSensor();
  sendJsonResponse(200, {
    jsonBoolField("success", lightSensorAvailable),
    jsonStringField("result", lightSensorTestResult),
    jsonNumberField("value", lightSensorValue)
  });
}

void handleDistanceSensorConfig() {
  if (server.hasArg("trig") && server.hasArg("echo")) {
    DISTANCE_TRIG_PIN = server.arg("trig").toInt();
    DISTANCE_ECHO_PIN = server.arg("echo").toInt();
    sendActionResponse(200, true, String(T().ok));
  } else {
    sendActionResponse(400, false, String(T().configuration_invalid));
  }
}

void handleDistanceSensorTest() {
  testDistanceSensor();
  sendJsonResponse(200, {
    jsonBoolField("success", distanceSensorAvailable),
    jsonStringField("result", distanceSensorTestResult),
    jsonFloatField("distance", distanceValue, 2)
  });
}

void handleMotionSensorConfig() {
  if (server.hasArg("pin")) {
    MOTION_SENSOR_PIN = server.arg("pin").toInt();
    sendActionResponse(200, true, String(T().ok));
  } else {
    sendActionResponse(400, false, String(T().configuration_invalid));
  }
}

void handleMotionSensorTest() {
  testMotionSensor();
  sendJsonResponse(200, {
    jsonBoolField("success", motionSensorAvailable),
    jsonStringField("result", motionSensorTestResult),
    jsonBoolField("motion", motionDetected)
  });
}

void handleBenchmark() {
  unsigned long cpuTime = benchmarkCPU();
  unsigned long memTime = benchmarkMemory();

  diagnosticData.cpuBenchmark = cpuTime;
  diagnosticData.memBenchmark = memTime;

  double cpuPerf = 100000.0 / static_cast<double>(cpuTime);
  double memSpeed = (10000.0 * sizeof(int) * 2.0) / static_cast<double>(memTime);
  sendJsonResponse(200, {
    jsonNumberField("cpu", cpuTime),
    jsonNumberField("memory", memTime),
    jsonFloatField("cpuPerf", cpuPerf, 2),
    jsonFloatField("memSpeed", memSpeed, 2)
  });
}

void handleStatus() {
  sendJsonResponse(200, {
    jsonBoolField("connected", WiFi.status() == WL_CONNECTED),
    jsonBoolField("bluetooth", bluetoothEnabled),
    jsonStringField("uptime", String(millis()))
  });
}

void handleSystemInfo() {
  collectDiagnosticInfo();
  String json;
  json.reserve(600);
  json = "{";
  json += "\"chipModel\":\"" + diagnosticData.chipModel + "\",";
  json += "\"chipRevision\":\"" + diagnosticData.chipRevision + "\",";
  json += "\"cpuCores\":" + String(diagnosticData.cpuCores) + ",";
  json += "\"cpuFreq\":" + String(diagnosticData.cpuFreqMHz) + ",";
  json += "\"macAddress\":\"" + diagnosticData.macAddress + "\",";
  json += "\"ipAddress\":\"" + diagnosticData.ipAddress + "\",";
  json += "\"mdnsReady\":" + String(diagnosticData.mdnsAvailable ? "true" : "false") + ",";
  json += "\"uptime\":" + String(diagnosticData.uptime);
  if (diagnosticData.temperature != -999) {
    json += ",\"temperature\":" + String(diagnosticData.temperature, 1);
  }
  json += "}";
  server.send(200, "application/json", json);
}

void handleMemory() {
  collectDetailedMemory();
  String json;
  json.reserve(500);
  json = "{";
  json += "\"heap\":{\"total\":" + String(diagnosticData.heapSize) +
          ",\"free\":" + String(diagnosticData.freeHeap) +
          ",\"used\":" + String(diagnosticData.heapSize - diagnosticData.freeHeap) + "},";
  json += "\"psram\":{\"total\":" + String(detailedMemory.psramTotal) +
          ",\"free\":" + String(detailedMemory.psramFree) +
          ",\"used\":" + String(detailedMemory.psramUsed) + "},";
  json += "\"fragmentation\":" + String(detailedMemory.fragmentationPercent, 1);
  json += "}";
  server.send(200, "application/json", json);
}

void handleWiFiInfo() {
  collectDiagnosticInfo();
  String json;
  json.reserve(400);
  json = "{";
  json += "\"connected\":" + String(WiFi.status() == WL_CONNECTED ? "true" : "false") + ",";
  json += "\"ssid\":\"" + diagnosticData.wifiSSID + "\",";
  json += "\"rssi\":" + String(diagnosticData.wifiRSSI) + ",";
  json += "\"quality_key\":\"" + String(getWiFiSignalQualityKey()) + "\",";
  json += "\"quality\":\"" + getWiFiSignalQuality() + "\",";
  json += "\"ip\":\"" + diagnosticData.ipAddress + "\",";
  json += "\"gateway\":\"" + WiFi.gatewayIP().toString() + "\",";
  json += "\"dns\":\"" + WiFi.dnsIP().toString() + "\"";
  json += "}";
  server.send(200, "application/json", json);
}

void handlePeripherals() {
  scanI2C();
  String json;
  json.reserve(300);
  json = "{";
  json += "\"i2c\":{\"count\":" + String(diagnosticData.i2cCount) +
          ",\"devices\":\"" + diagnosticData.i2cDevices + "\"},";
  json += "\"gpio\":{\"total\":" + String(diagnosticData.totalGPIO) +
          ",\"list\":\"" + diagnosticData.gpioList + "\"}";
  json += "}";
  server.send(200, "application/json", json);
}

void handleLedsInfo() {
  String json;
  json.reserve(400);
  json = "{";
  json += "\"builtin\":{\"pin\":" + String(BUILTIN_LED_PIN) +
          ",\"status\":\"" + builtinLedTestResult + "\"},";
  json += "\"neopixel\":{\"pin\":" + String(LED_PIN) +
          ",\"count\":" + String(LED_COUNT) +
          ",\"status\":\"" + neopixelTestResult + "\"}";
  json += "}";
  server.send(200, "application/json", json);
}

void handleScreensInfo() {
  String json;
  json.reserve(300);
  json = "{";
  json += "\"oled\":{\"available\":" + String(oledAvailable ? "true" : "false") +
          ",\"status\":\"" + oledTestResult + "\",";
  json += "\"pins\":{\"sda\":" + String(I2C_SDA) + ",\"scl\":" + String(I2C_SCL) + "},";
  json += "\"rotation\":" + String(oledRotation) + "}";
  json += "}";
  server.send(200, "application/json", json);
}

void handleOverview() {
  collectDiagnosticInfo();
  collectDetailedMemory();
  scanI2C();

  String json;
  json.reserve(2500);  // Reserve memory for the complete overview response
  json = "{";

  // Chip info
  json += "\"chip\":{";
  json += "\"model\":\"" + diagnosticData.chipModel + "\",";
  json += "\"revision\":\"" + diagnosticData.chipRevision + "\",";
  json += "\"cores\":" + String(diagnosticData.cpuCores) + ",";
  json += "\"freq\":" + String(diagnosticData.cpuFreqMHz) + ",";
  json += "\"mac\":\"" + diagnosticData.macAddress + "\",";
  json += "\"uptime\":" + String(diagnosticData.uptime);
  if (diagnosticData.temperature != -999) {
    json += ",\"temperature\":" + String(diagnosticData.temperature, 1);
  } else {
    json += ",\"temperature\":-999";
  }
  json += "},";

  // Memory info
  json += "\"memory\":{";
  json += "\"flash\":{\"real\":" + String(detailedMemory.flashSizeReal) +
          ",\"type\":\"" + getFlashType() + "\",\"speed\":\"" + getFlashSpeed() + "\"},";
  json += "\"sram\":{\"total\":" + String(detailedMemory.sramTotal) +
          ",\"free\":" + String(detailedMemory.sramFree) +
          ",\"used\":" + String(detailedMemory.sramUsed) + "},";
  json += "\"psram\":{\"total\":" + String(detailedMemory.psramTotal) +
          ",\"free\":" + String(detailedMemory.psramFree) +
          ",\"used\":" + String(detailedMemory.psramUsed) + "},";
  json += "\"fragmentation\":" + String(detailedMemory.fragmentationPercent, 1);
  json += "},";

  // WiFi info - Use translation key instead of translated string
  json += "\"wifi\":{";
  json += "\"ssid\":\"" + diagnosticData.wifiSSID + "\",";
  json += "\"rssi\":" + String(diagnosticData.wifiRSSI) + ",";
  json += "\"quality_key\":\"" + String(getWiFiSignalQualityKey()) + "\",";  // Return key, not translated string
  json += "\"quality\":\"" + getWiFiSignalQuality() + "\",";  // Keep for backward compatibility
  json += "\"ip\":\"" + diagnosticData.ipAddress + "\"";
  json += "},";

  // GPIO info
  json += "\"gpio\":{";
  json += "\"total\":" + String(diagnosticData.totalGPIO) + ",";
  json += "\"i2c_count\":" + String(diagnosticData.i2cCount) + ",";
  json += "\"i2c_devices\":\"" + diagnosticData.i2cDevices + "\"";
  json += "}";

  json += "}";
  server.send(200, "application/json", json);
}

void handleMemoryDetails() {
  collectDetailedMemory();

  String json;
  json.reserve(450);  // Reserve memory to avoid reallocations
  json = "{\"flash\":{\"real\":" + String(detailedMemory.flashSizeReal) + ",\"chip\":" + String(detailedMemory.flashSizeChip) + "},";
  json += "\"psram\":{\"available\":" + String(detailedMemory.psramAvailable ? "true" : "false") +
          ",\"configured\":" + String(detailedMemory.psramConfigured ? "true" : "false") +
          ",\"supported\":" + String(detailedMemory.psramBoardSupported ? "true" : "false") +
          ",\"type\":\"" + String(detailedMemory.psramType ? detailedMemory.psramType : T().unknown.str()) + "\"" +
          ",\"total\":" + String(detailedMemory.psramTotal) + ",\"free\":" + String(detailedMemory.psramFree) + "},";
  json += "\"sram\":{\"total\":" + String(detailedMemory.sramTotal) + ",\"free\":" + String(detailedMemory.sramFree) + "},";
  json += "\"fragmentation\":" + String(detailedMemory.fragmentationPercent, 1) + ",\"status\":\"" + detailedMemory.memoryStatus + "\"}";

  server.send(200, "application/json", json);
}

// ========== EXPORTS ==========
void handleExportTXT() {
  collectDiagnosticInfo();
  collectDetailedMemory();

  String txt;
  txt.reserve(4500);  // Reserve memory to avoid reallocations during export
  txt = "========================================\r\n";
  txt += String(T().title) + " " + String(T().version) + String(DIAGNOSTIC_VERSION) + "\r\n";
  txt += "========================================\r\n\r\n";
  
  txt += "=== CHIP ===\r\n";
  txt += String(T().model) + ": " + diagnosticData.chipModel + " " + String(T().revision) + diagnosticData.chipRevision + "\r\n";
  txt += "CPU: " + String(diagnosticData.cpuCores) + " " + String(T().cores) + " @ " + String(diagnosticData.cpuFreqMHz) + " MHz\r\n";
  txt += "MAC WiFi: " + diagnosticData.macAddress + "\r\n";
  txt += "SDK: " + diagnosticData.sdkVersion + "\r\n";
  txt += "ESP-IDF: " + diagnosticData.idfVersion + "\r\n";
  if (diagnosticData.temperature != -999) {
    txt += String(T().cpu_temp) + ": " + String(diagnosticData.temperature, 1) + " °C\r\n";
  }
  txt += "\r\n";
  
  txt += "=== " + String(T().memory_details) + " ===\r\n";
  txt += "Flash (" + String(T().board) + "): " + String(detailedMemory.flashSizeReal / 1048576.0, 2) + " MB\r\n";
  txt += "Flash (IDE): " + String(detailedMemory.flashSizeChip / 1048576.0, 2) + " MB\r\n";
  txt += String(T().flash_type) + ": " + getFlashType() + " @ " + getFlashSpeed() + "\r\n";
  txt += "PSRAM: " + String(detailedMemory.psramTotal / 1048576.0, 2) + " MB";
  if (detailedMemory.psramAvailable) {
    txt += " (" + String(T().free) + ": " + String(detailedMemory.psramFree / 1048576.0, 2) + " MB)\r\n";
  } else if (detailedMemory.psramBoardSupported) {
    String psramHint = String(T().enable_psram_hint);
    psramHint.replace("%TYPE%", detailedMemory.psramType ? detailedMemory.psramType : "PSRAM");
    txt += " (" + String(T().supported_not_enabled) + " - " + psramHint + ")\r\n";
  } else {
    txt += " (" + String(T().not_detected) + ")\r\n";
  }
  txt += "SRAM: " + String(detailedMemory.sramTotal / 1024.0, 2) + " KB";
  txt += " (" + String(T().free) + ": " + String(detailedMemory.sramFree / 1024.0, 2) + " KB)\r\n";
  txt += String(T().memory_fragmentation) + ": " + String(detailedMemory.fragmentationPercent, 1) + "%\r\n";
  txt += String(T().memory_status) + ": " + detailedMemory.memoryStatus + "\r\n";
  txt += "\r\n";
  
  txt += "=== WIFI ===\r\n";
  txt += "SSID: " + diagnosticData.wifiSSID + "\r\n";
  txt += "RSSI: " + String(diagnosticData.wifiRSSI) + " dBm (" + getWiFiSignalQuality() + ")\r\n";
  txt += "IP: " + diagnosticData.ipAddress + "\r\n";
  txt += "Lien constant: " + getStableAccessURL() + " (" + String(diagnosticData.mdnsAvailable ? "actif" : "en attente") + ")\r\n";
  txt += String(T().subnet_mask) + ": " + WiFi.subnetMask().toString() + "\r\n";
  txt += String(T().gateway) + ": " + WiFi.gatewayIP().toString() + "\r\n";
  txt += "DNS: " + WiFi.dnsIP().toString() + "\r\n";
  txt += "\r\n";

  txt += "=== BLUETOOTH ===\r\n";
  if (!bluetoothCapable) {
    txt += String(T().bluetooth_not_supported) + "\r\n";
  } else {
    txt += String(T().bluetooth_status) + ": " + getBluetoothStateLabel() + "\r\n";
    txt += String(T().bluetooth_name) + ": " + bluetoothDeviceName + "\r\n";
    txt += String(T().bluetooth_mac) + ": " + diagnosticData.bluetoothAddress + "\r\n";
    txt += String(T().bluetooth_advertising) + ": " + String((bluetoothCapable && bluetoothAdvertising) ? "ON" : "OFF") + "\r\n";
  }
  txt += "\r\n";

  txt += "=== GPIO ===\r\n";
  txt += String(T().total_gpio) + ": " + String(diagnosticData.totalGPIO) + " " + String(T().pins) + "\r\n";
  txt += String(T().gpio_list) + ": " + diagnosticData.gpioList + "\r\n";
  txt += "\r\n";
  
  txt += "=== " + String(T().i2c_peripherals) + " ===\r\n";
  txt += String(T().device_count) + ": " + String(diagnosticData.i2cCount) + " - " + diagnosticData.i2cDevices + "\r\n";
  txt += "SPI: " + spiInfo + "\r\n";
  txt += "\r\n";
  
  txt += "=== " + String(T().test) + " ===\r\n";
  txt += String(T().builtin_led) + ": " + builtinLedTestResult + "\r\n";
  txt += String(T().neopixel) + ": " + neopixelTestResult + "\r\n";
  txt += "OLED: " + oledTestResult + "\r\n";
  txt += "ADC: " + adcTestResult + "\r\n";
  txt += "PWM: " + pwmTestResult + "\r\n";
  txt += "\r\n";
  
  txt += "=== " + String(T().performance_bench) + " ===\r\n";
  if (diagnosticData.cpuBenchmark > 0) {
    txt += "CPU: " + String(diagnosticData.cpuBenchmark) + " us (" + String(100000.0 / diagnosticData.cpuBenchmark, 2) + " MFLOPS)\r\n";
    txt += String(T().memory_benchmark) + ": " + String(diagnosticData.memBenchmark) + " us\r\n";
  } else {
    txt += String(T().not_tested) + "\r\n";
  }
  txt += "Stress test: " + stressTestResult + "\r\n";
  txt += "\r\n";
  
  unsigned long seconds = diagnosticData.uptime / 1000;
  unsigned long minutes = seconds / 60;
  unsigned long hours = minutes / 60;
  unsigned long days = hours / 24;
  txt += "=== SYSTEM ===\r\n";
  txt += String(T().uptime) + ": " + String(days) + "d " + String(hours % 24) + "h " + String(minutes % 60) + "m\r\n";
  txt += String(T().last_reset) + ": " + getResetReason() + "\r\n";
  txt += "\r\n";
  txt += "========================================\r\n";
  txt += String(T().export_generated) + " " + String(millis()/1000) + "s " + String(T().export_after_boot) + "\r\n";
  txt += "========================================\r\n";
  
  server.sendHeader("Content-Disposition", "attachment; filename=esp32_diagnostic_v"+ String(DIAGNOSTIC_VERSION) +".txt");
  server.send(200, "text/plain; charset=utf-8", txt);
}

void handleExportJSON() {
  collectDiagnosticInfo();
  collectDetailedMemory();
  String stableUrl = getStableAccessURL();

  String json;
  json.reserve(3500);  // Reserve memory to avoid reallocations during export
  json = "{";
  json += "\"chip\":{";
  json += "\"model\":\"" + diagnosticData.chipModel + "\",";
  json += "\"revision\":\"" + diagnosticData.chipRevision + "\",";
  json += "\"cores\":" + String(diagnosticData.cpuCores) + ",";
  json += "\"freq_mhz\":" + String(diagnosticData.cpuFreqMHz) + ",";
  json += "\"mac\":\"" + diagnosticData.macAddress + "\",";
  json += "\"sdk\":\"" + diagnosticData.sdkVersion + "\",";
  json += "\"idf\":\"" + diagnosticData.idfVersion + "\"";
  if (diagnosticData.temperature != -999) {
    json += ",\"temperature\":" + String(diagnosticData.temperature, 1);
  }
  json += "},";
  
  json += "\"memory\":{";
  json += "\"flash_real_mb\":" + String(detailedMemory.flashSizeReal / 1048576.0, 2) + ",";
  json += "\"flash_config_mb\":" + String(detailedMemory.flashSizeChip / 1048576.0, 2) + ",";
  json += "\"flash_type\":\"" + getFlashType() + "\",";
  json += "\"flash_speed\":\"" + getFlashSpeed() + "\",";
  json += "\"psram_mb\":" + String(detailedMemory.psramTotal / 1048576.0, 2) + ",";
  json += "\"psram_free_mb\":" + String(detailedMemory.psramFree / 1048576.0, 2) + ",";
  json += "\"psram_available\":" + String(detailedMemory.psramAvailable ? "true" : "false") + ",";
  json += "\"psram_supported\":" + String(detailedMemory.psramBoardSupported ? "true" : "false") + ",";
  json += "\"psram_type\":\"" + String(detailedMemory.psramType ? detailedMemory.psramType : T().unknown.str()) + "\",";
  json += "\"sram_kb\":" + String(detailedMemory.sramTotal / 1024.0, 2) + ",";
  json += "\"sram_free_kb\":" + String(detailedMemory.sramFree / 1024.0, 2) + ",";
  json += "\"fragmentation\":" + String(detailedMemory.fragmentationPercent, 1) + ",";
  json += "\"status\":\"" + detailedMemory.memoryStatus + "\"";
  json += "},";
  
  json += "\"wifi\":{";
  json += "\"ssid\":\"" + diagnosticData.wifiSSID + "\",";
  json += "\"rssi\":" + String(diagnosticData.wifiRSSI) + ",";
  json += "\"quality\":\"" + getWiFiSignalQuality() + "\",";
  json += "\"ip\":\"" + diagnosticData.ipAddress + "\",";
  json += "\"mdns_ready\":" + String(diagnosticData.mdnsAvailable ? "true" : "false") + ",";
  json += "\"stable_url\":\"" + jsonEscape(stableUrl.c_str()) + "\",";
  json += "\"subnet\":\"" + WiFi.subnetMask().toString() + "\",";
  json += "\"gateway\":\"" + WiFi.gatewayIP().toString() + "\",";
  json += "\"dns\":\"" + WiFi.dnsIP().toString() + "\"";
  json += "},";

  json += "\"bluetooth\":{";
  json += "\"supported\":" + String(bluetoothCapable ? "true" : "false") + ",";
  json += "\"enabled\":" + String((bluetoothCapable && bluetoothEnabled) ? "true" : "false") + ",";
  json += "\"advertising\":" + String((bluetoothCapable && bluetoothAdvertising) ? "true" : "false") + ",";
#if BLE_STACK_SUPPORTED
  bool clientConnected = bluetoothClientConnected;
  uint32_t notifyCount = bluetoothNotifyCounter;
#else
  bool clientConnected = false;
  uint32_t notifyCount = 0;
#endif
  json += "\"clientConnected\":" + String((bluetoothCapable && clientConnected) ? "true" : "false") + ",";
  json += "\"notifyCount\":" + String(notifyCount) + ",";
  json += "\"name\":\"" + jsonEscape(bluetoothDeviceName.c_str()) + "\",";
  json += "\"mac\":\"" + jsonEscape(diagnosticData.bluetoothAddress.c_str()) + "\",";
  String btStatus = getBluetoothStateLabel();
  json += "\"status\":\"" + jsonEscape(btStatus.c_str()) + "\"";
  json += "},";
  
  json += "\"gpio\":{";
  json += "\"total\":" + String(diagnosticData.totalGPIO) + ",";
  json += "\"list\":\"" + diagnosticData.gpioList + "\"";
  json += "},";
  
  json += "\"peripherals\":{";
  json += "\"i2c_count\":" + String(diagnosticData.i2cCount) + ",";
  json += "\"i2c_devices\":\"" + diagnosticData.i2cDevices + "\",";
  json += "\"spi\":\"" + spiInfo + "\"";
  json += "},";
  
  json += "\"hardware_tests\":{";
  json += "\"builtin_led\":\"" + builtinLedTestResult + "\",";
  json += "\"neopixel\":\"" + neopixelTestResult + "\",";
  json += "\"oled\":\"" + oledTestResult + "\",";
  json += "\"adc\":\"" + adcTestResult + "\",";
  json += "\"pwm\":\"" + pwmTestResult + "\"";
  json += "},";
  
  json += "\"performance\":{";
  if (diagnosticData.cpuBenchmark > 0) {
    json += "\"cpu_us\":" + String(diagnosticData.cpuBenchmark) + ",";
    json += "\"cpu_mflops\":" + String(100000.0 / diagnosticData.cpuBenchmark, 2) + ",";
    json += "\"memory_us\":" + String(diagnosticData.memBenchmark);
  } else {
    json += "\"benchmarks\":\"not_run\"";
  }
  json += ",\"stress_test\":\"" + stressTestResult + "\"";
  json += "},";
  
  json += "\"system\":{";
  json += "\"uptime_ms\":" + String(diagnosticData.uptime) + ",";
  json += "\"reset_reason\":\"" + getResetReason() + "\",";
  json += "\"language\":\"" + String(currentLanguage == LANG_FR ? "fr" : "en") + "\"";
  json += "}";
  
  json += "}";
  
  server.sendHeader("Content-Disposition", "attachment; filename=esp32_diagnostic_v" + String(DIAGNOSTIC_VERSION) + ".json");
  server.send(200, "application/json", json);
}

void handleExportCSV() {
  collectDiagnosticInfo();
  collectDetailedMemory();

  String csv;
  csv.reserve(4000);  // Reserve memory to avoid reallocations during export
  csv = String(T().category) + "," + String(T().parameter) + "," + String(T().value) + "\r\n";
  
  csv += "Chip," + String(T().model) + "," + diagnosticData.chipModel + "\r\n";
  csv += "Chip," + String(T().revision) + "," + diagnosticData.chipRevision + "\r\n";
  csv += "Chip,CPU " + String(T().cores) + "," + String(diagnosticData.cpuCores) + "\r\n";
  csv += "Chip," + String(T().frequency) + " MHz," + String(diagnosticData.cpuFreqMHz) + "\r\n";
  csv += "Chip,MAC," + diagnosticData.macAddress + "\r\n";
  if (diagnosticData.temperature != -999) {
    csv += "Chip," + String(T().cpu_temp) + " C," + String(diagnosticData.temperature, 1) + "\r\n";
  }
  
  csv += String(T().memory_details) + ",Flash MB (" + String(T().board) + ")," + String(detailedMemory.flashSizeReal / 1048576.0, 2) + "\r\n";
  csv += String(T().memory_details) + ",Flash MB (config)," + String(detailedMemory.flashSizeChip / 1048576.0, 2) + "\r\n";
  csv += String(T().memory_details) + "," + String(T().flash_type) + "," + getFlashType() + "\r\n";
  csv += String(T().memory_details) + ",PSRAM MB," + String(detailedMemory.psramTotal / 1048576.0, 2) + "\r\n";
  csv += String(T().memory_details) + ",PSRAM " + String(T().free) + " MB," + String(detailedMemory.psramFree / 1048576.0, 2) + "\r\n";
  String psramStatus = detailedMemory.psramAvailable ? String(T().detected_active)
                                                    : (detailedMemory.psramBoardSupported ? String(T().supported_not_enabled)
                                                                                        : String(T().not_detected));
  csv += String(T().memory_details) + ",PSRAM Statut,\"" + psramStatus + "\"\r\n";
  if (!detailedMemory.psramAvailable && detailedMemory.psramBoardSupported) {
    String psramHint = String(T().enable_psram_hint);
    psramHint.replace("%TYPE%", detailedMemory.psramType ? detailedMemory.psramType : "PSRAM");
    psramHint.replace("\"", "'");
    csv += String(T().memory_details) + ",PSRAM Conseils,\"" + psramHint + "\"\r\n";
  }
  csv += String(T().memory_details) + ",SRAM KB," + String(detailedMemory.sramTotal / 1024.0, 2) + "\r\n";
  csv += String(T().memory_details) + ",SRAM " + String(T().free) + " KB," + String(detailedMemory.sramFree / 1024.0, 2) + "\r\n";
  csv += String(T().memory_details) + "," + String(T().memory_fragmentation) + " %," + String(detailedMemory.fragmentationPercent, 1) + "\r\n";
  
  csv += "WiFi,SSID," + diagnosticData.wifiSSID + "\r\n";
  csv += "WiFi,RSSI dBm," + String(diagnosticData.wifiRSSI) + "\r\n";
  csv += "WiFi,IP," + diagnosticData.ipAddress + "\r\n";
  csv += "WiFi," + String(T().gateway) + "," + WiFi.gatewayIP().toString() + "\r\n";

  csv += "Bluetooth," + String(T().bluetooth_status) + "," + getBluetoothStateLabel() + "\r\n";
  csv += "Bluetooth," + String(T().bluetooth_name) + "," + bluetoothDeviceName + "\r\n";
  csv += "Bluetooth," + String(T().bluetooth_mac) + "," + diagnosticData.bluetoothAddress + "\r\n";
  csv += "Bluetooth," + String(T().bluetooth_advertising) + "," + String((bluetoothCapable && bluetoothAdvertising) ? "ON" : "OFF") + "\r\n";

  csv += "GPIO," + String(T().total_gpio) + "," + String(diagnosticData.totalGPIO) + "\r\n";
  
  csv += String(T().i2c_peripherals) + "," + String(T().device_count) + "," + String(diagnosticData.i2cCount) + "\r\n";
  csv += String(T().i2c_peripherals) + "," + String(T().devices) + "," + diagnosticData.i2cDevices + "\r\n";
  
  csv += String(T().test) + "," + String(T().builtin_led) + "," + builtinLedTestResult + "\r\n";
  csv += String(T().test) + "," + String(T().neopixel) + "," + neopixelTestResult + "\r\n";
  csv += String(T().test) + ",OLED," + oledTestResult + "\r\n";
  csv += String(T().test) + ",ADC," + adcTestResult + "\r\n";
  csv += String(T().test) + ",PWM," + pwmTestResult + "\r\n";
  
  if (diagnosticData.cpuBenchmark > 0) {
    csv += String(T().performance_bench) + ",CPU us," + String(diagnosticData.cpuBenchmark) + "\r\n";
    csv += String(T().performance_bench) + "," + String(T().memory_benchmark) + " us," + String(diagnosticData.memBenchmark) + "\r\n";
  }
  
  csv += "System," + String(T().uptime) + " ms," + String(diagnosticData.uptime) + "\r\n";
  csv += "System," + String(T().last_reset) + "," + getResetReason() + "\r\n";
  
  server.sendHeader("Content-Disposition", "attachment; filename=esp32_diagnostic_v" + String(DIAGNOSTIC_VERSION) + ".csv");
  server.send(200, "text/csv; charset=utf-8", csv);
}

void handlePrintVersion() {
  collectDiagnosticInfo();
  collectDetailedMemory();
  
  String html = "<!DOCTYPE html><html><head><meta charset='UTF-8'><title>" + String(T().title) + " " + String(T().version) + String(DIAGNOSTIC_VERSION) + "</title>";
  html += "<style>";
  html += "@page{size:A4;margin:10mm}";
  html += "body{font:11px Arial;margin:10px;color:#333}";
  html += "h1{font-size:18px;margin:0 0 5px;border-bottom:3px solid #667eea;color:#667eea;padding-bottom:5px}";
  html += "h2{font-size:14px;margin:15px 0 8px;color:#667eea;border-bottom:1px solid #ddd;padding-bottom:3px}";
  html += ".section{margin-bottom:20px}";
  html += ".grid{display:grid;grid-template-columns:1fr 1fr;gap:10px;margin:10px 0}";
  html += ".row{display:flex;margin:3px 0;padding:5px;background:#f8f9fa;border-radius:3px}";
  html += ".row b{min-width:150px;color:#667eea}";
  html += ".badge{display:inline-block;padding:2px 8px;border-radius:10px;font-size:10px;font-weight:bold}";
  html += ".badge-success{background:#d4edda;color:#155724}";
  html += ".badge-warning{background:#fff3cd;color:#856404}";
  html += ".badge-danger{background:#f8d7da;color:#721c24}";
  html += "table{width:100%;border-collapse:collapse;margin:10px 0;font-size:10px}";
  html += "th{background:#667eea;color:#fff;padding:5px;text-align:left}";
  html += "td{border:1px solid #ddd;padding:4px}";
  html += ".footer{margin-top:20px;padding-top:10px;border-top:1px solid #ddd;font-size:9px;color:#666;text-align:center}";
  html += "</style></head>";
  html += "<body onload='window.print()'>";
  
  // Header traduit
  html += "<h1>" + String(T().title) + " " + String(T().version) + String(DIAGNOSTIC_VERSION) + "</h1>";
  html += "<div style='margin:10px 0;font-size:12px;color:#666'>";
  html += String(T().export_generated) + " " + String(millis()/1000) + "s " + String(T().export_after_boot) + " | IP: " + diagnosticData.ipAddress;
  html += "</div>";
  
  // Chip
  html += "<div class='section'>";
  html += "<h2>" + String(T().chip_info) + "</h2>";
  html += "<div class='grid'>";
  html += "<div class='row'><b>" + String(T().full_model) + ":</b><span>" + diagnosticData.chipModel + " Rev" + diagnosticData.chipRevision + "</span></div>";
  html += "<div class='row'><b>" + String(T().cpu_cores) + ":</b><span>" + String(diagnosticData.cpuCores) + " " + String(T().cores) + " @ " + String(diagnosticData.cpuFreqMHz) + " MHz</span></div>";
  html += "<div class='row'><b>" + String(T().mac_wifi) + ":</b><span>" + diagnosticData.macAddress + "</span></div>";
  html += "<div class='row'><b>" + String(T().sdk_version) + ":</b><span>" + diagnosticData.sdkVersion + "</span></div>";
  html += "<div class='row'><b>" + String(T().idf_version) + ":</b><span>" + diagnosticData.idfVersion + "</span></div>";
  if (diagnosticData.temperature != -999) {
    html += "<div class='row'><b>" + String(T().cpu_temp) + ":</b><span>" + String(diagnosticData.temperature, 1) + " °C</span></div>";
  }

  unsigned long seconds = diagnosticData.uptime / 1000;
  unsigned long minutes = seconds / 60;
  unsigned long hours = minutes / 60;
  unsigned long days = hours / 24;
  html += "<div class='row'><b>" + String(T().uptime) + ":</b><span>" + formatUptime(days, hours % 24, minutes % 60) + "</span></div>";
  html += "<div class='row'><b>" + String(T().last_reset) + ":</b><span>" + getResetReason() + "</span></div>";
  html += "</div></div>";

  // Mémoire
  html += "<div class='section'>";
  html += "<h2>" + String(T().memory_details) + "</h2>";
  html += "<table>";
  html += "<tr><th>" + String(T().category) + "</th><th>" + String(T().total_size) + "</th><th>" + String(T().free) + "</th><th>" + String(T().used) + "</th><th>" + String(T().status) + "</th></tr>";

  // Flash
  bool flashMatch = (detailedMemory.flashSizeReal == detailedMemory.flashSizeChip);
  html += "<tr><td><b>" + String(T().flash_memory) + "</b></td>";
  html += "<td>" + String(detailedMemory.flashSizeReal / 1048576.0, 1) + " MB</td>";
  html += "<td>-</td><td>-</td>";
  html += "<td><span class='badge " + String(flashMatch ? "badge-success'>" + String(T().ok) : "badge-warning'>" + String(T().ide_config)) + "</span></td></tr>";

  // PSRAM
  html += "<tr><td><b>" + String(T().psram_external) + "</b></td>";
  html += "<td>" + String(detailedMemory.psramTotal / 1048576.0, 1) + " MB</td>";
  if (detailedMemory.psramAvailable) {
    html += "<td>" + String(detailedMemory.psramFree / 1048576.0, 1) + " MB</td>";
    html += "<td>" + String(detailedMemory.psramUsed / 1048576.0, 1) + " MB</td>";
    html += "<td><span class='badge badge-success'>" + String(T().detected_active) + "</span></td>";
  } else if (detailedMemory.psramBoardSupported) {
    html += "<td>-</td><td>-</td>";
    String psramHint = String(T().enable_psram_hint);
    psramHint.replace("%TYPE%", detailedMemory.psramType ? detailedMemory.psramType : "PSRAM");
    html += "<td><span class='badge badge-warning'>" + String(T().supported_not_enabled) + "</span><br><small>" + psramHint + "</small></td>";
  } else {
    html += "<td>-</td><td>-</td>";
    html += "<td><span class='badge badge-danger'>" + String(T().not_detected) + "</span></td>";
  }
  html += "</tr>";

  // SRAM
  html += "<tr><td><b>" + String(T().internal_sram) + "</b></td>";
  html += "<td>" + String(detailedMemory.sramTotal / 1024.0, 1) + " KB</td>";
  html += "<td>" + String(detailedMemory.sramFree / 1024.0, 1) + " KB</td>";
  html += "<td>" + String(detailedMemory.sramUsed / 1024.0, 1) + " KB</td>";
  html += "<td><span class='badge badge-success'>" + detailedMemory.memoryStatus + "</span></td></tr>";
  html += "</table>";
  html += "<div class='row'><b>" + String(T().memory_fragmentation) + ":</b><span>" + String(detailedMemory.fragmentationPercent, 1) + "% - " + detailedMemory.memoryStatus + "</span></div>";
  html += "</div>";

  // WiFi
  html += "<div class='section'>";
  html += "<h2>" + String(T().wifi_connection) + "</h2>";
  html += "<div class='grid'>";
  html += "<div class='row'><b>" + String(T().connected_ssid) + ":</b><span>" + diagnosticData.wifiSSID + "</span></div>";
  html += "<div class='row'><b>" + String(T().signal_power) + ":</b><span>" + String(diagnosticData.wifiRSSI) + " dBm</span></div>";
  html += "<div class='row'><b>" + String(T().signal_quality) + ":</b><span>" + getWiFiSignalQuality() + "</span></div>";
  html += "<div class='row'><b>" + String(T().ip_address) + ":</b><span>" + diagnosticData.ipAddress + "</span></div>";
  html += "<div class='row'><b>" + String(T().subnet_mask) + ":</b><span>" + WiFi.subnetMask().toString() + "</span></div>";
  html += "<div class='row'><b>" + String(T().dns) + ":</b><span>" + WiFi.dnsIP().toString() + "</span></div>";
  html += "<div class='row'><b>" + String(T().gateway) + ":</b><span>" + WiFi.gatewayIP().toString() + "</span></div>";
  html += "</div></div>";

  html += "<div class='section'>";
  html += "<h2>" + String(T().bluetooth_section) + "</h2>";
  html += "<div class='grid'>";
  html += "<div class='row'><b>" + String(T().bluetooth_status) + ":</b><span>" + htmlEscape(getBluetoothStateLabel()) + "</span></div>";
  html += "<div class='row'><b>" + String(T().bluetooth_name) + ":</b><span>" + htmlEscape(bluetoothDeviceName) + "</span></div>";
  html += "<div class='row'><b>" + String(T().bluetooth_mac) + ":</b><span>" + htmlEscape(diagnosticData.bluetoothAddress) + "</span></div>";
  String printAdvertising = bluetoothCapable && bluetoothAdvertising ? String(T().bluetooth_advertising) : String(T().bluetooth_not_advertising);
  html += "<div class='row'><b>" + String(T().bluetooth_advertising_label) + ":</b><span>" + htmlEscape(printAdvertising) + "</span></div>";
  html += "</div></div>";

  // GPIO et Périphériques
  html += "<div class='section'>";
  html += "<h2>" + String(T().gpio_interfaces) + "</h2>";
  html += "<div class='grid'>";
  html += "<div class='row'><b>" + String(T().total_gpio) + ":</b><span>" + String(diagnosticData.totalGPIO) + " " + String(T().pins) + "</span></div>";
  html += "<div class='row'><b>" + String(T().i2c_peripherals) + ":</b><span>" + String(diagnosticData.i2cCount) + " " + String(T().devices) + " - " + diagnosticData.i2cDevices + "</span></div>";
  html += "<div class='row'><b>" + String(T().spi_bus) + ":</b><span>" + spiInfo + "</span></div>";
  html += "</div></div>";

  // Tests Matériels
  html += "<div class='section'>";
  html += "<h2>" + String(T().nav_tests) + "</h2>";
  html += "<table>";
  html += "<tr><th>" + String(T().parameter) + "</th><th>" + String(T().status) + "</th></tr>";
  html += "<tr><td>" + String(T().builtin_led) + "</td><td>" + builtinLedTestResult + "</td></tr>";
  html += "<tr><td>" + String(T().neopixel) + "</td><td>" + neopixelTestResult + "</td></tr>";
  html += "<tr><td>" + String(T().oled_screen) + "</td><td>" + oledTestResult + "</td></tr>";
  html += "<tr><td>" + String(T().adc_test) + "</td><td>" + adcTestResult + "</td></tr>";
  html += "<tr><td>" + String(T().pwm_test) + "</td><td>" + pwmTestResult + "</td></tr>";
  html += "</table></div>";

  // Performance
  if (diagnosticData.cpuBenchmark > 0) {
    html += "<div class='section'>";
    html += "<h2>" + String(T().performance_bench) + "</h2>";
    html += "<div class='grid'>";
    html += "<div class='row'><b>" + String(T().cpu_benchmark) + ":</b><span>" + String(diagnosticData.cpuBenchmark) + " µs (" + String(100000.0 / diagnosticData.cpuBenchmark, 2) + " MFLOPS)</span></div>";
    html += "<div class='row'><b>" + String(T().memory_benchmark) + ":</b><span>" + String(diagnosticData.memBenchmark) + " µs</span></div>";
    html += "<div class='row'><b>" + String(T().memory_stress) + ":</b><span>" + stressTestResult + "</span></div>";
    html += "</div></div>";
  }
  
  // Footer
  html += "<div class='footer'>";
  html += "ESP32 Diagnostic v"+ String(DIAGNOSTIC_VERSION) + " | " + diagnosticData.chipModel + " | MAC: " + diagnosticData.macAddress;
  html += "</div>";
  
  html += "</body></html>";
  
  server.send(200, "text/html; charset=utf-8", html);
}

// ========== HANDLER CHANGEMENT DE LANGUE ==========
void handleSetLanguage() {
  if (server.hasArg("lang")) {
    const String lang = server.arg("lang");
    if (lang == "fr") {
      setLanguage(LANG_FR);
      sendActionResponse(200, true, String(), {
        jsonStringField("lang", "fr")
      });
      return;
    }
    if (lang == "en") {
      setLanguage(LANG_EN);
      sendActionResponse(200, true, String(), {
        jsonStringField("lang", "en")
      });
      return;
    }
    sendActionResponse(400, false, String(), {
      jsonStringField("error", "unsupported_language")
    });
  } else {
    sendOperationError(400, T().configuration_invalid.str());
  }
}

String htmlEscape(const String& raw) {
  String escaped;
  escaped.reserve(raw.length() * 6);
  for (size_t i = 0; i < raw.length(); ++i) {
    char c = raw[i];
    switch (c) {
      case '&':
        escaped += "&amp;";
        break;
      case '<':
        escaped += "&lt;";
        break;
      case '>':
        escaped += "&gt;";
        break;
      case '\"':
        escaped += "&quot;";
        break;
      case '\'':
        escaped += "&#39;";
        break;
      default:
        escaped += c;
        break;
    }
  }
  return escaped;
}

String jsonEscape(const char* raw) {
  if (raw == nullptr) {
    return "";
  }

  const size_t rawLength = strlen(raw);
  String escaped;
  escaped.reserve(rawLength * 2);
  while (*raw) {
    char c = *raw++;
    switch (c) {
      case '\\':
        escaped += "\\\\";
        break;
      case '"':
        escaped += "\\\"";
        break;
      case '\n':
        escaped += "\\n";
        break;
      case '\r':
        escaped += "\\r";
        break;
      case '\t':
        escaped += "\\t";
        break;
      default:
        escaped += c;
        break;
    }
  }
  return escaped;
}

static inline void appendJsonField(String& json, bool& first, const JsonFieldSpec& field) {
  if (!first) {
    json += ',';
  }
  first = false;
  json += '"';
  json += field.key;
  json += '"';
  json += ':';
  if (field.raw) {
    json += field.value;
  } else {
    json += '"';
    json += jsonEscape(field.value.c_str());
    json += '"';
  }
}

String buildJsonObject(std::initializer_list<JsonFieldSpec> fields) {
  String json;
  json.reserve(fields.size() * 50 + 10);  // Estimate size based on field count
  json = "{";
  bool first = true;
  for (const auto& field : fields) {
    appendJsonField(json, first, field);
  }
  json += '}';
  return json;
}

inline void sendJsonResponse(int statusCode, std::initializer_list<JsonFieldSpec> fields) {
  server.send(statusCode, "application/json", buildJsonObject(fields));
}

static String buildActionResponseJson(bool success,
                                      const String& message,
                                      std::initializer_list<JsonFieldSpec> extraFields) {
  String json;
  json.reserve(extraFields.size() * 50 + message.length() + 50);  // Estimate size
  json = "{";
  bool first = true;
  appendJsonField(json, first, jsonBoolField("success", success));
  if (message.length() > 0) {
    appendJsonField(json, first, jsonStringField("message", message));
  }
  for (const auto& field : extraFields) {
    appendJsonField(json, first, field);
  }
  json += '}';
  return json;
}

inline void sendActionResponse(int statusCode,
                               bool success,
                               const String& message,
                               std::initializer_list<JsonFieldSpec> extraFields) {
  server.send(statusCode, "application/json", buildActionResponseJson(success, message, extraFields));
}

inline void sendOperationSuccess(const String& message,
                                 std::initializer_list<JsonFieldSpec> extraFields) {
  sendActionResponse(200, true, message, extraFields);
}

inline void sendOperationError(int statusCode,
                               const String& message,
                               std::initializer_list<JsonFieldSpec> extraFields) {
  sendActionResponse(statusCode, false, message, extraFields);
}

static inline void appendInfoItem(String& chunk,
                                  const char* labelKey,
                                  const char* labelText,
                                  const String& valueText,
                                  const String& valueAttrs = String(),
                                  const String& wrapperAttrs = String()) {
  chunk += F("<div class='info-item'");
  if (wrapperAttrs.length() > 0) {
    chunk += ' ';
    chunk += wrapperAttrs;
  }
  chunk += F("><div class='info-label");
  if (labelKey != nullptr && labelKey[0] != '\0') {
    chunk += F(" data-i18n='");
    chunk += labelKey;
    chunk += F("'>");
  } else {
    chunk += F("'>");
  }
  chunk += labelText;
  chunk += F("</div><div class='info-value'");
  if (valueAttrs.length() > 0) {
    chunk += ' ';
    chunk += valueAttrs;
  }
  chunk += F(">");
  chunk += valueText;
  chunk += F("</div></div>");
}

String jsonField(const char* key, const char* value, bool last = false) {
  String field;
  field.reserve(strlen(key) + strlen(value) + 10);  // Estimate size to avoid reallocation
  field = "\"";
  field += key;
  field += "\":\"";
  field += jsonEscape(value);
  field += last ? "\"" : "\",";
  return field;
}

String sanitizeBluetoothName(const String& raw) {
  String cleaned;
  cleaned.reserve(raw.length());
  for (size_t i = 0; i < raw.length(); ++i) {
    char c = raw[i];
    if (c >= 32 && c != 127) {
      cleaned += c;
    }
    if (cleaned.length() >= 29) {
      break;
    }
  }
  cleaned.trim();
  return cleaned;
}

void ensureBluetoothName() {
  if (bluetoothDeviceName.length() == 0) {
    bluetoothDeviceName = defaultBluetoothName.length() > 0 ? defaultBluetoothName : String("ESP32 Diagnostic");
  }
}

void syncBluetoothDiagnostics() {
  ensureBluetoothName();

  bool supported = bluetoothCapable;
  bool hasRadio = bluetoothCapable || diagnosticData.hasBLE || diagnosticData.hasBT;
  diagnosticData.bluetoothName = bluetoothDeviceName;
  diagnosticData.bluetoothEnabled = supported && bluetoothEnabled;
  diagnosticData.bluetoothAdvertising = supported && (bluetoothAdvertising
#if BLE_STACK_SUPPORTED
      || bluetoothClientConnected
#endif
  );

  if (hasRadio) {
    uint8_t mac[6] = {0};
    if (esp_read_mac(mac, ESP_MAC_BT) == ESP_OK) {
      char macStr[18];
      sprintf(macStr, "%02X:%02X:%02X:%02X:%02X:%02X",
              mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
      diagnosticData.bluetoothAddress = String(macStr);
    } else {
      diagnosticData.bluetoothAddress = String("--:--:--:--:--:--");
    }
  } else {
    diagnosticData.bluetoothAddress = String("--:--:--:--:--:--");
  }
}

String getBluetoothStateLabel() {
  if (!bluetoothCapable) {
    return String(T().bluetooth_not_supported);
  }
  if (!bluetoothEnabled) {
    return String(T().bluetooth_disabled);
  }
#if BLE_STACK_SUPPORTED
  if (bluetoothClientConnected) {
    return String(T().bluetooth_enabled);
  }
#endif
  if (bluetoothAdvertising) {
    return String(T().bluetooth_advertising);
  }
  return String(T().bluetooth_not_advertising);
}

String getBluetoothSummaryLabel() {
  if (!bluetoothCapable) {
    return String(T().bluetooth_not_supported);
  }
  if (!bluetoothEnabled) {
    return String(T().bluetooth_disabled);
  }

  ensureBluetoothName();
  String summary = bluetoothDeviceName;
  summary += " • ";
#if BLE_STACK_SUPPORTED
  if (bluetoothClientConnected) {
    summary += String(T().bluetooth_enabled);
  } else
#endif
  {
    summary += bluetoothAdvertising ? String(T().bluetooth_advertising) : String(T().bluetooth_not_advertising);
  }
  return summary;
}

String buildBluetoothJSON(bool success, const String& message) {
  syncBluetoothDiagnostics();

  String json = "{";
  json += "\"success\":" + String(success ? "true" : "false") + ",";
  json += "\"supported\":" + String(bluetoothCapable ? "true" : "false") + ",";
  json += "\"enabled\":" + String((bluetoothCapable && bluetoothEnabled) ? "true" : "false") + ",";
  json += "\"advertising\":" + String((bluetoothCapable && bluetoothAdvertising) ? "true" : "false") + ",";
  json += "\"name\":\"" + jsonEscape(bluetoothDeviceName.c_str()) + "\",";
  json += "\"mac\":\"" + jsonEscape(diagnosticData.bluetoothAddress.c_str()) + "\",";
  String status = getBluetoothStateLabel();
  json += "\"status\":\"" + jsonEscape(status.c_str()) + "\",";
  String summary = getBluetoothSummaryLabel();
  json += "\"summary\":\"" + jsonEscape(summary.c_str()) + "\",";
  json += "\"message\":\"" + jsonEscape(message.c_str()) + "\"";
  json += "}";
  return json;
}

#if BLE_STACK_SUPPORTED
void dispatchBluetoothTelemetry() {
  if (!bluetoothEnabled || !bluetoothClientConnected || !bluetoothCharacteristic) {
    return;
  }

  unsigned long now = millis();
  if (now - lastBluetoothNotify < 2000) {
    return;
  }

  lastBluetoothNotify = now;
  bluetoothNotifyCounter++;

  unsigned long uptimeSeconds = diagnosticData.uptime / 1000UL;
  char payload[96];
  int written = snprintf(payload, sizeof(payload),
                         "Diag %lu | Uptime %lus",
                         static_cast<unsigned long>(bluetoothNotifyCounter),
                         static_cast<unsigned long>(uptimeSeconds));
  if (written < 0) {
    return;
  }

  size_t length = (written >= static_cast<int>(sizeof(payload))) ? sizeof(payload) - 1 : static_cast<size_t>(written);
  if (length == 0) {
    return;
  }
  bluetoothCharacteristic->setValue(reinterpret_cast<uint8_t*>(payload), length);
  bluetoothCharacteristic->notify();
  Serial.printf("[BLE] Notification #%lu envoyée (%lus)\n",
                static_cast<unsigned long>(bluetoothNotifyCounter),
                static_cast<unsigned long>(uptimeSeconds));
}
#endif

bool startBluetooth() {
  ensureBluetoothName();

  if (!bluetoothCapable) {
    bluetoothEnabled = false;
    bluetoothAdvertising = false;
#if BLE_STACK_SUPPORTED
    bluetoothClientConnected = false;
    bluetoothNotifyCounter = 0;
    lastBluetoothNotify = 0;
#endif
    return false;
  }

#if BLE_STACK_SUPPORTED
  if (bluetoothAdvertising) {
    bluetoothEnabled = true;
    return true;
  }

  BLEDevice::init(bluetoothDeviceName.c_str());
  bluetoothServer = BLEDevice::createServer();
  if (!bluetoothServer) {
    bluetoothEnabled = false;
    bluetoothAdvertising = false;
    bluetoothClientConnected = false;
    bluetoothNotifyCounter = 0;
    lastBluetoothNotify = 0;
    return false;
  }

  bluetoothServer->setCallbacks(new DiagnosticBLECallbacks());

  bluetoothService = bluetoothServer->createService(BLEUUID(DIAG_BLE_SERVICE_UUID));
  if (!bluetoothService) {
    BLEDevice::deinit(true);
    bluetoothServer = nullptr;
    bluetoothEnabled = false;
    bluetoothAdvertising = false;
    bluetoothClientConnected = false;
    bluetoothNotifyCounter = 0;
    lastBluetoothNotify = 0;
    return false;
  }

  bluetoothCharacteristic = bluetoothService->createCharacteristic(
      BLEUUID(DIAG_BLE_CHARACTERISTIC_UUID),
      BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY);
  if (bluetoothCharacteristic) {
    bluetoothCharacteristic->setValue("ESP32 Diagnostic prêt");
    bluetoothCharacteristic->addDescriptor(new BLE2902());
  }

  bluetoothService->start();
  BLEAdvertising* advertising = BLEDevice::getAdvertising();
  if (!advertising) {
    bluetoothService->stop();
    BLEDevice::deinit(true);
    bluetoothServer = nullptr;
    bluetoothService = nullptr;
    bluetoothCharacteristic = nullptr;
    bluetoothEnabled = false;
    bluetoothAdvertising = false;
    bluetoothClientConnected = false;
    bluetoothNotifyCounter = 0;
    lastBluetoothNotify = 0;
    return false;
  }

  advertising->addServiceUUID(BLEUUID(DIAG_BLE_SERVICE_UUID));
  advertising->setScanResponse(true);
  advertising->setMinPreferred(0x06);
  advertising->setMinPreferred(0x12);
  advertising->start();
  BLEDevice::startAdvertising();

  bluetoothEnabled = true;
  bluetoothAdvertising = true;
  bluetoothClientConnected = false;
  bluetoothNotifyCounter = 0;
  lastBluetoothNotify = millis();
  return true;
#else
  return false;
#endif
}

void stopBluetooth() {
#if BLE_STACK_SUPPORTED
  if (bluetoothAdvertising) {
    BLEAdvertising* advertising = BLEDevice::getAdvertising();
    if (advertising) {
      advertising->stop();
    }
  }

  bluetoothClientConnected = false;
  bluetoothNotifyCounter = 0;
  lastBluetoothNotify = 0;

  if (bluetoothService) {
    bluetoothService->stop();
  }
  if (bluetoothServer) {
    bluetoothServer->disconnect(0);
  }

  BLEDevice::deinit(true);
  bluetoothServer = nullptr;
  bluetoothService = nullptr;
  bluetoothCharacteristic = nullptr;
#endif

  bluetoothEnabled = false;
  bluetoothAdvertising = false;
}

String buildTranslationsJSON() {
  String json = "{";
  json.reserve(8000);  // Increased reserve to avoid reallocations (~200+ fields)
  json += jsonField("title", T().title);
  json += jsonField("version", T().version);
  json += jsonField("nav_overview", T().nav_overview);
  json += jsonField("nav_leds", T().nav_leds);
  json += jsonField("nav_screens", T().nav_screens);
  json += jsonField("nav_tests", T().nav_tests);
  json += jsonField("nav_gpio", T().nav_gpio);
  json += jsonField("nav_wireless", T().nav_wireless);
  json += jsonField("nav_benchmark", T().nav_benchmark);
  json += jsonField("nav_export", T().nav_export);
  json += jsonField("nav_select_label", T().nav_select_label);
  json += jsonField("chip_info", T().chip_info);
  json += jsonField("full_model", T().full_model);
  json += jsonField("cpu_cores", T().cpu_cores);
  json += jsonField("mac_wifi", T().mac_wifi);
  json += jsonField("last_reset", T().last_reset);
  json += jsonField("poweron", T().poweron);
  json += jsonField("software_reset", T().software_reset);
  json += jsonField("deepsleep_exit", T().deepsleep_exit);
  json += jsonField("brownout", T().brownout);
  json += jsonField("other", T().other);
  json += jsonField("chip_features", T().chip_features);
  json += jsonField("sdk_version", T().sdk_version);
  json += jsonField("idf_version", T().idf_version);
  json += jsonField("uptime", T().uptime);
  json += jsonField("cpu_temp", T().cpu_temp);
  json += jsonField("revision", T().revision);
  json += jsonField("days", T().days);
  json += jsonField("hours", T().hours);
  json += jsonField("minutes", T().minutes);
  json += jsonField("memory_details", T().memory_details);
  json += jsonField("flash_memory", T().flash_memory);
  json += jsonField("real_size", T().real_size);
  json += jsonField("configured_ide", T().configured_ide);
  json += jsonField("configuration", T().configuration);
  json += jsonField("correct", T().correct);
  json += jsonField("to_fix", T().to_fix);
  json += jsonField("flash_type", T().flash_type);
  json += jsonField("flash_speed", T().flash_speed);
  json += jsonField("psram_external", T().psram_external);
  json += jsonField("hardware_status", T().hardware_status);
  json += jsonField("detected_active", T().detected_active);
  json += jsonField("supported_not_enabled", T().supported_not_enabled);
  json += jsonField("ide_config", T().ide_config);
  json += jsonField("total_size", T().total_size);
  json += jsonField("free", T().free);
  json += jsonField("used", T().used);
  json += jsonField("excellent", T().excellent);
  json += jsonField("very_good", T().very_good);
  json += jsonField("good", T().good);
  json += jsonField("warning", T().warning);
  json += jsonField("critical", T().critical);
  json += jsonField("weak", T().weak);
  json += jsonField("very_weak", T().very_weak);
  json += jsonField("none", T().none);
  json += jsonField("unknown", T().unknown);
  json += jsonField("enable_psram_hint", T().enable_psram_hint);
  json += jsonField("not_detected", T().not_detected);
  json += jsonField("internal_sram", T().internal_sram);
  json += jsonField("memory_fragmentation", T().memory_fragmentation);
  json += jsonField("refresh_memory", T().refresh_memory);
  json += jsonField("wifi_connection", T().wifi_connection);
  json += jsonField("connected_ssid", T().connected_ssid);
  json += jsonField("signal_power", T().signal_power);
  json += jsonField("signal_quality", T().signal_quality);
  json += jsonField("ip_address", T().ip_address);
  json += jsonField("subnet_mask", T().subnet_mask);
  json += jsonField("gateway", T().gateway);
  json += jsonField("dns", T().dns);
  json += jsonField("ip_unavailable", T().ip_unavailable);
  json += jsonField("access", T().access);
  json += jsonField("bluetooth_section", T().bluetooth_section);
  json += jsonField("bluetooth_status", T().bluetooth_status);
  json += jsonField("bluetooth_name", T().bluetooth_name);
  json += jsonField("bluetooth_mac", T().bluetooth_mac);
  json += jsonField("bluetooth_actions", T().bluetooth_actions);
  json += jsonField("bluetooth_enable", T().bluetooth_enable);
  json += jsonField("bluetooth_disable", T().bluetooth_disable);
  json += jsonField("bluetooth_rename", T().bluetooth_rename);
  json += jsonField("bluetooth_reset", T().bluetooth_reset);
  json += jsonField("bluetooth_placeholder", T().bluetooth_placeholder);
  json += jsonField("bluetooth_not_supported", T().bluetooth_not_supported);
  json += jsonField("bluetooth_disabled", T().bluetooth_disabled);
  json += jsonField("bluetooth_enabled", T().bluetooth_enabled);
  json += jsonField("bluetooth_advertising", T().bluetooth_advertising);
  json += jsonField("bluetooth_not_advertising", T().bluetooth_not_advertising);
  json += jsonField("bluetooth_updated", T().bluetooth_updated);
  json += jsonField("bluetooth_error", T().bluetooth_error);
  json += jsonField("bluetooth_reset_done", T().bluetooth_reset_done);
  json += jsonField("bluetooth_support_label", T().bluetooth_support_label);
  json += jsonField("bluetooth_support_yes", T().bluetooth_support_yes);
  json += jsonField("bluetooth_support_no", T().bluetooth_support_no);
  json += jsonField("bluetooth_advertising_label", T().bluetooth_advertising_label);
  json += jsonField("bluetooth_connection_label", T().bluetooth_connection_label);
  json += jsonField("bluetooth_client_connected", T().bluetooth_client_connected);
  json += jsonField("bluetooth_client_disconnected", T().bluetooth_client_disconnected);
  json += jsonField("bluetooth_notifications_label", T().bluetooth_notifications_label);
  json += jsonField("gpio", T().gpio);
  json += jsonField("status", T().status);
  json += jsonField("config", T().config);
  json += jsonField("test", T().test);
  json += jsonField("turn_on", T().turn_on);
  json += jsonField("turn_off", T().turn_off);
  json += jsonField("turn_off_all", T().turn_off_all);
  json += jsonField("blink", T().blink);
  json += jsonField("fade", T().fade);
  json += jsonField("off", T().off);
  json += jsonField("neopixel", T().neopixel);
  json += jsonField("animations", T().animations);
  json += jsonField("led_count", T().led_count);
  json += jsonField("rainbow", T().rainbow);
  json += jsonField("chase", T().chase);
  json += jsonField("color", T().color);
  json += jsonField("custom_color", T().custom_color);
  json += jsonField("gpio_interfaces", T().gpio_interfaces);
  json += jsonField("total_gpio", T().total_gpio);
  json += jsonField("i2c_peripherals", T().i2c_peripherals);
  json += jsonField("device_count", T().device_count);
  json += jsonField("detected_addresses", T().detected_addresses);
  json += jsonField("builtin_led", T().builtin_led);
  json += jsonField("oled_screen", T().oled_screen);
  json += jsonField("status", T().status);
  json += jsonField("i2c_pins", T().i2c_pins);
  json += jsonField("rotation", T().rotation);
  json += jsonField("apply_redetect", T().apply_redetect);
  json += jsonField("full_test", T().full_test);
  json += jsonField("oled_step_welcome", T().oled_step_welcome);
  json += jsonField("oled_step_big_text", T().oled_step_big_text);
  json += jsonField("oled_step_text_sizes", T().oled_step_text_sizes);
  json += jsonField("oled_step_shapes", T().oled_step_shapes);
  json += jsonField("oled_step_horizontal_lines", T().oled_step_horizontal_lines);
  json += jsonField("oled_step_diagonals", T().oled_step_diagonals);
  json += jsonField("oled_step_moving_square", T().oled_step_moving_square);
  json += jsonField("oled_step_progress_bar", T().oled_step_progress_bar);
  json += jsonField("oled_step_scroll_text", T().oled_step_scroll_text);
  json += jsonField("oled_step_final_message", T().oled_step_final_message);
  json += jsonField("oled_step_running", T().oled_step_running);
  json += jsonField("oled_message_required", T().oled_message_required);
  json += jsonField("oled_displaying_message", T().oled_displaying_message);
  json += jsonField("custom_message", T().custom_message);
  json += jsonField("show_message", T().show_message);
  json += jsonField("apply_color", T().apply_color);
  json += jsonField("adc_test", T().adc_test);
  json += jsonField("start_adc_test", T().start_adc_test);
  json += jsonField("pwm_test", T().pwm_test);
  json += jsonField("spi_bus", T().spi_bus);
  json += jsonField("flash_partitions", T().flash_partitions);
  json += jsonField("memory_stress", T().memory_stress);
  json += jsonField("stress_warning", T().stress_warning);
  json += jsonField("start_stress", T().start_stress);
  json += jsonField("gpio_test", T().gpio_test);
  json += jsonField("test_all_gpio", T().test_all_gpio);
  json += jsonField("click_to_test", T().click_to_test);
  json += jsonField("not_tested", T().not_tested);
  json += jsonField("gpio_warning", T().gpio_warning);
  json += jsonField("i2c_desc", T().i2c_desc);
  json += jsonField("adc_desc", T().adc_desc);
  json += jsonField("pwm_desc", T().pwm_desc);
  json += jsonField("spi_desc", T().spi_desc);
  json += jsonField("partitions_desc", T().partitions_desc);
  json += jsonField("stress_desc", T().stress_desc);
  json += jsonField("gpio_desc", T().gpio_desc);
  json += jsonField("wifi_desc", T().wifi_desc);
  json += jsonField("benchmark_desc", T().benchmark_desc);
  json += jsonField("wifi_scanner", T().wifi_scanner);
  json += jsonField("performance_bench", T().performance_bench);
  json += jsonField("data_export", T().data_export);
  json += jsonField("click_to_scan", T().click_to_scan);
  json += jsonField("updating", T().updating);
  json += jsonField("online", T().online);
  json += jsonField("offline", T().offline);
  json += jsonField("check_network", T().check_network);
  json += jsonField("language_label", T().language_label);
  json += jsonField("language_updated", T().language_updated);
  json += jsonField("language_switch_error", T().language_switch_error);
  json += jsonField("translation_error", T().translation_error);
  json += jsonField("bluetooth_invalid_name", T().bluetooth_invalid_name);
  json += jsonField("bluetooth_enabling", T().bluetooth_enabling);
  json += jsonField("bluetooth_disabling", T().bluetooth_disabling);
  json += jsonField("bluetooth_updating", T().bluetooth_updating);
  json += jsonField("bluetooth_resetting", T().bluetooth_resetting);
  json += jsonField("loading", T().loading);
  json += jsonField("configuring", T().configuring);
  json += jsonField("reconfiguring", T().reconfiguring);
  json += jsonField("transmission", T().transmission);
  json += jsonField("error_label", T().error_label);
  json += jsonField("test_failed", T().test_failed);
  json += jsonField("gpio_summary_template", T().gpio_summary_template);
  json += jsonField("wifi_networks_found", T().wifi_networks_found);
  json += jsonField("i2c_scan_result", T().i2c_scan_result);
  json += jsonField("gpio_invalid", T().gpio_invalid);
  json += jsonField("configuration_invalid", T().configuration_invalid);
  json += jsonField("testing", T().testing);
  json += jsonField("scan", T().scan);
  json += jsonField("scanning", T().scanning);
  json += jsonField("completed", T().completed);
  json += jsonField("cores", T().cores);
  json += jsonField("pins", T().pins);
  json += jsonField("devices", T().devices);
  json += jsonField("networks", T().networks);
  json += jsonField("tested", T().tested);
  json += jsonField("channels", T().channels);
  json += jsonField("oled_test_running", T().oled_test_running, true);
  json += "}";

  return json;
}

void handleGetTranslations() {
  server.send(200, "application/json; charset=utf-8", buildTranslationsJSON());
}

void handleBluetoothStatus() {
  String message = getBluetoothStateLabel();
  server.send(200, "application/json", buildBluetoothJSON(true, message));
}

void handleBluetoothToggle() {
  if (!server.hasArg("state")) {
    server.send(400, "application/json", buildBluetoothJSON(false, String(T().bluetooth_error)));
    return;
  }

  String state = server.arg("state");
  state.toLowerCase();
  bool enable = (state == "on" || state == "1" || state == "true");

  bool supported = bluetoothCapable;
  bool success = false;
  String message;

  if (!supported) {
    message = String(T().bluetooth_not_supported);
  } else if (enable) {
    success = startBluetooth();
    message = success ? String(T().bluetooth_enabled) : String(T().bluetooth_error);
  } else {
    stopBluetooth();
    success = true;
    message = String(T().bluetooth_disabled);
  }

  if (!supported) {
    success = false;
  }

  server.send(200, "application/json", buildBluetoothJSON(success, message));
}

void handleBluetoothName() {
  if (!server.hasArg("name")) {
    server.send(400, "application/json", buildBluetoothJSON(false, String(T().bluetooth_error)));
    return;
  }

  String candidate = sanitizeBluetoothName(server.arg("name"));
  if (candidate.length() == 0) {
    server.send(400, "application/json", buildBluetoothJSON(false, String(T().bluetooth_error)));
    return;
  }

  bool supported = bluetoothCapable;
  bool wasActive = supported && bluetoothEnabled;
  bool success = true;

  if (wasActive) {
    stopBluetooth();
  }

  bluetoothDeviceName = candidate;

  if (wasActive) {
    success = startBluetooth();
  }

  String message;
  if (!supported) {
    message = String(T().bluetooth_not_supported);
    success = false;
  } else {
    message = success ? String(T().bluetooth_updated) : String(T().bluetooth_error);
  }

  server.send(200, "application/json", buildBluetoothJSON(success, message));
}

void handleBluetoothReset() {
  ensureBluetoothName();
  bool supported = bluetoothCapable;
  bool wasActive = supported && bluetoothEnabled;
  bool success = true;

  if (wasActive) {
    stopBluetooth();
  }

  bluetoothDeviceName = defaultBluetoothName.length() > 0 ? defaultBluetoothName : String("ESP32 Diagnostic");

  if (wasActive) {
    success = startBluetooth();
  }

  String message;
  if (!supported) {
    message = String(T().bluetooth_not_supported);
    success = false;
  } else {
    message = success ? String(T().bluetooth_reset_done) : String(T().bluetooth_error);
  }

  server.send(200, "application/json", buildBluetoothJSON(success, message));
}

// ========== INTERFACE WEB PRINCIPALE MULTILINGUE ==========
void handleRoot() {
  collectDiagnosticInfo();
  collectDetailedMemory();

  server.setContentLength(CONTENT_LENGTH_UNKNOWN);
  server.send(200, "text/html; charset=utf-8", "");
  
  // CHUNK 1: HEAD + CSS
  String chunk = "<!DOCTYPE html><html lang='";
  chunk += (currentLanguage == LANG_FR) ? "fr" : "en";
  chunk += "'><head>";
  chunk += "<meta charset='UTF-8'><meta name='viewport' content='width=device-width, initial-scale=1.0'>";
  chunk += "<title>" + String(T().title) + " " + String(T().version) + String(DIAGNOSTIC_VERSION) + "</title>";


  chunk += "<style>";
  chunk += F(R"rawliteral(
:root{
  --gradient:linear-gradient(135deg,#312e81 0%,#7c3aed 100%);
  --bg-primary:#0f172a;
  --bg-surface:rgba(15,23,42,0.86);
  --bg-surface-alt:rgba(30,41,59,0.92);
  --border-glow:rgba(99,102,241,0.35);
  --border-muted:rgba(148,163,184,0.22);
  --text-primary:#f8fafc;
  --text-muted:#94a3b8;
  --accent:#38bdf8;
  --success:#22c55e;
  --warning:#facc15;
  --danger:#f87171;
  --radius:18px;
  --transition:all .25s ease;
}
*,*::before,*::after{box-sizing:border-box;margin:0;padding:0;}
body{
  font-family:'Inter',-apple-system,'Segoe UI',sans-serif;
  background:var(--gradient);
  color:var(--text-primary);
  min-height:100vh;
  padding:20px;
}
a{color:inherit;}
.app-shell{
  display:flex;
  flex-direction:column;
  gap:24px;
  min-height:calc(100vh - 48px);
}
.nav-wrapper{
  position:sticky;
  top:20px;
  z-index:95;
}
.nav-wrapper .masthead-card{
  background:var(--bg-surface);
  border-radius:var(--radius);
  box-shadow:0 25px 60px rgba(15,23,42,.45);
  border:1px solid var(--border-glow);
  display:flex;
  flex-direction:column;
  overflow:hidden;
  transition:var(--transition);
}
.nav-wrapper.is-sticky .masthead-card{
  box-shadow:0 30px 60px rgba(15,23,42,.6);
  border-color:rgba(148,163,184,0.28);
}
.app-header{
  padding:18px 24px;
  display:flex;
  flex-direction:column;
  gap:14px;
  background:transparent;
  border:none;
  box-shadow:none;
}
.masthead-divider{
  height:1px;
  background:rgba(148,163,184,0.18);
  margin:0 24px;
}
.masthead-nav{
  padding:16px 24px 12px;
  display:flex;
  flex-direction:column;
  gap:12px;
  background:rgba(148,163,184,0.04);
}
.inline-strip{
  border-top:1px solid rgba(148,163,184,0.18);
  background:rgba(148,163,184,0.08);
  padding:10px 24px;
  display:flex;
  align-items:center;
  justify-content:center;
}
.header-meta{
  display:flex;
  flex-wrap:wrap;
  justify-content:space-between;
  gap:16px;
  align-items:flex-start;
}
.branding{
  display:flex;
  flex-direction:column;
  gap:6px;
}
.branding .subtitle{
  text-transform:uppercase;
  letter-spacing:.28em;
  font-size:.7rem;
  color:var(--accent);
}
.branding h1{
  font-size:2rem;
  margin:0;
  line-height:1.2;
  display:flex;
  flex-wrap:wrap;
  align-items:center;
  gap:8px;
}
.branding .version-tag{
  font-size:1rem;
  font-weight:600;
  color:var(--text-muted);
}
.header-actions{
  display:flex;
  align-items:center;
  gap:12px;
  flex-wrap:wrap;
}
.lang-switcher{
  display:flex;
  background:rgba(255,255,255,0.06);
  border-radius:999px;
  padding:3px;
  border:1px solid var(--border-muted);
}
.lang-btn{
  border:none;
  background:transparent;
  color:var(--text-primary);
  font-weight:600;
  padding:6px 14px;
  border-radius:999px;
  cursor:pointer;
  transition:var(--transition);
  font-size:.9rem;
}
.lang-btn.active{
  background:var(--accent);
  color:#0f172a;
  box-shadow:0 8px 20px rgba(56,189,248,.3);
}
.status-chip{
  display:flex;
  align-items:center;
  gap:8px;
  padding:6px 12px;
  border-radius:999px;
  background:rgba(56,189,248,0.12);
  border:1px solid rgba(56,189,248,0.35);
  font-weight:600;
  font-size:.9rem;
}
.status-indicator{
  width:11px;
  height:11px;
  border-radius:50%;
  display:inline-block;
  background:var(--success);
  box-shadow:0 0 16px rgba(34,197,94,.55);
  transition:var(--transition);
}
.status-indicator.status-offline{
  background:var(--danger);
  box-shadow:0 0 16px rgba(248,113,113,.5);
}
.header-status{
  display:flex;
  flex-wrap:wrap;
  gap:12px;
  align-items:center;
}
.status-item{
  display:flex;
  align-items:center;
  gap:8px;
  background:var(--bg-surface-alt);
  border-radius:12px;
  padding:8px 12px;
  border:1px solid var(--border-muted);
  min-height:40px;
}
.status-item .status-label{
  font-size:.72rem;
  text-transform:uppercase;
  letter-spacing:.08em;
  color:var(--text-muted);
  font-weight:600;
}
.status-item .status-value{
  font-weight:600;
  color:var(--text-primary);
  display:flex;
  align-items:center;
  gap:8px;
  flex-wrap:wrap;
}
.status-dot{
  width:10px;
  height:10px;
  border-radius:50%;
  background:var(--success);
  box-shadow:0 0 12px rgba(34,197,94,.45);
  transition:var(--transition);
}
.status-dot.offline{
  background:var(--danger);
  box-shadow:0 0 12px rgba(248,113,113,.45);
}
.status-item a{
  color:var(--accent);
  text-decoration:none;
}
.status-item a:hover{
  text-decoration:underline;
}
.status-item .access-placeholder{
  opacity:.6;
}
.status-item .separator{
  opacity:.45;
}
.status-item.status-access{
  flex:1 1 280px;
}
.app-body{
  display:flex;
  flex-direction:column;
  gap:24px;
  flex:1;
}
.primary-nav{
  display:flex;
  flex-wrap:nowrap;
  gap:8px;
  background:var(--bg-surface);
  border-radius:var(--radius);
  padding:6px 10px;
  border:1px solid var(--border-glow);
  box-shadow:0 20px 48px rgba(15,23,42,.35);
  position:relative;
}
.nav-wrapper.is-sticky .primary-nav{
  box-shadow:0 22px 40px rgba(15,23,42,.55);
  border-color:rgba(148,163,184,0.28);
}
.nav-select{
  display:none;
  width:100%;
}
.nav-dropdown{
  width:100%;
  border-radius:12px;
  padding:10px 14px;
  background:var(--bg-surface);
  border:1px solid var(--border-glow);
  color:var(--text-primary);
  font-weight:600;
  box-shadow:0 18px 32px rgba(15,23,42,.4);
}
.nav-dropdown:focus{
  outline:2px solid var(--accent);
  outline-offset:2px;
}
.sr-only{
  position:absolute;
  width:1px;
  height:1px;
  padding:0;
  margin:-1px;
  overflow:hidden;
  clip:rect(0,0,0,0);
  white-space:nowrap;
  border:0;
}
@media (max-width:900px){
  .primary-nav{display:none;}
  .nav-select{display:block;}
  .nav-wrapper{padding:8px 0 10px;}
}
.nav-link{
  border:none;
  text-align:center;
  padding:6px 10px;
  border-radius:12px;
  color:var(--text-primary);
  background:rgba(148,163,184,0.08);
  font-weight:600;
  cursor:pointer;
  transition:var(--transition);
  display:flex;
  justify-content:center;
  align-items:center;
  gap:6px;
  text-decoration:none;
  flex:0 0 auto;
  min-width:118px;
  font-size:.85rem;
  line-height:1.2;
}
.nav-link .icon{font-size:1rem;}
.nav-link:hover{
  background:rgba(148,163,184,0.16);
  transform:translateY(-2px);
}
.nav-link.active{
  background:linear-gradient(120deg,#38bdf8,#818cf8);
  color:#0b1120;
  box-shadow:0 18px 30px rgba(56,189,248,.35);
}
.app-main{
  background:var(--bg-surface);
  border-radius:var(--radius);
  padding:24px;
  border:1px solid var(--border-muted);
  box-shadow:0 25px 60px rgba(15,23,42,.45);
}
.inline-message{
  min-height:24px;
  margin:0;
  padding:6px 18px;
  border-radius:999px;
  border:1px solid transparent;
  background:transparent;
  color:var(--text-muted);
  font-size:.9rem;
  display:flex;
  align-items:center;
  justify-content:center;
  opacity:0;
  visibility:hidden;
  transition:var(--transition);
}
.inline-strip .inline-message{
  border-color:rgba(148,163,184,0.14);
}
.inline-message.show{
  opacity:1;
  visibility:visible;
  background:rgba(148,163,184,0.14);
  border-color:rgba(148,163,184,0.18);
}
.inline-message.success{
  border-color:rgba(34,197,94,.45);
  background:rgba(34,197,94,.12);
  color:#bbf7d0;
}
.inline-message.error{
  border-color:rgba(248,113,113,.45);
  background:rgba(248,113,113,.12);
  color:#fecaca;
}
.warning-callout{
  border-radius:12px;
  border:1px solid rgba(250,204,21,0.35);
  background:rgba(250,204,21,0.12);
  color:#facc15;
  padding:12px 16px;
  margin:0 0 18px 0;
  font-size:.9rem;
  line-height:1.4;
}
.tab-container{
  display:flex;
  flex-direction:column;
  gap:24px;
}
.tab-content{display:none;}
.tab-content.active{display:block;}
.tab-content:target{display:block;}
.section{
  background:var(--bg-surface-alt);
  border-radius:var(--radius);
  padding:24px;
  border:1px solid var(--border-muted);
  box-shadow:0 12px 40px rgba(15,23,42,.35);
  margin-bottom:12px;
}
.section h2{
  display:flex;
  align-items:center;
  gap:12px;
  font-size:1.4rem;
  margin-bottom:12px;
  color:var(--accent);
}
.section-description{
  margin:-4px 0 18px;
  color:var(--text-muted);
  font-size:.92rem;
}
.section h3{
  font-size:1.15rem;
  margin:18px 0 12px;
  color:var(--text-primary);
}
.info-grid{
  display:grid;
  gap:16px;
  grid-template-columns:repeat(auto-fit,minmax(220px,1fr));
}
.info-item{
  background:rgba(148,163,184,0.12);
  padding:16px;
  border-radius:14px;
  border:1px solid rgba(148,163,184,0.18);
}
.info-label{
  font-size:.8rem;
  letter-spacing:.08em;
  text-transform:uppercase;
  color:var(--text-muted);
  margin-bottom:6px;
}
.info-value{
  font-size:1.05rem;
  font-weight:600;
}
.status-live{
  border-radius:12px;
  padding:12px;
  text-align:center;
  font-weight:600;
  background:rgba(148,163,184,0.12);
  border:1px solid rgba(148,163,184,0.18);
  color:var(--text-muted);
  margin-top:16px;
}
.status-live.success{
  color:#bbf7d0;
  background:rgba(34,197,94,0.12);
  border-color:rgba(34,197,94,0.45);
}
.status-live.error{
  color:#fecaca;
  background:rgba(248,113,113,0.12);
  border-color:rgba(248,113,113,0.45);
}
.btn{
  border:none;
  border-radius:12px;
  padding:12px 18px;
  font-weight:600;
  cursor:pointer;
  transition:var(--transition);
  display:inline-flex;
  align-items:center;
  gap:8px;
  color:#0b1120;
  background:rgba(148,163,184,0.2);
  margin:6px 4px 0 0;
}
.btn:hover{
  transform:translateY(-2px);
  box-shadow:0 12px 24px rgba(56,189,248,.2);
}
.btn-primary{background:linear-gradient(120deg,#38bdf8,#818cf8);}
.btn-success{background:linear-gradient(120deg,#4ade80,#22c55e);}
.btn-info{background:linear-gradient(120deg,#22d3ee,#38bdf8);}
.btn-danger{background:linear-gradient(120deg,#f87171,#ef4444);}
.btn-warning{background:linear-gradient(120deg,#fbbf24,#f97316);}
.btn-secondary{background:rgba(148,163,184,0.25);color:var(--text-primary);}
.progress-bar{
  background:rgba(148,163,184,0.2);
  border-radius:999px;
  height:16px;
  overflow:hidden;
  margin-top:12px;
}
.progress-fill{
  height:100%;
  border-radius:999px;
  background:linear-gradient(120deg,#38bdf8,#818cf8);
  text-align:center;
  font-size:.75rem;
  font-weight:600;
  color:#0b1120;
}
.gpio-grid{
  display:grid;
  grid-template-columns:repeat(auto-fit,minmax(90px,1fr));
  gap:12px;
  margin-top:18px;
}
.gpio-item{
  border-radius:12px;
  padding:14px;
  text-align:center;
  font-weight:600;
  background:rgba(148,163,184,0.18);
  border:1px solid rgba(148,163,184,0.28);
}
.gpio-ok{
  background:rgba(34,197,94,0.18);
  border-color:rgba(34,197,94,0.35);
}
.gpio-fail{
  background:rgba(248,113,113,0.18);
  border-color:rgba(248,113,113,0.35);
}
.wifi-list{
  display:grid;
  gap:12px;
  margin-top:18px;
}
.wifi-item{
  padding:16px;
  border-radius:14px;
  background:rgba(148,163,184,0.12);
  border:1px solid rgba(148,163,184,0.2);
  display:flex;
  justify-content:space-between;
  align-items:center;
  gap:16px;
  flex-wrap:wrap;
}
.card{
  background:rgba(148,163,184,0.12);
  border-radius:14px;
  padding:24px;
  border:1px solid rgba(148,163,184,0.2);
  text-align:center;
}
.footer{
  text-align:center;
  color:var(--text-muted);
  font-size:.85rem;
  margin-top:24px;
}
.update-indicator{
  position:fixed;
  right:24px;
  bottom:24px;
  background:linear-gradient(120deg,#38bdf8,#818cf8);
  color:#0b1120;
  padding:14px 20px;
  border-radius:14px;
  box-shadow:0 20px 40px rgba(56,189,248,.45);
  opacity:0;
  pointer-events:none;
  transition:var(--transition);
  font-weight:600;
}
.update-indicator.show{opacity:1;}
@media(max-width:1100px){
  body{padding:18px;}
  .primary-nav{padding:8px 10px;}
  .nav-link{min-width:140px;}
}
@media(max-width:640px){
  .app-header{padding:18px;}
  .branding h1{font-size:1.6rem;}
  .header-status{flex-direction:column;align-items:stretch;}
  .status-item{width:100%;}
  .nav-link{padding:10px 12px;}
  .app-main{padding:18px;}
}
)rawliteral");
  chunk += "</style></head><body>";
  server.sendContent(chunk);
  

  const char* updateLabel = T().updating;
  const char* connectionLabel = T().online;
  bool wifiConnected = (WiFi.status() == WL_CONNECTED);
  String wifiSummary = String(T().not_detected);
  if (diagnosticData.wifiSSID.length() > 0 && wifiConnected) {
    wifiSummary = diagnosticData.wifiSSID + " (" + String(diagnosticData.wifiRSSI) + " dBm)";
  }
  String wifiSummaryEscaped = htmlEscape(wifiSummary);
  String wifiOfflineEscaped = htmlEscape(String(T().not_detected));
  String wifiValueDisplay = wifiConnected ? wifiSummaryEscaped : wifiOfflineEscaped;
  String wifiDotClass = wifiConnected ? "status-dot" : "status-dot offline";
  String bluetoothSummary = getBluetoothSummaryLabel();
  String bluetoothSummaryEscaped = htmlEscape(bluetoothSummary);
  String bluetoothStatusText = getBluetoothStateLabel();
  String bluetoothStatusEscaped = htmlEscape(bluetoothStatusText);
  bool bluetoothActive = bluetoothCapable && bluetoothEnabled;
  String bluetoothDotClass = bluetoothActive ? "status-dot" : "status-dot offline";
  String bluetoothSupportedAttr = bluetoothCapable ? "true" : "false";
  String bluetoothDisabledText = htmlEscape(String(T().bluetooth_disabled));
  String bluetoothUnsupportedText = htmlEscape(String(T().bluetooth_not_supported));
  String bluetoothOfflineText = bluetoothCapable ? bluetoothDisabledText : bluetoothUnsupportedText;
  String bluetoothOnlineCombined = bluetoothStatusEscaped + " • " + bluetoothSummaryEscaped;
  String bluetoothDisplayText = bluetoothActive ? bluetoothOnlineCombined : bluetoothOfflineText;
  String bluetoothNameDisplay = diagnosticData.bluetoothName.length() > 0 ? diagnosticData.bluetoothName : defaultBluetoothName;
  String bluetoothNameEscaped = htmlEscape(bluetoothNameDisplay);
  bool bluetoothClientNow = false;
  uint32_t bluetoothNotifyDisplay = 0;
#if BLE_STACK_SUPPORTED
  bluetoothClientNow = bluetoothClientConnected;
  bluetoothNotifyDisplay = bluetoothNotifyCounter;
#endif
  bool bluetoothAdvertisingActive = bluetoothCapable && bluetoothEnabled && (bluetoothAdvertising
#if BLE_STACK_SUPPORTED
      || bluetoothClientNow
#endif
  );
  bool bluetoothConnectionActive = bluetoothCapable && bluetoothEnabled && bluetoothClientNow;
  String bluetoothSupportYes = htmlEscape(String(T().bluetooth_support_yes));
  String bluetoothSupportNo = htmlEscape(String(T().bluetooth_support_no));
  String bluetoothSupportValue = bluetoothCapable ? bluetoothSupportYes : bluetoothSupportNo;
  String bluetoothSupportAttr = bluetoothCapable ? "true" : "false";
  String bluetoothAdvertisingOn = htmlEscape(String(T().bluetooth_advertising));
  String bluetoothAdvertisingOff = htmlEscape(String(T().bluetooth_not_advertising));
  String bluetoothAdvertisingValue = bluetoothCapable ? (bluetoothAdvertisingActive ? bluetoothAdvertisingOn : bluetoothAdvertisingOff)
                                                      : htmlEscape(String(T().bluetooth_not_supported));
  String bluetoothAdvertisingAttr = bluetoothAdvertisingActive ? "true" : "false";
  String bluetoothConnectedLabel = htmlEscape(String(T().bluetooth_client_connected));
  String bluetoothDisconnectedLabel = htmlEscape(String(T().bluetooth_client_disconnected));
  String bluetoothConnectionValue = bluetoothConnectionActive ? bluetoothConnectedLabel : bluetoothDisconnectedLabel;
  String bluetoothConnectionStateAttr;
  if (!bluetoothCapable) {
    bluetoothConnectionStateAttr = "unsupported";
  } else if (!bluetoothEnabled) {
    bluetoothConnectionStateAttr = "disabled";
  } else if (bluetoothConnectionActive) {
    bluetoothConnectionStateAttr = "connected";
  } else {
    bluetoothConnectionStateAttr = "idle";
  }
  String bluetoothNotificationsValue = htmlEscape(String((unsigned long)bluetoothNotifyDisplay));
  String bluetoothDisableAttr = bluetoothCapable ? "" : " disabled";
  String bluetoothFeedbackText = bluetoothCapable ? "" : String(T().bluetooth_not_supported);
  bool ipAvailable = diagnosticData.ipAddress.length() > 0;
  String accessHost = diagnosticData.ipAddress;
  String accessUrl = ipAvailable ? buildAccessUrl(accessHost) : String(T().not_detected);
  String accessUrlLegacy = ipAvailable ? buildAccessUrl(accessHost, false) : String();
  String accessUrlHref = ipAvailable ? accessUrl : String("#");
  String accessUrlEscaped = htmlEscape(accessUrl);
  String accessUrlLegacyEscaped = htmlEscape(accessUrlLegacy);
  String accessPlaceholder = htmlEscape(String(T().not_detected));
  bool mdnsAvailable = diagnosticData.mdnsAvailable;
  String stableAccessHost = getStableAccessHost();
  String stableAccessUrl = getStableAccessURL();
  String stableAccessLegacy = buildAccessUrl(stableAccessHost, false);
  String stableAccessEscaped = htmlEscape(stableAccessUrl);
  String stableAccessLegacyEscaped = htmlEscape(stableAccessLegacy);
  String stableAccessHostEscaped = htmlEscape(stableAccessHost);
  String accessHostEscaped = htmlEscape(accessHost);
  String stableAccessHref = mdnsAvailable ? stableAccessUrl : String("#");
  String mdnsStateAttr = mdnsAvailable ? "true" : "false";
  String secureSchemeEscaped = htmlEscape(String(DIAGNOSTIC_SECURE_SCHEME));
  String legacySchemeEscaped = htmlEscape(String(DIAGNOSTIC_LEGACY_SCHEME));

  chunk = "<div class='app-shell'>";
  chunk += "<div id='updateIndicator' class='update-indicator'>";
  chunk += updateLabel;
  chunk += "</div>";
  chunk += "<div class='app-body'>";
  chunk += "<div class='nav-wrapper'>";
  chunk += "<div class='masthead-card'>";
  chunk += "<header class='app-header'>";
  chunk += "<div class='header-meta'>";
  chunk += "<div class='branding'><h1 id='main-title'><span class='title-text' data-i18n='title'>" + String(T().title) + "</span><span class='version-tag'>" + String(T().version) + String(DIAGNOSTIC_VERSION) + "</span></h1></div>";
  chunk += "<div class='header-actions'>";
  chunk += "<div class='lang-switcher' role='group' aria-label='Langue'>";
  chunk += "<button type='button' class='lang-btn " + String(currentLanguage == LANG_FR ? "active" : "") + "' data-lang='fr' onclick='changeLang(\"fr\",this)'>FR</button>";
  chunk += "<button type='button' class='lang-btn " + String(currentLanguage == LANG_EN ? "active" : "") + "' data-lang='en' onclick='changeLang(\"en\",this)'>EN</button>";
  chunk += "</div>";
  chunk += "<div class='status-chip'><span class='status-indicator status-online' id='statusIndicator'></span><span id='connectionLabel'>";
  chunk += connectionLabel;
  chunk += "</span></div>";
  chunk += "</div></div>";
  chunk += "<div class='header-status'>";
  chunk += "<div class='status-item'><span class='status-label' data-i18n='wifi_connection'>" + String(T().wifi_connection) + "</span>";
  chunk += "<span id='wifiStatusDot' class='" + wifiDotClass + "' aria-hidden='true'></span>";
  chunk += "<span class='status-value' id='wifiStatusValue' data-online='" + wifiSummaryEscaped + "' data-offline='" + wifiOfflineEscaped + "'>" + wifiValueDisplay + "</span></div>";
  chunk += "<div class='status-item'><span class='status-label' data-i18n='bluetooth_section'>" + String(T().bluetooth_section) + "</span>";
  chunk += "<span id='bluetoothStatusDot' class='" + bluetoothDotClass + "' aria-hidden='true'></span>";
  chunk += "<span class='status-value' id='bluetoothSummary' data-online='" + bluetoothOnlineCombined + "' data-offline='" + bluetoothOfflineText + "' data-disabled='" + bluetoothDisabledText + "' data-unsupported='" + bluetoothUnsupportedText + "' data-supported='" + bluetoothSupportedAttr + "'>" + bluetoothDisplayText + "</span></div>";
  chunk += "<div class='status-item status-access'><span class='status-label' data-i18n='access'>" + String(T().access) + "</span>";
  chunk += "<span class='status-value' data-mdns='" + mdnsStateAttr + "'>";
  if (mdnsAvailable) {
    chunk += "<a href='" + stableAccessHref + "' target='_blank' rel='noopener' data-access-host='" + stableAccessHostEscaped +
             "' data-secure='" + secureSchemeEscaped + "' data-legacy='" + legacySchemeEscaped + "' data-access-label='" +
             stableAccessHostEscaped + "' data-legacy-label='" + stableAccessLegacyEscaped + "'>" + stableAccessEscaped + "</a>";
  } else {
    chunk += "<span>" + stableAccessEscaped + "</span>";
  }
  chunk += "<span class='separator'>•</span>";
  if (ipAvailable) {
    chunk += "<a href='" + accessUrlHref + "' target='_blank' rel='noopener' data-access-host='" + accessHostEscaped +
             "' data-secure='" + secureSchemeEscaped + "' data-legacy='" + legacySchemeEscaped + "' data-access-label='" +
             accessHostEscaped + "' data-legacy-label='" + accessUrlLegacyEscaped + "'>" + accessUrlEscaped + "</a>";
  } else {
    chunk += "<span class='access-placeholder'>" + accessPlaceholder + "</span>";
  }
  chunk += "</span></div>";
  chunk += "</div>";
  chunk += "</header>";
  chunk += "<div class='masthead-divider' role='presentation'></div>";
  chunk += "<div class='masthead-nav'>";
  chunk += "<nav class='primary-nav' data-role='nav'>";
  chunk += "<a href='#overview' class='nav-link active' data-target='overview' onclick=\"return showTab('overview',this);\"><span class='label' data-i18n='nav_overview'>" + String(T().nav_overview) + "</span><span class='icon'>🏠</span></a>";
  chunk += "<a href='#leds' class='nav-link' data-target='leds' onclick=\"return showTab('leds',this);\"><span class='label' data-i18n='nav_leds'>" + String(T().nav_leds) + "</span><span class='icon'>💡</span></a>";
  chunk += "<a href='#screens' class='nav-link' data-target='screens' onclick=\"return showTab('screens',this);\"><span class='label' data-i18n='nav_screens'>" + String(T().nav_screens) + "</span><span class='icon'>🖥️</span></a>";
  chunk += "<a href='#tests' class='nav-link' data-target='tests' onclick=\"return showTab('tests',this);\"><span class='label' data-i18n='nav_tests'>" + String(T().nav_tests) + "</span><span class='icon'>🧪</span></a>";
  chunk += "<a href='#gpio' class='nav-link' data-target='gpio' onclick=\"return showTab('gpio',this);\"><span class='label' data-i18n='nav_gpio'>" + String(T().nav_gpio) + "</span><span class='icon'>🔌</span></a>";
  chunk += "<a href='#wireless' class='nav-link' data-target='wireless' onclick=\"return showTab('wireless',this);\"><span class='label' data-i18n='nav_wireless'>" + String(T().nav_wireless) + "</span><span class='icon'>📡</span></a>";
  chunk += "<a href='#benchmark' class='nav-link' data-target='benchmark' onclick=\"return showTab('benchmark',this);\"><span class='label' data-i18n='nav_benchmark'>" + String(T().nav_benchmark) + "</span><span class='icon'>⚡</span></a>";
  chunk += "<a href='#export' class='nav-link' data-target='export' onclick=\"return showTab('export',this);\"><span class='label' data-i18n='nav_export'>" + String(T().nav_export) + "</span><span class='icon'>💾</span></a>";
  chunk += "</nav>";
  chunk += "<div class='nav-select'><label for='navSelector' class='sr-only' data-i18n='nav_select_label'>" + String(T().nav_select_label) + "</label>";
  chunk += "<select id='navSelector' class='nav-dropdown' aria-label='" + String(T().nav_select_label) + "'>";
  chunk += "<option value='overview' data-i18n='nav_overview' selected>" + String(T().nav_overview) + "</option>";
  chunk += "<option value='leds' data-i18n='nav_leds'>" + String(T().nav_leds) + "</option>";
  chunk += "<option value='screens' data-i18n='nav_screens'>" + String(T().nav_screens) + "</option>";
  chunk += "<option value='tests' data-i18n='nav_tests'>" + String(T().nav_tests) + "</option>";
  chunk += "<option value='gpio' data-i18n='nav_gpio'>" + String(T().nav_gpio) + "</option>";
  chunk += "<option value='wireless' data-i18n='nav_wireless'>" + String(T().nav_wireless) + "</option>";
  chunk += "<option value='benchmark' data-i18n='nav_benchmark'>" + String(T().nav_benchmark) + "</option>";
  chunk += "<option value='export' data-i18n='nav_export'>" + String(T().nav_export) + "</option>";
  chunk += "</select></div>";
  chunk += "</div>";
  chunk += "<div class='inline-strip'><div id='inlineMessage' class='inline-message' role='status' aria-live='polite'></div></div>";
  chunk += "</div></div>";
  chunk += "<main class='app-main'>";
  chunk += "<div id='tabContainer' class='tab-container'>";
  server.sendContent(chunk);
  // CHUNK 3: OVERVIEW TAB - VERSION UNIQUE COMPLÈTE
  chunk = "<div id='overview' class='tab-content active'>";
  
  // Chip Info
  chunk += "<div class='section'><h2 data-i18n='chip_info'>" + String(T().chip_info) + "</h2><div class='info-grid'>";
  String modelValue = diagnosticData.chipModel + " <span data-i18n='revision'>" + String(T().revision) + "</span> " + diagnosticData.chipRevision;
  appendInfoItem(chunk, "full_model", T().full_model, modelValue);
  String cpuValue = String(diagnosticData.cpuCores) + " @ " + String(diagnosticData.cpuFreqMHz) + " MHz";
  appendInfoItem(chunk, "cpu_cores", T().cpu_cores, cpuValue);
  appendInfoItem(chunk, "mac_wifi", T().mac_wifi, diagnosticData.macAddress);
  const char* resetKey = getResetReasonKey();
  String resetAttrs;
  if (resetKey != nullptr) {
    resetAttrs = "data-i18n='";
    resetAttrs += resetKey;
    resetAttrs += "'";
  }
  appendInfoItem(chunk, "last_reset", T().last_reset, getResetReason(), resetAttrs);
  appendInfoItem(chunk, "chip_features", T().chip_features, getChipFeatures());
  appendInfoItem(chunk, "sdk_version", T().sdk_version, diagnosticData.sdkVersion);
  appendInfoItem(chunk, "idf_version", T().idf_version, diagnosticData.idfVersion);

  unsigned long seconds = diagnosticData.uptime / 1000;
  unsigned long minutes = seconds / 60;
  unsigned long hours = minutes / 60;
  unsigned long days = hours / 24;
  String uptimeAttrs = "id='uptime-display' data-days='" + String(days) + "' data-hours='" + String(hours % 24) + "' data-minutes='" + String(minutes % 60) + "'";
  appendInfoItem(chunk, "uptime", T().uptime, formatUptime(days, hours % 24, minutes % 60), uptimeAttrs);

  if (diagnosticData.temperature != -999) {
    String temperatureValue = String(diagnosticData.temperature, 1) + " °C";
    appendInfoItem(chunk, "cpu_temp", T().cpu_temp, temperatureValue);
  }
  chunk += "</div></div>";
  server.sendContent(chunk);
  
  // Memory - Flash
  chunk = "<div class='section'><h2 data-i18n='memory_details'>" + String(T().memory_details) + "</h2>";
  chunk += "<h3 data-i18n='flash_memory'>" + String(T().flash_memory) + "</h3><div class='info-grid'>";
  String flashReal = String(detailedMemory.flashSizeReal / 1048576.0, 2) + " MB";
  appendInfoItem(chunk, "real_size", T().real_size, flashReal);
  String flashConfigured = String(detailedMemory.flashSizeChip / 1048576.0, 2) + " MB";
  appendInfoItem(chunk, "configured_ide", T().configured_ide, flashConfigured);

  bool flashMatch = (detailedMemory.flashSizeReal == detailedMemory.flashSizeChip);
  String configValue = flashMatch ? "<span class='badge badge-success' data-i18n='correct'>" + String(T().correct)
                                  : "<span class='badge badge-warning' data-i18n='to_fix'>" + String(T().to_fix);
  configValue += "</span>";
  appendInfoItem(chunk, "configuration", T().configuration, configValue);
  appendInfoItem(chunk, "flash_type", T().flash_type, getFlashType());
  appendInfoItem(chunk, "flash_speed", T().flash_speed, getFlashSpeed());
  chunk += "</div>";
  server.sendContent(chunk);
  
  // Memory - PSRAM
  chunk = "<h3 data-i18n='psram_external'>" + String(T().psram_external) + "</h3><div class='info-grid'>";
  if (detailedMemory.psramAvailable) {
    String statusValue = "<span class='badge badge-success' data-i18n='detected_active'>" + String(T().detected_active) + "</span>";
    appendInfoItem(chunk, "hardware_status", T().hardware_status, statusValue);
    String psramTotal = String(detailedMemory.psramTotal / 1048576.0, 2) + " MB";
    appendInfoItem(chunk, "total_size", T().total_size, psramTotal);
    String psramFree = String(detailedMemory.psramFree / 1048576.0, 2) + " MB";
    appendInfoItem(chunk, "free", T().free, psramFree);
    String psramUsed = String(detailedMemory.psramUsed / 1048576.0, 2) + " MB";
    appendInfoItem(chunk, "used", T().used, psramUsed);
  } else if (detailedMemory.psramBoardSupported) {
    String statusValue = "<span class='badge badge-warning' data-i18n='supported_not_enabled'>" + String(T().supported_not_enabled) + "</span>";
    appendInfoItem(chunk, "hardware_status", T().hardware_status, statusValue);
    String psramHint = String(T().enable_psram_hint);
    psramHint.replace("%TYPE%", detailedMemory.psramType ? detailedMemory.psramType : "PSRAM");
    String psramHintAttrs = "data-i18n='enable_psram_hint' data-psram-type='" + htmlEscape(String(detailedMemory.psramType ? detailedMemory.psramType : "PSRAM")) + "'";
    appendInfoItem(chunk, "ide_config", T().ide_config, psramHint, psramHintAttrs, String(F("style='grid-column:1/-1'")));
  } else {
    String statusValue = "<span class='badge badge-danger' data-i18n='not_detected'>" + String(T().not_detected) + "</span>";
    appendInfoItem(chunk, "hardware_status", T().hardware_status, statusValue);
  }
  chunk += "</div>";
  server.sendContent(chunk);
  
  // Memory - SRAM
  chunk = "<h3 data-i18n='internal_sram'>" + String(T().internal_sram) + "</h3><div class='info-grid'>";
  String sramTotal = String(detailedMemory.sramTotal / 1024.0, 2) + " KB";
  appendInfoItem(chunk, "total_size", T().total_size, sramTotal);
  String sramFree = String(detailedMemory.sramFree / 1024.0, 2) + " KB";
  appendInfoItem(chunk, "free", T().free, sramFree);
  String sramUsed = String(detailedMemory.sramUsed / 1024.0, 2) + " KB";
  appendInfoItem(chunk, "used", T().used, sramUsed);
  String fragmentation = String(detailedMemory.fragmentationPercent, 1) + "%";
  appendInfoItem(chunk, "memory_fragmentation", T().memory_fragmentation, fragmentation);
  chunk += "</div>";
  chunk += "<div style='text-align:center;margin-top:15px'><button class='btn btn-info' data-i18n='refresh_memory' onclick='location.reload()'>" + String(T().refresh_memory) + "</button></div></div>";
  server.sendContent(chunk);
  
  // WiFi
  chunk = "<div class='section'><h2 data-i18n='wifi_connection'>" + String(T().wifi_connection) + "</h2><div class='info-grid'>";
  appendInfoItem(chunk, "connected_ssid", T().connected_ssid, diagnosticData.wifiSSID);
  String wifiRssi = String(diagnosticData.wifiRSSI) + " dBm";
  appendInfoItem(chunk, "signal_power", T().signal_power, wifiRssi);
  const char* wifiQualityKey = getWiFiSignalQualityKey();
  String wifiQualityAttrs;
  if (wifiQualityKey != nullptr) {
    wifiQualityAttrs = "data-i18n='";
    wifiQualityAttrs += wifiQualityKey;
    wifiQualityAttrs += "'";
  }
  appendInfoItem(chunk, "signal_quality", T().signal_quality, getWiFiSignalQuality(), wifiQualityAttrs);
  appendInfoItem(chunk, "ip_address", T().ip_address, diagnosticData.ipAddress);
  appendInfoItem(chunk, "subnet_mask", T().subnet_mask, WiFi.subnetMask().toString());
  appendInfoItem(chunk, "gateway", T().gateway, WiFi.gatewayIP().toString());
  appendInfoItem(chunk, "bluetooth_section", T().bluetooth_section, bluetoothSummaryEscaped);
  appendInfoItem(chunk, "bluetooth_mac", T().bluetooth_mac, diagnosticData.bluetoothAddress);
  chunk += "</div></div>";
  server.sendContent(chunk);

  chunk = "<div class='section'><h2 data-i18n='bluetooth_section'>" + String(T().bluetooth_section) + "</h2><div class='info-grid'>";
  appendInfoItem(chunk, "bluetooth_status", T().bluetooth_status, bluetoothStatusEscaped);
  appendInfoItem(chunk, "bluetooth_name", T().bluetooth_name, bluetoothNameEscaped);
  appendInfoItem(chunk, "bluetooth_mac", T().bluetooth_mac, diagnosticData.bluetoothAddress);
  chunk += "</div></div>";
  server.sendContent(chunk);

  // GPIO et I2C
  chunk = "<div class='section'><h2 data-i18n='gpio_interfaces'>" + String(T().gpio_interfaces) + "</h2><div class='info-grid'>";
  String totalGPIOValue = String(diagnosticData.totalGPIO) + " <span data-i18n='pins'>" + String(T().pins) + "</span>";
  appendInfoItem(chunk, "total_gpio", T().total_gpio, totalGPIOValue);
  if (ENABLE_I2C_SCAN) {
    String i2cCountValue = String(diagnosticData.i2cCount) + " <span data-i18n='devices'>" + String(T().devices) + "</span>";
    appendInfoItem(chunk, "i2c_peripherals", T().i2c_peripherals, i2cCountValue);
    if (diagnosticData.i2cCount > 0) {
      appendInfoItem(chunk, "detected_addresses", T().detected_addresses, diagnosticData.i2cDevices, String(), String(F("style='grid-column:1/-1'")));
    }
  }
  chunk += "</div></div>";
  chunk += "</div>"; // Fermeture div overview
  server.sendContent(chunk);

  // CHUNK 4: TAB LEDs
  chunk = "<div id='leds' class='tab-content'>";
  chunk += "<div class='section'><h2 data-i18n='builtin_led'>" + String(T().builtin_led) + "</h2><div class='info-grid'>";
  String builtinPinValue = "GPIO " + String(BUILTIN_LED_PIN);
  appendInfoItem(chunk, nullptr, T().gpio, builtinPinValue);
  appendInfoItem(chunk, nullptr, T().status, builtinLedTestResult, String(F("id='builtin-led-status'")));
  chunk += "<div class='info-item' style='grid-column:1/-1;text-align:center'>";
  chunk += "<input type='number' id='ledGPIO' value='" + String(BUILTIN_LED_PIN) + "' min='0' max='48' style='width:80px'>";
  chunk += "<button class='btn btn-info' onclick='configBuiltinLED()'>" + String(T().config) + "</button>";
  chunk += "<button class='btn btn-primary' onclick='testBuiltinLED()'>" + String(T().test) + "</button>";
  chunk += "<button class='btn btn-success' onclick='ledBlink()'>" + String(T().blink) + "</button>";
  chunk += "<button class='btn btn-info' onclick='ledFade()'>" + String(T().fade) + "</button>";
  chunk += "<button class='btn btn-danger' onclick='ledOff()'>" + String(T().off) + "</button>";
  chunk += "</div></div></div>";
  
  chunk += "<div class='section'><h2 data-i18n='neopixel'>" + String(T().neopixel) + "</h2><div class='info-grid'>";
  String neoPinValue = "GPIO " + String(LED_PIN);
  appendInfoItem(chunk, nullptr, T().gpio, neoPinValue);
  appendInfoItem(chunk, nullptr, T().status, neopixelTestResult, String(F("id='neopixel-status'")));
  chunk += "<div class='info-item' style='grid-column:1/-1;text-align:center'>";
  chunk += "<input type='number' id='neoGPIO' value='" + String(LED_PIN) + "' min='0' max='48' style='width:80px'>";
  chunk += "<input type='number' id='neoCount' value='" + String(LED_COUNT) + "' min='1' max='100' style='width:80px'>";
  chunk += "<button class='btn btn-info' onclick='configNeoPixel()'>" + String(T().config) + "</button>";
  chunk += "<button class='btn btn-primary' onclick='testNeoPixel()'>" + String(T().test) + "</button>";
  chunk += "<button class='btn btn-primary' onclick='neoPattern(\"rainbow\")'>" + String(T().rainbow) + "</button>";
  chunk += "<input type='color' id='neoColor' value='#ff0000' style='height:48px'>";
  chunk += "<button class='btn btn-primary' onclick='neoCustomColor()'>" + String(T().color) + "</button>";
  chunk += "<button class='btn btn-danger' onclick='neoPattern(\"off\")'>" + String(T().off) + "</button>";
  chunk += "</div></div></div></div>";
  server.sendContent(chunk);
  
  // CHUNK 5: TAB Screens
  chunk = "<div id='screens' class='tab-content'>";
  chunk += "<div class='section'><h2 data-i18n='oled_screen'>" + String(T().oled_screen) + "</h2><div class='info-grid'>";
  appendInfoItem(chunk, nullptr, T().status, oledTestResult, String(F("id='oled-status'")));
  String oledPinsValue = "SDA:" + String(I2C_SDA) + " SCL:" + String(I2C_SCL);
  appendInfoItem(chunk, nullptr, T().i2c_pins, oledPinsValue, String(F("id='oled-pins'")));
  appendInfoItem(chunk, nullptr, T().rotation, String(oledRotation), String(F("id='oled-rotation-display'")));
  chunk += "<div class='info-item' style='grid-column:1/-1;text-align:center'>";
  chunk += "SDA: <input type='number' id='oledSDA' value='" + String(I2C_SDA) + "' min='0' max='48' style='width:70px'> ";
  chunk += "SCL: <input type='number' id='oledSCL' value='" + String(I2C_SCL) + "' min='0' max='48' style='width:70px'> ";
  chunk += String(T().rotation) + ": <select id='oledRotation' style='padding:10px;border:2px solid #ddd;border-radius:5px'>";
  for (int rot = 0; rot < 4; rot++) {
    chunk += "<option value='" + String(rot) + "'" + String(oledRotation == rot ? " selected" : "") + ">" + String(rot) + "</option>";
  }
  chunk += "</select> ";
  chunk += "<button class='btn btn-info' data-i18n='apply_redetect' data-i18n-prefix='🔄 ' onclick='configOLED()'>" + String(T().apply_redetect) + "</button>";
  if (oledAvailable) {
    chunk += "<div style='margin-top:15px'>";
    chunk += "<button class='btn btn-primary' data-i18n='full_test' data-i18n-prefix='🧪 ' data-i18n-suffix=' (25s)' onclick='testOLED()'>" + String(T().full_test) + "</button>";
    chunk += "</div>";
    chunk += "<div class='oled-step-grid' style='margin-top:15px;display:grid;grid-template-columns:repeat(auto-fit,minmax(180px,1fr));gap:10px'>";
    chunk += "<button class='btn btn-secondary' data-i18n='oled_step_welcome' data-i18n-prefix='🏁 ' onclick='oledStep(\"welcome\")'>" + String(T().oled_step_welcome) + "</button>";
    chunk += "<button class='btn btn-secondary' data-i18n='oled_step_big_text' data-i18n-prefix='🔠 ' onclick='oledStep(\"big_text\")'>" + String(T().oled_step_big_text) + "</button>";
    chunk += "<button class='btn btn-secondary' data-i18n='oled_step_text_sizes' data-i18n-prefix='🔤 ' onclick='oledStep(\"text_sizes\")'>" + String(T().oled_step_text_sizes) + "</button>";
    chunk += "<button class='btn btn-secondary' data-i18n='oled_step_shapes' data-i18n-prefix='🟦 ' onclick='oledStep(\"shapes\")'>" + String(T().oled_step_shapes) + "</button>";
    chunk += "<button class='btn btn-secondary' data-i18n='oled_step_horizontal_lines' data-i18n-prefix='📏 ' onclick='oledStep(\"horizontal_lines\")'>" + String(T().oled_step_horizontal_lines) + "</button>";
    chunk += "<button class='btn btn-secondary' data-i18n='oled_step_diagonals' data-i18n-prefix='📐 ' onclick='oledStep(\"diagonals\")'>" + String(T().oled_step_diagonals) + "</button>";
    chunk += "<button class='btn btn-secondary' data-i18n='oled_step_moving_square' data-i18n-prefix='⬜ ' onclick='oledStep(\"moving_square\")'>" + String(T().oled_step_moving_square) + "</button>";
    chunk += "<button class='btn btn-secondary' data-i18n='oled_step_progress_bar' data-i18n-prefix='📊 ' onclick='oledStep(\"progress_bar\")'>" + String(T().oled_step_progress_bar) + "</button>";
    chunk += "<button class='btn btn-secondary' data-i18n='oled_step_scroll_text' data-i18n-prefix='📜 ' onclick='oledStep(\"scroll_text\")'>" + String(T().oled_step_scroll_text) + "</button>";
    chunk += "<button class='btn btn-secondary' data-i18n='oled_step_final_message' data-i18n-prefix='✅ ' onclick='oledStep(\"final_message\")'>" + String(T().oled_step_final_message) + "</button>";
    chunk += "</div>";
    chunk += "<div style='margin-top:15px'>";
    chunk += "<input type='text' id='oledMsg' data-i18n-placeholder='custom_message' placeholder='" + String(T().custom_message) + "' style='width:250px;margin:0 5px'>";
    chunk += "<button class='btn btn-success' data-i18n='show_message' data-i18n-prefix='📤 ' onclick='oledMessage()'>" + String(T().show_message) + "</button>";
    chunk += "</div>";
  }
  chunk += "</div></div></div></div>";
  server.sendContent(chunk);
  
  // CHUNK 6: TAB Tests
  chunk = "<div id='tests' class='tab-content'>";
  chunk += "<div class='section'><h2 data-i18n='i2c_peripherals'>" + String(T().i2c_peripherals) + "</h2><p class='section-description' data-i18n='i2c_desc'>" + String(T().i2c_desc) + "</p>";
  chunk += "<div style='text-align:center;margin:20px 0'>";
  chunk += "<button class='btn btn-primary' data-i18n='rescan_i2c' onclick='scanI2C()'>" + String(T().rescan_i2c) + "</button>";
  chunk += "<div id='i2c-status' class='status-live' data-i18n-key='click_button'>" + String(T().click_button) + "</div>";
  chunk += "</div></div>";

  chunk += "<div class='section'><h2 data-i18n='adc_test'>" + String(T().adc_test) + "</h2><p class='section-description' data-i18n='adc_desc'>" + String(T().adc_desc) + "</p>";
  chunk += "<div style='text-align:center;margin:20px 0'>";
  chunk += "<button class='btn btn-primary' data-i18n='test' onclick='testADC()'>" + String(T().test) + "</button>";
  String adcDefault = String(T().not_tested);
  String adcStatusAttrs = "id='adc-status' class='status-live'";
  if (adcTestResult == adcDefault) {
    adcStatusAttrs += " data-i18n-key='not_tested'";
  }
  chunk += "<div " + adcStatusAttrs + ">" + adcTestResult + "</div>";
  chunk += "</div><div id='adc-results' class='info-grid'></div></div>";

  chunk += "<div class='section'><h2 data-i18n='pwm_test'>" + String(T().pwm_test) + "</h2><p class='section-description' data-i18n='pwm_desc'>" + String(T().pwm_desc) + "</p>";
  chunk += "<div style='text-align:center;margin:20px 0'>";
  chunk += "<button class='btn btn-primary' data-i18n='test' onclick='testPWM()'>" + String(T().test) + "</button>";
  String pwmDefault = String(T().not_tested);
  String pwmStatusAttrs = "id='pwm-status' class='status-live'";
  if (pwmTestResult == pwmDefault) {
    pwmStatusAttrs += " data-i18n-key='not_tested'";
  }
  chunk += "<div " + pwmStatusAttrs + ">" + pwmTestResult + "</div>";
  chunk += "</div></div>";

  chunk += "<div class='section'><h2 data-i18n='spi_bus'>" + String(T().spi_bus) + "</h2><p class='section-description' data-i18n='spi_desc'>" + String(T().spi_desc) + "</p>";
  chunk += "<div style='text-align:center;margin:20px 0'>";
  chunk += "<button class='btn btn-primary' data-i18n='scan' onclick='scanSPI()'>" + String(T().scan) + "</button>";
  bool hasSpiInfo = spiInfo.length() > 0;
  chunk += "<div id='spi-status' class='status-live'";
  if (!hasSpiInfo) {
    chunk += "' data-i18n-key='click_button'";
  } else {
    chunk += "'";
  }
  chunk += ">" + (hasSpiInfo ? spiInfo : String(T().click_button)) + "</div>";
  chunk += "</div></div>";

  chunk += "<div class='section'><h2 data-i18n='flash_partitions'>" + String(T().flash_partitions) + "</h2><p class='section-description' data-i18n='partitions_desc'>" + String(T().partitions_desc) + "</p>";
  chunk += "<div style='text-align:center;margin:20px 0'>";
  chunk += "<button class='btn btn-primary' data-i18n='list_partitions' onclick='listPartitions()'>" + String(T().list_partitions) + "</button>";
  chunk += "</div><div id='partitions-results' style='background:#fff;color:#0b1120;padding:15px;border-radius:10px;font-family:monospace;white-space:pre-wrap;font-size:0.85em'";
  if (partitionsInfo.length() == 0) {
    chunk += "' data-i18n-key='click_button'";
  } else {
    chunk += "'";
  }
  chunk += ">";
  chunk += partitionsInfo.length() > 0 ? partitionsInfo : String(T().click_button);
  chunk += "</div></div>";

  chunk += "<div class='section'><h2 data-i18n='memory_stress'>" + String(T().memory_stress) + "</h2><p class='section-description' data-i18n='stress_desc'>" + String(T().stress_desc) + "</p>";
  chunk += "<div style='text-align:center;margin:20px 0'>";
  chunk += "<button class='btn btn-danger' data-i18n='start_stress' onclick='stressTest()'>" + String(T().start_stress) + "</button>";
  String stressDefault = String(T().not_tested);
  String stressStatusAttrs = "id='stress-status' class='status-live'";
  if (stressTestResult == stressDefault) {
    stressStatusAttrs += " data-i18n-key='not_tested'";
  }
  chunk += "<div " + stressStatusAttrs + ">" + stressTestResult + "</div>";
  chunk += "</div></div></div>";
  server.sendContent(chunk);
  
  // CHUNK 7: TAB GPIO
  chunk = "<div id='gpio' class='tab-content'>";
  chunk += "<div class='section'><h2 data-i18n='gpio_test'>" + String(T().gpio_test) + "</h2><p class='section-description' data-i18n='gpio_desc'>" + String(T().gpio_desc) + "</p>";
  chunk += "<div style='text-align:center;margin:20px 0'>";
  chunk += "<button class='btn btn-primary' data-i18n='test_all_gpio' onclick='testAllGPIO()'>" + String(T().test_all_gpio) + "</button>";
  chunk += "<div id='gpio-status' class='status-live' data-i18n-key='click_to_test'>" + String(T().click_to_test) + "</div>";
  chunk += "</div>";
  chunk += "<div class='warning-callout' data-i18n='gpio_warning'>" + String(T().gpio_warning) + "</div>";
  chunk += "<div id='gpio-results' class='gpio-grid'></div></div></div>";
  server.sendContent(chunk);
  
  // CHUNK 8: TAB Wireless
  chunk = "<div id='wireless' class='tab-content'>";
  chunk += "<div class='section'><h2 data-i18n='wifi_scanner'>" + String(T().wifi_scanner) + "</h2><p class='section-description' data-i18n='wifi_desc'>" + String(T().wifi_desc) + "</p>";
  chunk += "<div style='text-align:center;margin:20px 0'>";
  chunk += "<button class='btn btn-primary' data-i18n='scan_networks' onclick='scanWiFi()'>" + String(T().scan_networks) + "</button>";
  chunk += "<div id='wifi-status' class='status-live' data-i18n-key='click_to_test'>" + String(T().click_to_test) + "</div>";
  chunk += "</div><div id='wifi-results' class='wifi-list'></div></div>";
  chunk += "<div class='section'><h2 data-i18n='bluetooth_section'>" + String(T().bluetooth_section) + "</h2>";
  chunk += "<div class='info-grid'>";
  appendInfoItem(chunk, "bluetooth_status", T().bluetooth_status, htmlEscape(bluetoothStatusText), String(F("id='bluetooth-status'")));
  appendInfoItem(chunk, "bluetooth_name", T().bluetooth_name, bluetoothNameEscaped, String(F("id='bluetooth-name'")));
  appendInfoItem(chunk, "bluetooth_mac", T().bluetooth_mac, diagnosticData.bluetoothAddress, String(F("id='bluetooth-mac'")));
  String supportAttrs = "id='bluetooth-support' data-yes='" + bluetoothSupportYes + "' data-no='" + bluetoothSupportNo + "' data-supported='" + bluetoothSupportAttr + "'";
  appendInfoItem(chunk, "bluetooth_support_label", T().bluetooth_support_label, bluetoothSupportValue, supportAttrs);
  String advertisingAttrs = "id='bluetooth-advertising' data-on='" + bluetoothAdvertisingOn + "' data-off='" + bluetoothAdvertisingOff + "' data-active='" + bluetoothAdvertisingAttr + "'";
  appendInfoItem(chunk, "bluetooth_advertising_label", T().bluetooth_advertising_label, bluetoothAdvertisingValue, advertisingAttrs);
  String connectionAttrs = "id='bluetooth-connection' data-connected='" + bluetoothConnectedLabel + "' data-disconnected='" + bluetoothDisconnectedLabel + "' data-state='" + bluetoothConnectionStateAttr + "'";
  appendInfoItem(chunk, "bluetooth_connection_label", T().bluetooth_connection_label, bluetoothConnectionValue, connectionAttrs);
  appendInfoItem(chunk, "bluetooth_notifications_label", T().bluetooth_notifications_label, bluetoothNotificationsValue, String(F("id='bluetooth-notify-count'")));
  chunk += "</div>";
  chunk += "<div class='card' style='margin-top:20px'>";
  chunk += "<h3 data-i18n='bluetooth_actions'>" + String(T().bluetooth_actions) + "</h3>";
  chunk += "<div style='display:flex;flex-wrap:wrap;gap:10px;justify-content:center;margin-top:15px'>";
  chunk += "<button type='button' class='btn btn-success' data-i18n='bluetooth_enable' onclick=\"toggleBluetooth(true)\" data-bt-control='1'" + bluetoothDisableAttr + ">" + String(T().bluetooth_enable) + "</button>";
  chunk += "<button type='button' class='btn btn-warning' data-i18n='bluetooth_disable' onclick=\"toggleBluetooth(false)\" data-bt-control='1'" + bluetoothDisableAttr + ">" + String(T().bluetooth_disable) + "</button>";
  chunk += "<input type='text' id='bluetoothNameInput' data-bt-control='1' data-i18n-placeholder='bluetooth_placeholder' value='" + bluetoothNameEscaped + "' placeholder='" + String(T().bluetooth_placeholder) + "'" + bluetoothDisableAttr + ">";
  chunk += "<button type='button' class='btn btn-info' data-i18n='bluetooth_rename' onclick=\"renameBluetooth()\" data-bt-control='1'" + bluetoothDisableAttr + ">" + String(T().bluetooth_rename) + "</button>";
  chunk += "<button type='button' class='btn btn-danger' data-i18n='bluetooth_reset' onclick=\"resetBluetooth()\" data-bt-control='1'" + bluetoothDisableAttr + ">" + String(T().bluetooth_reset) + "</button>";
  chunk += "</div>";
  chunk += "<div id='bluetooth-feedback' class='status-live'>" + htmlEscape(bluetoothFeedbackText) + "</div>";
  chunk += "</div></div></div>";
  server.sendContent(chunk);
  
  // CHUNK 9: TAB Benchmark
  chunk = "<div id='benchmark' class='tab-content'>";
  chunk += "<div class='section'><h2 data-i18n='performance_bench'>" + String(T().performance_bench) + "</h2><p class='section-description' data-i18n='benchmark_desc'>" + String(T().benchmark_desc) + "</p>";
  chunk += "<div style='text-align:center;margin:20px 0'>";
  chunk += "<button class='btn btn-primary' data-i18n='run_benchmarks' onclick='runBenchmarks()'>" + String(T().run_benchmarks) + "</button>";
  chunk += "</div><div id='benchmark-results' class='info-grid'>";
  appendInfoItem(chunk, "cpu_benchmark", T().cpu_benchmark, String(T().not_tested), String(F("id='cpu-bench' data-i18n-key='not_tested'")));
  appendInfoItem(chunk, "memory_benchmark", T().memory_benchmark, String(T().not_tested), String(F("id='mem-bench' data-i18n-key='not_tested'")));
  appendInfoItem(chunk, "cpu_performance", T().cpu_performance, String(F("-")), String(F("id='cpu-perf'")));
  appendInfoItem(chunk, "memory_speed", T().memory_speed, String(F("-")), String(F("id='mem-speed'")));
  chunk += "</div></div></div>";
  server.sendContent(chunk);
  
  // CHUNK 10: TAB Export
  chunk = "<div id='export' class='tab-content'>";
  chunk += "<div class='section'><h2 data-i18n='data_export'>" + String(T().data_export) + "</h2>";
  chunk += "<div style='display:grid;grid-template-columns:repeat(auto-fit,minmax(200px,1fr));gap:20px;margin-top:20px'>";
  chunk += "<div style='text-align:center;padding:30px;background:#fff;border-radius:10px;border:2px solid #667eea'>";
  chunk += "<h3 style='color:#667eea;margin-bottom:15px' data-i18n='txt_file'>" + String(T().txt_file) + "</h3>";
  chunk += "<p style='font-size:0.9em;color:#666;margin-bottom:15px' data-i18n='readable_report'>" + String(T().readable_report) + "</p>";
  chunk += "<a href='/export/txt' class='btn btn-primary' data-i18n='download_txt'>" + String(T().download_txt) + "</a></div>";
  chunk += "<div style='text-align:center;padding:30px;background:#fff;border-radius:10px;border:2px solid #3a7bd5'>";
  chunk += "<h3 style='color:#3a7bd5;margin-bottom:15px' data-i18n='json_file'>" + String(T().json_file) + "</h3>";
  chunk += "<p style='font-size:0.9em;color:#666;margin-bottom:15px' data-i18n='structured_format'>" + String(T().structured_format) + "</p>";
  chunk += "<a href='/export/json' class='btn btn-info' data-i18n='download_json'>" + String(T().download_json) + "</a></div>";
  chunk += "<div style='text-align:center;padding:30px;background:#fff;border-radius:10px;border:2px solid #56ab2f'>";
  chunk += "<h3 style='color:#56ab2f;margin-bottom:15px' data-i18n='csv_file'>" + String(T().csv_file) + "</h3>";
  chunk += "<p style='font-size:0.9em;color:#666;margin-bottom:15px' data-i18n='for_excel'>" + String(T().for_excel) + "</p>";
  chunk += "<a href='/export/csv' class='btn btn-success' data-i18n='download_csv'>" + String(T().download_csv) + "</a></div>";
  chunk += "<div style='text-align:center;padding:30px;background:#fff;border-radius:10px;border:2px solid #667eea'>";
  chunk += "<h3 style='color:#667eea;margin-bottom:15px' data-i18n='printable_version'>" + String(T().printable_version) + "</h3>";
  chunk += "<p style='font-size:0.9em;color:#666;margin-bottom:15px' data-i18n='pdf_format'>" + String(T().pdf_format) + "</p>";
  chunk += "<a href='/print' target='_blank' class='btn btn-primary' data-i18n='open'>" + String(T().open) + "</a></div>";
  chunk += "</div></div></div>";
  chunk += "</div>"; // Fermeture tabContainer
  chunk += "</main>";
  chunk += "</div>"; // Fermeture app-body
  chunk += "<footer class='footer'>ESP32 Diagnostic v" + String(DIAGNOSTIC_VERSION) + " • " + diagnosticData.chipModel + " • MAC " + diagnosticData.macAddress + "</footer>";
  chunk += "</div>"; // Fermeture app-shell
  server.sendContent(chunk);
  // CHUNK 11: JavaScript complet
  chunk = "<script>";
  chunk += "var currentLang='" + String(currentLanguage == LANG_FR ? "fr" : "en") + "';";
  chunk += "var connectionState=true;";
  chunk += "var ignoreHashChange=false;";
  chunk += "var navDropdown=null;";
  chunk += "function preferredAccessScheme(){return window.location.protocol==='https:'?'https://':'http://';}";
  chunk += "function applyAccessLinkScheme(){var links=document.querySelectorAll('[data-access-host]');if(!links||!links.length){return;}var preferSecure=(preferredAccessScheme()==='https://');for(var i=0;i<links.length;i++){var link=links[i];if(!link){continue;}var host=link.getAttribute('data-access-host');if(!host){continue;}var secure=link.getAttribute('data-secure')||'https://';var legacy=link.getAttribute('data-legacy')||'http://';var scheme=preferSecure?secure:legacy;var labelHost=link.getAttribute('data-access-label')||host;var legacyLabel=link.getAttribute('data-legacy-label');link.href=scheme+host;if(!preferSecure&&legacyLabel){link.textContent=legacyLabel;}else{link.textContent=scheme+labelHost;}}}";
  chunk += "document.documentElement.setAttribute('lang',currentLang);";
  chunk += "function tr(key,fr,en){var dict=window.translations||{};var value=dict&&dict[key];if(typeof value==='string'){return value;}if(currentLang==='fr'){return typeof fr!=='undefined'?fr:(typeof en!=='undefined'?en:key);}return typeof en!=='undefined'?en:(typeof fr!=='undefined'?fr:key);}";
  chunk += "function showInlineMessage(text,state){var holder=document.getElementById('inlineMessage');if(!holder){return;}holder.className='inline-message';if(!text){return;}holder.textContent=text;holder.classList.add('show');if(state){holder.classList.add(state);}}";
  chunk += "function clearInlineMessage(){var holder=document.getElementById('inlineMessage');if(!holder){return;}holder.className='inline-message';holder.textContent='';}";
  chunk += "function updateStatus(id,text,state,key){var el=document.getElementById(id);if(el){if(typeof key==='string'&&key.length){el.setAttribute('data-i18n-key',key);}else if(key===null){el.removeAttribute('data-i18n-key');}else if(typeof key==='undefined'&&typeof text!=='undefined'&&text!==null){el.removeAttribute('data-i18n-key');}if(typeof text!=='undefined'&&text!==null){el.textContent=text;}el.classList.remove('success','error');if(state){el.classList.add(state);}}if(state==='error'||state==='success'){showInlineMessage(text,state);}else if(text===''){clearInlineMessage();}}";
  chunk += "function connectionLabelText(online){return online?tr('online','En ligne','Online'):tr('offline','Hors ligne','Offline');}";
  chunk += "function updateStatusIndicator(online){connectionState=!!online;var indicator=document.getElementById('statusIndicator');var label=document.getElementById('connectionLabel');if(indicator){indicator.classList.remove('status-online','status-offline');indicator.classList.add(online?'status-online':'status-offline');}if(label){label.textContent=connectionLabelText(online);}var wifiDot=document.getElementById('wifiStatusDot');if(wifiDot){wifiDot.classList.remove('offline');if(!online){wifiDot.classList.add('offline');}}var wifiValue=document.getElementById('wifiStatusValue');if(wifiValue){var offlineText=wifiValue.getAttribute('data-offline');var onlineText=wifiValue.getAttribute('data-online');if(!online&&offlineText){wifiValue.textContent=offlineText;}else if(online&&onlineText){wifiValue.textContent=onlineText;}}if(!online){showInlineMessage(connectionLabelText(false)+' — '+tr('check_network','Vérifiez le réseau.','Check the network.'),'error');}else{clearInlineMessage();}}";
  chunk += "function updateUptimeDisplay(){var el=document.getElementById('uptime-display');if(!el){return;}var days=parseInt(el.getAttribute('data-days')||'0',10);var hours=parseInt(el.getAttribute('data-hours')||'0',10);var minutes=parseInt(el.getAttribute('data-minutes')||'0',10);var tr=window.translations||null;var dayLabel=(tr&&tr.days)?tr.days:(currentLang==='fr'?'jours':'days');var hourLabel=(tr&&tr.hours)?tr.hours:(currentLang==='fr'?'heures':'hours');var minuteLabel=(tr&&tr.minutes)?tr.minutes:(currentLang==='fr'?'minutes':'minutes');var pieces=[];if(days>0){pieces.push(days+' '+dayLabel);}if(hours>0||pieces.length){pieces.push(hours+' '+hourLabel);}if(minutes>0||pieces.length===0){pieces.push(minutes+' '+minuteLabel);}el.textContent=pieces.join(' ');}";

  chunk += "function updateBluetoothFeedback(message,state,notify){var box=document.getElementById('bluetooth-feedback');if(!box){return;}if(typeof message==='undefined'||message===null){message='';}box.textContent=message;box.classList.remove('success','error');if(state){box.classList.add(state);}if(notify&&message){if(state==='error'){showInlineMessage(message,'error');}else if(state==='success'){showInlineMessage(message,'success');}}else if(notify&&!message){clearInlineMessage();}}";
  chunk += "function applyBluetoothState(data,notify){if(!data){return;}var statusEl=document.getElementById('bluetooth-status');if(statusEl&&typeof data.status!=='undefined'){statusEl.textContent=data.status;}var nameEl=document.getElementById('bluetooth-name');if(nameEl&&typeof data.name!=='undefined'){nameEl.textContent=data.name;}var macEl=document.getElementById('bluetooth-mac');if(macEl&&typeof data.mac!=='undefined'){macEl.textContent=data.mac;}var input=document.getElementById('bluetoothNameInput');if(input&&typeof data.name!=='undefined'){input.value=data.name;}var summary=document.getElementById('bluetoothSummary');if(summary){summary.setAttribute('data-supported',(data.supported===false)?'false':'true');var offlineStored=summary.getAttribute('data-offline')||'';var disabledStored=summary.getAttribute('data-disabled')||offlineStored;var unsupportedStored=summary.getAttribute('data-unsupported')||offlineStored;if(data.supported===false){var unsupportedText=unsupportedStored||disabledStored||offlineStored;summary.textContent=unsupportedText;summary.setAttribute('data-offline',unsupportedText);}else if(data.enabled===false){var disabledText=disabledStored||offlineStored;summary.textContent=disabledText;summary.setAttribute('data-offline',disabledText);}else{var pieces=[];if(typeof data.status==='string'){pieces.push(data.status);}if(typeof data.summary==='string'){pieces.push(data.summary);}if(pieces.length){var combined=pieces.join(' • ');summary.textContent=combined;summary.setAttribute('data-online',combined);summary.setAttribute('data-offline',disabledStored||offlineStored);}}}var supported=(data.supported===false)?false:true;var supportEl=document.getElementById('bluetooth-support');if(supportEl){var yesText=supportEl.getAttribute('data-yes')||'';var noText=supportEl.getAttribute('data-no')||'';supportEl.setAttribute('data-supported',supported?'true':'false');if(supported&&yesText){supportEl.textContent=yesText;}else if(!supported&&noText){supportEl.textContent=noText;}}var advEl=document.getElementById('bluetooth-advertising');if(advEl){var onText=advEl.getAttribute('data-on')||'';var offText=advEl.getAttribute('data-off')||'';var advActive=supported&&data.enabled!==false&&(data.advertising===true||data.clientConnected===true);advEl.setAttribute('data-active',advActive?'true':'false');if(!supported){var unsupportedTextForAdv=(typeof noText!=='undefined'&&noText)?noText:offText;if(unsupportedTextForAdv){advEl.textContent=unsupportedTextForAdv;}}else if(advActive&&onText){advEl.textContent=onText;}else if(offText){advEl.textContent=offText;}}var connectionEl=document.getElementById('bluetooth-connection');if(connectionEl){var connectedText=connectionEl.getAttribute('data-connected')||'';var disconnectedText=connectionEl.getAttribute('data-disconnected')||'';var stateValue='unsupported';if(supported){if(data.enabled===false){stateValue='disabled';}else if(data.clientConnected===true){stateValue='connected';}else{stateValue='idle';}}connectionEl.setAttribute('data-state',stateValue);if(stateValue==='connected'&&connectedText){connectionEl.textContent=connectedText;}else if(disconnectedText){connectionEl.textContent=disconnectedText;}}var notifyEl=document.getElementById('bluetooth-notify-count');if(notifyEl){var count=(typeof data.notifyCount!=='undefined')?data.notifyCount:0;notifyEl.textContent=count;}";
  chunk += "var indicator=document.getElementById('bluetoothStatusDot');if(indicator){indicator.classList.remove('offline');if(data.supported===false||data.enabled===false){indicator.classList.add('offline');}}var controls=document.querySelectorAll('[data-bt-control=\"1\"]');if(controls&&controls.length){for(var i=0;i<controls.length;i++){controls[i].disabled=(data.supported===false);}}var message=(typeof data.message==='string')?data.message:'';var state=null;if(typeof data.success!=='undefined'){state=data.success?'success':'error';}if(notify){updateBluetoothFeedback(message,state,true);}else if(data.supported===false){updateBluetoothFeedback(message,'error',false);}else{updateBluetoothFeedback('',null,false);}}";
  chunk += "function refreshBluetoothStatus(showNotice){fetch('/api/bluetooth/status').then(function(r){return r.json();}).then(function(data){applyBluetoothState(data,showNotice);}).catch(function(err){var message=tr('bluetooth_error','Erreur Bluetooth','Bluetooth error')+': '+(err&&err.message?err.message:err);updateBluetoothFeedback(message,'error',showNotice);});}";
  chunk += "function toggleBluetooth(enable){updateBluetoothFeedback(enable?tr('bluetooth_enabling','Activation...','Enabling...'):tr('bluetooth_disabling','Désactivation...','Disabling...'),null,false);fetch('/api/bluetooth/toggle?state='+(enable?'on':'off')).then(function(r){return r.json();}).then(function(data){applyBluetoothState(data,true);}).catch(function(err){var message=tr('bluetooth_error','Erreur Bluetooth','Bluetooth error')+': '+(err&&err.message?err.message:err);updateBluetoothFeedback(message,'error',true);});return false;}";
  chunk += "function renameBluetooth(){var input=document.getElementById('bluetoothNameInput');if(!input){return false;}var value=input.value||'';if(!value.trim()){updateBluetoothFeedback(tr('bluetooth_invalid_name','Nom Bluetooth invalide','Invalid Bluetooth name'),'error',true);return false;}updateBluetoothFeedback(tr('bluetooth_updating','Mise à jour...','Updating...'),null,false);fetch('/api/bluetooth/name?name='+encodeURIComponent(value.trim())).then(function(r){return r.json();}).then(function(data){applyBluetoothState(data,true);}).catch(function(err){var message=tr('bluetooth_error','Erreur Bluetooth','Bluetooth error')+': '+(err&&err.message?err.message:err);updateBluetoothFeedback(message,'error',true);});return false;}";
  chunk += "function resetBluetooth(){updateBluetoothFeedback(tr('bluetooth_resetting','Réinitialisation...','Resetting...'),null,false);fetch('/api/bluetooth/reset').then(function(r){return r.json();}).then(function(data){applyBluetoothState(data,true);}).catch(function(err){var message=tr('bluetooth_error','Erreur Bluetooth','Bluetooth error')+': '+(err&&err.message?err.message:err);updateBluetoothFeedback(message,'error',true);});return false;}";

  chunk += "function changeLang(lang,btn){";
  chunk += "fetch('/api/set-language?lang='+lang).then(function(r){return r.json();}).then(function(d){";
  chunk += "if(!d.success){throw new Error(d.message||'lang');}";
  chunk += "currentLang=lang;";
  chunk += "document.documentElement.setAttribute('lang',lang);";
  chunk += "var langButtons=document.querySelectorAll('.lang-btn');";
  chunk += "for(var i=0;i<langButtons.length;i++){langButtons[i].classList.remove('active');}";
  chunk += "if(btn){btn.classList.add('active');}";
  chunk += "updateTranslations(true);";
  chunk += "}).catch(function(err){var message=err&&err.message?err.message:err;showInlineMessage(tr('language_switch_error','Erreur changement langue','Language switch error')+': '+message,'error');});";
  chunk += "return false;";
  chunk += "}";

  chunk += "function updateTranslations(showNotice){";
  chunk += "fetch('/api/get-translations').then(function(r){return r.json();}).then(function(translations){";
  chunk += "window.translations=translations;";
  chunk += "var indicator=document.getElementById('updateIndicator');if(indicator&&translations.updating){indicator.textContent=translations.updating;}";
  chunk += "var mainTitle=document.getElementById('main-title');if(mainTitle){var versionSpan=mainTitle.querySelector('.version-tag');if(versionSpan){var versionLabel=(translations.version||'v');versionSpan.textContent=versionLabel+'" + String(DIAGNOSTIC_VERSION) + "';}}";
  chunk += "var nodes=document.querySelectorAll('[data-i18n]');";
  chunk += "for(var i=0;i<nodes.length;i++){var el=nodes[i];var key=el.getAttribute('data-i18n');if(!key){continue;}var value=translations[key];if(typeof value==='undefined'){continue;}if(key==='enable_psram_hint'){var psramType=el.getAttribute('data-psram-type')||'';value=value.replace('%TYPE%',psramType);}var prefix=el.getAttribute('data-i18n-prefix')||'';var suffix=el.getAttribute('data-i18n-suffix')||'';el.textContent=prefix+value+suffix;}";
  chunk += "var keyed=document.querySelectorAll('[data-i18n-key]');";
  chunk += "for(var j=0;j<keyed.length;j++){var target=keyed[j];var dynamicKey=target.getAttribute('data-i18n-key');if(!dynamicKey){continue;}var text=translations[dynamicKey];if(typeof text==='undefined'){continue;}var prefix=target.getAttribute('data-i18n-prefix')||'';var suffix=target.getAttribute('data-i18n-suffix')||'';target.textContent=prefix+text+suffix;}";
  chunk += "var placeholders=document.querySelectorAll('[data-i18n-placeholder]');";
  chunk += "for(var p=0;p<placeholders.length;p++){var field=placeholders[p];var placeholderKey=field.getAttribute('data-i18n-placeholder');if(!placeholderKey){continue;}var placeholderText=translations[placeholderKey];if(typeof placeholderText==='undefined'){continue;}var prefix=field.getAttribute('data-i18n-placeholder-prefix')||'';var suffix=field.getAttribute('data-i18n-placeholder-suffix')||'';field.setAttribute('placeholder',prefix+placeholderText+suffix);}";
  chunk += "if(translations.nav_select_label){var navLabel=document.querySelector('label[for=\\'navSelector\\']');if(navLabel){navLabel.textContent=translations.nav_select_label;}if(navDropdown){navDropdown.setAttribute('aria-label',translations.nav_select_label);}}";
  chunk += "var wifiValue=document.getElementById('wifiStatusValue');if(wifiValue&&translations.not_detected){wifiValue.setAttribute('data-offline',translations.not_detected);if(!connectionState){wifiValue.textContent=translations.not_detected;}}";
  chunk += "var btSummary=document.getElementById('bluetoothSummary');if(btSummary){if(translations.bluetooth_disabled){btSummary.setAttribute('data-disabled',translations.bluetooth_disabled);}if(translations.bluetooth_not_supported){btSummary.setAttribute('data-unsupported',translations.bluetooth_not_supported);}var supported=btSummary.getAttribute('data-supported');var btDot=document.getElementById('bluetoothStatusDot');var offlineState=btDot&&btDot.classList.contains('offline');if(offlineState){var offlineText=(supported==='false')?(translations.bluetooth_not_supported||btSummary.getAttribute('data-offline')):(translations.bluetooth_disabled||btSummary.getAttribute('data-offline'));if(offlineText){btSummary.textContent=offlineText;btSummary.setAttribute('data-offline',offlineText);}}}";
  chunk += "var btSupport=document.getElementById('bluetooth-support');if(btSupport){if(translations.bluetooth_support_yes){btSupport.setAttribute('data-yes',translations.bluetooth_support_yes);}if(translations.bluetooth_support_no){btSupport.setAttribute('data-no',translations.bluetooth_support_no);}var supportState=btSupport.getAttribute('data-supported')==='true';var yesValue=btSupport.getAttribute('data-yes')||'';var noValue=btSupport.getAttribute('data-no')||'';if(supportState&&yesValue){btSupport.textContent=yesValue;}else if(!supportState&&noValue){btSupport.textContent=noValue;}}";
  chunk += "var btAdvert=document.getElementById('bluetooth-advertising');if(btAdvert){if(translations.bluetooth_advertising){btAdvert.setAttribute('data-on',translations.bluetooth_advertising);}if(translations.bluetooth_not_advertising){btAdvert.setAttribute('data-off',translations.bluetooth_not_advertising);}var advertActive=btAdvert.getAttribute('data-active')==='true';var advertOn=btAdvert.getAttribute('data-on')||'';var advertOff=btAdvert.getAttribute('data-off')||'';if(advertActive&&advertOn){btAdvert.textContent=advertOn;}else if(advertOff){btAdvert.textContent=advertOff;}}";
  chunk += "var btConnection=document.getElementById('bluetooth-connection');if(btConnection){if(translations.bluetooth_client_connected){btConnection.setAttribute('data-connected',translations.bluetooth_client_connected);}if(translations.bluetooth_client_disconnected){btConnection.setAttribute('data-disconnected',translations.bluetooth_client_disconnected);}var connState=btConnection.getAttribute('data-state');var connectedText=btConnection.getAttribute('data-connected')||'';var disconnectedText=btConnection.getAttribute('data-disconnected')||'';if(connState==='connected'&&connectedText){btConnection.textContent=connectedText;}else if(disconnectedText){btConnection.textContent=disconnectedText;}}";
  chunk += "refreshBluetoothStatus(false);";
  chunk += "updateStatusIndicator(connectionState);";
  chunk += "updateUptimeDisplay();";
  chunk += "if(showNotice){showInlineMessage(tr('language_updated','Langue mise à jour','Language updated'),'success');}";
  chunk += "}).catch(function(err){var message=err&&err.message?err.message:err;showInlineMessage(tr('translation_error','Erreur traduction','Translation error')+': '+message,'error');});";
  chunk += "}";

  chunk += "function findNavTrigger(el){while(el&&el.classList&&!el.classList.contains('nav-link')){el=el.parentElement;}if(el&&el.classList&&el.classList.contains('nav-link')){return el;}return null;}";
  chunk += "function showTab(tabId,trigger,updateHash){if(!tabId){return false;}if(tabId==='wifi'){tabId='wireless';}clearInlineMessage();var tabs=document.querySelectorAll('.tab-content');for(var i=0;i<tabs.length;i++){tabs[i].classList.remove('active');}var target=document.getElementById(tabId);if(target){target.classList.add('active');var main=document.querySelector('.app-main');if(main){var nav=document.querySelector('.nav-wrapper');var top=main.getBoundingClientRect().top+(window.pageYOffset||document.documentElement.scrollTop||0);var adjust=nav?nav.offsetHeight:0;var destination=top-adjust-8;if(destination<0){destination=0;}window.scrollTo(0,destination);}else if(typeof target.scrollIntoView==='function'){target.scrollIntoView(true);}}var buttons=document.querySelectorAll('.nav-link');for(var j=0;j<buttons.length;j++){buttons[j].classList.remove('active');buttons[j].removeAttribute('aria-current');}var actual=trigger;if(!actual||!actual.classList){var selector=\".nav-link[data-target='\"+tabId+\"']\";actual=document.querySelector(selector);}if(actual&&actual.classList){actual.classList.add('active');actual.setAttribute('aria-current','page');}if(navDropdown&&navDropdown.value!==tabId){navDropdown.value=tabId;}if(updateHash!==false){if(window.location.hash!=='#'+tabId){ignoreHashChange=true;window.location.hash=tabId;setTimeout(function(){ignoreHashChange=false;},0);}}return false;}";
  chunk += "function initStickyNav(){var wrapper=document.querySelector('.nav-wrapper');if(!wrapper||wrapper.getAttribute('data-sticky-init')==='1'){return;}wrapper.setAttribute('data-sticky-init','1');var apply=function(state){if(state){wrapper.classList.add('is-sticky');}else{wrapper.classList.remove('is-sticky');}};";
  chunk += "var computeThreshold=function(){var rect=wrapper.getBoundingClientRect();var scrollTop=window.pageYOffset||document.documentElement.scrollTop||0;var stickyOffset=parseInt(window.getComputedStyle(wrapper).top,10)||0;return rect.top+scrollTop-stickyOffset;};var threshold=computeThreshold();";
  chunk += "var onScroll=function(){var scroll=window.pageYOffset||document.documentElement.scrollTop||0;var state=scroll>=threshold;apply(state);};window.addEventListener('scroll',onScroll);window.addEventListener('resize',function(){threshold=computeThreshold();onScroll();});onScroll();}";
  chunk += "function initNavigation(){updateUptimeDisplay();var navs=document.querySelectorAll('.primary-nav');if(navs&&navs.length){for(var n=0;n<navs.length;n++){(function(nav){nav.addEventListener('click',function(evt){var source=evt.target||evt.srcElement;var button=findNavTrigger(source);if(!button){return;}evt.preventDefault();var targetTab=button.getAttribute('data-target');if(targetTab){showTab(targetTab,button);}});})(navs[n]);}}var select=document.getElementById('navSelector');navDropdown=select;if(select){select.addEventListener('change',function(evt){var value=evt.target.value;if(value){showTab(value,null);}});}var initial=window.location.hash?window.location.hash.substring(1):null;var defaultButton=document.querySelector('.nav-link.active');if(initial==='wifi'){initial='wireless';}if(!initial&&defaultButton){initial=defaultButton.getAttribute('data-target');}if(!initial){var list=document.querySelectorAll('.nav-link');if(list.length>0){initial=list[0].getAttribute('data-target');defaultButton=list[0];}}var initialButton=null;if(initial){initialButton=document.querySelector(\".nav-link[data-target='\"+initial+\"']\");}if(initial){showTab(initial,initialButton,false);}else{showTab('overview',null,false);}initStickyNav();updateStatusIndicator(connectionState);applyAccessLinkScheme();refreshBluetoothStatus(false);}";
  chunk += "window.addEventListener('hashchange',function(){if(ignoreHashChange){return;}var target=window.location.hash?window.location.hash.substring(1):'overview';if(target==='wifi'){target='wireless';}showTab(target,null,false);});";
  chunk += "if(document.readyState==='loading'){document.addEventListener('DOMContentLoaded',function(){initNavigation();applyAccessLinkScheme();});}else{initNavigation();applyAccessLinkScheme();}";

  chunk += "function configBuiltinLED(){var fallback=tr('gpio_invalid','GPIO invalide','Invalid GPIO');updateStatus('builtin-led-status',tr('configuring','Configuration...','Configuring...'),null,'configuring');fetch('/api/builtin-led-config?gpio='+document.getElementById('ledGPIO').value).then(function(r){return r.json();}).then(function(d){var state=d.success?'success':'error';updateStatus('builtin-led-status',d.message||fallback,state,null);}).catch(function(e){updateStatus('builtin-led-status',tr('error_label','Erreur','Error')+': '+e,'error',null);});}";
  chunk += "function testBuiltinLED(){updateStatus('builtin-led-status',tr('testing','Test...','Testing...'),null,'testing');fetch('/api/builtin-led-test').then(function(r){return r.json();}).then(function(d){var state=d.success?'success':'error';var fallback=tr('test_failed','Test en échec','Test failed');updateStatus('builtin-led-status',d.result||fallback,state,null);}).catch(function(e){updateStatus('builtin-led-status',tr('error_label','Erreur','Error')+': '+e,'error',null);});}";
  chunk += "function ledBlink(){fetch('/api/builtin-led-control?action=blink').then(function(r){return r.json();}).then(function(d){updateStatus('builtin-led-status',d.message,d.success?'success':'error');}).catch(function(e){updateStatus('builtin-led-status',tr('error_label','Erreur','Error')+': '+e,'error');});}";
  chunk += "function ledFade(){fetch('/api/builtin-led-control?action=fade').then(function(r){return r.json();}).then(function(d){updateStatus('builtin-led-status',d.message,d.success?'success':'error');}).catch(function(e){updateStatus('builtin-led-status',tr('error_label','Erreur','Error')+': '+e,'error');});}";
  chunk += "function ledOff(){fetch('/api/builtin-led-control?action=off').then(function(r){return r.json();}).then(function(d){updateStatus('builtin-led-status',d.message,d.success?'success':'error');}).catch(function(e){updateStatus('builtin-led-status',tr('error_label','Erreur','Error')+': '+e,'error');});}";

  chunk += "function configNeoPixel(){updateStatus('neopixel-status',tr('configuring','Configuration...','Configuring...'),null,'configuring');fetch('/api/neopixel-config?gpio='+document.getElementById('neoGPIO').value+'&count='+document.getElementById('neoCount').value).then(function(r){return r.json();}).then(function(d){updateStatus('neopixel-status',d.message,d.success?'success':'error',null);}).catch(function(e){updateStatus('neopixel-status',tr('error_label','Erreur','Error')+': '+e,'error',null);});}";
  chunk += "function testNeoPixel(){updateStatus('neopixel-status',tr('testing','Test...','Testing...'),null,'testing');fetch('/api/neopixel-test').then(function(r){return r.json();}).then(function(d){updateStatus('neopixel-status',d.result,d.success?'success':'error',null);}).catch(function(e){updateStatus('neopixel-status',tr('error_label','Erreur','Error')+': '+e,'error',null);});}";
  chunk += "function neoPattern(p){fetch('/api/neopixel-pattern?pattern='+p).then(function(r){return r.json();}).then(function(d){updateStatus('neopixel-status',d.message,d.success?'success':'error');}).catch(function(e){updateStatus('neopixel-status',tr('error_label','Erreur','Error')+': '+e,'error');});}";
  chunk += "function neoCustomColor(){var c=document.getElementById('neoColor').value;var r=parseInt(c.substr(1,2),16);var g=parseInt(c.substr(3,2),16);var b=parseInt(c.substr(5,2),16);updateStatus('neopixel-status','RGB('+r+','+g+','+b+')...',null);fetch('/api/neopixel-color?r='+r+'&g='+g+'&b='+b).then(function(rsp){return rsp.json();}).then(function(d){updateStatus('neopixel-status',d.message,d.success?'success':'error');}).catch(function(e){updateStatus('neopixel-status',tr('error_label','Erreur','Error')+': '+e,'error');});}";

  chunk += "function testOLED(){updateStatus('oled-status',tr('oled_test_running','Test en cours (25s)...','Testing (25s)...'),null,'oled_test_running');fetch('/api/oled-test').then(function(r){return r.json();}).then(function(d){updateStatus('oled-status',d.result,d.success?'success':'error',null);}).catch(function(e){updateStatus('oled-status',tr('error_label','Erreur','Error')+': '+e,'error',null);});}";
  chunk += "function oledStep(step){updateStatus('oled-status',tr('testing','Test...','Testing...'),null,'testing');fetch('/api/oled-step?step='+step).then(function(r){return r.json();}).then(function(d){updateStatus('oled-status',d.message,d.success?'success':'error',null);}).catch(function(e){updateStatus('oled-status',tr('error_label','Erreur','Error')+': '+e,'error',null);});}";
  chunk += "function oledMessage(){var msg=document.getElementById('oledMsg').value;if(!msg){updateStatus('oled-status',tr('custom_message','Message personnalisé','Custom message'),'error','custom_message');return;}updateStatus('oled-status',tr('transmission','Transmission...','Transmitting...'),null,'transmission');fetch('/api/oled-message?message='+encodeURIComponent(msg)).then(function(r){return r.json();}).then(function(d){updateStatus('oled-status',d.message,d.success?'success':'error',null);}).catch(function(e){updateStatus('oled-status',tr('error_label','Erreur','Error')+': '+e,'error',null);});}";
  chunk += "function configOLED(){updateStatus('oled-status',tr('reconfiguring','Reconfiguration...','Reconfiguring...'),null,'reconfiguring');var params='sda='+document.getElementById('oledSDA').value+'&scl='+document.getElementById('oledSCL').value+'&rotation='+document.getElementById('oledRotation').value;fetch('/api/oled-config?'+params).then(function(r){return r.json();}).then(function(d){var state=d.success?'success':'error';var fallback=tr('configuration_invalid','Configuration invalide','Invalid configuration');updateStatus('oled-status',d.message||fallback,state,null);if(d.success&&typeof d.sda!=='undefined'){document.getElementById('oledSDA').value=d.sda;}if(d.success){var pins=document.getElementById('oled-pins');if(pins){pins.textContent='SDA:'+d.sda+' SCL:'+d.scl;}}if(d.success){var rotDisplay=document.getElementById('oled-rotation-display');if(rotDisplay){rotDisplay.textContent=d.rotation;}}}).catch(function(e){updateStatus('oled-status',tr('error_label','Erreur','Error')+': '+e,'error',null);});}";

  chunk += "function testADC(){var status=document.getElementById('adc-status');if(status){status.setAttribute('data-i18n-key','testing');status.textContent=tr('testing','Test...','Testing...');}fetch('/api/adc-test').then(function(r){return r.json();}).then(function(data){var h='';if(data.readings&&data.readings.forEach){data.readings.forEach(function(a){h+=\"<div class='info-item'><div class='info-label'>GPIO \"+a.pin+\"</div><div class='info-value'>\"+a.raw+\" (\"+a.voltage.toFixed(2)+\"V)</div></div>\";});}document.getElementById('adc-results').innerHTML=h;if(status){status.removeAttribute('data-i18n-key');status.textContent=data.result;}});}";
  chunk += "function testPWM(){var status=document.getElementById('pwm-status');if(status){status.setAttribute('data-i18n-key','testing');status.textContent=tr('testing','Test...','Testing...');}fetch('/api/pwm-test').then(function(r){return r.json();}).then(function(d){if(status){status.removeAttribute('data-i18n-key');status.textContent=d.result;}});}";
  chunk += "function scanSPI(){var status=document.getElementById('spi-status');if(status){status.setAttribute('data-i18n-key','scan');status.textContent=tr('scan','Scan...','Scan...');}fetch('/api/spi-scan').then(function(r){return r.json();}).then(function(d){if(status){status.removeAttribute('data-i18n-key');status.textContent=d.info;}});}";
  chunk += "function listPartitions(){var container=document.getElementById('partitions-results');if(container){container.setAttribute('data-i18n-key','scan');container.textContent=tr('scan','Scan...','Scan...');}fetch('/api/partitions-list').then(function(r){return r.json();}).then(function(d){if(container){container.removeAttribute('data-i18n-key');container.innerHTML=d.partitions;}});}";
  chunk += "function stressTest(){var status=document.getElementById('stress-status');if(status){status.setAttribute('data-i18n-key','testing');status.textContent=tr('testing','Test...','Testing...');}fetch('/api/stress-test').then(function(r){return r.json();}).then(function(d){if(status){status.removeAttribute('data-i18n-key');status.textContent=d.result;}});}";

  chunk += "function testAllGPIO(){var status=document.getElementById('gpio-status');if(status){status.setAttribute('data-i18n-key','testing');status.textContent=tr('testing','Test...','Testing...');}fetch('/api/test-gpio').then(function(r){return r.json();}).then(function(data){var h='';if(data.results&&data.results.forEach){data.results.forEach(function(g){h+=\"<div class='gpio-item \"+(g.working?\"gpio-ok\":\"gpio-fail\")+\"'>GPIO \"+g.pin+\"<br>\"+(g.working?\"OK\":\"FAIL\")+\"</div>\";});}document.getElementById('gpio-results').innerHTML=h;var template=tr('gpio_summary_template','Terminé - %COUNT% GPIO testés','Done - %COUNT% GPIO tested');if(status){status.removeAttribute('data-i18n-key');status.textContent=template.replace('%COUNT%',data.results.length);}});}";
  chunk += "function scanWiFi(){var status=document.getElementById('wifi-status');if(status){status.setAttribute('data-i18n-key','scan');status.textContent=tr('scan','Scan...','Scan...');}fetch('/api/wifi-scan').then(function(r){return r.json();}).then(function(data){var h='';if(data.networks&&data.networks.forEach){data.networks.forEach(function(n){var s=n.rssi>=-60?'🟢':(n.rssi>=-70?'🟡':'🔴');var freq=(n.freq&&n.freq>0)?n.freq+' MHz':'';var pieces=[];if(n.bssid){pieces.push(n.bssid);}if(n.channel){pieces.push('Ch'+n.channel);}if(n.band){pieces.push(n.band);}if(freq){pieces.push(freq);}if(n.bandwidth){pieces.push(n.bandwidth);}if(n.phy){pieces.push(n.phy);}if(n.encryption&&n.encryption!=='-'){pieces.push(n.encryption);}var details=pieces.join(' | ');h+=\"<div class='wifi-item'><div style='display:flex;justify-content:space-between'><div><strong>\"+s+\" \"+n.ssid+\"</strong><br><small>\"+details+\"</small></div><div style='font-size:1.2em;font-weight:bold'>\"+n.rssi+\" dBm</div></div></div>\";});}document.getElementById('wifi-results').innerHTML=h;var wifiLabel=tr('wifi_networks_found','{count} réseaux détectés','{count} networks found');if(status){status.removeAttribute('data-i18n-key');status.textContent=wifiLabel.replace('{count}',data.networks.length);}});}";
  chunk += "function scanI2C(){updateStatus('i2c-status',tr('scan','Scan...','Scan...'),null,'scan');fetch('/api/i2c-scan').then(function(r){return r.json();}).then(function(d){var label=tr('i2c_scan_result','I2C : %COUNT% périphérique(s)','I2C: %COUNT% device(s)');updateStatus('i2c-status',label.replace('%COUNT%',d.count),'success');var summary=document.getElementById('i2c-summary');if(summary){var countLabel=tr('device_count','Nombre de Périphériques','Device count');summary.textContent=d.count+' '+countLabel+' - '+d.devices;}}).catch(function(e){updateStatus('i2c-status',tr('error_label','Erreur','Error')+': '+e,'error',null);});}";

  chunk += "function runBenchmarks(){var cpu=document.getElementById('cpu-bench');var mem=document.getElementById('mem-bench');if(cpu){cpu.setAttribute('data-i18n-key','testing');cpu.textContent=tr('testing','Test...','Testing...');}if(mem){mem.setAttribute('data-i18n-key','testing');mem.textContent=tr('testing','Test...','Testing...');}fetch('/api/benchmark').then(function(r){return r.json();}).then(function(data){if(cpu){cpu.removeAttribute('data-i18n-key');cpu.textContent=data.cpu+' us';}if(mem){mem.removeAttribute('data-i18n-key');mem.textContent=data.memory+' us';}var perf=document.getElementById('cpu-perf');if(perf){perf.textContent=data.cpuPerf.toFixed(2)+' MFLOPS';}var speed=document.getElementById('mem-speed');if(speed){speed.textContent=data.memSpeed.toFixed(2)+' MB/s';}});}";

  chunk += "</script></body></html>";
  server.sendContent(chunk);
  
  // Signal de fin
  server.sendContent("");
}

// ========== SETUP COMPLET ==========
void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println("\r\n===============================================");
  Serial.println("     DIAGNOSTIC ESP32 MULTILINGUE");
  Serial.printf("     Version %s - FR/EN\r\n", DIAGNOSTIC_VERSION_STR);
  Serial.printf("     Arduino Core %s\r\n", getArduinoCoreVersionString().c_str());
  Serial.println("===============================================\r\n");

  uint8_t btMac[6] = {0};
  if (esp_read_mac(btMac, ESP_MAC_BT) != ESP_OK) {
    esp_read_mac(btMac, ESP_MAC_WIFI_STA);
  }
  char btSuffix[5];
  sprintf(btSuffix, "%02X%02X", btMac[4], btMac[5]);
  defaultBluetoothName = String("ESP32Diag-") + String(btSuffix);
  bluetoothDeviceName = sanitizeBluetoothName(defaultBluetoothName);
  bluetoothEnabled = false;
  bluetoothAdvertising = false;

  printPSRAMDiagnostic();

  // WiFi
  WiFi.mode(WIFI_STA);
  WiFi.persistent(false);

  WiFi.setSleep(false);
  configureNetworkHostname();
#if ESP_ARDUINO_VERSION >= ESP_ARDUINO_VERSION_VAL(3, 3, 0)
  WiFi.setScanMethod(WIFI_ALL_CHANNEL_SCAN);
#ifdef WIFI_CONNECT_AP_BY_SIGNAL
  WiFi.setSortMethod(WIFI_CONNECT_AP_BY_SIGNAL);
#endif
#endif
  wifiMulti.addAP(WIFI_SSID_1, WIFI_PASS_1);
  wifiMulti.addAP(WIFI_SSID_2, WIFI_PASS_2);
  
  Serial.println("Connexion WiFi...");
  int attempt = 0;
  while (wifiMulti.run() != WL_CONNECTED && attempt < 40) {
    delay(500);
    Serial.print(".");
    attempt++;
  }
  
  bool wifiConnected = (WiFi.status() == WL_CONNECTED);
#if DIAGNOSTIC_HAS_MDNS
  wifiPreviouslyConnected = wifiConnected;
#endif

  if (wifiConnected) {
    Serial.println("\r\n\r\nWiFi OK!");
    Serial.printf("SSID: %s\r\n", WiFi.SSID().c_str());
    Serial.printf("IP: %s\r\n\r\n", WiFi.localIP().toString().c_str());
    if (startMDNSService(true)) {
      Serial.printf("[Accès] Lien constant : %s\r\n", getStableAccessURL().c_str());
    } else {
      Serial.println("[mDNS] Initialisation différée - nouvel essai automatique");
    }
  } else {
    Serial.println("\r\n\r\nPas de WiFi\r\n");
    Serial.printf("[Accès] Lien constant disponible après connexion : %s\r\n", getStableAccessURL().c_str());
  }
  
  // Détections
  detectBuiltinLED();
  detectNeoPixelSupport();
  
  // Éteindre la NeoPixel au démarrage
  if (strip != nullptr) {
    strip->begin();
    strip->clear();
    strip->show();
  }
  
  detectOLED();
  
  if (ENABLE_I2C_SCAN) {
    scanI2C();
  }
  
  scanSPI();
  listPartitions();
  
  collectDiagnosticInfo();
  collectDetailedMemory();

  bool bleStarted = startBluetooth();
  syncBluetoothDiagnostics();
#if BLE_STACK_SUPPORTED
  Serial.println(bleStarted ? "[BLE] Publicité démarrée" : "[BLE] Service indisponible");
#else
  Serial.println("[BLE] Pile BLE non compilée");
#endif

  // ========== ROUTES SERVEUR ==========
  server.on("/", handleRoot);
  server.on("/js/app.js", handleJavaScript);

  // **NOUVELLES ROUTES MULTILINGUES**
  server.on("/api/set-language", handleSetLanguage);
  server.on("/api/get-translations", handleGetTranslations);

  // Data endpoints
  server.on("/api/status", handleStatus);
  server.on("/api/overview", handleOverview);
  server.on("/api/system-info", handleSystemInfo);
  server.on("/api/memory", handleMemory);
  server.on("/api/wifi-info", handleWiFiInfo);
  server.on("/api/peripherals", handlePeripherals);
  server.on("/api/leds-info", handleLedsInfo);
  server.on("/api/screens-info", handleScreensInfo);

  // GPIO & WiFi
  server.on("/api/test-gpio", handleTestGPIO);
  server.on("/api/wifi-scan", handleWiFiScan);
  server.on("/api/i2c-scan", handleI2CScan);

  // Bluetooth
  server.on("/api/bluetooth/status", handleBluetoothStatus);
  server.on("/api/bluetooth/toggle", handleBluetoothToggle);
  server.on("/api/bluetooth/name", handleBluetoothName);
  server.on("/api/bluetooth/reset", handleBluetoothReset);
  
  // LED intégrée
  server.on("/api/builtin-led-config", handleBuiltinLEDConfig);
  server.on("/api/builtin-led-test", handleBuiltinLEDTest);
  server.on("/api/builtin-led-control", handleBuiltinLEDControl);
  
  // NeoPixel
  server.on("/api/neopixel-config", handleNeoPixelConfig);
  server.on("/api/neopixel-test", handleNeoPixelTest);
  server.on("/api/neopixel-pattern", handleNeoPixelPattern);
  server.on("/api/neopixel-color", handleNeoPixelColor);
  
  // Écrans
  server.on("/api/oled-test", handleOLEDTest);
  server.on("/api/oled-step", handleOLEDStep);
  server.on("/api/oled-message", handleOLEDMessage);
  server.on("/api/oled-config", handleOLEDConfig);
  
  // Tests avancés
  server.on("/api/adc-test", handleADCTest);
  server.on("/api/pwm-test", handlePWMTest);
  server.on("/api/spi-scan", handleSPIScan);
  server.on("/api/partitions-list", handlePartitionsList);
  server.on("/api/stress-test", handleStressTest);

  // --- [NEW FEATURE] Routes API pour les nouveaux capteurs ---
  server.on("/api/rgb-led-config", handleRGBLedConfig);
  server.on("/api/rgb-led-test", handleRGBLedTest);
  server.on("/api/rgb-led-color", handleRGBLedColor);

  server.on("/api/buzzer-config", handleBuzzerConfig);
  server.on("/api/buzzer-test", handleBuzzerTest);
  server.on("/api/buzzer-tone", handleBuzzerTone);

  server.on("/api/dht11-config", handleDHT11Config);
  server.on("/api/dht11-test", handleDHT11Test);

  server.on("/api/light-sensor-config", handleLightSensorConfig);
  server.on("/api/light-sensor-test", handleLightSensorTest);

  server.on("/api/distance-sensor-config", handleDistanceSensorConfig);
  server.on("/api/distance-sensor-test", handleDistanceSensorTest);

  server.on("/api/motion-sensor-config", handleMotionSensorConfig);
  server.on("/api/motion-sensor-test", handleMotionSensorTest);

  // Performance & Mémoire
  server.on("/api/benchmark", handleBenchmark);
  server.on("/api/memory-details", handleMemoryDetails);
  
  // Exports
  server.on("/export/txt", handleExportTXT);
  server.on("/export/json", handleExportJSON);
  server.on("/export/csv", handleExportCSV);
  server.on("/print", handlePrintVersion);
  
  server.begin();
  Serial.println("Serveur Web OK!");
  Serial.println("\r\n===============================================");
  Serial.println("            PRET - En attente");
  Serial.println("   Langue par defaut: FRANCAIS");
  Serial.println("   Changement dynamique via interface web");
  Serial.println("===============================================\r\n");
}

// ========== LOOP ==========
void loop() {
  server.handleClient();
  maintainNetworkServices();
#if DIAGNOSTIC_HAS_MDNS && defined(DIAGNOSTIC_USES_ESPMDNS)
  if (mdnsServiceActive) {
    DIAGNOSTIC_MDNS_SERVER.update();
  }
#endif

  static unsigned long lastUpdate = 0;
  if (millis() - lastUpdate > 30000) {
    lastUpdate = millis();
    collectDiagnosticInfo();
    
    Serial.println("\r\n=== UPDATE ===");
    Serial.printf("Heap: %.2f KB | Uptime: %.2f h\r\n", 
                  diagnosticData.freeHeap / 1024.0, 
                  diagnosticData.uptime / 3600000.0);
    if (diagnosticData.temperature != -999) {
      Serial.printf("Temp: %.1f°C\r\n", diagnosticData.temperature);
    }
  }

#if BLE_STACK_SUPPORTED
  dispatchBluetoothTelemetry();
#endif

  delay(10);
}
