/*
 * ESP32 Diagnostic Suite v3.6.24-dev
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
  "3.6.24-dev - Reduce firmware size by trimming version history to last 15 entries",
  "3.6.23-dev - Replace template with classic function to fix 'N' not declared error",
  "3.6.22-dev - Remove duplicate async test runner definitions to fix compilation errors",
  "3.6.21-dev - Restore async test runner declarations for compilation",
  "3.6.20-dev - Reduce hardware test latency with faster routines",
  "3.6.19-dev - Compact translation JSON map to shrink firmware size",
  "3.6.18-dev - Restore language switch helper after translation refactor",
  "3.6.17-dev - Segment translation JSON builder to fix relocation overflow",
  "3.6.16-dev - Add configurable LED and NeoPixel pins via web UI, implement chase pattern",
  "3.6.15-dev - Add missing UI translations across wireless, sensors, export, and display tabs",
  "3.6.14-dev - Fix Bluetooth scan UI escaping for compilation",
  "3.6.13-dev - Fix translation map continuation and stabilize BLE scanning",
  "3.6.12-dev - Add Bluetooth overview metrics and wireless controls",
  "3.6.11-dev - Provide MDNS_HOSTNAME_STR definition for consistent linkage",
  "3.6.10-dev - Replace handleRoot() to use modern generateHTML() with dynamic tab navigation"
};

#undef DIAGNOSTIC_UNUSED

#include <WiFi.h>
#include <WiFiMulti.h>
#include <WebServer.h>

#if defined(ARDUINO_ARCH_ESP32)
  #include <freertos/FreeRTOS.h>
  #include <freertos/task.h>
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
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>

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
#include <string>
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

Language currentLanguage = LANG_FR;

struct TranslationJsonEntry;

// --- [BUGFIX] Restauration du sélecteur de langue ---
static inline void setLanguage(Language lang) {
  switch (lang) {
    case LANG_FR:
    case LANG_EN:
      currentLanguage = lang;
      break;
    default:
      break;
  }
}

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
#define DIAGNOSTIC_VERSION "3.6.24-dev"
#define DIAGNOSTIC_HOSTNAME "esp32-diagnostic"
#define CUSTOM_LED_PIN -1
#define CUSTOM_LED_COUNT 1
#define ENABLE_I2C_SCAN true
const char* DIAGNOSTIC_VERSION_STR = DIAGNOSTIC_VERSION;
// --- [BUGFIX] Définition centralisée du nom d'hôte mDNS ---
const char* MDNS_HOSTNAME_STR = DIAGNOSTIC_HOSTNAME;

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

// --- [NEW FEATURE] Exécution asynchrone des tests matériels ---
typedef void (*TestRoutine)();

struct AsyncTestRunner {
  const char* taskName;
  TaskHandle_t taskHandle;
  volatile bool running;
};

struct AsyncTestTaskArgs {
  AsyncTestRunner* runner;
  TestRoutine routine;
};

static void asyncTestTask(void* parameters) {
  AsyncTestTaskArgs* args = static_cast<AsyncTestTaskArgs*>(parameters);
  if (args && args->routine) {
    args->routine();
  }
  if (args) {
    if (args->runner) {
      args->runner->running = false;
      args->runner->taskHandle = nullptr;
    }
    delete args;
  }
  vTaskDelete(nullptr);
}

static bool startAsyncTest(AsyncTestRunner& runner,
                           TestRoutine routine,
                           bool& alreadyRunning,
                           uint32_t stackSize = 4096,
                           UBaseType_t priority = 1) {
  alreadyRunning = runner.running;
  if (runner.running) {
    return false;
  }

  AsyncTestTaskArgs* args = new AsyncTestTaskArgs{&runner, routine};
  runner.running = true;

#if CONFIG_FREERTOS_UNICORE
  const BaseType_t targetCore = tskNO_AFFINITY;
#else
  const BaseType_t targetCore = 1;
#endif

  BaseType_t result = xTaskCreatePinnedToCore(asyncTestTask,
                                              runner.taskName,
                                              stackSize,
                                              args,
                                              priority,
                                              &runner.taskHandle,
                                              targetCore);
  if (result != pdPASS) {
    runner.running = false;
    runner.taskHandle = nullptr;
    delete args;
    alreadyRunning = false;
    return false;
  }

  alreadyRunning = false;
  return true;
}

// --- [NEW FEATURE] Orchestrateurs asynchrones des tests lents ---
static AsyncTestRunner builtinLedTestRunner = {"BuiltinLEDTest", nullptr, false};
static AsyncTestRunner neopixelTestRunner = {"NeoPixelTest", nullptr, false};
static AsyncTestRunner oledTestRunner = {"OLEDTest", nullptr, false};
static AsyncTestRunner rgbLedTestRunner = {"RgbLedTest", nullptr, false};
static AsyncTestRunner buzzerTestRunner = {"BuzzerTest", nullptr, false};

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

// --- [BUGFIX] Include web interface after DiagnosticInfo definition ---
#include "web_interface.h"

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
  delayMicroseconds(250);
  bool highOk = digitalRead(pin) == HIGH;

  digitalWrite(pin, LOW);
  delayMicroseconds(250);
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
    delay(80);
    digitalWrite(BUILTIN_LED_PIN, LOW);
    delay(80);
  }

  for(int i = 0; i <= 255; i += 51) {
    analogWrite(BUILTIN_LED_PIN, i);
    delay(25);
    yield();
  }
  for(int i = 255; i >= 0; i -= 51) {
    analogWrite(BUILTIN_LED_PIN, i);
    delay(25);
    yield();
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
  delay(160);

  strip->setPixelColor(0, strip->Color(0, 255, 0));
  strip->show();
  delay(160);

  strip->setPixelColor(0, strip->Color(0, 0, 255));
  strip->show();
  delay(160);

  for(int i = 0; i < 256; i += 64) {
    strip->setPixelColor(0, strip->gamma32(strip->ColorHSV(i * 256)));
    strip->show();
    delay(28);
    yield();
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
    delay(120);
    strip->clear();
    strip->show();
    delay(120);
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

// --- [NEW FEATURE] Effet chenillard pour NeoPixel ---
void neopixelChase() {
  if (!strip) return;
  uint32_t colors[] = {
    strip->Color(255, 0, 0),    // Rouge
    strip->Color(0, 255, 0),    // Vert
    strip->Color(0, 0, 255),    // Bleu
    strip->Color(255, 255, 0),  // Jaune
    strip->Color(255, 0, 255),  // Magenta
    strip->Color(0, 255, 255)   // Cyan
  };
  int numColors = sizeof(colors) / sizeof(colors[0]);

  for(int cycle = 0; cycle < 3; cycle++) {
    for(int colorIndex = 0; colorIndex < numColors; colorIndex++) {
      for(int pos = 0; pos < LED_COUNT; pos++) {
        strip->clear();
        for(int i = 0; i < LED_COUNT; i++) {
          if((i + pos) % 3 == 0) {
            strip->setPixelColor(i, colors[colorIndex]);
          }
        }
        strip->show();
        delay(100);
      }
    }
  }
  strip->clear();
  strip->show();
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
  delay(700);
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
  delay(450);
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
  delay(550);
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
  delay(550);
}

void oledStepHorizontalLines() {
  if (!oledAvailable) return;
  applyOLEDOrientation();
  oled.clearDisplay();
  for (int i = 0; i < SCREEN_HEIGHT; i += 4) {
    oled.drawLine(0, i, SCREEN_WIDTH, i, SSD1306_WHITE);
  }
  oled.display();
  delay(350);
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
  delay(350);
}

void oledStepMovingSquare() {
  if (!oledAvailable) return;
  applyOLEDOrientation();
  for (int x = 0; x < SCREEN_WIDTH - 20; x += 6) {
    oled.clearDisplay();
    oled.fillRect(x, 22, 20, 20, SSD1306_WHITE);
    oled.display();
    delay(12);
    yield();
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
  for (int i = 0; i <= 100; i += 10) {
    oled.drawRect(10, 30, 108, 15, SSD1306_WHITE);
    oled.fillRect(12, 32, i, 11, SSD1306_WHITE);
    oled.setCursor(45, 50);
    oled.printf("%d%%", i);
    oled.display();
    delay(45);
    yield();
    if (i < 100) {
      oled.fillRect(12, 32, i, 11, SSD1306_BLACK);
      oled.fillRect(45, 50, 40, 10, SSD1306_BLACK);
    }
  }
  delay(300);
}

void oledStepScrollText() {
  if (!oledAvailable) return;
  applyOLEDOrientation();
  String scrollText = "  DIAGNOSTIC ESP32 COMPLET - OLED 0.96 pouces I2C  ";
  for (int offset = 0; offset < scrollText.length() * 6; offset += 6) {
    oled.clearDisplay();
    oled.setTextSize(1);
    oled.setTextColor(SSD1306_WHITE);
    oled.setCursor(-offset, 28);
    oled.print(scrollText);
    oled.display();
    delay(12);
    yield();
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
  delay(600);
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
    delay(80);
    yield();
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
  delay(120);

  digitalWrite(RGB_LED_PIN_R, HIGH);
  delay(150);
  digitalWrite(RGB_LED_PIN_R, LOW);

  digitalWrite(RGB_LED_PIN_G, HIGH);
  delay(150);
  digitalWrite(RGB_LED_PIN_G, LOW);

  digitalWrite(RGB_LED_PIN_B, HIGH);
  delay(150);
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

  tone(BUZZER_PIN, 1000, 160);
  delay(220);
  tone(BUZZER_PIN, 1500, 160);
  delay(220);
  tone(BUZZER_PIN, 2000, 160);
  delay(220);
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
  const int samples = 6;
  for (int i = 0; i < samples; i++) {
    sum += analogRead(LIGHT_SENSOR_PIN);
    delay(8);
    yield();
  }
  lightSensorValue = sum / samples;

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

  delay(30);
  yield();
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

// --- [NEW FEATURE] Routines de tests en tâche de fond ---
static void runBuiltinLedTestTask() {
  resetBuiltinLEDTest();
  testBuiltinLED();
}

static void runNeopixelTestTask() {
  resetNeoPixelTest();
  testNeoPixel();
}

static void runOledTestTask() {
  resetOLEDTest();
  testOLED();
}

static void runRgbLedTestTask() {
  testRGBLed();
}

static void runBuzzerTestTask() {
  testBuzzer();
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
  bool alreadyRunning = false;
  bool started = startAsyncTest(builtinLedTestRunner, runBuiltinLedTestTask, alreadyRunning);

  if (started) {
    sendActionResponse(202, true, String(T().test_in_progress), {
      jsonBoolField("running", true),
      jsonStringField("result", builtinLedTestResult)
    });
    return;
  }

  if (alreadyRunning) {
    sendActionResponse(200, true, String(T().test_in_progress), {
      jsonBoolField("running", true),
      jsonStringField("result", builtinLedTestResult)
    });
    return;
  }

  resetBuiltinLEDTest();
  testBuiltinLED();
  sendActionResponse(200, builtinLedAvailable, builtinLedTestResult, {
    jsonBoolField("running", false),
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
  bool alreadyRunning = false;
  bool started = startAsyncTest(neopixelTestRunner, runNeopixelTestTask, alreadyRunning);

  if (started) {
    sendActionResponse(202, true, String(T().test_in_progress), {
      jsonBoolField("running", true),
      jsonBoolField("available", neopixelAvailable),
      jsonStringField("result", neopixelTestResult)
    });
    return;
  }

  if (alreadyRunning) {
    sendActionResponse(200, true, String(T().test_in_progress), {
      jsonBoolField("running", true),
      jsonBoolField("available", neopixelAvailable),
      jsonStringField("result", neopixelTestResult)
    });
    return;
  }

  resetNeoPixelTest();
  testNeoPixel();
  sendActionResponse(200, neopixelAvailable, neopixelTestResult, {
    jsonBoolField("running", false),
    jsonBoolField("available", neopixelAvailable),
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
  } else if (pattern == "chase") {
    neopixelChase();
    message = String(T().chase) + " " + String(T().ok);
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
  bool alreadyRunning = false;
  bool started = startAsyncTest(oledTestRunner, runOledTestTask, alreadyRunning, 6144, 1);

  if (started) {
    sendActionResponse(202, true, String(T().test_in_progress), {
      jsonBoolField("running", true),
      jsonBoolField("available", oledAvailable),
      jsonStringField("result", oledTestResult)
    });
    return;
  }

  if (alreadyRunning) {
    sendActionResponse(200, true, String(T().test_in_progress), {
      jsonBoolField("running", true),
      jsonBoolField("available", oledAvailable),
      jsonStringField("result", oledTestResult)
    });
    return;
  }

  resetOLEDTest();
  testOLED();
  sendActionResponse(200, oledAvailable, oledTestResult, {
    jsonBoolField("running", false),
    jsonBoolField("available", oledAvailable),
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
  bool alreadyRunning = false;
  bool started = startAsyncTest(rgbLedTestRunner, runRgbLedTestTask, alreadyRunning, 3072, 1);

  if (started) {
    sendJsonResponse(202, {
      jsonBoolField("running", true),
      jsonBoolField("success", rgbLedAvailable),
      jsonStringField("result", rgbLedTestResult)
    });
    return;
  }

  if (alreadyRunning) {
    sendJsonResponse(200, {
      jsonBoolField("running", true),
      jsonBoolField("success", rgbLedAvailable),
      jsonStringField("result", rgbLedTestResult)
    });
    return;
  }

  testRGBLed();
  sendJsonResponse(200, {
    jsonBoolField("running", false),
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
  bool alreadyRunning = false;
  bool started = startAsyncTest(buzzerTestRunner, runBuzzerTestTask, alreadyRunning, 3072, 1);

  if (started) {
    sendJsonResponse(202, {
      jsonBoolField("running", true),
      jsonBoolField("success", buzzerAvailable),
      jsonStringField("result", buzzerTestResult)
    });
    return;
  }

  if (alreadyRunning) {
    sendJsonResponse(200, {
      jsonBoolField("running", true),
      jsonBoolField("success", buzzerAvailable),
      jsonStringField("result", buzzerTestResult)
    });
    return;
  }

  testBuzzer();
  sendJsonResponse(200, {
    jsonBoolField("running", false),
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
  collectDiagnosticInfo();
  collectDetailedMemory();

  bool wifiConnected = (WiFi.status() == WL_CONNECTED);
  unsigned long currentUptime = millis();

  String btStatus = getBluetoothStateLabel();
  String btAdvertisingLabel = (bluetoothCapable && bluetoothAdvertising)
                                  ? String(T().bluetooth_advertising)
                                  : String(T().bluetooth_not_advertising);
  const char* btSupportKey = getBluetoothSupportKey();
  const char* btStatusKey = getBluetoothStateKey();
  const char* btAdvKey = getBluetoothAdvertisingKey();
#if BLE_STACK_SUPPORTED
  bool btConnected = bluetoothCapable && bluetoothClientConnected;
  const char* btConnectionKey = getBluetoothConnectionKey();
  String btConnectionLabel = btConnected ? String(T().bluetooth_client_connected)
                                         : String(T().bluetooth_client_disconnected);
#else
  bool btConnected = false;
  const char* btConnectionKey = "bluetooth_client_disconnected";
  String btConnectionLabel = String(T().bluetooth_client_disconnected);
#endif

  String json;
  json.reserve(400);
  json = "{";
  json += "\"connected\":" + String(wifiConnected ? "true" : "false") + ",";
  json += "\"uptime\":" + String(currentUptime) + ",";
  json += "\"temperature\":" + String(diagnosticData.temperature) + ",";
  json += "\"sram\":{\"total\":" + String(detailedMemory.sramTotal) +
          ",\"free\":" + String(detailedMemory.sramFree) +
          ",\"used\":" + String(detailedMemory.sramUsed) + "},";
  json += "\"psram\":{\"total\":" + String(detailedMemory.psramTotal) +
          ",\"free\":" + String(detailedMemory.psramFree) +
          ",\"used\":" + String(detailedMemory.psramUsed) + "},";
  json += "\"fragmentation\":" + String(detailedMemory.fragmentationPercent, 1) + ",";
  json += "\"bluetooth\":{";
  json += "\"supported\":" + String(bluetoothCapable ? "true" : "false") + ",";
  json += "\"enabled\":" + String((bluetoothCapable && bluetoothEnabled) ? "true" : "false") + ",";
  json += "\"advertising\":" + String((bluetoothCapable && bluetoothAdvertising) ? "true" : "false") + ",";
  json += "\"connected\":" + String(btConnected ? "true" : "false") + ",";
  json += "\"support_key\":\"" + String(btSupportKey) + "\",";
  json += "\"status_key\":\"" + String(btStatusKey) + "\",";
  json += "\"advertising_key\":\"" + String(btAdvKey) + "\",";
  json += "\"connection_key\":\"" + String(btConnectionKey) + "\",";
  json += "\"status\":\"" + jsonEscape(btStatus.c_str()) + "\",";
  json += "\"advertising_label\":\"" + jsonEscape(btAdvertisingLabel.c_str()) + "\",";
  json += "\"connection_label\":\"" + jsonEscape(btConnectionLabel.c_str()) + "\",";
  json += "\"name\":\"" + jsonEscape(bluetoothDeviceName.c_str()) + "\",";
  json += "\"mac\":\"" + jsonEscape(diagnosticData.bluetoothAddress.c_str()) + "\"";
  json += "}";
  json += "}";

  server.send(200, "application/json", json);
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

  // --- [NEW FEATURE] Synthèse Bluetooth ---
  String btStatus = getBluetoothStateLabel();
  String btAdvertisingLabel = (bluetoothCapable && bluetoothAdvertising)
                                  ? String(T().bluetooth_advertising)
                                  : String(T().bluetooth_not_advertising);
  const char* btStatusKey = getBluetoothStateKey();
  const char* btAdvKey = getBluetoothAdvertisingKey();
  const char* btSupportKey = getBluetoothSupportKey();
#if BLE_STACK_SUPPORTED
  bool btConnected = bluetoothCapable && bluetoothClientConnected;
  const char* btConnectionKey = getBluetoothConnectionKey();
  String btConnectionLabel = btConnected ? String(T().bluetooth_client_connected)
                                         : String(T().bluetooth_client_disconnected);
#else
  bool btConnected = false;
  const char* btConnectionKey = "bluetooth_client_disconnected";
  String btConnectionLabel = String(T().bluetooth_client_disconnected);
#endif
  json += "\"bluetooth\":{";
  json += "\"supported\":" + String(bluetoothCapable ? "true" : "false") + ",";
  json += "\"enabled\":" + String((bluetoothCapable && bluetoothEnabled) ? "true" : "false") + ",";
  json += "\"advertising\":" + String((bluetoothCapable && bluetoothAdvertising) ? "true" : "false") + ",";
  json += "\"connected\":" + String(btConnected ? "true" : "false") + ",";
  json += "\"support_key\":\"" + String(btSupportKey) + "\",";
  json += "\"status_key\":\"" + String(btStatusKey) + "\",";
  json += "\"advertising_key\":\"" + String(btAdvKey) + "\",";
  json += "\"connection_key\":\"" + String(btConnectionKey) + "\",";
  json += "\"status\":\"" + jsonEscape(btStatus.c_str()) + "\",";
  json += "\"advertising_label\":\"" + jsonEscape(btAdvertisingLabel.c_str()) + "\",";
  json += "\"connection_label\":\"" + jsonEscape(btConnectionLabel.c_str()) + "\",";
  json += "\"name\":\"" + jsonEscape(bluetoothDeviceName.c_str()) + "\",";
  json += "\"mac\":\"" + jsonEscape(diagnosticData.bluetoothAddress.c_str()) + "\"";
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

// --- [NEW FEATURE] Table de mapping JSON compacte ---
struct TranslationJsonEntry {
  const char* key;
  TranslationField TranslationAccessor::* field;
};

static inline void appendTranslationEntry(String& json,
                                          const TranslationAccessor& translations,
                                          const TranslationJsonEntry& entry) {
  if (json.length() > 1) {
    json += ',';
  }

  json += '"';
  json += entry.key;
  json += "\":\"";
  json += jsonEscape((translations.*(entry.field)).c_str());
  json += '"';
}

static inline void appendTranslationEntries(
    String& json,
    const TranslationAccessor& translations,
    const TranslationJsonEntry* entries,
    size_t count) {
  for (size_t i = 0; i < count; ++i) {
    appendTranslationEntry(json, translations, entries[i]);
  }
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

// --- [NEW FEATURE] Clefs i18n pour l'état Bluetooth ---
const char* getBluetoothStateKey() {
  if (!bluetoothCapable) {
    return "bluetooth_not_supported";
  }
  if (!bluetoothEnabled) {
    return "bluetooth_disabled";
  }
#if BLE_STACK_SUPPORTED
  if (bluetoothClientConnected) {
    return "bluetooth_enabled";
  }
#endif
  if (bluetoothAdvertising) {
    return "bluetooth_advertising";
  }
  return "bluetooth_not_advertising";
}

const char* getBluetoothAdvertisingKey() {
  if (!bluetoothCapable) {
    return "bluetooth_not_supported";
  }
  return bluetoothAdvertising ? "bluetooth_advertising" : "bluetooth_not_advertising";
}

const char* getBluetoothSupportKey() {
  return bluetoothCapable ? "bluetooth_support_yes" : "bluetooth_support_no";
}

#if BLE_STACK_SUPPORTED
const char* getBluetoothConnectionKey() {
  return bluetoothClientConnected ? "bluetooth_client_connected" : "bluetooth_client_disconnected";
}
#endif

String buildBluetoothJSON(bool success, const String& message) {
  syncBluetoothDiagnostics();

  String status = getBluetoothStateLabel();
  String summary = getBluetoothSummaryLabel();
  String advertisingLabel = (bluetoothCapable && bluetoothAdvertising)
                                ? String(T().bluetooth_advertising)
                                : String(T().bluetooth_not_advertising);
#if BLE_STACK_SUPPORTED
  bool connected = bluetoothCapable && bluetoothClientConnected;
  String connectionLabel = connected ? String(T().bluetooth_client_connected)
                                     : String(T().bluetooth_client_disconnected);
  const char* connectionKey = getBluetoothConnectionKey();
#else
  bool connected = false;
  String connectionLabel = String(T().bluetooth_client_disconnected);
  const char* connectionKey = "bluetooth_client_disconnected";
#endif

  String json = "{";
  json += "\"success\":" + String(success ? "true" : "false") + ",";
  json += "\"supported\":" + String(bluetoothCapable ? "true" : "false") + ",";
  json += "\"enabled\":" + String((bluetoothCapable && bluetoothEnabled) ? "true" : "false") + ",";
  json += "\"advertising\":" + String((bluetoothCapable && bluetoothAdvertising) ? "true" : "false") + ",";
  json += "\"connected\":" + String(connected ? "true" : "false") + ",";
  json += "\"support_key\":\"" + jsonEscape(getBluetoothSupportKey()) + "\",";
  json += "\"status_key\":\"" + jsonEscape(getBluetoothStateKey()) + "\",";
  json += "\"advertising_key\":\"" + jsonEscape(getBluetoothAdvertisingKey()) + "\",";
  json += "\"connection_key\":\"" + jsonEscape(connectionKey) + "\",";
  json += "\"name\":\"" + jsonEscape(bluetoothDeviceName.c_str()) + "\",";
  json += "\"mac\":\"" + jsonEscape(diagnosticData.bluetoothAddress.c_str()) + "\",";
  json += "\"status\":\"" + jsonEscape(status.c_str()) + "\",";
  json += "\"summary\":\"" + jsonEscape(summary.c_str()) + "\",";
  json += "\"advertising_label\":\"" + jsonEscape(advertisingLabel.c_str()) + "\",";
  json += "\"connection_label\":\"" + jsonEscape(connectionLabel.c_str()) + "\",";
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

// --- [BUGFIX] Helpers to abstract BLE scan results across stacks ---
static int getScanResultCount(BLEScanResults& results) {
  return results.getCount();
}

static int getScanResultCount(BLEScanResults* results) {
  return (results != nullptr) ? results->getCount() : 0;
}

static BLEAdvertisedDevice getScanResultDevice(BLEScanResults& results, int index) {
  return results.getDevice(index);
}

static BLEAdvertisedDevice getScanResultDevice(BLEScanResults* results, int index) {
  return (results != nullptr) ? results->getDevice(index) : BLEAdvertisedDevice();
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

// --- [NEW FEATURE] Segmentation du JSON de traduction ---
static inline void appendOverviewTranslations(String& json, const TranslationAccessor& translations) {
  static const TranslationJsonEntry kEntries[] = {
      {"title", &TranslationAccessor::title},
      {"version", &TranslationAccessor::version},
      {"nav_overview", &TranslationAccessor::nav_overview},
      {"nav_leds", &TranslationAccessor::nav_leds},
      {"nav_screens", &TranslationAccessor::nav_screens},
      {"nav_tests", &TranslationAccessor::nav_tests},
      {"nav_gpio", &TranslationAccessor::nav_gpio},
      {"nav_wireless", &TranslationAccessor::nav_wireless},
      {"nav_benchmark", &TranslationAccessor::nav_benchmark},
      {"nav_export", &TranslationAccessor::nav_export},
      {"nav_select_label", &TranslationAccessor::nav_select_label},
      {"chip_info", &TranslationAccessor::chip_info},
      {"full_model", &TranslationAccessor::full_model},
      {"cpu_cores", &TranslationAccessor::cpu_cores},
      {"mac_wifi", &TranslationAccessor::mac_wifi},
      {"last_reset", &TranslationAccessor::last_reset},
      {"poweron", &TranslationAccessor::poweron},
      {"software_reset", &TranslationAccessor::software_reset},
      {"deepsleep_exit", &TranslationAccessor::deepsleep_exit},
      {"brownout", &TranslationAccessor::brownout},
      {"other", &TranslationAccessor::other},
      {"chip_features", &TranslationAccessor::chip_features},
      {"sdk_version", &TranslationAccessor::sdk_version},
      {"idf_version", &TranslationAccessor::idf_version},
      {"uptime", &TranslationAccessor::uptime},
      {"cpu_temp", &TranslationAccessor::cpu_temp},
      {"revision", &TranslationAccessor::revision},
      {"days", &TranslationAccessor::days},
      {"hours", &TranslationAccessor::hours},
      {"minutes", &TranslationAccessor::minutes},
      {"memory_details", &TranslationAccessor::memory_details},
      {"flash_memory", &TranslationAccessor::flash_memory},
      {"real_size", &TranslationAccessor::real_size},
      {"configured_ide", &TranslationAccessor::configured_ide},
      {"configuration", &TranslationAccessor::configuration},
      {"correct", &TranslationAccessor::correct},
      {"to_fix", &TranslationAccessor::to_fix},
      {"flash_type", &TranslationAccessor::flash_type},
      {"flash_speed", &TranslationAccessor::flash_speed},
      {"psram_external", &TranslationAccessor::psram_external},
      {"hardware_status", &TranslationAccessor::hardware_status},
      {"detected_active", &TranslationAccessor::detected_active},
      {"supported_not_enabled", &TranslationAccessor::supported_not_enabled},
      {"ide_config", &TranslationAccessor::ide_config},
      {"total_size", &TranslationAccessor::total_size},
      {"free", &TranslationAccessor::free},
      {"used", &TranslationAccessor::used},
      {"excellent", &TranslationAccessor::excellent},
      {"very_good", &TranslationAccessor::very_good},
      {"good", &TranslationAccessor::good},
      {"warning", &TranslationAccessor::warning},
      {"critical", &TranslationAccessor::critical},
      {"weak", &TranslationAccessor::weak},
      {"very_weak", &TranslationAccessor::very_weak},
      {"none", &TranslationAccessor::none},
      {"unknown", &TranslationAccessor::unknown},
      {"enable_psram_hint", &TranslationAccessor::enable_psram_hint},
      {"not_detected", &TranslationAccessor::not_detected},
      {"internal_sram", &TranslationAccessor::internal_sram},
      {"memory_fragmentation", &TranslationAccessor::memory_fragmentation},
      {"refresh_memory", &TranslationAccessor::refresh_memory},
      {"wifi_connection", &TranslationAccessor::wifi_connection},
      {"connected_ssid", &TranslationAccessor::connected_ssid},
      {"signal_power", &TranslationAccessor::signal_power},
      {"signal_quality", &TranslationAccessor::signal_quality},
      {"ip_address", &TranslationAccessor::ip_address},
      {"subnet_mask", &TranslationAccessor::subnet_mask},
      {"gateway", &TranslationAccessor::gateway},
      {"dns", &TranslationAccessor::dns},
      {"ip_unavailable", &TranslationAccessor::ip_unavailable},
      {"access", &TranslationAccessor::access},
      {"bluetooth_section", &TranslationAccessor::bluetooth_section},
      {"bluetooth_status", &TranslationAccessor::bluetooth_status},
      {"bluetooth_name", &TranslationAccessor::bluetooth_name},
      {"bluetooth_mac", &TranslationAccessor::bluetooth_mac},
      {"bluetooth_actions", &TranslationAccessor::bluetooth_actions},
      {"bluetooth_enable", &TranslationAccessor::bluetooth_enable},
      {"bluetooth_disable", &TranslationAccessor::bluetooth_disable},
      {"bluetooth_rename", &TranslationAccessor::bluetooth_rename},
      {"bluetooth_reset", &TranslationAccessor::bluetooth_reset},
      {"bluetooth_placeholder", &TranslationAccessor::bluetooth_placeholder},
      {"bluetooth_not_supported", &TranslationAccessor::bluetooth_not_supported},
      {"bluetooth_disabled", &TranslationAccessor::bluetooth_disabled},
      {"bluetooth_enabled", &TranslationAccessor::bluetooth_enabled},
      {"bluetooth_advertising", &TranslationAccessor::bluetooth_advertising},
      {"bluetooth_not_advertising", &TranslationAccessor::bluetooth_not_advertising},
  };
  appendTranslationEntries(json, translations, kEntries, sizeof(kEntries) / sizeof(kEntries[0]));
}

static inline void appendBluetoothScanTranslations(String& json, const TranslationAccessor& translations) {
  static const TranslationJsonEntry kEntries[] = {
      {"bluetooth_scan", &TranslationAccessor::bluetooth_scan},
      {"bluetooth_scan_hint", &TranslationAccessor::bluetooth_scan_hint},
      {"bluetooth_scan_in_progress", &TranslationAccessor::bluetooth_scan_in_progress},
      {"bluetooth_devices_found", &TranslationAccessor::bluetooth_devices_found},
      {"bluetooth_updated", &TranslationAccessor::bluetooth_updated},
      {"bluetooth_error", &TranslationAccessor::bluetooth_error},
      {"bluetooth_reset_done", &TranslationAccessor::bluetooth_reset_done},
      {"bluetooth_support_label", &TranslationAccessor::bluetooth_support_label},
      {"bluetooth_support_yes", &TranslationAccessor::bluetooth_support_yes},
      {"bluetooth_support_no", &TranslationAccessor::bluetooth_support_no},
      {"bluetooth_advertising_label", &TranslationAccessor::bluetooth_advertising_label},
      {"bluetooth_connection_label", &TranslationAccessor::bluetooth_connection_label},
      {"bluetooth_client_connected", &TranslationAccessor::bluetooth_client_connected},
      {"bluetooth_client_disconnected", &TranslationAccessor::bluetooth_client_disconnected},
      {"bluetooth_notifications_label", &TranslationAccessor::bluetooth_notifications_label},
  };
  appendTranslationEntries(json, translations, kEntries, sizeof(kEntries) / sizeof(kEntries[0]));
}

static inline void appendHardwareUiTranslations(String& json, const TranslationAccessor& translations) {
  static const TranslationJsonEntry kEntries[] = {
      {"gpio", &TranslationAccessor::gpio},
      {"status", &TranslationAccessor::status},
      {"config", &TranslationAccessor::config},
      {"test", &TranslationAccessor::test},
      {"turn_on", &TranslationAccessor::turn_on},
      {"turn_off", &TranslationAccessor::turn_off},
      {"turn_off_all", &TranslationAccessor::turn_off_all},
      {"blink", &TranslationAccessor::blink},
      {"fade", &TranslationAccessor::fade},
      {"off", &TranslationAccessor::off},
      {"neopixel", &TranslationAccessor::neopixel},
      {"animations", &TranslationAccessor::animations},
      {"led_count", &TranslationAccessor::led_count},
      {"rainbow", &TranslationAccessor::rainbow},
      {"chase", &TranslationAccessor::chase},
      {"color", &TranslationAccessor::color},
      {"custom_color", &TranslationAccessor::custom_color},
      {"gpio_interfaces", &TranslationAccessor::gpio_interfaces},
      {"total_gpio", &TranslationAccessor::total_gpio},
      {"i2c_peripherals", &TranslationAccessor::i2c_peripherals},
      {"device_count", &TranslationAccessor::device_count},
      {"detected_addresses", &TranslationAccessor::detected_addresses},
      {"builtin_led", &TranslationAccessor::builtin_led},
      {"oled_screen", &TranslationAccessor::oled_screen},
      {"status", &TranslationAccessor::status},
      {"i2c_pins", &TranslationAccessor::i2c_pins},
      {"rotation", &TranslationAccessor::rotation},
      {"apply_redetect", &TranslationAccessor::apply_redetect},
      {"full_test", &TranslationAccessor::full_test},
      {"oled_step_welcome", &TranslationAccessor::oled_step_welcome},
      {"oled_step_big_text", &TranslationAccessor::oled_step_big_text},
      {"oled_step_text_sizes", &TranslationAccessor::oled_step_text_sizes},
      {"oled_step_shapes", &TranslationAccessor::oled_step_shapes},
      {"oled_step_horizontal_lines", &TranslationAccessor::oled_step_horizontal_lines},
      {"oled_step_diagonals", &TranslationAccessor::oled_step_diagonals},
      {"oled_step_moving_square", &TranslationAccessor::oled_step_moving_square},
      {"oled_step_progress_bar", &TranslationAccessor::oled_step_progress_bar},
      {"oled_step_scroll_text", &TranslationAccessor::oled_step_scroll_text},
      {"oled_step_final_message", &TranslationAccessor::oled_step_final_message},
      {"oled_step_running", &TranslationAccessor::oled_step_running},
      {"oled_message_required", &TranslationAccessor::oled_message_required},
      {"oled_displaying_message", &TranslationAccessor::oled_displaying_message},
      {"custom_message", &TranslationAccessor::custom_message},
      {"show_message", &TranslationAccessor::show_message},
  };
  appendTranslationEntries(json, translations, kEntries, sizeof(kEntries) / sizeof(kEntries[0]));
}

static inline void appendTestFlowTranslations(String& json, const TranslationAccessor& translations) {
  static const TranslationJsonEntry kEntries[] = {
      {"apply_color", &TranslationAccessor::apply_color},
      {"adc_test", &TranslationAccessor::adc_test},
      {"start_adc_test", &TranslationAccessor::start_adc_test},
      {"pwm_test", &TranslationAccessor::pwm_test},
      {"spi_bus", &TranslationAccessor::spi_bus},
      {"flash_partitions", &TranslationAccessor::flash_partitions},
      {"memory_stress", &TranslationAccessor::memory_stress},
      {"stress_warning", &TranslationAccessor::stress_warning},
      {"start_stress", &TranslationAccessor::start_stress},
      {"gpio_test", &TranslationAccessor::gpio_test},
      {"test_all_gpio", &TranslationAccessor::test_all_gpio},
      {"click_to_test", &TranslationAccessor::click_to_test},
      {"not_tested", &TranslationAccessor::not_tested},
      {"gpio_warning", &TranslationAccessor::gpio_warning},
      {"i2c_desc", &TranslationAccessor::i2c_desc},
      {"adc_desc", &TranslationAccessor::adc_desc},
      {"pwm_desc", &TranslationAccessor::pwm_desc},
      {"spi_desc", &TranslationAccessor::spi_desc},
      {"partitions_desc", &TranslationAccessor::partitions_desc},
      {"stress_desc", &TranslationAccessor::stress_desc},
  };
  appendTranslationEntries(json, translations, kEntries, sizeof(kEntries) / sizeof(kEntries[0]));
}

static inline void appendStatusAndSummaryTranslations(String& json, const TranslationAccessor& translations) {
  static const TranslationJsonEntry kEntries[] = {
      {"gpio_desc", &TranslationAccessor::gpio_desc},
      {"wifi_desc", &TranslationAccessor::wifi_desc},
      {"benchmark_desc", &TranslationAccessor::benchmark_desc},
      {"wifi_scanner", &TranslationAccessor::wifi_scanner},
      {"performance_bench", &TranslationAccessor::performance_bench},
      {"data_export", &TranslationAccessor::data_export},
      {"click_to_scan", &TranslationAccessor::click_to_scan},
      {"updating", &TranslationAccessor::updating},
      {"online", &TranslationAccessor::online},
      {"offline", &TranslationAccessor::offline},
      {"check_network", &TranslationAccessor::check_network},
      {"language_label", &TranslationAccessor::language_label},
      {"language_updated", &TranslationAccessor::language_updated},
      {"language_switch_error", &TranslationAccessor::language_switch_error},
      {"translation_error", &TranslationAccessor::translation_error},
      {"bluetooth_invalid_name", &TranslationAccessor::bluetooth_invalid_name},
      {"bluetooth_enabling", &TranslationAccessor::bluetooth_enabling},
      {"bluetooth_disabling", &TranslationAccessor::bluetooth_disabling},
      {"bluetooth_updating", &TranslationAccessor::bluetooth_updating},
      {"bluetooth_resetting", &TranslationAccessor::bluetooth_resetting},
      {"loading", &TranslationAccessor::loading},
      {"configuring", &TranslationAccessor::configuring},
      {"reconfiguring", &TranslationAccessor::reconfiguring},
      {"transmission", &TranslationAccessor::transmission},
      {"error_label", &TranslationAccessor::error_label},
      {"test_failed", &TranslationAccessor::test_failed},
      {"gpio_summary_template", &TranslationAccessor::gpio_summary_template},
      {"wifi_networks_found", &TranslationAccessor::wifi_networks_found},
      {"i2c_scan_result", &TranslationAccessor::i2c_scan_result},
      {"gpio_invalid", &TranslationAccessor::gpio_invalid},
      {"configuration_invalid", &TranslationAccessor::configuration_invalid},
      {"testing", &TranslationAccessor::testing},
      {"scan", &TranslationAccessor::scan},
      {"scanning", &TranslationAccessor::scanning},
      {"completed", &TranslationAccessor::completed},
      {"cores", &TranslationAccessor::cores},
      {"pins", &TranslationAccessor::pins},
      {"devices", &TranslationAccessor::devices},
      {"networks", &TranslationAccessor::networks},
      {"tested", &TranslationAccessor::tested},
      {"channels", &TranslationAccessor::channels},
      {"oled_test_running", &TranslationAccessor::oled_test_running},
  };
  appendTranslationEntries(json, translations, kEntries, sizeof(kEntries) / sizeof(kEntries[0]));
}

String buildTranslationsJSON() {
  const TranslationAccessor translations = T();
  String json = "{";
  json.reserve(8000);  // Increased reserve to avoid reallocations (~200+ fields)

  appendOverviewTranslations(json, translations);
  appendBluetoothScanTranslations(json, translations);
  appendHardwareUiTranslations(json, translations);
  appendTestFlowTranslations(json, translations);
  appendStatusAndSummaryTranslations(json, translations);

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

// --- [NEW FEATURE] Scan Bluetooth depuis l'API ---
void handleBluetoothScan() {
#if BLE_STACK_SUPPORTED
  if (!bluetoothCapable) {
    server.send(200, "application/json", buildBluetoothJSON(false, String(T().bluetooth_not_supported)));
    return;
  }

  bool startedForScan = false;
  bool resumeAdvertising = false;
  BLEAdvertising* advertising = nullptr;

  if (!bluetoothEnabled) {
    if (!startBluetooth()) {
      server.send(200, "application/json", buildBluetoothJSON(false, String(T().bluetooth_error)));
      return;
    }
    startedForScan = true;
  }

  if (bluetoothAdvertising) {
    advertising = BLEDevice::getAdvertising();
    if (advertising) {
      advertising->stop();
    }
    bluetoothAdvertising = false;
    resumeAdvertising = true;
  }

  BLEScan* scanner = BLEDevice::getScan();
  if (!scanner) {
    if (resumeAdvertising && advertising) {
      advertising->start();
      BLEDevice::startAdvertising();
      bluetoothAdvertising = true;
    }
    if (startedForScan) {
      stopBluetooth();
    }
    server.send(200, "application/json", buildBluetoothJSON(false, String(T().bluetooth_error)));
    return;
  }

  scanner->setActiveScan(true);
  // --- [BUGFIX] Harmonise BLE scan results pointer/object handling ---
  auto rawResults = scanner->start(5, false);
  int count = getScanResultCount(rawResults);

  String devicesJson;
  devicesJson.reserve(static_cast<size_t>(count) * 80U);
  for (int i = 0; i < count; ++i) {
    BLEAdvertisedDevice device = getScanResultDevice(rawResults, i);
    String name = device.haveName() ? String(device.getName().c_str()) : String(T().unknown);
    String address = String(device.getAddress().toString().c_str());
    int rssi = device.getRSSI();
    if (i > 0) {
      devicesJson += ',';
    }
    devicesJson += "{\"name\":\"" + jsonEscape(name.c_str()) + "\",";
    devicesJson += "\"address\":\"" + jsonEscape(address.c_str()) + "\",";
    devicesJson += "\"rssi\":" + String(rssi) + "}";
  }

  scanner->stop();
  scanner->clearResults();

  if (resumeAdvertising && advertising) {
    advertising->start();
    BLEDevice::startAdvertising();
    bluetoothAdvertising = true;
  }

  if (startedForScan) {
    stopBluetooth();
  }

  String message = String(T().bluetooth_devices_found);
  message.replace("%COUNT%", String(count));
  message.replace("%count%", String(count));

  String base = buildBluetoothJSON(true, message);
  if (base.endsWith("}")) {
    base.remove(base.length() - 1);
    base += ",\"devices\":[";
    base += devicesJson;
    base += "]}";
  }
  server.send(200, "application/json", base);
#else
  server.send(200, "application/json", buildBluetoothJSON(false, String(T().bluetooth_error)));
#endif
}

// ========== INTERFACE WEB PRINCIPALE MULTILINGUE ==========
// --- [NEW FEATURE] Modern web interface with dynamic tabs ---
void handleRoot() {
  server.send(200, "text/html; charset=utf-8", generateHTML());
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
  // --- [NEW FEATURE] Endpoint scan Bluetooth ---
  server.on("/api/bluetooth/scan", handleBluetoothScan);
  
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
