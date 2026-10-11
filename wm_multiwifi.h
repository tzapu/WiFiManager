/**
 * wm_multiwifi.h
 * Multi WiFi credential list for WiFiManager (build flag WM_MULTIWIFI)
 *
 * Pure logic only (no Arduino / SDK calls) so it can be unit tested on a host,
 * see extras/test_multiwifi.cpp. Storage and radio handling live in WiFiManager.cpp
 *
 * @license MIT
 */

#ifndef _WM_MULTIWIFI_H_
#define _WM_MULTIWIFI_H_

#include <stdint.h>
#include <string.h>
#include <vector>

#ifndef WM_MULTIWIFI_MAX
  #define WM_MULTIWIFI_MAX 5 // max saved networks
#endif

#define WM_CRED_HIDDEN     0x01 // never seen in scans, connect by ssid only
#define WM_CRED_UNVERIFIED 0x02 // saved but never connected successfully

// stored as-is (raw blob), 104 bytes, no implicit padding
struct WiFiManagerCredential {
  char     ssid[33];
  char     pass[65];
  uint8_t  flags;
  uint8_t  reserved;
  uint32_t lastUsed; // connect sequence number, 0 = never connected, higher = more recent

  bool hidden() const     { return flags & WM_CRED_HIDDEN; }
  bool unverified() const { return flags & WM_CRED_UNVERIFIED; }
};

// a connect attempt planned from a scan (or blind for hidden networks)
struct WiFiManagerCandidate {
  uint8_t idx;      // index into credential list
  int8_t  rssi;     // 0 if not seen (hidden)
  uint8_t channel;  // 0 = any
  uint8_t bssid[6];
  bool    hasBssid;
};

// scan record handed to the planner
struct WiFiManagerScanItem {
  const char* ssid;
  int32_t     rssi;
  int32_t     channel;
  uint8_t     bssid[6];
  bool        hasBssid;
};

class WiFiManagerCredentialList {
  public:
    std::vector<WiFiManagerCredential> items;

    int find(const char* ssid) const {
      if(!ssid) return -1;
      for(size_t i = 0; i < items.size(); i++){
        if(strncmp(items[i].ssid, ssid, 32) == 0) return (int)i;
      }
      return -1;
    }

    bool full() const { return items.size() >= WM_MULTIWIFI_MAX; }

    uint32_t maxSeq() const {
      uint32_t m = 0;
      for(auto &c : items) if(c.lastUsed > m) m = c.lastUsed;
      return m;
    }

    // add or update by ssid; returns index, -1 if full (new ssid) or invalid
    int put(const char* ssid, const char* pass, uint8_t flags){
      if(!ssid || !ssid[0] || strlen(ssid) > 32) return -1;
      if(pass && strlen(pass) > 64) return -1;
      int i = find(ssid);
      if(i < 0){
        if(full()) return -1;
        WiFiManagerCredential c;
        memset(&c, 0, sizeof(c));
        strncpy(c.ssid, ssid, 32);
        items.push_back(c);
        i = items.size() - 1;
      }
      WiFiManagerCredential &c = items[i];
      memset(c.pass, 0, sizeof(c.pass));
      if(pass) strncpy(c.pass, pass, 64);
      c.flags = flags;
      return i;
    }

    // mark connected, becomes most recently used, returns true if anything changed
    bool touch(int i){
      if(i < 0 || i >= (int)items.size()) return false;
      WiFiManagerCredential &c = items[i];
      uint32_t m = maxSeq();
      bool changed = c.unverified() || c.lastUsed != m || m == 0;
      c.flags &= ~WM_CRED_UNVERIFIED;
      if(c.lastUsed != m || m == 0) c.lastUsed = m + 1;
      return changed;
    }

    bool remove(int i){
      if(i < 0 || i >= (int)items.size()) return false;
      items.erase(items.begin() + i);
      return true;
    }

    // most recently connected (verified) entry, -1 if none
    int mru() const {
      int best = -1;
      for(size_t i = 0; i < items.size(); i++){
        if(items[i].lastUsed && (best < 0 || items[i].lastUsed > items[best].lastUsed)) best = i;
      }
      return best;
    }

    // least recently connected, never-connected first (oldest added wins ties), -1 if empty
    int lru() const {
      int best = -1;
      for(size_t i = 0; i < items.size(); i++){
        if(best < 0 || items[i].lastUsed < items[best].lastUsed) best = i;
      }
      return best;
    }

    // indexes ordered most recent first, never-connected last in insertion order
    void order(std::vector<uint8_t> &out) const {
      out.clear();
      for(size_t i = 0; i < items.size(); i++) out.push_back(i);
      for(size_t i = 1; i < out.size(); i++){ // insertion sort, n <= WM_MULTIWIFI_MAX
        uint8_t v = out[i]; size_t j = i;
        while(j > 0 && items[out[j-1]].lastUsed < items[v].lastUsed){ out[j] = out[j-1]; j--; }
        out[j] = v;
      }
    }

    /**
     * plan connect attempts from a scan
     * visible saved networks strongest first, each locked to its strongest bssid+channel,
     * then hidden flagged networks (blind, most recent first)
     * @param skip  credential index to leave out (eg. last-good already tried), -1 for none
     */
    void plan(const WiFiManagerScanItem* scan, int n, int skip, std::vector<WiFiManagerCandidate> &out) const {
      out.clear();
      for(int s = 0; s < n; s++){
        if(!scan[s].ssid || !scan[s].ssid[0]) continue;
        int i = find(scan[s].ssid);
        if(i < 0 || i == skip) continue;
        int8_t rssi = scan[s].rssi < -127 ? -127 : (scan[s].rssi > 0 ? 0 : scan[s].rssi);
        bool dup = false;
        for(auto &c : out){
          if(c.idx != i) continue;
          dup = true;
          if(rssi > c.rssi) fill(c, i, rssi, scan[s]);
          break;
        }
        if(!dup){
          WiFiManagerCandidate c;
          fill(c, i, rssi, scan[s]);
          out.push_back(c);
        }
      }
      for(size_t a = 1; a < out.size(); a++){ // sort by rssi desc
        WiFiManagerCandidate v = out[a]; size_t b = a;
        while(b > 0 && out[b-1].rssi < v.rssi){ out[b] = out[b-1]; b--; }
        out[b] = v;
      }
      std::vector<uint8_t> ord;
      order(ord);
      for(uint8_t i : ord){
        if(i == skip || !items[i].hidden()) continue;
        bool seen = false;
        for(auto &c : out) if(c.idx == i) seen = true;
        if(seen) continue;
        WiFiManagerCandidate c;
        memset(&c, 0, sizeof(c));
        c.idx = i;
        out.push_back(c);
      }
    }

    // strongest scan entry for any saved network that beats `curRssi` by `gain` dB,
    // excluding the bssid we are on, returns false if none
    bool stronger(const WiFiManagerScanItem* scan, int n, const uint8_t* curBssid, int curRssi, int gain, WiFiManagerCandidate &out) const {
      bool found = false;
      for(int s = 0; s < n; s++){
        if(!scan[s].ssid || !scan[s].ssid[0] || !scan[s].hasBssid) continue;
        if(curBssid && memcmp(scan[s].bssid, curBssid, 6) == 0) continue;
        int i = find(scan[s].ssid);
        if(i < 0) continue;
        if(scan[s].rssi < curRssi + gain) continue;
        if(found && scan[s].rssi <= out.rssi) continue;
        fill(out, i, scan[s].rssi < -127 ? -127 : (int8_t)scan[s].rssi, scan[s]);
        found = true;
      }
      return found;
    }

    // serialize, blob = 4 byte header + records
    static const uint8_t VERSION = 1;
    size_t blobSize() const { return 4 + items.size() * sizeof(WiFiManagerCredential); }
    void toBlob(uint8_t* buf) const {
      buf[0] = 'W'; buf[1] = 'M'; buf[2] = VERSION; buf[3] = items.size();
      if(!items.empty()) memcpy(buf + 4, items.data(), items.size() * sizeof(WiFiManagerCredential));
    }
    bool fromBlob(const uint8_t* buf, size_t len){
      items.clear();
      if(len < 4 || buf[0] != 'W' || buf[1] != 'M' || buf[2] != VERSION) return false;
      size_t n = buf[3];
      if(len < 4 + n * sizeof(WiFiManagerCredential)) return false;
      for(size_t i = 0; i < n; i++){
        WiFiManagerCredential c;
        memcpy(&c, buf + 4 + i * sizeof(c), sizeof(c));
        c.ssid[32] = 0; c.pass[64] = 0;
        if(c.ssid[0] && find(c.ssid) < 0) items.push_back(c);
      }
      while(items.size() > WM_MULTIWIFI_MAX) remove(lru()); // capacity shrank between builds
      return true;
    }

  private:
    static void fill(WiFiManagerCandidate &c, int i, int8_t rssi, const WiFiManagerScanItem &s){
      c.idx = i;
      c.rssi = rssi;
      c.channel = (s.channel > 0 && s.channel < 256) ? s.channel : 0;
      c.hasBssid = s.hasBssid;
      if(s.hasBssid) memcpy(c.bssid, s.bssid, 6); else memset(c.bssid, 0, 6);
    }
};

#endif
