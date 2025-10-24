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
  const char* supported_not_enabled;
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
  const char* enable_psram_hint;
  
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
  const char* full_test;
  const char* oled_screen;
  const char* i2c_pins;
  const char* i2c_address;
  const char* rotation;
  const char* no_device;
  const char* pin_config;
  const char* apply_redetect;
  const char* changes_pins;
  const char* custom_message;
  const char* show_message;
  const char* no_detected;
  const char* check_wiring;
  const char* oled_step_welcome;
  const char* oled_step_big_text;
  const char* oled_step_text_sizes;
  const char* oled_step_shapes;
  const char* oled_step_horizontal_lines;
  const char* oled_step_diagonals;
  const char* oled_step_moving_square;
  const char* oled_step_progress_bar;
  const char* oled_step_scroll_text;
  const char* oled_step_final_message;
  const char* oled_step_executed_prefix;
  const char* oled_step_unavailable;
  const char* oled_step_unknown;
  
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
  "Support détecté (désactivée dans l'IDE)",
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
  "Activez la PSRAM %TYPE% dans l'IDE Arduino (Outils → PSRAM).",
  
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
  "Test Complet",
  "Écran OLED 0.96\" I2C",
  "Pins I2C",
  "Adresse I2C",
  "Rotation",
  "aucun périphérique",
  "Configuration Pins I2C",
  "Appliquer et Re-détecter",
  "Modifie les pins I2C et relance la détection",
  "Message personnalisé",
  "Afficher Message",
  "Aucun écran OLED détecté. Vérifiez le câblage et les pins I2C ci-dessus.",
  "Vérifiez le câblage et les pins I2C",
  "Accueil",
  "Texte grand format",
  "Tailles de texte",
  "Formes géométriques",
  "Lignes horizontales",
  "Lignes diagonales",
  "Carré en mouvement",
  "Barre de progression",
  "Texte défilant",
  "Message final",
  "Étape exécutée :",
  "OLED non disponible",
  "Étape inconnue",
  
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
  "heures"
  
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
  "Support detected (disabled in IDE)",
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
  "Enable the %TYPE% PSRAM in Arduino IDE (Tools → PSRAM).",
  
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
  "Full Test",
  "OLED Screen 0.96\" I2C",
  "I2C Pins",
  "I2C Address",
  "Rotation",
  "no device",
  "I2C Pin Configuration",
  "Apply and Re-detect",
  "Changes I2C pins and restarts detection",
  "Custom message",
  "Display Message",
  "No OLED screen detected. Check wiring and I2C pins above.",
  "Check wiring and I2C pins",
  "Welcome",
  "Large text",
  "Text sizes",
  "Geometric shapes",
  "Horizontal lines",
  "Diagonal lines",
  "Moving square",
  "Progress bar",
  "Scrolling text",
  "Final message",
  "Step executed:",
  "OLED not available",
  "Unknown step",
  
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
  "hours"
    
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
const Translations& T() {
  return (currentLanguage == LANG_FR) ? LANG_FR_TEXTS : LANG_EN_TEXTS;
}

// Fonction pour changer la langue
void setLanguage(Language lang) {
  currentLanguage = lang;
}

#endif
