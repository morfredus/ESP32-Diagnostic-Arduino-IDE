#ifndef LANGUAGES_H
#define LANGUAGES_H

#include <Arduino.h>
#include <pgmspace.h>

// ========== SYSTÈME DE TRADUCTION OPTIMISÉ v3.6.19 ==========
// Optimisations appliquées :
// - Élimination des doublons (scan/scanning)
// - Harmonisation des clés similaires
// - Conservation de toutes les fonctionnalités

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
  X(nav_display_signal, "Affichage & Signal", "Display & Signal") \
  X(nav_sensors, "Capteurs", "Sensors") \
  X(nav_hardware_tests, "Tests Matériel", "Hardware Tests") \
  X(nav_leds, "LEDs", "LEDs") \
  X(nav_screens, "Écrans", "Screens") \
  X(nav_tests, "Tests Avancés", "Advanced Tests") \
  X(nav_gpio, "GPIO", "GPIO") \
  X(nav_wireless, "Sans fil", "Wireless") \
  X(nav_benchmark, "Performance", "Performance") \
  X(nav_export, "Export", "Export") \
  X(nav_select_label, "Sélectionner une section", "Select a section") \
  X(chip_info, "Informations détaillées sur la puce", "Detailed Chip Information") \
  X(full_model, "Modèle complet", "Full Model") \
  X(cpu_cores, "Cœurs CPU", "CPU Cores") \
  X(mac_wifi, "Adresse MAC WiFi", "WiFi MAC Address") \
  X(last_reset, "Raison du dernier reset", "Last Reset Reason") \
  X(chip_features, "Fonctionnalités de la puce", "Chip Features") \
  X(sdk_version, "Version du SDK", "SDK Version") \
  X(idf_version, "Version d'ESP-IDF", "ESP-IDF Version") \
  X(uptime, "Uptime", "Uptime") \
  X(cpu_temp, "Température du CPU", "CPU Temperature") \
  X(memory_details, "Mémoire détaillée", "Detailed Memory") \
  X(flash_memory, "Mémoire flash", "Flash Memory") \
  X(real_size, "Taille réelle (carte)", "Actual Size (board)") \
  X(configured_ide, "Configurée (IDE)", "Configured (IDE)") \
  X(configuration, "Configuration", "Configuration") \
  X(correct, "Correcte", "Correct") \
  X(to_fix, "À corriger", "Needs fix") \
  X(flash_type, "Type de flash", "Flash Type") \
  X(flash_speed, "Vitesse de flash", "Flash Speed") \
  X(action_required, "Action Requise", "Action Required") \
  X(psram_external, "PSRAM (mémoire externe)", "PSRAM (External Memory)") \
  X(hardware_status, "Statut matériel", "Hardware Status") \
  X(detected_active, "Détectée et active", "Detected and Active") \
  X(supported_not_enabled, "Support détecté (désactivée dans l'IDE)", "Support Detected (disabled in IDE)") \
  X(ide_config, "Configuration IDE", "IDE Configuration") \
  X(enabled, "Activée", "Enabled") \
  X(total_size, "Taille Totale", "Total Size") \
  X(free, "Libre", "Free") \
  X(used, "Utilisée", "Used") \
  X(largest_block, "Plus grand bloc", "Largest Block") \
  X(integrity_test, "Test d'intégrité", "Integrity Test") \
  X(not_detected, "Non détectée", "Not Detected") \
  X(disabled, "Désactivée", "Disabled") \
  X(psram_usage, "Utilisation de la PSRAM", "PSRAM Usage") \
  X(enable_psram_hint, "Activez la PSRAM %TYPE% dans l'IDE Arduino (Outils → PSRAM).", "Enable the %TYPE% PSRAM in Arduino IDE (Tools → PSRAM).") \
  X(internal_sram, "SRAM interne", "Internal SRAM") \
  X(min_free, "Min libre (record)", "Min Free (Record)") \
  X(max_alloc, "Allocation max", "Max Allocation") \
  X(global_analysis, "Analyse globale", "Global Analysis") \
  X(memory_fragmentation, "Fragmentation mémoire", "Memory Fragmentation") \
  X(memory_status, "État de la mémoire", "Memory Status") \
  X(total_ram, "RAM totale (SRAM+PSRAM)", "Total RAM (SRAM+PSRAM)") \
  X(free_ram_total, "RAM libre totale", "Total Free RAM") \
  X(refresh_memory, "Actualiser la mémoire", "Refresh Memory") \
  X(wifi_connection, "Connexion WiFi", "WiFi Connection") \
  X(connected_ssid, "SSID connecté", "Connected SSID") \
  X(signal_power, "Puissance du signal (RSSI)", "Signal Power (RSSI)") \
  X(signal_quality, "Qualité du signal", "Signal Quality") \
  X(ip_address, "Adresse IP", "IP Address") \
  X(subnet_mask, "Masque de sous-réseau", "Subnet Mask") \
  X(gateway, "Passerelle", "Gateway") \
  X(dns, "DNS", "DNS") \
  X(ip_unavailable, "IP indisponible", "IP unavailable") \
  X(wifi_channel, "Canal WiFi", "WiFi Channel") \
  X(wifi_open_auth, "Ouvert", "Open") \
  X(bluetooth_section, "Bluetooth", "Bluetooth") \
  X(bluetooth_desc, "Gérez l'état, le nom et la diffusion Bluetooth.", "Manage Bluetooth state, name, and advertising.") \
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
  X(bluetooth_scan, "Scanner les périphériques Bluetooth", "Scan Bluetooth Devices") \
  X(bluetooth_scan_hint, "Activez le BLE sur vos appareils proches avant de scanner.", "Enable BLE on nearby devices before scanning.") \
  X(bluetooth_scan_in_progress, "Scan Bluetooth en cours...", "Scanning Bluetooth...") \
  X(bluetooth_devices_found, "%COUNT% périphérique(s) détecté(s)", "%COUNT% device(s) detected") \
  X(bluetooth_updated, "Paramètres Bluetooth mis à jour", "Bluetooth Settings Updated") \
  X(bluetooth_error, "Erreur Bluetooth", "Bluetooth Error") \
  X(bluetooth_reset_done, "Nom Bluetooth réinitialisé", "Bluetooth Name Reset") \
  X(bluetooth_support_label, "Compatibilité Bluetooth", "Bluetooth Support") \
  X(bluetooth_support_yes, "Pris en charge", "Supported") \
  X(bluetooth_support_no, "Non pris en charge", "Not Supported") \
  X(bluetooth_advertising_label, "Diffusion BLE", "BLE Advertising") \
  X(bluetooth_connection_label, "Connexion BLE", "BLE Connection") \
  X(bluetooth_client_connected, "Client connecté", "Client Connected") \
  X(bluetooth_client_disconnected, "Aucun client connecté", "No Client Connected") \
  X(bluetooth_notifications_label, "Notifications envoyées", "Notifications Sent") \
  X(gpio_interfaces, "GPIO et interfaces", "GPIO and Interfaces") \
  X(total_gpio, "Nombre de GPIO", "Total GPIO") \
  X(gpio_list, "Liste des GPIO", "GPIO List") \
  X(i2c_peripherals, "Périphériques I2C", "I2C Peripherals") \
  X(device_count, "Nombre de périphériques", "Number of Devices") \
  X(detected_addresses, "Adresses détectées", "Detected Addresses") \
  X(rescan_i2c, "Relancer le scan I2C", "Re-scan I2C") \
  X(builtin_led, "LED intégrée", "Built-in LED") \
  X(builtin_led_desc, "Contrôlez la LED embarquée et vérifiez sa réponse.", "Control the onboard LED and confirm it responds.") \
  X(gpio, "GPIO", "GPIO") \
  X(status, "Statut", "Status") \
  X(config, "Configuration", "Configuration") \
  X(test, "Test", "Test") \
  X(turn_on, "Allumer", "Turn On") \
  X(turn_off, "Éteindre", "Turn Off") \
  X(turn_off_all, "Tout éteindre", "Turn Everything Off") \
  X(blink, "Clignotement", "Blink") \
  X(fade, "Fondu", "Fade") \
  X(off, "Arrêt", "Off") \
  X(neopixel, "NeoPixel", "NeoPixel") \
  X(neopixel_desc, "Pilotez vos bandes NeoPixel et testez les animations.", "Drive your NeoPixel strips and test animations.") \
  X(animations, "Animations", "Animations") \
  X(led_count, "Nombre de LEDs", "LED Count") \
  X(configure_gpio, "Configurer GPIO", "Configure GPIO") \
  X(configure_led_pin, "Configurer le pin de la LED", "Configure LED pin") \
  X(configure_neopixel, "Configurer NeoPixel", "Configure NeoPixel") \
  X(apply_configuration, "Appliquer la configuration", "Apply Configuration") \
  X(rainbow, "Arc-en-ciel", "Rainbow") \
  X(chase, "Chenillard", "Chase") \
  X(color, "Couleur", "Color") \
  X(custom_color, "Couleur personnalisée", "Custom Color") \
  X(full_test, "Test complet", "Full Test") \
  X(oled_screen, "Écran OLED 0.96\" I2C", "OLED Screen 0.96\" I2C") \
  X(i2c_pins, "Pins I2C", "I2C Pins") \
  X(label_sda, "SDA", "SDA") \
  X(label_scl, "SCL", "SCL") \
  X(i2c_address, "Adresse I2C", "I2C Address") \
  X(rotation, "Rotation", "Rotation") \
  X(no_device, "Aucun périphérique", "No device") \
  X(pin_config, "Configuration des pins I2C", "I2C Pin Configuration") \
  X(apply_redetect, "Appliquer et relancer la détection", "Apply and Re-detect") \
  X(changes_pins, "Modifie les pins I2C et relance la détection", "Changes I2C pins and restarts detection") \
  X(custom_message, "Message personnalisé", "Custom Message") \
  X(show_message, "Afficher le message", "Display Message") \
  X(apply_color, "Appliquer la couleur", "Apply Color") \
  X(no_detected, "Aucun écran OLED détecté. Vérifiez le câblage et les pins I2C ci-dessus.", "No OLED screen detected. Check wiring and I2C pins above.") \
  X(check_wiring, "Vérifiez le câblage et les pins I2C", "Check wiring and I2C pins") \
  X(oled_step_welcome, "Accueil", "Welcome") \
  X(oled_step_big_text, "Texte grand format", "Large Text") \
  X(oled_step_text_sizes, "Tailles de texte", "Text Sizes") \
  X(oled_step_shapes, "Formes géométriques", "Geometric Shapes") \
  X(oled_step_horizontal_lines, "Lignes horizontales", "Horizontal Lines") \
  X(oled_step_diagonals, "Lignes diagonales", "Diagonal Lines") \
  X(oled_step_moving_square, "Carré en mouvement", "Moving Square") \
  X(oled_step_progress_bar, "Barre de progression", "Progress Bar") \
  X(oled_step_scroll_text, "Texte défilant", "Scrolling Text") \
  X(oled_step_final_message, "Message final", "Final Message") \
  X(oled_step_executed_prefix, "Étape exécutée :", "Step executed:") \
  X(oled_step_unavailable, "OLED non disponible", "OLED unavailable") \
  X(oled_step_unknown, "Étape inconnue", "Unknown step") \
  X(oled_step_running, "Étape en cours...", "Step in progress...") \
  X(oled_message_required, "Message requis", "Message required") \
  X(oled_displaying_message, "Affichage en cours...", "Displaying message...") \
  X(adc_test, "Test ADC", "ADC Test") \
  X(start_adc_test, "Lancer le test ADC", "Start ADC Test") \
  X(pwm_test, "Test PWM", "PWM Test") \
  X(spi_bus, "Bus SPI", "SPI Bus") \
  X(flash_partitions, "Partitions flash", "Flash Partitions") \
  X(list_partitions, "Lister les partitions", "List Partitions") \
  X(click_button, "Cliquez sur le bouton", "Click the button") \
  X(memory_stress, "Stress test mémoire", "Memory Stress Test") \
  X(stress_warning, "Peut ralentir l'ESP32 temporairement", "May temporarily slow the ESP32") \
  X(start_stress, "Lancer le stress test", "Start Stress Test") \
  X(test_in_progress, "Test en cours...", "Test in progress...") \
  X(gpio_test_complete, "Terminé - {count} GPIO testés", "Completed - {count} GPIO tested") \
  X(wifi_scan_in_progress, "Scan WiFi en cours...", "Scanning...") \
  X(wifi_networks_found, "{count} réseaux détectés", "{count} networks found") \
  X(stress_running, "⚠️ Test en cours... Patientez", "⚠️ Test running... Please wait") \
  X(cpu_time, "Temps CPU", "CPU Time") \
  X(memory_time, "Temps mémoire", "Memory Time") \
  X(allocations_label, "Allocations", "Allocations") \
  X(i2c_desc, "Détecte les périphériques reliés au bus I2C.", "Detects peripherals connected on the I2C bus.") \
  X(adc_desc, "Contrôle les entrées analogiques pour valider l'ADC.", "Checks analog inputs to validate the ADC.") \
  X(pwm_desc, "Vérifie la génération de PWM sur les sorties sélectionnées.", "Verifies PWM generation on the selected outputs.") \
  X(spi_desc, "Inspecte le bus SPI pour repérer les périphériques actifs.", "Inspects the SPI bus to spot active peripherals.") \
  X(partitions_desc, "Affiche la table actuelle des partitions flash.", "Displays the current flash partition table.") \
  X(stress_desc, "Pousse la mémoire pour détecter les instabilités.", "Pushes memory usage to uncover instability.") \
  X(gpio_test, "Test GPIO", "GPIO Test") \
  X(test_all_gpio, "Tester tous les GPIO", "Test All GPIO Pins") \
  X(click_to_test, "Cliquez pour tester", "Click to test") \
  X(gpio_warning, "Un GPIO en échec peut provenir de la configuration ou d'un périphérique externe : vérifiez câblage et alimentation avant de conclure à un défaut.", "A failed GPIO result may stem from configuration or external hardware—double-check wiring and power before assuming the pin is faulty.") \
  X(gpio_desc, "Teste automatiquement l'ensemble des broches GPIO.", "Automatically exercises every GPIO pin.") \
  X(wifi_scanner, "Scanner WiFi", "WiFi Scanner") \
  X(wireless_intro, "Analysez et pilotez les interfaces WiFi et Bluetooth.", "Analyze and control the WiFi and Bluetooth interfaces.") \
  X(scan_networks, "Scanner les réseaux WiFi", "Scan WiFi Networks") \
  X(click_to_scan, "Cliquez pour scanner", "Click to scan") \
  X(wifi_desc, "Analyse les réseaux WiFi et leurs paramètres radio.", "Analyzes nearby WiFi networks and radio parameters.") \
  X(performance_bench, "Benchmarks de performance", "Performance Benchmarks") \
  X(run_benchmarks, "Lancer les benchmarks", "Run Benchmarks") \
  X(cpu_benchmark, "Benchmark CPU", "CPU Benchmark") \
  X(memory_benchmark, "Benchmark mémoire", "Memory Benchmark") \
  X(cpu_performance, "Performances CPU", "CPU Performance") \
  X(memory_speed, "Vitesse de la mémoire", "Memory Speed") \
  X(not_tested, "Non testé", "Not tested") \
  X(benchmark_desc, "Mesure les performances CPU et mémoire du module.", "Measures CPU and memory performance of the module.") \
  X(data_export, "Export des données", "Data Export") \
  X(export_intro, "Téléchargez les rapports de diagnostic dans le format souhaité.", "Download diagnostic reports in the format you need.") \
  X(txt_file, "Fichier TXT", "TXT File") \
  X(readable_report, "Rapport texte lisible", "Readable text report") \
  X(download_txt, "Télécharger TXT", "Download TXT") \
  X(json_file, "Fichier JSON", "JSON File") \
  X(structured_format, "Format structuré", "Structured Format") \
  X(download_json, "Télécharger JSON", "Download JSON") \
  X(csv_file, "Fichier CSV", "CSV File") \
  X(for_excel, "Pour Excel", "For Excel") \
  X(download_csv, "Télécharger CSV", "Download CSV") \
  X(printable_version, "Version imprimable", "Printable Version") \
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
  X(psram_mb, "PSRAM (Mo)", "PSRAM MB") \
  X(detected, "Détecté", "Detected") \
  X(sram_kb, "SRAM (Ko)", "SRAM KB") \
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
  X(configuration_invalid, "Configuration invalide", "Invalid configuration") \
  X(unknown, "Inconnu", "Unknown") \
  X(testing, "Test...", "Testing...") \
  X(completed, "Terminé", "Completed") \
  X(scanning, "Scan en cours...", "Scanning...") \
  X(scan, "Scanner", "Scan") \
  X(cores, "cœurs", "cores") \
  X(pins, "broches", "pins") \
  X(devices, "périphériques", "devices") \
  X(networks, "réseaux", "networks") \
  X(tested, "testés", "tested") \
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
  X(language_label, "Langue", "Language") \
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
  X(transmission, "Transmission en cours...", "Transmitting...") \
  X(error_label, "Erreur", "Error") \
  X(test_failed, "Test en échec", "Test failed") \
  X(gpio_summary_template, "Terminé - %COUNT% GPIO testés", "Done - %COUNT% GPIO tested") \
  X(i2c_scan_result, "I2C : %COUNT% périphérique(s)", "I2C: %COUNT% device(s)") \
  X(gpio_invalid, "GPIO invalide", "Invalid GPIO") \
  X(oled_test_running, "Test en cours (25s)...", "Testing (25s)...") \
  X(message_displayed, "Message affiché", "Message displayed") \
  X(seconds, "secondes", "seconds") \
  X(interface_loaded, "Interface chargée", "Interface loaded") \
  X(rgb_led, "LED RGB", "RGB LED") \
  X(rgb_led_desc, "Test de la LED RGB (3 canaux R, G, B)", "Test RGB LED (3 channels R, G, B)") \
  X(rgb_led_pins, "Pins RGB (R, G, B)", "RGB Pins (R, G, B)") \
  X(test_rgb_led, "Tester la LED RGB", "Test RGB LED") \
  X(set_color, "Définir la couleur", "Set Color") \
  X(red, "Rouge", "Red") \
  X(green, "Vert", "Green") \
  X(blue, "Bleu", "Blue") \
  X(white, "Blanc", "White") \
  X(buzzer, "Buzzer", "Buzzer") \
  X(buzzer_desc, "Test du buzzer avec différentes fréquences", "Test the buzzer with different frequencies") \
  X(buzzer_pin, "Pin du buzzer", "Buzzer Pin") \
  X(test_buzzer, "Tester le buzzer", "Test Buzzer") \
  X(play_tone, "Jouer une tonalité", "Play Tone") \
  X(beep, "Bip", "Beep") \
  X(melody, "Mélodie", "Melody") \
  X(dht11_sensor, "Capteur DHT11", "DHT11 Sensor") \
  X(dht11_desc, "Capteur de température et d'humidité", "Temperature and Humidity Sensor") \
  X(dht11_pin, "Pin DHT11", "DHT11 Pin") \
  X(test_dht11, "Lire DHT11", "Read DHT11") \
  X(temperature, "Température", "Temperature") \
  X(humidity, "Humidité", "Humidity") \
  X(light_sensor, "Capteur de luminosité", "Light Sensor") \
  X(light_sensor_desc, "Capteur de luminosité analogique", "Analog Light Sensor") \
  X(light_sensor_pin, "Pin du capteur de luminosité", "Light Sensor Pin") \
  X(test_light_sensor, "Mesurer la luminosité", "Read Light Sensor") \
  X(light_level, "Niveau de lumière", "Light Level") \
  X(distance_sensor, "Capteur de distance", "Distance Sensor") \
  X(distance_sensor_desc, "Capteur ultrason HC-SR04", "HC-SR04 Ultrasonic Sensor") \
  X(distance_pins, "Pins (Trig, Echo)", "Pins (Trig, Echo)") \
  X(label_trig, "Trig", "Trig") \
  X(label_echo, "Echo", "Echo") \
  X(test_distance_sensor, "Mesurer la distance", "Measure Distance") \
  X(distance, "Distance", "Distance") \
  X(motion_sensor, "Capteur de présence", "Motion Sensor") \
  X(motion_sensor_desc, "Détecteur de mouvement PIR", "PIR Motion Detector") \
  X(motion_sensor_pin, "Pin du capteur de présence", "Motion Sensor Pin") \
  X(test_motion_sensor, "Tester la présence", "Test Motion Sensor") \
  X(motion_detected, "Mouvement détecté", "Motion detected") \
  X(no_motion, "Aucun mouvement", "No motion") \
  X(sensors_section, "Tests des capteurs", "Sensor Tests") \
  X(sensors_intro, "Configurez et testez les capteurs connectés à l'ESP32.", "Configure and test the sensors connected to the ESP32.") \
  X(display_signal_section, "Affichage & Signal", "Display & Signal") \
  X(display_signal_intro, "Préparez les sorties visuelles et sonores pour vos diagnostics.", "Prepare visual and audio outputs for your diagnostics.") \
  X(hardware_tests_section, "Tests matériel", "Hardware Tests") \
  X(apply_config, "Appliquer la configuration", "Apply Configuration") \
  X(pin_configuration, "Configuration des pins", "Pin Configuration") \
  X(refresh_data, "Actualiser", "Refresh")

// --- Contenu de languages.h à partir de TranslationKey ---
enum TranslationKey {
#define DECL_KEY(identifier, fr, en) TR_##identifier,
  TRANSLATION_MAP(DECL_KEY)
#undef DECL_KEY
  TR_COUNT
};

#define T() TranslationAccessor(&currentLanguage)

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

  // Déclaration des champs pour l'accès
#define DECL_FIELD(identifier, fr, en) TranslationField identifier;
  TRANSLATION_MAP(DECL_FIELD)
#undef DECL_FIELD
};

#endif // LANGUAGES_H