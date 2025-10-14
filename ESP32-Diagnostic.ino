/*
 * DIAGNOSTIC COMPLET ESP32 - VERSION 3.0-dev
 * Compatible: ESP32, ESP32-S2, ESP32-S3, ESP32-C3
 * Optimisé pour ESP32 Arduino Core 3.1.3
 * Carte testée: ESP32-S3 avec PSRAM OPI
 * Auteur: morfredus
 *
 * Nouveautés v3.0-dev:
 * - Interface web 100% dynamique
 * - Mise à jour temps réel (5s)
 * - Architecture API REST
 * - Séparation HTML/CSS/JavaScript
 * - Chargement asynchrone des onglets
 * - Indicateur de connexion en direct
 * - Animations et transitions fluides
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
#include <Wire.h>
#include <Adafruit_NeoPixel.h>
#include <TFT_eSPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>
#include <vector>
#include <math.h>

// Configuration WiFi
#include "config.h"

// Système de traduction
#include "languages.h"

// ========== CONFIGURATION ==========
#define DIAGNOSTIC_VERSION "3.0-dev"
#define CUSTOM_LED_PIN -1
#define CUSTOM_LED_COUNT 1
#define ENABLE_I2C_SCAN true
#define MDNS_HOSTNAME "esp32-diagnostic"

// Variables globales pour web_interface.h
const char* DIAGNOSTIC_VERSION_STR = DIAGNOSTIC_VERSION;
const char* MDNS_HOSTNAME_STR = MDNS_HOSTNAME;

// Pins I2C pour OLED (modifiables via web)
int I2C_SDA = 8;
int I2C_SCL = 9;

// Pins TFT (FIXES - non modifiables)
#define TFT_MOSI  45
#define TFT_SCLK   3
#define TFT_CS    14
#define TFT_DC    47
#define TFT_RST   21
#define TFT_MISO  46

// OLED 0.96" I2C
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C

// Capteurs
const int DEFAULT_PIR_PIN = 6;
const int DEFAULT_DISTANCE_TRIG_PIN = 7;
const int DEFAULT_DISTANCE_ECHO_PIN = 8;
const int DEFAULT_DHT_PIN = 4;
const int DEFAULT_LDR_PIN = 5;

int PIR_PIN = DEFAULT_PIR_PIN;
int DISTANCE_TRIG_PIN = DEFAULT_DISTANCE_TRIG_PIN;
int DISTANCE_ECHO_PIN = DEFAULT_DISTANCE_ECHO_PIN;
int DHT_PIN = DEFAULT_DHT_PIN;
int DHT_TYPE_CURRENT = DHT11;
int LDR_PIN = DEFAULT_LDR_PIN;

DHT* dhtSensor = nullptr;

// ========== OBJETS GLOBAUX ==========
WebServer server(80);
WiFiMulti wifiMulti;
TFT_eSPI tft = TFT_eSPI();
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

// TFT
bool tftTested = false;
bool tftAvailable = false;
String tftTestResult = "En attente d'initialisation";
int tftWidth = 0;
int tftHeight = 0;

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

struct SensorStatusEntry {
  bool tested = false;
  bool success = false;
  String message = "Non teste";
};

struct SensorsDiagnostics {
  SensorStatusEntry pir;
  bool pirMotion = false;

  SensorStatusEntry distance;
  float distanceCm = 0.0f;

  SensorStatusEntry dht;
  float dhtTemperature = NAN;
  float dhtHumidity = NAN;

  SensorStatusEntry ldr;
  int ldrRaw = 0;
  float ldrVoltage = 0.0f;
} sensorsDiag;

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
  
  bool tftTested;
  bool tftAvailable;
  String tftResult;
  int tftWidth;
  int tftHeight;
  
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

// Interface web dynamique (APRÈS toutes les déclarations)
#include "web_interface.h"

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
  
  #if defined(CONFIG_SPIRAM)
    detailedMemory.psramConfigured = true;
  #endif
  #if defined(CONFIG_SPIRAM_SUPPORT)
    detailedMemory.psramConfigured = true;
  #endif
  #if defined(BOARD_HAS_PSRAM)
    detailedMemory.psramConfigured = true;
  #endif
  #if defined(CONFIG_SPIRAM_MODE_OCT)
    detailedMemory.psramConfigured = true;
  #endif
  #if defined(CONFIG_SPIRAM_MODE_QUAD)
    detailedMemory.psramConfigured = true;
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
    detailedMemory.memoryStatus = "Moyen";
  } else {
    detailedMemory.memoryStatus = T().critical;
  }
}

void printPSRAMDiagnostic() {
  Serial.println("\r\n=== DIAGNOSTIC PSRAM DETAILLE ===");
  Serial.printf("ESP.getPsramSize(): %u octets (%.2f MB)\r\n", 
                ESP.getPsramSize(), ESP.getPsramSize() / 1048576.0);
  
  Serial.println("\r\nFlags de compilation detectes:");
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
    Serial.println("  ✓ CONFIG_SPIRAM_MODE_OCT (OPI)");
    psramType = "OPI";
    anyFlag = true;
  #endif
  #ifdef CONFIG_SPIRAM_MODE_QUAD
    Serial.println("  ✓ CONFIG_SPIRAM_MODE_QUAD (QSPI)");
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
    Serial.printf("  → Carte compatible PSRAM %s mais DESACTIVEE\r\n", psramType.c_str());
    Serial.println("  → Pour activer: Tools → PSRAM → OPI PSRAM");
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

// ========== TFT ==========
void detectTFT() {
  Serial.println("\r\n=== DETECTION TFT ===");
  pinMode(TFT_CS, OUTPUT);
  pinMode(TFT_DC, OUTPUT);
  pinMode(TFT_RST, OUTPUT);
  
  digitalWrite(TFT_RST, LOW);
  delay(20);
  digitalWrite(TFT_RST, HIGH);
  delay(150);
  
  tftAvailable = false;
  tftTestResult = "Non teste - Config SPI prete";
  Serial.printf("TFT: Pins configures CS=%d DC=%d RST=%d\r\n", TFT_CS, TFT_DC, TFT_RST);
}

void testTFT() {
  if (tftTested) return;
  
  Serial.println("\r\n=== TEST TFT ===");
  tft.init();
  tft.setRotation(1);
  
  tft.fillScreen(TFT_RED);
  delay(500);
  tft.fillScreen(TFT_GREEN);
  delay(500);
  tft.fillScreen(TFT_BLUE);
  delay(500);
  tft.fillScreen(TFT_WHITE);
  delay(500);
  
  tft.fillScreen(TFT_BLACK);
  int sq = 20;
  for(int y = 0; y < 240; y += sq) {
    for(int x = 0; x < 320; x += sq) {
      if ((x/sq + y/sq) % 2 == 0) {
        tft.fillRect(x, y, sq, sq, TFT_WHITE);
      }
    }
  }
  delay(1000);
  
  tft.fillScreen(TFT_NAVY);
  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(2);
  tft.setCursor(50, 100);
  tft.println("TEST TFT OK");
  delay(2000);
  
  tft.fillScreen(TFT_BLACK);
  
  tftWidth = tft.width();
  tftHeight = tft.height();
  tftAvailable = true;
  tftTestResult = "Test OK - " + String(tftWidth) + "x" + String(tftHeight);
  tftTested = true;
  Serial.println("TFT: OK");
}

void resetTFTTest() {
  tftTested = false;
  tftAvailable = false;
  tft.fillScreen(TFT_BLACK);
}

void tftTestColors() {
  if (!tftAvailable) return;
  tft.fillScreen(TFT_RED);
  delay(300);
  tft.fillScreen(TFT_GREEN);
  delay(300);
  tft.fillScreen(TFT_BLUE);
  delay(300);
  tft.fillScreen(TFT_BLACK);
}

void tftTestCheckerboard() {
  if (!tftAvailable) return;
  tft.fillScreen(TFT_BLACK);
  int sq = 20;
  for(int y = 0; y < 240; y += sq) {
    for(int x = 0; x < 320; x += sq) {
      if ((x/sq + y/sq) % 2 == 0) {
        tft.fillRect(x, y, sq, sq, TFT_WHITE);
      }
    }
  }
}

void tftClear() {
  tft.fillScreen(TFT_BLACK);
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

void testOLED() {
  if (!oledAvailable || oledTested) return;
  
  Serial.println("\r\n=== TEST OLED ===");

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

  oled.clearDisplay();
  oled.setTextSize(2);
  oled.setCursor(20, 20);
  oled.println("ESP32");
  oled.display();
  delay(1500);

  oled.clearDisplay();
  oled.setTextSize(1);
  oled.setCursor(0, 0);
  oled.println("Taille 1");
  oled.setTextSize(2);
  oled.println("Taille 2");
  oled.setTextSize(1);
  oled.println("Retour taille 1");
  oled.display();
  delay(2000);

  oled.clearDisplay();
  oled.drawRect(10, 10, 30, 20, SSD1306_WHITE);
  oled.fillRect(50, 10, 30, 20, SSD1306_WHITE);
  oled.drawCircle(25, 50, 10, SSD1306_WHITE);
  oled.fillCircle(65, 50, 10, SSD1306_WHITE);
  oled.drawTriangle(95, 30, 85, 10, 105, 10, SSD1306_WHITE);
  oled.display();
  delay(2000);

  oled.clearDisplay();
  for(int i = 0; i < SCREEN_HEIGHT; i += 4) {
    oled.drawLine(0, i, SCREEN_WIDTH, i, SSD1306_WHITE);
  }
  oled.display();
  delay(1500);

  oled.clearDisplay();
  for(int i = 0; i < SCREEN_WIDTH; i += 8) {
    oled.drawLine(0, 0, i, SCREEN_HEIGHT-1, SSD1306_WHITE);
    oled.drawLine(SCREEN_WIDTH-1, 0, i, SCREEN_HEIGHT-1, SSD1306_WHITE);
  }
  oled.display();
  delay(1500);

  for(int x = 0; x < SCREEN_WIDTH - 20; x += 4) {
    oled.clearDisplay();
    oled.fillRect(x, 22, 20, 20, SSD1306_WHITE);
    oled.display();
    delay(20);
  }

  oled.clearDisplay();
  oled.setTextSize(1);
  oled.setCursor(20, 10);
  oled.println("Chargement");
  for(int i = 0; i <= 100; i += 5) {
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

  String scrollText = "  DIAGNOSTIC ESP32 COMPLET - OLED 0.96 pouces I2C  ";
  for(int offset = 0; offset < scrollText.length() * 6; offset += 2) {
    oled.clearDisplay();
    oled.setTextSize(1);
    oled.setCursor(-offset, 28);
    oled.print(scrollText);
    oled.display();
    delay(30);
  }

  oled.clearDisplay();
  oled.setTextSize(1);
  oled.setCursor(30, 20);
  oled.println("TEST OK!");
  oled.drawRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, SSD1306_WHITE);
  oled.display();
  delay(2000);
  
  oled.clearDisplay();
  oled.display();
  
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

// ========== CAPTEURS ==========

void resetSensorStatus(SensorStatusEntry &status) {
  status.tested = false;
  status.success = false;
  status.message = "Non teste";
}

String dhtTypeToString(int type) {
  switch (type) {
    case DHT11: return "DHT11";
    case DHT22: return "DHT22";
    case DHT21: return "DHT21";
    default: return "UNKNOWN";
  }
}

int stringToDhtType(const String &type) {
  String upper = type;
  upper.toUpperCase();
  if (upper == "DHT22") return DHT22;
  if (upper == "DHT21") return DHT21;
  return DHT11;
}

void initDHTSensor() {
  if (dhtSensor != nullptr) {
    delete dhtSensor;
    dhtSensor = nullptr;
  }
  dhtSensor = new DHT(DHT_PIN, DHT_TYPE_CURRENT);
  dhtSensor->begin();
}

void configurePIRPin(int pin) {
  PIR_PIN = pin;
  pinMode(PIR_PIN, INPUT);
  sensorsDiag.pirMotion = false;
  resetSensorStatus(sensorsDiag.pir);
}

bool isValidGPIOPin(int pin) {
  return pin >= 0 && pin <= 48;
}

String boolToStr(bool value) {
  return value ? "true" : "false";
}

void configureDistancePins(int trigPin, int echoPin) {
  DISTANCE_TRIG_PIN = trigPin;
  DISTANCE_ECHO_PIN = echoPin;
  pinMode(DISTANCE_TRIG_PIN, OUTPUT);
  digitalWrite(DISTANCE_TRIG_PIN, LOW);
  pinMode(DISTANCE_ECHO_PIN, INPUT);
  sensorsDiag.distanceCm = 0.0f;
  resetSensorStatus(sensorsDiag.distance);
}

void configureDHTSensor(int pin, int type) {
  DHT_PIN = pin;
  DHT_TYPE_CURRENT = type;
  initDHTSensor();
  sensorsDiag.dhtHumidity = NAN;
  sensorsDiag.dhtTemperature = NAN;
  resetSensorStatus(sensorsDiag.dht);
}

void configureLDRPin(int pin) {
  LDR_PIN = pin;
  pinMode(LDR_PIN, INPUT);
  sensorsDiag.ldrRaw = 0;
  sensorsDiag.ldrVoltage = 0.0f;
  resetSensorStatus(sensorsDiag.ldr);
}

void initializeSensors() {
  configurePIRPin(PIR_PIN);
  configureDistancePins(DISTANCE_TRIG_PIN, DISTANCE_ECHO_PIN);
  configureDHTSensor(DHT_PIN, DHT_TYPE_CURRENT);
  configureLDRPin(LDR_PIN);
}

void testPIRSensor() {
  sensorsDiag.pir.tested = true;
  pinMode(PIR_PIN, INPUT);
  int value = digitalRead(PIR_PIN);
  sensorsDiag.pirMotion = (value == HIGH);
  sensorsDiag.pir.success = true;
  sensorsDiag.pir.message = sensorsDiag.pirMotion ? "Mouvement detecte" : "Pas de mouvement";
}

void testDistanceSensor() {
  sensorsDiag.distance.tested = true;
  pinMode(DISTANCE_TRIG_PIN, OUTPUT);
  pinMode(DISTANCE_ECHO_PIN, INPUT);
  digitalWrite(DISTANCE_TRIG_PIN, LOW);
  delayMicroseconds(5);
  digitalWrite(DISTANCE_TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(DISTANCE_TRIG_PIN, LOW);
  long duration = pulseIn(DISTANCE_ECHO_PIN, HIGH, 30000);
  if (duration <= 0) {
    sensorsDiag.distance.success = false;
    sensorsDiag.distance.message = "Aucun echo";
    sensorsDiag.distanceCm = 0.0f;
    return;
  }
  sensorsDiag.distanceCm = (duration * 0.0343f) / 2.0f;
  sensorsDiag.distance.success = true;
  sensorsDiag.distance.message = "Distance: " + String(sensorsDiag.distanceCm, 1) + " cm";
}

void testDHTSensor() {
  sensorsDiag.dht.tested = true;
  if (dhtSensor == nullptr) {
    initDHTSensor();
  }
  float humidity = dhtSensor->readHumidity();
  float temperature = dhtSensor->readTemperature();
  if (isnan(humidity) || isnan(temperature)) {
    sensorsDiag.dht.success = false;
    sensorsDiag.dht.message = "Lecture invalide";
    sensorsDiag.dhtHumidity = NAN;
    sensorsDiag.dhtTemperature = NAN;
    return;
  }
  sensorsDiag.dhtHumidity = humidity;
  sensorsDiag.dhtTemperature = temperature;
  sensorsDiag.dht.success = true;
  sensorsDiag.dht.message = "T:" + String(temperature, 1) + "°C H:" + String(humidity, 1) + "%";
}

void testLDRSensor() {
  sensorsDiag.ldr.tested = true;
  sensorsDiag.ldrRaw = analogRead(LDR_PIN);
  sensorsDiag.ldrVoltage = (sensorsDiag.ldrRaw / 4095.0f) * 3.3f;
  sensorsDiag.ldr.success = true;
  sensorsDiag.ldr.message = "Brut: " + String(sensorsDiag.ldrRaw);
}

String sensorsToJson() {
  String json = "{";
  json += "\"pir\":{";
  json += "\"pin\":" + String(PIR_PIN) + ",";
  json += "\"motion\":" + boolToStr(sensorsDiag.pirMotion) + ",";
  json += "\"tested\":" + boolToStr(sensorsDiag.pir.tested) + ",";
  json += "\"success\":" + boolToStr(sensorsDiag.pir.success) + ",";
  json += "\"status\":\"" + sensorsDiag.pir.message + "\"";
  json += "},";
  json += "\"distance\":{";
  json += "\"trig\":" + String(DISTANCE_TRIG_PIN) + ",";
  json += "\"echo\":" + String(DISTANCE_ECHO_PIN) + ",";
  json += "\"tested\":" + boolToStr(sensorsDiag.distance.tested) + ",";
  json += "\"success\":" + boolToStr(sensorsDiag.distance.success) + ",";
  json += "\"distance_cm\":" + String(sensorsDiag.distanceCm, 1) + ",";
  json += "\"status\":\"" + sensorsDiag.distance.message + "\"";
  json += "},";
  json += "\"dht\":{";
  json += "\"pin\":" + String(DHT_PIN) + ",";
  json += "\"type\":\"" + dhtTypeToString(DHT_TYPE_CURRENT) + "\",";
  json += "\"tested\":" + boolToStr(sensorsDiag.dht.tested) + ",";
  json += "\"success\":" + boolToStr(sensorsDiag.dht.success) + ",";
  json += "\"temperature\":" + String(isnan(sensorsDiag.dhtTemperature) ? 0.0f : sensorsDiag.dhtTemperature, 1) + ",";
  json += "\"humidity\":" + String(isnan(sensorsDiag.dhtHumidity) ? 0.0f : sensorsDiag.dhtHumidity, 1) + ",";
  json += "\"status\":\"" + sensorsDiag.dht.message + "\"";
  json += "},";
  json += "\"ldr\":{";
  json += "\"pin\":" + String(LDR_PIN) + ",";
  json += "\"tested\":" + boolToStr(sensorsDiag.ldr.tested) + ",";
  json += "\"success\":" + boolToStr(sensorsDiag.ldr.success) + ",";
  json += "\"raw\":" + String(sensorsDiag.ldrRaw) + ",";
  json += "\"voltage\":" + String(sensorsDiag.ldrVoltage, 3) + ",";
  json += "\"status\":\"" + sensorsDiag.ldr.message + "\"";
  json += "}";
  json += "}";
  return json;
}

void handleSensorsInfo() {
  String response = "{\"success\":true,\"sensors\":" + sensorsToJson() + "}";
  server.send(200, "application/json", response);
}

void handleSensorTest() {
  if (!server.hasArg("sensor")) {
    server.send(400, "application/json", "{\"success\":false,\"message\":\"Parametre sensor manquant\"}");
    return;
  }

  String sensor = server.arg("sensor");
  if (sensor == "pir") {
    testPIRSensor();
  } else if (sensor == "distance") {
    testDistanceSensor();
  } else if (sensor == "dht") {
    testDHTSensor();
  } else if (sensor == "ldr") {
    testLDRSensor();
  } else {
    server.send(400, "application/json", "{\"success\":false,\"message\":\"Capteur inconnu\"}");
    return;
  }

  String response = "{\"success\":true,\"sensors\":" + sensorsToJson() + "}";
  server.send(200, "application/json", response);
}

void handleSensorConfig() {
  if (!server.hasArg("sensor")) {
    server.send(400, "application/json", "{\"success\":false,\"message\":\"Parametre sensor manquant\"}");
    return;
  }

  String sensor = server.arg("sensor");
  if (sensor == "pir") {
    if (!server.hasArg("pin")) {
      server.send(400, "application/json", "{\"success\":false,\"message\":\"Parametre pin manquant\"}");
      return;
    }
    int pin = server.arg("pin").toInt();
    if (!isValidGPIOPin(pin)) {
      server.send(400, "application/json", "{\"success\":false,\"message\":\"GPIO invalide\"}");
      return;
    }
    configurePIRPin(pin);
  } else if (sensor == "distance") {
    if (!server.hasArg("trig") || !server.hasArg("echo")) {
      server.send(400, "application/json", "{\"success\":false,\"message\":\"Parametres trig/echo manquants\"}");
      return;
    }
    int trig = server.arg("trig").toInt();
    int echo = server.arg("echo").toInt();
    if (!isValidGPIOPin(trig) || !isValidGPIOPin(echo)) {
      server.send(400, "application/json", "{\"success\":false,\"message\":\"GPIO invalide\"}");
      return;
    }
    configureDistancePins(trig, echo);
  } else if (sensor == "dht") {
    if (!server.hasArg("pin")) {
      server.send(400, "application/json", "{\"success\":false,\"message\":\"Parametre pin manquant\"}");
      return;
    }
    int pin = server.arg("pin").toInt();
    if (!isValidGPIOPin(pin)) {
      server.send(400, "application/json", "{\"success\":false,\"message\":\"GPIO invalide\"}");
      return;
    }
    int type = DHT_TYPE_CURRENT;
    if (server.hasArg("type")) {
      type = stringToDhtType(server.arg("type"));
    }
    configureDHTSensor(pin, type);
  } else if (sensor == "ldr") {
    if (!server.hasArg("pin")) {
      server.send(400, "application/json", "{\"success\":false,\"message\":\"Parametre pin manquant\"}");
      return;
    }
    int pin = server.arg("pin").toInt();
    if (!isValidGPIOPin(pin)) {
      server.send(400, "application/json", "{\"success\":false,\"message\":\"GPIO invalide\"}");
      return;
    }
    configureLDRPin(pin);
  } else {
    server.send(400, "application/json", "{\"success\":false,\"message\":\"Capteur inconnu\"}");
    return;
  }

  String response = "{\"success\":true,\"sensors\":" + sensorsToJson() + "}";
  server.send(200, "application/json", response);
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
  
  ledcAttach(testPin, 5000, 8);
  
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
  
  diagnosticData.tftTested = tftTested;
  diagnosticData.tftAvailable = tftAvailable;
  diagnosticData.tftResult = tftTestResult;
  diagnosticData.tftWidth = tftWidth;
  diagnosticData.tftHeight = tftHeight;
  
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

void handleGetTranslations() {
  String json = "{";

  if (currentLanguage == LANG_FR) {
    json += "\"overview\":\"Vue d'ensemble\",";
    json += "\"leds\":\"LEDs\",";
    json += "\"screens\":\"Écrans\",";
    json += "\"tests\":\"Tests\",";
    json += "\"gpio\":\"GPIO\",";
    json += "\"wifi\":\"WiFi\",";
    json += "\"benchmark\":\"Performance\",";
    json += "\"export\":\"Export\",";
    json += "\"processor\":\"🔧 Informations Processeur\",";
    json += "\"memory\":\"💾 Mémoire Détaillée\",";
    json += "\"wifi_title\":\"📡 Connexion WiFi\",";
    json += "\"builtin_led\":\"💡 LED Intégrée\",";
    json += "\"neopixel\":\"🌈 NeoPixel / WS2812B\",";
    json += "\"status\":\"Statut\",";
    json += "\"test\":\"🧪 Test complet\",";
    json += "\"blink\":\"⚡ Clignoter\",";
    json += "\"fade\":\"🌊 Fade\",";
    json += "\"on\":\"💡 Allumer\",";
    json += "\"off\":\"⭕ Éteindre\",";
    json += "\"rainbow\":\"🌈 Arc-en-ciel\",";
    json += "\"apply_color\":\"🎨 Appliquer couleur\"";
  } else {
    json += "\"overview\":\"Overview\",";
    json += "\"leds\":\"LEDs\",";
    json += "\"screens\":\"Screens\",";
    json += "\"tests\":\"Tests\",";
    json += "\"gpio\":\"GPIO\",";
    json += "\"wifi\":\"WiFi\",";
    json += "\"benchmark\":\"Benchmark\",";
    json += "\"export\":\"Export\",";
    json += "\"processor\":\"🔧 Processor Information\",";
    json += "\"memory\":\"💾 Detailed Memory\",";
    json += "\"wifi_title\":\"📡 WiFi Connection\",";
    json += "\"builtin_led\":\"💡 Built-in LED\",";
    json += "\"neopixel\":\"🌈 NeoPixel / WS2812B\",";
    json += "\"status\":\"Status\",";
    json += "\"test\":\"🧪 Full test\",";
    json += "\"blink\":\"⚡ Blink\",";
    json += "\"fade\":\"🌊 Fade\",";
    json += "\"on\":\"💡 Turn on\",";
    json += "\"off\":\"⭕ Turn off\",";
    json += "\"rainbow\":\"🌈 Rainbow\",";
    json += "\"apply_color\":\"🎨 Apply color\"";
  }

  json += "}";

  server.send(200, "application/json", json);
}
void handleTestGPIO() {
  testAllGPIOs();
  String json = "{\"results\":[";
  for (size_t i = 0; i < gpioResults.size(); i++) {
    if (i > 0) json += ",";
    json += "{\"pin\":" + String(gpioResults[i].pin) +
            ",\"working\":" + String(gpioResults[i].working ? "true" : "false") + "}";
  }
  json += "]}";
  server.send(200, "application/json", json);
}

void handleWiFiScan() {
  scanWiFiNetworks();
  String json = "{\"networks\":[";
  for (size_t i = 0; i < wifiNetworks.size(); i++) {
    if (i > 0) json += ",";
    json += "{\"ssid\":\"" + wifiNetworks[i].ssid +
            "\",\"rssi\":" + String(wifiNetworks[i].rssi) +
            ",\"channel\":" + String(wifiNetworks[i].channel) +
            ",\"encryption\":\"" + wifiNetworks[i].encryption +
            "\",\"bssid\":\"" + wifiNetworks[i].bssid + "\"}";
  }
  json += "]}";
  server.send(200, "application/json", json);
}

void handleI2CScan() {
  scanI2C();
  server.send(200, "application/json",
              "{\"count\":" + String(diagnosticData.i2cCount) +
              ",\"devices\":\"" + diagnosticData.i2cDevices + "\"}");
}

void handleBuiltinLEDConfig() {
  if (server.hasArg("gpio")) {
    int newGPIO = server.arg("gpio").toInt();
    if (newGPIO >= 0 && newGPIO <= 48) {
      BUILTIN_LED_PIN = newGPIO;
      resetBuiltinLEDTest();
      server.send(200, "application/json",
                  "{\"success\":true,\"message\":\"LED GPIO " + String(BUILTIN_LED_PIN) + "\"}");
      return;
    }
  }
  server.send(400, "application/json", "{\"success\":false}");
}

void handleBuiltinLEDTest() {
  resetBuiltinLEDTest();
  testBuiltinLED();
  server.send(200, "application/json",
              "{\"success\":" + String(builtinLedAvailable ? "true" : "false") +
              ",\"result\":\"" + builtinLedTestResult + "\"}");
}

void handleBuiltinLEDControl() {
  if (!builtinLedAvailable) {
    server.send(200, "application/json", "{\"success\":false,\"message\":\"LED intégrée non disponible\"}");
    return;
  }
  
  if (server.hasArg("action")) {
    String action = server.arg("action");
    String message = "";
    bool success = true;

    if (action == "on") {
      digitalWrite(BUILTIN_LED_PIN, HIGH);
      message = "LED allumée";
      Serial.println("[LED] Allumée");
    }
    else if (action == "off") {
      digitalWrite(BUILTIN_LED_PIN, LOW);
      message = "LED éteinte";
      Serial.println("[LED] Éteinte");
    }
    else if (action == "blink") {
      message = "Clignotement 10x (2 Hz)";
      Serial.println("[LED] Clignotement démarré...");
      for (int i = 0; i < 10; i++) {
        digitalWrite(BUILTIN_LED_PIN, HIGH);
        delay(250);
        digitalWrite(BUILTIN_LED_PIN, LOW);
        delay(250);
      }
      Serial.println("[LED] Clignotement terminé");
    }
    else if (action == "fade") {
      message = "Fade 3 cycles";
      Serial.println("[LED] Fade démarré...");

      // Vérifier si le GPIO supporte PWM
      if (ledcAttach(BUILTIN_LED_PIN, 5000, 8)) {
        // 3 cycles de fade in/out
        for (int cycle = 0; cycle < 3; cycle++) {
          // Fade in (0 -> 255)
          for (int brightness = 0; brightness <= 255; brightness += 5) {
            ledcWrite(BUILTIN_LED_PIN, brightness);
            delay(10);
          }
          // Fade out (255 -> 0)
          for (int brightness = 255; brightness >= 0; brightness -= 5) {
            ledcWrite(BUILTIN_LED_PIN, brightness);
            delay(10);
          }
        }

        // Remettre en mode digital normal
        ledcDetach(BUILTIN_LED_PIN);
        pinMode(BUILTIN_LED_PIN, OUTPUT);
        digitalWrite(BUILTIN_LED_PIN, LOW);

        Serial.println("[LED] Fade terminé");
      } else {
        message = "Erreur: Fade non supporté sur GPIO " + String(BUILTIN_LED_PIN);
        success = false;
        Serial.println("[LED] Erreur: PWM non supporté sur ce GPIO");
      }
    }
    else {
      message = "Action inconnue: " + action;
      success = false;
      Serial.println("[LED] Action inconnue: " + action);
    }

    String json = "{";
    json += "\"success\":" + String(success ? "true" : "false") + ",";
    json += "\"message\":\"" + message + "\"";
    json += "}";

    server.send(200, "application/json", json);
  } else {
    server.send(400, "application/json", "{\"success\":false,\"message\":\"Paramètre 'action' manquant\"}");
  }
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
      server.send(200, "application/json",
                  "{\"success\":true,\"message\":\"Config GPIO " + String(LED_PIN) + "\"}");
      return;
    }
  }
  server.send(400, "application/json", "{\"success\":false}");
}

void handleNeoPixelTest() {
  resetNeoPixelTest();
  testNeoPixel();
  server.send(200, "application/json",
              "{\"success\":" + String(neopixelAvailable ? "true" : "false") +
              ",\"result\":\"" + neopixelTestResult + "\"}");
}

void handleNeoPixelPattern() {
  if (!neopixelAvailable || strip == nullptr) {
    server.send(200, "application/json", "{\"success\":false,\"message\":\"NeoPixel non disponible\"}");
    return;
  }
  
  if (server.hasArg("pattern")) {
    String pattern = server.arg("pattern");
    String message = "";

    Serial.println("[NeoPixel] Pattern demandé: " + pattern);

    // Répondre IMMÉDIATEMENT au navigateur
    String json = "{";
    json += "\"success\":true,";
    json += "\"message\":\"Pattern '" + pattern + "' démarré...\"";
    json += "}";
    server.send(200, "application/json", json);

    // PUIS exécuter le pattern
    if (pattern == "rainbow") {
      Serial.println("[NeoPixel] Arc-en-ciel démarré...");
      for (int j = 0; j < 256; j++) {
        for (int i = 0; i < LED_COUNT; i++) {
          int pixelHue = (i * 65536L / LED_COUNT) + (j * 256);
          strip->setPixelColor(i, strip->gamma32(strip->ColorHSV(pixelHue, 255, 255)));
        }
        strip->show();
        delay(10);
      }
      Serial.println("[NeoPixel] Arc-en-ciel terminé");
    }
    else if (pattern == "blink") {
      Serial.println("[NeoPixel] Clignotement démarré...");
      for (int i = 0; i < 10; i++) {
        strip->fill(strip->Color(255, 0, 0));
        strip->show();
        delay(200);
        strip->clear();
        strip->show();
        delay(200);
      }
      Serial.println("[NeoPixel] Clignotement terminé");
    }
    else if (pattern == "fade") {
      Serial.println("[NeoPixel] Fade démarré...");
      for (int cycle = 0; cycle < 3; cycle++) {
        for (int brightness = 0; brightness <= 255; brightness += 5) {
          strip->fill(strip->Color(brightness, brightness, brightness));
          strip->show();
          delay(10);
        }
        for (int brightness = 255; brightness >= 0; brightness -= 5) {
          strip->fill(strip->Color(brightness, brightness, brightness));
          strip->show();
          delay(10);
        }
      }
      Serial.println("[NeoPixel] Fade terminé");
    }
    else if (pattern == "chase") {
      Serial.println("[NeoPixel] Chase démarré...");
      for (int round = 0; round < 20; round++) {
        for (int i = 0; i < LED_COUNT; i++) {
          strip->clear();
          strip->setPixelColor(i, strip->Color(0, 0, 255));
          if (i > 0) strip->setPixelColor(i - 1, strip->Color(0, 0, 128));
          if (i > 1) strip->setPixelColor(i - 2, strip->Color(0, 0, 64));
          strip->show();
          delay(50);
        }
      }
      strip->clear();
      strip->show();
      Serial.println("[NeoPixel] Chase terminé");
    }
    else if (pattern == "off") {
      Serial.println("[NeoPixel] Extinction...");
      strip->clear();
      strip->show();
      Serial.println("[NeoPixel] Éteint");
    }
    else {
      Serial.println("[NeoPixel] Pattern inconnu: " + pattern);
    }
  } else {
    server.send(400, "application/json", "{\"success\":false,\"message\":\"Paramètre 'pattern' manquant\"}");
  }
}

void handleNeoPixelColor() {
  if (!neopixelAvailable || strip == nullptr) {
    server.send(200, "application/json", "{\"success\":false,\"message\":\"NeoPixel non disponible\"}");
    return;
  }
  
  if (server.hasArg("r") && server.hasArg("g") && server.hasArg("b")) {
    int r = server.arg("r").toInt();
    int g = server.arg("g").toInt();
    int b = server.arg("b").toInt();

    // Valider les valeurs
    r = constrain(r, 0, 255);
    g = constrain(g, 0, 255);
    b = constrain(b, 0, 255);

    Serial.printf("[NeoPixel] Couleur personnalisée: RGB(%d, %d, %d)\n", r, g, b);

    // Appliquer la couleur à tous les LEDs
    strip->fill(strip->Color(r, g, b));
    strip->show();

    String json = "{";
    json += "\"success\":true,";
    json += "\"message\":\"Couleur RGB(" + String(r) + "," + String(g) + "," + String(b) + ") appliquée\",";
    json += "\"color\":{\"r\":" + String(r) + ",\"g\":" + String(g) + ",\"b\":" + String(b) + "}";
    json += "}";

    server.send(200, "application/json", json);
    Serial.println("[NeoPixel] Couleur appliquée avec succès");
  } else {
    server.send(400, "application/json", "{\"success\":false,\"message\":\"Paramètres r, g, b manquants\"}");
  }
}

void handleTFTTest() {
  resetTFTTest();
  testTFT();
  server.send(200, "application/json",
              "{\"success\":" + String(tftAvailable ? "true" : "false") +
              ",\"result\":\"" + tftTestResult +
              "\",\"width\":" + String(tftWidth) +
              ",\"height\":" + String(tftHeight) + "}");
}

void handleTFTPattern() {
  if (!server.hasArg("pattern")) {
    server.send(400, "application/json", "{\"success\":false}");
    return;
  }
  
  String pattern = server.arg("pattern");
  String message = "";
  
  if (pattern == "colors") {
    tftTestColors();
    message = "Couleurs OK";
  } else if (pattern == "checkerboard") {
    tftTestCheckerboard();
    message = "Damier OK";
  } else if (pattern == "clear") {
    tftClear();
    tftTested = false;
    message = "Ecran efface";
  } else {
    server.send(400, "application/json", "{\"success\":false}");
    return;
  }
  
  server.send(200, "application/json",
              "{\"success\":true,\"message\":\"" + message + "\"}");
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
      server.send(200, "application/json",
                  "{\"success\":true,\"message\":\"I2C reconfigure: SDA:" + String(I2C_SDA) + " SCL:" + String(I2C_SCL) + "\"}");
      return;
    }
  }
  server.send(400, "application/json",
              "{\"success\":false,\"message\":\"Pins invalides\"}");
}

void handleOLEDTest() {
  resetOLEDTest();
  testOLED();
  server.send(200, "application/json",
              "{\"success\":" + String(oledAvailable ? "true" : "false") +
              ",\"result\":\"" + oledTestResult + "\"}");
}

void handleOLEDMessage() {
  if (!server.hasArg("message")) {
    server.send(400, "application/json", "{\"success\":false}");
    return;
  }
  
  String message = server.arg("message");
  oledShowMessage(message);
  server.send(200, "application/json",
              "{\"success\":true,\"message\":\"Message affiche\"}");
}

void handleADCTest() {
  testADC();
  String json = "{\"readings\":[";
  for (size_t i = 0; i < adcReadings.size(); i++) {
    if (i > 0) json += ",";
    json += "{\"pin\":" + String(adcReadings[i].pin) +
            ",\"raw\":" + String(adcReadings[i].rawValue) +
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
    json += "{\"pin\":" + String(touchReadings[i].pin) +
            ",\"value\":" + String(touchReadings[i].value) + "}";
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
  Serial.println("\n[STRESS TEST] Démarrage...");

  unsigned long startTime = millis();

  // Test 1 : Calculs intensifs CPU
  Serial.println("Test CPU intensif...");
  volatile float result = 0;
  for (int i = 0; i < 100000; i++) {
    result += sqrt(i) * sin(i) * cos(i);
  }
  unsigned long cpuTime = millis() - startTime;

  // Test 2 : Allocations mémoire
  Serial.println("Test allocations mémoire...");
  startTime = millis();
  const int numAllocs = 100;
  void* ptrs[numAllocs];
  int successCount = 0;

  for (int i = 0; i < numAllocs; i++) {
    ptrs[i] = malloc(1024); // 1KB par allocation
    if (ptrs[i] != NULL) {
      memset(ptrs[i], 0xAA, 1024); // Remplir la mémoire
      successCount++;
    }
  }

  // Libérer la mémoire
  for (int i = 0; i < numAllocs; i++) {
    if (ptrs[i] != NULL) {
      free(ptrs[i]);
    }
  }

  unsigned long memTime = millis() - startTime;

  // Test 3 : Fragmentation
  collectDetailedMemory();

  String json = "{";
  json += "\"success\":true,";
  json += "\"result\":\"Test terminé\",";
  json += "\"cpu_time\":" + String(cpuTime) + ",";
  json += "\"mem_time\":" + String(memTime) + ",";
  json += "\"allocs_success\":" + String(successCount) + ",";
  json += "\"allocs_total\":" + String(numAllocs) + ",";
  json += "\"fragmentation\":" + String(detailedMemory.fragmentationPercent) + ",";
  json += "\"sram_free\":" + String(detailedMemory.sramFree) + ",";
  json += "\"message\":\"CPU: " + String(cpuTime) + "ms | Mem: " + String(memTime) + "ms | Allocs: " + String(successCount) + "/" + String(numAllocs) + " | Frag: " + String(detailedMemory.fragmentationPercent, 1) + "%\"";
  json += "}";

  server.send(200, "application/json", json);

  Serial.println("[STRESS TEST] Terminé !");
  Serial.println("- Temps CPU: " + String(cpuTime) + " ms");
  Serial.println("- Temps Mémoire: " + String(memTime) + " ms");
  Serial.println("- Allocations réussies: " + String(successCount) + "/" + String(numAllocs));
  Serial.println("- Fragmentation: " + String(detailedMemory.fragmentationPercent, 1) + "%");
}

void handleBenchmark() {
  unsigned long cpuTime = benchmarkCPU();
  unsigned long memTime = benchmarkMemory();
  
  diagnosticData.cpuBenchmark = cpuTime;
  diagnosticData.memBenchmark = memTime;
  
  server.send(200, "application/json",
              "{\"cpu\":" + String(cpuTime) +
              ",\"memory\":" + String(memTime) +
              ",\"cpuPerf\":" + String(100000.0 / cpuTime, 2) +
              ",\"memSpeed\":" + String((10000 * sizeof(int) * 2) / (float)memTime, 2) + "}");
}

void handleMemoryDetails() {
  collectDetailedMemory();
  
  String json = "{\"flash\":{\"real\":" + String(detailedMemory.flashSizeReal) +
                ",\"chip\":" + String(detailedMemory.flashSizeChip) + "},";
  json += "\"psram\":{\"available\":" + String(detailedMemory.psramAvailable ? "true" : "false") + 
          ",\"configured\":" + String(detailedMemory.psramConfigured ? "true" : "false") + 
          ",\"total\":" + String(detailedMemory.psramTotal) +
          ",\"free\":" + String(detailedMemory.psramFree) + "},";
  json += "\"sram\":{\"total\":" + String(detailedMemory.sramTotal) +
          ",\"free\":" + String(detailedMemory.sramFree) + "},";
  json += "\"fragmentation\":" + String(detailedMemory.fragmentationPercent, 1) +
          ",\"status\":\"" + detailedMemory.memoryStatus + "\"}";

  server.send(200, "application/json", json);
}

// ========== HANDLERS API TEMPS RÉEL v3.0-dev ==========

void handleAPIStatus() {
  collectDiagnosticInfo();
  collectDetailedMemory();

  String json = "{";
  json += "\"uptime\":" + String(diagnosticData.uptime) + ",";
  json += "\"temperature\":" + String(diagnosticData.temperature) + ",";
  json += "\"sram\":{";
  json += "\"total\":" + String(detailedMemory.sramTotal) + ",";
  json += "\"free\":" + String(detailedMemory.sramFree) + ",";
  json += "\"used\":" + String(detailedMemory.sramUsed);
  json += "},";
  json += "\"psram\":{";
  json += "\"total\":" + String(detailedMemory.psramTotal) + ",";
  json += "\"free\":" + String(detailedMemory.psramFree) + ",";
  json += "\"used\":" + String(detailedMemory.psramUsed);
  json += "},";
  json += "\"fragmentation\":" + String(detailedMemory.fragmentationPercent);
  json += "}";

  server.send(200, "application/json", json);
}

void handleAPISystemInfo() {
  collectDiagnosticInfo();

  String json = "{";
  json += "\"chipModel\":\"" + diagnosticData.chipModel + "\",";
  json += "\"ipAddress\":\"" + diagnosticData.ipAddress + "\"";
  json += "}";

  server.send(200, "application/json", json);
}

void handleAPIOverview() {
  collectDiagnosticInfo();
  collectDetailedMemory();

  String json = "{";

  json += "\"chip\":{";
  json += "\"model\":\"" + diagnosticData.chipModel + "\",";
  json += "\"revision\":\"" + diagnosticData.chipRevision + "\",";
  json += "\"cores\":" + String(diagnosticData.cpuCores) + ",";
  json += "\"freq\":" + String(diagnosticData.cpuFreqMHz) + ",";
  json += "\"mac\":\"" + diagnosticData.macAddress + "\",";
  json += "\"uptime\":" + String(diagnosticData.uptime) + ",";
  json += "\"temperature\":" + String(diagnosticData.temperature);
  json += "},";

  json += "\"memory\":{";
  json += "\"flash\":{";
  json += "\"real\":" + String(detailedMemory.flashSizeReal) + ",";
  json += "\"config\":" + String(detailedMemory.flashSizeChip) + ",";
  json += "\"free\":0,";
  json += "\"type\":\"" + getFlashType() + "\",";
  json += "\"speed\":\"" + getFlashSpeed() + "\"";
  json += "},";
  json += "\"psram\":{";
  json += "\"total\":" + String(detailedMemory.psramTotal) + ",";
  json += "\"free\":" + String(detailedMemory.psramFree) + ",";
  json += "\"used\":" + String(detailedMemory.psramUsed);
  json += "},";
  json += "\"sram\":{";
  json += "\"total\":" + String(detailedMemory.sramTotal) + ",";
  json += "\"free\":" + String(detailedMemory.sramFree) + ",";
  json += "\"used\":" + String(detailedMemory.sramUsed);
  json += "},";
  json += "\"fragmentation\":" + String(detailedMemory.fragmentationPercent);
  json += "},";

  json += "\"wifi\":{";
  json += "\"ssid\":\"" + diagnosticData.wifiSSID + "\",";
  json += "\"rssi\":" + String(diagnosticData.wifiRSSI) + ",";
  json += "\"ip\":\"" + diagnosticData.ipAddress + "\",";
  json += "\"quality\":\"" + getWiFiSignalQuality() + "\",";
  json += "\"subnet\":\"" + WiFi.subnetMask().toString() + "\",";
  json += "\"gateway\":\"" + WiFi.gatewayIP().toString() + "\",";
  json += "\"dns\":\"" + WiFi.dnsIP().toString() + "\"";
  json += "},";

  json += "\"gpio\":{";
  json += "\"total\":" + String(diagnosticData.totalGPIO) + ",";
  json += "\"list\":\"" + diagnosticData.gpioList + "\",";
  json += "\"i2c_count\":" + String(diagnosticData.i2cCount) + ",";
  json += "\"i2c_devices\":\"" + diagnosticData.i2cDevices + "\"";
  json += "},";
  json += "\"sensors\":" + sensorsToJson();
  json += "}";

  server.send(200, "application/json", json);
}

void handleAPIMemory() {
  collectDetailedMemory();

  String json = "{";
  json += "\"flash\":{";
  json += "\"real\":" + String(detailedMemory.flashSizeReal) + ",";
  json += "\"config\":" + String(detailedMemory.flashSizeChip) + ",";
  json += "\"type\":\"" + getFlashType() + "\",";
  json += "\"speed\":\"" + getFlashSpeed() + "\"";
  json += "},";
  json += "\"psram\":{";
  json += "\"total\":" + String(detailedMemory.psramTotal) + ",";
  json += "\"free\":" + String(detailedMemory.psramFree) + ",";
  json += "\"used\":" + String(detailedMemory.psramUsed) + ",";
  json += "\"available\":" + String(detailedMemory.psramAvailable ? "true" : "false");
  json += "},";
  json += "\"sram\":{";
  json += "\"total\":" + String(detailedMemory.sramTotal) + ",";
  json += "\"free\":" + String(detailedMemory.sramFree) + ",";
  json += "\"used\":" + String(detailedMemory.sramUsed);
  json += "},";
  json += "\"fragmentation\":" + String(detailedMemory.fragmentationPercent) + ",";
  json += "\"status\":\"" + detailedMemory.memoryStatus + "\"";
  json += "}";

  server.send(200, "application/json", json);
}

void handleAPIWiFiInfo() {
  String json = "{";
  json += "\"ssid\":\"" + diagnosticData.wifiSSID + "\",";
  json += "\"rssi\":" + String(diagnosticData.wifiRSSI) + ",";
  json += "\"quality\":\"" + getWiFiSignalQuality() + "\",";
  json += "\"ip\":\"" + diagnosticData.ipAddress + "\",";
  json += "\"subnet\":\"" + WiFi.subnetMask().toString() + "\",";
  json += "\"gateway\":\"" + WiFi.gatewayIP().toString() + "\",";
  json += "\"dns\":\"" + WiFi.dnsIP().toString() + "\"";
  json += "}";

  server.send(200, "application/json", json);
}

void handleAPIPeripherals() {
  String json = "{";
  json += "\"i2c\":{";
  json += "\"count\":" + String(diagnosticData.i2cCount) + ",";
  json += "\"devices\":\"" + diagnosticData.i2cDevices + "\"";
  json += "},";
  json += "\"spi\":\"" + spiInfo + "\"";
  json += "}";

  server.send(200, "application/json", json);
}

void handleAPILedsInfo() {
  String json = "{";
  json += "\"builtin\":{";
  json += "\"pin\":" + String(BUILTIN_LED_PIN) + ",";
  json += "\"status\":\"" + builtinLedTestResult + "\",";
  json += "\"available\":" + String(builtinLedAvailable ? "true" : "false");
  json += "},";
  json += "\"neopixel\":{";
  json += "\"pin\":" + String(LED_PIN) + ",";
  json += "\"count\":" + String(LED_COUNT) + ",";
  json += "\"status\":\"" + neopixelTestResult + "\",";
  json += "\"available\":" + String(neopixelAvailable ? "true" : "false");
  json += "}";
  json += "}";

  server.send(200, "application/json", json);
}

void handleAPIScreensInfo() {
  String json = "{";
  json += "\"tft\":{";
  json += "\"status\":\"" + tftTestResult + "\",";
  json += "\"available\":" + String(tftAvailable ? "true" : "false") + ",";
  json += "\"width\":" + String(tftWidth) + ",";
  json += "\"height\":" + String(tftHeight) + ",";
  json += "\"pins\":{";
  json += "\"cs\":" + String(TFT_CS) + ",";
  json += "\"dc\":" + String(TFT_DC) + ",";
  json += "\"rst\":" + String(TFT_RST);
  json += "}";
  json += "},";
  json += "\"oled\":{";
  json += "\"status\":\"" + oledTestResult + "\",";
  json += "\"available\":" + String(oledAvailable ? "true" : "false") + ",";
  json += "\"pins\":{";
  json += "\"sda\":" + String(I2C_SDA) + ",";
  json += "\"scl\":" + String(I2C_SCL);
  json += "}";
  json += "}";
  json += "}";

  server.send(200, "application/json", json);
}

void handleAPIVersion() {
  String json = "{";
  json += "\"version\":\"" + String(DIAGNOSTIC_VERSION) + "\",";
  json += "\"build_date\":\"" + String(__DATE__) + "\",";
  json += "\"build_time\":\"" + String(__TIME__) + "\"";
  json += "}";
  
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
  txt += "TFT: " + tftTestResult + "\r\n";
  txt += "OLED: " + oledTestResult + "\r\n";
  txt += "ADC: " + adcTestResult + "\r\n";
  txt += "Touch: " + touchTestResult + "\r\n";
  txt += "PWM: " + pwmTestResult + "\r\n";
  txt += "\r\n";

  txt += "=== " + String(T().sensors_section) + " ===\r\n";
  txt += String(T().pir_sensor) + " (GPIO " + String(PIR_PIN) + "): " + sensorsDiag.pir.message + "\r\n";
  txt += String(T().distance_sensor) + " (TRIG " + String(DISTANCE_TRIG_PIN) + " / ECHO " + String(DISTANCE_ECHO_PIN) + "): " + sensorsDiag.distance.message + "\r\n";
  txt += String(T().dht_sensor) + " (" + dhtTypeToString(DHT_TYPE_CURRENT) + " GPIO " + String(DHT_PIN) + "): " + sensorsDiag.dht.message + "\r\n";
  txt += String(T().ldr_sensor) + " (GPIO " + String(LDR_PIN) + "): " + sensorsDiag.ldr.message + " (" + String(sensorsDiag.ldrVoltage, 2) + "V)\r\n";
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
  
  server.sendHeader("Content-Disposition", "attachment; filename=esp32_diagnostic_v24.txt");
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
  
  json += "\"wifi\":{";
  json += "\"ssid\":\"" + diagnosticData.wifiSSID + "\",";
  json += "\"rssi\":" + String(diagnosticData.wifiRSSI) + ",";
  json += "\"quality\":\"" + getWiFiSignalQuality() + "\",";
  json += "\"ip\":\"" + diagnosticData.ipAddress + "\",";
  json += "\"subnet\":\"" + WiFi.subnetMask().toString() + "\",";
  json += "\"gateway\":\"" + WiFi.gatewayIP().toString() + "\",";
  json += "\"dns\":\"" + WiFi.dnsIP().toString() + "\"";
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
  json += "\"tft\":\"" + tftTestResult + "\",";
  json += "\"oled\":\"" + oledTestResult + "\",";
  json += "\"adc\":\"" + adcTestResult + "\",";
  json += "\"touch\":\"" + touchTestResult + "\",";
  json += "\"pwm\":\"" + pwmTestResult + "\"";
  json += "},";

  json += "\"sensors\":" + sensorsToJson() + ",";

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
  
  server.sendHeader("Content-Disposition", "attachment; filename=esp32_diagnostic_v24.json");
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
  
  csv += "GPIO," + String(T().total_gpio) + "," + String(diagnosticData.totalGPIO) + "\r\n";
  
  csv += String(T().i2c_peripherals) + "," + String(T().device_count) + "," + String(diagnosticData.i2cCount) + "\r\n";
  csv += String(T().i2c_peripherals) + "," + String(T().devices) + "," + diagnosticData.i2cDevices + "\r\n";
  
  csv += String(T().test) + "," + String(T().builtin_led) + "," + builtinLedTestResult + "\r\n";
  csv += String(T().test) + ",NeoPixel," + neopixelTestResult + "\r\n";
  csv += String(T().test) + ",TFT," + tftTestResult + "\r\n";
  csv += String(T().test) + ",OLED," + oledTestResult + "\r\n";
  csv += String(T().test) + ",ADC," + adcTestResult + "\r\n";
  csv += String(T().test) + ",Touch," + touchTestResult + "\r\n";
  csv += String(T().test) + ",PWM," + pwmTestResult + "\r\n";

  csv += String(T().sensors_section) + "," + String(T().pir_sensor) + " (GPIO)," + String(PIR_PIN) + " - " + sensorsDiag.pir.message + "\r\n";
  csv += String(T().sensors_section) + "," + String(T().distance_sensor) + " (TRIG/ECHO)," + String(DISTANCE_TRIG_PIN) + "/" + String(DISTANCE_ECHO_PIN) + " - " + sensorsDiag.distance.message + "\r\n";
  csv += String(T().sensors_section) + "," + String(T().dht_sensor) + " (" + dhtTypeToString(DHT_TYPE_CURRENT) + ")," + String(DHT_PIN) + " - " + sensorsDiag.dht.message + "\r\n";
  csv += String(T().sensors_section) + "," + String(T().ldr_sensor) + " (GPIO)," + String(LDR_PIN) + " - " + sensorsDiag.ldr.message + " (" + String(sensorsDiag.ldrVoltage, 2) + "V)\r\n";

  if (diagnosticData.cpuBenchmark > 0) {
    csv += String(T().performance_bench) + ",CPU us," + String(diagnosticData.cpuBenchmark) + "\r\n";
    csv += String(T().performance_bench) + "," + String(T().memory_benchmark) + " us," + String(diagnosticData.memBenchmark) + "\r\n";
  }
  
  csv += "System," + String(T().uptime) + " ms," + String(diagnosticData.uptime) + "\r\n";
  csv += "System," + String(T().last_reset) + "," + getResetReason() + "\r\n";
  
  server.sendHeader("Content-Disposition", "attachment; filename=esp32_diagnostic_v24.csv");
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
  html += "<tr><td>Écran TFT</td><td>" + tftTestResult + "</td></tr>";
  html += "<tr><td>Écran OLED</td><td>" + oledTestResult + "</td></tr>";
  html += "<tr><td>ADC</td><td>" + adcTestResult + "</td></tr>";
  html += "<tr><td>Touch Pads</td><td>" + touchTestResult + "</td></tr>";
  html += "<tr><td>PWM</td><td>" + pwmTestResult + "</td></tr>";
  html += "</table></div>";

  html += "<div class='section'>";
  html += "<h2>" + String(T().sensors_section) + "</h2>";
  html += "<table>";
  html += "<tr><th>" + String(T().parameter) + "</th><th>" + String(T().value) + "</th></tr>";
  html += "<tr><td>" + String(T().pir_sensor) + " (GPIO " + String(PIR_PIN) + ")</td><td>" + sensorsDiag.pir.message + (sensorsDiag.pir.tested ? (sensorsDiag.pirMotion ? " - " + String(T().motion_detected) : " - " + String(T().motion_none)) : "") + "</td></tr>";
  html += "<tr><td>" + String(T().distance_sensor) + " (TRIG " + String(DISTANCE_TRIG_PIN) + " / ECHO " + String(DISTANCE_ECHO_PIN) + ")</td><td>" + sensorsDiag.distance.message + "</td></tr>";
  html += "<tr><td>" + String(T().dht_sensor) + " (" + dhtTypeToString(DHT_TYPE_CURRENT) + " GPIO " + String(DHT_PIN) + ")</td><td>" + sensorsDiag.dht.message + "</td></tr>";
  html += "<tr><td>" + String(T().ldr_sensor) + " (GPIO " + String(LDR_PIN) + ")</td><td>" + sensorsDiag.ldr.message + " - " + String(sensorsDiag.ldrVoltage, 2) + "V</td></tr>";
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

// ========== HANDLERS LANGUE ==========

void handleSetLanguage() {
  if (server.hasArg("lang")) {
    String lang = server.arg("lang");

    if (lang == "fr") {
      currentLanguage = LANG_FR;
      Serial.println("Langue changée: Français");
    } else if (lang == "en") {
      currentLanguage = LANG_EN;
      Serial.println("Langue changée: English");
    }

    String json = "{";
    json += "\"success\":true,";
    json += "\"language\":\"" + lang + "\",";
    json += "\"message\":\"Langue changée en " + lang + "\"";
    json += "}";

    server.send(200, "application/json", json);
  } else {
    server.send(400, "application/json", "{\"success\":false,\"message\":\"Paramètre manquant\"}");
  }
}

void handleTFTText() {
  if (!tftAvailable) {
    server.send(200, "application/json", "{\"success\":false,\"message\":\"TFT non disponible\"}");
    return;
  }

  if (server.hasArg("text")) {
    String text = server.arg("text");

    #ifdef USE_TFT
    tft.fillScreen(TFT_BLACK);
    tft.setCursor(10, 10);
    tft.setTextColor(TFT_WHITE);
    tft.setTextSize(2);
    tft.println(text);
    #endif

    String json = "{";
    json += "\"success\":true,";
    json += "\"message\":\"Texte affiché: " + text + "\"";
    json += "}";

    server.send(200, "application/json", json);
    Serial.println("[TFT] Texte affiché: " + text);
  } else {
    server.send(400, "application/json", "{\"success\":false,\"message\":\"Paramètre 'text' manquant\"}");
  }
}

void handleGetTranslations() {
  String json = "{";
  json += "\"title\":\"" + String(T().title) + "\",";
  json += "\"nav_overview\":\"" + String(T().nav_overview) + "\",";
  json += "\"nav_leds\":\"" + String(T().nav_leds) + "\",";
  json += "\"nav_screens\":\"" + String(T().nav_screens) + "\",";
  json += "\"nav_tests\":\"" + String(T().nav_tests) + "\",";
  json += "\"nav_gpio\":\"" + String(T().nav_gpio) + "\",";
  json += "\"nav_wifi\":\"" + String(T().nav_wifi) + "\",";
  json += "\"nav_benchmark\":\"" + String(T().nav_benchmark) + "\",";
  json += "\"nav_export\":\"" + String(T().nav_export) + "\"";
  json += "}";
  
  server.send(200, "application/json", json);
}

// ========== SETUP COMPLET ==========

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  Serial.println("\r\n===============================================");
  Serial.println("     DIAGNOSTIC ESP32 v" + String(DIAGNOSTIC_VERSION));
  Serial.println("     Interface Web Dynamique");
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
  
  // Détections matériel
  detectBuiltinLED();
  detectNeoPixelSupport();

  if (strip != nullptr) {
    strip->begin();
    strip->clear();
    strip->show();
  }
  
  detectTFT();
  detectOLED();

  if (ENABLE_I2C_SCAN) {
    scanI2C();
  }

  initializeSensors();

  scanSPI();
  listPartitions();
  
  collectDiagnosticInfo();
  collectDetailedMemory();
  
  // ========== CONFIGURATION ROUTES SERVEUR v3.0-dev ==========

  Serial.println("Configuration du serveur web...");

  // Route principale - Interface web dynamique
  server.on("/", HTTP_GET, handleRoot);

  // JavaScript
  server.on("/js/app.js", HTTP_GET, handleJavaScript);

  // ========== API - Temps réel ==========
  server.on("/api/status", HTTP_GET, handleAPIStatus);
  server.on("/api/system-info", HTTP_GET, handleAPISystemInfo);
  server.on("/api/overview", HTTP_GET, handleAPIOverview);
  server.on("/api/memory", HTTP_GET, handleAPIMemory);
  server.on("/api/wifi-info", HTTP_GET, handleAPIWiFiInfo);
  server.on("/api/peripherals", HTTP_GET, handleAPIPeripherals);
  server.on("/api/leds-info", HTTP_GET, handleAPILedsInfo);
  server.on("/api/screens-info", HTTP_GET, handleAPIScreensInfo);
  server.on("/api/sensors-info", HTTP_GET, handleSensorsInfo);

  // ========== API - Langues ==========
  server.on("/api/set-language", HTTP_GET, handleSetLanguage);
  server.on("/api/get-translations", HTTP_GET, handleGetTranslations);

  // ========== API - GPIO & WiFi ==========
  server.on("/api/test-gpio", HTTP_GET, handleTestGPIO);
  server.on("/api/wifi-scan", HTTP_GET, handleWiFiScan);
  server.on("/api/i2c-scan", HTTP_GET, handleI2CScan);

  // ========== API - LED intégrée ==========
  server.on("/api/builtin-led-config", HTTP_GET, handleBuiltinLEDConfig);
  server.on("/api/builtin-led-test", HTTP_GET, handleBuiltinLEDTest);
  server.on("/api/builtin-led-control", HTTP_GET, handleBuiltinLEDControl);

  // ========== API - NeoPixel ==========
  server.on("/api/neopixel-config", HTTP_GET, handleNeoPixelConfig);
  server.on("/api/neopixel-test", HTTP_GET, handleNeoPixelTest);
  server.on("/api/neopixel-pattern", HTTP_GET, handleNeoPixelPattern);
  server.on("/api/neopixel-color", HTTP_GET, handleNeoPixelColor);

  // ========== API - Écrans ==========
  server.on("/api/tft-test", HTTP_GET, handleTFTTest);
  server.on("/api/tft-pattern", HTTP_GET, handleTFTPattern);
  server.on("/api/oled-test", HTTP_GET, handleOLEDTest);
  server.on("/api/oled-message", HTTP_GET, handleOLEDMessage);
  server.on("/api/oled-config", HTTP_GET, handleOLEDConfig);
  server.on("/api/tft-text", HTTP_GET, handleTFTText);
  server.on("/api/sensor-test", HTTP_GET, handleSensorTest);
  server.on("/api/sensor-config", HTTP_GET, handleSensorConfig);
  // ========== API - Tests avancés ==========
  server.on("/api/adc-test", HTTP_GET, handleADCTest);
  server.on("/api/touch-test", HTTP_GET, handleTouchTest);
  server.on("/api/pwm-test", HTTP_GET, handlePWMTest);
  server.on("/api/spi-scan", HTTP_GET, handleSPIScan);
  server.on("/api/partitions-list", HTTP_GET, handlePartitionsList);
  server.on("/api/stress-test", HTTP_GET, handleStressTest);

  // ========== API - Performance & Mémoire ==========
  server.on("/api/benchmark", HTTP_GET, handleBenchmark);
  server.on("/api/memory-details", HTTP_GET, handleMemoryDetails);

  // ========== Exports ==========
  server.on("/export/txt", HTTP_GET, handleExportTXT);
  server.on("/export/json", HTTP_GET, handleExportJSON);
  server.on("/export/csv", HTTP_GET, handleExportCSV);
  server.on("/print", HTTP_GET, handlePrintVersion);

  // Démarrer le serveur
  server.begin();

  Serial.println("Serveur Web OK!");
  Serial.println("\r\n===============================================");
  Serial.println("            PRET - v" + String(DIAGNOSTIC_VERSION));
  Serial.println("   Interface Web 100% Dynamique");
  Serial.println("   Mise a jour auto toutes les 5s");
  Serial.println("   Architecture API REST");
  Serial.println("===============================================\r\n");
}

// ========== LOOP ==========

void loop() {
  // Gestion des requêtes HTTP
  server.handleClient();
  
  // Mise à jour périodique des données (toutes les 30 secondes)
  static unsigned long lastUpdate = 0;
  if (millis() - lastUpdate > 30000) {
    lastUpdate = millis();
    collectDiagnosticInfo();
    
    Serial.println("\r\n=== UPDATE AUTOMATIQUE ===");
    Serial.printf("Heap libre: %.2f KB | Uptime: %.2f h\r\n",
                  diagnosticData.freeHeap / 1024.0, 
                  diagnosticData.uptime / 3600000.0);
    if (diagnosticData.temperature != -999) {
      Serial.printf("Temperature: %.1f°C\r\n", diagnosticData.temperature);
    }
    Serial.printf("Clients connectes: %d\r\n", WiFi.softAPgetStationNum());
  }
  
  // Petit délai pour éviter le watchdog
  delay(10);
}
