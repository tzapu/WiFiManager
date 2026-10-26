# Changelog

## 3.0.0

### Breaking changes

- ESP32 only. ESP8266 support has been removed.
- Requires Arduino-ESP32 3.x / ESP-IDF 5.x.
- WiFi credentials and custom parameters are stored in a dedicated NVS namespace via `Preferences`.
- `erase(true)` now clears only the WiFiManager namespace, not the entire NVS partition.
- OTA requires authentication by default. Call `setAuth()` before using the OTA page.
- `WiFiManagerParameter` is non-copyable and has a virtual destructor.

### Added

- `WiFiManagerStorage` with schema versioning.
- Automatic persistence and restoration of custom parameters.
- HTTP basic auth via `setAuth()`.
- CSRF protection for WiFi, parameter, and OTA forms.
- `setAPPasswordRequired()` and `setOTAAuthRequired()`.
- Credential redaction in debug logs.
- HTML escaping for custom parameter values.
- Native IDF component metadata (`idf_component.yml`, `Kconfig`).
- Pinned ESP32-only CI.
- Multiple saved network credentials with automatic retry.
- Non-blocking asynchronous scan completion.
- Optional OTA SHA-256 verification via `setOTASHA256()`.
- Common captive portal probe endpoints.
- Portal/connection state tracking via `getState()`.
- Consolidated language-independent constants in `wm_consts.h`.
- HTML buffer reservations to reduce heap fragmentation.

### Changed

- Removed all `WiFi.persistent()` toggling.
- Replaced target-specific `rom/rtc.h` includes with `esp_system.h`.
- Replaced `WiFiGenericClass::getMode()` with `WiFi.getMode()`.
- Replaced the VLA in scan rendering with `std::vector`.
- Fixed the `WM_G` temporary pointer.
- Fixed broken Arduino version gating.

### Removed

- ESP8266 code paths, constants, strings, and metadata.
- WPS support.
- `WM_FIXERASECONFIG`, `WM_NOASYNC`, `WM_NOAUTH`, `WM_NOSOFTAPSSID`, `WM_DISCONWORKAROUND`.
