/**
 * WiFiManagerStorage.h
 *
 * Preferences-backed persistence for WiFiManager.
 *
 * All data is stored in a dedicated NVS namespace so that erasing
 * WiFiManager data never touches application, BLE, or other subsystem data.
 *
 * @license MIT
 */

#ifndef WiFiManagerStorage_h
#define WiFiManagerStorage_h

#include <Arduino.h>
#include <Preferences.h>

class WiFiManagerStorage {
  public:
    WiFiManagerStorage();
    ~WiFiManagerStorage();

    /**
     * Open the NVS namespace and initialise the schema version.
     * Safe to call more than once.
     */
    bool begin();

    bool isReady() const;

    bool hasCredentials();
    String getSSID();
    String getPassword();
    bool saveCredentials(const String &ssid, const String &password);
    bool clearCredentials();

    // multiple saved networks
    bool addNetwork(const String &ssid, const String &password);
    uint8_t getNetworkCount();
    String getNetworkSSID(uint8_t index);
    String getNetworkPassword(uint8_t index);
    bool clearNetworks();

    bool saveParam(const String &id, const String &value);
    String loadParam(const String &id, const String &defaultValue = "");
    bool clearParam(const String &id);

    /**
     * Clear only the WiFiManager namespace. Application data in other
     * namespaces is left untouched.
     */
    bool clearAll();

    uint32_t getSchemaVersion();
    bool setSchemaVersion(uint32_t version);

  private:
    Preferences _prefs;
    bool _ready;

    String keyForParam(const String &id) const;
    String networkKey(uint8_t index, const char *suffix) const;

    static const char *NAMESPACE;
    static const uint32_t SCHEMA_VERSION;
    static const uint8_t MAX_NETWORKS = 5;
};

#endif
