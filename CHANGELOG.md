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
- `wm_html.h`: single source of truth for markup, CSS and JS, with `WM_HTML_*` templates so language files only contain translatable words.
- `WM_CSS_CUSTOM` build flag to replace the default stylesheet.
- `WM_NOHELP` build flag to compile out the help table.
- Kconfig options `WIFIMANAGER_SAVE_TIMEOUT` and `WIFIMANAGER_DISABLE_PORTAL_RADIO_TUNING`.

### Changed

- Removed all `WiFi.persistent()` toggling.
- Replaced target-specific `rom/rtc.h` includes with `esp_system.h`.
- Replaced `WiFiGenericClass::getMode()` with `WiFi.getMode()`.
- Replaced the VLA in scan rendering with `std::vector`.
- Fixed the `WM_G` temporary pointer.
- Fixed broken Arduino version gating.
- Modernized the captive portal stylesheet: system font stack, card layout, 44px touch targets, focus rings, CSS custom properties and dark mode.
- `getHTTPHead()` now reserves 4096 bytes to match the real head size (shell + script + stylesheet).
- Fixed the Spanish portal `lang` attribute, the non-clickable show-password label in es/pt/pt-br, and untranslated Spanish strings.
- Fixed the dark mode rule that painted links and headings as black blocks in de/es/fr/pt/pt-br.
- Portal stability: station auto-reconnect and modem sleep are disabled while the config portal is open and restored on shutdown, so background STA activity no longer drags the softAP off channel and drops portal clients.
- Non-blocking portal now disconnects and disables STA after a failed save, matching the blocking path instead of leaving AP+STA wedged.
- Default save connect timeout is now 15 s (`WIFI_MANAGER_SAVE_TIMEOUT`) instead of unbounded, bounding how long a failed save blocks the portal.
- `setCleanConnect()` is now enabled by default (`WIFI_MANAGER_CLEAN_CONNECT`), avoiding hung STA connects.

### Removed

- ESP8266 code paths, constants, strings, and metadata.
- WPS support.
- Duplicated HTML/CSS/JS constants from every `wm_strings_*.h` file.
- Dead `.footer` CSS rule and the no-op button transition rules.
- `WM_FIXERASECONFIG`, `WM_NOASYNC`, `WM_NOAUTH`, `WM_NOSOFTAPSSID`, `WM_DISCONWORKAROUND`.
