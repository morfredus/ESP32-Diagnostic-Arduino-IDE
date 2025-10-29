# ESP32 Diagnostic Suite – Web Interface Guide (v3.4.0)

The built-in web dashboard is the primary control surface for technicians. This guide covers the layout, interaction patterns, and
customisation hooks available in version 3.4.0.

## Layout overview
```
┌──────────────────────────────────────────┐
│ Header bar: board identity & network     │
├──────────────────────────────────────────┤
│ Status tiles (Wi-Fi, BLE, version, etc.) │
├──────────────────────────────────────────┤
│ Diagnostic controls & live log           │
├──────────────────────────────────────────┤
│ Results table & export actions           │
└──────────────────────────────────────────┘
```

### Header bar
- Displays board hostname, active firmware version, Wi-Fi SSID, IP address, and mDNS URL.
- Shows coloured indicators (green/orange/red) based on the latest diagnostic status.
- Provides quick access buttons for restarting diagnostics and toggling language.

### Status tiles
- **Wi-Fi tile** – connection strength, channel, last latency sample.
- **BLE tile** – enabled on boards with BLE hardware; shows advertisement count.
- **Version tile** – echoes `DIAGNOSTIC_VERSION` and build timestamp.
- **Storage tile** – indicates free heap and PSRAM when detected.

### Diagnostic controls
- `Run full diagnostics` – executes the complete module pipeline.
- `Run quick check` – executes connectivity, memory, and reporting modules only.
- `Stop` – aborts current run; results are preserved for later review.
- Live log streams module status messages with timestamp and severity badges.

### Results table
- Displays PASS/WARN/FAIL badges per module with summary metrics.
- Clicking a row expands the section to show extended data (JSON preview, raw sample counts).
- Export buttons generate TXT, JSON, CSV, or printer-friendly HTML on demand.

## Accessibility & localisation
- All actionable controls include ARIA labels and keyboard shortcuts (`Space/Enter` to trigger primary buttons).
- Language switch toggles between English and French without reloading the page; text resources are loaded from `languages.h`.
- Version 3.4.0 restores translation-safe `String` conversions so tile labels and exports stay synchronised after toggles.
- High-contrast theme activated automatically when the browser prefers dark mode.

## Custom branding
To customise the dashboard appearance:

1. Edit the CSS block in `web_interface.h` under `/* THEME */` comments.
2. Replace the logo placeholder in the HTML template (search for `<!-- LOGO -->`).
3. Adjust colour tokens to match your branding guidelines while keeping sufficient contrast (WCAG AA+ recommended).

## Extending the interface
- Use the placeholder `<div id="custom-widgets">` to inject additional panels at runtime.
- Subscribe to the `diagnosticUpdate` event in the bundled JavaScript to receive module-by-module updates.
- Add custom export handlers by extending `window.DiagnosticExports` with new functions.

## Offline usage
- The dashboard assets are embedded and cached by the ESP32 file system; once loaded, the UI continues operating even if the
  network connection is temporarily interrupted.
- When running in hotspot mode, the interface falls back to `http://192.168.4.1/` with the same features.

## Related resources
- [Usage guide](USAGE.md) – operational workflows and export automation recipes.
- [REST API reference](API_REFERENCE.md) – programmatic triggers that mirror UI actions.
- [Security guidelines](SECURITY.md) – recommendations for protecting the web dashboard in shared environments.
