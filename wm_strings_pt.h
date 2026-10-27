/**
 * wm_strings_pt.h
 * portuguese strings for
 * WiFiManager, a library for the ESP32 platform
 * for configuration of WiFi credentials using a Captive Portal
 *
 * Only translatable words belong in this file. All markup, css and js lives in
 * wm_html.h and is pulled in through the WM_HTML_* macros.
 *
 * @author Creator tzapu
 * @author tablatronix
 * @version 0.0.0
 * @translated airlomba
 * @license MIT
 */

#ifndef _WM_STRINGS_PT_H_
#define _WM_STRINGS_PT_H_


/**
 * ADD TO BUILD FLAGS
 * -DLANG_PT
 */

#ifndef WIFI_MANAGER_OVERRIDE_STRINGS
// !!! ABOVE WILL NOT WORK if you define in your sketch, must be build flag, if anyone one knows how to order includes to be able to do this it would be neat.. I have seen it done..

// strings files must include a consts file and the shared html skeleton!
// Copy and change to custom locale tokens if necessary, but strings should be good enough
#include "wm_consts.h" // include constants, tokens, routes
#include "wm_html.h"   // include shared html/css/js and WM_HTML_* templates

const char WM_LANGUAGE[] PROGMEM = "pt-PT"; // i18n lang code

const char HTTP_HEAD_START[]       PROGMEM = WM_HTML_HEAD_START("pt");

const char * const HTTP_PORTAL_MENU[] PROGMEM = {
  WM_HTML_MENU_ITEM("/wifi", "Configurar WiFi"),            // MENU_WIFI
  WM_HTML_MENU_ITEM("/0wifi", "Configurar WiFi (sem scan)"), // MENU_WIFINOSCAN
  WM_HTML_MENU_ITEM_S("/info", "Informação"),               // MENU_INFO
  WM_HTML_MENU_ITEM("/param", "Configuração"),              // MENU_PARAM
  WM_HTML_MENU_ITEM_S("/close", "Fechar"),                  // MENU_CLOSE
  WM_HTML_MENU_ITEM("/restart", "Reiniciar"),               // MENU_RESTART
  WM_HTML_MENU_ITEM_S("/exit", "Sair"),                     // MENU_EXIT
  WM_HTML_MENU_ITEM_D("/erase", "Apagar"),                  // MENU_ERASE
  WM_HTML_MENU_ITEM("/update", "Actualizar"),               // MENU_UPDATE
  "<hr><br/>"                                               // MENU_SEP
};

const char HTTP_FORM_WIFI[]        PROGMEM = WM_HTML_FORM_WIFI("Palavra-passe", "Mostrar palavra-passe");
const char HTTP_FORM_END[]         PROGMEM = WM_HTML_FORM_END("Guardar");

const char HTTP_SCAN_LINK[]        PROGMEM = WM_HTML_SCAN_LINK("Actualizar");
const char HTTP_SAVED[]            PROGMEM = WM_HTML_SAVED("Guardando as credenciais<br/>Tentando ligar à rede ESP.<br/>Se falhar, volte a ligar ao AP para nova tentativa");
const char HTTP_PARAMSAVED[]       PROGMEM = WM_HTML_PARAMSAVED("Guardado<br/>");
const char HTTP_ERASEBTN[]         PROGMEM = WM_HTML_ERASEBTN("Apagar Config WiFi");
const char HTTP_UPDATEBTN[]        PROGMEM = WM_HTML_UPDATEBTN("Actualizar");
const char HTTP_BACKBTN[]          PROGMEM = WM_HTML_BACKBTN("Atrás");

const char HTTP_STATUS_ON[]        PROGMEM = WM_HTML_STATUS_ON("Conectado", "a", "com IP");
const char HTTP_STATUS_OFF[]       PROGMEM = WM_HTML_STATUS_OFF("Não conectado", "a"); // {c=class} {v=ssid} {r=status_off}
const char HTTP_STATUS_OFFPW[]     PROGMEM = "<br/>Autenticação falhada"; // STATION_WRONG_PASSWORD,  no eps32
const char HTTP_STATUS_OFFNOAP[]   PROGMEM = "<br/>Não Encontrado";   // WL_NO_SSID_AVAIL
const char HTTP_STATUS_OFFFAIL[]   PROGMEM = "<br/>Não foi possível conectar"; // WL_CONNECT_FAILED
const char HTTP_STATUS_NONE[]      PROGMEM = WM_HTML_STATUS_NONE("Sem AP atribuído");

#ifndef WM_NOHELP
const char HTTP_HELP[]             PROGMEM = WM_HTML_HELP(
  "Páginas disponíveis", "Página", "Função",
  "Menu.",
  "Exibir resultados da pesquisa WiFi e inserir configuração WiFi.(/0wifi noscan)",
  "Guardar configuração WiFi e configurar dispositivo. Necessita os dados fornecidos.",
  "Parâmetros",
  "Informação",
  "Actualização OTA",
  "Fechar o popup captiveportal, configportal continua activo",
  "Sair da Configuração",
  "Reiniciar o dispositivo",
  "Apagar a configuração WiFi e reiniciar o dispositivo. O dispositivo não se conectará à rede até que uma nova configuração WiFi seja inserida."
);
#else
const char HTTP_HELP[]             PROGMEM = "";
#endif

const char HTTP_UPDATE[] PROGMEM = WM_HTML_UPDATE(
  "Upload Novo Firmware",
  "Actualizar",
  "* Pode não funcionar dentro do captive portal, abrir no browser http://192.168.4.1"
);
const char HTTP_UPDATE_FAIL[] PROGMEM = WM_HTML_UPDATE_FAIL("Falhou a actualização!", "Reinicie o dispositivo e volte a tentar");
const char HTTP_UPDATE_SUCCESS[] PROGMEM = WM_HTML_UPDATE_SUCCESS("Actualizado!", "Dispositivo a reiniciar...");

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
const char HTTP_INFO_chiprev[]    PROGMEM = WM_HTML_INFO("Chip Rev");
const char HTTP_INFO_lastreset[]  PROGMEM = WM_HTML_INFO_LASTRESET("Motivo do último reset");
const char HTTP_INFO_aphost[]     PROGMEM = WM_HTML_INFO("Access Point Hostname");
const char HTTP_INFO_psrsize[]    PROGMEM = WM_HTML_INFO_BYTES("Tamanho PSRAM");
const char HTTP_INFO_temp[]       PROGMEM = WM_HTML_INFO_TEMP("Temperatura");
const char HTTP_INFO_hall[]       PROGMEM = WM_HTML_INFO("Hall");

const char HTTP_INFO_memsketch[]  PROGMEM = WM_HTML_INFO_MEMSKETCH("Memória - Tamanho do Sketch", "Usado / Total bytes");
const char HTTP_INFO_freeheap[]   PROGMEM = WM_HTML_INFO_BYTES_AVAIL("Memória - Livre");
const char HTTP_INFO_uptime[]     PROGMEM = WM_HTML_INFO_UPTIME("Tempo ligado", "Mins", "Secs");
const char HTTP_INFO_chipid[]     PROGMEM = WM_HTML_INFO("ID do chip");
const char HTTP_INFO_idesize[]    PROGMEM = WM_HTML_INFO_BYTES("Tamanho da Flash");
const char HTTP_INFO_sdkver[]     PROGMEM = WM_HTML_INFO("Versão do SDK");
const char HTTP_INFO_cpufreq[]    PROGMEM = WM_HTML_INFO_MHZ("Frequência do CPU");
const char HTTP_INFO_apip[]       PROGMEM = WM_HTML_INFO("IP do Access Point");
const char HTTP_INFO_apmac[]      PROGMEM = WM_HTML_INFO("MAC do Access Point");
const char HTTP_INFO_apssid[]     PROGMEM = WM_HTML_INFO("SSID do Access Point");
const char HTTP_INFO_apbssid[]    PROGMEM = WM_HTML_INFO("BSSID");
const char HTTP_INFO_stassid[]    PROGMEM = WM_HTML_INFO("SSID do nó");
const char HTTP_INFO_staip[]      PROGMEM = WM_HTML_INFO("IP do nó");
const char HTTP_INFO_stagw[]      PROGMEM = WM_HTML_INFO("Gateway do nó");
const char HTTP_INFO_stasub[]     PROGMEM = WM_HTML_INFO("Subnet do nó");
const char HTTP_INFO_dnss[]       PROGMEM = WM_HTML_INFO("Servidor DNS");
const char HTTP_INFO_host[]       PROGMEM = WM_HTML_INFO("Hostname");
const char HTTP_INFO_stamac[]     PROGMEM = WM_HTML_INFO("MAC do nó");
const char HTTP_INFO_conx[]       PROGMEM = WM_HTML_INFO("Conectado");
const char HTTP_INFO_autoconx[]   PROGMEM = WM_HTML_INFO("Autoconnect");

const char HTTP_INFO_aboutver[]     PROGMEM = WM_HTML_INFO("WiFiManager");
const char HTTP_INFO_aboutarduino[] PROGMEM = WM_HTML_INFO("Arduino");
const char HTTP_INFO_aboutsdk[]     PROGMEM = WM_HTML_INFO("ESP-SDK/IDF");
const char HTTP_INFO_aboutdate[]    PROGMEM = WM_HTML_INFO("Data da versão");

const char S_brand[]              PROGMEM = "WiFiManager";
const char S_debugPrefix[]        PROGMEM = "*wm:";
const char S_y[]                  PROGMEM = "Sim";
const char S_n[]                  PROGMEM = "Não";
const char S_enable[]             PROGMEM = "Activado";
const char S_disable[]            PROGMEM = "Desactivado";
const char S_GET[]                PROGMEM = "GET";
const char S_POST[]               PROGMEM = "POST";
const char S_NA[]                 PROGMEM = "Desconhecido";
const char S_passph[]             PROGMEM = "********";
const char S_titlewifisaved[]     PROGMEM = "Credenciais Guardadas";
const char S_titlewifisettings[]  PROGMEM = "Configuração Guardada";
const char S_titlewifi[]          PROGMEM = "Config ESP";
const char S_titleinfo[]          PROGMEM = "Info";
const char S_titleparam[]         PROGMEM = "Configuração";
const char S_titleparamsaved[]    PROGMEM = "Configuração Guardada";
const char S_titleexit[]          PROGMEM = "Sair";
const char S_titlereset[]         PROGMEM = "Reiniciar";
const char S_titleerase[]         PROGMEM = "Apagar";
const char S_titleclose[]         PROGMEM = "Fechar";
const char S_options[]            PROGMEM = "opções";
const char S_nonetworks[]         PROGMEM = "Nenhuma rede encontrada. Actualize para nova procura.";
const char S_staticip[]           PROGMEM = "IP estático";
const char S_staticgw[]           PROGMEM = "Gateway estática";
const char S_staticdns[]          PROGMEM = "DNS estático";
const char S_subnet[]             PROGMEM = "Subnet";
const char S_exiting[]            PROGMEM = "Saindo";
const char S_resetting[]          PROGMEM = "O dispositivo reiniciará dentro de segundos.";
const char S_closing[]            PROGMEM = "Pode fechar a página, o portal continuará a funcionar";
const char S_error[]              PROGMEM = "Ocorreu um erro";
const char S_notfound[]           PROGMEM = "Ficheiro não encontrado\n\n";
const char S_uri[]                PROGMEM = "URI: ";
const char S_method[]             PROGMEM = "\nMétodo: ";
const char S_args[]               PROGMEM = "\nArgumentos: ";
const char S_parampre[]           PROGMEM = "param_";

// debug strings
const char D_HR[]                 PROGMEM = "--------------------";


// softap ssid default prefix
const char S_ssidpre[]        PROGMEM = "ESP32";

// END WIFI_MANAGER_OVERRIDE_STRINGS
#endif

#endif
