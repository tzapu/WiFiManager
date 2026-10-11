// host unit test for wm_multiwifi.h
// g++ -std=c++11 -Wall -Wextra -I../.. test_multiwifi.cpp -o /tmp/t && /tmp/t
#include "wm_multiwifi.h"
#include <cstdio>
#include <cstdlib>

static int fails = 0;
#define CHECK(x) do{ if(!(x)){ printf("FAIL %s:%d %s\n", __FILE__, __LINE__, #x); fails++; } }while(0)

static WiFiManagerScanItem S(const char* ssid, int rssi, int ch, uint8_t b){
  WiFiManagerScanItem it; it.ssid = ssid; it.rssi = rssi; it.channel = ch; it.hasBssid = true;
  for(int i = 0; i < 6; i++) it.bssid[i] = b;
  return it;
}

int main(){
  static_assert(sizeof(WiFiManagerCredential) == 104, "credential layout");
  WiFiManagerCredentialList l;

  // put / find / validation
  CHECK(l.put("home", "pw1", 0) == 0);
  CHECK(l.put("", "x", 0) == -1);
  CHECK(l.put("123456789012345678901234567890123", "x", 0) == -1); // 33 chars
  CHECK(l.put("12345678901234567890123456789012", "x", 0) == 1);   // 32 chars ok
  char longpass[66]; for(int i = 0; i < 65; i++) longpass[i] = 'a'; longpass[65] = 0;
  CHECK(l.put("p", longpass, 0) == -1);
  longpass[64] = 0;
  CHECK(l.put("p64", longpass, 0) == 2);
  CHECK(strlen(l.items[2].pass) == 64);
  CHECK(l.put("home", "pw2", WM_CRED_HIDDEN) == 0); // update in place
  CHECK(strcmp(l.items[0].pass, "pw2") == 0 && l.items[0].hidden());
  CHECK(l.items.size() == 3);

  // capacity, refuse when full
  CHECK(l.put("d", "", 0) == 3);
  CHECK(l.put("e", "", 0) == 4);
  CHECK(l.full());
  CHECK(l.put("f", "", 0) == -1);
  CHECK(l.put("e", "new", 0) == 4); // update still ok when full

  // lru / mru / touch
  CHECK(l.mru() == -1);
  CHECK(l.lru() == 0); // all never connected, oldest first
  l.touch(3); l.touch(0); l.touch(4);
  CHECK(l.mru() == 4);
  CHECK(l.lru() == 1); // never connected
  CHECK(!l.touch(4));  // already mru, unchanged -> no store
  l.items[3].flags = WM_CRED_UNVERIFIED;
  CHECK(l.touch(3) && !l.items[3].unverified() && l.mru() == 3);
  std::vector<uint8_t> ord; l.order(ord);
  CHECK(ord.size() == 5 && ord[0] == 3 && ord[1] == 4 && ord[2] == 0 && ord[3] == 1 && ord[4] == 2);
  CHECK(l.remove(l.lru()) && l.items.size() == 4 && l.find("12345678901234567890123456789012") < 0);
  CHECK(!l.remove(-1) && !l.remove(9));

  // blob round trip
  std::vector<uint8_t> blob(l.blobSize());
  l.toBlob(blob.data());
  WiFiManagerCredentialList r;
  CHECK(r.fromBlob(blob.data(), blob.size()));
  CHECK(r.items.size() == 4 && strcmp(r.items[0].ssid, "home") == 0 && r.items[0].hidden() && r.mru() == l.mru());
  CHECK(!r.fromBlob(blob.data(), 3));
  CHECK(!r.fromBlob(blob.data(), blob.size() - 1)); // truncated
  blob[2] = 99; CHECK(!r.fromBlob(blob.data(), blob.size())); // version
  WiFiManagerCredentialList empty; std::vector<uint8_t> eb(empty.blobSize()); empty.toBlob(eb.data());
  CHECK(r.fromBlob(eb.data(), eb.size()) && r.items.empty()); // stored empty != never stored

  // plan: visible strongest first, best bssid per ssid, hidden last, skip
  WiFiManagerCredentialList p;
  p.put("A", "a", 0); p.put("B", "b", 0); p.put("H", "h", WM_CRED_HIDDEN); p.put("C", "c", 0); p.put("H2", "h", WM_CRED_HIDDEN);
  p.touch(4); p.touch(2); // H most recent hidden
  WiFiManagerScanItem scan[] = { S("A", -80, 1, 1), S("x", -30, 6, 2), S("B", -60, 11, 3), S("A", -50, 6, 4), S("", -40, 3, 5), S("C", -90, 1, 6) };
  std::vector<WiFiManagerCandidate> c;
  p.plan(scan, 6, -1, c);
  CHECK(c.size() == 5);
  CHECK(c[0].idx == 0 && c[0].rssi == -50 && c[0].channel == 6 && c[0].bssid[0] == 4 && c[0].hasBssid); // mesh: strongest A
  CHECK(c[1].idx == 1 && c[1].channel == 11);
  CHECK(c[2].idx == 3);
  CHECK(c[3].idx == 2 && !c[3].hasBssid && c[3].channel == 0); // hidden blind, mru first
  CHECK(c[4].idx == 4);
  p.plan(scan, 6, 0, c);
  CHECK(c.size() == 4 && c[0].idx == 1); // last good skipped
  p.plan(nullptr, 0, -1, c);
  CHECK(c.size() == 2 && c[0].idx == 2); // nothing visible: hidden only
  // visible network that is also flagged hidden is tried once, from the scan
  WiFiManagerScanItem scan2[] = { S("H", -70, 4, 9) };
  p.plan(scan2, 1, -1, c);
  CHECK(c.size() == 2 && c[0].idx == 2 && c[0].hasBssid && c[1].idx == 4);

  // stronger: same ssid other bssid (mesh), threshold, exclude current bssid
  WiFiManagerCandidate s;
  uint8_t cur[6] = {1,1,1,1,1,1};
  CHECK(p.stronger(scan, 6, cur, -80, 8, s) && s.idx == 0 && s.bssid[0] == 4);  // A -50 vs -80
  CHECK(!p.stronger(scan, 6, cur, -55, 8, s));                                   // -50 not >= -47
  uint8_t cur4[6] = {4,4,4,4,4,4};
  CHECK(p.stronger(scan, 6, cur4, -75, 8, s) && s.idx == 1);                      // excludes own bssid, B -60
  CHECK(!p.stronger(scan, 6, cur4, -40, 8, s));

  // capacity shrink between builds
  WiFiManagerCredentialList big;
  for(int i = 0; i < WM_MULTIWIFI_MAX; i++){ char n[4]; snprintf(n, 4, "n%d", i); big.put(n, "", 0); big.touch(i); }
  std::vector<uint8_t> bb(big.blobSize()); big.toBlob(bb.data());
  bb[3] = WM_MULTIWIFI_MAX + 1; bb.resize(4 + (WM_MULTIWIFI_MAX + 1) * sizeof(WiFiManagerCredential));
  WiFiManagerCredential extra; memset(&extra, 0, sizeof(extra)); strcpy(extra.ssid, "zz"); extra.lastUsed = 100;
  memcpy(bb.data() + 4 + WM_MULTIWIFI_MAX * sizeof(extra), &extra, sizeof(extra));
  CHECK(r.fromBlob(bb.data(), bb.size()) && r.items.size() == WM_MULTIWIFI_MAX && r.find("zz") >= 0 && r.find("n0") < 0);

  printf(fails ? "%d FAILED\n" : "all passed\n", fails);
  return fails ? 1 : 0;
}
