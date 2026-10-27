/**
 * wm_strings_en.h
 * english strings for
 * WiFiManager, a library for the ESP32 platform
 * for configuration of WiFi credentials using a Captive Portal
 *
 * Only translatable words belong in this file. All markup, css and js lives in
 * wm_html.h and is pulled in through the WM_HTML_* macros.
 *
 * @author Creator tzapu
 * @author tablatronix
 * @version 0.0.0
 * @license MIT
 */

#ifndef _WM_STRINGS_EN_H_
#define _WM_STRINGS_EN_H_


#ifndef WIFI_MANAGER_OVERRIDE_STRINGS
// !!! ABOVE WILL NOT WORK if you define in your sketch, must be build flag, if anyone one knows how to order includes to be able to do this it would be neat.. I have seen it done..

// strings files must include a consts file and the shared html skeleton!
#include "wm_consts.h" // include constants, tokens, routes
#include "wm_html.h"   // include shared html/css/js and WM_HTML_* templates

const char WM_LANGUAGE[] PROGMEM = "en-US"; // i18n lang code

const char HTTP_HEAD_START[]       PROGMEM = WM_HTML_HEAD_START("en");

const char * const HTTP_PORTAL_MENU[] PROGMEM = {
  WM_HTML_MENU_ITEM("/wifi", "Configure WiFi"),            // MENU_WIFI
  WM_HTML_MENU_ITEM("/0wifi", "Configure WiFi (No scan)"), // MENU_WIFINOSCAN
  WM_HTML_MENU_ITEM_S("/info", "Info"),                    // MENU_INFO
  WM_HTML_MENU_ITEM("/param", "Setup"),                    // MENU_PARAM
  WM_HTML_MENU_ITEM_S("/close", "Close"),                  // MENU_CLOSE
  WM_HTML_MENU_ITEM("/restart", "Restart"),                // MENU_RESTART
  WM_HTML_MENU_ITEM_S("/exit", "Exit"),                    // MENU_EXIT
  WM_HTML_MENU_ITEM_D("/erase", "Erase"),                  // MENU_ERASE
  WM_HTML_MENU_ITEM("/update", "Update"),                  // MENU_UPDATE
  "<hr><br/>"                                              // MENU_SEP
};

const char HTTP_FORM_WIFI[]        PROGMEM = WM_HTML_FORM_WIFI("Password", "Show Password");
const char HTTP_FORM_END[]         PROGMEM = WM_HTML_FORM_END("Save");

const char HTTP_SCAN_LINK[]        PROGMEM = WM_HTML_SCAN_LINK("Refresh");
const char HTTP_SAVED[]            PROGMEM = WM_HTML_SAVED("Saving Credentials<br/>Trying to connect ESP to network.<br/>If it fails reconnect to AP to try again");
const char HTTP_PARAMSAVED[]       PROGMEM = WM_HTML_PARAMSAVED("Saved<br/>");
const char HTTP_ERASEBTN[]         PROGMEM = WM_HTML_ERASEBTN("Erase WiFi config");
const char HTTP_UPDATEBTN[]        PROGMEM = WM_HTML_UPDATEBTN("Update");
const char HTTP_BACKBTN[]          PROGMEM = WM_HTML_BACKBTN("Back");

const char HTTP_STATUS_ON[]        PROGMEM = WM_HTML_STATUS_ON("Connected", "to", "with IP");
const char HTTP_STATUS_OFF[]       PROGMEM = WM_HTML_STATUS_OFF("Not connected", "to"); // {c=class} {v=ssid} {r=status_off}
const char HTTP_STATUS_OFFPW[]     PROGMEM = "<br/>Authentication failure"; // STATION_WRONG_PASSWORD,  no eps32
const char HTTP_STATUS_OFFNOAP[]   PROGMEM = "<br/>AP not found";   // WL_NO_SSID_AVAIL
const char HTTP_STATUS_OFFFAIL[]   PROGMEM = "<br/>Could not connect"; // WL_CONNECT_FAILED
const char HTTP_STATUS_NONE[]      PROGMEM = WM_HTML_STATUS_NONE("No AP set");

#ifndef WM_NOHELP
const char HTTP_HELP[]             PROGMEM = WM_HTML_HELP(
  "Available pages", "Page", "Function",
  "Menu page.",
  "Show WiFi scan results and enter WiFi configuration.(/0wifi noscan)",
  "Save WiFi configuration information and configure device. Needs variables supplied.",
  "Parameter page",
  "Information page",
  "OTA Update",
  "Close the captiveportal popup, config portal will remain active",
  "Exit Config portal, config portal will close",
  "Reboot the device",
  "Erase WiFi configuration and reboot device. Device will not reconnect to a network until new WiFi configuration data is entered."
);
#else
const char HTTP_HELP[]             PROGMEM = "";
#endif

const char HTTP_UPDATE[] PROGMEM = WM_HTML_UPDATE(
  "Upload new firmware",
  "Update",
  "* May not function inside captive portal, open in browser http://192.168.4.1"
);
const char HTTP_UPDATE_FAIL[] PROGMEM = WM_HTML_UPDATE_FAIL("Update failed!", "Reboot device and try again");
const char HTTP_UPDATE_SUCCESS[] PROGMEM = WM_HTML_UPDATE_SUCCESS("Update successful.", "Device rebooting now...");

#ifdef WM_JSTEST
const char HTTP_JS[] PROGMEM =
"<script>function postAjax(url, data, success) {"
"    var params = typeof data == 'string' ? data : Object.keys(data).map("
"            function(k){ return encodeURIComponent(k) + '=' + encodeURIComponent(data[k]) }"
"        ).join('&');"
"    var xhr = window.XMLHttpRequest ? new XMLHttpRequest() : new ActiveXObject(\"Microsoft.XMLHTTP\");"
"    xhr.open('POST', url);"
"    xhr.onreadystatechange = function() {"
"        if (xhr.readyState>3 && xhr.status==200) { success(xhr.responseText); }"
"    };"
"    xhr.setRequestHeader('X-Requested-With', 'XMLHttpRequest');"
"    xhr.setRequestHeader('Content-Type', 'application/x-www-form-urlencoded');"
"    xhr.send(params);"
"    return xhr;}"
"postAjax('/status', 'p1=1&p2=Hello+World', function(data){ console.log(data); });"
"postAjax('/status', { p1: 1, p2: 'Hello World' }, function(data){ console.log(data); });"
"</script>";
#endif

// Info html, markup comes from wm_html.h
const char HTTP_INFO_chiprev[]    PROGMEM = WM_HTML_INFO("Chip rev");
const char HTTP_INFO_lastreset[]  PROGMEM = WM_HTML_INFO_LASTRESET("Last reset reason");
const char HTTP_INFO_aphost[]     PROGMEM = WM_HTML_INFO("Access point hostname");
const char HTTP_INFO_psrsize[]    PROGMEM = WM_HTML_INFO_BYTES("PSRAM Size");
const char HTTP_INFO_temp[]       PROGMEM = WM_HTML_INFO_TEMP("Temperature");
const char HTTP_INFO_hall[]       PROGMEM = WM_HTML_INFO("Hall");

const char HTTP_INFO_memsketch[]  PROGMEM = WM_HTML_INFO_MEMSKETCH("Memory - Sketch size", "Used / Total bytes");
const char HTTP_INFO_freeheap[]   PROGMEM = WM_HTML_INFO_BYTES_AVAIL("Memory - Free heap");
const char HTTP_INFO_uptime[]     PROGMEM = WM_HTML_INFO_UPTIME("Uptime", "mins", "secs");
const char HTTP_INFO_chipid[]     PROGMEM = WM_HTML_INFO("Chip ID");
const char HTTP_INFO_idesize[]    PROGMEM = WM_HTML_INFO_BYTES("Flash size");
const char HTTP_INFO_sdkver[]     PROGMEM = WM_HTML_INFO("SDK version");
const char HTTP_INFO_cpufreq[]    PROGMEM = WM_HTML_INFO_MHZ("CPU frequency");
const char HTTP_INFO_apip[]       PROGMEM = WM_HTML_INFO("Access point IP");
const char HTTP_INFO_apmac[]      PROGMEM = WM_HTML_INFO("Access point MAC");
const char HTTP_INFO_apssid[]     PROGMEM = WM_HTML_INFO("Access point SSID");
const char HTTP_INFO_apbssid[]    PROGMEM = WM_HTML_INFO("BSSID");
const char HTTP_INFO_stassid[]    PROGMEM = WM_HTML_INFO("Station SSID");
const char HTTP_INFO_staip[]      PROGMEM = WM_HTML_INFO("Station IP");
const char HTTP_INFO_stagw[]      PROGMEM = WM_HTML_INFO("Station gateway");
const char HTTP_INFO_stasub[]     PROGMEM = WM_HTML_INFO("Station subnet");
const char HTTP_INFO_dnss[]       PROGMEM = WM_HTML_INFO("DNS Server");
const char HTTP_INFO_host[]       PROGMEM = WM_HTML_INFO("Hostname");
const char HTTP_INFO_stamac[]     PROGMEM = WM_HTML_INFO("Station MAC");
const char HTTP_INFO_conx[]       PROGMEM = WM_HTML_INFO("Connected");
const char HTTP_INFO_autoconx[]   PROGMEM = WM_HTML_INFO("Autoconnect");

const char HTTP_INFO_aboutver[]     PROGMEM = WM_HTML_INFO("WiFiManager");
const char HTTP_INFO_aboutarduino[] PROGMEM = WM_HTML_INFO("Arduino");
const char HTTP_INFO_aboutsdk[]     PROGMEM = WM_HTML_INFO("ESP-SDK/IDF");
const char HTTP_INFO_aboutdate[]    PROGMEM = WM_HTML_INFO("Build date");

const char S_brand[]              PROGMEM = "WiFiManager";
const char S_debugPrefix[]        PROGMEM = "*wm:";
const char S_y[]                  PROGMEM = "Yes";
const char S_n[]                  PROGMEM = "No";
const char S_enable[]             PROGMEM = "Enabled";
const char S_disable[]            PROGMEM = "Disabled";
const char S_GET[]                PROGMEM = "GET";
const char S_POST[]               PROGMEM = "POST";
const char S_NA[]                 PROGMEM = "Unknown";
const char S_passph[]             PROGMEM = "********";
const char S_titlewifisaved[]     PROGMEM = "Credentials saved";
const char S_titlewifisettings[]  PROGMEM = "Settings saved";
const char S_titlewifi[]          PROGMEM = "Config ESP";
const char S_titleinfo[]          PROGMEM = "Info";
const char S_titleparam[]         PROGMEM = "Setup";
const char S_titleparamsaved[]    PROGMEM = "Setup saved";
const char S_titleexit[]          PROGMEM = "Exit";
const char S_titlereset[]         PROGMEM = "Reset";
const char S_titleerase[]         PROGMEM = "Erase";
const char S_titleclose[]         PROGMEM = "Close";
const char S_options[]            PROGMEM = "options";
const char S_nonetworks[]         PROGMEM = "No networks found. Refresh to scan again.";
const char S_staticip[]           PROGMEM = "Static IP";
const char S_staticgw[]           PROGMEM = "Static gateway";
const char S_staticdns[]          PROGMEM = "Static DNS";
const char S_subnet[]             PROGMEM = "Subnet";
const char S_exiting[]            PROGMEM = "Exiting";
const char S_resetting[]          PROGMEM = "Module will reset in a few seconds.";
const char S_closing[]            PROGMEM = "You can close the page, portal will continue to run";
const char S_error[]              PROGMEM = "An error occured";
const char S_notfound[]           PROGMEM = "File not found\n\n";
const char S_uri[]                PROGMEM = "URI: ";
const char S_method[]             PROGMEM = "\nMethod: ";
const char S_args[]               PROGMEM = "\nArguments: ";
const char S_parampre[]           PROGMEM = "param_";

// debug strings
const char D_HR[]                 PROGMEM = "--------------------";


// softap ssid default prefix
const char S_ssidpre[]        PROGMEM = "ESP32";

// END WIFI_MANAGER_OVERRIDE_STRINGS
#endif

#endif
