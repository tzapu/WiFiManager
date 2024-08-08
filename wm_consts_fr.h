/**
 * wm_consts.h
 * internal const strings/tokens
 * WiFiManager, a library for the ESP8266/Arduino platform
 * for configuration of WiFi credentials using a Captive Portal
 *
 * @author Creator tzapu
 * @author tablatronix
 * @version 0.0.1
 * @translated AByGCreation
 * @license MIT
 */

#ifndef _WM_CONSTS_H
#define _WM_CONSTS_H


// -----------------------------------------------------------------------------------------------
// TOKENS

const char WM_VERSION_STR[] PROGMEM = "v2.0.17";

static const char _wifi_token[]       PROGMEM = "wifi";
static const char _wifinoscan_token[] PROGMEM = "wifinoscan";
static const char _info_token[]       PROGMEM = "info";
static const char _param_token[]      PROGMEM = "param";
static const char _close_token[]      PROGMEM = "close";
static const char _restart_token[]    PROGMEM = "restart";
static const char _exit_token[]       PROGMEM = "exit";
static const char _erase_token[]      PROGMEM = "erase";
static const char _update_token[]     PROGMEM = "update";
static const char _sep_token[]        PROGMEM = "sep";
static const char _custom_token[]     PROGMEM = "custom";
static PGM_P _menutokens[] PROGMEM = {
    _wifi_token,
    _wifinoscan_token,
    _info_token,
    _param_token,
    _close_token,
    _restart_token,
    _exit_token,
    _erase_token,
    _update_token,
    _sep_token,
    _custom_token
};
const uint8_t _nummenutokens = (sizeof(_menutokens) / sizeof(PGM_P));


const char R_root[]               PROGMEM = "/";
const char R_wifi[]               PROGMEM = "/wifi";
const char R_wifinoscan[]         PROGMEM = "/0wifi";
const char R_wifisave[]           PROGMEM = "/wifisave";
const char R_info[]               PROGMEM = "/info";
const char R_param[]              PROGMEM = "/param";
const char R_paramsave[]          PROGMEM = "/paramsave";
const char R_restart[]            PROGMEM = "/restart";
const char R_exit[]               PROGMEM = "/exit";
const char R_close[]              PROGMEM = "/close";
const char R_erase[]              PROGMEM = "/erase";
const char R_status[]             PROGMEM = "/status";
const char R_update[]             PROGMEM = "/update";
const char R_updatedone[]         PROGMEM = "/u";


//Strings
const char S_ip[]                 PROGMEM = "ip";
const char S_gw[]                 PROGMEM = "gw";
const char S_sn[]                 PROGMEM = "sn";
const char S_dns[]                PROGMEM = "dns";



//Tokens
//@todo consolidate and reduce
const char T_ss[]                 PROGMEM = "{"; // token start sentinel
const char T_es[]                 PROGMEM = "}"; // token end sentinel
const char T_1[]                  PROGMEM = "{1}"; // @token 1
const char T_2[]                  PROGMEM = "{2}"; // @token 2
const char T_3[]                  PROGMEM = "{3}"; // @token 2
const char T_v[]                  PROGMEM = "{v}"; // @token v
const char T_V[]                  PROGMEM = "{V}"; // @token v
const char T_I[]                  PROGMEM = "{I}"; // @token I
const char T_i[]                  PROGMEM = "{i}"; // @token i
const char T_n[]                  PROGMEM = "{n}"; // @token n
const char T_p[]                  PROGMEM = "{p}"; // @token p
const char T_t[]                  PROGMEM = "{t}"; // @token t
const char T_l[]                  PROGMEM = "{l}"; // @token l
const char T_c[]                  PROGMEM = "{c}"; // @token c
const char T_e[]                  PROGMEM = "{e}"; // @token e
const char T_q[]                  PROGMEM = "{q}"; // @token q
const char T_r[]                  PROGMEM = "{r}"; // @token r
const char T_R[]                  PROGMEM = "{R}"; // @token R
const char T_h[]                  PROGMEM = "{h}"; // @token h

// http
const char HTTP_HEAD_CL[]         PROGMEM = "Content-Length";
const char HTTP_HEAD_CT[]         PROGMEM = "text/html";
const char HTTP_HEAD_CT2[]        PROGMEM = "text/plain";
const char HTTP_HEAD_CORS[]       PROGMEM = "Access-Control-Allow-Origin";
const char HTTP_HEAD_CORS_ALLOW_ALL[]  PROGMEM = "*";

const char * const WIFI_STA_STATUS[] PROGMEM
{
  "WL_IDLE_STATUS",     // 0 STATION_IDLE
  "WL_NO_SSID_AVAIL",   // 1 STATION_NO_AP_FOUND
  "WL_SCAN_COMPLETED",  // 2
  "WL_CONNECTED",       // 3 STATION_GOT_IP
  "WL_CONNECT_FAILED",  // 4 STATION_CONNECT_FAIL, STATION_WRONG_PASSWORD(NI)
  "WL_CONNECTION_LOST", // 5
  "WL_DISCONNECTED",    // 6
  "WL_STATION_WRONG_PASSWORD" // 7 KLUDGE
};

#ifdef ESP32
const char * const AUTH_MODE_NAMES[] PROGMEM
{
    "OPEN",
    "WEP",
    "WPA_PSK",
    "WPA2_PSK",
    "WPA_WPA2_PSK",
    "WPA2_ENTERPRISE",
    "MAX"
};
#elif defined(ESP8266)
const char * const AUTH_MODE_NAMES[] PROGMEM
{
    "",
    "",
    "WPA_PSK",      // 2 ENC_TYPE_TKIP
    "",
    "WPA2_PSK",     // 4 ENC_TYPE_CCMP
    "WEP",          // 5 ENC_TYPE_WEP
    "",
    "OPEN",         //7 ENC_TYPE_NONE
    "WPA_WPA2_PSK", // 8 ENC_TYPE_AUTO
};
#endif

const char* const WIFI_MODES[] PROGMEM = { "NULL", "STA", "AP", "STA+AP" };


#ifdef ESP32
// as 2.5.2
// typedef struct {
//     char                  cc[3];   /**< country code string */
//     uint8_t               schan;   /**< start channel */
//     uint8_t               nchan;   /**< total channel number */
//     int8_t                max_tx_power;   /**< This field is used for getting WiFi maximum transmitting power, call esp_wifi_set_max_tx_power to set the maximum transmitting power. */
//     wifi_country_policy_t policy;  /**< country policy */
// } wifi_country_t;
const wifi_country_t WM_COUNTRY_US{"US",1,11,CONFIG_ESP32_PHY_MAX_WIFI_TX_POWER,WIFI_COUNTRY_POLICY_AUTO};
const wifi_country_t WM_COUNTRY_CN{"CN",1,13,CONFIG_ESP32_PHY_MAX_WIFI_TX_POWER,WIFI_COUNTRY_POLICY_AUTO};
const wifi_country_t WM_COUNTRY_JP{"JP",1,14,CONFIG_ESP32_PHY_MAX_WIFI_TX_POWER,WIFI_COUNTRY_POLICY_AUTO};
#elif defined(ESP8266) && !defined(WM_NOCOUNTRY)
// typedef struct {
//     char cc[3];               /**< country code string */
//     uint8_t schan;            /**< start channel */
//     uint8_t nchan;            /**< total channel number */
//     uint8_t policy;           /**< country policy */
// } wifi_country_t;
const wifi_country_t WM_COUNTRY_US{"US",1,11,WIFI_COUNTRY_POLICY_AUTO};
const wifi_country_t WM_COUNTRY_CN{"CN",1,13,WIFI_COUNTRY_POLICY_AUTO};
const wifi_country_t WM_COUNTRY_JP{"JP",1,14,WIFI_COUNTRY_POLICY_AUTO};
#endif


/*
* ESP32 WiFi Events

0  SYSTEM_EVENT_WIFI_READY               < ESP32 WiFi ready
1  SYSTEM_EVENT_SCAN_DONE                < ESP32 finish scanning AP
2  SYSTEM_EVENT_STA_START                < ESP32 station start
3  SYSTEM_EVENT_STA_STOP                 < ESP32 station stop
4  SYSTEM_EVENT_STA_CONNECTED            < ESP32 station connected to AP
5  SYSTEM_EVENT_STA_DISCONNECTED         < ESP32 station disconnected from AP
6  SYSTEM_EVENT_STA_AUTHMODE_CHANGE      < the auth mode of AP connected by ESP32 station changed
7  SYSTEM_EVENT_STA_GOT_IP               < ESP32 station got IP from connected AP
8  SYSTEM_EVENT_STA_LOST_IP              < ESP32 station lost IP and the IP is reset to 0
9  SYSTEM_EVENT_STA_WPS_ER_SUCCESS       < ESP32 station wps succeeds in enrollee mode
10 SYSTEM_EVENT_STA_WPS_ER_FAILED        < ESP32 station wps fails in enrollee mode
11 SYSTEM_EVENT_STA_WPS_ER_TIMEOUT       < ESP32 station wps timeout in enrollee mode
12 SYSTEM_EVENT_STA_WPS_ER_PIN           < ESP32 station wps pin code in enrollee mode
13 SYSTEM_EVENT_AP_START                 < ESP32 soft-AP start
14 SYSTEM_EVENT_AP_STOP                  < ESP32 soft-AP stop
15 SYSTEM_EVENT_AP_STACONNECTED          < a station connected to ESP32 soft-AP
16 SYSTEM_EVENT_AP_STADISCONNECTED       < a station disconnected from ESP32 soft-AP
17 SYSTEM_EVENT_AP_STAIPASSIGNED         < ESP32 soft-AP assign an IP to a connected station
18 SYSTEM_EVENT_AP_PROBEREQRECVED        < Receive probe request packet in soft-AP interface
19 SYSTEM_EVENT_GOT_IP6                  < ESP32 station or ap or ethernet interface v6IP addr is preferred
20 SYSTEM_EVENT_ETH_START                < ESP32 ethernet start
21 SYSTEM_EVENT_ETH_STOP                 < ESP32 ethernet stop
22 SYSTEM_EVENT_ETH_CONNECTED            < ESP32 ethernet phy link up
23 SYSTEM_EVENT_ETH_DISCONNECTED         < ESP32 ethernet phy link down
24 SYSTEM_EVENT_ETH_GOT_IP               < ESP32 ethernet got IP from connected AP
25 SYSTEM_EVENT_MAX


typedef enum {
    ARDUINO_EVENT_WIFI_READY = 0,
    ARDUINO_EVENT_WIFI_SCAN_DONE,
    ARDUINO_EVENT_WIFI_STA_START,
    ARDUINO_EVENT_WIFI_STA_STOP,
    ARDUINO_EVENT_WIFI_STA_CONNECTED,
    ARDUINO_EVENT_WIFI_STA_DISCONNECTED,
    ARDUINO_EVENT_WIFI_STA_AUTHMODE_CHANGE,
    ARDUINO_EVENT_WIFI_STA_GOT_IP,
    ARDUINO_EVENT_WIFI_STA_GOT_IP6,
    ARDUINO_EVENT_WIFI_STA_LOST_IP,
    ARDUINO_EVENT_WIFI_AP_START,
    ARDUINO_EVENT_WIFI_AP_STOP,
    ARDUINO_EVENT_WIFI_AP_STACONNECTED,
    ARDUINO_EVENT_WIFI_AP_STADISCONNECTED,
    ARDUINO_EVENT_WIFI_AP_STAIPASSIGNED,
    ARDUINO_EVENT_WIFI_AP_PROBEREQRECVED,
    ARDUINO_EVENT_WIFI_AP_GOT_IP6,
    ARDUINO_EVENT_WIFI_FTM_REPORT,
    ARDUINO_EVENT_ETH_START,
    ARDUINO_EVENT_ETH_STOP,
    ARDUINO_EVENT_ETH_CONNECTED,
    ARDUINO_EVENT_ETH_DISCONNECTED,
    ARDUINO_EVENT_ETH_GOT_IP,
    ARDUINO_EVENT_ETH_GOT_IP6,
    ARDUINO_EVENT_WPS_ER_SUCCESS,
    ARDUINO_EVENT_WPS_ER_FAILED,
    ARDUINO_EVENT_WPS_ER_TIMEOUT,
    ARDUINO_EVENT_WPS_ER_PIN,
    ARDUINO_EVENT_WPS_ER_PBC_OVERLAP,
    ARDUINO_EVENT_SC_SCAN_DONE,
    ARDUINO_EVENT_SC_FOUND_CHANNEL,
    ARDUINO_EVENT_SC_GOT_SSID_PSWD,
    ARDUINO_EVENT_SC_SEND_ACK_DONE,
    ARDUINO_EVENT_PROV_INIT,
    ARDUINO_EVENT_PROV_DEINIT,
    ARDUINO_EVENT_PROV_START,
    ARDUINO_EVENT_PROV_END,
    ARDUINO_EVENT_PROV_CRED_RECV,
    ARDUINO_EVENT_PROV_CRED_FAIL,
    ARDUINO_EVENT_PROV_CRED_SUCCESS,
    ARDUINO_EVENT_MAX
} arduino_event_id_t;

typedef union {
    wifi_event_sta_scan_done_t wifi_scan_done;
    wifi_event_sta_authmode_change_t wifi_sta_authmode_change;
    wifi_event_sta_connected_t wifi_sta_connected;
    wifi_event_sta_disconnected_t wifi_sta_disconnected;
    wifi_event_sta_wps_er_pin_t wps_er_pin;
    wifi_event_sta_wps_fail_reason_t wps_fail_reason;
    wifi_event_ap_probe_req_rx_t wifi_ap_probereqrecved;
    wifi_event_ap_staconnected_t wifi_ap_staconnected;
    wifi_event_ap_stadisconnected_t wifi_ap_stadisconnected;
    wifi_event_ftm_report_t wifi_ftm_report;
    ip_event_ap_staipassigned_t wifi_ap_staipassigned;
    ip_event_got_ip_t got_ip;
    ip_event_got_ip6_t got_ip6;
    smartconfig_event_got_ssid_pswd_t sc_got_ssid_pswd;
    esp_eth_handle_t eth_connected;
    wifi_sta_config_t prov_cred_recv;
    wifi_prov_sta_fail_reason_t prov_fail_reason;
} arduino_event_info_t;

*/

#endif