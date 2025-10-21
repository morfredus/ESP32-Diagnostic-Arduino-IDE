#pragma once

// ============================================================================
// ESP32-Diagnostic user configuration
// ----------------------------------------------------------------------------
// Edit the values below to match your test bench before compiling.
// This file is versioned so you can track the options available with each
// release. Feel free to adjust the defaults to suit your board.
// ============================================================================

// --- Network identity -------------------------------------------------------
#ifndef MDNS_HOSTNAME
#define MDNS_HOSTNAME "esp32-diagnostic"
#endif

// --- Built-in LED / NeoPixel defaults --------------------------------------
#ifndef CUSTOM_LED_PIN
// Set to -1 to let the firmware auto-detect the built-in LED GPIO.
#define CUSTOM_LED_PIN (-1)
#endif

#ifndef CUSTOM_LED_COUNT
// Number of pixels for the optional NeoPixel strip connected to LED_PIN.
#define CUSTOM_LED_COUNT (1)
#endif

// --- I2C bus configuration --------------------------------------------------
#ifndef ENABLE_I2C_SCAN
// Enable or disable the automatic I2C scanner at boot.
#define ENABLE_I2C_SCAN true
#endif

#ifndef DEFAULT_I2C_SDA
#define DEFAULT_I2C_SDA 21
#endif

#ifndef DEFAULT_I2C_SCL
#define DEFAULT_I2C_SCL 20
#endif

// --- OLED display defaults --------------------------------------------------
#ifndef SCREEN_WIDTH
#define SCREEN_WIDTH 128
#endif

#ifndef SCREEN_HEIGHT
#define SCREEN_HEIGHT 64
#endif

#ifndef OLED_RESET
#define OLED_RESET (-1)
#endif

#ifndef SCREEN_ADDRESS
#define SCREEN_ADDRESS 0x3C
#endif

// --- Bluetooth diagnostics --------------------------------------------------
#ifndef ENABLE_BLUETOOTH_AUTOTEST
// Keep to true to propose the Bluetooth self-test button when the stack is
// available. Set to false to hide the action while keeping status reporting.
#define ENABLE_BLUETOOTH_AUTOTEST true
#endif

// --- Wireless indicator refresh (milliseconds) ------------------------------
#ifndef WIRELESS_STATUS_REFRESH_MS
#define WIRELESS_STATUS_REFRESH_MS 3000
#endif

// Add any project-wide constants you want to expose to the firmware below.
// They will remain available across updates thanks to this dedicated file.

