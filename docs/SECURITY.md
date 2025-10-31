# ESP32 Diagnostic Suite – Security Guidelines (v3.5.0)

Although the ESP32 Diagnostic Suite is typically used in controlled lab environments, following these security guidelines reduces
exposure when testing pre-production hardware.

## Network hardening
- Prefer a dedicated QA Wi-Fi network with WPA2-PSK or WPA2-Enterprise credentials.
- Change default hotspot SSID/password in `wifi-config.h` before field trials.
- Disable hotspot fallback entirely by setting `ENABLE_AP_FALLBACK` to `0` when not required.
- Restrict access to the REST API via firewall rules or reverse proxy authentication when deployed on shared networks.

## Credential management
- Store Wi-Fi credentials in `wifi-config.h` and exclude the file from version control (see `.gitignore`).
- Rotate passwords regularly and update all flashed units before decommissioning old credentials.
- For enterprise WPA2/EAP, use device-specific certificates and revoke them if a unit is lost.

## Firmware integrity
- Verify the SHA-256 hash of compiled binaries before flashing large batches.
- Keep the build environment patched and reproducible (documented in [BUILD_AND_DEPLOY.md](BUILD_AND_DEPLOY.md)).
- Tag releases with annotated Git tags (e.g., `v3.5.0`) to ensure traceability between code and deployed firmware.

## Web interface best practices
- Avoid exposing the dashboard directly to the public internet.
- Use modern browsers with up-to-date TLS libraries when reverse proxying over HTTPS.
- Enable automatic logout in the proxy when diagnostics are run in shared labs.
- Review [WEB_INTERFACE.md](WEB_INTERFACE.md) for ARIA and accessibility considerations that also benefit kiosk deployments.

## Data retention
- Exported JSON/CSV/TXT reports can contain board identifiers and operator notes; treat them as confidential QA records.
- Store exports on secured storage with backup policies aligned with your organisation's compliance requirements.
- Purge device caches after retrieving reports by issuing `POST /api/stop` followed by a power cycle.

## Incident response
- If tampering is suspected, perform a factory reset: erase flash via `esptool.py erase_flash`, then reflash 3.5.0.
- Revoke associated Wi-Fi credentials and update audit logs with report exports for the affected unit.
- File an incident ticket referencing the report `run_id` for traceability.

## Related resources
- [Configuration guide](CONFIG.md)
- [REST API reference](API_REFERENCE.md)
- [Build & deploy](BUILD_AND_DEPLOY.md)
