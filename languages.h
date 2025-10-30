#ifndef LANGUAGES_H
#define LANGUAGES_H

#include <Arduino.h>
#include <pgmspace.h>

// ========== SYSTÈME DE TRADUCTION ==========

enum Language {
  LANG_FR = 0,
  LANG_EN = 1
};

extern Language currentLanguage;

#define TRANSLATION_MAP(X) \
  X(title, "ESP32 Diagnostic Exhaustif", "ESP32 Complete Diagnostic") \
  X(version, "v", "v") \
  X(access, "Accès", "Access") \
  X(or_text, "ou", "or") \
  X(nav_overview, "Vue d'ensemble", "Overview") \
  X(nav_leds, "LEDs", "LEDs") \
  X(nav_screens, "Écrans", "Screens") \
  X(nav_tests, "Tests Avancés", "Advanced Tests") \
  X(nav_gpio, "GPIO", "GPIO") \
  X(nav_wireless, "Sans fil", "Wireless") \
  X(nav_benchmark, "Performance", "Performance") \
  X(nav_export, "Export", "Export") \
  X(nav_select_label, "Sélectionner une section", "Select a section") \
  X(chip_info, "Informations Chip Détaillées", "Detailed Chip Information") \
  X(full_model, "Modèle Complet", "Full Model") \
  X(cpu_cores, "Coeurs CPU", "CPU Cores") \
  X(mac_wifi, "Adresse MAC WiFi", "WiFi MAC Address") \
  X(last_reset, "Raison Dernier Reset", "Last Reset Reason") \
  X(chip_features, "Fonctionnalités Chip", "Chip Features") \
  X(sdk_version, "Version SDK", "SDK Version") \
  X(idf_version, "Version ESP-IDF", "ESP-IDF Version") \
  X(uptime, "Uptime", "Uptime") \
  X(cpu_temp, "Température CPU", "CPU Temperature") \
  X(memory_details, "Mémoire Détaillée", "Detailed Memory") \
  X(flash_memory, "Flash Memory", "Flash Memory") \
  X(real_size, "Taille Réelle (carte)", "Real Size (board)") \
  X(configured_ide, "Configurée (IDE)", "Configured (IDE)") \
  X(configuration, "Configuration", "Configuration") \
  X(correct, "Correcte", "Correct") \
  X(to_fix, "À corriger", "To fix") \
  X(flash_type, "Type Flash", "Flash Type") \
  X(flash_speed, "Vitesse Flash", "Flash Speed") \
  X(action_required, "Action Requise", "Action Required") \
  X(psram_external, "PSRAM (Mémoire Externe)", "PSRAM (External Memory)") \
  X(hardware_status, "Statut Hardware", "Hardware Status") \
  X(detected_active, "Détectée et active", "Detected and Active") \
  X(supported_not_enabled, "Support détecté (désactivée dans l'IDE)", "Support Detected (disabled in IDE)") \
  X(ide_config, "Configuration IDE", "IDE Configuration") \
  X(enabled, "Activée", "Enabled") \
  X(total_size, "Taille Totale", "Total Size") \
  X(free, "Libre", "Free") \
  X(used, "Utilisée", "Used") \
  X(largest_block, "Plus Gros Bloc", "Largest Block") \
  X(integrity_test, "Test Intégrité", "Integrity Test") \
  X(not_detected, "Non détectée", "Not Detected") \
  X(disabled, "Désactivée", "Disabled") \
  X(psram_usage, "Utilisation PSRAM", "PSRAM Usage") \
  X(enable_psram_hint, "Activez la PSRAM %TYPE% dans l'IDE Arduino (Outils → PSRAM).", "Enable the %TYPE% PSRAM in Arduino IDE (Tools → PSRAM).") \
  X(internal_sram, "SRAM Interne", "Internal SRAM") \
  X(min_free, "Min Libre (Record)", "Min Free (Record)") \
  X(max_alloc, "Max Allocation", "Max Allocation") \
  X(global_analysis, "Analyse Globale", "Global Analysis") \
  X(memory_fragmentation, "Fragmentation Mémoire", "Memory Fragmentation") \
  X(memory_status, "État Mémoire", "Memory Status") \
  X(total_ram, "RAM Totale (SRAM+PSRAM)", "Total RAM (SRAM+PSRAM)") \
  X(free_ram_total, "RAM Libre Totale", "Total Free RAM") \
  X(refresh_memory, "Actualiser Mémoire", "Refresh Memory") \
  X(wifi_connection, "WiFi", "WiFi") \
  X(connected_ssid, "SSID Connecté", "Connected SSID") \
  X(signal_power, "Puissance Signal (RSSI)", "Signal Power (RSSI)") \
  X(signal_quality, "Qualité Signal", "Signal Quality") \
  X(ip_address, "Adresse IP", "IP Address") \
  X(subnet_mask, "Masque Sous-réseau", "Subnet Mask") \
  X(gateway, "Passerelle", "Gateway") \
  X(dns, "DNS", "DNS") \
  X(wifi_channel, "Canal WiFi", "WiFi Channel") \
  X(bluetooth_section, "Bluetooth", "Bluetooth") \
  X(bluetooth_status, "Statut Bluetooth", "Bluetooth Status") \
  X(bluetooth_name, "Nom diffusé", "Advertised Name") \
  X(bluetooth_mac, "Adresse MAC Bluetooth", "Bluetooth MAC Address") \
  X(bluetooth_actions, "Actions Bluetooth", "Bluetooth Actions") \
  X(bluetooth_enable, "Activer le Bluetooth", "Enable Bluetooth") \
  X(bluetooth_disable, "Désactiver le Bluetooth", "Disable Bluetooth") \
  X(bluetooth_rename, "Mettre à jour le nom", "Update Name") \
  X(bluetooth_reset, "Réinitialiser le Bluetooth", "Reset Bluetooth") \
  X(bluetooth_placeholder, "Nom Bluetooth (1-29 caractères)", "Bluetooth Name (1-29 chars)") \
  X(bluetooth_not_supported, "Bluetooth non pris en charge", "Bluetooth Not Supported") \
  X(bluetooth_disabled, "Bluetooth désactivé", "Bluetooth Disabled") \
  X(bluetooth_enabled, "Bluetooth activé", "Bluetooth Enabled") \
  X(bluetooth_advertising, "Diffusion active", "Advertising Active") \
  X(bluetooth_not_advertising, "Diffusion arrêtée", "Advertising Stopped") \
  X(bluetooth_updated, "Paramètres Bluetooth mis à jour", "Bluetooth Settings Updated") \
  X(bluetooth_error, "Erreur Bluetooth", "Bluetooth Error") \
  X(bluetooth_reset_done, "Nom Bluetooth réinitialisé", "Bluetooth Name Reset") \
  X(bluetooth_support_label, "Support Bluetooth", "Bluetooth Support") \
  X(bluetooth_support_yes, "Pris en charge", "Supported") \
  X(bluetooth_support_no, "Non pris en charge", "Not Supported") \
  X(bluetooth_advertising_label, "Diffusion BLE", "BLE Advertising") \
  X(bluetooth_connection_label, "Connexion BLE", "BLE Connection") \
  X(bluetooth_client_connected, "Client connecté", "Client Connected") \
  X(bluetooth_client_disconnected, "Aucun client connecté", "No Client Connected") \
  X(bluetooth_notifications_label, "Notifications envoyées", "Notifications Sent") \
  X(gpio_interfaces, "GPIO et Interfaces", "GPIO and Interfaces") \
  X(total_gpio, "Total GPIO", "Total GPIO") \
  X(gpio_list, "Liste GPIO", "GPIO List") \
  X(i2c_peripherals, "Périphériques I2C", "I2C Peripherals") \
  X(device_count, "Nombre de Périphériques", "Number of Devices") \
  X(detected_addresses, "Adresses Détectées", "Detected Addresses") \
  X(rescan_i2c, "Re-scanner I2C", "Re-scan I2C") \
  X(builtin_led, "LED Intégrée", "Built-in LED") \
  X(gpio, "GPIO", "GPIO") \
  X(status, "Statut", "Status") \
  X(config, "Config", "Config") \
  X(test, "Test", "Test") \
  X(blink, "Blink", "Blink") \
  X(fade, "Fade", "Fade") \
  X(off, "Off", "Off") \
  X(neopixel, "NeoPixel", "NeoPixel") \
  X(rainbow, "Rainbow", "Rainbow") \
  X(color, "Couleur", "Color") \
  X(full_test, "Test Complet", "Full Test") \
  X(oled_screen, "Écran OLED 0.96\" I2C", "OLED Screen 0.96\" I2C") \
  X(i2c_pins, "Pins I2C", "I2C Pins") \
  X(i2c_address, "Adresse I2C", "I2C Address") \
  X(rotation, "Rotation", "Rotation") \
  X(no_device, "aucun périphérique", "no device") \
  X(pin_config, "Configuration Pins I2C", "I2C Pin Configuration") \
  X(apply_redetect, "Appliquer et Re-détecter", "Apply and Re-detect") \
  X(changes_pins, "Modifie les pins I2C et relance la détection", "Changes I2C pins and restarts detection") \
  X(custom_message, "Message personnalisé", "Custom message") \
  X(show_message, "Afficher Message", "Display Message") \
  X(no_detected, "Aucun écran OLED détecté. Vérifiez le câblage et les pins I2C ci-dessus.", "No OLED screen detected. Check wiring and I2C pins above.") \
  X(check_wiring, "Vérifiez le câblage et les pins I2C", "Check wiring and I2C pins") \
  X(oled_step_welcome, "Accueil", "Welcome") \
  X(oled_step_big_text, "Texte grand format", "Large text") \
  X(oled_step_text_sizes, "Tailles de texte", "Text sizes") \
  X(oled_step_shapes, "Formes géométriques", "Geometric shapes") \
  X(oled_step_horizontal_lines, "Lignes horizontales", "Horizontal lines") \
  X(oled_step_diagonals, "Lignes diagonales", "Diagonal lines") \
  X(oled_step_moving_square, "Carré en mouvement", "Moving square") \
  X(oled_step_progress_bar, "Barre de progression", "Progress bar") \
  X(oled_step_scroll_text, "Texte défilant", "Scrolling text") \
  X(oled_step_final_message, "Message final", "Final message") \
  X(oled_step_executed_prefix, "Étape exécutée :", "Step executed:") \
  X(oled_step_unavailable, "OLED non disponible", "OLED not available") \
  X(oled_step_unknown, "Étape inconnue", "Unknown step") \
  X(oled_step_running, "Étape en cours...", "Step in progress...") \
  X(oled_message_required, "Message requis", "Message required") \
  X(oled_displaying_message, "Affichage en cours...", "Displaying message...") \
  X(adc_test, "Test ADC", "ADC Test") \
  X(pwm_test, "Test PWM", "PWM Test") \
  X(spi_bus, "Bus SPI", "SPI Bus") \
  X(flash_partitions, "Partitions Flash", "Flash Partitions") \
  X(list_partitions, "Lister Partitions", "List Partitions") \
  X(click_button, "Cliquez sur le bouton", "Click the button") \
  X(memory_stress, "Stress Test Mémoire", "Memory Stress Test") \
  X(start_stress, "Lancer Stress Test", "Start Stress Test") \
  X(i2c_desc, "Détecte les périphériques reliés au bus I2C.", "Detects peripherals connected on the I2C bus.") \
  X(adc_desc, "Contrôle les entrées analogiques pour valider l'ADC.", "Checks analog inputs to validate the ADC.") \
  X(pwm_desc, "Vérifie la génération de PWM sur les sorties sélectionnées.", "Verifies PWM generation on the selected outputs.") \
  X(spi_desc, "Inspecte le bus SPI pour repérer les périphériques actifs.", "Inspects the SPI bus to spot active peripherals.") \
  X(partitions_desc, "Affiche la table actuelle des partitions flash.", "Displays the current flash partition table.") \
  X(stress_desc, "Pousse la mémoire pour détecter les instabilités.", "Pushes memory usage to uncover instability.") \
  X(gpio_test, "Test GPIO", "GPIO Test") \
  X(test_all_gpio, "Tester Tous les GPIO", "Test All GPIO") \
  X(click_to_test, "Cliquez pour tester", "Click to test") \
  X(gpio_warning, "Un GPIO en échec peut provenir de la configuration ou d'un périphérique externe : vérifiez câblage et alimentation avant de conclure à un défaut.", "A failed GPIO result may stem from configuration or external hardware—double-check wiring and power before assuming the pin is faulty.") \
  X(gpio_desc, "Teste automatiquement l'ensemble des broches GPIO.", "Automatically exercises every GPIO pin.") \
  X(wifi_scanner, "Scanner WiFi", "WiFi Scanner") \
  X(scan_networks, "Scanner Réseaux WiFi", "Scan WiFi Networks") \
  X(wifi_desc, "Analyse les réseaux WiFi et leurs paramètres radio.", "Analyzes nearby WiFi networks and radio parameters.") \
  X(performance_bench, "Benchmarks de Performance", "Performance Benchmarks") \
  X(run_benchmarks, "Lancer Benchmarks", "Run Benchmarks") \
  X(cpu_benchmark, "CPU Benchmark", "CPU Benchmark") \
  X(memory_benchmark, "Mémoire Benchmark", "Memory Benchmark") \
  X(cpu_performance, "Performance CPU", "CPU Performance") \
  X(memory_speed, "Vitesse Mémoire", "Memory Speed") \
  X(not_tested, "Non testé", "Not tested") \
  X(benchmark_desc, "Mesure les performances CPU et mémoire du module.", "Measures CPU and memory performance of the module.") \
  X(data_export, "Export des Données", "Data Export") \
  X(txt_file, "Fichier TXT", "TXT File") \
  X(readable_report, "Rapport texte lisible", "Readable text report") \
  X(download_txt, "Télécharger TXT", "Download TXT") \
  X(json_file, "Fichier JSON", "JSON File") \
  X(structured_format, "Format structuré", "Structured format") \
  X(download_json, "Télécharger JSON", "Download JSON") \
  X(csv_file, "Fichier CSV", "CSV File") \
  X(for_excel, "Pour Excel", "For Excel") \
  X(download_csv, "Télécharger CSV", "Download CSV") \
  X(printable_version, "Version Imprimable", "Printable Version") \
  X(pdf_format, "Format PDF", "PDF Format") \
  X(open, "Ouvrir", "Open") \
  X(export_report, "Rapport de diagnostic", "Diagnostic Report") \
  X(export_generated, "Rapport généré le", "Report generated at") \
  X(export_after_boot, "secondes après démarrage", "seconds after boot") \
  X(category, "Catégorie", "Category") \
  X(parameter, "Paramètre", "Parameter") \
  X(value, "Valeur", "Value") \
  X(model, "Modèle", "Model") \
  X(revision, "Révision", "Revision") \
  X(frequency, "Fréquence", "Frequency") \
  X(real, "Réel", "Real") \
  X(board, "Carte", "Board") \
  X(psram_mb, "PSRAM MB", "PSRAM MB") \
  X(detected, "Détecté", "Detected") \
  X(sram_kb, "SRAM KB", "SRAM KB") \
  X(connected, "Connecté", "Connected") \
  X(signal, "Signal", "Signal") \
  X(list, "Liste", "List") \
  X(days, "jours", "days") \
  X(hours, "heures", "hours") \
  X(minutes, "minutes", "minutes") \
  X(ok, "OK", "OK") \
  X(fail, "FAIL", "FAIL") \
  X(excellent, "Excellent", "Excellent") \
  X(very_good, "Très bon", "Very good") \
  X(good, "Bon", "Good") \
  X(warning, "Attention", "Warning") \
  X(critical, "Critique", "Critical") \
  X(weak, "Faible", "Weak") \
  X(very_weak, "Très faible", "Very weak") \
  X(none, "Aucune", "None") \
  X(unknown, "Inconnu", "Unknown") \
  X(testing, "Test...", "Testing...") \
  X(completed, "Terminé", "Completed") \
  X(scan, "Scan...", "Scanning...") \
  X(scanning, "Scan...", "Scanning...") \
  X(cores, "coeurs", "cores") \
  X(pins, "broches", "pins") \
  X(devices, "périphérique(s)", "device(s)") \
  X(networks, "réseaux", "networks") \
  X(tested, "testé(s)", "tested") \
  X(channels, "canaux", "channels") \
  X(poweron, "Mise sous tension", "Power on") \
  X(software_reset, "Reset logiciel", "Software reset") \
  X(deepsleep_exit, "Sortie de deep sleep", "Deep sleep exit") \
  X(brownout, "Brownout", "Brownout") \
  X(other, "Autre", "Other") \
  X(updating, "Mise à jour...", "Updating...") \
  X(online, "En ligne", "Online") \
  X(offline, "Hors ligne", "Offline") \
  X(check_network, "Vérifiez le réseau.", "Check the network.") \
  X(language_updated, "Langue mise à jour", "Language updated") \
  X(language_switch_error, "Erreur changement langue", "Language switch error") \
  X(translation_error, "Erreur traduction", "Translation error") \
  X(bluetooth_invalid_name, "Nom Bluetooth invalide", "Invalid Bluetooth name") \
  X(bluetooth_enabling, "Activation...", "Enabling...") \
  X(bluetooth_disabling, "Désactivation...", "Disabling...") \
  X(bluetooth_updating, "Mise à jour...", "Updating...") \
  X(bluetooth_resetting, "Réinitialisation...", "Resetting...") \
  X(loading, "Chargement...", "Loading...") \
  X(configuring, "Configuration...", "Configuring...") \
  X(reconfiguring, "Reconfiguration...", "Reconfiguring...") \
  X(transmission, "Transmission...", "Transmitting...") \
  X(error_label, "Erreur", "Error") \
  X(test_failed, "Test en échec", "Test failed") \
  X(gpio_summary_template, "Terminé - %COUNT% GPIO testés", "Done - %COUNT% GPIO tested") \
  X(wifi_networks_found, "%COUNT% réseaux détectés", "%COUNT% networks found") \
  X(i2c_scan_result, "I2C : %COUNT% périphérique(s)", "I2C: %COUNT% device(s)") \
  X(gpio_invalid, "GPIO invalide", "Invalid GPIO") \
  X(configuration_invalid, "Configuration invalide", "Invalid configuration") \
  X(oled_test_running, "Test en cours (25s)...", "Testing (25s)...")

enum TranslationKey {
#define DECL_KEY(identifier, fr, en) TR_##identifier,
  TRANSLATION_MAP(DECL_KEY)
#undef DECL_KEY
  TR_COUNT
};

static const char* const TRANSLATIONS_FR[] PROGMEM = {
#define DECL_FR(identifier, fr, en) fr,
  TRANSLATION_MAP(DECL_FR)
#undef DECL_FR
};

static const char* const TRANSLATIONS_EN[] PROGMEM = {
#define DECL_EN(identifier, fr, en) en,
  TRANSLATION_MAP(DECL_EN)
#undef DECL_EN
};

inline const char* lookupTranslation(Language lang, TranslationKey key) {
  return (lang == LANG_FR) ? TRANSLATIONS_FR[key] : TRANSLATIONS_EN[key];
}

class TranslationField {
 public:
  constexpr TranslationField() : langPtr(nullptr), key(static_cast<TranslationKey>(0)) {}
  constexpr TranslationField(const Language* lang, TranslationKey idx) : langPtr(lang), key(idx) {}

  operator const char*() const { return c_str(); }

  const char* c_str() const {
    return (langPtr != nullptr) ? lookupTranslation(*langPtr, key) : "";
  }

  String str() const { return String(c_str()); }

 private:
  const Language* langPtr;
  TranslationKey key;
};

class TranslationAccessor {
 public:
  explicit TranslationAccessor(const Language* lang) {
#define INIT_FIELD(identifier, fr, en) identifier = TranslationField(lang, TR_##identifier);
    TRANSLATION_MAP(INIT_FIELD)
#undef INIT_FIELD
  }

#define DECL_FIELD(identifier, fr, en) TranslationField identifier;
  TRANSLATION_MAP(DECL_FIELD)
#undef DECL_FIELD
};

inline const TranslationAccessor& T() {
  static TranslationAccessor accessor(&currentLanguage);
  return accessor;
}

inline void setLanguage(Language lang) {
  currentLanguage = lang;
}

#undef TRANSLATION_MAP

#endif
