/**
 * WiFiManagerStorage.cpp
 *
 * Preferences-backed persistence for WiFiManager.
 *
 * @license MIT
 */

#include "WiFiManagerStorage.h"

const char *WiFiManagerStorage::NAMESPACE = "wifimgr";
const uint32_t WiFiManagerStorage::SCHEMA_VERSION = 1;

WiFiManagerStorage::WiFiManagerStorage() : _ready(false) {
}

WiFiManagerStorage::~WiFiManagerStorage() {
  if (_ready) {
    _prefs.end();
  }
}

bool WiFiManagerStorage::begin() {
  if (_ready) {
    return true;
  }

  _ready = _prefs.begin(NAMESPACE, false);
  if (!_ready) {
    return false;
  }

  uint32_t version = _prefs.getUInt("schema", 0);
  if (version == 0) {
    _prefs.putUInt("schema", SCHEMA_VERSION);
  }

  return true;
}

bool WiFiManagerStorage::isReady() const {
  return _ready;
}

bool WiFiManagerStorage::hasCredentials() {
  if (!_ready) {
    return false;
  }
  return _prefs.isKey("ssid") && _prefs.getString("ssid", "") != "";
}

String WiFiManagerStorage::getSSID() {
  if (!_ready) {
    return String();
  }
  return _prefs.getString("ssid", "");
}

String WiFiManagerStorage::getPassword() {
  if (!_ready) {
    return String();
  }
  return _prefs.getString("pass", "");
}

bool WiFiManagerStorage::saveCredentials(const String &ssid, const String &password) {
  if (!_ready || ssid == "") {
    return false;
  }

  bool ok = _prefs.putString("ssid", ssid) > 0;
  ok = (_prefs.putString("pass", password) > 0) && ok;
  ok = addNetwork(ssid, password) && ok;
  return ok;
}

bool WiFiManagerStorage::clearCredentials() {
  if (!_ready) {
    return false;
  }

  bool ok = _prefs.remove("ssid");
  ok = _prefs.remove("pass") && ok;
  ok = clearNetworks() && ok;
  return ok;
}

String WiFiManagerStorage::networkKey(uint8_t index, const char *suffix) const {
  return String("net") + String(index) + suffix;
}

bool WiFiManagerStorage::addNetwork(const String &ssid, const String &password) {
  if (!_ready || ssid == "") {
    return false;
  }

  uint8_t count = getNetworkCount();
  if (count >= MAX_NETWORKS) {
    // Drop the oldest entry and shift the rest down.
    for (uint8_t i = 1; i < MAX_NETWORKS; i++) {
      String srcSSID = networkKey(i, "_ssid");
      String srcPass = networkKey(i, "_pass");
      String dstSSID = networkKey(i - 1, "_ssid");
      String dstPass = networkKey(i - 1, "_pass");
      _prefs.putString(dstSSID.c_str(), _prefs.getString(srcSSID.c_str(), ""));
      _prefs.putString(dstPass.c_str(), _prefs.getString(srcPass.c_str(), ""));
    }
    count = MAX_NETWORKS - 1;
  }

  String ssidKey = networkKey(count, "_ssid");
  String passKey = networkKey(count, "_pass");
  bool ok = _prefs.putString(ssidKey.c_str(), ssid) > 0;
  ok = (_prefs.putString(passKey.c_str(), password) > 0) && ok;
  ok = (_prefs.putUChar("net_count", count + 1) > 0) && ok;
  return ok;
}

uint8_t WiFiManagerStorage::getNetworkCount() {
  if (!_ready) {
    return 0;
  }
  return _prefs.getUChar("net_count", 0);
}

String WiFiManagerStorage::getNetworkSSID(uint8_t index) {
  if (!_ready || index >= MAX_NETWORKS) {
    return String();
  }
  String key = networkKey(index, "_ssid");
  return _prefs.getString(key.c_str(), "");
}

String WiFiManagerStorage::getNetworkPassword(uint8_t index) {
  if (!_ready || index >= MAX_NETWORKS) {
    return String();
  }
  String key = networkKey(index, "_pass");
  return _prefs.getString(key.c_str(), "");
}

bool WiFiManagerStorage::clearNetworks() {
  if (!_ready) {
    return false;
  }

  bool ok = true;
  for (uint8_t i = 0; i < MAX_NETWORKS; i++) {
    String ssidKey = networkKey(i, "_ssid");
    String passKey = networkKey(i, "_pass");
    ok = _prefs.remove(ssidKey.c_str()) && ok;
    ok = _prefs.remove(passKey.c_str()) && ok;
  }
  ok = (_prefs.putUChar("net_count", 0) > 0) && ok;
  return ok;
}

String WiFiManagerStorage::keyForParam(const String &id) const {
  // NVS keys are limited to 15 characters. Hash the parameter ID so that
  // arbitrary user-supplied IDs map to a fixed-length, deterministic key.
  uint32_t hash = 2166136261u;
  for (size_t i = 0; i < id.length(); i++) {
    hash ^= (uint8_t)id[i];
    hash *= 16777619u;
  }
  char buf[16];
  snprintf(buf, sizeof(buf), "p_%08lx", (unsigned long)hash);
  return String(buf);
}

bool WiFiManagerStorage::saveParam(const String &id, const String &value) {
  if (!_ready || id == "") {
    return false;
  }
  return _prefs.putString(keyForParam(id), value) > 0;
}

String WiFiManagerStorage::loadParam(const String &id, const String &defaultValue) {
  if (!_ready || id == "") {
    return defaultValue;
  }
  return _prefs.getString(keyForParam(id), defaultValue);
}

bool WiFiManagerStorage::clearParam(const String &id) {
  if (!_ready || id == "") {
    return false;
  }
  return _prefs.remove(keyForParam(id));
}

bool WiFiManagerStorage::clearAll() {
  if (!_ready) {
    return false;
  }
  return _prefs.clear();
}

uint32_t WiFiManagerStorage::getSchemaVersion() {
  if (!_ready) {
    return 0;
  }
  return _prefs.getUInt("schema", 0);
}

bool WiFiManagerStorage::setSchemaVersion(uint32_t version) {
  if (!_ready) {
    return false;
  }
  return _prefs.putUInt("schema", version) > 0;
}
