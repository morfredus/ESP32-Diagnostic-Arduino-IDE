#ifndef LANGUAGES_H
#define LANGUAGES_H

#include <Arduino.h>
#include <pgmspace.h>

// ========== ENGLISH-ONLY TRANSLATION SYSTEM v3.7.06 ==========
// Optimized for minimal memory footprint
// Removed bilingual support to save ~50% flash memory

#define TRANSLATION_MAP(X) \
  X(title, "ESP32 Complete Diagnostic") \
  X(version, "v") \
  X(access, "Access") \
  X(or_text, "or") \
  X(nav_overview, "Overview") \
  X(nav_display_signal, "Display & Signal") \
  X(nav_sensors, "Sensors") \
  X(nav_hardware_tests, "Hardware Tests") \
  X(nav_leds, "LEDs") \
  X(nav_screens, "Screens") \
  X(nav_tests, "Advanced Tests") \
  X(nav_gpio, "GPIO") \
  X(nav_wireless, "Wireless") \
  X(nav_benchmark, "Performance") \
  X(nav_export, "Export") \
  X(nav_select_label, "Select a section") \
  X(chip_info, "Detailed Chip Information") \
  X(full_model, "Full Model") \
  X(cpu_cores, "CPU Cores") \
  X(mac_wifi, "WiFi MAC Address") \
  X(last_reset, "Last Reset Reason") \
  X(chip_features, "Chip Features") \
  X(sdk_version, "SDK Version") \
  X(idf_version, "ESP-IDF Version") \
  X(uptime, "Uptime") \
  X(cpu_temp, "CPU Temperature") \
  X(memory_details, "Detailed Memory") \
  X(flash_memory, "Flash Memory") \
  X(real_size, "Actual Size (board)") \
  X(configured_ide, "Configured (IDE)") \
  X(total_size, "Total Size") \
  X(used_size, "Used") \
  X(total_ram, "Total RAM") \
  X(free_ram, "Free") \
  X(used_ram, "Used") \
  X(largest_block, "Largest Block") \
  X(total_psram, "Total PSRAM") \
  X(free_psram, "Free") \
  X(used_psram, "Used") \
  X(psram_unavailable, "PSRAM unavailable") \
  X(detailed_memory, "Detailed Memory") \
  X(allocation_method, "Allocation Method") \
  X(internal, "Internal") \
  X(internal_dma, "Internal (DMA-capable)") \
  X(spiram, "SPIRAM") \
  X(default_caps, "Default") \
  X(partition_list, "Partition List") \
  X(label, "Label") \
  X(type, "Type") \
  X(subtype, "Subtype") \
  X(address, "Address") \
  X(size, "Size") \
  X(app, "app") \
  X(data, "data") \
  X(factory, "factory") \
  X(nvs, "nvs") \
  X(spiffs, "spiffs") \
  X(nvs_keys, "nvs_keys") \
  X(efuse, "efuse") \
  X(esphttpd, "esphttpd") \
  X(fat, "fat") \
  X(ota, "ota") \
  X(phy, "phy") \
  X(coredump, "coredump") \
  X(unknown, "unknown") \
  X(wifi_module, "WiFi Module") \
  X(status, "Status") \
  X(connected, "Connected") \
  X(disconnected, "Disconnected") \
  X(ssid, "SSID") \
  X(ip_address, "IP Address") \
  X(signal_strength, "Signal Strength") \
  X(channel, "Channel") \
  X(quality, "Quality") \
  X(excellent, "Excellent") \
  X(good, "Good") \
  X(average, "Average") \
  X(weak, "Weak") \
  X(very_weak, "Very weak") \
  X(mac_address, "MAC Address") \
  X(hostname, "Hostname") \
  X(peripherals_detected, "Detected Peripherals") \
  X(no_peripherals, "No peripherals detected") \
  X(builtin_led, "Built-in LED") \
  X(builtin_led_desc, "Control the onboard LED of your ESP32 module") \
  X(available, "Available") \
  X(not_available, "Not available") \
  X(test, "Test") \
  X(turn_on, "Turn ON") \
  X(turn_off, "Turn OFF") \
  X(blink, "Blink") \
  X(ws2812b_led, "WS2812B LED Strip") \
  X(neopixel_desc, "Control an external addressable RGB LED strip (WS2812B/NeoPixel)") \
  X(pin, "Pin") \
  X(led_count, "LED Count") \
  X(configure, "Configure") \
  X(test_colors, "Test Colors") \
  X(rainbow, "Rainbow") \
  X(clear, "Clear") \
  X(oled_display, "OLED Display") \
  X(label_sda, "SDA Pin") \
  X(label_scl, "SCL Pin") \
  X(oled_desc, "Configure and test an I2C OLED screen (SSD1306 128x64)") \
  X(changes_pins, "Modify I2C pins and test the OLED screen") \
  X(test_display, "Test Display") \
  X(rgb_led, "RGB LED") \
  X(rgb_led_desc, "Test an RGB LED with separate red, green, and blue pins") \
  X(pin_red, "Red Pin") \
  X(pin_green, "Green Pin") \
  X(pin_blue, "Blue Pin") \
  X(test_red, "Test Red") \
  X(test_green, "Test Green") \
  X(test_blue, "Test Blue") \
  X(test_white, "Test White") \
  X(test_cycle, "Cycle Colors") \
  X(buzzer, "Buzzer") \
  X(buzzer_desc, "Test an active or passive buzzer") \
  X(beep, "Beep") \
  X(melody, "Melody") \
  X(sensors_intro, "Connect sensors to test them (DHT11, photoresistor, HC-SR04, PIR)") \
  X(dht11_sensor, "DHT11 (Temperature & Humidity)") \
  X(dht11_desc, "Digital temperature and humidity sensor") \
  X(read_sensor, "Read Sensor") \
  X(temperature, "Temperature") \
  X(humidity, "Humidity") \
  X(light_sensor, "Photoresistor (Light Sensor)") \
  X(light_sensor_desc, "Analog light intensity sensor") \
  X(light_value, "Light Value") \
  X(distance_sensor, "HC-SR04 (Ultrasonic Distance)") \
  X(distance_sensor_desc, "Ultrasonic distance measurement sensor") \
  X(trigger_pin, "Trigger Pin") \
  X(echo_pin, "Echo Pin") \
  X(measure_distance, "Measure Distance") \
  X(distance, "Distance") \
  X(motion_sensor, "PIR (Motion Detector)") \
  X(motion_sensor_desc, "Passive infrared motion detection sensor") \
  X(check_motion, "Check Motion") \
  X(motion_detected, "Motion detected") \
  X(no_motion, "No motion") \
  X(display_signal_intro, "Prepare visual and audio outputs for various tests and signals") \
  X(gpio_desc, "Test any GPIO pin as output or input") \
  X(gpio, "GPIO") \
  X(gpio_test, "GPIO Test") \
  X(gpio_pin, "Pin") \
  X(mode, "Mode") \
  X(output, "Output") \
  X(input, "Input") \
  X(input_pullup, "Input (PULL-UP)") \
  X(input_pulldown, "Input (PULL-DOWN)") \
  X(action, "Action") \
  X(set_high, "Set HIGH") \
  X(set_low, "Set LOW") \
  X(pulse, "Pulse") \
  X(read_state, "Read State") \
  X(high, "HIGH") \
  X(low, "LOW") \
  X(adc_test, "ADC Test (Analog Reading)") \
  X(adc_pin, "ADC Pin") \
  X(read_adc, "Read ADC") \
  X(raw_value, "Raw Value") \
  X(voltage, "Voltage") \
  X(wifi_scan, "WiFi Network Scan") \
  X(wireless_intro, "Scan nearby WiFi networks and manage Bluetooth") \
  X(scan_networks, "Scan Networks") \
  X(no_networks, "No networks found") \
  X(encryption, "Encryption") \
  X(bluetooth_module, "Bluetooth BLE Module") \
  X(bluetooth_desc, "Manage BLE advertising and scan nearby Bluetooth devices") \
  X(bluetooth_capable, "BLE Capable") \
  X(yes, "Yes") \
  X(no, "No") \
  X(bluetooth_enabled, "BLE Enabled") \
  X(bluetooth_advertising, "BLE Advertising") \
  X(bluetooth_name, "BLE Device Name") \
  X(enable_bluetooth, "Enable BLE") \
  X(disable_bluetooth, "Disable BLE") \
  X(scan_bluetooth, "Scan BLE Devices") \
  X(update_name, "Update Name") \
  X(reset_name, "Reset Name") \
  X(no_bluetooth_devices, "No Bluetooth devices found") \
  X(rssi, "RSSI") \
  X(bluetooth_disabled, "BLE disabled") \
  X(bluetooth_error, "BLE error") \
  X(bluetooth_not_supported, "BLE not supported on this device") \
  X(bluetooth_updated, "BLE name updated") \
  X(bluetooth_reset_done, "BLE name reset to default") \
  X(bluetooth_devices_found, "Found %COUNT% Bluetooth device(s)") \
  X(i2c_scan, "I2C Device Scan") \
  X(scan_i2c, "Scan I2C Bus") \
  X(no_i2c_devices, "No I2C devices found") \
  X(i2c_address, "I2C Address") \
  X(wifi_channel, "Channel") \
  X(cpu_benchmark, "CPU Benchmark") \
  X(memory_benchmark, "Memory Benchmark") \
  X(run_benchmarks, "Run Benchmarks") \
  X(iterations_label, "Iterations") \
  X(allocations_label, "Allocations") \
  X(cpu_time, "CPU Time") \
  X(memory_time, "Memory Time") \
  X(gpio_stress, "GPIO Stress Test") \
  X(start_stress, "Start Stress Test") \
  X(stop_stress, "Stop Test") \
  X(test_duration, "Test Duration") \
  X(test_in_progress, "Test in progress...") \
  X(gpio_test_complete, "GPIO test completed") \
  X(stress_running, "Stress test running...") \
  X(export_intro, "Export diagnostic reports in various formats") \
  X(txt_file, "TXT File") \
  X(json_file, "JSON File") \
  X(csv_file, "CSV File") \
  X(readable_report, "Human-readable diagnostic report") \
  X(structured_format, "Structured data format for automation") \
  X(for_excel, "Tabular format for Excel/spreadsheets") \
  X(download_txt, "Download TXT") \
  X(download_json, "Download JSON") \
  X(download_csv, "Download CSV") \
  X(printable_version, "Printable Version") \
  X(pdf_format, "PDF-optimized format for printing") \
  X(open, "Open") \
  X(interface_loaded, "Interface loaded") \
  X(error_loading, "Error loading data") \
  X(loading, "Loading...") \
  X(refresh, "Refresh") \
  X(save, "Save") \
  X(cancel, "Cancel") \
  X(apply, "Apply") \
  X(name, "Name") \
  X(value, "Value") \
  X(description, "Description") \
  X(enabled, "Enabled") \
  X(disabled, "Disabled") \
  X(success, "Success") \
  X(error, "Error") \
  X(warning, "Warning") \
  X(info, "Info") \
  X(close, "Close") \
  X(led_strip, "LED Strip") \
  X(oled_screen, "OLED Screen") \
  X(detected, "Detected") \
  X(not_detected, "Not detected") \
  X(i2c_devices, "I2C Devices") \
  X(gpio_available, "Available GPIO") \
  X(apply_config, "Apply Configuration") \
  X(pin_configuration, "Pin Configuration") \
  X(refresh_data, "Refresh")

enum TranslationKey {
#define DECL_KEY(identifier, en) TR_##identifier,
  TRANSLATION_MAP(DECL_KEY)
#undef DECL_KEY
  TR_COUNT
};

#define T() TranslationAccessor()

static const char* const TRANSLATIONS[] PROGMEM = {
#define DECL_EN(identifier, en) en,
  TRANSLATION_MAP(DECL_EN)
#undef DECL_EN
};

inline const char* lookupTranslation(TranslationKey key) {
  return TRANSLATIONS[key];
}

class TranslationField {
 public:
  constexpr TranslationField() : key(static_cast<TranslationKey>(0)) {}
  constexpr TranslationField(TranslationKey idx) : key(idx) {}

  operator const char*() const { return c_str(); }

  const char* c_str() const {
    return lookupTranslation(key);
  }

  String str() const { return String(c_str()); }

 private:
  TranslationKey key;
};

class TranslationAccessor {
 public:
  TranslationAccessor() {
#define INIT_FIELD(identifier, en) identifier = TranslationField(TR_##identifier);
    TRANSLATION_MAP(INIT_FIELD)
#undef INIT_FIELD
  }

  // Field declarations for access
#define DECL_FIELD(identifier, en) TranslationField identifier;
  TRANSLATION_MAP(DECL_FIELD)
#undef DECL_FIELD
};

#endif // LANGUAGES_H
