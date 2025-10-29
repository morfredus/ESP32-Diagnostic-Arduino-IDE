# Troubleshooting (EN)

## BLE advertising not visible
- Confirm your board supports BLE (ESP32, ESP32-S3, ESP32-C3, ESP32-C6, ESP32-H2).
- Check the Serial Monitor for BLE initialisation errors.
- Ensure no other application is holding the BLE stack (restart the board).

## Language switch does not change texts
- Ensure you are running firmware 3.4.0 so the unified JSON helpers back `/api/set-language` with explicit FR/EN successes, 400 responses for unsupported codes, and harmonised labels.
- Verify `languages.h` is present alongside the sketch and compiles correctly.
- Clear the browser cache (Ctrl+F5) to reload the translation bundle.
- Confirm `/api/get-translations` returns HTTP 200 using the browser console or a REST client.

## Additional diagnostics show "Non teste" without accent
- Flash firmware 3.4.0 (or any later 3.x maintenance build) so the French catalogue keeps the "Non testé" placeholder and benefits from the restored translation-safe `String` handling.
- Refresh the dashboard (Ctrl+F5) to reload cached translations after updating.

## OLED screen stays blank
- Re-run `/api/oled-config?sda=<pin>&scl=<pin>` with the correct wiring.
- Check the I2C address (0x3C by default). Replace the display if it does not answer on the bus.
- Ensure the display has adequate power (3.3 V, GND shared with the ESP32).

## Wi-Fi connection fails
- Double-check SSID/password pairs in `wifi-config.h`.
- Watch the Serial Monitor for `WL_NO_SSID_AVAIL` or `WL_CONNECT_FAILED` codes.
- Move closer to the access point or try a 2.4 GHz-only SSID.

## Memory fragmentation warnings in exports
- Call `/api/memory-details` and review the `fragmentation` percentage alongside the PSRAM flags produced by the unified JSON helpers in 3.4.0.
- Ensure PSRAM is enabled on ESP32-S3 boards to keep the `psramAvailable` flag consistent with the new documentation.
- If fragmentation stays above 20%, power-cycle the board and re-run diagnostics to clear transient heap usage.

## GPIO tests report conflicts
- Free the pin from other peripherals (I2C, SPI, UART) before launching the sweep.
- Some pins are input-only or reserved (e.g., GPIO34-39). Verify your board schematic.

## Exports download empty files
- Wait until the diagnostic run completes before triggering an export.
- Ensure the browser allows downloads from the device IP.
- Check Serial logs for memory warnings that may interrupt streaming.
