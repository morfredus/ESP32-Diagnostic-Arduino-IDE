
#ifndef LANGUAGES_H
#define LANGUAGES_H

#include <Arduino.h>
#include <pgmspace.h>

// --- [NEW FEATURE] Static English UI string registry ---
class TextField {
 public:
  TextField() : value(nullptr) {}
  explicit TextField(const __FlashStringHelper* text) : value(text) {}

  operator const __FlashStringHelper*() const { return value; }

  String str() const {
    return value ? String(value) : String();
  }

 private:
  const __FlashStringHelper* value;
};

#define TEXT_RESOURCE_MAP(X) \
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
  X(partitions_desc, "Inspect the detected flash partitions and sizes") \
  X(load_partitions, "Load Partition Table") \
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
  X(available, "Available") \
  X(not_available, "Not available") \
  X(test, "Test") \
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
  X(sensors_intro, "Connect sensors to test them (DHT11/DHT22, photoresistor, HC-SR04, PIR)") \
  X(dht_sensor, "DHT Sensor") \
  X(dht_sensor_desc, "Digital temperature and humidity sensor") \
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
  X(cpu_score, "CPU Performance Score") \
  X(memory_benchmark, "Memory Benchmark") \
  X(memory_throughput, "Memory Throughput") \
  X(ops_per_us, "ops/µs") \
  X(megabytes_per_sec, "MB/s") \
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
  X(refresh_data, "Refresh") \
  X(bluetooth_advertising_label, "Advertising") \
  X(bluetooth_client_connected, "Client connected") \
  X(bluetooth_client_disconnected, "Client disconnected") \
  X(bluetooth_mac, "Bluetooth MAC") \
  X(bluetooth_not_advertising, "Not advertising") \
  X(bluetooth_section, "Bluetooth") \
  X(bluetooth_status, "Bluetooth Status") \
  X(board, "Board") \
  X(brownout, "Brownout") \
  X(category, "Category") \
  X(channels, "Channels") \
  X(chase, "Chase") \
  X(config, "Config") \
  X(configuration_invalid, "Invalid configuration") \
  X(connected_ssid, "Connected SSID") \
  X(cores, "Cores") \
  X(critical, "Critical") \
  X(days, "days") \
  X(deepsleep_exit, "Deep sleep exit") \
  X(detected_active, "Detected (active)") \
  X(device_count, "Device count") \
  X(devices, "Devices") \
  X(dns, "DNS") \
  X(enable_psram_hint, "Enable PSRAM in the IDE settings") \
  X(error_label, "Error") \
  X(export_after_boot, "Export after boot") \
  X(export_generated, "Export generated") \
  X(fade, "Fade") \
  X(fail, "Fail") \
  X(flash_type, "Flash Type") \
  X(free, "Free") \
  X(frequency, "Frequency") \
  X(gateway, "Gateway") \
  X(gpio_interfaces, "GPIO Interfaces") \
  X(gpio_invalid, "Invalid GPIO") \
  X(gpio_list, "GPIO List") \
  X(hours, "hours") \
  X(i2c_peripherals, "I2C Peripherals") \
  X(ide_config, "IDE Configuration") \
  X(internal_sram, "Internal SRAM") \
  X(max_alloc, "Max Allocation") \
  X(memory_fragmentation, "Memory Fragmentation") \
  X(memory_status, "Memory Status") \
  X(memory_stress, "Memory Stress") \
  X(message_displayed, "Message displayed") \
  X(minutes, "minutes") \
  X(model, "Model") \
  X(neopixel, "NeoPixel") \
  X(none, "None") \
  X(not_tested, "Not tested") \
  X(off, "OFF") \
  X(ok, "OK") \
  X(oled_step_big_text, "Display large text") \
  X(oled_step_diagonals, "Draw diagonals") \
  X(oled_step_executed_prefix, "Step executed:") \
  X(oled_step_final_message, "OLED test complete") \
  X(oled_step_horizontal_lines, "Draw horizontal lines") \
  X(oled_step_moving_square, "Animate moving square") \
  X(oled_step_progress_bar, "Render progress bar") \
  X(oled_step_scroll_text, "Scroll text") \
  X(oled_step_shapes, "Draw shapes") \
  X(oled_step_text_sizes, "Show text sizes") \
  X(oled_step_unavailable, "OLED test unavailable") \
  X(oled_step_unknown, "Unknown OLED step") \
  X(oled_step_welcome, "OLED diagnostic sequence") \
  X(other, "Other") \
  X(parameter, "Parameter") \
  X(performance_bench, "Performance Bench") \
  X(pins, "Pins") \
  X(poweron, "Power-on") \
  X(psram_external, "External PSRAM") \
  X(pwm_test, "PWM Test") \
  X(pwm_desc, "Generate a duty-cycle sweep on the default PWM pin") \
  X(run_pwm_test, "Run PWM Test") \
  X(revision, "Revision") \
  X(signal_power, "Signal power") \
  X(signal_quality, "Signal quality") \
  X(software_reset, "Software reset") \
  X(spi_bus, "SPI Bus") \
  X(spi_desc, "Identify available SPI controllers on the board") \
  X(scan_spi, "Scan SPI Buses") \
  X(subnet_mask, "Subnet Mask") \
  X(supported_not_enabled, "Supported but not enabled") \
  X(total_gpio, "Total GPIO") \
  X(used, "Used") \
  X(very_good, "Very good") \
  X(wifi_connection, "WiFi connection") \
  X(wifi_open_auth, "Open authentication") \
  X(adc_desc, "Analog input diagnostics") \
  X(animations, "Animations") \
  X(apply_color, "Apply Color") \
  X(apply_redetect, "Apply & Redetect") \
  X(benchmark_desc, "Run CPU and memory benchmarks") \
  X(blue, "Blue") \
  X(bluetooth_connection_label, "Connection") \
  X(bluetooth_disable, "Disable Bluetooth") \
  X(bluetooth_enable, "Enable Bluetooth") \
  X(bluetooth_placeholder, "Enter Bluetooth name") \
  X(bluetooth_rename, "Rename Bluetooth") \
  X(bluetooth_reset, "Reset Bluetooth") \
  X(bluetooth_scan, "Scan Bluetooth devices") \
  X(bluetooth_scan_hint, "Start a BLE scan to discover nearby devices") \
  X(bluetooth_support_label, "BLE Support") \
  X(bluetooth_support_no, "Not supported") \
  X(buzzer_pin, "Buzzer Pin") \
  X(check_wiring, "Check wiring") \
  X(click_to_scan, "Click to scan") \
  X(click_to_test, "Click to test") \
  X(configure_neopixel, "Configure NeoPixel") \
  X(custom_color, "Custom Color") \
  X(custom_message, "Custom Message") \
  X(data_export, "Data Export") \
  X(detected_addresses, "Detected Addresses") \
  X(dht_sensor_pin, "DHT Sensor Pin") \
  X(dht_sensor_type, "Sensor type") \
  X(dht11_option, "DHT11") \
  X(dht22_option, "DHT22") \
  X(distance_pins, "Distance Sensor Pins") \
  X(flash_speed, "Flash Speed") \
  X(full_test, "Full Test") \
  X(gpio_warning, "Warning: Automatic GPIO scans toggle every pin. Disconnect sensitive hardware before running this test.") \
  X(green, "Green") \
  X(i2c_pins, "I2C Pins") \
  X(ip_unavailable, "IP unavailable") \
  X(label_echo, "Echo Pin") \
  X(label_trig, "Trigger Pin") \
  X(light_level, "Light Level") \
  X(light_sensor_pin, "Light Sensor Pin") \
  X(motion_sensor_pin, "Motion Sensor Pin") \
  X(no_detected, "No devices detected") \
  X(red, "Red") \
  X(rgb_led_pins, "RGB LED Pins") \
  X(rotation, "Rotation") \
  X(sensors_section, "Sensors") \
  X(show_message, "Show Message") \
  X(start_adc_test, "Start ADC Test") \
  X(stress_desc, "Run stress tests on the device") \
  X(stress_warning, "Stress test may reboot the device") \
  X(test_all_gpio, "Test all GPIO") \
  X(test_buzzer, "Test Buzzer") \
  X(test_dht_sensor, "Test DHT Sensor") \
  X(test_distance_sensor, "Test Distance Sensor") \
  X(test_light_sensor, "Test Light Sensor") \
  X(test_motion_sensor, "Test Motion Sensor") \
  X(test_rgb_led, "Test RGB LED") \
  X(turn_off_all, "Turn off all") \
  X(updating, "Updating...") \
  X(white, "White") \
  X(wifi_desc, "Scan nearby WiFi networks") \
  X(wifi_scanner, "WiFi Scanner") \

namespace Texts {
#define DECLARE_TEXT(identifier, value) \
  inline const TextField identifier = TextField(F(value));
  TEXT_RESOURCE_MAP(DECLARE_TEXT)
#undef DECLARE_TEXT
}  // namespace Texts

#endif  // LANGUAGES_H
