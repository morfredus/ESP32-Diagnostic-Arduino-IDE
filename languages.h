#ifndef LANGUAGES_H
#define LANGUAGES_H

// ========== SYSTÈME DE TRADUCTION ==========

enum Language {
  LANG_FR = 0,
  LANG_EN = 1
};

// Variable globale pour la langue courante
Language currentLanguage = LANG_FR;

// Structure pour les traductions
struct Translations {
  // Titres et navigation
  const char* title;
  const char* version;
  const char* access;
  const char* or_text;
  
  // Navigation
  const char* nav_overview;
  const char* nav_leds;
  const char* nav_screens;
  const char* nav_sensors;
  const char* nav_tests;
  const char* nav_gpio;
  const char* nav_wifi;
  const char* nav_benchmark;
  const char* nav_export;
  
  // Sections Vue d'ensemble
  const char* chip_info;
  const char* full_model;
  const char* cpu_cores;
  const char* mac_wifi;
  const char* last_reset;
  const char* chip_features;
  const char* sdk_version;
  const char* idf_version;
  const char* uptime;
  const char* cpu_temp;
  
  // Mémoire
  const char* memory_details;
  const char* flash_memory;
  const char* real_size;
  const char* configured_ide;
  const char* configuration;
  const char* correct;
  const char* to_fix;
  const char* flash_type;
  const char* flash_speed;
  const char* action_required;
  
  const char* psram_external;
  const char* hardware_status;
  const char* detected_active;
  const char* ide_config;
  const char* enabled;
  const char* total_size;
  const char* free;
  const char* used;
  const char* largest_block;
  const char* integrity_test;
  const char* not_detected;
  const char* disabled;
  const char* psram_usage;
  
  const char* internal_sram;
  const char* min_free;
  const char* max_alloc;
  
  const char* global_analysis;
  const char* memory_fragmentation;
  const char* memory_status;
  const char* total_ram;
  const char* free_ram_total;
  const char* refresh_memory;
  
  // WiFi
  const char* wifi_connection;
  const char* connected_ssid;
  const char* signal_power;
  const char* signal_quality;
  const char* ip_address;
  const char* subnet_mask;
  const char* gateway;
  const char* dns;
  const char* wifi_channel;
  
  // GPIO et périphériques
  const char* gpio_interfaces;
  const char* total_gpio;
  const char* gpio_list;
  const char* i2c_peripherals;
  const char* device_count;
  const char* detected_addresses;
  const char* rescan_i2c;
  
  // LEDs
  const char* builtin_led;
  const char* gpio;
  const char* status;
  const char* config;
  const char* test;
  const char* blink;
  const char* fade;
  const char* off;
  const char* neopixel;
  const char* rainbow;
  const char* color;
  
  // Écrans
  const char* tft_screen;
  const char* spi_pins;
  const char* full_test;
  const char* colors;
  const char* checkerboard;
  const char* clear;
  
  const char* oled_screen;
  const char* i2c_pins;
  const char* i2c_address;
  const char* no_device;
  const char* pin_config;
  const char* apply_redetect;
  const char* changes_pins;
  const char* custom_message;
  const char* show_message;
  const char* no_detected;
  const char* check_wiring;
  
  // Tests avancés
  const char* adc_test;
  const char* touch_test;
  const char* pwm_test;
  const char* spi_bus;
  const char* flash_partitions;
  const char* list_partitions;
  const char* click_button;
  const char* memory_stress;
  const char* start_stress;
  
  // GPIO
  const char* gpio_test;
  const char* test_all_gpio;
  const char* click_to_test;
  
  // WiFi Scanner
  const char* wifi_scanner;
  const char* scan_networks;
  
  // Performance
  const char* performance_bench;
  const char* run_benchmarks;
  const char* cpu_benchmark;
  const char* memory_benchmark;
  const char* cpu_performance;
  const char* memory_speed;
  const char* not_tested;
  
  // Export
  const char* data_export;
  const char* txt_file;
  const char* readable_report;
  const char* download_txt;
  const char* json_file;
  const char* structured_format;
  const char* download_json;
  const char* csv_file;
  const char* for_excel;
  const char* download_csv;
  const char* printable_version;
  const char* pdf_format;
  const char* open;

    // Nouveaux champs pour les exports
  const char* export_report;
  const char* export_generated;
  const char* export_after_boot;
  const char* category;
  const char* parameter;
  const char* value;
  const char* model;
  const char* revision;
  const char* frequency;
  const char* real;
  const char* board;
  const char* psram_mb;
  const char* detected;
  const char* sram_kb;
  const char* connected;
  const char* signal;
  const char* list;
  const char* days;
  const char* hours;

  // Aide interface dynamique
  const char* loading;
  const char* error_title;
  const char* error_generic;
  const char* update_indicator;
  const char* animations;
  const char* custom_color;
  const char* apply_color;
  const char* turn_off_all;
  const char* resolution;
  const char* info_label;
  const char* shapes_label;
  const char* grid_label;
  const char* scroll_label;
  const char* display_text;
  const char* display_message;
  const char* stress_warning;
  const char* click_to_scan;
  const char* language_changed;
  const char* language_error;
  const char* displaying;
  const char* command_sent;
  const char* communication_error;

  // Capteurs
  const char* sensors_section;
  const char* pir_sensor;
  const char* distance_sensor;
  const char* dht_sensor;
  const char* ldr_sensor;
  const char* trig_pin_label;
  const char* echo_pin_label;
  const char* sensor_type;
  const char* temperature_label;
  const char* humidity_label;
  const char* voltage_label;
  const char* motion_detected;
  const char* motion_none;
  const char* distance_value;
  const char* light_level;
  const char* sensor_config;
  const char* apply_changes;
  const char* run_sensor_test;

  // Statuts et messages
  const char* ok;
  const char* fail;
  const char* excellent;
  const char* very_good;
  const char* good;
  const char* warning;
  const char* critical;
  const char* weak;
  const char* very_weak;
  const char* none;
  const char* unknown;
  const char* testing;
  const char* completed;
  const char* scan;
  const char* scanning;
  
  // Unités
  const char* cores;
  const char* pins;
  const char* devices;
  const char* networks;
  const char* tested;
  const char* channels;
  
  // Actions
  const char* poweron;
  const char* software_reset;
  const char* deepsleep_exit;
  const char* brownout;
  const char* other;
};

// Traductions françaises
const Translations LANG_FR_TEXTS = {
  // Titres
  "ESP32 Diagnostic Exhaustif",
  "v",
  "Accès",
  "ou",
  
  // Navigation
  "Vue d'ensemble",
  "LEDs",
  "Écrans",
  "Capteurs",
  "Tests Avancés",
  "GPIO",
  "WiFi",
  "Performance",
  "Export",
  
  // Chip
  "Informations Chip Détaillées",
  "Modèle Complet",
  "Coeurs CPU",
  "Adresse MAC WiFi",
  "Raison Dernier Reset",
  "Fonctionnalités Chip",
  "Version SDK",
  "Version ESP-IDF",
  "Uptime",
  "Température CPU",
  
  // Mémoire
  "Mémoire Détaillée",
  "Flash Memory",
  "Taille Réelle (carte)",
  "Configurée (IDE)",
  "Configuration",
  "Correcte",
  "À corriger",
  "Type Flash",
  "Vitesse Flash",
  "Action Requise",
  
  "PSRAM (Mémoire Externe)",
  "Statut Hardware",
  "Détectée et active",
  "Configuration IDE",
  "Activée",
  "Taille Totale",
  "Libre",
  "Utilisée",
  "Plus Gros Bloc",
  "Test Intégrité",
  "Non détectée",
  "Désactivée",
  "Utilisation PSRAM",
  
  "SRAM Interne",
  "Min Libre (Record)",
  "Max Allocation",
  
  "Analyse Globale",
  "Fragmentation Mémoire",
  "État Mémoire",
  "RAM Totale (SRAM+PSRAM)",
  "RAM Libre Totale",
  "Actualiser Mémoire",
  
  // WiFi
  "Connexion WiFi Détaillée",
  "SSID Connecté",
  "Puissance Signal (RSSI)",
  "Qualité Signal",
  "Adresse IP",
  "Masque Sous-réseau",
  "Passerelle",
  "DNS",
  "Canal WiFi",
  
  // GPIO
  "GPIO et Interfaces",
  "Total GPIO",
  "Liste GPIO",
  "Périphériques I2C",
  "Nombre de Périphériques",
  "Adresses Détectées",
  "Re-scanner I2C",
  
  // LEDs
  "LED Intégrée",
  "GPIO",
  "Statut",
  "Config",
  "Test",
  "Blink",
  "Fade",
  "Off",
  "NeoPixel",
  "Rainbow",
  "Couleur",
  
  // Écrans
  "Écran TFT 320x240",
  "Pins SPI",
  "Test Complet",
  "Couleurs",
  "Damier",
  "Effacer",
  
  "Écran OLED 0.96\" I2C",
  "Pins I2C",
  "Adresse I2C",
  "aucun périphérique",
  "Configuration Pins I2C",
  "Appliquer et Re-détecter",
  "Modifie les pins I2C et relance la détection",
  "Message personnalisé",
  "Afficher Message",
  "Aucun écran OLED détecté. Vérifiez le câblage et les pins I2C ci-dessus.",
  "Vérifiez le câblage et les pins I2C",
  
  // Tests
  "Test ADC",
  "Test Touch Pads",
  "Test PWM",
  "Bus SPI",
  "Partitions Flash",
  "Lister Partitions",
  "Cliquez sur le bouton",
  "Stress Test Mémoire",
  "Lancer Stress Test",
  
  // GPIO
  "Test GPIO",
  "Tester Tous les GPIO",
  "Cliquez pour tester",
  
  // WiFi
  "Scanner WiFi",
  "Scanner Réseaux WiFi",
  
  // Performance
  "Benchmarks de Performance",
  "Lancer Benchmarks",
  "CPU Benchmark",
  "Mémoire Benchmark",
  "Performance CPU",
  "Vitesse Mémoire",
  "Non testé",
  
  // Export
  "Export des Données",
  "Fichier TXT",
  "Rapport texte lisible",
  "Télécharger TXT",
  "Fichier JSON",
  "Format structuré",
  "Télécharger JSON",
  "Fichier CSV",
  "Pour Excel",
  "Télécharger CSV",
  "Version Imprimable",
  "Format PDF",
  "Ouvrir",
  
    // Nouveaux champs exports
  "Rapport de diagnostic",
  "Rapport généré le",
  "secondes après démarrage",
  "Catégorie",
  "Paramètre",
  "Valeur",
  "Modèle",
  "Révision",
  "Fréquence",
  "Réel",
  "Carte",
  "PSRAM MB",
  "Détecté",
  "SRAM KB",
  "Connecté",
  "Signal",
  "Liste",
  "jours",
  "heures",

  // Interface dynamique
  "Chargement...",
  "❌ Erreur",
  "Une erreur est survenue",
  "Mise à jour...",
  "Animations :",
  "Couleur personnalisée :",
  "Appliquer couleur",
  "Tout éteindre",
  "Résolution",
  "Infos",
  "Formes",
  "Grille",
  "Défilement",
  "Afficher texte",
  "Afficher message",
  "⚠️ Peut ralentir l'ESP32 temporairement",
  "Cliquez pour scanner",
  "Langue changée",
  "Erreur changement langue",
  "Affichage en cours...",
  "Commande envoyée",
  "Erreur de communication",

  // Capteurs
  "Capteurs",
  "Capteur PIR",
  "Capteur distance",
  "Capteur DHT",
  "Capteur LDR",
  "Broche TRIG",
  "Broche ECHO",
  "Type",
  "Température",
  "Humidité",
  "Tension",
  "Mouvement détecté",
  "Pas de mouvement",
  "Distance",
  "Luminosité",
  "Configuration capteur",
  "Appliquer",
  "Tester le capteur",

  // Statuts
  "OK",
  "FAIL",
  "Excellent",
  "Très bon",
  "Bon",
  "Attention",
  "Critique",
  "Faible",
  "Très faible",
  "Aucune",
  "Inconnu",
  "Test...",
  "Terminé",
  "Scan...",
  "Scan...",
  
  // Unités
  "coeurs",
  "broches",
  "périphérique(s)",
  "réseaux",
  "testé(s)",
  "canaux",
  
  // Reset
  "Mise sous tension",
  "Reset logiciel",
  "Sortie de deep sleep",
  "Brownout",
  "Autre"
};

// Traductions anglaises
const Translations LANG_EN_TEXTS = {
  // Titles
  "ESP32 Complete Diagnostic",
  "v",
  "Access",
  "or",
  
  // Navigation
  "Overview",
  "LEDs",
  "Screens",
  "Sensors",
  "Advanced Tests",
  "GPIO",
  "WiFi",
  "Performance",
  "Export",
  
  // Chip
  "Detailed Chip Information",
  "Full Model",
  "CPU Cores",
  "WiFi MAC Address",
  "Last Reset Reason",
  "Chip Features",
  "SDK Version",
  "ESP-IDF Version",
  "Uptime",
  "CPU Temperature",
  
  // Memory
  "Detailed Memory",
  "Flash Memory",
  "Real Size (board)",
  "Configured (IDE)",
  "Configuration",
  "Correct",
  "To fix",
  "Flash Type",
  "Flash Speed",
  "Action Required",
  
  "PSRAM (External Memory)",
  "Hardware Status",
  "Detected and active",
  "IDE Configuration",
  "Enabled",
  "Total Size",
  "Free",
  "Used",
  "Largest Block",
  "Integrity Test",
  "Not detected",
  "Disabled",
  "PSRAM Usage",
  
  "Internal SRAM",
  "Min Free (Record)",
  "Max Allocation",
  
  "Global Analysis",
  "Memory Fragmentation",
  "Memory Status",
  "Total RAM (SRAM+PSRAM)",
  "Total Free RAM",
  "Refresh Memory",
  
  // WiFi
  "Detailed WiFi Connection",
  "Connected SSID",
  "Signal Power (RSSI)",
  "Signal Quality",
  "IP Address",
  "Subnet Mask",
  "Gateway",
  "DNS",
  "WiFi Channel",
  
  // GPIO
  "GPIO and Interfaces",
  "Total GPIO",
  "GPIO List",
  "I2C Peripherals",
  "Number of Devices",
  "Detected Addresses",
  "Re-scan I2C",
  
  // LEDs
  "Built-in LED",
  "GPIO",
  "Status",
  "Config",
  "Test",
  "Blink",
  "Fade",
  "Off",
  "NeoPixel",
  "Rainbow",
  "Color",
  
  // Screens
  "TFT Screen 320x240",
  "SPI Pins",
  "Full Test",
  "Colors",
  "Checkerboard",
  "Clear",
  
  "OLED Screen 0.96\" I2C",
  "I2C Pins",
  "I2C Address",
  "no device",
  "I2C Pin Configuration",
  "Apply and Re-detect",
  "Changes I2C pins and restarts detection",
  "Custom message",
  "Display Message",
  "No OLED screen detected. Check wiring and I2C pins above.",
  "Check wiring and I2C pins",
  
  // Tests
  "ADC Test",
  "Touch Pads Test",
  "PWM Test",
  "SPI Bus",
  "Flash Partitions",
  "List Partitions",
  "Click the button",
  "Memory Stress Test",
  "Start Stress Test",
  
  // GPIO
  "GPIO Test",
  "Test All GPIO",
  "Click to test",
  
  // WiFi
  "WiFi Scanner",
  "Scan WiFi Networks",
  
  // Performance
  "Performance Benchmarks",
  "Run Benchmarks",
  "CPU Benchmark",
  "Memory Benchmark",
  "CPU Performance",
  "Memory Speed",
  "Not tested",
  
  // Export
  "Data Export",
  "TXT File",
  "Readable text report",
  "Download TXT",
  "JSON File",
  "Structured format",
  "Download JSON",
  "CSV File",
  "For Excel",
  "Download CSV",
  "Printable Version",
  "PDF Format",
  "Open",
  
    // Nouveaux champs exports
  "Diagnostic Report",
  "Report generated at",
  "seconds after boot",
  "Category",
  "Parameter",
  "Value",
  "Model",
  "Revision",
  "Frequency",
  "Real",
  "Board",
  "PSRAM MB",
  "Detected",
  "SRAM KB",
  "Connected",
  "Signal",
  "List",
  "days",
  "hours",

  // Dynamic interface
  "Loading...",
  "❌ Error",
  "An error occurred",
  "Updating...",
  "Animations:",
  "Custom color:",
  "Apply color",
  "Turn everything off",
  "Resolution",
  "Info",
  "Shapes",
  "Grid",
  "Scrolling",
  "Display text",
  "Display message",
  "⚠️ May temporarily slow the ESP32",
  "Click to scan",
  "Language changed",
  "Language change error",
  "Displaying...",
  "Command sent",
  "Communication error",

  // Sensors
  "Sensors",
  "PIR sensor",
  "Distance sensor",
  "DHT sensor",
  "LDR sensor",
  "TRIG pin",
  "ECHO pin",
  "Type",
  "Temperature",
  "Humidity",
  "Voltage",
  "Motion detected",
  "No motion",
  "Distance",
  "Light level",
  "Sensor configuration",
  "Apply",
  "Run sensor test",

  // Status
  "OK",
  "FAIL",
  "Excellent",
  "Very good",
  "Good",
  "Warning",
  "Critical",
  "Weak",
  "Very weak",
  "None",
  "Unknown",
  "Testing...",
  "Completed",
  "Scanning...",
  "Scanning...",
  
  // Units
  "cores",
  "pins",
  "device(s)",
  "networks",
  "tested",
  "channels",
  
  // Reset
  "Power on",
  "Software reset",
  "Deep sleep exit",
  "Brownout",
  "Other"
};

// Fonction pour obtenir le texte traduit
inline const Translations& T() {
  return (currentLanguage == LANG_FR) ? LANG_FR_TEXTS : LANG_EN_TEXTS;
}

// Variante permettant de récupérer les traductions pour une langue précise
inline const Translations& T(Language lang) {
  return (lang == LANG_FR) ? LANG_FR_TEXTS : LANG_EN_TEXTS;
}

// Fonction pour changer la langue
inline void setLanguage(Language lang) {
  currentLanguage = lang;
}

struct TranslationField {
  const char* key;
  const char* const Translations::*member;
};

static const TranslationField TRANSLATION_FIELDS[] = {
  {"title",&Translations::title},
  {"version",&Translations::version},
  {"access",&Translations::access},
  {"or_text",&Translations::or_text},
  {"nav_overview",&Translations::nav_overview},
  {"nav_leds",&Translations::nav_leds},
  {"nav_screens",&Translations::nav_screens},
  {"nav_sensors",&Translations::nav_sensors},
  {"nav_tests",&Translations::nav_tests},
  {"nav_gpio",&Translations::nav_gpio},
  {"nav_wifi",&Translations::nav_wifi},
  {"nav_benchmark",&Translations::nav_benchmark},
  {"nav_export",&Translations::nav_export},
  {"chip_info",&Translations::chip_info},
  {"full_model",&Translations::full_model},
  {"cpu_cores",&Translations::cpu_cores},
  {"mac_wifi",&Translations::mac_wifi},
  {"last_reset",&Translations::last_reset},
  {"chip_features",&Translations::chip_features},
  {"sdk_version",&Translations::sdk_version},
  {"idf_version",&Translations::idf_version},
  {"uptime",&Translations::uptime},
  {"cpu_temp",&Translations::cpu_temp},
  {"memory_details",&Translations::memory_details},
  {"flash_memory",&Translations::flash_memory},
  {"real_size",&Translations::real_size},
  {"configured_ide",&Translations::configured_ide},
  {"configuration",&Translations::configuration},
  {"correct",&Translations::correct},
  {"to_fix",&Translations::to_fix},
  {"flash_type",&Translations::flash_type},
  {"flash_speed",&Translations::flash_speed},
  {"action_required",&Translations::action_required},
  {"psram_external",&Translations::psram_external},
  {"hardware_status",&Translations::hardware_status},
  {"detected_active",&Translations::detected_active},
  {"ide_config",&Translations::ide_config},
  {"enabled",&Translations::enabled},
  {"total_size",&Translations::total_size},
  {"free",&Translations::free},
  {"used",&Translations::used},
  {"largest_block",&Translations::largest_block},
  {"integrity_test",&Translations::integrity_test},
  {"not_detected",&Translations::not_detected},
  {"disabled",&Translations::disabled},
  {"psram_usage",&Translations::psram_usage},
  {"internal_sram",&Translations::internal_sram},
  {"min_free",&Translations::min_free},
  {"max_alloc",&Translations::max_alloc},
  {"global_analysis",&Translations::global_analysis},
  {"memory_fragmentation",&Translations::memory_fragmentation},
  {"memory_status",&Translations::memory_status},
  {"total_ram",&Translations::total_ram},
  {"free_ram_total",&Translations::free_ram_total},
  {"refresh_memory",&Translations::refresh_memory},
  {"wifi_connection",&Translations::wifi_connection},
  {"connected_ssid",&Translations::connected_ssid},
  {"signal_power",&Translations::signal_power},
  {"signal_quality",&Translations::signal_quality},
  {"ip_address",&Translations::ip_address},
  {"subnet_mask",&Translations::subnet_mask},
  {"gateway",&Translations::gateway},
  {"dns",&Translations::dns},
  {"wifi_channel",&Translations::wifi_channel},
  {"gpio_interfaces",&Translations::gpio_interfaces},
  {"total_gpio",&Translations::total_gpio},
  {"gpio_list",&Translations::gpio_list},
  {"i2c_peripherals",&Translations::i2c_peripherals},
  {"device_count",&Translations::device_count},
  {"detected_addresses",&Translations::detected_addresses},
  {"rescan_i2c",&Translations::rescan_i2c},
  {"builtin_led",&Translations::builtin_led},
  {"gpio",&Translations::gpio},
  {"status",&Translations::status},
  {"config",&Translations::config},
  {"test",&Translations::test},
  {"blink",&Translations::blink},
  {"fade",&Translations::fade},
  {"off",&Translations::off},
  {"neopixel",&Translations::neopixel},
  {"rainbow",&Translations::rainbow},
  {"color",&Translations::color},
  {"tft_screen",&Translations::tft_screen},
  {"spi_pins",&Translations::spi_pins},
  {"full_test",&Translations::full_test},
  {"colors",&Translations::colors},
  {"checkerboard",&Translations::checkerboard},
  {"clear",&Translations::clear},
  {"oled_screen",&Translations::oled_screen},
  {"i2c_pins",&Translations::i2c_pins},
  {"i2c_address",&Translations::i2c_address},
  {"no_device",&Translations::no_device},
  {"pin_config",&Translations::pin_config},
  {"apply_redetect",&Translations::apply_redetect},
  {"changes_pins",&Translations::changes_pins},
  {"custom_message",&Translations::custom_message},
  {"show_message",&Translations::show_message},
  {"no_detected",&Translations::no_detected},
  {"check_wiring",&Translations::check_wiring},
  {"adc_test",&Translations::adc_test},
  {"touch_test",&Translations::touch_test},
  {"pwm_test",&Translations::pwm_test},
  {"spi_bus",&Translations::spi_bus},
  {"flash_partitions",&Translations::flash_partitions},
  {"list_partitions",&Translations::list_partitions},
  {"click_button",&Translations::click_button},
  {"memory_stress",&Translations::memory_stress},
  {"start_stress",&Translations::start_stress},
  {"gpio_test",&Translations::gpio_test},
  {"test_all_gpio",&Translations::test_all_gpio},
  {"click_to_test",&Translations::click_to_test},
  {"wifi_scanner",&Translations::wifi_scanner},
  {"scan_networks",&Translations::scan_networks},
  {"performance_bench",&Translations::performance_bench},
  {"run_benchmarks",&Translations::run_benchmarks},
  {"cpu_benchmark",&Translations::cpu_benchmark},
  {"memory_benchmark",&Translations::memory_benchmark},
  {"cpu_performance",&Translations::cpu_performance},
  {"memory_speed",&Translations::memory_speed},
  {"not_tested",&Translations::not_tested},
  {"data_export",&Translations::data_export},
  {"txt_file",&Translations::txt_file},
  {"readable_report",&Translations::readable_report},
  {"download_txt",&Translations::download_txt},
  {"json_file",&Translations::json_file},
  {"structured_format",&Translations::structured_format},
  {"download_json",&Translations::download_json},
  {"csv_file",&Translations::csv_file},
  {"for_excel",&Translations::for_excel},
  {"download_csv",&Translations::download_csv},
  {"printable_version",&Translations::printable_version},
  {"pdf_format",&Translations::pdf_format},
  {"open",&Translations::open},
  {"export_report",&Translations::export_report},
  {"export_generated",&Translations::export_generated},
  {"export_after_boot",&Translations::export_after_boot},
  {"category",&Translations::category},
  {"parameter",&Translations::parameter},
  {"value",&Translations::value},
  {"model",&Translations::model},
  {"revision",&Translations::revision},
  {"frequency",&Translations::frequency},
  {"real",&Translations::real},
  {"board",&Translations::board},
  {"psram_mb",&Translations::psram_mb},
  {"detected",&Translations::detected},
  {"sram_kb",&Translations::sram_kb},
  {"connected",&Translations::connected},
  {"signal",&Translations::signal},
  {"list",&Translations::list},
  {"days",&Translations::days},
  {"hours",&Translations::hours},
  {"loading",&Translations::loading},
  {"error_title",&Translations::error_title},
  {"error_generic",&Translations::error_generic},
  {"update_indicator",&Translations::update_indicator},
  {"animations",&Translations::animations},
  {"custom_color",&Translations::custom_color},
  {"apply_color",&Translations::apply_color},
  {"turn_off_all",&Translations::turn_off_all},
  {"resolution",&Translations::resolution},
  {"info_label",&Translations::info_label},
  {"shapes_label",&Translations::shapes_label},
  {"grid_label",&Translations::grid_label},
  {"scroll_label",&Translations::scroll_label},
  {"display_text",&Translations::display_text},
  {"display_message",&Translations::display_message},
  {"stress_warning",&Translations::stress_warning},
  {"click_to_scan",&Translations::click_to_scan},
  {"language_changed",&Translations::language_changed},
  {"language_error",&Translations::language_error},
  {"displaying",&Translations::displaying},
  {"command_sent",&Translations::command_sent},
  {"communication_error",&Translations::communication_error},
  {"sensors_section",&Translations::sensors_section},
  {"pir_sensor",&Translations::pir_sensor},
  {"distance_sensor",&Translations::distance_sensor},
  {"dht_sensor",&Translations::dht_sensor},
  {"ldr_sensor",&Translations::ldr_sensor},
  {"trig_pin_label",&Translations::trig_pin_label},
  {"echo_pin_label",&Translations::echo_pin_label},
  {"sensor_type",&Translations::sensor_type},
  {"temperature_label",&Translations::temperature_label},
  {"humidity_label",&Translations::humidity_label},
  {"voltage_label",&Translations::voltage_label},
  {"motion_detected",&Translations::motion_detected},
  {"motion_none",&Translations::motion_none},
  {"distance_value",&Translations::distance_value},
  {"light_level",&Translations::light_level},
  {"sensor_config",&Translations::sensor_config},
  {"apply_changes",&Translations::apply_changes},
  {"run_sensor_test",&Translations::run_sensor_test},
  {"ok",&Translations::ok},
  {"fail",&Translations::fail},
  {"excellent",&Translations::excellent},
  {"very_good",&Translations::very_good},
  {"good",&Translations::good},
  {"warning",&Translations::warning},
  {"critical",&Translations::critical},
  {"weak",&Translations::weak},
  {"very_weak",&Translations::very_weak},
  {"none",&Translations::none},
  {"unknown",&Translations::unknown},
  {"testing",&Translations::testing},
  {"completed",&Translations::completed},
  {"scan",&Translations::scan},
  {"scanning",&Translations::scanning},
  {"cores",&Translations::cores},
  {"pins",&Translations::pins},
  {"devices",&Translations::devices},
  {"networks",&Translations::networks},
  {"tested",&Translations::tested},
  {"channels",&Translations::channels},
  {"poweron",&Translations::poweron},
  {"software_reset",&Translations::software_reset},
  {"deepsleep_exit",&Translations::deepsleep_exit},
  {"brownout",&Translations::brownout},
  {"other",&Translations::other},
};

inline String jsonEscape(const char* input) {
  if (!input) {
    return String("");
  }
  String out;
  while (*input) {
    char c = *input++;
    switch (c) {
      case '\\': out += "\\\\"; break;
      case '"': out += "\\\""; break;
      case '\n': out += "\\n"; break;
      case '\r': out += "\\r"; break;
      case '\t': out += "\\t"; break;
      default: out += c; break;
    }
  }
  return out;
}

inline String translationsToJson(const Translations& tr) {
  String json = "{";
  const size_t count = sizeof(TRANSLATION_FIELDS) / sizeof(TRANSLATION_FIELDS[0]);
  for (size_t i = 0; i < count; ++i) {
    if (i > 0) {
      json += ',';
    }
    json += '"';
    json += TRANSLATION_FIELDS[i].key;
    json += '"';
    json += ':';
    json += '"';
    json += jsonEscape(tr.*(TRANSLATION_FIELDS[i].member));
    json += '"';
  }
  json += '}';
  return json;
}

#endif
