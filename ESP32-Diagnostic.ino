/*
 * DIAGNOSTIC COMPLET ESP32 - VERSION MULTILINGUE v3.0.x
 * Compatible: ESP32, ESP32-S2, ESP32-S3, ESP32-C3, ESP32-C6, ESP32-H2
 * Optimisé pour ESP32 Arduino Core 3.3.2
 * Carte testée: ESP32-S3 avec PSRAM OPI
 * Auteur: morfredus
 *
 * Nouveautés v3.0.x:
 * - Migration complète vers Arduino Core 3.3.2 (WiFi & I2C actualisés)
 * - Analyse avancée du scan WiFi (bandes, PHY, largeur de bande)
 * - Réinitialisation I2C résiliente et auto-détection mise à jour
 */

// Version de dev : 3.0.16-dev - UI affinée, message inline sticky & avertissement GPIO
// Version de dev : 3.0.15-dev - Gestion Bluetooth & onglet Sans fil
// Version de dev : 3.0.14-dev - Menu horizontal sticky & reset alertes
// Version de dev : 3.0.13-dev - Correction JSON traductions dynamiques
// Version de dev : 3.0.12-dev - Navigation hash + compatibilité JS
// Version de dev : 3.0.11-dev - Refonte UI moderne responsive
// Version de dev : 3.0.10-dev - Restauration clic onglets via double liaison
// Version de dev : 3.0.09-dev - Délégation universelle des clics onglets
// Version de dev : 3.0.08-dev - Correction finale navigation onglets
// Version de dev : 3.0.07-dev - Réactivation complète navigation onglets
// Version de dev : 3.0.06-dev - Correction compilation fallback onglets
// Version de dev : 3.0.05-dev - Onglets web compatibles tous navigateurs
// Version de dev : 3.0.04-dev - Correction navigation onglets principale
// Version de dev : 3.0.03-dev - Rotation OLED ajustable et messages inline
// Version de dev : 3.0.02-dev - Correction API scan WiFi pour core 3.3.2

#include <WiFi.h>
#include <WiFiMulti.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <esp_chip_info.h>
#include <esp_mac.h>
#include <esp_flash.h>
#include <esp_heap_caps.h>
#include <esp_partition.h>
#include <esp_wifi.h>
#include <soc/soc.h>
#include <soc/rtc.h>
#include <Wire.h>
#include <Adafruit_NeoPixel.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#if (defined(CONFIG_BT_ENABLED) && defined(CONFIG_BT_BLE_ENABLED) && CONFIG_BT_ENABLED && CONFIG_BT_BLE_ENABLED)
  // --- [NEW FEATURE] Support Bluetooth Low Energy ---
  #include <BLEDevice.h>
  #include <BLEServer.h>
  #include <BLEUtils.h>
  #include <BLE2902.h>
  #define BLE_STACK_SUPPORTED 1
#else
  #define BLE_STACK_SUPPORTED 0
#endif
#include <vector>

// --- [NEW FEATURE] Inclusion automatique de la configuration WiFi ---
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
#define DIAGNOSTIC_VERSION "3.0.16-dev"
#define CUSTOM_LED_PIN -1
#define CUSTOM_LED_COUNT 1
#define ENABLE_I2C_SCAN true
#define MDNS_HOSTNAME "esp32-diagnostic"

// --- [NEW FEATURE] Références texte partagées pour l'interface web ---
const char* DIAGNOSTIC_VERSION_STR = DIAGNOSTIC_VERSION;
const char* MDNS_HOSTNAME_STR = MDNS_HOSTNAME;

// --- [NEW FEATURE] Aide pour afficher la version du core Arduino ---
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

// --- [NEW FEATURE] Orientation OLED paramétrable ---
uint8_t oledRotation = 0;

// ========== OBJETS GLOBAUX ==========
WebServer server(80);
WiFiMulti wifiMulti;
#if BLE_STACK_SUPPORTED
BLEServer* bluetoothServer = nullptr;
BLEService* bluetoothService = nullptr;
BLECharacteristic* bluetoothCharacteristic = nullptr;
#endif
// --- [NEW FEATURE] Gestion Bluetooth Low Energy ---
bool bluetoothCapable = false;
bool bluetoothEnabled = false;
bool bluetoothAdvertising = false;
String bluetoothDeviceName = "";
String defaultBluetoothName = "";
Adafruit_SSD1306 oled(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// NeoPixel
int LED_PIN = CUSTOM_LED_PIN;
int LED_COUNT = CUSTOM_LED_COUNT;
Adafruit_NeoPixel *strip = nullptr;

bool neopixelTested = false;
bool neopixelAvailable = false;
bool neopixelSupported = false;
String neopixelTestResult = "En attente d'initialisation";

// LED intégrée
int BUILTIN_LED_PIN = -1;
bool builtinLedTested = false;
bool builtinLedAvailable = false;
String builtinLedTestResult = "En attente d'initialisation";

// OLED
bool oledTested = false;
bool oledAvailable = false;
String oledTestResult = "En attente d'initialisation";

// Tests additionnels
String adcTestResult = "Non teste";
String touchTestResult = "Non teste";
String pwmTestResult = "Non teste";
String partitionsInfo = "";
String spiInfo = "";
String stressTestResult = "Non teste";

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

struct TouchReading {
  int pin;
  int value;
};

std::vector<TouchReading> touchReadings;

#define HISTORY_SIZE 60
float heapHistory[HISTORY_SIZE];
float tempHistory[HISTORY_SIZE];
int historyIndex = 0;

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
    features = T().none;
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
    return T().unknown;
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
    return T().unknown;
  #endif
}

String getResetReason() {
  esp_reset_reason_t reason = esp_reset_reason();
  switch (reason) {
    case ESP_RST_POWERON: return T().poweron;
    case ESP_RST_SW: return T().software_reset;
    case ESP_RST_DEEPSLEEP: return T().deepsleep_exit;
    case ESP_RST_BROWNOUT: return T().brownout;
    default: return T().other;
  }
}

String getMemoryStatus() {
  float heapUsagePercent = ((float)(diagnosticData.heapSize - diagnosticData.freeHeap) / diagnosticData.heapSize) * 100;
  if (heapUsagePercent < 50) return T().excellent;
  else if (heapUsagePercent < 70) return T().good;
  else if (heapUsagePercent < 85) return T().warning;
  else return T().critical;
}

String getWiFiSignalQuality() {
  if (diagnosticData.wifiRSSI >= -50) return T().excellent;
  else if (diagnosticData.wifiRSSI >= -60) return T().very_good;
  else if (diagnosticData.wifiRSSI >= -70) return T().good;
  else if (diagnosticData.wifiRSSI >= -80) return T().weak;
  else return T().very_weak;
}

// --- [NEW FEATURE] Utilitaires WiFi avancés compatibles Arduino 3.3.2 ---
String wifiAuthModeToString(wifi_auth_mode_t mode) {
  switch (mode) {
    case WIFI_AUTH_OPEN: return "Ouvert";
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
    default: return "Inconnu";
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
    detailedMemory.memoryStatus = T().excellent;
  } else if (detailedMemory.fragmentationPercent < 40) {
    detailedMemory.memoryStatus = T().good;
  } else if (detailedMemory.fragmentationPercent < 60) {
    detailedMemory.memoryStatus = "Moyen"; // Pas traduit (statut technique)
  } else {
    detailedMemory.memoryStatus = T().critical;
  }
}

void printPSRAMDiagnostic() {
  Serial.println("\r\n=== DIAGNOSTIC PSRAM DETAILLE ===");
  Serial.printf("ESP.getPsramSize(): %u octets (%.2f MB)\r\n", 
                ESP.getPsramSize(), ESP.getPsramSize() / 1048576.0);
  
  Serial.println("\r\nFlags de compilation detectes (indication du type supporte par la carte):");
  bool anyFlag = false;
  String psramType = detailedMemory.psramType ? detailedMemory.psramType : "Inconnu";
  
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
// --- [NEW FEATURE] Réinitialisation I2C compatible core 3.3.2 ---
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
    // --- [NEW FEATURE] Analyse avancée du scan WiFi ---
    // --- [NEW FEATURE] Compatibilite API WiFiScan core 3.3.2 ---
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
    result.notes = result.working ? "OK" : "Erreur";
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
  
  builtinLedTestResult = "Pret - GPIO " + String(BUILTIN_LED_PIN);
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
  builtinLedTestResult = "Test OK - GPIO " + String(BUILTIN_LED_PIN);
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
  neopixelTestResult = "Pret - GPIO " + String(LED_PIN);
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
  neopixelTestResult = "Test OK - GPIO " + String(LED_PIN);
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

// --- [NEW FEATURE] Application centralisée de la rotation OLED ---
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
    oledTestResult = "Detecte a 0x" + String(SCREEN_ADDRESS, HEX);
    Serial.println("OLED: Detecte!\r\n");
  } else {
    oledAvailable = false;
    if (i2cDetected) {
      oledTestResult = "Peripherique I2C present mais init echoue";
    } else {
      oledTestResult = "Non detecte (SDA:" + String(I2C_SDA) + " SCL:" + String(I2C_SCL) + ")";
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
  oled.println("Chargement");
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
  oledTestResult = "Test OK - 128x64";
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
  
  adcTestResult = String(numADC) + " ADC testes - OK";
  Serial.printf("ADC: %d canaux testes\r\n", numADC);
}

// ========== TEST TOUCH PADS ==========
void testTouchPads() {
  Serial.println("\r\n=== TEST TOUCH PADS ===");
  touchReadings.clear();
  
  #ifdef CONFIG_IDF_TARGET_ESP32
    int touchPins[] = {4, 0, 2, 15, 13, 12, 14, 27, 33, 32};
    int numTouch = 10;
  #elif defined(CONFIG_IDF_TARGET_ESP32S3)
    int touchPins[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14};
    int numTouch = 14;
  #else
    int numTouch = 0;
  #endif
  
  if (numTouch == 0) {
    touchTestResult = "Non supporte sur ce modele";
    Serial.println("Touch: Non supporte");
    return;
  }
  
  for(int i = 0; i < numTouch; i++) {
    TouchReading reading;
    reading.pin = touchPins[i];
    reading.value = touchRead(touchPins[i]);
    touchReadings.push_back(reading);
    
    Serial.printf("Touch%d (GPIO%d): %d\r\n", i, reading.pin, reading.value);
  }
  
  touchTestResult = String(numTouch) + " Touch Pads detectes";
  Serial.printf("Touch: %d pads testes\r\n", numTouch);
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
  
  pwmTestResult = "Test OK sur GPIO" + String(testPin);
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
  
  stressTestResult = "Max: " + String(maxAllocs) + " KB alloues";
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
  diagnosticData.hasBLE = (chip_info.features & CHIP_FEATURE_BLE);
  bluetoothCapable = diagnosticData.hasBLE && BLE_STACK_SUPPORTED;
  
  if (WiFi.status() == WL_CONNECTED) {
    diagnosticData.wifiSSID = WiFi.SSID();
    diagnosticData.wifiRSSI = WiFi.RSSI();
    diagnosticData.ipAddress = WiFi.localIP().toString();
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
  String json = "{\"results\":[";
  for (size_t i = 0; i < gpioResults.size(); i++) {
    if (i > 0) json += ",";
    json += "{\"pin\":" + String(gpioResults[i].pin) + ",\"working\":" + String(gpioResults[i].working ? "true" : "false") + "}";
  }
  json += "]}";
  server.send(200, "application/json", json);
}

void handleWiFiScan() {
  scanWiFiNetworks();
  String json = "{\"networks\":[";
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
  server.send(200, "application/json", "{\"count\":" + String(diagnosticData.i2cCount) + ",\"devices\":\"" + diagnosticData.i2cDevices + "\"}");
}

void handleBuiltinLEDConfig() {
  if (server.hasArg("gpio")) {
    int newGPIO = server.arg("gpio").toInt();
    if (newGPIO >= 0 && newGPIO <= 48) {
      BUILTIN_LED_PIN = newGPIO;
      resetBuiltinLEDTest();
      server.send(200, "application/json", "{\"success\":true,\"message\":\"LED GPIO " + String(BUILTIN_LED_PIN) + "\"}");
      return;
    }
  }
  server.send(400, "application/json", "{\"success\":false}");
}

void handleBuiltinLEDTest() {
  resetBuiltinLEDTest();
  testBuiltinLED();
  server.send(200, "application/json", "{\"success\":" + String(builtinLedAvailable ? "true" : "false") + ",\"result\":\"" + builtinLedTestResult + "\"}");
}

void handleBuiltinLEDControl() {
  if (!server.hasArg("action")) {
    server.send(400, "application/json", "{\"success\":false}");
    return;
  }
  
  String action = server.arg("action");
  if (BUILTIN_LED_PIN == -1) {
    server.send(400, "application/json", "{\"success\":false,\"message\":\"LED non configuree\"}");
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
    message = "Clignotement OK";
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
    message = "Fade OK";
  } else if (action == "off") {
    digitalWrite(BUILTIN_LED_PIN, LOW);
    builtinLedTested = false;
    message = "LED eteinte";
  } else {
    server.send(400, "application/json", "{\"success\":false}");
    return;
  }
  
  server.send(200, "application/json", "{\"success\":true,\"message\":\"" + message + "\"}");
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
      server.send(200, "application/json", "{\"success\":true,\"message\":\"Config GPIO " + String(LED_PIN) + "\"}");
      return;
    }
  }
  server.send(400, "application/json", "{\"success\":false}");
}

void handleNeoPixelTest() {
  resetNeoPixelTest();
  testNeoPixel();
  server.send(200, "application/json", "{\"success\":" + String(neopixelAvailable ? "true" : "false") + ",\"result\":\"" + neopixelTestResult + "\"}");
}

void handleNeoPixelPattern() {
  if (!server.hasArg("pattern")) {
    server.send(400, "application/json", "{\"success\":false}");
    return;
  }
  
  String pattern = server.arg("pattern");
  if (!strip) {
    server.send(400, "application/json", "{\"success\":false,\"message\":\"NeoPixel non init\"}");
    return;
  }
  
  String message = "";
  if (pattern == "rainbow") {
    neopixelRainbow();
    message = "Arc-en-ciel OK";
  } else if (pattern == "blink") {
    neopixelBlink(strip->Color(255, 0, 0), 5);
    message = "Blink OK";
  } else if (pattern == "fade") {
    neopixelFade(strip->Color(0, 0, 255));
    message = "Fade OK";
  } else if (pattern == "off") {
    strip->clear();
    strip->show();
    neopixelTested = false;
    message = "Off";
  } else {
    server.send(400, "application/json", "{\"success\":false}");
    return;
  }
  
  server.send(200, "application/json", "{\"success\":true,\"message\":\"" + message + "\"}");
}

void handleNeoPixelColor() {
  if (!server.hasArg("r") || !server.hasArg("g") || !server.hasArg("b") || !strip) {
    server.send(400, "application/json", "{\"success\":false}");
    return;
  }
  
  int r = server.arg("r").toInt();
  int g = server.arg("g").toInt();
  int b = server.arg("b").toInt();
  
  strip->fill(strip->Color(r, g, b));
  strip->show();
  neopixelTested = false;
  
  server.send(200, "application/json", "{\"success\":true,\"message\":\"RGB(" + String(r) + "," + String(g) + "," + String(b) + ")\"}");
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
      server.send(200, "application/json", "{\"success\":true,\"message\":\"" + message + "\",\"sda\":" + String(I2C_SDA) + ",\"scl\":" + String(I2C_SCL) + ",\"rotation\":" + String(oledRotation) + "}");
      return;
    }
  }
  server.send(400, "application/json", "{\"success\":false,\"message\":\"Configuration invalide\"}");
}

void handleOLEDTest() {
  resetOLEDTest();
  testOLED();
  server.send(200, "application/json", "{\"success\":" + String(oledAvailable ? "true" : "false") + ",\"result\":\"" + oledTestResult + "\"}");
}

void handleOLEDStep() {
  if (!server.hasArg("step")) {
    server.send(400, "application/json", "{\"success\":false,\"message\":\"" + String(T().oled_step_unknown) + "\"}");
    return;
  }

  String stepId = server.arg("step");

  if (!oledAvailable) {
    server.send(200, "application/json", "{\"success\":false,\"message\":\"" + String(T().oled_step_unavailable) + "\"}");
    return;
  }

  bool ok = performOLEDStep(stepId);
  if (!ok) {
    server.send(400, "application/json", "{\"success\":false,\"message\":\"" + String(T().oled_step_unknown) + "\"}");
    return;
  }

  String label = getOLEDStepLabel(stepId);
  server.send(200, "application/json", "{\"success\":true,\"message\":\"" + String(T().oled_step_executed_prefix) + " " + label + "\"}");
}

void handleOLEDMessage() {
  if (!server.hasArg("message")) {
    server.send(400, "application/json", "{\"success\":false}");
    return;
  }
  
  String message = server.arg("message");
  oledShowMessage(message);
  server.send(200, "application/json", "{\"success\":true,\"message\":\"Message affiche\"}");
}

void handleADCTest() {
  testADC();
  String json = "{\"readings\":[";
  for (size_t i = 0; i < adcReadings.size(); i++) {
    if (i > 0) json += ",";
    json += "{\"pin\":" + String(adcReadings[i].pin) + ",\"raw\":" + String(adcReadings[i].rawValue) + 
            ",\"voltage\":" + String(adcReadings[i].voltage, 2) + "}";
  }
  json += "],\"result\":\"" + adcTestResult + "\"}";
  server.send(200, "application/json", json);
}

void handleTouchTest() {
  testTouchPads();
  String json = "{\"readings\":[";
  for (size_t i = 0; i < touchReadings.size(); i++) {
    if (i > 0) json += ",";
    json += "{\"pin\":" + String(touchReadings[i].pin) + ",\"value\":" + String(touchReadings[i].value) + "}";
  }
  json += "],\"result\":\"" + touchTestResult + "\"}";
  server.send(200, "application/json", json);
}

void handlePWMTest() {
  testPWM();
  server.send(200, "application/json", "{\"result\":\"" + pwmTestResult + "\"}");
}

void handleSPIScan() {
  scanSPI();
  server.send(200, "application/json", "{\"info\":\"" + spiInfo + "\"}");
}

void handlePartitionsList() {
  listPartitions();
  server.send(200, "application/json", "{\"partitions\":\"" + partitionsInfo + "\"}");
}

void handleStressTest() {
  memoryStressTest();
  server.send(200, "application/json", "{\"result\":\"" + stressTestResult + "\"}");
}

void handleBenchmark() {
  unsigned long cpuTime = benchmarkCPU();
  unsigned long memTime = benchmarkMemory();
  
  diagnosticData.cpuBenchmark = cpuTime;
  diagnosticData.memBenchmark = memTime;
  
  server.send(200, "application/json", "{\"cpu\":" + String(cpuTime) + ",\"memory\":" + String(memTime) + 
              ",\"cpuPerf\":" + String(100000.0 / cpuTime, 2) + ",\"memSpeed\":" + String((10000 * sizeof(int) * 2) / (float)memTime, 2) + "}");
}

void handleMemoryDetails() {
  collectDetailedMemory();
  
  String json = "{\"flash\":{\"real\":" + String(detailedMemory.flashSizeReal) + ",\"chip\":" + String(detailedMemory.flashSizeChip) + "},";
  json += "\"psram\":{\"available\":" + String(detailedMemory.psramAvailable ? "true" : "false") +
          ",\"configured\":" + String(detailedMemory.psramConfigured ? "true" : "false") +
          ",\"supported\":" + String(detailedMemory.psramBoardSupported ? "true" : "false") +
          ",\"type\":\"" + String(detailedMemory.psramType ? detailedMemory.psramType : "Inconnu") + "\"" +
          ",\"total\":" + String(detailedMemory.psramTotal) + ",\"free\":" + String(detailedMemory.psramFree) + "},";
  json += "\"sram\":{\"total\":" + String(detailedMemory.sramTotal) + ",\"free\":" + String(detailedMemory.sramFree) + "},";
  json += "\"fragmentation\":" + String(detailedMemory.fragmentationPercent, 1) + ",\"status\":\"" + detailedMemory.memoryStatus + "\"}";
  
  server.send(200, "application/json", json);
}

// ========== EXPORTS ==========
void handleExportTXT() {
  collectDiagnosticInfo();
  collectDetailedMemory();
  
  String txt = "========================================\r\n";
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
  txt += "NeoPixel: " + neopixelTestResult + "\r\n";
  txt += "OLED: " + oledTestResult + "\r\n";
  txt += "ADC: " + adcTestResult + "\r\n";
  txt += "Touch: " + touchTestResult + "\r\n";
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
  
  String json = "{";
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
  json += "\"psram_type\":\"" + String(detailedMemory.psramType ? detailedMemory.psramType : "Inconnu") + "\",";
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
  json += "\"subnet\":\"" + WiFi.subnetMask().toString() + "\",";
  json += "\"gateway\":\"" + WiFi.gatewayIP().toString() + "\",";
  json += "\"dns\":\"" + WiFi.dnsIP().toString() + "\"";
  json += "},";

  json += "\"bluetooth\":{";
  json += "\"supported\":" + String(bluetoothCapable ? "true" : "false") + ",";
  json += "\"enabled\":" + String((bluetoothCapable && bluetoothEnabled) ? "true" : "false") + ",";
  json += "\"advertising\":" + String((bluetoothCapable && bluetoothAdvertising) ? "true" : "false") + ",";
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
  json += "\"touch\":\"" + touchTestResult + "\",";
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
  
  String csv = String(T().category) + "," + String(T().parameter) + "," + String(T().value) + "\r\n";
  
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
  csv += String(T().test) + ",NeoPixel," + neopixelTestResult + "\r\n";
  csv += String(T().test) + ",OLED," + oledTestResult + "\r\n";
  csv += String(T().test) + ",ADC," + adcTestResult + "\r\n";
  csv += String(T().test) + ",Touch," + touchTestResult + "\r\n";
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
  html += "<h2>Informations Processeur</h2>";
  html += "<div class='grid'>";
  html += "<div class='row'><b>Modèle:</b><span>" + diagnosticData.chipModel + " Rev" + diagnosticData.chipRevision + "</span></div>";
  html += "<div class='row'><b>CPU:</b><span>" + String(diagnosticData.cpuCores) + " coeurs @ " + String(diagnosticData.cpuFreqMHz) + " MHz</span></div>";
  html += "<div class='row'><b>MAC WiFi:</b><span>" + diagnosticData.macAddress + "</span></div>";
  html += "<div class='row'><b>SDK:</b><span>" + diagnosticData.sdkVersion + "</span></div>";
  html += "<div class='row'><b>ESP-IDF:</b><span>" + diagnosticData.idfVersion + "</span></div>";
  if (diagnosticData.temperature != -999) {
    html += "<div class='row'><b>Température:</b><span>" + String(diagnosticData.temperature, 1) + " °C</span></div>";
  }
  
  unsigned long seconds = diagnosticData.uptime / 1000;
  unsigned long minutes = seconds / 60;
  unsigned long hours = minutes / 60;
  unsigned long days = hours / 24;
  html += "<div class='row'><b>Uptime:</b><span>" + String(days) + "j " + String(hours % 24) + "h " + String(minutes % 60) + "m</span></div>";
  html += "<div class='row'><b>Dernier Reset:</b><span>" + getResetReason() + "</span></div>";
  html += "</div></div>";
  
  // Mémoire
  html += "<div class='section'>";
  html += "<h2>Mémoire</h2>";
  html += "<table>";
  html += "<tr><th>Type</th><th>Taille</th><th>Libre</th><th>Utilisée</th><th>Statut</th></tr>";
  
  // Flash
  bool flashMatch = (detailedMemory.flashSizeReal == detailedMemory.flashSizeChip);
  html += "<tr><td><b>Flash</b></td>";
  html += "<td>" + String(detailedMemory.flashSizeReal / 1048576.0, 1) + " MB</td>";
  html += "<td>-</td><td>-</td>";
  html += "<td><span class='badge " + String(flashMatch ? "badge-success'>OK" : "badge-warning'>Config IDE") + "</span></td></tr>";
  
  // PSRAM
  html += "<tr><td><b>PSRAM</b></td>";
  html += "<td>" + String(detailedMemory.psramTotal / 1048576.0, 1) + " MB</td>";
  if (detailedMemory.psramAvailable) {
    html += "<td>" + String(detailedMemory.psramFree / 1048576.0, 1) + " MB</td>";
    html += "<td>" + String(detailedMemory.psramUsed / 1048576.0, 1) + " MB</td>";
    html += "<td><span class='badge badge-success'>Détectée</span></td>";
  } else if (detailedMemory.psramBoardSupported) {
    html += "<td>-</td><td>-</td>";
    String psramHint = String(T().enable_psram_hint);
    psramHint.replace("%TYPE%", detailedMemory.psramType ? detailedMemory.psramType : "PSRAM");
    html += "<td><span class='badge badge-warning'>" + String(T().supported_not_enabled) + "</span><br><small>" + psramHint + "</small></td>";
  } else {
    html += "<td>-</td><td>-</td>";
    html += "<td><span class='badge badge-danger'>Non détectée</span></td>";
  }
  html += "</tr>";
  
  // SRAM
  html += "<tr><td><b>SRAM</b></td>";
  html += "<td>" + String(detailedMemory.sramTotal / 1024.0, 1) + " KB</td>";
  html += "<td>" + String(detailedMemory.sramFree / 1024.0, 1) + " KB</td>";
  html += "<td>" + String(detailedMemory.sramUsed / 1024.0, 1) + " KB</td>";
  html += "<td><span class='badge badge-success'>OK</span></td></tr>";
  html += "</table>";
  html += "<div class='row'><b>Fragmentation:</b><span>" + String(detailedMemory.fragmentationPercent, 1) + "% - " + detailedMemory.memoryStatus + "</span></div>";
  html += "</div>";
  
  // WiFi
  html += "<div class='section'>";
  html += "<h2>Connexion WiFi</h2>";
  html += "<div class='grid'>";
  html += "<div class='row'><b>SSID:</b><span>" + diagnosticData.wifiSSID + "</span></div>";
  html += "<div class='row'><b>Signal:</b><span>" + String(diagnosticData.wifiRSSI) + " dBm (" + getWiFiSignalQuality() + ")</span></div>";
  html += "<div class='row'><b>Adresse IP:</b><span>" + diagnosticData.ipAddress + "</span></div>";
  html += "<div class='row'><b>Masque:</b><span>" + WiFi.subnetMask().toString() + "</span></div>";
  html += "<div class='row'><b>Passerelle:</b><span>" + WiFi.gatewayIP().toString() + "</span></div>";
  html += "<div class='row'><b>DNS:</b><span>" + WiFi.dnsIP().toString() + "</span></div>";
  html += "</div></div>";

  html += "<div class='section'>";
  html += "<h2>Bluetooth</h2>";
  html += "<div class='grid'>";
  html += "<div class='row'><b>" + String(T().bluetooth_status) + ":</b><span>" + htmlEscape(getBluetoothStateLabel()) + "</span></div>";
  html += "<div class='row'><b>" + String(T().bluetooth_name) + ":</b><span>" + htmlEscape(bluetoothDeviceName) + "</span></div>";
  html += "<div class='row'><b>" + String(T().bluetooth_mac) + ":</b><span>" + htmlEscape(diagnosticData.bluetoothAddress) + "</span></div>";
  String printAdvertising = bluetoothCapable && bluetoothAdvertising ? String(T().bluetooth_advertising) : String(T().bluetooth_not_advertising);
  html += "<div class='row'><b>" + String(T().bluetooth_advertising) + ":</b><span>" + htmlEscape(printAdvertising) + "</span></div>";
  html += "</div></div>";

  // GPIO et Périphériques
  html += "<div class='section'>";
  html += "<h2>GPIO et Périphériques</h2>";
  html += "<div class='grid'>";
  html += "<div class='row'><b>GPIO Total:</b><span>" + String(diagnosticData.totalGPIO) + " broches</span></div>";
  html += "<div class='row'><b>I2C:</b><span id='i2c-summary'>" + String(diagnosticData.i2cCount) + " périphérique(s) - " + diagnosticData.i2cDevices + "</span></div>";
  html += "<div class='row'><b>SPI:</b><span>" + spiInfo + "</span></div>";
  html += "</div></div>";
  
  // Tests Matériels
  html += "<div class='section'>";
  html += "<h2>Tests Matériels</h2>";
  html += "<table>";
  html += "<tr><th>Périphérique</th><th>Résultat</th></tr>";
  html += "<tr><td>LED intégrée</td><td>" + builtinLedTestResult + "</td></tr>";
  html += "<tr><td>NeoPixel</td><td>" + neopixelTestResult + "</td></tr>";
  html += "<tr><td>Écran OLED</td><td>" + oledTestResult + "</td></tr>";
  html += "<tr><td>ADC</td><td>" + adcTestResult + "</td></tr>";
  html += "<tr><td>Touch Pads</td><td>" + touchTestResult + "</td></tr>";
  html += "<tr><td>PWM</td><td>" + pwmTestResult + "</td></tr>";
  html += "</table></div>";
  
  // Performance
  if (diagnosticData.cpuBenchmark > 0) {
    html += "<div class='section'>";
    html += "<h2>Performance</h2>";
    html += "<div class='grid'>";
    html += "<div class='row'><b>CPU:</b><span>" + String(diagnosticData.cpuBenchmark) + " µs (" + String(100000.0 / diagnosticData.cpuBenchmark, 2) + " MFLOPS)</span></div>";
    html += "<div class='row'><b>Mémoire:</b><span>" + String(diagnosticData.memBenchmark) + " µs</span></div>";
    html += "<div class='row'><b>Stress Test:</b><span>" + stressTestResult + "</span></div>";
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
    String lang = server.arg("lang");
    if (lang == "fr") {
      setLanguage(LANG_FR);
    } else if (lang == "en") {
      setLanguage(LANG_EN);
    }
    server.send(200, "application/json", "{\"success\":true,\"lang\":\"" + lang + "\"}");
  } else {
    server.send(400, "application/json", "{\"success\":false}");
  }
}

// --- [NEW FEATURE] Échappement HTML sécurisé ---
String htmlEscape(const String& raw) {
  String escaped;
  escaped.reserve(raw.length());
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

// --- [BUGFIX] Échappement JSON pour l'API de traduction ---
String jsonEscape(const char* raw) {
  if (raw == nullptr) {
    return "";
  }

  String escaped;
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

// --- [BUGFIX] Construction sécurisée des champs JSON de traduction ---
String jsonField(const char* key, const char* value, bool last = false) {
  String field = "\"";
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
  bool hasRadio = diagnosticData.hasBLE || diagnosticData.hasBT;
  diagnosticData.bluetoothName = bluetoothDeviceName;
  diagnosticData.bluetoothEnabled = supported && bluetoothEnabled;
  diagnosticData.bluetoothAdvertising = supported && bluetoothAdvertising;

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
  summary += bluetoothAdvertising ? String(T().bluetooth_advertising) : String(T().bluetooth_not_advertising);
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

bool startBluetooth() {
  ensureBluetoothName();

  if (!bluetoothCapable || !BLE_STACK_SUPPORTED) {
    bluetoothEnabled = false;
    bluetoothAdvertising = false;
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
    return false;
  }

  bluetoothService = bluetoothServer->createService(BLEUUID((uint16_t)0x180A));
  if (!bluetoothService) {
    BLEDevice::deinit(true);
    bluetoothServer = nullptr;
    bluetoothEnabled = false;
    bluetoothAdvertising = false;
    return false;
  }

  bluetoothCharacteristic = bluetoothService->createCharacteristic(
      BLEUUID((uint16_t)0x2A29),
      BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY);
  if (bluetoothCharacteristic) {
    bluetoothCharacteristic->setValue("ESP32 Diagnostic");
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
    return false;
  }

  advertising->addServiceUUID(bluetoothService->getUUID());
  advertising->setScanResponse(true);
  advertising->start();

  bluetoothEnabled = true;
  bluetoothAdvertising = true;
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

void handleGetTranslations() {
  // Envoie toutes les traductions en JSON pour mise à jour dynamique
  String json = "{";
  json.reserve(1024);
  json += jsonField("title", T().title);
  json += jsonField("nav_overview", T().nav_overview);
  json += jsonField("nav_leds", T().nav_leds);
  json += jsonField("nav_screens", T().nav_screens);
  json += jsonField("nav_tests", T().nav_tests);
  json += jsonField("nav_gpio", T().nav_gpio);
  json += jsonField("nav_wireless", T().nav_wireless);
  json += jsonField("nav_benchmark", T().nav_benchmark);
  json += jsonField("nav_export", T().nav_export);
  json += jsonField("chip_info", T().chip_info);
  json += jsonField("memory_details", T().memory_details);
  json += jsonField("wifi_connection", T().wifi_connection);
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
  json += jsonField("gpio_interfaces", T().gpio_interfaces);
  json += jsonField("i2c_peripherals", T().i2c_peripherals);
  json += jsonField("builtin_led", T().builtin_led);
  json += jsonField("oled_screen", T().oled_screen);
  json += jsonField("adc_test", T().adc_test);
  json += jsonField("touch_test", T().touch_test);
  json += jsonField("pwm_test", T().pwm_test);
  json += jsonField("spi_bus", T().spi_bus);
  json += jsonField("flash_partitions", T().flash_partitions);
  json += jsonField("memory_stress", T().memory_stress);
  json += jsonField("gpio_test", T().gpio_test);
  json += jsonField("test_all_gpio", T().test_all_gpio);
  json += jsonField("click_to_test", T().click_to_test);
  json += jsonField("gpio_warning", T().gpio_warning);
  json += jsonField("wifi_scanner", T().wifi_scanner);
  json += jsonField("performance_bench", T().performance_bench);
  json += jsonField("data_export", T().data_export, true);
  json += "}";

  server.send(200, "application/json", json);
}

// --- [NEW FEATURE] API de contrôle Bluetooth ---
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

  bool supported = bluetoothCapable && BLE_STACK_SUPPORTED;
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
    server.send(200, "application/json", buildBluetoothJSON(false, String(T().bluetooth_error)));
    return;
  }

  bool supported = bluetoothCapable && BLE_STACK_SUPPORTED;
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
  bool supported = bluetoothCapable && BLE_STACK_SUPPORTED;
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
.app-header{
  background:var(--bg-surface);
  border-radius:var(--radius);
  padding:22px 26px;
  box-shadow:0 25px 60px rgba(15,23,42,.45);
  border:1px solid var(--border-glow);
}
.header-meta{
  display:flex;
  flex-wrap:wrap;
  justify-content:space-between;
  gap:24px;
  align-items:center;
}
.branding{
  display:flex;
  flex-direction:column;
  gap:8px;
}
.branding .subtitle{
  text-transform:uppercase;
  letter-spacing:.3em;
  font-size:.75rem;
  color:var(--accent);
}
.branding h1{
  font-size:2.4rem;
  margin:0;
}
.header-actions{
  display:flex;
  align-items:center;
  gap:16px;
  flex-wrap:wrap;
}
.lang-switcher{
  display:flex;
  background:rgba(255,255,255,0.06);
  border-radius:999px;
  padding:4px;
  border:1px solid var(--border-muted);
}
.lang-btn{
  border:none;
  background:transparent;
  color:var(--text-primary);
  font-weight:600;
  padding:8px 18px;
  border-radius:999px;
  cursor:pointer;
  transition:var(--transition);
}
.lang-btn.active{
  background:var(--accent);
  color:#0f172a;
  box-shadow:0 10px 25px rgba(56,189,248,.35);
}
.status-chip{
  display:flex;
  align-items:center;
  gap:10px;
  padding:8px 16px;
  border-radius:999px;
  background:rgba(56,189,248,0.12);
  border:1px solid rgba(56,189,248,0.35);
  font-weight:600;
}
.status-indicator{
  width:12px;
  height:12px;
  border-radius:50%;
  display:inline-block;
  background:var(--success);
  box-shadow:0 0 18px rgba(34,197,94,.65);
  transition:var(--transition);
}
.status-indicator.status-offline{
  background:var(--danger);
  box-shadow:0 0 18px rgba(248,113,113,.6);
}
.header-info{
  margin-top:18px;
  display:grid;
  grid-template-columns:repeat(auto-fit,minmax(240px,1fr));
  gap:18px;
}
.header-card{
  background:var(--bg-surface-alt);
  border-radius:var(--radius);
  padding:18px;
  border:1px solid var(--border-muted);
}
.header-card strong{
  display:block;
  font-size:.85rem;
  text-transform:uppercase;
  letter-spacing:.1em;
  color:var(--text-muted);
  margin-bottom:6px;
}
.header-card span{
  font-size:1.05rem;
  font-weight:600;
}
.app-body{
  display:flex;
  flex-direction:column;
  gap:24px;
  flex:1;
}
.nav-wrapper{
  position:sticky;
  top:0;
  z-index:90;
  transition:var(--transition);
  display:flex;
  flex-direction:column;
  gap:10px;
  padding-top:6px;
  padding-bottom:6px;
}
.nav-wrapper.is-sticky{
  background:rgba(11,17,32,0.92);
  backdrop-filter:blur(18px);
  padding-top:12px;
}
.primary-nav{
  display:flex;
  flex-wrap:wrap;
  gap:12px;
  background:var(--bg-surface);
  border-radius:var(--radius);
  padding:10px 16px;
  border:1px solid var(--border-glow);
  box-shadow:0 25px 60px rgba(15,23,42,.35);
  position:relative;
}
.nav-wrapper.is-sticky .primary-nav{
  box-shadow:0 22px 40px rgba(15,23,42,.55);
  border-color:rgba(148,163,184,0.28);
}
.nav-link{
  border:none;
  text-align:center;
  padding:10px 16px;
  border-radius:14px;
  color:var(--text-primary);
  background:rgba(148,163,184,0.08);
  font-weight:600;
  cursor:pointer;
  transition:var(--transition);
  display:flex;
  justify-content:center;
  align-items:center;
  gap:10px;
  text-decoration:none;
  flex:1 1 140px;
}
.nav-link .icon{font-size:1.1rem;}
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
  padding:26px;
  border:1px solid var(--border-muted);
  box-shadow:0 25px 60px rgba(15,23,42,.45);
}
.inline-message{
  min-height:32px;
  margin:0;
  padding:10px 16px;
  border-radius:12px;
  border:1px solid transparent;
  background:rgba(148,163,184,0.08);
  color:var(--text-muted);
  font-size:.95rem;
  display:none;
}
.nav-wrapper .inline-message{
  border:1px solid rgba(148,163,184,0.18);
  background:rgba(148,163,184,0.14);
}
.inline-message.show{display:block;}
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
  margin-bottom:18px;
  color:var(--accent);
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
  .primary-nav{flex-wrap:nowrap;overflow-x:auto;}
  .nav-link{flex:0 0 auto;min-width:180px;}
}
@media(max-width:640px){
  .app-header{padding:20px;}
  .branding h1{font-size:1.8rem;}
  .header-info{grid-template-columns:1fr;}
  .nav-link{padding:12px 14px;}
  .app-main{padding:20px;}
}
)rawliteral");
  chunk += "</style></head><body>";
  server.sendContent(chunk);
  

// --- [NEW FEATURE] En-tête unifié et navigation responsive ---
  const char* updateLabel = (currentLanguage == LANG_FR) ? "Mise à jour..." : "Updating...";
  const char* connectionLabel = (currentLanguage == LANG_FR) ? "En ligne" : "Online";
  String wifiSummary = String(T().not_detected);
  if (diagnosticData.wifiSSID.length() > 0 && WiFi.status() == WL_CONNECTED) {
    wifiSummary = diagnosticData.wifiSSID + " (" + String(diagnosticData.wifiRSSI) + " dBm)";
  }
  String bluetoothSummary = getBluetoothSummaryLabel();
  String bluetoothStatusText = getBluetoothStateLabel();
  String bluetoothNameDisplay = diagnosticData.bluetoothName.length() > 0 ? diagnosticData.bluetoothName : defaultBluetoothName;
  String bluetoothDisableAttr = bluetoothCapable ? "" : " disabled";
  String bluetoothFeedbackText = bluetoothCapable ? "" : String(T().bluetooth_not_supported);
  String accessSummary = "<a href='http://" + String(MDNS_HOSTNAME) + ".local' target='_blank' style='color:inherit;text-decoration:none'>http://" + String(MDNS_HOSTNAME) + ".local</a><br>" + diagnosticData.ipAddress;

  chunk = "<div class='app-shell'>";
  chunk += "<div id='updateIndicator' class='update-indicator'>";
  chunk += updateLabel;
  chunk += "</div>";
  chunk += "<header class='app-header'>";
  chunk += "<div class='header-meta'>";
  chunk += "<div class='branding'><span class='subtitle' data-i18n='title'>" + String(T().title) + "</span>";
  chunk += "<h1 id='main-title'>" + String(T().version) + String(DIAGNOSTIC_VERSION) + "</h1></div>";
  chunk += "<div class='header-actions'>";
  chunk += "<div class='lang-switcher' role='group' aria-label='Langue'>";
  chunk += "<button type='button' class='lang-btn " + String(currentLanguage == LANG_FR ? "active" : "") + "' data-lang='fr' onclick='changeLang(\"fr\",this)'>FR</button>";
  chunk += "<button type='button' class='lang-btn " + String(currentLanguage == LANG_EN ? "active" : "") + "' data-lang='en' onclick='changeLang(\"en\",this)'>EN</button>";
  chunk += "</div>";
  chunk += "<div class='status-chip'><span class='status-indicator status-online' id='statusIndicator'></span><span id='connectionLabel'>";
  chunk += connectionLabel;
  chunk += "</span></div>";
  chunk += "</div></div>";
  chunk += "<div class='header-info'>";
  chunk += "<div class='header-card'><strong data-i18n='chip_info'>" + String(T().chip_info) + "</strong><span>" + diagnosticData.chipModel + "</span></div>";
  chunk += "<div class='header-card'><strong data-i18n='wifi_connection'>" + String(T().wifi_connection) + "</strong><span>" + wifiSummary + "</span></div>";
  chunk += "<div class='header-card'><strong data-i18n='bluetooth_section'>" + String(T().bluetooth_section) + "</strong><span id='bluetoothSummary'>" + htmlEscape(bluetoothSummary) + "</span></div>";
  chunk += "<div class='header-card'><strong data-i18n='access'>" + String(T().access) + "</strong><span>" + accessSummary + "</span></div>";
  chunk += "<div class='header-card'><strong>Arduino Core</strong><span>" + getArduinoCoreVersionString() + "</span></div>";
  chunk += "</div>";
  chunk += "</header>";
  chunk += "<div class='app-body'>";
  chunk += "<div class='nav-wrapper'>";
  // --- [NEW FEATURE] Message inline collé sous la navigation ---
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
  chunk += "<div id='inlineMessage' class='inline-message' role='status' aria-live='polite'></div>";
  chunk += "</div>";
  chunk += "<main class='app-main'>";
  chunk += "<div id='tabContainer' class='tab-container'>";
  server.sendContent(chunk);
  // CHUNK 3: OVERVIEW TAB - VERSION UNIQUE COMPLÈTE
  chunk = "<div id='overview' class='tab-content active'>";
  
  // Chip Info
  chunk += "<div class='section'><h2>" + String(T().chip_info) + "</h2><div class='info-grid'>";
  chunk += "<div class='info-item'><div class='info-label'>" + String(T().full_model) + "</div><div class='info-value'>" + diagnosticData.chipModel + " Rev" + diagnosticData.chipRevision + "</div></div>";
  chunk += "<div class='info-item'><div class='info-label'>" + String(T().cpu_cores) + "</div><div class='info-value'>" + String(diagnosticData.cpuCores) + " @ " + String(diagnosticData.cpuFreqMHz) + " MHz</div></div>";
  chunk += "<div class='info-item'><div class='info-label'>" + String(T().mac_wifi) + "</div><div class='info-value'>" + diagnosticData.macAddress + "</div></div>";
  chunk += "<div class='info-item'><div class='info-label'>" + String(T().last_reset) + "</div><div class='info-value'>" + getResetReason() + "</div></div>";
  chunk += "<div class='info-item'><div class='info-label'>" + String(T().chip_features) + "</div><div class='info-value'>" + getChipFeatures() + "</div></div>";
  chunk += "<div class='info-item'><div class='info-label'>" + String(T().sdk_version) + "</div><div class='info-value'>" + diagnosticData.sdkVersion + "</div></div>";
  chunk += "<div class='info-item'><div class='info-label'>" + String(T().idf_version) + "</div><div class='info-value'>" + diagnosticData.idfVersion + "</div></div>";
  
  unsigned long seconds = diagnosticData.uptime / 1000;
  unsigned long minutes = seconds / 60;
  unsigned long hours = minutes / 60;
  unsigned long days = hours / 24;
  chunk += "<div class='info-item'><div class='info-label'>" + String(T().uptime) + "</div><div class='info-value'>" + String(days) + "j " + String(hours % 24) + "h " + String(minutes % 60) + "m</div></div>";
  
  if (diagnosticData.temperature != -999) {
    chunk += "<div class='info-item'><div class='info-label'>" + String(T().cpu_temp) + "</div><div class='info-value'>" + String(diagnosticData.temperature, 1) + " °C</div></div>";
  }
  chunk += "</div></div>";
  server.sendContent(chunk);
  
  // Memory - Flash
  chunk = "<div class='section'><h2>" + String(T().memory_details) + "</h2>";
  chunk += "<h3>" + String(T().flash_memory) + "</h3><div class='info-grid'>";
  chunk += "<div class='info-item'><div class='info-label'>" + String(T().real_size) + "</div><div class='info-value'>" + String(detailedMemory.flashSizeReal / 1048576.0, 2) + " MB</div></div>";
  chunk += "<div class='info-item'><div class='info-label'>" + String(T().configured_ide) + "</div><div class='info-value'>" + String(detailedMemory.flashSizeChip / 1048576.0, 2) + " MB</div></div>";
  
  bool flashMatch = (detailedMemory.flashSizeReal == detailedMemory.flashSizeChip);
  chunk += "<div class='info-item'><div class='info-label'>" + String(T().configuration) + "</div><div class='info-value'><span class='badge ";
  chunk += flashMatch ? "badge-success'>" + String(T().correct) : "badge-warning'>" + String(T().to_fix);
  chunk += "</span></div></div>";
  chunk += "<div class='info-item'><div class='info-label'>" + String(T().flash_type) + "</div><div class='info-value'>" + getFlashType() + "</div></div>";
  chunk += "<div class='info-item'><div class='info-label'>" + String(T().flash_speed) + "</div><div class='info-value'>" + getFlashSpeed() + "</div></div>";
  chunk += "</div>";
  server.sendContent(chunk);
  
  // Memory - PSRAM
  chunk = "<h3>" + String(T().psram_external) + "</h3><div class='info-grid'>";
  if (detailedMemory.psramAvailable) {
    chunk += "<div class='info-item'><div class='info-label'>" + String(T().hardware_status) + "</div><div class='info-value'><span class='badge badge-success'>" + String(T().detected_active) + "</span></div></div>";
    chunk += "<div class='info-item'><div class='info-label'>" + String(T().total_size) + "</div><div class='info-value'>" + String(detailedMemory.psramTotal / 1048576.0, 2) + " MB</div></div>";
    chunk += "<div class='info-item'><div class='info-label'>" + String(T().free) + "</div><div class='info-value'>" + String(detailedMemory.psramFree / 1048576.0, 2) + " MB</div></div>";
    chunk += "<div class='info-item'><div class='info-label'>" + String(T().used) + "</div><div class='info-value'>" + String(detailedMemory.psramUsed / 1048576.0, 2) + " MB</div></div>";
  } else if (detailedMemory.psramBoardSupported) {
    chunk += "<div class='info-item'><div class='info-label'>" + String(T().hardware_status) + "</div><div class='info-value'><span class='badge badge-warning'>" + String(T().supported_not_enabled) + "</span></div></div>";
    String psramHint = String(T().enable_psram_hint);
    psramHint.replace("%TYPE%", detailedMemory.psramType ? detailedMemory.psramType : "PSRAM");
    chunk += "<div class='info-item' style='grid-column:1/-1'><div class='info-label'>" + String(T().ide_config) + "</div><div class='info-value'>" + psramHint + "</div></div>";
  } else {
    chunk += "<div class='info-item'><div class='info-label'>" + String(T().hardware_status) + "</div><div class='info-value'><span class='badge badge-danger'>" + String(T().not_detected) + "</span></div></div>";
  }
  chunk += "</div>";
  server.sendContent(chunk);
  
  // Memory - SRAM
  chunk = "<h3>" + String(T().internal_sram) + "</h3><div class='info-grid'>";
  chunk += "<div class='info-item'><div class='info-label'>" + String(T().total_size) + "</div><div class='info-value'>" + String(detailedMemory.sramTotal / 1024.0, 2) + " KB</div></div>";
  chunk += "<div class='info-item'><div class='info-label'>" + String(T().free) + "</div><div class='info-value'>" + String(detailedMemory.sramFree / 1024.0, 2) + " KB</div></div>";
  chunk += "<div class='info-item'><div class='info-label'>" + String(T().used) + "</div><div class='info-value'>" + String(detailedMemory.sramUsed / 1024.0, 2) + " KB</div></div>";
  chunk += "<div class='info-item'><div class='info-label'>" + String(T().memory_fragmentation) + "</div><div class='info-value'>" + String(detailedMemory.fragmentationPercent, 1) + "%</div></div>";
  chunk += "</div>";
  chunk += "<div style='text-align:center;margin-top:15px'><button class='btn btn-info' onclick='location.reload()'>" + String(T().refresh_memory) + "</button></div></div>";
  server.sendContent(chunk);
  
  // WiFi
  chunk = "<div class='section'><h2>" + String(T().wifi_connection) + "</h2><div class='info-grid'>";
  chunk += "<div class='info-item'><div class='info-label'>" + String(T().connected_ssid) + "</div><div class='info-value'>" + diagnosticData.wifiSSID + "</div></div>";
  chunk += "<div class='info-item'><div class='info-label'>" + String(T().signal_power) + "</div><div class='info-value'>" + String(diagnosticData.wifiRSSI) + " dBm</div></div>";
  chunk += "<div class='info-item'><div class='info-label'>" + String(T().signal_quality) + "</div><div class='info-value'>" + getWiFiSignalQuality() + "</div></div>";
  chunk += "<div class='info-item'><div class='info-label'>" + String(T().ip_address) + "</div><div class='info-value'>" + diagnosticData.ipAddress + "</div></div>";
  chunk += "<div class='info-item'><div class='info-label'>" + String(T().subnet_mask) + "</div><div class='info-value'>" + WiFi.subnetMask().toString() + "</div></div>";
  chunk += "<div class='info-item'><div class='info-label'>" + String(T().gateway) + "</div><div class='info-value'>" + WiFi.gatewayIP().toString() + "</div></div>";
  chunk += "<div class='info-item'><div class='info-label' data-i18n='bluetooth_section'>" + String(T().bluetooth_section) + "</div><div class='info-value'>" + htmlEscape(bluetoothSummary) + "</div></div>";
  chunk += "<div class='info-item'><div class='info-label' data-i18n='bluetooth_mac'>" + String(T().bluetooth_mac) + "</div><div class='info-value'>" + diagnosticData.bluetoothAddress + "</div></div>";
  chunk += "</div></div>";
  server.sendContent(chunk);
  
  // GPIO et I2C
  chunk = "<div class='section'><h2>" + String(T().gpio_interfaces) + "</h2><div class='info-grid'>";
  chunk += "<div class='info-item'><div class='info-label'>" + String(T().total_gpio) + "</div><div class='info-value'>" + String(diagnosticData.totalGPIO) + " " + String(T().pins) + "</div></div>";
  if (ENABLE_I2C_SCAN) {
    chunk += "<div class='info-item'><div class='info-label'>" + String(T().i2c_peripherals) + "</div><div class='info-value'>" + String(diagnosticData.i2cCount) + " " + String(T().devices) + "</div></div>";
    if (diagnosticData.i2cCount > 0) {
      chunk += "<div class='info-item' style='grid-column:1/-1'><div class='info-label'>" + String(T().detected_addresses) + "</div><div class='info-value'>" + diagnosticData.i2cDevices + "</div></div>";
    }
  }
  chunk += "</div></div>";
  chunk += "</div>"; // Fermeture div overview
  server.sendContent(chunk);

  // CHUNK 4: TAB LEDs
  chunk = "<div id='leds' class='tab-content'>";
  chunk += "<div class='section'><h2>" + String(T().builtin_led) + "</h2><div class='info-grid'>";
  chunk += "<div class='info-item'><div class='info-label'>" + String(T().gpio) + "</div><div class='info-value'>GPIO " + String(BUILTIN_LED_PIN) + "</div></div>";
  chunk += "<div class='info-item'><div class='info-label'>" + String(T().status) + "</div><div class='info-value' id='builtin-led-status'>" + builtinLedTestResult + "</div></div>";
  chunk += "<div class='info-item' style='grid-column:1/-1;text-align:center'>";
  chunk += "<input type='number' id='ledGPIO' value='" + String(BUILTIN_LED_PIN) + "' min='0' max='48' style='width:80px'>";
  chunk += "<button class='btn btn-info' onclick='configBuiltinLED()'>" + String(T().config) + "</button>";
  chunk += "<button class='btn btn-primary' onclick='testBuiltinLED()'>" + String(T().test) + "</button>";
  chunk += "<button class='btn btn-success' onclick='ledBlink()'>" + String(T().blink) + "</button>";
  chunk += "<button class='btn btn-info' onclick='ledFade()'>" + String(T().fade) + "</button>";
  chunk += "<button class='btn btn-danger' onclick='ledOff()'>" + String(T().off) + "</button>";
  chunk += "</div></div></div>";
  
  chunk += "<div class='section'><h2>" + String(T().neopixel) + "</h2><div class='info-grid'>";
  chunk += "<div class='info-item'><div class='info-label'>" + String(T().gpio) + "</div><div class='info-value'>GPIO " + String(LED_PIN) + "</div></div>";
  chunk += "<div class='info-item'><div class='info-label'>" + String(T().status) + "</div><div class='info-value' id='neopixel-status'>" + neopixelTestResult + "</div></div>";
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
  chunk += "<div class='section'><h2>" + String(T().oled_screen) + "</h2><div class='info-grid'>";
  chunk += "<div class='info-item'><div class='info-label'>" + String(T().status) + "</div><div class='info-value' id='oled-status'>" + oledTestResult + "</div></div>";
  chunk += "<div class='info-item'><div class='info-label'>" + String(T().i2c_pins) + "</div><div class='info-value' id='oled-pins'>SDA:" + String(I2C_SDA) + " SCL:" + String(I2C_SCL) + "</div></div>";
  chunk += "<div class='info-item'><div class='info-label'>" + String(T().rotation) + "</div><div class='info-value' id='oled-rotation-display'>" + String(oledRotation) + "</div></div>";
  chunk += "<div class='info-item' style='grid-column:1/-1;text-align:center'>";
  chunk += "SDA: <input type='number' id='oledSDA' value='" + String(I2C_SDA) + "' min='0' max='48' style='width:70px'> ";
  chunk += "SCL: <input type='number' id='oledSCL' value='" + String(I2C_SCL) + "' min='0' max='48' style='width:70px'> ";
  chunk += String(T().rotation) + ": <select id='oledRotation' style='padding:10px;border:2px solid #ddd;border-radius:5px'>";
  for (int rot = 0; rot < 4; rot++) {
    chunk += "<option value='" + String(rot) + "'" + String(oledRotation == rot ? " selected" : "") + ">" + String(rot) + "</option>";
  }
  chunk += "</select> ";
  chunk += "<button class='btn btn-info' onclick='configOLED()'>" + String(T().apply_redetect) + "</button>";
  if (oledAvailable) {
    chunk += "<div style='margin-top:15px'>";
    chunk += "<button class='btn btn-primary' onclick='testOLED()'>" + String(T().full_test) + "</button>";
    chunk += "</div>";
    chunk += "<div class='oled-step-grid' style='margin-top:15px;display:grid;grid-template-columns:repeat(auto-fit,minmax(180px,1fr));gap:10px'>";
    chunk += "<button class='btn btn-secondary' onclick='oledStep(\"welcome\")'>" + String(T().oled_step_welcome) + "</button>";
    chunk += "<button class='btn btn-secondary' onclick='oledStep(\"big_text\")'>" + String(T().oled_step_big_text) + "</button>";
    chunk += "<button class='btn btn-secondary' onclick='oledStep(\"text_sizes\")'>" + String(T().oled_step_text_sizes) + "</button>";
    chunk += "<button class='btn btn-secondary' onclick='oledStep(\"shapes\")'>" + String(T().oled_step_shapes) + "</button>";
    chunk += "<button class='btn btn-secondary' onclick='oledStep(\"horizontal_lines\")'>" + String(T().oled_step_horizontal_lines) + "</button>";
    chunk += "<button class='btn btn-secondary' onclick='oledStep(\"diagonals\")'>" + String(T().oled_step_diagonals) + "</button>";
    chunk += "<button class='btn btn-secondary' onclick='oledStep(\"moving_square\")'>" + String(T().oled_step_moving_square) + "</button>";
    chunk += "<button class='btn btn-secondary' onclick='oledStep(\"progress_bar\")'>" + String(T().oled_step_progress_bar) + "</button>";
    chunk += "<button class='btn btn-secondary' onclick='oledStep(\"scroll_text\")'>" + String(T().oled_step_scroll_text) + "</button>";
    chunk += "<button class='btn btn-secondary' onclick='oledStep(\"final_message\")'>" + String(T().oled_step_final_message) + "</button>";
    chunk += "</div>";
    chunk += "<div style='margin-top:15px'>";
    chunk += "<input type='text' id='oledMsg' placeholder='" + String(T().custom_message) + "' style='width:250px;margin:0 5px'>";
    chunk += "<button class='btn btn-success' onclick='oledMessage()'>" + String(T().show_message) + "</button>";
    chunk += "</div>";
  }
  chunk += "</div></div></div></div>";
  server.sendContent(chunk);
  
  // CHUNK 6: TAB Tests
  chunk = "<div id='tests' class='tab-content'>";
  chunk += "<div class='section'><h2>" + String(T().i2c_peripherals) + "</h2>";
  chunk += "<div style='text-align:center;margin:20px 0'>";
  chunk += "<button class='btn btn-primary' onclick='scanI2C()'>" + String(T().rescan_i2c) + "</button>";
  chunk += "<div id='i2c-status' class='status-live'>" + String(T().click_button) + "</div>";
  chunk += "</div></div>";
  chunk += "<div class='section'><h2>" + String(T().adc_test) + "</h2>";
  chunk += "<div style='text-align:center;margin:20px 0'>";
  chunk += "<button class='btn btn-primary' onclick='testADC()'>" + String(T().test) + "</button>";
  chunk += "<div id='adc-status' class='status-live'>" + adcTestResult + "</div>";
  chunk += "</div><div id='adc-results' class='info-grid'></div></div>";
  
  chunk += "<div class='section'><h2>" + String(T().touch_test) + "</h2>";
  chunk += "<div style='text-align:center;margin:20px 0'>";
  chunk += "<button class='btn btn-primary' onclick='testTouch()'>" + String(T().test) + "</button>";
  chunk += "<div id='touch-status' class='status-live'>" + touchTestResult + "</div>";
  chunk += "</div><div id='touch-results' class='info-grid'></div></div>";
  
  chunk += "<div class='section'><h2>" + String(T().pwm_test) + "</h2>";
  chunk += "<div style='text-align:center;margin:20px 0'>";
  chunk += "<button class='btn btn-primary' onclick='testPWM()'>" + String(T().test) + "</button>";
  chunk += "<div id='pwm-status' class='status-live'>" + pwmTestResult + "</div>";
  chunk += "</div></div>";
  
  chunk += "<div class='section'><h2>" + String(T().spi_bus) + "</h2>";
  chunk += "<div style='text-align:center;margin:20px 0'>";
  chunk += "<button class='btn btn-primary' onclick='scanSPI()'>" + String(T().scan) + "</button>";
  chunk += "<div id='spi-status' class='status-live'>" + (spiInfo.length() > 0 ? spiInfo : String(T().click_button)) + "</div>";
  chunk += "</div></div>";
  
  chunk += "<div class='section'><h2>" + String(T().flash_partitions) + "</h2>";
  chunk += "<div style='text-align:center;margin:20px 0'>";
  chunk += "<button class='btn btn-primary' onclick='listPartitions()'>" + String(T().list_partitions) + "</button>";
  chunk += "</div><div id='partitions-results' style='background:#fff;padding:15px;border-radius:10px;font-family:monospace;white-space:pre-wrap;font-size:0.85em'>";
  chunk += partitionsInfo.length() > 0 ? partitionsInfo : String(T().click_button);
  chunk += "</div></div>";
  
  chunk += "<div class='section'><h2>" + String(T().memory_stress) + "</h2>";
  chunk += "<div style='text-align:center;margin:20px 0'>";
  chunk += "<button class='btn btn-danger' onclick='stressTest()'>" + String(T().start_stress) + "</button>";
  chunk += "<div id='stress-status' class='status-live'>" + stressTestResult + "</div>";
  chunk += "</div></div></div>";
  server.sendContent(chunk);
  
  // CHUNK 7: TAB GPIO
  chunk = "<div id='gpio' class='tab-content'>";
  chunk += "<div class='section'><h2>" + String(T().gpio_test) + "</h2>";
  chunk += "<div style='text-align:center;margin:20px 0'>";
  chunk += "<button class='btn btn-primary' onclick='testAllGPIO()'>" + String(T().test_all_gpio) + "</button>";
  chunk += "<div id='gpio-status' class='status-live'>" + String(T().click_to_test) + "</div>";
  chunk += "</div>";
  // --- [NEW FEATURE] Avertissement contextualisé pour le test GPIO ---
  chunk += "<div class='warning-callout' data-i18n='gpio_warning'>" + String(T().gpio_warning) + "</div>";
  chunk += "<div id='gpio-results' class='gpio-grid'></div></div></div>";
  server.sendContent(chunk);
  
  // CHUNK 8: TAB Wireless
  chunk = "<div id='wireless' class='tab-content'>";
  chunk += "<div class='section'><h2>" + String(T().wifi_scanner) + "</h2>";
  chunk += "<div style='text-align:center;margin:20px 0'>";
  chunk += "<button class='btn btn-primary' onclick='scanWiFi()'>" + String(T().scan_networks) + "</button>";
  chunk += "<div id='wifi-status' class='status-live'>" + String(T().click_to_test) + "</div>";
  chunk += "</div><div id='wifi-results' class='wifi-list'></div></div>";
  chunk += "<div class='section'><h2 data-i18n='bluetooth_section'>" + String(T().bluetooth_section) + "</h2>";
  chunk += "<div class='info-grid'>";
  chunk += "<div class='info-item'><div class='info-label' data-i18n='bluetooth_status'>" + String(T().bluetooth_status) + "</div><div class='info-value' id='bluetooth-status'>" + htmlEscape(bluetoothStatusText) + "</div></div>";
  chunk += "<div class='info-item'><div class='info-label' data-i18n='bluetooth_name'>" + String(T().bluetooth_name) + "</div><div class='info-value' id='bluetooth-name'>" + htmlEscape(bluetoothNameDisplay) + "</div></div>";
  chunk += "<div class='info-item'><div class='info-label' data-i18n='bluetooth_mac'>" + String(T().bluetooth_mac) + "</div><div class='info-value' id='bluetooth-mac'>" + diagnosticData.bluetoothAddress + "</div></div>";
  chunk += "</div>";
  chunk += "<div class='card' style='margin-top:20px'>";
  chunk += "<h3 data-i18n='bluetooth_actions'>" + String(T().bluetooth_actions) + "</h3>";
  chunk += "<div style='display:flex;flex-wrap:wrap;gap:10px;justify-content:center;margin-top:15px'>";
  chunk += "<button type='button' class='btn btn-success' data-i18n='bluetooth_enable' onclick=\"toggleBluetooth(true)\" data-bt-control='1'" + bluetoothDisableAttr + ">" + String(T().bluetooth_enable) + "</button>";
  chunk += "<button type='button' class='btn btn-warning' data-i18n='bluetooth_disable' onclick=\"toggleBluetooth(false)\" data-bt-control='1'" + bluetoothDisableAttr + ">" + String(T().bluetooth_disable) + "</button>";
  chunk += "<input type='text' id='bluetoothNameInput' data-bt-control='1' value='" + htmlEscape(bluetoothNameDisplay) + "' placeholder='" + String(T().bluetooth_placeholder) + "'" + bluetoothDisableAttr + ">";
  chunk += "<button type='button' class='btn btn-info' data-i18n='bluetooth_rename' onclick=\"renameBluetooth()\" data-bt-control='1'" + bluetoothDisableAttr + ">" + String(T().bluetooth_rename) + "</button>";
  chunk += "<button type='button' class='btn btn-danger' data-i18n='bluetooth_reset' onclick=\"resetBluetooth()\" data-bt-control='1'" + bluetoothDisableAttr + ">" + String(T().bluetooth_reset) + "</button>";
  chunk += "</div>";
  chunk += "<div id='bluetooth-feedback' class='status-live'>" + htmlEscape(bluetoothFeedbackText) + "</div>";
  chunk += "</div></div></div>";
  server.sendContent(chunk);
  
  // CHUNK 9: TAB Benchmark
  chunk = "<div id='benchmark' class='tab-content'>";
  chunk += "<div class='section'><h2>" + String(T().performance_bench) + "</h2>";
  chunk += "<div style='text-align:center;margin:20px 0'>";
  chunk += "<button class='btn btn-primary' onclick='runBenchmarks()'>" + String(T().run_benchmarks) + "</button>";
  chunk += "</div><div id='benchmark-results' class='info-grid'>";
  chunk += "<div class='info-item'><div class='info-label'>" + String(T().cpu_benchmark) + "</div><div class='info-value' id='cpu-bench'>" + String(T().not_tested) + "</div></div>";
  chunk += "<div class='info-item'><div class='info-label'>" + String(T().memory_benchmark) + "</div><div class='info-value' id='mem-bench'>" + String(T().not_tested) + "</div></div>";
  chunk += "<div class='info-item'><div class='info-label'>" + String(T().cpu_performance) + "</div><div class='info-value' id='cpu-perf'>-</div></div>";
  chunk += "<div class='info-item'><div class='info-label'>" + String(T().memory_speed) + "</div><div class='info-value' id='mem-speed'>-</div></div>";
  chunk += "</div></div></div>";
  server.sendContent(chunk);
  
  // CHUNK 10: TAB Export
  chunk = "<div id='export' class='tab-content'>";
  chunk += "<div class='section'><h2>" + String(T().data_export) + "</h2>";
  chunk += "<div style='display:grid;grid-template-columns:repeat(auto-fit,minmax(200px,1fr));gap:20px;margin-top:20px'>";
  chunk += "<div style='text-align:center;padding:30px;background:#fff;border-radius:10px;border:2px solid #667eea'>";
  chunk += "<h3 style='color:#667eea;margin-bottom:15px'>" + String(T().txt_file) + "</h3>";
  chunk += "<p style='font-size:0.9em;color:#666;margin-bottom:15px'>" + String(T().readable_report) + "</p>";
  chunk += "<a href='/export/txt' class='btn btn-primary'>" + String(T().download_txt) + "</a></div>";
  chunk += "<div style='text-align:center;padding:30px;background:#fff;border-radius:10px;border:2px solid #3a7bd5'>";
  chunk += "<h3 style='color:#3a7bd5;margin-bottom:15px'>" + String(T().json_file) + "</h3>";
  chunk += "<p style='font-size:0.9em;color:#666;margin-bottom:15px'>" + String(T().structured_format) + "</p>";
  chunk += "<a href='/export/json' class='btn btn-info'>" + String(T().download_json) + "</a></div>";
  chunk += "<div style='text-align:center;padding:30px;background:#fff;border-radius:10px;border:2px solid #56ab2f'>";
  chunk += "<h3 style='color:#56ab2f;margin-bottom:15px'>" + String(T().csv_file) + "</h3>";
  chunk += "<p style='font-size:0.9em;color:#666;margin-bottom:15px'>" + String(T().for_excel) + "</p>";
  chunk += "<a href='/export/csv' class='btn btn-success'>" + String(T().download_csv) + "</a></div>";
  chunk += "<div style='text-align:center;padding:30px;background:#fff;border-radius:10px;border:2px solid #667eea'>";
  chunk += "<h3 style='color:#667eea;margin-bottom:15px'>" + String(T().printable_version) + "</h3>";
  chunk += "<p style='font-size:0.9em;color:#666;margin-bottom:15px'>" + String(T().pdf_format) + "</p>";
  chunk += "<a href='/print' target='_blank' class='btn btn-primary'>" + String(T().open) + "</a></div>";
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
  chunk += "document.documentElement.setAttribute('lang',currentLang);";
  chunk += "function showInlineMessage(text,state){var holder=document.getElementById('inlineMessage');if(!holder){return;}holder.className='inline-message';if(!text){return;}holder.textContent=text;holder.classList.add('show');if(state){holder.classList.add(state);}}";
  chunk += "function clearInlineMessage(){var holder=document.getElementById('inlineMessage');if(!holder){return;}holder.className='inline-message';holder.textContent='';}";
  chunk += "function updateStatus(id,text,state){var el=document.getElementById(id);if(el){el.textContent=text;el.classList.remove('success','error');if(state){el.classList.add(state);}}if(state==='error'||state==='success'){showInlineMessage(text,state);}else if(text===''){clearInlineMessage();}}";
  chunk += "function connectionLabelText(online){return online?(currentLang==='fr'?'En ligne':'Online'):(currentLang==='fr'?'Hors ligne':'Offline');}";
  chunk += "function updateStatusIndicator(online){connectionState=!!online;var indicator=document.getElementById('statusIndicator');var label=document.getElementById('connectionLabel');if(indicator){indicator.classList.remove('status-online','status-offline');indicator.classList.add(online?'status-online':'status-offline');}if(label){label.textContent=connectionLabelText(online);}if(!online){showInlineMessage(connectionLabelText(false)+' — '+(currentLang==='fr'?'Vérifiez le réseau.':'Check the network.'),'error');}else{clearInlineMessage();}}";

  chunk += "function updateBluetoothFeedback(message,state,notify){var box=document.getElementById('bluetooth-feedback');if(!box){return;}if(typeof message==='undefined'||message===null){message='';}box.textContent=message;box.classList.remove('success','error');if(state){box.classList.add(state);}if(notify&&message){if(state==='error'){showInlineMessage(message,'error');}else if(state==='success'){showInlineMessage(message,'success');}}else if(notify&&!message){clearInlineMessage();}}";
  chunk += "function applyBluetoothState(data,notify){if(!data){return;}var statusEl=document.getElementById('bluetooth-status');if(statusEl&&typeof data.status!=='undefined'){statusEl.textContent=data.status;}var nameEl=document.getElementById('bluetooth-name');if(nameEl&&typeof data.name!=='undefined'){nameEl.textContent=data.name;}var macEl=document.getElementById('bluetooth-mac');if(macEl&&typeof data.mac!=='undefined'){macEl.textContent=data.mac;}var input=document.getElementById('bluetoothNameInput');if(input&&typeof data.name!=='undefined'){input.value=data.name;}var summary=document.getElementById('bluetoothSummary');if(summary&&typeof data.summary!=='undefined'){summary.textContent=data.summary;}var controls=document.querySelectorAll('[data-bt-control=\"1\"]');if(controls&&controls.length){for(var i=0;i<controls.length;i++){controls[i].disabled=(data.supported===false);}}var message=(typeof data.message==='string')?data.message:'';var state=null;if(typeof data.success!=='undefined'){state=data.success?'success':'error';}if(notify){updateBluetoothFeedback(message,state,true);}else if(data.supported===false){updateBluetoothFeedback(message,'error',false);}else{updateBluetoothFeedback('',null,false);}}";
  chunk += "function refreshBluetoothStatus(showNotice){fetch('/api/bluetooth/status').then(function(r){return r.json();}).then(function(data){applyBluetoothState(data,showNotice);}).catch(function(err){var message=(currentLang==='fr'?'Erreur Bluetooth: ':'Bluetooth error: ')+(err&&err.message?err.message:err);updateBluetoothFeedback(message,'error',showNotice);});}";
  chunk += "function toggleBluetooth(enable){updateBluetoothFeedback(enable?(currentLang==='fr'?'Activation...':'Enabling...'):(currentLang==='fr'?'Désactivation...':'Disabling...'),null,false);fetch('/api/bluetooth/toggle?state='+(enable?'on':'off')).then(function(r){return r.json();}).then(function(data){applyBluetoothState(data,true);}).catch(function(err){var message=(currentLang==='fr'?'Erreur Bluetooth: ':'Bluetooth error: ')+(err&&err.message?err.message:err);updateBluetoothFeedback(message,'error',true);});return false;}";
  chunk += "function renameBluetooth(){var input=document.getElementById('bluetoothNameInput');if(!input){return false;}var value=input.value||'';if(!value.trim()){updateBluetoothFeedback(currentLang==='fr'?'Nom Bluetooth invalide':'Invalid Bluetooth name','error',true);return false;}updateBluetoothFeedback(currentLang==='fr'?'Mise à jour...':'Updating...',null,false);fetch('/api/bluetooth/name?name='+encodeURIComponent(value.trim())).then(function(r){return r.json();}).then(function(data){applyBluetoothState(data,true);}).catch(function(err){var message=(currentLang==='fr'?'Erreur Bluetooth: ':'Bluetooth error: ')+(err&&err.message?err.message:err);updateBluetoothFeedback(message,'error',true);});return false;}";
  chunk += "function resetBluetooth(){updateBluetoothFeedback(currentLang==='fr'?'Réinitialisation...':'Resetting...',null,false);fetch('/api/bluetooth/reset').then(function(r){return r.json();}).then(function(data){applyBluetoothState(data,true);}).catch(function(err){var message=(currentLang==='fr'?'Erreur Bluetooth: ':'Bluetooth error: ')+(err&&err.message?err.message:err);updateBluetoothFeedback(message,'error',true);});return false;}";

  chunk += "function changeLang(lang,btn){";
  chunk += "fetch('/api/set-language?lang='+lang).then(function(r){return r.json();}).then(function(d){";
  chunk += "if(!d.success){throw new Error(d.message||'lang');}";
  chunk += "currentLang=lang;";
  chunk += "document.documentElement.setAttribute('lang',lang);";
  chunk += "var langButtons=document.querySelectorAll('.lang-btn');";
  chunk += "for(var i=0;i<langButtons.length;i++){langButtons[i].classList.remove('active');}";
  chunk += "if(btn){btn.classList.add('active');}";
  chunk += "updateTranslations(true);";
  chunk += "}).catch(function(err){var message=err&&err.message?err.message:err;showInlineMessage((currentLang==='fr'?'Erreur changement langue: ':'Language switch error: ')+message,'error');});";
  chunk += "return false;";
  chunk += "}";

  chunk += "function updateTranslations(showNotice){";
  chunk += "fetch('/api/get-translations').then(function(r){return r.json();}).then(function(tr){";
  chunk += "var mainTitle=document.getElementById('main-title');if(mainTitle){mainTitle.textContent=tr.title+' v" + String(DIAGNOSTIC_VERSION) + "';}";
  chunk += "var nodes=document.querySelectorAll('[data-i18n]');";
  chunk += "for(var i=0;i<nodes.length;i++){var key=nodes[i].getAttribute('data-i18n');if(tr[key]){nodes[i].textContent=tr[key];}}";
  chunk += "var btInput=document.getElementById('bluetoothNameInput');if(btInput&&tr.bluetooth_placeholder){btInput.setAttribute('placeholder',tr.bluetooth_placeholder);}";
  chunk += "refreshBluetoothStatus(false);";
  chunk += "updateStatusIndicator(connectionState);";
  chunk += "if(showNotice){showInlineMessage(currentLang==='fr'?'Langue mise à jour':'Language updated','success');}";
  chunk += "}).catch(function(err){var message=err&&err.message?err.message:err;showInlineMessage((currentLang==='fr'?'Erreur traduction: ':'Translation error: ')+message,'error');});";
  chunk += "}";

  // --- [NEW FEATURE] Navigation accessible avec repli hash ---
  chunk += "function findNavTrigger(el){while(el&&el.classList&&!el.classList.contains('nav-link')){el=el.parentElement;}if(el&&el.classList&&el.classList.contains('nav-link')){return el;}return null;}";
  chunk += "function showTab(tabId,trigger,updateHash){if(!tabId){return false;}if(tabId==='wifi'){tabId='wireless';}clearInlineMessage();var tabs=document.querySelectorAll('.tab-content');for(var i=0;i<tabs.length;i++){tabs[i].classList.remove('active');}var target=document.getElementById(tabId);if(target){target.classList.add('active');if(typeof target.scrollIntoView==='function'){target.scrollIntoView();}}var buttons=document.querySelectorAll('.nav-link');for(var j=0;j<buttons.length;j++){buttons[j].classList.remove('active');buttons[j].removeAttribute('aria-current');}var actual=trigger;if(!actual||!actual.classList){var selector=\".nav-link[data-target='\"+tabId+\"']\";actual=document.querySelector(selector);}if(actual&&actual.classList){actual.classList.add('active');actual.setAttribute('aria-current','page');}if(updateHash!==false){if(window.location.hash!=='#'+tabId){ignoreHashChange=true;window.location.hash=tabId;setTimeout(function(){ignoreHashChange=false;},0);}}return false;}";
  chunk += "function initStickyNav(){var wrapper=document.querySelector('.nav-wrapper');if(!wrapper||wrapper.getAttribute('data-sticky-init')==='1'){return;}wrapper.setAttribute('data-sticky-init','1');var header=document.querySelector('.app-header');var apply=function(state){if(state){wrapper.classList.add('is-sticky');}else{wrapper.classList.remove('is-sticky');}};";
  chunk += "if('IntersectionObserver' in window&&header){var observer=new IntersectionObserver(function(entries){for(var i=0;i<entries.length;i++){if(entries[i].target===header){apply(!entries[i].isIntersecting);}}},{threshold:0,rootMargin:'-1px 0px 0px 0px'});observer.observe(header);}else{var last=false;window.addEventListener('scroll',function(){var offset=window.pageYOffset||document.documentElement.scrollTop||0;var limit=header?header.offsetHeight:220;var state=offset>limit;if(state!==last){last=state;apply(state);}});} }";
  chunk += "function initNavigation(){var navs=document.querySelectorAll('.primary-nav');if(navs&&navs.length){for(var n=0;n<navs.length;n++){(function(nav){nav.addEventListener('click',function(evt){var source=evt.target||evt.srcElement;var button=findNavTrigger(source);if(!button){return;}evt.preventDefault();var targetTab=button.getAttribute('data-target');if(targetTab){showTab(targetTab,button);}});})(navs[n]);}}var initial=window.location.hash?window.location.hash.substring(1):null;var defaultButton=document.querySelector('.nav-link.active');if(initial==='wifi'){initial='wireless';}if(!initial&&defaultButton){initial=defaultButton.getAttribute('data-target');}if(!initial){var list=document.querySelectorAll('.nav-link');if(list.length>0){initial=list[0].getAttribute('data-target');defaultButton=list[0];}}var initialButton=null;if(initial){initialButton=document.querySelector(\".nav-link[data-target='\"+initial+\"']\");}if(initial){showTab(initial,initialButton,false);}else{showTab('overview',null,false);}initStickyNav();updateStatusIndicator(connectionState);refreshBluetoothStatus(false);}";
  chunk += "window.addEventListener('hashchange',function(){if(ignoreHashChange){return;}var target=window.location.hash?window.location.hash.substring(1):'overview';if(target==='wifi'){target='wireless';}showTab(target,null,false);});";
  chunk += "if(document.readyState==='loading'){document.addEventListener('DOMContentLoaded',initNavigation);}else{initNavigation();}";

  chunk += "function configBuiltinLED(){updateStatus('builtin-led-status','Configuration...',null);fetch('/api/builtin-led-config?gpio='+document.getElementById('ledGPIO').value).then(function(r){return r.json();}).then(function(d){var state=d.success?'success':'error';updateStatus('builtin-led-status',d.message||'GPIO invalide',state);}).catch(function(e){updateStatus('builtin-led-status','Erreur: '+e,'error');});}";
  chunk += "function testBuiltinLED(){updateStatus('builtin-led-status','Test...',null);fetch('/api/builtin-led-test').then(function(r){return r.json();}).then(function(d){var state=d.success?'success':'error';updateStatus('builtin-led-status',d.result||'Test en echec',state);}).catch(function(e){updateStatus('builtin-led-status','Erreur: '+e,'error');});}";
  chunk += "function ledBlink(){fetch('/api/builtin-led-control?action=blink').then(function(r){return r.json();}).then(function(d){updateStatus('builtin-led-status',d.message,d.success?'success':'error');}).catch(function(e){updateStatus('builtin-led-status','Erreur: '+e,'error');});}";
  chunk += "function ledFade(){fetch('/api/builtin-led-control?action=fade').then(function(r){return r.json();}).then(function(d){updateStatus('builtin-led-status',d.message,d.success?'success':'error');}).catch(function(e){updateStatus('builtin-led-status','Erreur: '+e,'error');});}";
  chunk += "function ledOff(){fetch('/api/builtin-led-control?action=off').then(function(r){return r.json();}).then(function(d){updateStatus('builtin-led-status',d.message,d.success?'success':'error');}).catch(function(e){updateStatus('builtin-led-status','Erreur: '+e,'error');});}";

  chunk += "function configNeoPixel(){updateStatus('neopixel-status','Configuration...',null);fetch('/api/neopixel-config?gpio='+document.getElementById('neoGPIO').value+'&count='+document.getElementById('neoCount').value).then(function(r){return r.json();}).then(function(d){updateStatus('neopixel-status',d.message,d.success?'success':'error');}).catch(function(e){updateStatus('neopixel-status','Erreur: '+e,'error');});}";
  chunk += "function testNeoPixel(){updateStatus('neopixel-status','Test...',null);fetch('/api/neopixel-test').then(function(r){return r.json();}).then(function(d){updateStatus('neopixel-status',d.result,d.success?'success':'error');}).catch(function(e){updateStatus('neopixel-status','Erreur: '+e,'error');});}";
  chunk += "function neoPattern(p){fetch('/api/neopixel-pattern?pattern='+p).then(function(r){return r.json();}).then(function(d){updateStatus('neopixel-status',d.message,d.success?'success':'error');}).catch(function(e){updateStatus('neopixel-status','Erreur: '+e,'error');});}";
  chunk += "function neoCustomColor(){var c=document.getElementById('neoColor').value;var r=parseInt(c.substr(1,2),16);var g=parseInt(c.substr(3,2),16);var b=parseInt(c.substr(5,2),16);updateStatus('neopixel-status','RGB('+r+','+g+','+b+')...',null);fetch('/api/neopixel-color?r='+r+'&g='+g+'&b='+b).then(function(rsp){return rsp.json();}).then(function(d){updateStatus('neopixel-status',d.message,d.success?'success':'error');}).catch(function(e){updateStatus('neopixel-status','Erreur: '+e,'error');});}";

  chunk += "function testOLED(){updateStatus('oled-status','Test en cours (25s)...',null);fetch('/api/oled-test').then(function(r){return r.json();}).then(function(d){updateStatus('oled-status',d.result,d.success?'success':'error');}).catch(function(e){updateStatus('oled-status','Erreur: '+e,'error');});}";
  chunk += "function oledStep(step){updateStatus('oled-status','" + String(T().testing) + "',null);fetch('/api/oled-step?step='+step).then(function(r){return r.json();}).then(function(d){updateStatus('oled-status',d.message,d.success?'success':'error');}).catch(function(e){updateStatus('oled-status','Erreur: '+e,'error');});}";
  chunk += "function oledMessage(){var msg=document.getElementById('oledMsg').value;if(!msg){updateStatus('oled-status','" + String(T().custom_message) + "','error');return;}updateStatus('oled-status','Transmission...',null);fetch('/api/oled-message?message='+encodeURIComponent(msg)).then(function(r){return r.json();}).then(function(d){updateStatus('oled-status',d.message,d.success?'success':'error');}).catch(function(e){updateStatus('oled-status','Erreur: '+e,'error');});}";
  chunk += "function configOLED(){updateStatus('oled-status','Reconfiguration...',null);var params='sda='+document.getElementById('oledSDA').value+'&scl='+document.getElementById('oledSCL').value+'&rotation='+document.getElementById('oledRotation').value;fetch('/api/oled-config?'+params).then(function(r){return r.json();}).then(function(d){var state=d.success?'success':'error';updateStatus('oled-status',d.message||'Configuration invalide',state);if(d.success&&typeof d.sda!=='undefined'){document.getElementById('oledSDA').value=d.sda;}if(d.success){var pins=document.getElementById('oled-pins');if(pins){pins.textContent='SDA:'+d.sda+' SCL:'+d.scl;}}if(d.success){var rotDisplay=document.getElementById('oled-rotation-display');if(rotDisplay){rotDisplay.textContent=d.rotation;}}}).catch(function(e){updateStatus('oled-status','Erreur: '+e,'error');});}";

  chunk += "function testADC(){document.getElementById('adc-status').innerHTML='Test...';fetch('/api/adc-test').then(function(r){return r.json();}).then(function(data){var h='';if(data.readings&&data.readings.forEach){data.readings.forEach(function(a){h+='<div class=\"info-item\"><div class=\"info-label\">GPIO '+a.pin+'</div><div class=\"info-value\">'+a.raw+' ('+a.voltage.toFixed(2)+'V)</div></div>';});}document.getElementById('adc-results').innerHTML=h;document.getElementById('adc-status').innerHTML=data.result;});}";
  chunk += "function testTouch(){document.getElementById('touch-status').innerHTML='Test...';fetch('/api/touch-test').then(function(r){return r.json();}).then(function(data){var h='';if(data.readings&&data.readings.forEach){data.readings.forEach(function(t){h+='<div class=\"info-item\"><div class=\"info-label\">Touch'+t.pin+'</div><div class=\"info-value\">'+t.value+'</div></div>';});}document.getElementById('touch-results').innerHTML=h;document.getElementById('touch-status').innerHTML=data.result;});}";
  chunk += "function testPWM(){document.getElementById('pwm-status').innerHTML='Test...';fetch('/api/pwm-test').then(function(r){return r.json();}).then(function(d){document.getElementById('pwm-status').innerHTML=d.result;});}";
  chunk += "function scanSPI(){document.getElementById('spi-status').innerHTML='Scan...';fetch('/api/spi-scan').then(function(r){return r.json();}).then(function(d){document.getElementById('spi-status').innerHTML=d.info;});}";
  chunk += "function listPartitions(){document.getElementById('partitions-results').innerHTML='Scan...';fetch('/api/partitions-list').then(function(r){return r.json();}).then(function(d){document.getElementById('partitions-results').innerHTML=d.partitions;});}";
  chunk += "function stressTest(){document.getElementById('stress-status').innerHTML='Test...';fetch('/api/stress-test').then(function(r){return r.json();}).then(function(d){document.getElementById('stress-status').innerHTML=d.result;});}";

  chunk += "function testAllGPIO(){document.getElementById('gpio-status').innerHTML='Test...';fetch('/api/test-gpio').then(function(r){return r.json();}).then(function(data){var h='';if(data.results&&data.results.forEach){data.results.forEach(function(g){h+='<div class=\"gpio-item '+(g.working?'gpio-ok':'gpio-fail')+'\">GPIO '+g.pin+'<br>'+(g.working?'OK':'FAIL')+'</div>';});}document.getElementById('gpio-results').innerHTML=h;document.getElementById('gpio-status').innerHTML='Termine - '+data.results.length+' GPIO testes';});}";

  chunk += "function scanWiFi(){document.getElementById('wifi-status').innerHTML='Scan...';fetch('/api/wifi-scan').then(function(r){return r.json();}).then(function(data){var h='';if(data.networks&&data.networks.forEach){data.networks.forEach(function(n){var s=n.rssi>=-60?'🟢':(n.rssi>=-70?'🟡':'🔴');var freq=(n.freq&&n.freq>0)?n.freq+' MHz':'';var pieces=[];if(n.bssid){pieces.push(n.bssid);}if(n.channel){pieces.push('Ch'+n.channel);}if(n.band){pieces.push(n.band);}if(freq){pieces.push(freq);}if(n.bandwidth){pieces.push(n.bandwidth);}if(n.phy){pieces.push(n.phy);}if(n.encryption&&n.encryption!=='-'){pieces.push(n.encryption);}var details=pieces.join(' | ');h+='<div class=\"wifi-item\"><div style=\"display:flex;justify-content:space-between\"><div><strong>'+s+' '+n.ssid+'</strong><br><small>'+details+'</small></div><div style=\"font-size:1.2em;font-weight:bold\">'+n.rssi+' dBm</div></div></div>';});}document.getElementById('wifi-results').innerHTML=h;document.getElementById('wifi-status').innerHTML=data.networks.length+' reseaux detectes';});}";

  chunk += "function scanI2C(){updateStatus('i2c-status','Scan...',null);fetch('/api/i2c-scan').then(function(r){return r.json();}).then(function(d){var msg='I2C: '+d.count+' peripherique(s)';updateStatus('i2c-status',msg,'success');var summary=document.getElementById('i2c-summary');if(summary){summary.textContent=d.count+' peripherique(s) - '+d.devices;}}).catch(function(e){updateStatus('i2c-status','Erreur: '+e,'error');});}";

  chunk += "function runBenchmarks(){document.getElementById('cpu-bench').innerHTML='Test...';document.getElementById('mem-bench').innerHTML='Test...';fetch('/api/benchmark').then(function(r){return r.json();}).then(function(data){document.getElementById('cpu-bench').innerHTML=data.cpu+' us';document.getElementById('mem-bench').innerHTML=data.memory+' us';document.getElementById('cpu-perf').innerHTML=data.cpuPerf.toFixed(2)+' MFLOPS';document.getElementById('mem-speed').innerHTML=data.memSpeed.toFixed(2)+' MB/s';});}";

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
  // --- [NEW FEATURE] Préconfiguration WiFi pour le core 3.3.2 ---
  WiFi.mode(WIFI_STA);
  WiFi.persistent(false);
  WiFi.setHostname(MDNS_HOSTNAME_STR);
  WiFi.setSleep(false);
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
  
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\r\n\r\nWiFi OK!");
    Serial.printf("SSID: %s\r\n", WiFi.SSID().c_str());
    Serial.printf("IP: %s\r\n\r\n", WiFi.localIP().toString().c_str());
    
    if (MDNS.begin(MDNS_HOSTNAME)) {
      Serial.println("════════════════════════════════════════");
      Serial.printf("   http://%s.local\r\n", MDNS_HOSTNAME);
      Serial.printf("   http://%s\r\n", WiFi.localIP().toString().c_str());
      Serial.println("════════════════════════════════════════\r\n");
      MDNS.addService("http", "tcp", 80);
    } else {
      Serial.println("mDNS erreur");
      Serial.printf("Utilisez IP: http://%s\r\n\r\n", WiFi.localIP().toString().c_str());
    }
  } else {
    Serial.println("\r\n\r\nPas de WiFi\r\n");
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
  
  // ========== ROUTES SERVEUR ==========
  server.on("/", handleRoot);
  
  // **NOUVELLES ROUTES MULTILINGUES**
  server.on("/api/set-language", handleSetLanguage);
  server.on("/api/get-translations", handleGetTranslations);
  
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
  server.on("/api/touch-test", handleTouchTest);
  server.on("/api/pwm-test", handlePWMTest);
  server.on("/api/spi-scan", handleSPIScan);
  server.on("/api/partitions-list", handlePartitionsList);
  server.on("/api/stress-test", handleStressTest);
  
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
  
  delay(10);
}
