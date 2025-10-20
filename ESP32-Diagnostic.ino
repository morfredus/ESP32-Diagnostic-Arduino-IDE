/*
 * DIAGNOSTIC COMPLET ESP32 - VERSION MULTILINGUE v2.8.6
 * Compatible: ESP32, ESP32-S2, ESP32-S3, ESP32-C3
 * Optimisé pour ESP32 Arduino Core 3.3.2
 * Carte testée: ESP32-S3 avec PSRAM OPI
 * Auteur: morfredus
 *
 * Nouveautés v2.8.6:
 * - Ajout d'une bannière fixe indiquant en direct l'état WiFi/Bluetooth sans bloquer l'interface.
 * - Les tests LED, NeoPixel et OLED appliquent automatiquement la configuration saisie avant leur première exécution tout en conservant le bouton Config pour des modifications ultérieures.

 * Nouveautés v2.8.5:
 * - Réécriture du script client pour lever toutes les erreurs JavaScript, restaurer la navigation par onglets et la bascule de langue.
 * - Ajout d'un gabarit JS avec export global automatique des fonctions et initialisation centralisée des événements.

 * Nouveautés v2.8.4:
 * - Ajustement des guillemets HTML générés par `app_script.h` pour éliminer les erreurs `operator""info` et `exponent has no digits` sous Arduino Core 3.3.2.
 * - Stabilisation des blocs de résultats ADC/GPIO/WiFi en conservant l'affichage inline sans conflit de compilation.
 *
 * Nouveautés v2.8.3:
 * - Correction de l'échappement du script embarqué pour supprimer les erreurs `stray '\'` à la compilation.
 * - Restauration de l'ensemble des handlers REST (WiFi, I2C, LEDs, NeoPixel, OLED, tests matériels, exports).
 * - API `/api/wireless-info` enrichie avec l'état Bluetooth et réponses JSON uniformément échappées.

 * Nouveautés v2.8.2:
 * - Ajout du fichier `app_script.h` embarquant la fonction `buildAppScript()`.
 * - Publication du script `/js/app.js` lors des exports afin d'éviter les erreurs "fonction manquante".
 *
 * Nouveautés v2.8.1:
 * - Restauration du script `/js/app.js` pour que les onglets et la bascule FR/EN répondent sans erreur.
 * - Externalisation du JavaScript de l'interface afin de fiabiliser la navigation avec le core 3.3.2.
 *
 * Nouveautés v2.7.1:
 * - Détection automatique de la disponibilité des en-têtes Bluetooth pour éviter les erreurs de compilation.
 * - Messages Bluetooth conservés pour guider l'utilisateur même sans pile logicielle intégrée.
 *
 * Nouveautés v2.7.0:
 * - Ajout d'un onglet "Sans fil" réunissant le WiFi et le Bluetooth
 * - Nouveau test automatique du contrôleur Bluetooth avec messages détaillés
 * - Informations WiFi enrichies et messages d'aide sans pop-up
 *
 * Nouveautés v2.6.3:
 * - Changement de langue immédiat sans rechargement manuel
 * - Harmonisation des libellés lumineux en français
 *
 * Nouveautés v2.6.2:
 * - Correction de l'affichage inline après reconfiguration OLED
 * - Stabilisation visuelle des zones de statut et rappel GPIO
 *
 * Nouveautés v2.6.1:
 * - Suppression des popups bloquantes sur l'interface web
 * - Ajout de zones de statut persistantes pour tous les tests interactifs
 * - Avertissement spécifique lors des campagnes de test GPIO
 *
 * Nouveautés v2.5:
 * - Traduction des exports (Français/Anglais)
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
#include <esp_flash.h>
#include <esp_heap_caps.h>
#include <esp_partition.h>
#include <soc/soc.h>
#include <soc/rtc.h>
#include <soc/soc_caps.h>
#include <esp_wifi.h>

#if __has_include(<esp_arduino_version.h>)
#include <esp_arduino_version.h>
#endif

#if __has_include(<esp_idf_version.h>)
#include <esp_idf_version.h>
#endif

#ifndef ESP_IDF_VERSION_VAL
#define ESP_IDF_VERSION_VAL(major, minor, patch) ((major) * 10000 + (minor) * 100 + (patch))
#endif

#ifndef ESP_IDF_VERSION
#define ESP_IDF_VERSION 0
#endif

#ifndef ESP_ARDUINO_VERSION_STR
#define ESP_ARDUINO_VERSION_STR "unknown"
#endif

#if ESP_IDF_VERSION >= ESP_IDF_VERSION_VAL(5, 4, 2)
#define HAS_WIFI_BAND_MODE_API 1
#else
#define HAS_WIFI_BAND_MODE_API 0
#endif

#if defined(ARDUINO_ARCH_ESP32) && defined(SOC_BT_SUPPORTED) && SOC_BT_SUPPORTED
  #if defined(__has_include)
    #if __has_include(<esp_bt.h>) && __has_include(<esp_bt_main.h>)
      #define HAS_BLUETOOTH_HEADERS 1
    #else
      #define HAS_BLUETOOTH_HEADERS 0
    #endif
  #else
    #define HAS_BLUETOOTH_HEADERS 1
  #endif

  #if HAS_BLUETOOTH_HEADERS && (defined(CONFIG_BT_ENABLED) || defined(CONFIG_BT_CONTROLLER_ENABLED) || defined(CONFIG_BT_BLUEDROID_ENABLED) || defined(CONFIG_BT_LE_CONTROLLER_ENABLED) || defined(CONFIG_BT_NIMBLE_ENABLED) || defined(CONFIG_ESP_HOSTED_ENABLE_BT_NIMBLE))
    #define HAS_NATIVE_BLUETOOTH 1
  #else
    #define HAS_NATIVE_BLUETOOTH 0
  #endif
#else
  #define HAS_NATIVE_BLUETOOTH 0
#endif

#if HAS_NATIVE_BLUETOOTH
#include <esp_bt.h>
#include <esp_bt_main.h>
#endif
#include <Wire.h>
#include <Adafruit_NeoPixel.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <vector>

// Configuration WiFi
#include "config.h"

// Système de traduction
#include "languages.h"

// JavaScript embarqué
#include "app_script.h"

// ========== CONFIGURATION ==========
#define DIAGNOSTIC_VERSION "2.8.6"
#define CUSTOM_LED_PIN -1
#define CUSTOM_LED_COUNT 1
#define ENABLE_I2C_SCAN true
#define MDNS_HOSTNAME "esp32-diagnostic"

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

struct BluetoothDiagnostics {
  bool compileEnabled;
  bool hardwareClassic;
  bool hardwareBLE;
  bool controllerInitialized;
  bool controllerEnabled;
  String controllerStatus;
  bool lastTestSuccess;
  String lastTestMessage;
  String availabilityHint;
};

BluetoothDiagnostics bluetoothInfo = {false, false, false, false, false, String(), false, String(), String()};

String jsonEscape(const String& input) {
  String output;
  output.reserve(input.length() + 4);
  for (size_t i = 0; i < input.length(); ++i) {
    char c = input[i];
    switch (c) {
      case '\\': output += "\\\\"; break;
      case '\"': output += "\\\""; break;
      case '\n': output += "\\n"; break;
      case '\r': output += "\\r"; break;
      case '\t': output += "\\t"; break;
      default:
        if (static_cast<uint8_t>(c) < 0x20) {
          char buf[7];
          sprintf(buf, "\\u%04x", static_cast<unsigned char>(c));
          output += buf;
        } else {
          output += c;
        }
    }
  }
  return output;
}

#if HAS_NATIVE_BLUETOOTH
String describeBluetoothControllerStatus(esp_bt_controller_status_t status) {
  switch (status) {
    case ESP_BT_CONTROLLER_STATUS_IDLE:
      return String(T().bluetooth_status_idle);
    case ESP_BT_CONTROLLER_STATUS_INITED:
      return String(T().bluetooth_status_inited);
    case ESP_BT_CONTROLLER_STATUS_ENABLED:
      return String(T().bluetooth_status_enabled);
    case ESP_BT_CONTROLLER_STATUS_ENABLING:
      return String(T().bluetooth_status_enabling);
    case ESP_BT_CONTROLLER_STATUS_DISABLING:
      return String(T().bluetooth_status_disabling);
    case ESP_BT_CONTROLLER_STATUS_UNINITIALIZED:
      return String(T().bluetooth_status_uninitialized);
    default:
      return String(T().unknown);
  }
}
#else
String describeBluetoothControllerStatus(int) {
  return String(T().bluetooth_disabled_build);
}
#endif

void updateBluetoothDerivedState() {
  if (!bluetoothInfo.lastTestMessage.length()) {
    bluetoothInfo.lastTestMessage = String(T().not_tested);
  }

  if (!bluetoothInfo.compileEnabled) {
    if (bluetoothInfo.hardwareClassic || bluetoothInfo.hardwareBLE) {
      bluetoothInfo.availabilityHint = String(T().bluetooth_enable_hint);
    } else {
      bluetoothInfo.availabilityHint = String(T().bluetooth_not_available);
    }
  } else if (!(bluetoothInfo.hardwareClassic || bluetoothInfo.hardwareBLE)) {
    bluetoothInfo.availabilityHint = String(T().bluetooth_not_available);
  } else {
    bluetoothInfo.availabilityHint = "";
  }
}

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
  String wifiHostname;
  int wifiChannel;
  String wifiMode;
  String wifiSleepMode;
  String wifiBand;
  String wifiBandMode;
  float wifiTxPowerDbm;
  int wifiTxPowerCode;
  bool wifiSupports5G;

  String gpioList;
  int totalGPIO;

  String sdkVersion;
  String idfVersion;
  String arduinoCoreVersion;
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

String describeWiFiModeLabel(wifi_mode_t mode) {
  switch (mode) {
    case WIFI_MODE_STA:
      return String(T().wifi_mode_sta);
    case WIFI_MODE_AP:
      return String(T().wifi_mode_ap);
    case WIFI_MODE_APSTA:
      return String(T().wifi_mode_apsta);
    case WIFI_MODE_NULL:
    default:
      return String(T().wifi_mode_null);
  }
}

String describeWiFiSleepLabel(wifi_ps_type_t sleepMode) {
  switch (sleepMode) {
    case WIFI_PS_NONE:
      return String(T().wifi_sleep_none);
    case WIFI_PS_MIN_MODEM:
      return String(T().wifi_sleep_min_modem);
    case WIFI_PS_MAX_MODEM:
      return String(T().wifi_sleep_max_modem);
    default:
      return String(T().unknown);
  }
}

String describeWiFiBandLabel(bool supports5G) {
#if defined(WIFI_BAND_5G)
  wifi_band_t band = WiFiGenericClass::getBand();
  switch (band) {
    case WIFI_BAND_5G:
      return String(T().wifi_band_5g);
    case WIFI_BAND_2G:
    default:
      return String(T().wifi_band_2g);
  }
#else
  (void)supports5G;
  return String(T().wifi_band_2g);
#endif
}

String describeWiFiBandModeLabel(bool supports5G) {
#if HAS_WIFI_BAND_MODE_API
  wifi_band_mode_t mode = WiFi.getBandMode();
  switch (mode) {
    case WIFI_BAND_MODE_AUTO:
      return String(T().wifi_band_mode_auto);
    case WIFI_BAND_MODE_5G_ONLY:
      return String(T().wifi_band_mode_5g);
    case WIFI_BAND_MODE_2G_ONLY:
    default:
      return String(T().wifi_band_mode_2g);
  }
#else
  (void)supports5G;
  return supports5G ? String(T().wifi_band_mode_auto) : String(T().wifi_band_mode_2g);
#endif
}

float wifiTxPowerToDbm(wifi_power_t power) {
  return static_cast<float>(power) / 4.0f;
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
    diagnosticData.i2cDevices = "Aucun";
  }
  Serial.printf("I2C: %d peripherique(s)\r\n", diagnosticData.i2cCount);
}

// ========== SCAN WIFI ==========
void scanWiFiNetworks() {
  Serial.println("\r\n=== SCAN WIFI ===");
  wifiNetworks.clear();
  
  int n = WiFi.scanNetworks();
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
    switch (auth) {
      case WIFI_AUTH_OPEN: net.encryption = "Ouvert"; break;
      case WIFI_AUTH_WPA2_PSK: net.encryption = "WPA2-PSK"; break;
      case WIFI_AUTH_WPA3_PSK: net.encryption = "WPA3-PSK"; break;
      default: net.encryption = "WPA/WPA2"; break;
    }
    
    wifiNetworks.push_back(net);
  }
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

// ========== OLED 0.96" ==========
void detectOLED() {
  Serial.println("\r\n=== DETECTION OLED ===");
  Wire.begin(I2C_SDA, I2C_SCL);
  Serial.printf("I2C: SDA=%d, SCL=%d\r\n", I2C_SDA, I2C_SCL);
  
  Wire.beginTransmission(SCREEN_ADDRESS);
  bool i2cDetected = (Wire.endTransmission() == 0);
  
  if(i2cDetected && oled.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    oledAvailable = true;
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
  oled.clearDisplay();
  for (int i = 0; i < SCREEN_HEIGHT; i += 4) {
    oled.drawLine(0, i, SCREEN_WIDTH, i, SSD1306_WHITE);
  }
  oled.display();
  delay(1500);
}

void oledStepDiagonals() {
  if (!oledAvailable) return;
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
  for (int x = 0; x < SCREEN_WIDTH - 20; x += 4) {
    oled.clearDisplay();
    oled.fillRect(x, 22, 20, 20, SSD1306_WHITE);
    oled.display();
    delay(20);
  }
}

void oledStepProgressBar() {
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

void oledStepScrollText() {
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

void oledStepFinalMessage() {
  if (!oledAvailable) return;
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

  bluetoothInfo.hardwareClassic = diagnosticData.hasBT;
  bluetoothInfo.hardwareBLE = diagnosticData.hasBLE;
#if HAS_NATIVE_BLUETOOTH
  bluetoothInfo.compileEnabled = true;
  esp_bt_controller_status_t btStatus = esp_bt_controller_get_status();
  bluetoothInfo.controllerStatus = describeBluetoothControllerStatus(btStatus);
  bluetoothInfo.controllerEnabled = (btStatus == ESP_BT_CONTROLLER_STATUS_ENABLED);
  bluetoothInfo.controllerInitialized = (btStatus == ESP_BT_CONTROLLER_STATUS_ENABLED ||
                                         btStatus == ESP_BT_CONTROLLER_STATUS_IDLE ||
                                         btStatus == ESP_BT_CONTROLLER_STATUS_INITED ||
                                         btStatus == ESP_BT_CONTROLLER_STATUS_ENABLING ||
                                         btStatus == ESP_BT_CONTROLLER_STATUS_DISABLING);
#else
  bluetoothInfo.compileEnabled = false;
  if (bluetoothInfo.hardwareClassic || bluetoothInfo.hardwareBLE) {
    bluetoothInfo.controllerStatus = String(T().bluetooth_disabled_build);
  } else {
    bluetoothInfo.controllerStatus = String(T().bluetooth_not_available);
  }
  bluetoothInfo.controllerEnabled = false;
  bluetoothInfo.controllerInitialized = false;
#endif
  updateBluetoothDerivedState();

  if (WiFi.status() == WL_CONNECTED) {
    diagnosticData.wifiSSID = WiFi.SSID();
    diagnosticData.wifiRSSI = WiFi.RSSI();
    diagnosticData.ipAddress = WiFi.localIP().toString();
  }

  const char* hostnamePtr = WiFi.getHostname();
  diagnosticData.wifiHostname = hostnamePtr ? String(hostnamePtr) : String();
  diagnosticData.wifiChannel = WiFi.channel();
  wifi_mode_t currentMode = WiFiGenericClass::getMode();
  diagnosticData.wifiMode = describeWiFiModeLabel(currentMode);
  wifi_ps_type_t sleepMode = WiFi.getSleep();
  diagnosticData.wifiSleepMode = describeWiFiSleepLabel(sleepMode);
  wifi_power_t txPower = WiFi.getTxPower();
  diagnosticData.wifiTxPowerCode = static_cast<int>(txPower);
  diagnosticData.wifiTxPowerDbm = wifiTxPowerToDbm(txPower);
#if defined(SOC_WIFI_SUPPORT_5G)
  diagnosticData.wifiSupports5G = SOC_WIFI_SUPPORT_5G;
#else
  diagnosticData.wifiSupports5G = false;
#endif
  diagnosticData.wifiBand = describeWiFiBandLabel(diagnosticData.wifiSupports5G);
  diagnosticData.wifiBandMode = describeWiFiBandModeLabel(diagnosticData.wifiSupports5G);

  diagnosticData.gpioList = getGPIOList();
  diagnosticData.totalGPIO = countGPIO();

  diagnosticData.sdkVersion = ESP.getSdkVersion();
  diagnosticData.idfVersion = esp_get_idf_version();
  diagnosticData.arduinoCoreVersion = String(ESP_ARDUINO_VERSION_STR);
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

void handleWirelessInfo() {
  collectDiagnosticInfo();

  bool wifiConnected = (WiFi.status() == WL_CONNECTED);
  String json = "{";
  json += "\"wifi\":{";
  json += "\"connected\":" + String(wifiConnected ? "true" : "false") + ",";
  json += "\"ssid\":\"" + jsonEscape(diagnosticData.wifiSSID) + "\",";
  json += "\"rssi\":" + String(diagnosticData.wifiRSSI) + ",";
  json += "\"quality\":\"" + jsonEscape(getWiFiSignalQuality()) + "\",";
  json += "\"ip\":\"" + jsonEscape(diagnosticData.ipAddress) + "\",";
  json += "\"subnet\":\"" + jsonEscape(WiFi.subnetMask().toString()) + "\",";
  json += "\"gateway\":\"" + jsonEscape(WiFi.gatewayIP().toString()) + "\",";
  json += "\"dns\":\"" + jsonEscape(WiFi.dnsIP().toString()) + "\",";
  json += "\"channel\":" + String(diagnosticData.wifiChannel) + ",";
  json += "\"mode\":\"" + jsonEscape(diagnosticData.wifiMode) + "\",";
  json += "\"sleep\":\"" + jsonEscape(diagnosticData.wifiSleepMode) + "\",";
  json += "\"band\":\"" + jsonEscape(diagnosticData.wifiBand) + "\",";
  json += "\"band_mode\":\"" + jsonEscape(diagnosticData.wifiBandMode) + "\",";
  json += "\"tx_power_dbm\":" + String(diagnosticData.wifiTxPowerDbm, 2) + ",";
  json += "\"tx_power_code\":" + String(diagnosticData.wifiTxPowerCode) + ",";
  json += "\"supports_5g\":" + String(diagnosticData.wifiSupports5G ? "true" : "false") + ",";
  json += "\"hostname\":\"" + jsonEscape(diagnosticData.wifiHostname) + "\"";
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

void handleWiFiScan() {
  scanWiFiNetworks();

  String json = "{\"networks\":[";
  for (size_t i = 0; i < wifiNetworks.size(); ++i) {
    if (i > 0) json += ",";
    json += "{\"ssid\":\"" + jsonEscape(wifiNetworks[i].ssid) + "\",";
    json += "\"rssi\":" + String(wifiNetworks[i].rssi) + ",";
    json += "\"channel\":" + String(wifiNetworks[i].channel) + ",";
    json += "\"encryption\":\"" + jsonEscape(wifiNetworks[i].encryption) + "\",";
    json += "\"bssid\":\"" + jsonEscape(wifiNetworks[i].bssid) + "\"}";
  }
  json += "]}";

  server.send(200, "application/json", json);
}

void handleI2CScan() {
  scanI2C();
  String json = "{\"count\":" + String(diagnosticData.i2cCount) + ",\"devices\":\"" + jsonEscape(diagnosticData.i2cDevices) + "\"}";
  server.send(200, "application/json", json);
}

void handleBuiltinLEDConfig() {
  if (server.hasArg("gpio")) {
    int newGPIO = server.arg("gpio").toInt();
    if (newGPIO >= 0 && newGPIO <= 48) {
      BUILTIN_LED_PIN = newGPIO;
      resetBuiltinLEDTest();
      String json = "{\"success\":true,\"message\":\"" + jsonEscape("LED GPIO " + String(BUILTIN_LED_PIN)) + "\"}";
      server.send(200, "application/json", json);
      return;
    }
  }

  server.send(400, "application/json", "{\"success\":false}");
}

void handleBuiltinLEDTest() {
  resetBuiltinLEDTest();
  testBuiltinLED();
  String json = "{\"success\":" + String(builtinLedAvailable ? "true" : "false") + ",\"result\":\"" + jsonEscape(builtinLedTestResult) + "\"}";
  server.send(200, "application/json", json);
}

void handleBuiltinLEDControl() {
  if (!server.hasArg("action")) {
    server.send(400, "application/json", "{\"success\":false}");
    return;
  }

  if (BUILTIN_LED_PIN == -1) {
    server.send(400, "application/json", "{\"success\":false,\"message\":\"LED non configuree\"}");
    return;
  }

  String action = server.arg("action");
  pinMode(BUILTIN_LED_PIN, OUTPUT);
  String message;

  if (action == "blink") {
    for (int i = 0; i < 5; ++i) {
      digitalWrite(BUILTIN_LED_PIN, HIGH);
      delay(200);
      digitalWrite(BUILTIN_LED_PIN, LOW);
      delay(200);
    }
    message = "Clignotement OK";
  } else if (action == "fade") {
    for (int i = 0; i <= 255; i += 5) {
      analogWrite(BUILTIN_LED_PIN, i);
      delay(10);
    }
    for (int i = 255; i >= 0; i -= 5) {
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

  String json = "{\"success\":true,\"message\":\"" + jsonEscape(message) + "\"}";
  server.send(200, "application/json", json);
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
      String json = "{\"success\":true,\"message\":\"" + jsonEscape("Config GPIO " + String(LED_PIN)) + "\"}";
      server.send(200, "application/json", json);
      return;
    }
  }

  server.send(400, "application/json", "{\"success\":false}");
}

void handleNeoPixelTest() {
  resetNeoPixelTest();
  testNeoPixel();
  String json = "{\"success\":" + String(neopixelAvailable ? "true" : "false") + ",\"result\":\"" + jsonEscape(neopixelTestResult) + "\"}";
  server.send(200, "application/json", json);
}

void handleNeoPixelPattern() {
  if (!server.hasArg("pattern")) {
    server.send(400, "application/json", "{\"success\":false}");
    return;
  }

  if (!strip) {
    server.send(400, "application/json", "{\"success\":false,\"message\":\"NeoPixel non init\"}");
    return;
  }

  String pattern = server.arg("pattern");
  String message;

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

  String json = "{\"success\":true,\"message\":\"" + jsonEscape(message) + "\"}";
  server.send(200, "application/json", json);
}

void handleNeoPixelColor() {
  if (!strip || !server.hasArg("r") || !server.hasArg("g") || !server.hasArg("b")) {
    server.send(400, "application/json", "{\"success\":false}");
    return;
  }

  int r = server.arg("r").toInt();
  int g = server.arg("g").toInt();
  int b = server.arg("b").toInt();

  strip->fill(strip->Color(r, g, b));
  strip->show();
  neopixelTested = false;

  String json = "{\"success\":true,\"message\":\"" + jsonEscape("RGB(" + String(r) + "," + String(g) + "," + String(b) + ")") + "\"}";
  server.send(200, "application/json", json);
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
      String json = "{\"success\":true,\"message\":\"" + jsonEscape("I2C reconfigure: SDA:" + String(I2C_SDA) + " SCL:" + String(I2C_SCL)) + "\"}";
      server.send(200, "application/json", json);
      return;
    }
  }

  server.send(400, "application/json", "{\"success\":false,\"message\":\"Pins invalides\"}");
}

void handleOLEDTest() {
  resetOLEDTest();
  testOLED();
  String json = "{\"success\":" + String(oledAvailable ? "true" : "false") + ",\"result\":\"" + jsonEscape(oledTestResult) + "\"}";
  server.send(200, "application/json", json);
}

void handleOLEDStep() {
  if (!server.hasArg("step")) {
    server.send(400, "application/json", "{\"success\":false,\"message\":\"" + String(T().oled_step_unknown) + "\"}");
    return;
  }

  if (!oledAvailable) {
    server.send(200, "application/json", "{\"success\":false,\"message\":\"" + String(T().oled_step_unavailable) + "\"}");
    return;
  }

  String stepId = server.arg("step");
  bool ok = performOLEDStep(stepId);
  if (!ok) {
    server.send(400, "application/json", "{\"success\":false,\"message\":\"" + String(T().oled_step_unknown) + "\"}");
    return;
  }

  String label = getOLEDStepLabel(stepId);
  String json = "{\"success\":true,\"message\":\"" + jsonEscape(String(T().oled_step_executed_prefix) + " " + label) + "\"}";
  server.send(200, "application/json", json);
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
  for (size_t i = 0; i < adcReadings.size(); ++i) {
    if (i > 0) json += ",";
    json += "{\"pin\":" + String(adcReadings[i].pin) + ",\"raw\":" + String(adcReadings[i].rawValue) + ",\"voltage\":" + String(adcReadings[i].voltage, 2) + "}";
  }
  json += "],\"result\":\"" + jsonEscape(adcTestResult) + "\"}";

  server.send(200, "application/json", json);
}

void handleTouchTest() {
  testTouchPads();

  String json = "{\"readings\":[";
  for (size_t i = 0; i < touchReadings.size(); ++i) {
    if (i > 0) json += ",";
    json += "{\"pin\":" + String(touchReadings[i].pin) + ",\"value\":" + String(touchReadings[i].value) + "}";
  }
  json += "],\"result\":\"" + jsonEscape(touchTestResult) + "\"}";

  server.send(200, "application/json", json);
}

void handlePWMTest() {
  testPWM();
  String json = "{\"result\":\"" + jsonEscape(pwmTestResult) + "\"}";
  server.send(200, "application/json", json);
}

void handleSPIScan() {
  scanSPI();
  server.send(200, "application/json", "{\"info\":\"" + jsonEscape(spiInfo) + "\"}");
}

void handlePartitionsList() {
  listPartitions();
  server.send(200, "application/json", "{\"partitions\":\"" + jsonEscape(partitionsInfo) + "\"}");
}

void handleStressTest() {
  memoryStressTest();
  server.send(200, "application/json", "{\"result\":\"" + jsonEscape(stressTestResult) + "\"}");
}

void handleBenchmark() {
  unsigned long cpuTime = benchmarkCPU();
  unsigned long memTime = benchmarkMemory();

  diagnosticData.cpuBenchmark = cpuTime;
  diagnosticData.memBenchmark = memTime;

  String json = "{\"cpu\":" + String(cpuTime) + ",\"memory\":" + String(memTime);
  if (cpuTime > 0) {
    json += ",\"cpuPerf\":" + String(100000.0 / cpuTime, 2);
  } else {
    json += ",\"cpuPerf\":0";
  }
  if (memTime > 0) {
    json += ",\"memSpeed\":" + String((10000 * sizeof(int) * 2) / (float)memTime, 2);
  } else {
    json += ",\"memSpeed\":0";
  }
  json += "}";

  server.send(200, "application/json", json);
}

void handleMemoryDetails() {
  collectDetailedMemory();

  String json = "{\"flash\":{\"real\":" + String(detailedMemory.flashSizeReal) + ",\"chip\":" + String(detailedMemory.flashSizeChip) + "},";
  json += "\"psram\":{\"available\":" + String(detailedMemory.psramAvailable ? "true" : "false") + ",";
  json += "\"configured\":" + String(detailedMemory.psramConfigured ? "true" : "false") + ",";
  json += "\"supported\":" + String(detailedMemory.psramBoardSupported ? "true" : "false") + ",";
  json += "\"type\":\"" + jsonEscape(String(detailedMemory.psramType ? detailedMemory.psramType : "Inconnu")) + "\",";
  json += "\"total\":" + String(detailedMemory.psramTotal) + ",\"free\":" + String(detailedMemory.psramFree) + "},";
  json += "\"sram\":{\"total\":" + String(detailedMemory.sramTotal) + ",\"free\":" + String(detailedMemory.sramFree) + "},";
  json += "\"fragmentation\":" + String(detailedMemory.fragmentationPercent, 1) + ",\"status\":\"" + jsonEscape(detailedMemory.memoryStatus) + "\"}";

  server.send(200, "application/json", json);
}

void handleBluetoothTest() {
  bool success = false;
  String message;

#if HAS_NATIVE_BLUETOOTH
  esp_bt_controller_status_t status = esp_bt_controller_get_status();
  esp_err_t err = ESP_OK;

  if (status == ESP_BT_CONTROLLER_STATUS_UNINITIALIZED) {
    esp_bt_controller_config_t bt_cfg = BT_CONTROLLER_INIT_CONFIG_DEFAULT();
    err = esp_bt_controller_init(&bt_cfg);
    if (err != ESP_OK && err != ESP_ERR_INVALID_STATE) {
      message = String(T().bluetooth_init_failed) + " (" + String(esp_err_to_name(err)) + ")";
    }
    status = esp_bt_controller_get_status();
  }

  if (!message.length() && status != ESP_BT_CONTROLLER_STATUS_ENABLED) {
    err = esp_bt_controller_enable(ESP_BT_MODE_BTDM);
    if (err != ESP_OK && err != ESP_ERR_INVALID_STATE) {
      message = String(T().bluetooth_enable_failed) + " (" + String(esp_err_to_name(err)) + ")";
    }
    status = esp_bt_controller_get_status();
  }

  if (!message.length()) {
    success = (status == ESP_BT_CONTROLLER_STATUS_ENABLED);
    message = success ? String(T().bluetooth_test_success) : describeBluetoothControllerStatus(status);
  }
#else
  message = String(T().bluetooth_test_not_compiled);
#endif

  bluetoothInfo.lastTestSuccess = success;
  bluetoothInfo.lastTestMessage = message;
  updateBluetoothDerivedState();
  collectDiagnosticInfo();

  String json = "{\"success\":" + String(success ? "true" : "false") + ",\"message\":\"" + jsonEscape(message) + "\"}";
  server.send(200, "application/json", json);
}

void handleExportTXT() {
  collectDiagnosticInfo();
  collectDetailedMemory();

  bool wifiConnected = (WiFi.status() == WL_CONNECTED);
  String wifiSSID = wifiConnected ? diagnosticData.wifiSSID : String("-");
  String wifiRSSI = wifiConnected ? String(diagnosticData.wifiRSSI) + " dBm" : String("-");
  String wifiQuality = wifiConnected ? getWiFiSignalQuality() : String("-");
  String wifiIP = wifiConnected ? diagnosticData.ipAddress : String("-");
  String wifiSubnet = wifiConnected ? WiFi.subnetMask().toString() : String("-");
  String wifiGateway = wifiConnected ? WiFi.gatewayIP().toString() : String("-");
  String wifiDNS = wifiConnected ? WiFi.dnsIP().toString() : String("-");
  String wifiHostname = diagnosticData.wifiHostname.length() ? diagnosticData.wifiHostname : String("-");
  String wifiChannel = diagnosticData.wifiChannel > 0 ? String(diagnosticData.wifiChannel) : String("-");
  String wifiMode = diagnosticData.wifiMode.length() ? diagnosticData.wifiMode : String("-");
  String wifiSleep = diagnosticData.wifiSleepMode.length() ? diagnosticData.wifiSleepMode : String("-");
  String wifiBand = diagnosticData.wifiBand.length() ? diagnosticData.wifiBand : String("-");
  String wifiBandMode = diagnosticData.wifiBandMode.length() ? diagnosticData.wifiBandMode : String("-");
  String wifiTxPower = String(diagnosticData.wifiTxPowerDbm, 1) + " dBm (" + String(diagnosticData.wifiTxPowerCode) + ")";

  String txt = "========================================\r\n";
  txt += String(T().title) + " " + String(T().version) + String(DIAGNOSTIC_VERSION) + "\r\n";
  txt += "========================================\r\n\r\n";

  txt += "=== CHIP ===\r\n";
  txt += String(T().model) + ": " + diagnosticData.chipModel + " " + String(T().revision) + diagnosticData.chipRevision + "\r\n";
  txt += "CPU: " + String(diagnosticData.cpuCores) + " " + String(T().cores) + " @ " + String(diagnosticData.cpuFreqMHz) + " MHz\r\n";
  txt += "MAC WiFi: " + diagnosticData.macAddress + "\r\n";
  txt += String(T().sdk_version) + ": " + diagnosticData.sdkVersion + "\r\n";
  txt += String(T().idf_version) + ": " + diagnosticData.idfVersion + "\r\n";
  txt += String(T().arduino_core_version) + ": " + diagnosticData.arduinoCoreVersion + "\r\n";
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
    txt += " - " + String(T().supported_not_enabled) + "\r\n";
  } else {
    txt += " - " + String(T().not_detected) + "\r\n";
  }
  txt += String(T().memory_fragmentation) + ": " + String(detailedMemory.fragmentationPercent, 1) + "%\r\n";
  txt += "SRAM: " + String(detailedMemory.sramTotal / 1024.0, 2) + " KB (" + String(T().free) + ": " + String(detailedMemory.sramFree / 1024.0, 2) + " KB)\r\n\r\n";

  txt += "=== WIFI ===\r\n";
  txt += String(T().connected_ssid) + ": " + wifiSSID + "\r\n";
  txt += String(T().signal_power) + ": " + wifiRSSI + "\r\n";
  txt += String(T().signal_quality) + ": " + wifiQuality + "\r\n";
  txt += String(T().ip_address) + ": " + wifiIP + "\r\n";
  txt += String(T().subnet_mask) + ": " + wifiSubnet + "\r\n";
  txt += String(T().gateway) + ": " + wifiGateway + "\r\n";
  txt += String(T().dns) + ": " + wifiDNS + "\r\n";
  txt += String(T().wifi_channel) + ": " + wifiChannel + "\r\n";
  txt += String(T().wifi_mode) + ": " + wifiMode + "\r\n";
  txt += String(T().wifi_sleep) + ": " + wifiSleep + "\r\n";
  txt += String(T().wifi_band) + ": " + wifiBand + "\r\n";
  txt += String(T().wifi_band_mode) + ": " + wifiBandMode + "\r\n";
  txt += String(T().wifi_tx_power) + ": " + wifiTxPower + "\r\n";
  txt += String(T().wifi_hostname) + ": " + wifiHostname + "\r\n\r\n";

  txt += "=== GPIO ===\r\n";
  txt += String(T().total_gpio) + ": " + String(diagnosticData.totalGPIO) + " " + String(T().pins) + "\r\n";
  txt += String(T().gpio_list) + ": " + diagnosticData.gpioList + "\r\n\r\n";

  txt += "=== " + String(T().i2c_peripherals) + " ===\r\n";
  txt += String(T().device_count) + ": " + String(diagnosticData.i2cCount) + " - " + diagnosticData.i2cDevices + "\r\n";
  txt += "SPI: " + spiInfo + "\r\n\r\n";

  txt += "=== " + String(T().test) + " ===\r\n";
  txt += String(T().builtin_led) + ": " + builtinLedTestResult + "\r\n";
  txt += "NeoPixel: " + neopixelTestResult + "\r\n";
  txt += "OLED: " + oledTestResult + "\r\n";
  txt += "ADC: " + adcTestResult + "\r\n";
  txt += "Touch: " + touchTestResult + "\r\n";
  txt += "PWM: " + pwmTestResult + "\r\n\r\n";

  txt += "=== " + String(T().performance_bench) + " ===\r\n";
  if (diagnosticData.cpuBenchmark > 0) {
    txt += "CPU: " + String(diagnosticData.cpuBenchmark) + " us (" + String(100000.0 / diagnosticData.cpuBenchmark, 2) + " MFLOPS)\r\n";
    txt += String(T().memory_benchmark) + ": " + String(diagnosticData.memBenchmark) + " us\r\n";
  } else {
    txt += String(T().not_tested) + "\r\n";
  }
  txt += "Stress test: " + stressTestResult + "\r\n\r\n";

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

  server.sendHeader("Content-Disposition", "attachment; filename=esp32_diagnostic_v" + String(DIAGNOSTIC_VERSION) + ".txt");
  server.send(200, "text/plain; charset=utf-8", txt);
}

void handleExportJSON() {
  collectDiagnosticInfo();
  collectDetailedMemory();

  bool wifiConnected = (WiFi.status() == WL_CONNECTED);
  String wifiSSID = wifiConnected ? diagnosticData.wifiSSID : String("-");
  String wifiIP = wifiConnected ? diagnosticData.ipAddress : String("-");
  String wifiSubnet = wifiConnected ? WiFi.subnetMask().toString() : String("-");
  String wifiGateway = wifiConnected ? WiFi.gatewayIP().toString() : String("-");
  String wifiDNS = wifiConnected ? WiFi.dnsIP().toString() : String("-");
  String wifiQuality = wifiConnected ? getWiFiSignalQuality() : String("-");
  String flashType = getFlashType();
  String flashSpeed = getFlashSpeed();
  String psramType = detailedMemory.psramType ? String(detailedMemory.psramType) : String("Inconnu");
  String resetReason = getResetReason();
  String wifiHostname = diagnosticData.wifiHostname.length() ? diagnosticData.wifiHostname : String("-");

  String json = "{";
  json += "\"chip\":{";
  json += "\"model\":\"" + jsonEscape(diagnosticData.chipModel) + "\",";
  json += "\"revision\":\"" + jsonEscape(diagnosticData.chipRevision) + "\",";
  json += "\"cores\":" + String(diagnosticData.cpuCores) + ",";
  json += "\"freq_mhz\":" + String(diagnosticData.cpuFreqMHz) + ",";
  json += "\"mac\":\"" + jsonEscape(diagnosticData.macAddress) + "\",";
  json += "\"sdk\":\"" + jsonEscape(diagnosticData.sdkVersion) + "\",";
  json += "\"idf\":\"" + jsonEscape(diagnosticData.idfVersion) + "\",";
  json += "\"arduino_core\":\"" + jsonEscape(diagnosticData.arduinoCoreVersion) + "\"";
  if (diagnosticData.temperature != -999) {
    json += ",\"temperature\":" + String(diagnosticData.temperature, 1);
  }
  json += "},";

  json += "\"memory\":{";
  json += "\"flash_real_mb\":" + String(detailedMemory.flashSizeReal / 1048576.0, 2) + ",";
  json += "\"flash_config_mb\":" + String(detailedMemory.flashSizeChip / 1048576.0, 2) + ",";
  json += "\"flash_type\":\"" + jsonEscape(flashType) + "\",";
  json += "\"flash_speed\":\"" + jsonEscape(flashSpeed) + "\",";
  json += "\"psram_mb\":" + String(detailedMemory.psramTotal / 1048576.0, 2) + ",";
  json += "\"psram_free_mb\":" + String(detailedMemory.psramFree / 1048576.0, 2) + ",";
  json += "\"psram_available\":" + String(detailedMemory.psramAvailable ? "true" : "false") + ",";
  json += "\"psram_supported\":" + String(detailedMemory.psramBoardSupported ? "true" : "false") + ",";
  json += "\"psram_type\":\"" + jsonEscape(psramType) + "\",";
  json += "\"sram_kb\":" + String(detailedMemory.sramTotal / 1024.0, 2) + ",";
  json += "\"sram_free_kb\":" + String(detailedMemory.sramFree / 1024.0, 2) + ",";
  json += "\"fragmentation\":" + String(detailedMemory.fragmentationPercent, 1) + ",";
  json += "\"status\":\"" + jsonEscape(detailedMemory.memoryStatus) + "\"";
  json += "},";

  json += "\"wifi\":{";
  json += "\"connected\":" + String(wifiConnected ? "true" : "false") + ",";
  json += "\"ssid\":\"" + jsonEscape(wifiSSID) + "\",";
  json += "\"rssi\":" + String(diagnosticData.wifiRSSI) + ",";
  json += "\"quality\":\"" + jsonEscape(wifiQuality) + "\",";
  json += "\"ip\":\"" + jsonEscape(wifiIP) + "\",";
  json += "\"subnet\":\"" + jsonEscape(wifiSubnet) + "\",";
  json += "\"gateway\":\"" + jsonEscape(wifiGateway) + "\",";
  json += "\"dns\":\"" + jsonEscape(wifiDNS) + "\",";
  json += "\"channel\":" + String(diagnosticData.wifiChannel) + ",";
  json += "\"mode\":\"" + jsonEscape(diagnosticData.wifiMode) + "\",";
  json += "\"sleep\":\"" + jsonEscape(diagnosticData.wifiSleepMode) + "\",";
  json += "\"band\":\"" + jsonEscape(diagnosticData.wifiBand) + "\",";
  json += "\"band_mode\":\"" + jsonEscape(diagnosticData.wifiBandMode) + "\",";
  json += "\"tx_power_dbm\":" + String(diagnosticData.wifiTxPowerDbm, 2) + ",";
  json += "\"tx_power_code\":" + String(diagnosticData.wifiTxPowerCode) + ",";
  json += "\"supports_5g\":" + String(diagnosticData.wifiSupports5G ? "true" : "false") + ",";
  json += "\"hostname\":\"" + jsonEscape(wifiHostname) + "\"";
  json += "},";

  json += "\"gpio\":{";
  json += "\"total\":" + String(diagnosticData.totalGPIO) + ",";
  json += "\"list\":\"" + jsonEscape(diagnosticData.gpioList) + "\"";
  json += "},";
  json += "\"bluetooth\":{";
  json += "\"compile_enabled\":" + String(bluetoothInfo.compileEnabled ? "true" : "false") + ",";
  json += "\"classic\":" + String(bluetoothInfo.hardwareClassic ? "true" : "false") + ",";
  json += "\"ble\":" + String(bluetoothInfo.hardwareBLE ? "true" : "false") + ",";
  json += "\"controller\":\"" + jsonEscape(bluetoothInfo.controllerStatus) + "\",";
  json += "\"controller_enabled\":" + String(bluetoothInfo.controllerEnabled ? "true" : "false") + ",";
  json += "\"controller_initialized\":" + String(bluetoothInfo.controllerInitialized ? "true" : "false") + ",";
  json += "\"last_test_success\":" + String(bluetoothInfo.lastTestSuccess ? "true" : "false") + ",";
  json += "\"last_test_message\":\"" + jsonEscape(bluetoothInfo.lastTestMessage) + "\",";
  json += "\"hint\":\"" + jsonEscape(bluetoothInfo.availabilityHint) + "\"";
  json += "},";

  json += "\"peripherals\":{";
  json += "\"i2c_count\":" + String(diagnosticData.i2cCount) + ",";
  json += "\"i2c_devices\":\"" + jsonEscape(diagnosticData.i2cDevices) + "\",";
  json += "\"spi\":\"" + jsonEscape(spiInfo) + "\"";
  json += "},";

  json += "\"hardware_tests\":{";
  json += "\"builtin_led\":\"" + jsonEscape(builtinLedTestResult) + "\",";
  json += "\"neopixel\":\"" + jsonEscape(neopixelTestResult) + "\",";
  json += "\"oled\":\"" + jsonEscape(oledTestResult) + "\",";
  json += "\"adc\":\"" + jsonEscape(adcTestResult) + "\",";
  json += "\"touch\":\"" + jsonEscape(touchTestResult) + "\",";
  json += "\"pwm\":\"" + jsonEscape(pwmTestResult) + "\"";
  json += "},";

  json += "\"performance\":{";
  if (diagnosticData.cpuBenchmark > 0) {
    json += "\"cpu_us\":" + String(diagnosticData.cpuBenchmark) + ",";
    json += "\"cpu_mflops\":" + String(100000.0 / diagnosticData.cpuBenchmark, 2) + ",";
    json += "\"memory_us\":" + String(diagnosticData.memBenchmark) + ",";
  } else {
    json += "\"benchmarks\":\"not_run\",";
  }
  json += "\"stress_test\":\"" + jsonEscape(stressTestResult) + "\"";
  json += "},";

  json += "\"system\":{";
  json += "\"uptime_ms\":" + String(diagnosticData.uptime) + ",";
  json += "\"reset_reason\":\"" + jsonEscape(resetReason) + "\",";
  json += "\"language\":\"" + String(currentLanguage == LANG_FR ? "fr" : "en") + "\"";
  json += "}";

  json += "}";

  server.sendHeader("Content-Disposition", "attachment; filename=esp32_diagnostic_v" + String(DIAGNOSTIC_VERSION) + ".json");
  server.send(200, "application/json", json);
}

void handleExportCSV() {
  collectDiagnosticInfo();
  collectDetailedMemory();

  bool wifiConnected = (WiFi.status() == WL_CONNECTED);
  String wifiSSID = wifiConnected ? diagnosticData.wifiSSID : String("-");
  String wifiRSSI = wifiConnected ? String(diagnosticData.wifiRSSI) + " dBm" : String("-");
  String wifiQuality = wifiConnected ? getWiFiSignalQuality() : String("-");
  String wifiIP = wifiConnected ? diagnosticData.ipAddress : String("-");
  String wifiSubnet = wifiConnected ? WiFi.subnetMask().toString() : String("-");
  String wifiGateway = wifiConnected ? WiFi.gatewayIP().toString() : String("-");
  String wifiDNS = wifiConnected ? WiFi.dnsIP().toString() : String("-");
  String wifiChannel = diagnosticData.wifiChannel > 0 ? String(diagnosticData.wifiChannel) : String("-");
  String wifiMode = diagnosticData.wifiMode.length() ? diagnosticData.wifiMode : String("-");
  String wifiSleep = diagnosticData.wifiSleepMode.length() ? diagnosticData.wifiSleepMode : String("-");
  String wifiBand = diagnosticData.wifiBand.length() ? diagnosticData.wifiBand : String("-");
  String wifiBandMode = diagnosticData.wifiBandMode.length() ? diagnosticData.wifiBandMode : String("-");
  String wifiTxPower = String(diagnosticData.wifiTxPowerDbm, 1) + " dBm (" + String(diagnosticData.wifiTxPowerCode) + ")";
  String wifiHostname = diagnosticData.wifiHostname.length() ? diagnosticData.wifiHostname : String("-");
  String resetReason = getResetReason();

  String csv = String(T().category) + "," + String(T().parameter) + "," + String(T().value) + "\r\n";

  csv += "Chip," + String(T().model) + "," + diagnosticData.chipModel + "\r\n";
  csv += "Chip," + String(T().revision) + "," + diagnosticData.chipRevision + "\r\n";
  csv += "Chip,CPU " + String(T().cores) + "," + String(diagnosticData.cpuCores) + "\r\n";
  csv += "Chip," + String(T().frequency) + " MHz," + String(diagnosticData.cpuFreqMHz) + "\r\n";
  csv += "Chip,MAC," + diagnosticData.macAddress + "\r\n";
  csv += "Chip," + String(T().sdk_version) + "," + diagnosticData.sdkVersion + "\r\n";
  csv += "Chip," + String(T().idf_version) + "," + diagnosticData.idfVersion + "\r\n";
  csv += "Chip," + String(T().arduino_core_version) + "," + diagnosticData.arduinoCoreVersion + "\r\n";
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

  String ssidEscaped = wifiSSID; ssidEscaped.replace("\"", "'");
  String hostnameEscaped = wifiHostname; hostnameEscaped.replace("\"", "'");
  String i2cDevices = diagnosticData.i2cDevices; i2cDevices.replace("\"", "'");

  csv += "WiFi,SSID,\"" + ssidEscaped + "\"\r\n";
  csv += "WiFi,RSSI," + wifiRSSI + "\r\n";
  csv += "WiFi," + String(T().signal_quality) + "," + wifiQuality + "\r\n";
  csv += "WiFi," + String(T().ip_address) + "," + wifiIP + "\r\n";
  csv += "WiFi," + String(T().subnet_mask) + "," + wifiSubnet + "\r\n";
  csv += "WiFi," + String(T().gateway) + "," + wifiGateway + "\r\n";
  csv += "WiFi," + String(T().dns) + "," + wifiDNS + "\r\n";
  csv += "WiFi," + String(T().wifi_channel) + "," + wifiChannel + "\r\n";
  csv += "WiFi," + String(T().wifi_mode) + "," + wifiMode + "\r\n";
  csv += "WiFi," + String(T().wifi_sleep) + "," + wifiSleep + "\r\n";
  csv += "WiFi," + String(T().wifi_band) + "," + wifiBand + "\r\n";
  csv += "WiFi," + String(T().wifi_band_mode) + "," + wifiBandMode + "\r\n";
  csv += "WiFi," + String(T().wifi_tx_power) + "," + wifiTxPower + "\r\n";
  csv += "WiFi," + String(T().wifi_hostname) + ",\"" + hostnameEscaped + "\"\r\n";

  csv += "GPIO," + String(T().total_gpio) + "," + String(diagnosticData.totalGPIO) + "\r\n";
  csv += String(T().i2c_peripherals) + "," + String(T().device_count) + "," + String(diagnosticData.i2cCount) + "\r\n";
  csv += String(T().i2c_peripherals) + "," + String(T().devices) + ",\"" + i2cDevices + "\"\r\n";
  csv += "Interfaces,SPI," + spiInfo + "\r\n";

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
  csv += "System," + String(T().last_reset) + "," + resetReason + "\r\n";

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
  html += "<div class='row'><b>" + String(T().arduino_core_version) + ":</b><span>" + diagnosticData.arduinoCoreVersion + "</span></div>";
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
  html += "<div class='row'><b>" + String(T().wifi_channel) + ":</b><span>" + (diagnosticData.wifiChannel > 0 ? String("Ch ") + String(diagnosticData.wifiChannel) : String("—")) + "</span></div>";
  html += "<div class='row'><b>" + String(T().wifi_mode) + ":</b><span>" + (diagnosticData.wifiMode.length() ? diagnosticData.wifiMode : String("—")) + "</span></div>";
  html += "<div class='row'><b>" + String(T().wifi_sleep) + ":</b><span>" + (diagnosticData.wifiSleepMode.length() ? diagnosticData.wifiSleepMode : String("—")) + "</span></div>";
  html += "<div class='row'><b>" + String(T().wifi_band) + ":</b><span>" + (diagnosticData.wifiBand.length() ? diagnosticData.wifiBand : String("—")) + "</span></div>";
  html += "<div class='row'><b>" + String(T().wifi_band_mode) + ":</b><span>" + (diagnosticData.wifiBandMode.length() ? diagnosticData.wifiBandMode : String("—")) + "</span></div>";
  html += "<div class='row'><b>" + String(T().wifi_tx_power) + ":</b><span>" + String(diagnosticData.wifiTxPowerDbm, 1) + " dBm (" + String(diagnosticData.wifiTxPowerCode) + ")</span></div>";
  html += "<div class='row'><b>" + String(T().wifi_hostname) + ":</b><span>" + (diagnosticData.wifiHostname.length() ? diagnosticData.wifiHostname : String("—")) + "</span></div>";
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

void handleGetTranslations() {
  // Envoie toutes les traductions en JSON pour mise à jour dynamique
  String json = "{";
  json += "\"title\":\"" + String(T().title) + "\",";
  json += "\"nav_overview\":\"" + String(T().nav_overview) + "\",";
  json += "\"nav_leds\":\"" + String(T().nav_leds) + "\",";
  json += "\"nav_screens\":\"" + String(T().nav_screens) + "\",";
  json += "\"nav_tests\":\"" + String(T().nav_tests) + "\",";
  json += "\"nav_gpio\":\"" + String(T().nav_gpio) + "\",";
  json += "\"nav_wifi\":\"" + String(T().nav_wifi) + "\",";
  json += "\"nav_benchmark\":\"" + String(T().nav_benchmark) + "\",";
  json += "\"nav_export\":\"" + String(T().nav_export) + "\",";
  json += "\"chip_info\":\"" + String(T().chip_info) + "\",";
  json += "\"arduino_core_version\":\"" + String(T().arduino_core_version) + "\",";
  json += "\"memory_details\":\"" + String(T().memory_details) + "\",";
  json += "\"wifi_connection\":\"" + String(T().wifi_connection) + "\",";
  json += "\"wifi_channel\":\"" + String(T().wifi_channel) + "\",";
  json += "\"wifi_mode\":\"" + String(T().wifi_mode) + "\",";
  json += "\"wifi_sleep\":\"" + String(T().wifi_sleep) + "\",";
  json += "\"wifi_band\":\"" + String(T().wifi_band) + "\",";
  json += "\"wifi_band_mode\":\"" + String(T().wifi_band_mode) + "\",";
  json += "\"wifi_tx_power\":\"" + String(T().wifi_tx_power) + "\",";
  json += "\"wifi_hostname\":\"" + String(T().wifi_hostname) + "\",";
  json += "\"gpio_interfaces\":\"" + String(T().gpio_interfaces) + "\",";
  json += "\"i2c_peripherals\":\"" + String(T().i2c_peripherals) + "\",";
  json += "\"builtin_led\":\"" + String(T().builtin_led) + "\",";
  json += "\"oled_screen\":\"" + String(T().oled_screen) + "\",";
  json += "\"adc_test\":\"" + String(T().adc_test) + "\",";
  json += "\"touch_test\":\"" + String(T().touch_test) + "\",";
  json += "\"pwm_test\":\"" + String(T().pwm_test) + "\",";
  json += "\"spi_bus\":\"" + String(T().spi_bus) + "\",";
  json += "\"flash_partitions\":\"" + String(T().flash_partitions) + "\",";
  json += "\"memory_stress\":\"" + String(T().memory_stress) + "\",";
  json += "\"gpio_test\":\"" + String(T().gpio_test) + "\",";
  json += "\"wifi_scanner\":\"" + String(T().wifi_scanner) + "\",";
  json += "\"performance_bench\":\"" + String(T().performance_bench) + "\",";
  json += "\"data_export\":\"" + String(T().data_export) + "\"";
  json += "}";
  
  server.send(200, "application/json", json);
}
void handleAppScript() {
  server.send(200, "application/javascript; charset=utf-8", buildAppScript());
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
  chunk += "body{font-family:'Segoe UI',sans-serif;background:linear-gradient(135deg,#667eea 0%,#764ba2 100%);padding:100px 20px 20px}";
  chunk += ".container{max-width:1400px;margin:0 auto;background:#fff;border-radius:20px;box-shadow:0 20px 60px rgba(0,0,0,.3);overflow:hidden}";
  chunk += ".header{background:linear-gradient(135deg,#667eea 0%,#764ba2 100%);color:#fff;padding:30px;text-align:center;position:relative}";
  chunk += ".header h1{font-size:2.5em;margin-bottom:10px}";
  chunk += ".lang-switcher{position:absolute;top:20px;right:20px;display:flex;gap:5px}";
  chunk += ".lang-btn{padding:8px 15px;background:rgba(255,255,255,.2);border:2px solid rgba(255,255,255,.3);border-radius:5px;color:#fff;cursor:pointer;font-weight:bold;transition:all .3s}";
  chunk += ".lang-btn:hover{background:rgba(255,255,255,.3)}";
  chunk += ".lang-btn.active{background:rgba(255,255,255,.4);border-color:rgba(255,255,255,.6)}";
  chunk += ".status-bar{position:fixed;top:0;left:0;right:0;display:flex;gap:12px;justify-content:center;align-items:center;padding:12px 24px;background:rgba(17,24,39,.85);backdrop-filter:blur(8px);z-index:2500;color:#fff;box-shadow:0 12px 30px rgba(0,0,0,.35)}";
  chunk += ".status-pill{display:flex;align-items:center;gap:10px;padding:6px 18px;border-radius:999px;background:rgba(255,255,255,.12);border:1px solid rgba(255,255,255,.25);font-weight:600;font-size:.95em}";
  chunk += ".status-dot{width:12px;height:12px;border-radius:50%;box-shadow:0 0 10px rgba(255,255,255,.4)}";
  chunk += ".status-dot.online{background:#22c55e;animation:statusBlink 1.4s ease-in-out infinite;box-shadow:0 0 14px rgba(34,197,94,.9)}";
  chunk += ".status-dot.offline{background:#ef4444;box-shadow:0 0 14px rgba(239,68,68,.85)}";
  chunk += ".status-dot.pending{background:#f97316;animation:statusPulse 2s ease-in-out infinite;box-shadow:0 0 14px rgba(249,115,22,.8)}";
  chunk += "@keyframes statusBlink{0%,100%{opacity:1}50%{opacity:.35}}";
  chunk += "@keyframes statusPulse{0%,100%{opacity:.6}50%{opacity:1}}";
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
  chunk += ".info-value{font-size:1.1em;color:#333;min-height:1.6em;display:flex;align-items:center;}";
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
  chunk += ".status-live{padding:10px;background:#f0f0f0;border-radius:5px;text-align:center;font-weight:bold;margin:10px 0;min-height:1.6em;display:flex;align-items:center;justify-content:center;gap:8px;}";
  chunk += ".status-field{gap:8px;}";
  chunk += ".gpio-hint{margin-top:12px;padding:12px;border-radius:10px;background:#eef2ff;color:#4c51bf;font-size:0.9em;min-height:1.6em;}";
  chunk += "input[type='number'],input[type='color'],input[type='text']{padding:10px;border:2px solid #ddd;border-radius:5px;font-size:1em}";
  chunk += "@media print{.nav,.btn,.lang-switcher{display:none}}";
  chunk += "</style></head><body>";
  server.sendContent(chunk);

  // CHUNK 2: STATUS BAR
  chunk = "<div class='status-bar'>";
  chunk += "<div class='status-pill' id='wifi-status-pill'><span class='status-dot offline' id='wifi-status-dot'></span><span id='wifi-status-label'>" + String(T().indicator_wifi) + " · " + String(T().disconnected) + "</span></div>";
  chunk += "<div class='status-pill' id='bt-status-pill'><span class='status-dot pending' id='bt-status-dot'></span><span id='bt-status-label'>" + String(T().indicator_bluetooth) + " · " + String(T().indicator_unavailable) + "</span></div>";
  chunk += "</div>";
  server.sendContent(chunk);

  // CHUNK 3: HEADER + NAV
  chunk = "<div class='container'><div class='header'>";
  chunk += "<div class='lang-switcher'>";
  chunk += "<button class='lang-btn" + String(currentLanguage == LANG_FR ? " active" : "") + "' data-lang='fr'>FR</button>";
  chunk += "<button class='lang-btn" + String(currentLanguage == LANG_EN ? " active" : "") + "' data-lang='en'>EN</button>";
  chunk += "</div>";
  chunk += "<h1 id='main-title'>" + String(T().title) + " " + String(T().version) + String(DIAGNOSTIC_VERSION) + "</h1>";
  chunk += "<div style='font-size:1.2em;margin:10px 0'>" + diagnosticData.chipModel + "</div>";
  chunk += "<div style='font-size:.9em;opacity:.9;margin:10px 0'>";
  chunk += String(T().access) + ": <a href='http://" + String(MDNS_HOSTNAME) + ".local' style='color:#fff;text-decoration:underline'><strong>http://" + String(MDNS_HOSTNAME) + ".local</strong></a> " + String(T().or_text) + " <strong>" + diagnosticData.ipAddress + "</strong>";
  chunk += "</div>";
  chunk += "<div class='nav'>";
  chunk += "<button type='button' class='nav-btn active' data-nav='overview' data-i18n='nav_overview'>" + String(T().nav_overview) + "</button>";
  chunk += "<button type='button' class='nav-btn' data-nav='leds' data-i18n='nav_leds'>" + String(T().nav_leds) + "</button>";
  chunk += "<button type='button' class='nav-btn' data-nav='screens' data-i18n='nav_screens'>" + String(T().nav_screens) + "</button>";
  chunk += "<button type='button' class='nav-btn' data-nav='tests' data-i18n='nav_tests'>" + String(T().nav_tests) + "</button>";
  chunk += "<button type='button' class='nav-btn' data-nav='gpio' data-i18n='nav_gpio'>" + String(T().nav_gpio) + "</button>";
  chunk += "<button type='button' class='nav-btn' data-nav='wireless' data-i18n='nav_wifi'>" + String(T().nav_wifi) + "</button>";
  chunk += "<button type='button' class='nav-btn' data-nav='benchmark' data-i18n='nav_benchmark'>" + String(T().nav_benchmark) + "</button>";
  chunk += "<button type='button' class='nav-btn' data-nav='export' data-i18n='nav_export'>" + String(T().nav_export) + "</button>";
  chunk += "</div></div><div class='content'>";
  server.sendContent(chunk);

// CHUNK 4: OVERVIEW TAB - VERSION UNIQUE COMPLÈTE
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
  chunk += "</div></div>";
  server.sendContent(chunk);
  
  // GPIO et I2C
  chunk = "<div class='section'><h2>" + String(T().gpio_interfaces) + "</h2><div class='info-grid'>";
  chunk += "<div class='info-item'><div class='info-label'>" + String(T().total_gpio) + "</div><div class='info-value'>" + String(diagnosticData.totalGPIO) + " " + String(T().pins) + "</div></div>";
  if (ENABLE_I2C_SCAN) {
    chunk += "<div class='info-item'><div class='info-label'>" + String(T().i2c_peripherals) + "</div><div class='info-value' id='i2c-count'>" + String(diagnosticData.i2cCount) + " " + String(T().devices) + "</div></div>";
    chunk += "<div class='info-item' style='grid-column:1/-1'><div class='info-label'>" + String(T().detected_addresses) + "</div><div class='info-value' id='i2c-devices'>" + (diagnosticData.i2cCount > 0 ? diagnosticData.i2cDevices : String("—")) + "</div></div>";
  }
  chunk += "</div></div>";
  chunk += "</div>"; // Fermeture div overview
  server.sendContent(chunk);

  // CHUNK 5: TAB LEDs
  chunk = "<div id='leds' class='tab-content'>";
  chunk += "<div class='section'><h2>" + String(T().builtin_led) + "</h2><div class='info-grid'>";
  chunk += "<div class='info-item'><div class='info-label'>" + String(T().gpio) + "</div><div class='info-value'>GPIO " + String(BUILTIN_LED_PIN) + "</div></div>";
  chunk += "<div class='info-item'><div class='info-label'>" + String(T().status) + "</div><div class='info-value status-field' id='builtin-led-status'>" + builtinLedTestResult + "</div></div>";
  chunk += "<div class='info-item' style='grid-column:1/-1;text-align:center'>";
  chunk += "<input type='number' id='ledGPIO' value='" + String(BUILTIN_LED_PIN) + "' min='0' max='48' style='width:80px'>";
  chunk += "<button class='btn btn-info' onclick='configBuiltinLED()'>" + String(T().config) + "</button>";
  chunk += "<button class='btn btn-primary' onclick='testBuiltinLED()'>" + String(T().test) + "</button>";
  chunk += "<button class='btn btn-success' onclick='ledBlink()'>" + String(T().blink) + "</button>";
  chunk += "<button class='btn btn-info' onclick='ledFade()'>" + String(T().fade) + "</button>";
  chunk += "<button class='btn btn-danger' onclick='ledOff()'>" + String(T().off) + "</button>";
  chunk += "</div></div>";
  chunk += "<div class='gpio-hint'>" + String(T().led_auto_hint) + "</div>";
  chunk += "</div>";

  chunk += "<div class='section'><h2>" + String(T().neopixel) + "</h2><div class='info-grid'>";
  chunk += "<div class='info-item'><div class='info-label'>" + String(T().gpio) + "</div><div class='info-value'>GPIO " + String(LED_PIN) + "</div></div>";
  chunk += "<div class='info-item'><div class='info-label'>" + String(T().status) + "</div><div class='info-value status-field' id='neopixel-status'>" + neopixelTestResult + "</div></div>";
  chunk += "<div class='info-item' style='grid-column:1/-1;text-align:center'>";
  chunk += "<input type='number' id='neoGPIO' value='" + String(LED_PIN) + "' min='0' max='48' style='width:80px'>";
  chunk += "<input type='number' id='neoCount' value='" + String(LED_COUNT) + "' min='1' max='100' style='width:80px'>";
  chunk += "<button class='btn btn-info' onclick='configNeoPixel()'>" + String(T().config) + "</button>";
  chunk += "<button class='btn btn-primary' onclick='testNeoPixel()'>" + String(T().test) + "</button>";
  chunk += "<button class='btn btn-primary' onclick='neoPattern(\"rainbow\")'>" + String(T().rainbow) + "</button>";
  chunk += "<input type='color' id='neoColor' value='#ff0000' style='height:48px'>";
  chunk += "<button class='btn btn-primary' onclick='neoCustomColor()'>" + String(T().color) + "</button>";
  chunk += "<button class='btn btn-danger' onclick='neoPattern(\"off\")'>" + String(T().off) + "</button>";
  chunk += "</div></div>";
  chunk += "<div class='gpio-hint'>" + String(T().neopixel_auto_hint) + "</div>";
  chunk += "</div></div>";
  server.sendContent(chunk);
  
  // CHUNK 6: TAB Screens
  chunk = "<div id='screens' class='tab-content'>";
  chunk += "<div class='section'><h2>" + String(T().oled_screen) + "</h2><div class='info-grid'>";
  chunk += "<div class='info-item'><div class='info-label'>" + String(T().status) + "</div><div class='info-value status-field' id='oled-status'>" + oledTestResult + "</div></div>";
  chunk += "<div class='info-item'><div class='info-label'>" + String(T().i2c_pins) + "</div><div class='info-value' id='oled-pins'>SDA:" + String(I2C_SDA) + " SCL:" + String(I2C_SCL) + "</div></div>";
  chunk += "<div class='info-item' style='grid-column:1/-1;text-align:center'>";
  chunk += "SDA: <input type='number' id='oledSDA' value='" + String(I2C_SDA) + "' min='0' max='48' style='width:70px'> ";
  chunk += "SCL: <input type='number' id='oledSCL' value='" + String(I2C_SCL) + "' min='0' max='48' style='width:70px'> ";
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
  chunk += "</div>";
  chunk += "<div class='gpio-hint'>" + String(T().oled_auto_hint) + "</div>";
  chunk += "</div></div></div>";
  server.sendContent(chunk);
  
  // CHUNK 7: TAB Tests
  chunk = "<div id='tests' class='tab-content'>";
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
  
  // CHUNK 8: TAB GPIO
  chunk = "<div id='gpio' class='tab-content'>";
  chunk += "<div class='section'><h2>" + String(T().gpio_test) + "</h2>";
  chunk += "<div style='text-align:center;margin:20px 0'>";
  chunk += "<button class='btn btn-primary' onclick='testAllGPIO()'>" + String(T().test_all_gpio) + "</button>";
  chunk += "<div id='gpio-status' class='status-live'>" + String(T().click_to_test) + "</div>";
  chunk += "</div><p class='gpio-hint'>ℹ️ Un GPIO indiqué comme ❌ FAIL peut simplement être réservé ou non câblé. Vérifiez le schéma matériel avant de conclure à une défaillance.</p><div id='gpio-results' class='gpio-grid'></div></div></div>";
  server.sendContent(chunk);
  
  // CHUNK 9: TAB Wireless
  chunk = "<div id='wireless' class='tab-content'>";
  chunk += "<div class='section'><h2>" + String(T().wifi_connection) + "</h2><div class='info-grid'>";
  chunk += "<div class='info-item'><div class='info-label'>" + String(T().status) + "</div><div class='info-value' id='wifi-connection-state'>-</div></div>";
  chunk += "<div class='info-item'><div class='info-label'>" + String(T().connected_ssid) + "</div><div class='info-value' id='wifi-ssid'>-</div></div>";
  chunk += "<div class='info-item'><div class='info-label'>" + String(T().signal_power) + "</div><div class='info-value' id='wifi-rssi'>-</div></div>";
  chunk += "<div class='info-item'><div class='info-label'>" + String(T().signal_quality) + "</div><div class='info-value' id='wifi-quality'>-</div></div>";
  chunk += "<div class='info-item'><div class='info-label'>" + String(T().ip_address) + "</div><div class='info-value' id='wifi-ip'>-</div></div>";
  chunk += "<div class='info-item'><div class='info-label'>" + String(T().subnet_mask) + "</div><div class='info-value' id='wifi-subnet'>-</div></div>";
  chunk += "<div class='info-item'><div class='info-label'>" + String(T().gateway) + "</div><div class='info-value' id='wifi-gateway'>-</div></div>";
  chunk += "<div class='info-item'><div class='info-label'>" + String(T().dns) + "</div><div class='info-value' id='wifi-dns'>-</div></div>";
  chunk += "<div class='info-item'><div class='info-label'>" + String(T().wifi_channel) + "</div><div class='info-value' id='wifi-channel'>-</div></div>";
  chunk += "<div class='info-item'><div class='info-label'>" + String(T().wifi_mode) + "</div><div class='info-value' id='wifi-mode'>-</div></div>";
  chunk += "<div class='info-item'><div class='info-label'>" + String(T().wifi_sleep) + "</div><div class='info-value' id='wifi-sleep'>-</div></div>";
  chunk += "<div class='info-item'><div class='info-label'>" + String(T().wifi_band) + "</div><div class='info-value' id='wifi-band'>-</div></div>";
  chunk += "<div class='info-item'><div class='info-label'>" + String(T().wifi_band_mode) + "</div><div class='info-value' id='wifi-band-mode'>-</div></div>";
  chunk += "<div class='info-item'><div class='info-label'>" + String(T().wifi_tx_power) + "</div><div class='info-value' id='wifi-tx-power'>-</div></div>";
  chunk += "<div class='info-item'><div class='info-label'>" + String(T().wifi_hostname) + "</div><div class='info-value' id='wifi-hostname'>-</div></div>";
  chunk += "</div></div>";

  chunk += "<div class='section'><h2>" + String(T().wifi_scanner) + "</h2>";
  chunk += "<div style='text-align:center;margin:20px 0'>";
  chunk += "<button class='btn btn-primary' onclick='scanWiFi()'>" + String(T().scan_networks) + "</button>";
  chunk += "<div id='wifi-status' class='status-live'>" + String(T().click_to_test) + "</div>";
  chunk += "</div><div id='wifi-results' class='wifi-list'></div></div>";

  chunk += "<div class='section'><h2>" + String(T().bluetooth_section) + "</h2>";
  chunk += "<div class='info-grid'>";
  chunk += "<div class='info-item'><div class='info-label'>" + String(T().bluetooth_state) + "</div><div class='info-value' id='bluetooth-controller'>-</div></div>";
  chunk += "<div class='info-item'><div class='info-label'>" + String(T().bluetooth_capabilities) + "</div><div class='info-value' id='bluetooth-capabilities'>-</div></div>";
  chunk += "<div class='info-item'><div class='info-label'>" + String(T().bluetooth_last_test) + "</div><div class='info-value' id='bluetooth-last-test'>" + String(T().not_tested) + "</div></div>";
  chunk += "</div>";
  chunk += "<div style='text-align:center;margin:20px 0'>";
  chunk += "<button class='btn btn-primary' onclick='testBluetooth()'>" + String(T().bluetooth_test_button) + "</button>";
  chunk += "<div id='bluetooth-status' class='status-live'>" + String(T().not_tested) + "</div>";
  chunk += "</div>";
  chunk += "<p id='bluetooth-hint' class='gpio-hint'></p>";
  chunk += "</div></div>";
  server.sendContent(chunk);
  
  // CHUNK 10: TAB Benchmark
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
  
  // CHUNK 11: TAB Export
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
  chunk += "</div></div>"; // Fermeture content + container
  chunk += "<script src='/js/app.js'></script>";
  chunk += "</body></html>";
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
  Serial.println("     Version 2.4 - FR/EN");
  Serial.println("     Optimise Arduino Core 3.1.3");
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
  
  // ========== ROUTES SERVEUR ==========
  server.on("/", handleRoot);
  server.on("/js/app.js", handleAppScript);
  
  // **NOUVELLES ROUTES MULTILINGUES**
  server.on("/api/set-language", handleSetLanguage);
  server.on("/api/get-translations", handleGetTranslations);
  
  // GPIO & WiFi
  server.on("/api/test-gpio", handleTestGPIO);
  server.on("/api/wifi-scan", handleWiFiScan);
  server.on("/api/wireless-info", handleWirelessInfo);
  server.on("/api/bluetooth-test", handleBluetoothTest);
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
