/*
 * DIAGNOSTIC COMPLET ESP32 - VERSION MULTILINGUE v4.0.14
 * Compatible: ESP32, ESP32-S2, ESP32-S3, ESP32-C3
 * Optimisé pour ESP32 Arduino Core 3.3.2
 * Carte testée: ESP32-S3 avec PSRAM OPI
 * Auteur: morfredus
 *
 * Nouveautés v4.0.14:
 * - Préfère automatiquement BLEDevice.h ou NimBLEDevice.h selon la pile disponible pour activer le scan BLE natif
 * - Expose la bibliothèque BLE utilisée (ou manquante) dans l'API Sans fil, l'interface web, le moniteur série et les exports
 * - Affiche la carte Bluetooth même hors support avec un libellé de pile clair et des conseils localisés

 * Nouveautés v4.0.13:
 * - Ajoute un panneau Bluetooth détaillé dans l'onglet Sans fil avec messages explicites même hors support BLE
 * - Expose l'IP, le masque, la passerelle et le DNS Wi-Fi dans l'API, l'interface web et le moniteur série
 * - Met à jour la documentation et l'interface pour refléter la version 4.0.13

 * Nouveautés v4.0.12:
 * - Corrige la génération JSON du statut Sans fil pour que la carte Bluetooth reste visible même hors connexion
 * - Fiabilise les exports JSON WiFi/BLE lorsque les textes contiennent des caractères spéciaux
 * - Met à jour les références documentaires et l'interface vers la version 4.0.12
 *
 * Nouveautés v4.0.11:
 * - Assure l'affichage permanent de la fiche Bluetooth avec messages explicites même sans pile BLE active
 * - Ajoute un récapitulatif Sans fil (WiFi + BLE) dans le moniteur série, les exports TXT/JSON/CSV et la version imprimable
 * - Désactive proprement le scan BLE côté interface et bouton quand le firmware ou l'IDE ne propose pas la pile native
 *
 * Nouveautés v4.0.6:
 * - Rend tous les tests OLED (complet, message, motifs) accessibles même avant détection automatique
 * - Affiche un avertissement GPIO pour rappeler qu'un FAIL peut signaler un GPIO réservé plutôt qu'une panne
 * - Modernise l'interface web dynamique avec rappels locaux et messages OLED sans rechargement
 *
 * Nouveautés v4.0.5:
 * - Retire les derniers éléments liés au TFT et simplifie l'onglet écrans
 * - Remplace les popups bloquantes par des notifications proches des contrôles
 * - Rafraîchit la documentation et la version avec les changements UX

 * Nouveautés v4.0.4:
 * - Désactive le test de LED intégrée lorsque le GPIO est partagé avec la NeoPixel
 * - Localise intégralement les réponses API LED/OLED et harmonise les statuts
 * - Ajoute les raccourcis OLED à l'interface web historique sans rechargement

 * Nouveautés v4.0.2:
 * - Corrige la génération du catalogue de traductions pour compatibilité Arduino Core 3.3.2
 *
 * Nouveautés v4.0.1:
 * - Corrige les appels API TFT pour refléter l'état désactivé sans erreur 400
 *
 * Nouveautés v4.0.0:
 * - Support matériel TFT retiré (API web conservée)
 * - Scanner WiFi enrichi (toutes les méthodes d'authentification)
 * - Calcul MFLOPS du benchmark CPU corrigé
 * - Statuts mémoire/I2C désormais entièrement traduits
 *
 * Nouveautés v3.0.0:
 * - Compatibilité validée avec ESP32 Arduino Core 3.3.2
 * - Mise à jour complète de la documentation FR/EN
 *
 * Nouveautés v2.5:
 * - Traducion des exports (Français/Anglais)
 * 
 * Nouveautés v2.4:
 * - Interface multilingue (Français/Anglais)
 * - Changement de langue dynamique sans rechargement
 * - Toutes fonctionnalités v2.3 préservées
 */

#include <WiFi.h>
#include <WiFiMulti.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <esp_chip_info.h>
#include <esp_mac.h>
#include <esp_system.h>
#include <esp_flash.h>
#include <esp_heap_caps.h>
#include <esp_partition.h>
#include <soc/soc_caps.h>
#include <Wire.h>
#include <Adafruit_NeoPixel.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#if defined(SOC_BLE_SUPPORTED) && SOC_BLE_SUPPORTED
  #if defined(__has_include)
    #if __has_include(<esp_gap_ble_api.h>) && __has_include(<BLEDevice.h>)
      #include <BLEDevice.h>
      #define BLE_BACKEND_ARDUINO 1
    #elif __has_include(<NimBLEDevice.h>)
      #include <NimBLEDevice.h>
      #define BLE_BACKEND_NIMBLE 1
    #else
      #define BLE_BACKEND_NONE 1
    #endif
  #else
    #include <BLEDevice.h>
    #define BLE_BACKEND_ARDUINO 1
  #endif
#else
  #define BLE_BACKEND_NONE 1
#endif

#if defined(BLE_BACKEND_ARDUINO) || defined(BLE_BACKEND_NIMBLE)
  #define HAS_NATIVE_BLE 1
#else
  #define HAS_NATIVE_BLE 0
#endif
#include <cmath>
#include <vector>
#include <string>

// Configuration WiFi
#include "config.h"

// Système de traduction
#include "languages.h"

// ========== CONFIGURATION ==========
#define DIAGNOSTIC_VERSION "4.0.14"
#define CUSTOM_LED_PIN -1
#define CUSTOM_LED_COUNT 1
#define ENABLE_I2C_SCAN true
#define MDNS_HOSTNAME "esp32-diagnostic"

const char* DIAGNOSTIC_VERSION_STR = DIAGNOSTIC_VERSION;
const char* MDNS_HOSTNAME_STR = MDNS_HOSTNAME;

// Pins I2C pour OLED (modifiables via web)
int I2C_SCL = 20;
int I2C_SDA = 21;

// OLED 0.96" I2C
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C

// ========== OBJETS GLOBAUX ==========
WebServer server(80);
WiFiMulti wifiMulti;
Adafruit_SSD1306 oled(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// NeoPixel
int LED_PIN = CUSTOM_LED_PIN;
int LED_COUNT = CUSTOM_LED_COUNT;
Adafruit_NeoPixel *strip = nullptr;

bool neopixelTested = false;
bool neopixelAvailable = false;
bool neopixelSupported = false;
String neopixelTestResult = "";
int adcChannelsTested = 0;
int touchPadsDetected = 0;
int pwmLastPin = -1;
int stressMaxAllocs = -1;

// LED intégrée
int BUILTIN_LED_PIN = -1;
bool builtinLedTested = false;
bool builtinLedAvailable = false;
String builtinLedTestResult = "";

// OLED
bool oledTested = false;
bool oledAvailable = false;
String oledTestResult = "";
bool oledI2cProbeDetected = false;

// Tests additionnels
String adcTestResult = "";
String touchTestResult = "";
String pwmTestResult = "";
String partitionsInfo = "";
String spiInfo = "";
String stressTestResult = "";

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
  bool bleChipCapable;
  bool bleStackAvailable;
  String bleStatusMessage;
  String bleHint;
  String bleBackend;
  String wifiSSID;
  int wifiRSSI;
  String ipAddress;
  
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
  bool psramSupported;
  String psramMode;
  String psramModeLabel;
  String psramStatusMessage;
  String psramRecommendation;

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
};

std::vector<WiFiNetwork> wifiNetworks;

struct BLEDeviceInfo {
  String name;
  String address;
  int rssi;
};

std::vector<BLEDeviceInfo> bleDevices;

#if HAS_NATIVE_BLE
  #if defined(BLE_BACKEND_ARDUINO)
BLEScan* bleScanner = nullptr;
  #elif defined(BLE_BACKEND_NIMBLE)
NimBLEScan* bleScanner = nullptr;
  #endif
#endif
bool bleInitialized = false;

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

constexpr unsigned long CPU_BENCH_ITERATIONS = 100000;
constexpr unsigned long CPU_BENCH_FLOPS_PER_ITERATION = 2;

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

double calculateCpuMflops(unsigned long durationMicros) {
  if (durationMicros == 0) {
    return 0.0;
  }

  const double seconds = durationMicros / 1000000.0;
  const double operations = static_cast<double>(CPU_BENCH_ITERATIONS) *
                            static_cast<double>(CPU_BENCH_FLOPS_PER_ITERATION);
  return (operations / seconds) / 1000000.0;
}

void refreshLocalizedStatusMessages() {
  if (BUILTIN_LED_PIN == -1) {
    builtinLedTestResult = String(T().led_not_configured);
  } else if (!builtinLedTested) {
    builtinLedTestResult = String(T().led_ready) + " - GPIO " + String(BUILTIN_LED_PIN);
  } else if (!builtinLedAvailable) {
    builtinLedTestResult = String(T().led_conflict) + " - GPIO " + String(BUILTIN_LED_PIN);
  } else {
    builtinLedTestResult = String(T().led_test_ok) + " - GPIO " + String(BUILTIN_LED_PIN);
  }

  if (LED_PIN >= 0 && !neopixelTested) {
    neopixelTestResult = String(T().led_ready) + " - GPIO " + String(LED_PIN);
  } else if (LED_PIN < 0) {
    neopixelTestResult = String(T().not_detected);
  }

  if (!oledAvailable) {
    if (oledI2cProbeDetected) {
      oledTestResult = String(T().oled_init_failed);
    } else {
      oledTestResult = String(T().no_detected) + " (SDA:" + String(I2C_SDA) + " SCL:" + String(I2C_SCL) + ")";
    }
  } else if (!oledTested) {
    oledTestResult = String(T().detected) + " 0x" + String(SCREEN_ADDRESS, HEX);
  }

  if (adcChannelsTested > 0) {
    adcTestResult = String(adcChannelsTested) + " " + String(T().tested) + " - " + String(T().ok);
  } else {
    adcTestResult = String(T().not_tested);
  }

  if (touchPadsDetected < 0) {
    touchTestResult = String(T().not_supported);
  } else if (touchPadsDetected > 0) {
    touchTestResult = String(touchPadsDetected) + " " + String(T().tested) + " - " + String(T().ok);
  } else {
    touchTestResult = String(T().not_tested);
  }

  if (pwmLastPin >= 0) {
    pwmTestResult = String(T().pwm_test) + " - " + String(T().completed) + " (GPIO " + String(pwmLastPin) + ")";
  } else {
    pwmTestResult = String(T().not_tested);
  }

  if (stressMaxAllocs >= 0) {
    stressTestResult = String(T().max_allocated) + ": " + String(stressMaxAllocs) + " KB";
  } else {
    stressTestResult = String(T().not_tested);
  }
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
  refreshLocalizedStatusMessages();
  uint32_t flashSizeReal;
  esp_flash_get_size(NULL, &flashSizeReal);

  detailedMemory.flashSizeChip = flashSizeReal;
  detailedMemory.flashSizeReal = ESP.getFlashChipSize();

  detailedMemory.psramTotal = ESP.getPsramSize();
  detailedMemory.psramAvailable = (detailedMemory.psramTotal > 0);
  detailedMemory.psramConfigured = detailedMemory.psramAvailable;

  bool psramFlagDetected = false;
  String psramModeShort = "";

  #if defined(CONFIG_SPIRAM_MODE_OCT)
    psramFlagDetected = true;
    psramModeShort = "OPI";
  #endif
  #if defined(CONFIG_SPIRAM_MODE_QUAD)
    psramFlagDetected = true;
    if (psramModeShort.length() == 0) psramModeShort = "QSPI";
  #endif
  #if defined(CONFIG_SPIRAM)
    psramFlagDetected = true;
  #endif
  #if defined(CONFIG_SPIRAM_SUPPORT)
    psramFlagDetected = true;
  #endif
  #if defined(BOARD_HAS_PSRAM)
    psramFlagDetected = true;
  #endif

  detailedMemory.psramSupported = psramFlagDetected;
  detailedMemory.psramMode = psramModeShort;

  String psramModeLabel;
  if (psramModeShort == "OPI") {
    psramModeLabel = String(T().psram_mode_opi);
  } else if (psramModeShort == "QSPI") {
    psramModeLabel = String(T().psram_mode_qspi);
  } else if (psramFlagDetected) {
    psramModeLabel = String(T().psram_mode_unknown);
  } else {
    psramModeLabel = String(T().none);
  }
  detailedMemory.psramModeLabel = psramModeLabel;

  if (detailedMemory.psramAvailable) {
    detailedMemory.psramFree = ESP.getFreePsram();
    detailedMemory.psramUsed = detailedMemory.psramTotal - detailedMemory.psramFree;

    multi_heap_info_t info;
    heap_caps_get_info(&info, MALLOC_CAP_SPIRAM);
    detailedMemory.psramLargestBlock = info.largest_free_block;
    detailedMemory.psramStatusMessage = String(T().psram_status_active);
    detailedMemory.psramStatusMessage.replace("%MODE%", psramModeLabel);
    detailedMemory.psramRecommendation = "";
  } else {
    detailedMemory.psramFree = 0;
    detailedMemory.psramUsed = 0;
    detailedMemory.psramLargestBlock = 0;
    if (psramFlagDetected) {
      detailedMemory.psramStatusMessage = String(T().psram_status_disabled);
      detailedMemory.psramStatusMessage.replace("%MODE%", psramModeLabel);
      detailedMemory.psramRecommendation = String(T().psram_enable_hint);
    } else {
      detailedMemory.psramStatusMessage = String(T().psram_status_not_supported);
      detailedMemory.psramRecommendation = "";
    }
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
    detailedMemory.memoryStatus = T().warning;
  } else {
    detailedMemory.memoryStatus = T().critical;
  }
}

void printPSRAMDiagnostic() {
  collectDetailedMemory();
  Serial.println("\r\n=== DIAGNOSTIC PSRAM DETAILLE ===");
  Serial.printf("ESP.getPsramSize(): %u octets (%.2f MB)\r\n",
                ESP.getPsramSize(), ESP.getPsramSize() / 1048576.0);
  
  Serial.println("\r\nFlags de compilation detectes (indication du type supporte par la carte):");
  bool anyFlag = false;
  String psramType = "Inconnu";
  
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
  Serial.print("  → ");
  Serial.println(detailedMemory.psramStatusMessage);
  if (detailedMemory.psramRecommendation.length() > 0) {
    Serial.print("  → ");
    Serial.println(detailedMemory.psramRecommendation);
  }
  Serial.println("=====================================\r\n");
}

void printWirelessDiagnostic() {
  Serial.println("\r\n=== STATUT SANS FIL ===");

  bool wifiConnected = WiFi.status() == WL_CONNECTED;
  if (wifiConnected) {
    Serial.printf("WiFi: CONNECTE (%s) RSSI %d dBm\r\n",
                  diagnosticData.wifiSSID.c_str(),
                  diagnosticData.wifiRSSI);
    Serial.printf("Qualite: %s\r\n", getWiFiSignalQuality().c_str());
    Serial.printf("IP: %s\r\n", WiFi.localIP().toString().c_str());
    Serial.printf("Masque: %s\r\n", WiFi.subnetMask().toString().c_str());
    Serial.printf("Passerelle: %s\r\n", WiFi.gatewayIP().toString().c_str());
    Serial.printf("DNS: %s\r\n\r\n", WiFi.dnsIP().toString().c_str());
  } else {
    Serial.println("WiFi: Non connecte");
  }

  Serial.printf("BLE - Compatibilite puce: %s\r\n",
                diagnosticData.bleChipCapable ? "OUI" : "NON");
  Serial.printf("BLE - Pile firmware: %s\r\n",
                diagnosticData.bleStackAvailable ? "OUI" : "NON");
  Serial.printf("BLE - Fonctions actives: %s\r\n",
                diagnosticData.hasBLE ? "OUI" : "NON");
  Serial.printf("BLE - %s: %s\r\n",
                T().ble_backend_label,
                diagnosticData.bleBackend.c_str());
  Serial.printf("BLE - Statut: %s\r\n", diagnosticData.bleStatusMessage.c_str());
  if (diagnosticData.bleHint.length() > 0) {
    Serial.printf("Conseil: %s\r\n", diagnosticData.bleHint.c_str());
  }
  Serial.println("=====================================\r\n");
}

// ========== BENCHMARKS ==========
unsigned long benchmarkCPU() {
  Serial.println("\r\n=== BENCHMARK CPU ===");
  unsigned long start = micros();
  
  volatile float result = 0;
  for (unsigned long i = 0; i < CPU_BENCH_ITERATIONS; i++) {
    result += sqrt(i) * sin(i);
  }

  unsigned long duration = micros() - start;
  Serial.printf("CPU: %lu us (%.2f MFLOPS)\r\n", duration, calculateCpuMflops(duration));
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
void scanI2C() {
  if (!ENABLE_I2C_SCAN) return;
  
  Serial.println("\r\n=== SCAN I2C ===");
  Wire.begin(I2C_SDA, I2C_SCL);
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
    diagnosticData.i2cDevices = String(T().none);
  }
  Serial.printf("I2C: %d peripherique(s)\r\n", diagnosticData.i2cCount);
}

// ========== SCAN WIFI ==========
String wifiAuthModeToString(wifi_auth_mode_t auth) {
  switch (auth) {
    case WIFI_AUTH_OPEN: return "Open";
    case WIFI_AUTH_WEP: return "WEP";
    case WIFI_AUTH_WPA_PSK: return "WPA-PSK";
    case WIFI_AUTH_WPA2_PSK: return "WPA2-PSK";
    case WIFI_AUTH_WPA_WPA2_PSK: return "WPA/WPA2-PSK";
    case WIFI_AUTH_WPA2_ENTERPRISE: return "WPA2-Enterprise";
    case WIFI_AUTH_WPA3_PSK: return "WPA3-PSK";
    case WIFI_AUTH_WPA2_WPA3_PSK: return "WPA2/WPA3-PSK";
    case WIFI_AUTH_WAPI_PSK: return "WAPI-PSK";
    case WIFI_AUTH_OWE: return "OWE";
    default: return String(T().unknown);
  }
}

void scanWiFiNetworks() {
  Serial.println("\r\n=== SCAN WIFI ===");
  wifiNetworks.clear();

  int n = WiFi.scanNetworks();
  if (n < 0) {
    Serial.printf("WiFi: scan error (%d)\r\n", n);
    return;
  }

  for (int i = 0; i < n; i++) {
    WiFiNetwork net;
    net.ssid = WiFi.SSID(i);
    net.rssi = WiFi.RSSI(i);
    net.channel = WiFi.channel(i);

    uint8_t* bssid = WiFi.BSSID(i);
    char bssidStr[18];
    sprintf(bssidStr, "%02X:%02X:%02X:%02X:%02X:%02X",
            bssid[0], bssid[1], bssid[2], bssid[3], bssid[4], bssid[5]);
    net.bssid = String(bssidStr);

    wifi_auth_mode_t auth = WiFi.encryptionType(i);
    net.encryption = wifiAuthModeToString(auth);

    wifiNetworks.push_back(net);
  }
  Serial.printf("WiFi: %d reseaux trouves\r\n", n);
}

#if HAS_NATIVE_BLE
void ensureBleScanner() {
  if (bleInitialized) return;

  #if defined(BLE_BACKEND_ARDUINO)
    BLEDevice::init("ESP32 Diagnostic");
    bleScanner = BLEDevice::getScan();
    if (bleScanner != nullptr) {
      bleScanner->setActiveScan(true);
      bleScanner->setInterval(100);
      bleScanner->setWindow(80);
    }
  #elif defined(BLE_BACKEND_NIMBLE)
    NimBLEDevice::init("ESP32 Diagnostic");
    bleScanner = NimBLEDevice::getScan();
    if (bleScanner != nullptr) {
      bleScanner->setActiveScan(true);
      bleScanner->setInterval(45);
      bleScanner->setWindow(30);
    }
  #endif

  bleInitialized = true;
}

void scanBLEDevices() {
  Serial.println("\r\n=== SCAN BLE ===");
  bleDevices.clear();

  if (!diagnosticData.hasBLE) {
    Serial.println("BLE: not supported on this board");
    return;
  }

  ensureBleScanner();
  if (bleScanner == nullptr) {
    Serial.println("BLE: scanner unavailable");
    return;
  }

  #if defined(BLE_BACKEND_ARDUINO)
    BLEScanResults results = bleScanner->start(5, false);
    int count = results.getCount();
    for (int i = 0; i < count; i++) {
      BLEAdvertisedDevice device = results.getDevice(i);
      BLEDeviceInfo info;
      info.name = device.haveName() ? String(device.getName().c_str()) : String(T().unknown);
      info.address = String(device.getAddress().toString().c_str());
      info.rssi = device.getRSSI();
      bleDevices.push_back(info);
    }
    bleScanner->clearResults();
    Serial.printf("BLE: %d devices found\r\n", count);
  #elif defined(BLE_BACKEND_NIMBLE)
    NimBLEScanResults results = bleScanner->start(5, false);
    int count = results.getCount();
    for (int i = 0; i < count; i++) {
      NimBLEAdvertisedDevice device = results.getDevice(i);
      BLEDeviceInfo info;
      std::string deviceName = device.getName();
      bool hasName = deviceName.length() > 0;
      info.name = hasName ? String(deviceName.c_str()) : String(T().unknown);
      info.address = String(device.getAddress().toString().c_str());
      info.rssi = device.getRSSI();
      bleDevices.push_back(info);
    }
    bleScanner->clearResults();
    Serial.printf("BLE: %d devices found\r\n", count);
  #endif
}
#else
void ensureBleScanner() {}

void scanBLEDevices() {
  Serial.println("\r\n=== SCAN BLE (disabled) ===");
  bleDevices.clear();
  Serial.println("BLE: stack not available in this build");
}
#endif

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

  #if defined(LED_BUILTIN)
    BUILTIN_LED_PIN = LED_BUILTIN;
  #elif defined(BUILTIN_LED)
    BUILTIN_LED_PIN = BUILTIN_LED;
  #else
    if (chipModel == "ESP32-S3") BUILTIN_LED_PIN = 2;
    else if (chipModel == "ESP32-C3") BUILTIN_LED_PIN = 8;
    else if (chipModel == "ESP32-S2") BUILTIN_LED_PIN = 15;
    else BUILTIN_LED_PIN = 2;
  #endif

  builtinLedTested = false;
  if (BUILTIN_LED_PIN >= 0) {
    builtinLedAvailable = true;
    builtinLedTestResult = String(T().led_ready) + " - GPIO " + String(BUILTIN_LED_PIN);
  } else {
    builtinLedAvailable = false;
    builtinLedTestResult = String(T().not_detected);
  }
  Serial.printf("LED integree: GPIO %d\r\n", BUILTIN_LED_PIN);
}

bool builtinLedSharesNeoPixelPin() {
  return (BUILTIN_LED_PIN >= 0 && strip != nullptr && LED_PIN == BUILTIN_LED_PIN);
}

void builtinLedFadeSequence() {
  if (BUILTIN_LED_PIN == -1) return;

  ledcAttach(BUILTIN_LED_PIN, 5000, 8);
  for (int duty = 0; duty <= 255; duty += 5) {
    ledcWrite(BUILTIN_LED_PIN, duty);
    delay(10);
  }
  for (int duty = 255; duty >= 0; duty -= 5) {
    ledcWrite(BUILTIN_LED_PIN, duty);
    delay(10);
  }
  ledcWrite(BUILTIN_LED_PIN, 0);
  ledcDetach(BUILTIN_LED_PIN);
}

void testBuiltinLED() {
  if (BUILTIN_LED_PIN == -1) {
    builtinLedAvailable = false;
    builtinLedTestResult = String(T().not_detected);
    Serial.println("LED: GPIO invalide");
    return;
  }

  Serial.println("\r\n=== TEST LED ===");
  pinMode(BUILTIN_LED_PIN, OUTPUT);

  bool sharedWithNeoPixel = builtinLedSharesNeoPixelPin();
  if (sharedWithNeoPixel) {
    Serial.println("LED: test ignore (GPIO partage avec NeoPixel)");
    builtinLedAvailable = false;
    builtinLedTested = true;
    builtinLedTestResult = String(T().led_conflict) + " - GPIO " + String(BUILTIN_LED_PIN);
    return;
  }

  for (int i = 0; i < 5; i++) {
    digitalWrite(BUILTIN_LED_PIN, HIGH);
    delay(200);
    digitalWrite(BUILTIN_LED_PIN, LOW);
    delay(200);
  }

  builtinLedFadeSequence();

  digitalWrite(BUILTIN_LED_PIN, LOW);
  builtinLedAvailable = true;
  builtinLedTested = true;
  builtinLedTestResult = String(T().led_test_ok) + " - GPIO " + String(BUILTIN_LED_PIN);
  Serial.println("LED: OK");
}

void resetBuiltinLEDTest() {
  builtinLedTested = false;
  builtinLedAvailable = (BUILTIN_LED_PIN >= 0);
  if (BUILTIN_LED_PIN != -1) {
    digitalWrite(BUILTIN_LED_PIN, LOW);
    builtinLedTestResult = String(T().led_ready) + " - GPIO " + String(BUILTIN_LED_PIN);
  } else {
    builtinLedTestResult = String(T().not_detected);
  }
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
  neopixelTestResult = String(T().led_ready) + " - GPIO " + String(LED_PIN);
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
  neopixelTestResult = String(T().led_test_ok) + " - GPIO " + String(LED_PIN);
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
  if (LED_PIN >= 0) {
    neopixelTestResult = String(T().led_ready) + " - GPIO " + String(LED_PIN);
  } else {
    neopixelTestResult = String(T().not_detected);
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

// ========== OLED 0.96" ==========
void detectOLED() {
  Serial.println("\r\n=== DETECTION OLED ===");
  Wire.begin(I2C_SDA, I2C_SCL);
  Serial.printf("I2C: SDA=%d, SCL=%d\r\n", I2C_SDA, I2C_SCL);
  
  Wire.beginTransmission(SCREEN_ADDRESS);
  bool i2cDetected = (Wire.endTransmission() == 0);
  oledI2cProbeDetected = i2cDetected;
  
  if(i2cDetected && oled.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    oledAvailable = true;
    oledTestResult = String(T().detected) + " 0x" + String(SCREEN_ADDRESS, HEX);
    Serial.println("OLED: Detecte!\r\n");
  } else {
    oledAvailable = false;
    if (i2cDetected) {
      oledTestResult = String(T().oled_init_failed);
    } else {
      oledTestResult = String(T().no_detected) + " (SDA:" + String(I2C_SDA) + " SCL:" + String(I2C_SCL) + ")";
    }
    Serial.println("OLED: Non detecte\r\n");
  }
}

void oledIntroScreen() {
  if (!oledAvailable) return;
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

void oledLargeText() {
  if (!oledAvailable) return;
  oled.clearDisplay();
  oled.setTextSize(2);
  oled.setTextColor(SSD1306_WHITE);
  oled.setCursor(20, 20);
  oled.println("ESP32");
  oled.display();
  delay(1500);
}

void oledFontSizes() {
  if (!oledAvailable) return;
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

void oledShapes() {
  if (!oledAvailable) return;
  oled.clearDisplay();
  oled.drawRect(10, 10, 30, 20, SSD1306_WHITE);
  oled.fillRect(50, 10, 30, 20, SSD1306_WHITE);
  oled.drawCircle(25, 50, 10, SSD1306_WHITE);
  oled.fillCircle(65, 50, 10, SSD1306_WHITE);
  oled.drawTriangle(95, 30, 85, 10, 105, 10, SSD1306_WHITE);
  oled.display();
  delay(2000);
}

void oledHorizontalLines() {
  if (!oledAvailable) return;
  oled.clearDisplay();
  for (int i = 0; i < SCREEN_HEIGHT; i += 4) {
    oled.drawLine(0, i, SCREEN_WIDTH, i, SSD1306_WHITE);
  }
  oled.display();
  delay(1500);
}

void oledDiagonalLines() {
  if (!oledAvailable) return;
  oled.clearDisplay();
  for (int i = 0; i < SCREEN_WIDTH; i += 8) {
    oled.drawLine(0, 0, i, SCREEN_HEIGHT - 1, SSD1306_WHITE);
    oled.drawLine(SCREEN_WIDTH - 1, 0, i, SCREEN_HEIGHT - 1, SSD1306_WHITE);
  }
  oled.display();
  delay(1500);
}

void oledMovingSquare() {
  if (!oledAvailable) return;
  for (int x = 0; x < SCREEN_WIDTH - 20; x += 4) {
    oled.clearDisplay();
    oled.fillRect(x, 22, 20, 20, SSD1306_WHITE);
    oled.display();
    delay(20);
  }
  oled.clearDisplay();
  oled.display();
}

void oledProgressBar() {
  if (!oledAvailable) return;
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

void oledScrollingText() {
  if (!oledAvailable) return;
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

void oledFinalFrame() {
  if (!oledAvailable) return;
  oled.clearDisplay();
  oled.setTextSize(1);
  oled.setTextColor(SSD1306_WHITE);
  oled.setCursor(30, 20);
  oled.println(String(T().test) + " " + String(T().completed));
  oled.drawRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, SSD1306_WHITE);
  oled.display();
  delay(2000);
  oled.clearDisplay();
  oled.display();
}

struct OledPatternEntry {
  const char* id;
  void (*action)();
  const char* Translations::*label;
};

const OledPatternEntry OLED_PATTERNS[] = {
  {"intro", oledIntroScreen, &Translations::oled_intro},
  {"large", oledLargeText, &Translations::oled_large_text},
  {"fonts", oledFontSizes, &Translations::oled_fonts},
  {"shapes", oledShapes, &Translations::oled_shapes},
  {"lines", oledHorizontalLines, &Translations::oled_lines},
  {"diagonals", oledDiagonalLines, &Translations::oled_diagonals},
  {"square", oledMovingSquare, &Translations::oled_animation},
  {"progress", oledProgressBar, &Translations::oled_progress},
  {"scroll", oledScrollingText, &Translations::oled_scroll},
  {"final", oledFinalFrame, &Translations::oled_final},
};

String normalizePatternId(String pattern) {
  pattern.trim();
  pattern.toLowerCase();
  return pattern;
}

String getOledPatternLabel(const String& rawPattern) {
  String normalized = normalizePatternId(rawPattern);
  for (const auto& entry : OLED_PATTERNS) {
    if (normalized == entry.id) {
      return String(T().*entry.label);
    }
  }
  return normalized;
}

bool runOledPattern(const String& rawPattern) {
  if (!oledAvailable) return false;

  String normalized = normalizePatternId(rawPattern);
  for (const auto& entry : OLED_PATTERNS) {
    if (normalized == entry.id) {
      if (entry.action) {
        entry.action();
      }
      return true;
    }
  }
  return false;
}

void testOLED() {
  if (!oledAvailable) {
    oledTestResult = String(T().no_detected);
    return;
  }

  Serial.println("\r\n=== TEST OLED ===");

  oledTested = false;
  oledIntroScreen();
  oledLargeText();
  oledFontSizes();
  oledShapes();
  oledHorizontalLines();
  oledDiagonalLines();
  oledMovingSquare();
  oledProgressBar();
  oledScrollingText();
  oledFinalFrame();

  oledTested = true;
  oledTestResult = String(T().completed) + " - 128x64";
  Serial.println("OLED: Tests complets OK\r\n");
}

void resetOLEDTest() {
  oledTested = false;
  if (oledAvailable) {
    oled.clearDisplay();
    oled.display();
  }
  if (oledAvailable) {
    oledTestResult = String(T().detected) + " 0x" + String(SCREEN_ADDRESS, HEX);
  } else if (oledI2cProbeDetected) {
    oledTestResult = String(T().oled_init_failed);
  } else {
    oledTestResult = String(T().no_detected) + " (SDA:" + String(I2C_SDA) + " SCL:" + String(I2C_SCL) + ")";
  }
}

void oledShowMessage(String message) {
  if (!oledAvailable) return;
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
  
  adcChannelsTested = numADC;
  adcTestResult = String(adcChannelsTested) + " " + String(T().tested) + " - " + String(T().ok);
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
    touchPadsDetected = -1;
    touchTestResult = String(T().not_supported);
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
  
  touchPadsDetected = numTouch;
  touchTestResult = String(touchPadsDetected) + " " + String(T().tested) + " - " + String(T().ok);
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
  
  pwmLastPin = testPin;
  pwmTestResult = String(T().pwm_test) + " - " + String(T().completed) + " (GPIO " + String(pwmLastPin) + ")";
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
  
  stressMaxAllocs = maxAllocs;
  stressTestResult = String(T().max_allocated) + ": " + String(stressMaxAllocs) + " KB";
  Serial.println("Stress test: OK");
}

// ========== COLLECTE DONNÉES ==========
void collectDiagnosticInfo() {
  refreshLocalizedStatusMessages();
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
  bool bleFeatureBit = (chip_info.features & CHIP_FEATURE_BLE);
  diagnosticData.bleChipCapable = bleFeatureBit;
  diagnosticData.bleStackAvailable = HAS_NATIVE_BLE;
  diagnosticData.hasBLE = bleFeatureBit && HAS_NATIVE_BLE;
  #if defined(BLE_BACKEND_ARDUINO)
    diagnosticData.bleBackend = String(T().ble_backend_arduino);
  #elif defined(BLE_BACKEND_NIMBLE)
    diagnosticData.bleBackend = String(T().ble_backend_nimble);
  #else
    diagnosticData.bleBackend = String(T().ble_backend_missing);
  #endif

  if (diagnosticData.hasBLE) {
    diagnosticData.bleStatusMessage = String(T().ble_status_active);
    diagnosticData.bleHint = "";
  } else if (bleFeatureBit && !HAS_NATIVE_BLE) {
    diagnosticData.bleStatusMessage = String(T().ble_status_stack_missing);
    diagnosticData.bleHint = String(T().ble_enable_hint);
  } else {
    diagnosticData.bleStatusMessage = String(T().ble_status_not_supported);
    diagnosticData.bleHint = "";
  }

  if (WiFi.status() == WL_CONNECTED) {
    diagnosticData.wifiSSID = WiFi.SSID();
    diagnosticData.wifiRSSI = WiFi.RSSI();
    diagnosticData.ipAddress = WiFi.localIP().toString();
  } else {
    diagnosticData.wifiSSID = "";
    diagnosticData.wifiRSSI = -120;
    diagnosticData.ipAddress = "";
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
  
  heapHistory[historyIndex] = (float)diagnosticData.freeHeap / 1024.0;
  if (diagnosticData.temperature != -999) {
    tempHistory[historyIndex] = diagnosticData.temperature;
  }
  historyIndex = (historyIndex + 1) % HISTORY_SIZE;
}

// ========== HANDLERS API ==========
void handleOverview() {
  collectDiagnosticInfo();
  collectDetailedMemory();

  String json = "{";
  json += "\"chip\":{\"model\":\"" + jsonEscape(diagnosticData.chipModel) + "\",";
  json += "\"revision\":\"" + jsonEscape(diagnosticData.chipRevision) + "\",";
  json += "\"cores\":" + String(diagnosticData.cpuCores) + ",";
  json += "\"freq\":" + String(diagnosticData.cpuFreqMHz) + ",";
  json += "\"mac\":\"" + diagnosticData.macAddress + "\",";
  json += "\"uptime\":" + String(diagnosticData.uptime) + ",";
  json += "\"temperature\":" + String(diagnosticData.temperature, 1) + "},";

  json += "\"memory\":{";
  json += "\"flash\":{\"real\":" + String(detailedMemory.flashSizeReal) + ",\"chip\":" + String(detailedMemory.flashSizeChip) + ",\"type\":\"" + jsonEscape(getFlashType()) + "\"},";
  json += "\"fragmentation\":" + String(detailedMemory.fragmentationPercent, 1) + ",";
  json += "\"sram\":{\"total\":" + String(detailedMemory.sramTotal) + ",\"free\":" + String(detailedMemory.sramFree) + ",\"used\":" + String(detailedMemory.sramUsed) + "},";
  json += "\"psram\":{\"available\":" + String(detailedMemory.psramAvailable ? "true" : "false") + ",";
  json += "\"supported\":" + String(detailedMemory.psramSupported ? "true" : "false") + ",";
  json += "\"configured\":" + String(detailedMemory.psramConfigured ? "true" : "false") + ",";
  json += "\"total\":" + String(detailedMemory.psramTotal) + ",\"free\":" + String(detailedMemory.psramFree) + ",";
  json += "\"used\":" + String(detailedMemory.psramUsed) + ",\"mode\":\"" + jsonEscape(detailedMemory.psramMode) + "\",";
  json += "\"mode_label\":\"" + jsonEscape(detailedMemory.psramModeLabel) + "\",\"status\":\"" + jsonEscape(detailedMemory.psramStatusMessage) + "\"";
  if (detailedMemory.psramRecommendation.length() > 0) {
    json += ",\"hint\":\"" + jsonEscape(detailedMemory.psramRecommendation) + "\"";
  }
  json += "}},";

  json += "\"wifi\":{\"ssid\":\"" + jsonEscape(diagnosticData.wifiSSID) + "\",\"rssi\":" + String(diagnosticData.wifiRSSI) + ",\"quality\":\"" + jsonEscape(getWiFiSignalQuality()) + "\",\"ip\":\"" + jsonEscape(diagnosticData.ipAddress) + "\"},";

  json += "\"gpio\":{\"total\":" + String(diagnosticData.totalGPIO) + ",\"i2c_count\":" + String(diagnosticData.i2cCount) + ",\"i2c_devices\":\"" + jsonEscape(diagnosticData.i2cDevices) + "\"}";

  json += "}";

  server.send(200, "application/json", json);
}

void handleWirelessStatus() {
  collectDiagnosticInfo();

  String json = "{";
  bool wifiConnected = WiFi.status() == WL_CONNECTED;
  json += "\"wifi\":{\"connected\":" + String(wifiConnected ? "true" : "false") + ",";
  json += "\"ssid\":\"" + jsonEscape(diagnosticData.wifiSSID) + "\",\"rssi\":" + String(diagnosticData.wifiRSSI) + ",\"quality\":\"" + jsonEscape(getWiFiSignalQuality()) + "\",";
  json += "\"ip\":\"" + jsonEscape(diagnosticData.ipAddress) + "\",\"subnet\":\"" + jsonEscape(WiFi.subnetMask().toString()) + "\",";
  json += "\"gateway\":\"" + jsonEscape(WiFi.gatewayIP().toString()) + "\",\"dns\":\"" + jsonEscape(WiFi.dnsIP().toString()) + "\"}";

  json += ",\"ble\":{\"chipCapable\":" + String(diagnosticData.bleChipCapable ? "true" : "false") + ",";
  json += "\"stackAvailable\":" + String(diagnosticData.bleStackAvailable ? "true" : "false") + ",";
  json += "\"enabled\":" + String(diagnosticData.hasBLE ? "true" : "false") + ",\"status\":\"" + jsonEscape(diagnosticData.bleStatusMessage) + "\",";
  json += "\"backend\":\"" + jsonEscape(diagnosticData.bleBackend) + "\"";
  if (diagnosticData.bleHint.length() > 0) {
    json += ",\"hint\":\"" + jsonEscape(diagnosticData.bleHint) + "\"";
  }
  json += "}";

  json += "}";

  server.send(200, "application/json", json);
}

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
            "\",\"bssid\":\"" + wifiNetworks[i].bssid + "\"}";
  }
  json += "]}";
  server.send(200, "application/json", json);
}

void handleBLEScan() {
  String unsupportedJson = "{\"supported\":false,\"message\":\"" + jsonEscape(String(T().ble_not_supported)) + "\"}";

#if !HAS_NATIVE_BLE
  bleDevices.clear();
  server.send(200, "application/json", unsupportedJson);
  return;
#endif

  if (!diagnosticData.hasBLE) {
    server.send(200, "application/json", unsupportedJson);
    return;
  }

  scanBLEDevices();
  String json = "{\"supported\":true,\"devices\":[";
  for (size_t i = 0; i < bleDevices.size(); i++) {
    if (i > 0) json += ",";
    json += "{\"name\":\"" + jsonEscape(bleDevices[i].name) + "\",\"address\":\"" + bleDevices[i].address +
            "\",\"rssi\":" + String(bleDevices[i].rssi) + "}";
  }
  json += "],\"count\":" + String(bleDevices.size()) + "}";
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
      String msg = String(T().led_configured) + " " + String(BUILTIN_LED_PIN);
      server.send(200, "application/json", "{\"success\":true,\"message\":\"" + jsonEscape(msg) + "\"}");
      return;
    }
  }
  server.send(400, "application/json", "{\"success\":false,\"message\":\"" + jsonEscape(String(T().invalid_pins)) + "\"}");
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
    server.send(400, "application/json", "{\"success\":false,\"message\":\"" + jsonEscape(String(T().led_not_configured)) + "\"}");
    return;
  }

  pinMode(BUILTIN_LED_PIN, OUTPUT);
  String message = "";
  bool sharedWithNeoPixel = builtinLedSharesNeoPixelPin();

  if (action == "blink") {
    if (sharedWithNeoPixel) {
      server.send(409, "application/json", "{\"success\":false,\"message\":\"" + jsonEscape(String(T().led_test_skipped)) + "\"}");
      return;
    }
    for(int i = 0; i < 5; i++) {
      digitalWrite(BUILTIN_LED_PIN, HIGH);
      delay(200);
      digitalWrite(BUILTIN_LED_PIN, LOW);
      delay(200);
    }
    message = String(T().blink) + " - " + String(T().completed);
  } else if (action == "fade") {
    if (sharedWithNeoPixel) {
      server.send(409, "application/json", "{\"success\":false,\"message\":\"" + jsonEscape(String(T().led_test_skipped)) + "\"}");
      return;
    }
    builtinLedFadeSequence();
    digitalWrite(BUILTIN_LED_PIN, LOW);
    message = String(T().fade) + " - " + String(T().completed);
  } else if (action == "on") {
    digitalWrite(BUILTIN_LED_PIN, HIGH);
    builtinLedTested = true;
    builtinLedAvailable = true;
    message = String(T().led_on_state);
  } else if (action == "off") {
    digitalWrite(BUILTIN_LED_PIN, LOW);
    builtinLedTested = false;
    message = String(T().led_off_state);
  } else {
    server.send(400, "application/json", "{\"success\":false}");
    return;
  }

  server.send(200, "application/json", "{\"success\":true,\"message\":\"" + jsonEscape(message) + "\"}");
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
    server.send(400, "application/json", "{\"success\":false,\"message\":\"" + jsonEscape(String(T().pattern_missing)) + "\"}");
    return;
  }

  String pattern = server.arg("pattern");
  if (!strip) {
    server.send(400, "application/json", "{\"success\":false,\"message\":\"" + jsonEscape(String(T().neopixel_not_ready)) + "\"}");
    return;
  }

  String message = "";
  if (pattern == "rainbow") {
    neopixelRainbow();
    message = String(T().rainbow) + " - " + String(T().completed);
  } else if (pattern == "blink") {
    neopixelBlink(strip->Color(255, 0, 0), 5);
    message = String(T().blink) + " - " + String(T().completed);
  } else if (pattern == "fade") {
    neopixelFade(strip->Color(0, 0, 255));
    message = String(T().fade) + " - " + String(T().completed);
  } else if (pattern == "off") {
    strip->clear();
    strip->show();
    neopixelTested = false;
    message = String(T().off);
  } else {
    server.send(400, "application/json", "{\"success\":false,\"message\":\"" + jsonEscape(String(T().pattern_unknown)) + "\"}");
    return;
  }

  server.send(200, "application/json", "{\"success\":true,\"message\":\"" + jsonEscape(message) + "\"}");
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
  if (server.hasArg("sda") && server.hasArg("scl")) {
    int newSDA = server.arg("sda").toInt();
    int newSCL = server.arg("scl").toInt();

    if (newSDA >= 0 && newSDA <= 48 && newSCL >= 0 && newSCL <= 48) {
      I2C_SDA = newSDA;
      I2C_SCL = newSCL;
      resetOLEDTest();
      Wire.end();
      detectOLED();
      String msg = String(T().oled_reconfigured) + ": SDA " + String(I2C_SDA) + " SCL " + String(I2C_SCL);
      server.send(200, "application/json", "{\"success\":true,\"message\":\"" + jsonEscape(msg) + "\"}");
      return;
    }
  }
  server.send(400, "application/json", "{\"success\":false,\"message\":\"" + jsonEscape(String(T().invalid_pins)) + "\"}");
}

void handleOLEDTest() {
  resetOLEDTest();
  testOLED();
  server.send(200, "application/json", "{\"success\":" + String(oledAvailable ? "true" : "false") + ",\"result\":\"" + oledTestResult + "\"}");
}

void handleOLEDPattern() {
  if (!server.hasArg("pattern")) {
    server.send(400, "application/json", "{\"success\":false,\"message\":\"" + jsonEscape(String(T().pattern_missing)) + "\"}");
    return;
  }

  if (!oledAvailable) {
    String err = oledI2cProbeDetected ? String(T().oled_init_failed) : String(T().no_detected);
    server.send(400, "application/json", "{\"success\":false,\"message\":\"" + jsonEscape(err) + "\"}");
    return;
  }

  String pattern = server.arg("pattern");
  String normalizedPattern = normalizePatternId(pattern);
  if (!runOledPattern(normalizedPattern)) {
    server.send(400, "application/json", "{\"success\":false,\"message\":\"" + jsonEscape(String(T().pattern_unknown)) + "\"}");
    return;
  }

  String label = getOledPatternLabel(normalizedPattern);
  String message = label + " - " + String(T().completed);
  oledTestResult = message;
  oledTested = true;
  server.send(200, "application/json", "{\"success\":true,\"pattern\":\"" + jsonEscape(normalizedPattern) + "\",\"message\":\"" + jsonEscape(message) + "\"}");
}

void handleOLEDMessage() {
  if (!server.hasArg("message")) {
    server.send(400, "application/json", "{\"success\":false}");
    return;
  }

  String message = server.arg("message");
  if (!oledAvailable) {
    String err = oledI2cProbeDetected ? String(T().oled_init_failed) : String(T().no_detected);
    server.send(400, "application/json", "{\"success\":false,\"message\":\"" + jsonEscape(err) + "\"}");
    return;
  }

  oledShowMessage(message);
  server.send(200, "application/json", "{\"success\":true,\"message\":\"" + jsonEscape(String(T().message_displayed)) + "\"}");
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
              ",\"cpuPerf\":" + String(calculateCpuMflops(cpuTime), 2) + ",\"memSpeed\":" + String((10000 * sizeof(int) * 2) / (float)memTime, 2) + "}");
}

void handleMemoryDetails() {
  collectDetailedMemory();
  
  String json = "{\"flash\":{\"real\":" + String(detailedMemory.flashSizeReal) + ",\"chip\":" + String(detailedMemory.flashSizeChip) + "},";
  json += "\"psram\":{\"available\":" + String(detailedMemory.psramAvailable ? "true" : "false") +
          ",\"supported\":" + String(detailedMemory.psramSupported ? "true" : "false") +
          ",\"configured\":" + String(detailedMemory.psramConfigured ? "true" : "false") +
          ",\"total\":" + String(detailedMemory.psramTotal) + ",\"free\":" + String(detailedMemory.psramFree) +
          ",\"used\":" + String(detailedMemory.psramUsed) + ",\"mode\":\"" + jsonEscape(detailedMemory.psramMode) +
          "\",\"mode_label\":\"" + jsonEscape(detailedMemory.psramModeLabel) +
          "\",\"status\":\"" + jsonEscape(detailedMemory.psramStatusMessage) + "\"";
  if (detailedMemory.psramRecommendation.length() > 0) {
    json += ",\"hint\":\"" + jsonEscape(detailedMemory.psramRecommendation) + "\"";
  }
  json += "},";
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
  txt += String(T().ble_chip_support) + ": " + String(diagnosticData.bleChipCapable ? T().status_yes : T().status_no) + "\r\n";
  txt += String(T().ble_stack_support) + ": " + String(diagnosticData.bleStackAvailable ? T().status_yes : T().status_missing) + "\r\n";
  txt += String(T().ble_runtime_status) + ": " + String(diagnosticData.hasBLE ? T().status_yes : T().status_missing) + "\r\n";
  txt += String(T().ble_backend_label) + ": " + diagnosticData.bleBackend + "\r\n";
  txt += String(T().status) + ": " + diagnosticData.bleStatusMessage + "\r\n";
  if (diagnosticData.bleHint.length() > 0) {
    txt += String(T().recommendation) + ": " + diagnosticData.bleHint + "\r\n";
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
  txt += "CPU: " + String(diagnosticData.cpuBenchmark) + " us (" + String(calculateCpuMflops(diagnosticData.cpuBenchmark), 2) + " MFLOPS)\r\n";
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
  json += "\"sram_kb\":" + String(detailedMemory.sramTotal / 1024.0, 2) + ",";
  json += "\"sram_free_kb\":" + String(detailedMemory.sramFree / 1024.0, 2) + ",";
  json += "\"fragmentation\":" + String(detailedMemory.fragmentationPercent, 1) + ",";
  json += "\"status\":\"" + detailedMemory.memoryStatus + "\"";
  json += "},";
  
  bool wifiConnected = WiFi.status() == WL_CONNECTED;
  json += "\"wifi\":{";
  json += "\"connected\":" + String(wifiConnected ? "true" : "false") + ",";
  json += "\"ssid\":\"" + jsonEscape(diagnosticData.wifiSSID) + "\",";
  json += "\"rssi\":" + String(diagnosticData.wifiRSSI) + ",";
  json += "\"quality\":\"" + jsonEscape(getWiFiSignalQuality()) + "\",";
  json += "\"ip\":\"" + diagnosticData.ipAddress + "\",";
  json += "\"subnet\":\"" + WiFi.subnetMask().toString() + "\",";
  json += "\"gateway\":\"" + WiFi.gatewayIP().toString() + "\",";
  json += "\"dns\":\"" + WiFi.dnsIP().toString() + "\"";
  json += "},";

  json += "\"bluetooth\":{";
  json += "\"chip_capable\":" + String(diagnosticData.bleChipCapable ? "true" : "false") + ",";
  json += "\"stack_available\":" + String(diagnosticData.bleStackAvailable ? "true" : "false") + ",";
  json += "\"enabled\":" + String(diagnosticData.hasBLE ? "true" : "false") + ",";
  json += "\"status\":\"" + jsonEscape(diagnosticData.bleStatusMessage) + "\",";
  json += "\"backend\":\"" + jsonEscape(diagnosticData.bleBackend) + "\"";
  if (diagnosticData.bleHint.length() > 0) {
    json += ",\"hint\":\"" + jsonEscape(diagnosticData.bleHint) + "\"";
  }
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
    json += "\"cpu_mflops\":" + String(calculateCpuMflops(diagnosticData.cpuBenchmark), 2) + ",";
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
  csv += String(T().memory_details) + ",SRAM KB," + String(detailedMemory.sramTotal / 1024.0, 2) + "\r\n";
  csv += String(T().memory_details) + ",SRAM " + String(T().free) + " KB," + String(detailedMemory.sramFree / 1024.0, 2) + "\r\n";
  csv += String(T().memory_details) + "," + String(T().memory_fragmentation) + " %," + String(detailedMemory.fragmentationPercent, 1) + "\r\n";
  
  csv += "WiFi,SSID," + diagnosticData.wifiSSID + "\r\n";
  csv += "WiFi,RSSI dBm," + String(diagnosticData.wifiRSSI) + "\r\n";
  csv += "WiFi,IP," + diagnosticData.ipAddress + "\r\n";
  csv += "WiFi," + String(T().gateway) + "," + WiFi.gatewayIP().toString() + "\r\n";

  csv += String(T().ble_label) + "," + String(T().ble_chip_support) + "," + String(diagnosticData.bleChipCapable ? T().status_yes : T().status_no) + "\r\n";
  csv += String(T().ble_label) + "," + String(T().ble_stack_support) + "," + String(diagnosticData.bleStackAvailable ? T().status_yes : T().status_missing) + "\r\n";
  csv += String(T().ble_label) + "," + String(T().ble_runtime_status) + "," + String(diagnosticData.hasBLE ? T().status_yes : T().status_missing) + "\r\n";
  csv += String(T().ble_label) + "," + String(T().ble_backend_label) + "," + diagnosticData.bleBackend + "\r\n";
  csv += String(T().ble_label) + "," + String(T().status) + "," + diagnosticData.bleStatusMessage + "\r\n";
  if (diagnosticData.bleHint.length() > 0) {
    csv += String(T().ble_label) + "," + String(T().recommendation) + "," + diagnosticData.bleHint + "\r\n";
  }

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
  html += "<h2>Bluetooth Low Energy</h2>";
  html += "<div class='grid'>";
  html += "<div class='row'><b>Compatibilité puce:</b><span>" + String(diagnosticData.bleChipCapable ? "Oui" : "Non") + "</span></div>";
  html += "<div class='row'><b>Pile firmware:</b><span>" + String(diagnosticData.bleStackAvailable ? "Oui" : "Manquant") + "</span></div>";
  html += "<div class='row'><b>Fonctions actives:</b><span>" + String(diagnosticData.hasBLE ? "Oui" : "Manquant") + "</span></div>";
  html += "<div class='row'><b>" + String(T().ble_backend_label) + ":</b><span>" + diagnosticData.bleBackend + "</span></div>";
  html += "<div class='row'><b>Statut:</b><span>" + diagnosticData.bleStatusMessage + "</span></div>";
  if (diagnosticData.bleHint.length() > 0) {
    html += "<div class='row'><b>Conseil:</b><span>" + diagnosticData.bleHint + "</span></div>";
  }
  html += "</div></div>";

  // GPIO et Périphériques
  html += "<div class='section'>";
  html += "<h2>GPIO et Périphériques</h2>";
  html += "<div class='grid'>";
  html += "<div class='row'><b>GPIO Total:</b><span>" + String(diagnosticData.totalGPIO) + " broches</span></div>";
  html += "<div class='row'><b>I2C:</b><span>" + String(diagnosticData.i2cCount) + " périphérique(s) - " + diagnosticData.i2cDevices + "</span></div>";
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
    html += "<div class='row'><b>CPU:</b><span>" + String(diagnosticData.cpuBenchmark) + " µs (" + String(calculateCpuMflops(diagnosticData.cpuBenchmark), 2) + " MFLOPS)</span></div>";
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
    refreshLocalizedStatusMessages();
    server.send(200, "application/json", "{\"success\":true,\"lang\":\"" + lang + "\"}");
  } else {
    server.send(400, "application/json", "{\"success\":false}");
  }
}

String jsonEscape(const String& input) {
  String escaped = input;
  escaped.replace("\\", "\\\\");
  escaped.replace("\"", "\\\"");
  escaped.replace("\n", "\\n");
  escaped.replace("\r", "\\r");
  escaped.replace("\t", "\\t");
  return escaped;
}

void appendJsonField(String& json, const char* key, const String& value) {
  json += "\"";
  json += key;
  json += "\":\"";
  json += jsonEscape(value);
  json += "\",";
}

void handleGetTranslations() {
  // Envoie toutes les traductions en JSON pour mise à jour dynamique
  String json = "{";
  appendJsonField(json, "lang", String(currentLanguage == LANG_FR ? "fr" : "en"));
  appendJsonField(json, "title", String(T().title));
  appendJsonField(json, "version", String(T().version));
  appendJsonField(json, "access", String(T().access));
  appendJsonField(json, "or_text", String(T().or_text));

  appendJsonField(json, "nav_overview", String(T().nav_overview));
  appendJsonField(json, "nav_leds", String(T().nav_leds));
  appendJsonField(json, "nav_screens", String(T().nav_screens));
  appendJsonField(json, "nav_tests", String(T().nav_tests));
  appendJsonField(json, "nav_gpio", String(T().nav_gpio));
  appendJsonField(json, "nav_wifi", String(T().nav_wifi));
  appendJsonField(json, "nav_benchmark", String(T().nav_benchmark));
  appendJsonField(json, "nav_export", String(T().nav_export));

  appendJsonField(json, "chip_info", String(T().chip_info));
  appendJsonField(json, "full_model", String(T().full_model));
  appendJsonField(json, "cpu_cores", String(T().cpu_cores));
  appendJsonField(json, "mac_wifi", String(T().mac_wifi));
  appendJsonField(json, "last_reset", String(T().last_reset));
  appendJsonField(json, "chip_features", String(T().chip_features));
  appendJsonField(json, "sdk_version", String(T().sdk_version));
  appendJsonField(json, "idf_version", String(T().idf_version));
  appendJsonField(json, "uptime", String(T().uptime));
  appendJsonField(json, "cpu_temp", String(T().cpu_temp));

  appendJsonField(json, "memory_details", String(T().memory_details));
  appendJsonField(json, "flash_memory", String(T().flash_memory));
  appendJsonField(json, "real_size", String(T().real_size));
  appendJsonField(json, "configured_ide", String(T().configured_ide));
  appendJsonField(json, "configuration", String(T().configuration));
  appendJsonField(json, "correct", String(T().correct));
  appendJsonField(json, "to_fix", String(T().to_fix));
  appendJsonField(json, "flash_type", String(T().flash_type));
  appendJsonField(json, "flash_speed", String(T().flash_speed));
  appendJsonField(json, "action_required", String(T().action_required));

  appendJsonField(json, "psram_external", String(T().psram_external));
  appendJsonField(json, "hardware_status", String(T().hardware_status));
  appendJsonField(json, "detected_active", String(T().detected_active));
  appendJsonField(json, "ide_config", String(T().ide_config));
  appendJsonField(json, "enabled", String(T().enabled));
  appendJsonField(json, "total_size", String(T().total_size));
  appendJsonField(json, "free", String(T().free));
  appendJsonField(json, "used", String(T().used));
  appendJsonField(json, "largest_block", String(T().largest_block));
  appendJsonField(json, "integrity_test", String(T().integrity_test));
  appendJsonField(json, "not_detected", String(T().not_detected));
  appendJsonField(json, "disabled", String(T().disabled));
  appendJsonField(json, "psram_usage", String(T().psram_usage));
  appendJsonField(json, "psram_mode_label", String(T().psram_mode_label));
  appendJsonField(json, "psram_status_active", String(T().psram_status_active));
  appendJsonField(json, "psram_status_disabled", String(T().psram_status_disabled));
  appendJsonField(json, "psram_status_not_supported", String(T().psram_status_not_supported));
  appendJsonField(json, "psram_enable_hint", String(T().psram_enable_hint));
  appendJsonField(json, "psram_mode_opi", String(T().psram_mode_opi));
  appendJsonField(json, "psram_mode_qspi", String(T().psram_mode_qspi));
  appendJsonField(json, "psram_mode_unknown", String(T().psram_mode_unknown));

  appendJsonField(json, "internal_sram", String(T().internal_sram));
  appendJsonField(json, "min_free", String(T().min_free));
  appendJsonField(json, "max_alloc", String(T().max_alloc));

  appendJsonField(json, "global_analysis", String(T().global_analysis));
  appendJsonField(json, "memory_fragmentation", String(T().memory_fragmentation));
  appendJsonField(json, "memory_status", String(T().memory_status));
  appendJsonField(json, "total_ram", String(T().total_ram));
  appendJsonField(json, "free_ram_total", String(T().free_ram_total));
  appendJsonField(json, "refresh_memory", String(T().refresh_memory));

  appendJsonField(json, "wifi_connection", String(T().wifi_connection));
  appendJsonField(json, "connected_ssid", String(T().connected_ssid));
  appendJsonField(json, "signal_power", String(T().signal_power));
  appendJsonField(json, "signal_quality", String(T().signal_quality));
  appendJsonField(json, "ip_address", String(T().ip_address));
  appendJsonField(json, "subnet_mask", String(T().subnet_mask));
  appendJsonField(json, "gateway", String(T().gateway));
  appendJsonField(json, "dns", String(T().dns));
  appendJsonField(json, "wifi_channel", String(T().wifi_channel));
  appendJsonField(json, "wifi_click_to_scan", String(T().wifi_click_to_scan));
  appendJsonField(json, "wifi_no_networks", String(T().wifi_no_networks));
  appendJsonField(json, "wifi_not_connected", String(T().wifi_not_connected));
  appendJsonField(json, "ble_scanner", String(T().ble_scanner));
  appendJsonField(json, "scan_ble_devices", String(T().scan_ble_devices));
  appendJsonField(json, "ble_click_to_scan", String(T().ble_click_to_scan));
  appendJsonField(json, "ble_no_devices", String(T().ble_no_devices));
  appendJsonField(json, "ble_not_supported", String(T().ble_not_supported));
  appendJsonField(json, "wireless_status", String(T().wireless_status));
  appendJsonField(json, "wifi_label", String(T().wifi_label));
  appendJsonField(json, "ble_label", String(T().ble_label));
  appendJsonField(json, "recommendation", String(T().recommendation));
  appendJsonField(json, "ble_status_active", String(T().ble_status_active));
  appendJsonField(json, "ble_status_stack_missing", String(T().ble_status_stack_missing));
  appendJsonField(json, "ble_status_not_supported", String(T().ble_status_not_supported));
  appendJsonField(json, "ble_enable_hint", String(T().ble_enable_hint));
  appendJsonField(json, "ble_backend_label", String(T().ble_backend_label));
  appendJsonField(json, "ble_backend_missing", String(T().ble_backend_missing));
  appendJsonField(json, "ble_backend_arduino", String(T().ble_backend_arduino));
  appendJsonField(json, "ble_backend_nimble", String(T().ble_backend_nimble));

  appendJsonField(json, "gpio_interfaces", String(T().gpio_interfaces));
  appendJsonField(json, "total_gpio", String(T().total_gpio));
  appendJsonField(json, "gpio_list", String(T().gpio_list));
  appendJsonField(json, "i2c_peripherals", String(T().i2c_peripherals));
  appendJsonField(json, "device_count", String(T().device_count));
  appendJsonField(json, "detected_addresses", String(T().detected_addresses));
  appendJsonField(json, "rescan_i2c", String(T().rescan_i2c));

  appendJsonField(json, "builtin_led", String(T().builtin_led));
  appendJsonField(json, "gpio", String(T().gpio));
  appendJsonField(json, "status", String(T().status));
  appendJsonField(json, "config", String(T().config));
  appendJsonField(json, "test", String(T().test));
  appendJsonField(json, "blink", String(T().blink));
  appendJsonField(json, "fade", String(T().fade));
  appendJsonField(json, "off", String(T().off));
  appendJsonField(json, "neopixel", String(T().neopixel));
  appendJsonField(json, "rainbow", String(T().rainbow));
  appendJsonField(json, "color", String(T().color));
  appendJsonField(json, "led_conflict", String(T().led_conflict));
  appendJsonField(json, "pwm_disabled", String(T().pwm_disabled));
  appendJsonField(json, "simple_mode", String(T().simple_mode));
  appendJsonField(json, "led_ready", String(T().led_ready));
  appendJsonField(json, "led_test_ok", String(T().led_test_ok));
  appendJsonField(json, "led_configured", String(T().led_configured));
  appendJsonField(json, "led_not_configured", String(T().led_not_configured));
  appendJsonField(json, "led_on_state", String(T().led_on_state));
  appendJsonField(json, "led_off_state", String(T().led_off_state));
  appendJsonField(json, "led_test_skipped", String(T().led_test_skipped));
  appendJsonField(json, "neopixel_not_ready", String(T().neopixel_not_ready));

  appendJsonField(json, "oled_screen", String(T().oled_screen));
  appendJsonField(json, "i2c_pins", String(T().i2c_pins));
  appendJsonField(json, "i2c_address", String(T().i2c_address));
  appendJsonField(json, "no_device", String(T().no_device));
  appendJsonField(json, "pin_config", String(T().pin_config));
  appendJsonField(json, "apply_redetect", String(T().apply_redetect));
  appendJsonField(json, "changes_pins", String(T().changes_pins));
  appendJsonField(json, "custom_message", String(T().custom_message));
  appendJsonField(json, "show_message", String(T().show_message));
  appendJsonField(json, "no_detected", String(T().no_detected));
  appendJsonField(json, "check_wiring", String(T().check_wiring));
  appendJsonField(json, "oled_intro", String(T().oled_intro));
  appendJsonField(json, "oled_large_text", String(T().oled_large_text));
  appendJsonField(json, "oled_fonts", String(T().oled_fonts));
  appendJsonField(json, "oled_shapes", String(T().oled_shapes));
  appendJsonField(json, "oled_lines", String(T().oled_lines));
  appendJsonField(json, "oled_diagonals", String(T().oled_diagonals));
  appendJsonField(json, "oled_animation", String(T().oled_animation));
  appendJsonField(json, "oled_progress", String(T().oled_progress));
  appendJsonField(json, "oled_scroll", String(T().oled_scroll));
  appendJsonField(json, "oled_final", String(T().oled_final));
  appendJsonField(json, "message_required", String(T().message_required));
  appendJsonField(json, "language_changed", String(T().language_changed));
  appendJsonField(json, "pattern_missing", String(T().pattern_missing));
  appendJsonField(json, "pattern_unknown", String(T().pattern_unknown));
  appendJsonField(json, "message_displayed", String(T().message_displayed));
  appendJsonField(json, "oled_reconfigured", String(T().oled_reconfigured));
  appendJsonField(json, "invalid_pins", String(T().invalid_pins));
  appendJsonField(json, "oled_init_failed", String(T().oled_init_failed));

  appendJsonField(json, "adc_test", String(T().adc_test));
  appendJsonField(json, "touch_test", String(T().touch_test));
  appendJsonField(json, "pwm_test", String(T().pwm_test));
  appendJsonField(json, "spi_bus", String(T().spi_bus));
  appendJsonField(json, "flash_partitions", String(T().flash_partitions));
  appendJsonField(json, "list_partitions", String(T().list_partitions));
  appendJsonField(json, "click_button", String(T().click_button));
  appendJsonField(json, "memory_stress", String(T().memory_stress));
  appendJsonField(json, "start_stress", String(T().start_stress));

  appendJsonField(json, "gpio_test", String(T().gpio_test));
  appendJsonField(json, "test_all_gpio", String(T().test_all_gpio));
  appendJsonField(json, "click_to_test", String(T().click_to_test));
  appendJsonField(json, "gpio_fail_hint", String(T().gpio_fail_hint));

  appendJsonField(json, "wifi_scanner", String(T().wifi_scanner));
  appendJsonField(json, "scan_networks", String(T().scan_networks));

  appendJsonField(json, "performance_bench", String(T().performance_bench));
  appendJsonField(json, "run_benchmarks", String(T().run_benchmarks));
  appendJsonField(json, "cpu_benchmark", String(T().cpu_benchmark));
  appendJsonField(json, "memory_benchmark", String(T().memory_benchmark));
  appendJsonField(json, "cpu_performance", String(T().cpu_performance));
  appendJsonField(json, "memory_speed", String(T().memory_speed));
  appendJsonField(json, "not_tested", String(T().not_tested));

  appendJsonField(json, "data_export", String(T().data_export));
  appendJsonField(json, "txt_file", String(T().txt_file));
  appendJsonField(json, "readable_report", String(T().readable_report));
  appendJsonField(json, "download_txt", String(T().download_txt));
  appendJsonField(json, "json_file", String(T().json_file));
  appendJsonField(json, "structured_format", String(T().structured_format));
  appendJsonField(json, "download_json", String(T().download_json));
  appendJsonField(json, "csv_file", String(T().csv_file));
  appendJsonField(json, "for_excel", String(T().for_excel));
  appendJsonField(json, "download_csv", String(T().download_csv));
  appendJsonField(json, "printable_version", String(T().printable_version));
  appendJsonField(json, "pdf_format", String(T().pdf_format));
  appendJsonField(json, "open", String(T().open));

  appendJsonField(json, "export_report", String(T().export_report));
  appendJsonField(json, "export_generated", String(T().export_generated));
  appendJsonField(json, "export_after_boot", String(T().export_after_boot));
  appendJsonField(json, "category", String(T().category));
  appendJsonField(json, "parameter", String(T().parameter));
  appendJsonField(json, "value", String(T().value));
  appendJsonField(json, "model", String(T().model));
  appendJsonField(json, "revision", String(T().revision));
  appendJsonField(json, "frequency", String(T().frequency));
  appendJsonField(json, "real", String(T().real));
  appendJsonField(json, "board", String(T().board));
  appendJsonField(json, "psram_mb", String(T().psram_mb));
  appendJsonField(json, "detected", String(T().detected));
  appendJsonField(json, "sram_kb", String(T().sram_kb));
  appendJsonField(json, "connected", String(T().connected));
  appendJsonField(json, "signal", String(T().signal));
  appendJsonField(json, "list", String(T().list));
  appendJsonField(json, "days", String(T().days));
  appendJsonField(json, "hours", String(T().hours));
  appendJsonField(json, "cores", String(T().cores));
  appendJsonField(json, "pins", String(T().pins));
  appendJsonField(json, "devices", String(T().devices));
  appendJsonField(json, "networks", String(T().networks));
  appendJsonField(json, "tested", String(T().tested));
  appendJsonField(json, "channels", String(T().channels));

  appendJsonField(json, "ok", String(T().ok));
  appendJsonField(json, "fail", String(T().fail));
  appendJsonField(json, "excellent", String(T().excellent));
  appendJsonField(json, "very_good", String(T().very_good));
  appendJsonField(json, "good", String(T().good));
  appendJsonField(json, "warning", String(T().warning));
  appendJsonField(json, "critical", String(T().critical));
  appendJsonField(json, "weak", String(T().weak));
  appendJsonField(json, "very_weak", String(T().very_weak));
  appendJsonField(json, "none", String(T().none));
  appendJsonField(json, "unknown", String(T().unknown));
  appendJsonField(json, "testing", String(T().testing));
  appendJsonField(json, "completed", String(T().completed));
  appendJsonField(json, "scan", String(T().scan));
  appendJsonField(json, "scanning", String(T().scanning));
  appendJsonField(json, "feature_disabled", String(T().feature_disabled));
  appendJsonField(json, "not_supported", String(T().not_supported));
  appendJsonField(json, "max_allocated", String(T().max_allocated));
  appendJsonField(json, "brownout", String(T().brownout));
  appendJsonField(json, "poweron", String(T().poweron));
  appendJsonField(json, "software_reset", String(T().software_reset));
  appendJsonField(json, "deepsleep_exit", String(T().deepsleep_exit));
  appendJsonField(json, "other", String(T().other));

  if (json.endsWith(",")) {
    json.remove(json.length() - 1);
  }
  json += "}";

  server.send(200, "application/json", json);
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
  chunk += "*{margin:0;padding:0;box-sizing:border-box}";
  chunk += "body{font-family:'Segoe UI',sans-serif;background:linear-gradient(135deg,#667eea 0%,#764ba2 100%);padding:20px}";
  chunk += ".container{max-width:1400px;margin:0 auto;background:#fff;border-radius:20px;box-shadow:0 20px 60px rgba(0,0,0,.3);overflow:hidden}";
  chunk += ".header{background:linear-gradient(135deg,#667eea 0%,#764ba2 100%);color:#fff;padding:30px;text-align:center;position:relative}";
  chunk += ".header h1{font-size:2.5em;margin-bottom:10px}";
  chunk += ".lang-switcher{position:absolute;top:20px;right:20px;display:flex;gap:5px}";
  chunk += ".lang-btn{padding:8px 15px;background:rgba(255,255,255,.2);border:2px solid rgba(255,255,255,.3);border-radius:5px;color:#fff;cursor:pointer;font-weight:bold;transition:all .3s}";
  chunk += ".lang-btn:hover{background:rgba(255,255,255,.3)}";
  chunk += ".lang-btn.active{background:rgba(255,255,255,.4);border-color:rgba(255,255,255,.6)}";
  chunk += ".nav{display:flex;justify-content:center;gap:10px;margin-top:20px;flex-wrap:wrap}";
  chunk += ".nav-btn{padding:10px 20px;background:rgba(255,255,255,.2);border:none;border-radius:5px;color:#fff;cursor:pointer;font-weight:bold}";
  chunk += ".nav-btn:hover{background:rgba(255,255,255,.3)}";
  chunk += ".nav-btn.active{background:rgba(255,255,255,.4)}";
  chunk += ".content{padding:30px}";
  chunk += ".tab-content{display:none}";
  chunk += ".tab-content.active{display:block}";
  chunk += ".section{background:#f8f9fa;border-radius:15px;padding:25px;margin-bottom:20px;border-left:5px solid #667eea}";
  chunk += ".section h2{color:#667eea;margin-bottom:20px;font-size:1.5em}";
  chunk += ".section h3{color:#667eea;margin:15px 0 10px;font-size:1.2em}";
  chunk += ".info-grid{display:grid;grid-template-columns:repeat(auto-fit,minmax(300px,1fr));gap:15px}";
  chunk += ".info-item{background:#fff;padding:15px;border-radius:10px;border:1px solid #e0e0e0}";
  chunk += ".info-label{font-weight:bold;color:#667eea;margin-bottom:5px;font-size:.9em}";
  chunk += ".info-value{font-size:1.1em;color:#333}";
  chunk += ".badge{display:inline-block;padding:5px 15px;border-radius:20px;font-size:.9em;font-weight:bold}";
  chunk += ".badge-success{background:#d4edda;color:#155724}";
  chunk += ".badge-warning{background:#fff3cd;color:#856404}";
  chunk += ".badge-danger{background:#f8d7da;color:#721c24}";
  chunk += ".btn{padding:12px 24px;border:none;border-radius:8px;font-size:1em;font-weight:bold;cursor:pointer;margin:5px;text-decoration:none;display:inline-block}";
  chunk += ".btn-primary{background:linear-gradient(135deg,#667eea 0%,#764ba2 100%);color:#fff}";
  chunk += ".btn-success{background:linear-gradient(135deg,#56ab2f 0%,#a8e063 100%);color:#fff}";
  chunk += ".btn-info{background:linear-gradient(135deg,#3a7bd5 0%,#00d2ff 100%);color:#fff}";
  chunk += ".btn-danger{background:linear-gradient(135deg,#eb3349 0%,#f45c43 100%);color:#fff}";
  chunk += ".btn:hover{opacity:.9;transform:translateY(-2px);transition:all .3s}";
  chunk += ".progress-bar{background:#e0e0e0;border-radius:10px;height:20px;overflow:hidden;margin-top:10px}";
  chunk += ".progress-fill{height:100%;border-radius:10px;transition:width .3s}";
  chunk += ".gpio-grid{display:grid;grid-template-columns:repeat(auto-fill,minmax(80px,1fr));gap:10px}";
  chunk += ".gpio-item{padding:15px;background:#fff;border:2px solid #ddd;border-radius:8px;text-align:center;font-weight:bold}";
  chunk += ".gpio-ok{border-color:#28a745;background:#d4edda}";
  chunk += ".gpio-fail{border-color:#dc3545;background:#f8d7da}";
  chunk += ".wifi-list{max-height:400px;overflow-y:auto}";
  chunk += ".wifi-item{background:#fff;padding:15px;margin:10px 0;border-radius:10px;border-left:4px solid #667eea}";
  chunk += ".status-live{padding:10px;background:#f0f0f0;border-radius:5px;text-align:center;font-weight:bold;margin:10px 0}";
  chunk += ".inline-feedback{margin-top:8px;font-size:0.9em;opacity:0;transition:opacity .3s;color:#0c5460;}";
  chunk += ".inline-feedback.show{opacity:1;}";
  chunk += ".inline-feedback.success{color:#155724;}";
  chunk += ".inline-feedback.error{color:#721c24;}";
  chunk += ".inline-feedback.info{color:#0c5460;}";
  chunk += "input[type='number'],input[type='color'],input[type='text']{padding:10px;border:2px solid #ddd;border-radius:5px;font-size:1em}";
  chunk += "@media print{.nav,.btn,.lang-switcher{display:none}}";
  chunk += "</style></head><body>";
  server.sendContent(chunk);
  
  // CHUNK 2: HEADER + NAV
  chunk = "<div class='container'><div class='header'>";
  chunk += "<div class='lang-switcher'>";
  chunk += "<button class='lang-btn " + String(currentLanguage == LANG_FR ? "active" : "") + "' data-lang='fr' onclick='changeLang(\"fr\",event)'>FR</button>";
  chunk += "<button class='lang-btn " + String(currentLanguage == LANG_EN ? "active" : "") + "' data-lang='en' onclick='changeLang(\"en\",event)'>EN</button>";
  chunk += "</div>";
  chunk += "<h1 id='main-title'><span data-i18n='title'>" + String(T().title) + "</span> <span data-i18n='version'>" + String(T().version) + "</span>" + String(DIAGNOSTIC_VERSION) + "</h1>";
  chunk += "<div style='font-size:1.2em;margin:10px 0' id='chipModel'>" + diagnosticData.chipModel + "</div>";
  chunk += "<div style='font-size:.9em;opacity:.9;margin:10px 0'>";
  chunk += "<span data-i18n='access'>" + String(T().access) + "</span>: <a href='http://" + String(MDNS_HOSTNAME) + ".local' style='color:#fff;text-decoration:underline'><strong>http://" + String(MDNS_HOSTNAME) + ".local</strong></a> <span data-i18n='or_text'>" + String(T().or_text) + "</span> <strong id='ipAddress'>" + diagnosticData.ipAddress + "</strong>";
  chunk += "</div>";
  chunk += "<div class='nav'>";
  chunk += "<button class='nav-btn active' onclick='showTab(\"overview\",event)' data-i18n='nav_overview'>" + String(T().nav_overview) + "</button>";
  chunk += "<button class='nav-btn' onclick='showTab(\"leds\",event)' data-i18n='nav_leds'>" + String(T().nav_leds) + "</button>";
  chunk += "<button class='nav-btn' onclick='showTab(\"screens\",event)' data-i18n='nav_screens'>" + String(T().nav_screens) + "</button>";
  chunk += "<button class='nav-btn' onclick='showTab(\"tests\",event)' data-i18n='nav_tests'>" + String(T().nav_tests) + "</button>";
  chunk += "<button class='nav-btn' onclick='showTab(\"gpio\",event)' data-i18n='nav_gpio'>" + String(T().nav_gpio) + "</button>";
  chunk += "<button class='nav-btn' onclick='showTab(\"wifi\",event)' data-i18n='nav_wifi'>" + String(T().nav_wifi) + "</button>";
  chunk += "<button class='nav-btn' onclick='showTab(\"benchmark\",event)' data-i18n='nav_benchmark'>" + String(T().nav_benchmark) + "</button>";
  chunk += "<button class='nav-btn' onclick='showTab(\"export\",event)' data-i18n='nav_export'>" + String(T().nav_export) + "</button>";
  chunk += "</div></div><div class='content'>";
  server.sendContent(chunk);
  
// CHUNK 3: OVERVIEW TAB - VERSION UNIQUE COMPLÈTE
  chunk = "<div id='overview' class='tab-content active'>";
  
  // Chip Info
  chunk += "<div class='section'><h2 data-i18n='chip_info'>" + String(T().chip_info) + "</h2><div class='info-grid'>";
  chunk += "<div class='info-item'><div class='info-label' data-i18n='full_model'>" + String(T().full_model) + "</div><div class='info-value'>" + diagnosticData.chipModel + " Rev" + diagnosticData.chipRevision + "</div></div>";
  chunk += "<div class='info-item'><div class='info-label' data-i18n='cpu_cores'>" + String(T().cpu_cores) + "</div><div class='info-value'>" + String(diagnosticData.cpuCores) + " @ " + String(diagnosticData.cpuFreqMHz) + " MHz</div></div>";
  chunk += "<div class='info-item'><div class='info-label' data-i18n='mac_wifi'>" + String(T().mac_wifi) + "</div><div class='info-value'>" + diagnosticData.macAddress + "</div></div>";
  chunk += "<div class='info-item'><div class='info-label' data-i18n='last_reset'>" + String(T().last_reset) + "</div><div class='info-value'>" + getResetReason() + "</div></div>";
  chunk += "<div class='info-item'><div class='info-label' data-i18n='chip_features'>" + String(T().chip_features) + "</div><div class='info-value'>" + getChipFeatures() + "</div></div>";
  chunk += "<div class='info-item'><div class='info-label' data-i18n='sdk_version'>" + String(T().sdk_version) + "</div><div class='info-value'>" + diagnosticData.sdkVersion + "</div></div>";
  chunk += "<div class='info-item'><div class='info-label' data-i18n='idf_version'>" + String(T().idf_version) + "</div><div class='info-value'>" + diagnosticData.idfVersion + "</div></div>";
  
  unsigned long seconds = diagnosticData.uptime / 1000;
  unsigned long minutes = seconds / 60;
  unsigned long hours = minutes / 60;
  unsigned long days = hours / 24;
  chunk += "<div class='info-item'><div class='info-label' data-i18n='uptime'>" + String(T().uptime) + "</div><div class='info-value'>" + String(days) + "j " + String(hours % 24) + "h " + String(minutes % 60) + "m</div></div>";
  
  if (diagnosticData.temperature != -999) {
    chunk += "<div class='info-item'><div class='info-label' data-i18n='cpu_temp'>" + String(T().cpu_temp) + "</div><div class='info-value'>" + String(diagnosticData.temperature, 1) + " °C</div></div>";
  }
  chunk += "</div></div>";
  server.sendContent(chunk);
  
  // Memory - Flash
  chunk = "<div class='section'><h2 data-i18n='memory_details'>" + String(T().memory_details) + "</h2>";
  chunk += "<h3 data-i18n='flash_memory'>" + String(T().flash_memory) + "</h3><div class='info-grid'>";
  chunk += "<div class='info-item'><div class='info-label' data-i18n='real_size'>" + String(T().real_size) + "</div><div class='info-value'>" + String(detailedMemory.flashSizeReal / 1048576.0, 2) + " MB</div></div>";
  chunk += "<div class='info-item'><div class='info-label' data-i18n='configured_ide'>" + String(T().configured_ide) + "</div><div class='info-value'>" + String(detailedMemory.flashSizeChip / 1048576.0, 2) + " MB</div></div>";
  
  bool flashMatch = (detailedMemory.flashSizeReal == detailedMemory.flashSizeChip);
  chunk += "<div class='info-item'><div class='info-label' data-i18n='configuration'>" + String(T().configuration) + "</div><div class='info-value'><span class='badge ";
  chunk += flashMatch ? "badge-success'>" + String(T().correct) : "badge-warning'>" + String(T().to_fix);
  chunk += "</span></div></div>";
  chunk += "<div class='info-item'><div class='info-label' data-i18n='flash_type'>" + String(T().flash_type) + "</div><div class='info-value'>" + getFlashType() + "</div></div>";
  chunk += "<div class='info-item'><div class='info-label' data-i18n='flash_speed'>" + String(T().flash_speed) + "</div><div class='info-value'>" + getFlashSpeed() + "</div></div>";
  chunk += "</div>";
  server.sendContent(chunk);
  
  // Memory - PSRAM
  chunk = "<h3 data-i18n='psram_external'>" + String(T().psram_external) + "</h3><div class='info-grid'>";
  chunk += "<div class='info-item'><div class='info-label' data-i18n='hardware_status'>" + String(T().hardware_status) + "</div><div class='info-value'>" + detailedMemory.psramStatusMessage + "</div></div>";
  chunk += "<div class='info-item'><div class='info-label' data-i18n='psram_mode_label'>" + String(T().psram_mode_label) + "</div><div class='info-value'>" + detailedMemory.psramModeLabel + "</div></div>";
  if (detailedMemory.psramAvailable) {
    chunk += "<div class='info-item'><div class='info-label' data-i18n='total_size'>" + String(T().total_size) + "</div><div class='info-value'>" + String(detailedMemory.psramTotal / 1048576.0, 2) + " MB</div></div>";
    chunk += "<div class='info-item'><div class='info-label' data-i18n='free'>" + String(T().free) + "</div><div class='info-value'>" + String(detailedMemory.psramFree / 1048576.0, 2) + " MB</div></div>";
    chunk += "<div class='info-item'><div class='info-label' data-i18n='used'>" + String(T().used) + "</div><div class='info-value'>" + String(detailedMemory.psramUsed / 1048576.0, 2) + " MB</div></div>";
  }
  if (detailedMemory.psramRecommendation.length() > 0) {
    chunk += "<div class='info-item' style='grid-column:1/-1'><div class='info-label' data-i18n='recommendation'>" + String(T().recommendation) + "</div><div class='info-value'>" + detailedMemory.psramRecommendation + "</div></div>";
  }
  chunk += "</div>";
  server.sendContent(chunk);
  
  // Memory - SRAM
  chunk = "<h3 data-i18n='internal_sram'>" + String(T().internal_sram) + "</h3><div class='info-grid'>";
  chunk += "<div class='info-item'><div class='info-label' data-i18n='total_size'>" + String(T().total_size) + "</div><div class='info-value'>" + String(detailedMemory.sramTotal / 1024.0, 2) + " KB</div></div>";
  chunk += "<div class='info-item'><div class='info-label' data-i18n='free'>" + String(T().free) + "</div><div class='info-value'>" + String(detailedMemory.sramFree / 1024.0, 2) + " KB</div></div>";
  chunk += "<div class='info-item'><div class='info-label' data-i18n='used'>" + String(T().used) + "</div><div class='info-value'>" + String(detailedMemory.sramUsed / 1024.0, 2) + " KB</div></div>";
  chunk += "<div class='info-item'><div class='info-label' data-i18n='memory_fragmentation'>" + String(T().memory_fragmentation) + "</div><div class='info-value'>" + String(detailedMemory.fragmentationPercent, 1) + "%</div></div>";
  chunk += "</div>";
  chunk += "<div style='text-align:center;margin-top:15px'><button class='btn btn-info' onclick='location.reload()' data-i18n='refresh_memory'>" + String(T().refresh_memory) + "</button></div></div>";
  server.sendContent(chunk);
  
  // WiFi
  chunk = "<div class='section'><h2 data-i18n='wifi_connection'>" + String(T().wifi_connection) + "</h2><div class='info-grid'>";
  chunk += "<div class='info-item'><div class='info-label' data-i18n='connected_ssid'>" + String(T().connected_ssid) + "</div><div class='info-value'>" + diagnosticData.wifiSSID + "</div></div>";
  chunk += "<div class='info-item'><div class='info-label' data-i18n='signal_power'>" + String(T().signal_power) + "</div><div class='info-value'>" + String(diagnosticData.wifiRSSI) + " dBm</div></div>";
  chunk += "<div class='info-item'><div class='info-label' data-i18n='signal_quality'>" + String(T().signal_quality) + "</div><div class='info-value'>" + getWiFiSignalQuality() + "</div></div>";
  chunk += "<div class='info-item'><div class='info-label' data-i18n='ip_address'>" + String(T().ip_address) + "</div><div class='info-value'>" + diagnosticData.ipAddress + "</div></div>";
  chunk += "<div class='info-item'><div class='info-label' data-i18n='subnet_mask'>" + String(T().subnet_mask) + "</div><div class='info-value'>" + WiFi.subnetMask().toString() + "</div></div>";
  chunk += "<div class='info-item'><div class='info-label' data-i18n='gateway'>" + String(T().gateway) + "</div><div class='info-value'>" + WiFi.gatewayIP().toString() + "</div></div>";
  chunk += "<div class='info-item'><div class='info-label' data-i18n='dns'>" + String(T().dns) + "</div><div class='info-value'>" + WiFi.dnsIP().toString() + "</div></div>";
  chunk += "</div></div>";
  server.sendContent(chunk);
  
  // GPIO et I2C
  chunk = "<div class='section'><h2 data-i18n='gpio_interfaces'>" + String(T().gpio_interfaces) + "</h2><div class='info-grid'>";
  chunk += "<div class='info-item'><div class='info-label' data-i18n='total_gpio'>" + String(T().total_gpio) + "</div><div class='info-value'>" + String(diagnosticData.totalGPIO) + " " + String(T().pins) + "</div></div>";
  if (ENABLE_I2C_SCAN) {
    chunk += "<div class='info-item'><div class='info-label' data-i18n='i2c_peripherals'>" + String(T().i2c_peripherals) + "</div><div class='info-value'>" + String(diagnosticData.i2cCount) + " " + String(T().devices) + "</div></div>";
    if (diagnosticData.i2cCount > 0) {
      chunk += "<div class='info-item' style='grid-column:1/-1'><div class='info-label' data-i18n='detected_addresses'>" + String(T().detected_addresses) + "</div><div class='info-value'>" + diagnosticData.i2cDevices + "</div></div>";
    }
  }
  chunk += "</div></div>";
  chunk += "</div>"; // Fermeture div overview
  server.sendContent(chunk);

  // CHUNK 4: TAB LEDs
  chunk = "<div id='leds' class='tab-content'>";
  chunk += "<div class='section'><h2 data-i18n='builtin_led'>" + String(T().builtin_led) + "</h2><div class='info-grid'>";
  chunk += "<div class='info-item'><div class='info-label' data-i18n='gpio'>" + String(T().gpio) + "</div><div class='info-value'>GPIO " + String(BUILTIN_LED_PIN) + "</div></div>";
  chunk += "<div class='info-item'><div class='info-label' data-i18n='status'>" + String(T().status) + "</div><div class='info-value' id='builtin-led-status'>" + builtinLedTestResult + "</div></div>";
  if (LED_PIN == BUILTIN_LED_PIN && BUILTIN_LED_PIN >= 0) {
    chunk += "<div class='info-item' style='grid-column:1/-1'><div class='info-label' data-i18n='warning'>" + String(T().warning) + "</div><div class='info-value' data-i18n='led_conflict'>" + String(T().led_conflict) + "</div></div>";
  }
  chunk += "<div class='info-item' style='grid-column:1/-1;text-align:center'>";
  chunk += "<input type='number' id='ledGPIO' value='" + String(BUILTIN_LED_PIN) + "' min='0' max='48' style='width:80px'>";
  chunk += "<button class='btn btn-info' onclick='configBuiltinLED()' data-i18n='config'>" + String(T().config) + "</button>";
  chunk += "<button class='btn btn-primary' onclick='testBuiltinLED()' data-i18n='test'>" + String(T().test) + "</button>";
  chunk += "<button class='btn btn-success' onclick='ledBlink()' data-i18n='blink'>" + String(T().blink) + "</button>";
  chunk += "<button class='btn btn-info' onclick='ledFade()' data-i18n='fade'>" + String(T().fade) + "</button>";
  chunk += "<button class='btn btn-danger' onclick='ledOff()' data-i18n='off'>" + String(T().off) + "</button>";
  chunk += "<div class='inline-feedback' id='builtin-led-feedback'></div>";
  chunk += "</div></div></div>";

  chunk += "<div class='section'><h2 data-i18n='neopixel'>" + String(T().neopixel) + "</h2><div class='info-grid'>";
  chunk += "<div class='info-item'><div class='info-label' data-i18n='gpio'>" + String(T().gpio) + "</div><div class='info-value'>GPIO " + String(LED_PIN) + "</div></div>";
  chunk += "<div class='info-item'><div class='info-label' data-i18n='status'>" + String(T().status) + "</div><div class='info-value' id='neopixel-status'>" + neopixelTestResult + "</div></div>";
  chunk += "<div class='info-item' style='grid-column:1/-1;text-align:center'>";
  chunk += "<input type='number' id='neoGPIO' value='" + String(LED_PIN) + "' min='0' max='48' style='width:80px'>";
  chunk += "<input type='number' id='neoCount' value='" + String(LED_COUNT) + "' min='1' max='100' style='width:80px'>";
  chunk += "<button class='btn btn-info' onclick='configNeoPixel()' data-i18n='config'>" + String(T().config) + "</button>";
  chunk += "<button class='btn btn-primary' onclick='testNeoPixel()' data-i18n='test'>" + String(T().test) + "</button>";
  chunk += "<button class='btn btn-primary' onclick='neoPattern(\"rainbow\")' data-i18n='rainbow'>" + String(T().rainbow) + "</button>";
  chunk += "<input type='color' id='neoColor' value='#ff0000' style='height:48px'>";
  chunk += "<button class='btn btn-primary' onclick='neoCustomColor()' data-i18n='color'>" + String(T().color) + "</button>";
  chunk += "<button class='btn btn-danger' onclick='neoPattern(\"off\")' data-i18n='off'>" + String(T().off) + "</button>";
  chunk += "<div class='inline-feedback' id='neopixel-feedback'></div>";
  chunk += "</div></div></div></div>";
  server.sendContent(chunk);
  
  // CHUNK 5: TAB Screens
  chunk = "<div id='screens' class='tab-content'>";
  chunk += "<div class='section'><h2 data-i18n='oled_screen'>" + String(T().oled_screen) + "</h2><div class='info-grid'>";
  chunk += "<div class='info-item'><div class='info-label' data-i18n='status'>" + String(T().status) + "</div><div class='info-value' id='oled-status'>" + oledTestResult + "</div></div>";
  chunk += "<div class='info-item'><div class='info-label' data-i18n='i2c_pins'>" + String(T().i2c_pins) + "</div><div class='info-value'>SDA:" + String(I2C_SDA) + " SCL:" + String(I2C_SCL) + "</div></div>";
  chunk += "<div class='info-item' style='grid-column:1/-1;text-align:center'>";
  chunk += "SDA: <input type='number' id='oledSDA' value='" + String(I2C_SDA) + "' min='0' max='48' style='width:70px'> ";
  chunk += "SCL: <input type='number' id='oledSCL' value='" + String(I2C_SCL) + "' min='0' max='48' style='width:70px'> ";
  chunk += "<button class='btn btn-info' onclick='configOLED()' data-i18n='apply_redetect'>" + String(T().apply_redetect) + "</button>";
  chunk += "<div class='inline-feedback' id='oled-feedback'></div>";
  chunk += "</div>";
  if (!oledAvailable) {
    chunk += "<div class='info-item' style='grid-column:1/-1'>";
    chunk += "<div class='info-value badge-warning' data-i18n='no_detected'>" + String(T().no_detected) + "</div>";
    chunk += "</div>";
  }
  chunk += "<div class='info-item' style='grid-column:1/-1;text-align:center'>";
  chunk += "<div><button class='btn btn-primary' onclick='testOLED()' data-i18n='full_test'>" + String(T().full_test) + "</button></div>";
  chunk += "<div style='margin-top:10px'><input type='text' id='oledMsg' placeholder='" + String(T().custom_message) + "' data-i18n-placeholder='custom_message' style='width:250px;margin:0 5px'>";
  chunk += "<button class='btn btn-success' onclick='oledMessage()' data-i18n='show_message'>" + String(T().show_message) + "</button></div>";
  chunk += "<div style='margin-top:15px'>";
  chunk += "<button class='btn btn-primary' onclick='oledRun(\"intro\")' data-i18n='oled_intro'>" + String(T().oled_intro) + "</button>";
  chunk += "<button class='btn btn-primary' onclick='oledRun(\"large\")' data-i18n='oled_large_text'>" + String(T().oled_large_text) + "</button>";
  chunk += "<button class='btn btn-primary' onclick='oledRun(\"fonts\")' data-i18n='oled_fonts'>" + String(T().oled_fonts) + "</button>";
  chunk += "<button class='btn btn-primary' onclick='oledRun(\"shapes\")' data-i18n='oled_shapes'>" + String(T().oled_shapes) + "</button>";
  chunk += "</div>";
  chunk += "<div style='margin-top:5px'>";
  chunk += "<button class='btn btn-primary' onclick='oledRun(\"lines\")' data-i18n='oled_lines'>" + String(T().oled_lines) + "</button>";
  chunk += "<button class='btn btn-primary' onclick='oledRun(\"diagonals\")' data-i18n='oled_diagonals'>" + String(T().oled_diagonals) + "</button>";
  chunk += "<button class='btn btn-primary' onclick='oledRun(\"square\")' data-i18n='oled_animation'>" + String(T().oled_animation) + "</button>";
  chunk += "<button class='btn btn-primary' onclick='oledRun(\"progress\")' data-i18n='oled_progress'>" + String(T().oled_progress) + "</button>";
  chunk += "</div>";
  chunk += "<div style='margin-top:5px'>";
  chunk += "<button class='btn btn-primary' onclick='oledRun(\"scroll\")' data-i18n='oled_scroll'>" + String(T().oled_scroll) + "</button>";
  chunk += "<button class='btn btn-danger' onclick='oledRun(\"final\")' data-i18n='oled_final'>" + String(T().oled_final) + "</button>";
  chunk += "</div>";
  chunk += "</div>";
  chunk += "</div></div></div></div>";
  server.sendContent(chunk);
  
  // CHUNK 6: TAB Tests
  chunk = "<div id='tests' class='tab-content'>";
  chunk += "<div class='section'><h2 data-i18n='adc_test'>" + String(T().adc_test) + "</h2>";
  chunk += "<div style='text-align:center;margin:20px 0'>";
  chunk += "<button class='btn btn-primary' onclick='testADC()' data-i18n='test'>" + String(T().test) + "</button>";
  chunk += "<div id='adc-status' class='status-live'>" + adcTestResult + "</div>";
  chunk += "</div><div id='adc-results' class='info-grid'></div></div>";

  chunk += "<div class='section'><h2 data-i18n='touch_test'>" + String(T().touch_test) + "</h2>";
  chunk += "<div style='text-align:center;margin:20px 0'>";
  chunk += "<button class='btn btn-primary' onclick='testTouch()' data-i18n='test'>" + String(T().test) + "</button>";
  chunk += "<div id='touch-status' class='status-live'>" + touchTestResult + "</div>";
  chunk += "</div><div id='touch-results' class='info-grid'></div></div>";

  chunk += "<div class='section'><h2 data-i18n='pwm_test'>" + String(T().pwm_test) + "</h2>";
  chunk += "<div style='text-align:center;margin:20px 0'>";
  chunk += "<button class='btn btn-primary' onclick='testPWM()' data-i18n='test'>" + String(T().test) + "</button>";
  chunk += "<div id='pwm-status' class='status-live'>" + pwmTestResult + "</div>";
  chunk += "</div></div>";

  chunk += "<div class='section'><h2 data-i18n='spi_bus'>" + String(T().spi_bus) + "</h2>";
  chunk += "<div style='text-align:center;margin:20px 0'>";
  chunk += "<button class='btn btn-primary' onclick='scanSPI()' data-i18n='scan'>" + String(T().scan) + "</button>";
  chunk += "<div id='spi-status' class='status-live'>" + (spiInfo.length() > 0 ? spiInfo : String(T().click_button)) + "</div>";
  chunk += "</div></div>";

  chunk += "<div class='section'><h2 data-i18n='flash_partitions'>" + String(T().flash_partitions) + "</h2>";
  chunk += "<div style='text-align:center;margin:20px 0'>";
  chunk += "<button class='btn btn-primary' onclick='listPartitions()' data-i18n='list_partitions'>" + String(T().list_partitions) + "</button>";
  chunk += "</div><div id='partitions-results' style='background:#fff;padding:15px;border-radius:10px;font-family:monospace;white-space:pre-wrap;font-size:0.85em'>";
  chunk += partitionsInfo.length() > 0 ? partitionsInfo : String(T().click_button);
  chunk += "</div></div>";

  chunk += "<div class='section'><h2 data-i18n='memory_stress'>" + String(T().memory_stress) + "</h2>";
  chunk += "<div style='text-align:center;margin:20px 0'>";
  chunk += "<button class='btn btn-danger' onclick='stressTest()' data-i18n='start_stress'>" + String(T().start_stress) + "</button>";
  chunk += "<div id='stress-status' class='status-live'>" + stressTestResult + "</div>";
  chunk += "</div></div></div>";
  server.sendContent(chunk);
  
  // CHUNK 7: TAB GPIO
  chunk = "<div id='gpio' class='tab-content'>";
  chunk += "<div class='section'><h2 data-i18n='gpio_test'>" + String(T().gpio_test) + "</h2>";
  chunk += "<div style='text-align:center;margin:20px 0'>";
  chunk += "<button class='btn btn-primary' onclick='testAllGPIO()' data-i18n='test_all_gpio'>" + String(T().test_all_gpio) + "</button>";
  chunk += "<div id='gpio-status' class='status-live' data-i18n='click_to_test'>" + String(T().click_to_test) + "</div>";
  chunk += "</div>";
  chunk += "<p class='status-live' style='margin:10px 0;background:#fff3cd;color:#856404;border-left:4px solid #f2c94c;padding:12px' data-i18n='gpio_fail_hint'>" + String(T().gpio_fail_hint) + "</p>";
  chunk += "<div id='gpio-results' class='gpio-grid'></div></div></div>";
  server.sendContent(chunk);
  
  // CHUNK 8: TAB WiFi
  chunk = "<div id='wifi' class='tab-content'>";
  chunk += "<div class='section'><h2 data-i18n='wifi_scanner'>" + String(T().wifi_scanner) + "</h2>";
  chunk += "<div style='text-align:center;margin:20px 0'>";
  chunk += "<button class='btn btn-primary' onclick='scanWiFi()' data-i18n='scan_networks'>" + String(T().scan_networks) + "</button>";
  chunk += "<div id='wifi-status' class='status-live' data-i18n='click_to_test'>" + String(T().click_to_test) + "</div>";
  chunk += "</div><div id='wifi-results' class='wifi-list'></div></div></div>";
  server.sendContent(chunk);
  
  // CHUNK 9: TAB Benchmark
  chunk = "<div id='benchmark' class='tab-content'>";
  chunk += "<div class='section'><h2 data-i18n='performance_bench'>" + String(T().performance_bench) + "</h2>";
  chunk += "<div style='text-align:center;margin:20px 0'>";
  chunk += "<button class='btn btn-primary' onclick='runBenchmarks()' data-i18n='run_benchmarks'>" + String(T().run_benchmarks) + "</button>";
  chunk += "</div><div id='benchmark-results' class='info-grid'>";
  chunk += "<div class='info-item'><div class='info-label' data-i18n='cpu_benchmark'>" + String(T().cpu_benchmark) + "</div><div class='info-value' id='cpu-bench' data-i18n='not_tested'>" + String(T().not_tested) + "</div></div>";
  chunk += "<div class='info-item'><div class='info-label' data-i18n='memory_benchmark'>" + String(T().memory_benchmark) + "</div><div class='info-value' id='mem-bench' data-i18n='not_tested'>" + String(T().not_tested) + "</div></div>";
  chunk += "<div class='info-item'><div class='info-label' data-i18n='cpu_performance'>" + String(T().cpu_performance) + "</div><div class='info-value' id='cpu-perf'>-</div></div>";
  chunk += "<div class='info-item'><div class='info-label' data-i18n='memory_speed'>" + String(T().memory_speed) + "</div><div class='info-value' id='mem-speed'>-</div></div>";
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
  chunk += "</div></div>"; // Fermeture content + container
  server.sendContent(chunk);
  // CHUNK 11: JavaScript complet
  chunk = "<script>";
  chunk += "let currentLang='" + String(currentLanguage == LANG_FR ? "fr" : "en") + "';";
  chunk += "let translations={};";
  chunk += "const inlineNoticeTimers={};";
  chunk += "function isSuccessFlag(v){return v===true||v==='true';}";
  chunk += "function showInlineNotice(id,message,type){const el=document.getElementById(id);if(!el||!message)return;const tone=type||'info';el.classList.remove('success','error','info','show');el.classList.add('inline-feedback');el.classList.add(tone);el.textContent=message;void el.offsetWidth;el.classList.add('show');clearTimeout(inlineNoticeTimers[id]);inlineNoticeTimers[id]=setTimeout(()=>{el.classList.remove('show');},4000);}";

  // Helpers
  chunk += "function updateLangButtons(lang){";
  chunk += "document.querySelectorAll('.lang-btn').forEach(btn=>btn.classList.toggle('active',btn.dataset.lang===lang));";
  chunk += "}";
  chunk += "function applyText(el,value){if(!el)return;const stored=el.dataset.i18nValue;if(!stored||el.textContent===stored){el.textContent=value;}el.dataset.i18nValue=value;}";
  chunk += "function applyPlaceholder(el,value){if(!el)return;const stored=el.dataset.i18nPlaceholderValue;if(!stored||el.getAttribute('placeholder')===stored){el.setAttribute('placeholder',value);}el.dataset.i18nPlaceholderValue=value;}";

  // Changement de langue
  chunk += "function changeLang(lang,evt){";
  chunk += "if(evt)evt.preventDefault();";
  chunk += "fetch('/api/set-language?lang='+lang).then(r=>r.json()).then(d=>{if(d.success){currentLang=lang;updateLangButtons(lang);updateTranslations();}}).catch(err=>console.error('Lang switch',err));";
  chunk += "}";

  // Mise à jour traductions
  chunk += "function updateTranslations(){";
  chunk += "return fetch('/api/get-translations').then(r=>r.json()).then(tr=>{";
  chunk += "translations=tr;";
  chunk += "document.documentElement.setAttribute('lang',tr.lang||currentLang);";
  chunk += "document.title=tr.title+' '+tr.version+'" + String(DIAGNOSTIC_VERSION) + "';";
  chunk += "const titleSpan=document.querySelector('#main-title [data-i18n=\\'title\\']');";
  chunk += "if(titleSpan)applyText(titleSpan,tr.title);";
  chunk += "const versionSpan=document.querySelector('#main-title [data-i18n=\\'version\\']');";
  chunk += "if(versionSpan)applyText(versionSpan,tr.version);";
  chunk += "document.querySelectorAll('[data-i18n]').forEach(el=>{const key=el.getAttribute('data-i18n');if(key==='title'||key==='version')return;const value=tr[key];if(value)applyText(el,value);});";
  chunk += "document.querySelectorAll('[data-i18n-placeholder]').forEach(el=>{const key=el.getAttribute('data-i18n-placeholder');const value=tr[key];if(value)applyPlaceholder(el,value);});";
  chunk += "}).catch(err=>console.error('updateTranslations',err));";
  chunk += "}";

  chunk += "document.addEventListener('DOMContentLoaded',()=>{updateLangButtons(currentLang);updateTranslations();});";

  // Navigation onglets
  chunk += "function showTab(t,evt){";
  chunk += "document.querySelectorAll('.tab-content').forEach(e=>e.classList.remove('active'));";
  chunk += "document.querySelectorAll('.nav-btn').forEach(e=>e.classList.remove('active'));";
  chunk += "const tab=document.getElementById(t);";
  chunk += "if(tab)tab.classList.add('active');";
  chunk += "if(evt&&evt.target)evt.target.classList.add('active');";
  chunk += "}";
  
  // LED intégrée
  chunk += "function configBuiltinLED(){fetch('/api/builtin-led-config?gpio='+document.getElementById('ledGPIO').value)";
  chunk += ".then(r=>r.json()).then(d=>{const ok=isSuccessFlag(d.success);const msg=d.message||(ok?(translations.led_configured||'Configuration mise à jour'):(translations.invalid_pins||'Erreur'));";
  chunk += "document.getElementById('builtin-led-status').innerHTML=msg;showInlineNotice('builtin-led-feedback',msg,ok?'success':'error');})";
  chunk += ".catch(err=>{console.error('configBuiltinLED',err);showInlineNotice('builtin-led-feedback','Erreur configuration','error');});}";
  chunk += "function testBuiltinLED(){document.getElementById('builtin-led-status').innerHTML=(translations.testing||'Test...');";
  chunk += "fetch('/api/builtin-led-test').then(r=>r.json()).then(d=>{const ok=isSuccessFlag(d.success);const msg=d.result||(ok?(translations.completed||'Terminé'):'Erreur');document.getElementById('builtin-led-status').innerHTML=msg;showInlineNotice('builtin-led-feedback',msg,ok?'success':'error');})";
  chunk += ".catch(err=>{console.error('testBuiltinLED',err);showInlineNotice('builtin-led-feedback','Erreur test','error');});}";
  chunk += "function ledBlink(){fetch('/api/builtin-led-control?action=blink').then(r=>r.json()).then(d=>{const ok=isSuccessFlag(d.success);const msg=d.message||(ok?(translations.completed||'Terminé'):'Erreur');document.getElementById('builtin-led-status').innerHTML=msg;showInlineNotice('builtin-led-feedback',msg,ok?'success':'error');}).catch(err=>{console.error('ledBlink',err);showInlineNotice('builtin-led-feedback','Erreur test','error');});}";
  chunk += "function ledFade(){fetch('/api/builtin-led-control?action=fade').then(r=>r.json()).then(d=>{const ok=isSuccessFlag(d.success);const msg=d.message||(ok?(translations.completed||'Terminé'):'Erreur');document.getElementById('builtin-led-status').innerHTML=msg;showInlineNotice('builtin-led-feedback',msg,ok?'success':'error');}).catch(err=>{console.error('ledFade',err);showInlineNotice('builtin-led-feedback','Erreur test','error');});}";
  chunk += "function ledOff(){fetch('/api/builtin-led-control?action=off').then(r=>r.json()).then(d=>{const ok=isSuccessFlag(d.success);const msg=d.message||(ok?(translations.led_off_state||'LED éteinte'):'Erreur');document.getElementById('builtin-led-status').innerHTML=msg;showInlineNotice('builtin-led-feedback',msg,ok?'success':'error');}).catch(err=>{console.error('ledOff',err);showInlineNotice('builtin-led-feedback','Erreur test','error');});}";
  
  // NeoPixel
  chunk += "function configNeoPixel(){fetch('/api/neopixel-config?gpio='+document.getElementById('neoGPIO').value+'&count='+document.getElementById('neoCount').value)";
  chunk += ".then(r=>r.json()).then(d=>{const ok=isSuccessFlag(d.success);const msg=d.message||(ok?(translations.led_configured||'Configuration mise à jour'):'Erreur');document.getElementById('neopixel-status').innerHTML=msg;showInlineNotice('neopixel-feedback',msg,ok?'success':'error');})";
  chunk += ".catch(err=>{console.error('configNeoPixel',err);showInlineNotice('neopixel-feedback','Erreur configuration NeoPixel','error');});}";
  chunk += "function testNeoPixel(){fetch('/api/neopixel-test').then(r=>r.json()).then(d=>{const ok=isSuccessFlag(d.success);const msg=d.result||(ok?(translations.completed||'Terminé'):'Erreur');document.getElementById('neopixel-status').innerHTML=msg;showInlineNotice('neopixel-feedback',msg,ok?'success':'error');}).catch(err=>{console.error('testNeoPixel',err);showInlineNotice('neopixel-feedback','Erreur test','error');});}";
  chunk += "function neoPattern(p){fetch('/api/neopixel-pattern?pattern='+p).then(r=>r.json()).then(d=>{const ok=isSuccessFlag(d.success);const msg=d.message||(ok?(translations.completed||'Terminé'):'Erreur');document.getElementById('neopixel-status').innerHTML=msg;showInlineNotice('neopixel-feedback',msg,ok?'success':'error');}).catch(err=>{console.error('neoPattern',err);showInlineNotice('neopixel-feedback','Erreur test','error');});}";
  chunk += "function neoCustomColor(){const c=document.getElementById('neoColor').value;";
  chunk += "const r=parseInt(c.substr(1,2),16),g=parseInt(c.substr(3,2),16),b=parseInt(c.substr(5,2),16);";
  chunk += "fetch('/api/neopixel-color?r='+r+'&g='+g+'&b='+b).then(r=>r.json()).then(d=>{const ok=isSuccessFlag(d.success);const msg=d.message||(ok?'RGB('+r+','+g+','+b+')':'Erreur');document.getElementById('neopixel-status').innerHTML=msg;showInlineNotice('neopixel-feedback',msg,ok?'success':'error');}).catch(err=>{console.error('neoCustomColor',err);showInlineNotice('neopixel-feedback','Erreur couleur','error');});}";
  
  // OLED
  chunk += "function testOLED(){document.getElementById('oled-status').innerHTML=(translations.testing||'Test...')+' (25s)...';";
  chunk += "fetch('/api/oled-test').then(r=>r.json()).then(d=>{const ok=isSuccessFlag(d.success);const msg=d.result||(ok?(translations.completed||'Terminé'):'Erreur');document.getElementById('oled-status').innerHTML=msg;showInlineNotice('oled-feedback',msg,ok?'success':'error');}).catch(err=>{console.error('testOLED',err);showInlineNotice('oled-feedback','Erreur test OLED','error');});}";
  chunk += "function oledRun(p){const status=document.getElementById('oled-status');if(status)status.innerHTML=(translations.testing||'Test...');";
  chunk += "fetch('/api/oled-pattern?pattern='+encodeURIComponent(p)).then(r=>r.json()).then(d=>{const ok=isSuccessFlag(d.success);const msg=d.message||(ok?(translations.completed||'Terminé'):'Erreur');if(status)status.innerHTML=msg;showInlineNotice('oled-feedback',msg,ok?'success':'error');}).catch(err=>{console.error('oledRun',err);showInlineNotice('oled-feedback','Erreur motif OLED','error');});}";
  chunk += "function oledMessage(){fetch('/api/oled-message?message='+encodeURIComponent(document.getElementById('oledMsg').value))";
  chunk += ".then(r=>r.json()).then(d=>{const ok=isSuccessFlag(d.success);const msg=d.message||(ok?(translations.message_displayed||'Message affiché'):'Erreur');document.getElementById('oled-status').innerHTML=msg;showInlineNotice('oled-feedback',msg,ok?'success':'error');}).catch(err=>{console.error('oledMessage',err);showInlineNotice('oled-feedback','Erreur message OLED','error');});}";
  chunk += "function configOLED(){const status=document.getElementById('oled-status');if(status)status.innerHTML=(translations.scanning||'Scan...');";
  chunk += "fetch('/api/oled-config?sda='+document.getElementById('oledSDA').value+'&scl='+document.getElementById('oledSCL').value)";
  chunk += ".then(r=>r.json()).then(d=>{const ok=isSuccessFlag(d.success);const msg=d.message||(ok?'OK':(translations.invalid_pins||'Erreur'));if(status)status.innerHTML=msg;showInlineNotice('oled-feedback',msg,ok?'success':'error');}).catch(err=>{console.error('configOLED',err);if(status)status.innerHTML='Erreur configuration';showInlineNotice('oled-feedback','Erreur configuration','error');})}";
  
  // Tests avancés
  chunk += "function testADC(){document.getElementById('adc-status').innerHTML=(translations.testing||'Test...');";
  chunk += "fetch('/api/adc-test').then(r=>r.json()).then(data=>{let h='';";
  chunk += "data.readings.forEach(a=>{h+='<div class=\"info-item\"><div class=\"info-label\">GPIO '+a.pin+'</div><div class=\"info-value\">'+a.raw+' ('+a.voltage.toFixed(2)+'V)</div></div>'});";
  chunk += "document.getElementById('adc-results').innerHTML=h;document.getElementById('adc-status').innerHTML=data.result})}";
  
  chunk += "function testTouch(){document.getElementById('touch-status').innerHTML=(translations.testing||'Test...');";
  chunk += "fetch('/api/touch-test').then(r=>r.json()).then(data=>{let h='';";
  chunk += "data.readings.forEach(t=>{h+='<div class=\"info-item\"><div class=\"info-label\">Touch'+t.pin+'</div><div class=\"info-value\">'+t.value+'</div></div>'});";
  chunk += "document.getElementById('touch-results').innerHTML=h;document.getElementById('touch-status').innerHTML=data.result})}";
  
  chunk += "function testPWM(){document.getElementById('pwm-status').innerHTML=(translations.testing||'Test...');";
  chunk += "fetch('/api/pwm-test').then(r=>r.json()).then(d=>document.getElementById('pwm-status').innerHTML=d.result)}";
  chunk += "function scanSPI(){document.getElementById('spi-status').innerHTML=(translations.scanning||'Scan...');";
  chunk += "fetch('/api/spi-scan').then(r=>r.json()).then(d=>document.getElementById('spi-status').innerHTML=d.info)}";
  chunk += "function listPartitions(){document.getElementById('partitions-results').innerHTML=(translations.scanning||'Scan...');";
  chunk += "fetch('/api/partitions-list').then(r=>r.json()).then(d=>document.getElementById('partitions-results').innerHTML=d.partitions)}";
  chunk += "function stressTest(){document.getElementById('stress-status').innerHTML=(translations.testing||'Test...');";
  chunk += "fetch('/api/stress-test').then(r=>r.json()).then(d=>document.getElementById('stress-status').innerHTML=d.result)}";
  
  // GPIO
  chunk += "function testAllGPIO(){document.getElementById('gpio-status').innerHTML=(translations.testing||'Test...');";
  chunk += "fetch('/api/test-gpio').then(r=>r.json()).then(data=>{let h='';";
  chunk += "data.results.forEach(g=>{h+='<div class=\"gpio-item '+(g.working?'gpio-ok':'gpio-fail')+'\">GPIO '+g.pin+'<br>'+(g.working?'OK':'FAIL')+'</div>'});";
  chunk += "document.getElementById('gpio-results').innerHTML=h;document.getElementById('gpio-status').innerHTML=(translations.completed||'Terminé')+' - '+data.results.length+' GPIO '+(translations.tested||'testés')})}";
  
  // WiFi
  chunk += "function scanWiFi(){document.getElementById('wifi-status').innerHTML=(translations.scanning||'Scan...');";
  chunk += "fetch('/api/wifi-scan').then(r=>r.json()).then(data=>{let h='';";
  chunk += "data.networks.forEach(n=>{let s=n.rssi>=-60?'🟢':n.rssi>=-70?'🟡':'🔴';";
  chunk += "h+='<div class=\"wifi-item\"><div style=\"display:flex;justify-content:space-between\"><div><strong>'+s+' '+n.ssid+'</strong><br><small>'+n.bssid+' | Ch'+n.channel+' | '+n.encryption+'</small></div>';";
  chunk += "h+='<div style=\"font-size:1.2em;font-weight:bold\">'+n.rssi+' dBm</div></div></div>'});";
  chunk += "document.getElementById('wifi-results').innerHTML=h;document.getElementById('wifi-status').innerHTML=data.networks.length+' '+(translations.networks||'réseaux')+' '+(translations.detected||'détectés')})}";
  
  // I2C
  chunk += "function scanI2C(){fetch('/api/i2c-scan').then(r=>r.json()).then(d=>{console.log('I2C scan',d.count,d.devices);location.reload();}).catch(err=>console.error('scanI2C',err));}";
  
  // Benchmarks
  chunk += "function runBenchmarks(){";
  chunk += "document.getElementById('cpu-bench').innerHTML=(translations.testing||'Test...');";
  chunk += "document.getElementById('mem-bench').innerHTML=(translations.testing||'Test...');";
  chunk += "fetch('/api/benchmark').then(r=>r.json()).then(data=>{";
  chunk += "document.getElementById('cpu-bench').innerHTML=data.cpu+' us';";
  chunk += "document.getElementById('mem-bench').innerHTML=data.memory+' us';";
  chunk += "document.getElementById('cpu-perf').innerHTML=data.cpuPerf.toFixed(2)+' MFLOPS';";
  chunk += "document.getElementById('mem-speed').innerHTML=data.memSpeed.toFixed(2)+' MB/s'})}";
  
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
  Serial.println("     Version 4.0.14 - FR/EN");
  Serial.println("     Optimise Arduino Core 3.3.2");
  Serial.println("===============================================\r\n");
  
  printPSRAMDiagnostic();
  
  // WiFi
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
  printWirelessDiagnostic();

  // ========== ROUTES SERVEUR ==========
  server.on("/", handleRoot);
  
  // **NOUVELLES ROUTES MULTILINGUES**
  server.on("/api/set-language", handleSetLanguage);
  server.on("/api/get-translations", handleGetTranslations);

  server.on("/api/overview", handleOverview);
  server.on("/api/wireless-status", handleWirelessStatus);

  // GPIO & WiFi
  server.on("/api/test-gpio", handleTestGPIO);
  server.on("/api/wifi-scan", handleWiFiScan);
  server.on("/api/ble-scan", handleBLEScan);
  server.on("/api/i2c-scan", handleI2CScan);
  
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
  server.on("/api/oled-pattern", handleOLEDPattern);
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
