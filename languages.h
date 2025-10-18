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
  const char* psram_mode_label;
  const char* psram_status_active;
  const char* psram_status_disabled;
  const char* psram_status_not_supported;
  const char* psram_enable_hint;
  const char* psram_mode_opi;
  const char* psram_mode_qspi;
  const char* psram_mode_unknown;

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
  const char* wifi_click_to_scan;
  const char* wifi_no_networks;
  const char* wifi_not_connected;

  // Bluetooth
  const char* ble_scanner;
  const char* scan_ble_devices;
  const char* ble_click_to_scan;
  const char* ble_no_devices;
  const char* ble_not_supported;
  const char* wireless_status;
  const char* wifi_label;
  const char* ble_label;
  const char* recommendation;
  const char* ble_status_active;
  const char* ble_status_stack_missing;
  const char* ble_status_not_supported;
  const char* ble_enable_hint;

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
  const char* gpio_fail_hint;
  
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
  const char* feature_disabled;
  const char* not_supported;
  const char* max_allocated;

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

  // Messages complémentaires LED/OLED
  const char* led_conflict;
  const char* pwm_disabled;
  const char* simple_mode;
  const char* led_ready;
  const char* led_test_ok;
  const char* led_configured;
  const char* led_not_configured;
  const char* led_on_state;
  const char* led_off_state;
  const char* led_test_skipped;
  const char* neopixel_not_ready;
  const char* pattern_missing;
  const char* pattern_unknown;
  const char* message_displayed;
  const char* oled_reconfigured;
  const char* invalid_pins;
  const char* oled_init_failed;
  const char* oled_intro;
  const char* oled_large_text;
  const char* oled_fonts;
  const char* oled_shapes;
  const char* oled_lines;
  const char* oled_diagonals;
  const char* oled_animation;
  const char* oled_progress;
  const char* oled_scroll;
  const char* oled_final;
  const char* message_required;
  const char* language_changed;
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
  "Tests Avancés",
  "GPIO",
  "Sans fil",
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
  "Mode supporté",
  "PSRAM active (%MODE%)",
  "Carte compatible PSRAM %MODE% mais désactivée dans l'IDE",
  "PSRAM non supportée par la carte",
  "Activez-la via Outils → PSRAM → OPI/QSPI",
  "Mode OPI",
  "Mode QSPI",
  "Mode PSRAM inconnu",

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
  "Cliquez pour scanner",
  "Aucun réseau détecté",
  "Non connecté",

  // Bluetooth
  "Scanner Bluetooth Low Energy",
  "Scanner les appareils BLE",
  "Cliquez pour scanner",
  "Aucun appareil détecté",
  "Bluetooth LE non disponible sur ce modèle",
  "Statut sans fil",
  "Wi-Fi",
  "Bluetooth Low Energy",
  "Recommandation",
  "BLE opérationnel",
  "BLE supporté par la puce mais absent dans ce firmware",
  "BLE non supporté par cette puce",
  "Activez ou recompilez le support BLE dans l'IDE Arduino",

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
  "Un GPIO \"FAIL\" peut être réservé (UART, Flash, capteur...). Vérifiez son usage avant de conclure qu'il est défectueux.",
  
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
  "Fonction désactivée dans ce firmware",
  "Non supporté sur ce modèle",
  "Max",
  
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
  "Autre",

  // Messages complémentaires LED/OLED
  "Conflit NeoPixel (GPIO partagé)",
  "PWM désactivée",
  "Mode simple",
  "Prêt",
  "Test OK",
  "LED configurée sur GPIO",
  "LED non configurée",
  "LED allumée",
  "LED éteinte",
  "Test ignoré (conflit NeoPixel)",
  "NeoPixel non initialisé",
  "Motif manquant",
  "Motif inconnu",
  "Message affiché",
  "I2C reconfiguré",
  "Pins invalides",
  "Périphérique OLED détecté mais initialisation échouée",
  "Écran d'accueil",
  "Texte géant",
  "Tailles de police",
  "Formes géométriques",
  "Rayures horizontales",
  "Diagonales",
  "Carré animé",
  "Barre de progression",
  "Texte défilant",
  "Écran final",
  "Veuillez saisir un message",
  "Langue changée"
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
  "Advanced Tests",
  "GPIO",
  "Wireless",
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
  "Supported mode",
  "PSRAM active (%MODE%)",
  "Board supports PSRAM %MODE% but it is disabled in the IDE",
  "Board does not support PSRAM",
  "Enable it via Tools → PSRAM → OPI/QSPI",
  "OPI mode",
  "QSPI mode",
  "Unknown PSRAM mode",
  
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
  "Click to scan",
  "No networks found",
  "Not connected",

  // Bluetooth
  "Bluetooth Low Energy Scanner",
  "Scan BLE devices",
  "Click to scan",
  "No devices found",
  "Bluetooth LE not available on this board",
  "Wireless status",
  "Wi-Fi",
  "Bluetooth Low Energy",
  "Recommendation",
  "BLE ready",
  "BLE supported by the chip but missing from this firmware",
  "BLE not supported by this chip",
  "Enable or rebuild the firmware with BLE support in the Arduino IDE",

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
  "A GPIO marked \"FAIL\" may simply be reserved (UART, flash, sensors...). Double-check its role before assuming it's faulty.",
  
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
  "Feature disabled in this firmware",
  "Not supported on this board",
  "Max",
  
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
  "Other",

  // Additional LED/OLED messages
  "NeoPixel conflict (shared GPIO)",
  "PWM disabled",
  "Basic mode",
  "Ready",
  "Test OK",
  "LED configured on GPIO",
  "LED not configured",
  "LED on",
  "LED off",
  "Test skipped (NeoPixel conflict)",
  "NeoPixel not initialised",
  "Missing pattern",
  "Unknown pattern",
  "Message displayed",
  "I2C reconfigured",
  "Invalid pins",
  "OLED device detected but initialisation failed",
  "Welcome screen",
  "Large text",
  "Font sizes",
  "Geometric shapes",
  "Horizontal stripes",
  "Diagonals",
  "Moving square",
  "Progress bar",
  "Scrolling text",
  "Final screen",
  "Please enter a message",
  "Language changed"
};

// Fonction pour obtenir le texte traduit
const Translations& T() {
  return (currentLanguage == LANG_FR) ? LANG_FR_TEXTS : LANG_EN_TEXTS;
}

// Fonction pour changer la langue
void setLanguage(Language lang) {
  currentLanguage = lang;
}

#endif
