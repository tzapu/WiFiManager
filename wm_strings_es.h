/**
 * wm_strings_es.h
 * spanish strings for
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

#ifndef _WM_STRINGS_ES_H_
#define _WM_STRINGS_ES_H_


/**
 * ADD TO BUILD FLAGS
 * -DLANG_ES
 */

#ifndef WIFI_MANAGER_OVERRIDE_STRINGS
// !!! ABOVE WILL NOT WORK if you define in your sketch, must be build flag, if anyone one knows how to order includes to be able to do this it would be neat.. I have seen it done..

// strings files must include a consts file and the shared html skeleton!
// Copy and change to custom locale tokens if necessary, but strings should be good enough
#include "wm_consts.h" // include constants, tokens, routes
#include "wm_html.h"   // include shared html/css/js and WM_HTML_* templates

const char WM_LANGUAGE[] PROGMEM = "es-ES"; // i18n lang code

const char HTTP_HEAD_START[]       PROGMEM = WM_HTML_HEAD_START("es");

const char * const HTTP_PORTAL_MENU[] PROGMEM = {
  WM_HTML_MENU_ITEM("/wifi", "Configurar WiFi"),            // MENU_WIFI
  WM_HTML_MENU_ITEM("/0wifi", "Configurar WiFi (sin escanear)"), // MENU_WIFINOSCAN
  WM_HTML_MENU_ITEM_S("/info", "Información"),              // MENU_INFO
  WM_HTML_MENU_ITEM("/param", "Configuración"),             // MENU_PARAM
  WM_HTML_MENU_ITEM_S("/close", "Cerrar"),                  // MENU_CLOSE
  WM_HTML_MENU_ITEM("/restart", "Reiniciar"),               // MENU_RESTART
  WM_HTML_MENU_ITEM_S("/exit", "Salir"),                    // MENU_EXIT
  WM_HTML_MENU_ITEM_D("/erase", "Borrar"),                  // MENU_ERASE
  WM_HTML_MENU_ITEM("/update", "Actualizar"),               // MENU_UPDATE
  "<hr><br/>"                                               // MENU_SEP
};

const char HTTP_FORM_WIFI[]        PROGMEM = WM_HTML_FORM_WIFI("Contraseña", "Mostrar contraseña");
const char HTTP_FORM_END[]         PROGMEM = WM_HTML_FORM_END("Guardar");

const char HTTP_SCAN_LINK[]        PROGMEM = WM_HTML_SCAN_LINK("Actualizar");
const char HTTP_SAVED[]            PROGMEM = WM_HTML_SAVED("Guardando credenciales<br/>Intentando conectar el ESP a la red.<br/>Si falla, vuelva a conectarse al AP para intentarlo de nuevo.");
const char HTTP_PARAMSAVED[]       PROGMEM = WM_HTML_PARAMSAVED("Guardado<br/>");
const char HTTP_ERASEBTN[]         PROGMEM = WM_HTML_ERASEBTN("Borrar configuración WiFi");
const char HTTP_UPDATEBTN[]        PROGMEM = WM_HTML_UPDATEBTN("Actualizar");
const char HTTP_BACKBTN[]          PROGMEM = WM_HTML_BACKBTN("Atrás");

const char HTTP_STATUS_ON[]        PROGMEM = WM_HTML_STATUS_ON("Conectado", "a", "con IP");
const char HTTP_STATUS_OFF[]       PROGMEM = WM_HTML_STATUS_OFF("No conectado", "a"); // {c=class} {v=ssid} {r=status_off}
const char HTTP_STATUS_OFFPW[]     PROGMEM = "<br/>Fallo de autenticación"; // STATION_WRONG_PASSWORD,  no eps32
const char HTTP_STATUS_OFFNOAP[]   PROGMEM = "<br/>AP no encontrado";   // WL_NO_SSID_AVAIL
const char HTTP_STATUS_OFFFAIL[]   PROGMEM = "<br/>No se pudo conectar"; // WL_CONNECT_FAILED
const char HTTP_STATUS_NONE[]      PROGMEM = WM_HTML_STATUS_NONE("Sin AP configurado");

#ifndef WM_NOHELP
const char HTTP_HELP[]             PROGMEM = WM_HTML_HELP(
  "Páginas disponibles", "Página", "Función",
  "Página de menú.",
  "Mostrar resultados del escaneo WiFi e introducir la configuración WiFi.(/0wifi sin escaneo)",
  "Guardar la configuración WiFi y configurar el dispositivo. Requiere que se envíen las variables.",
  "Página de parámetros",
  "Página de información",
  "Actualización OTA",
  "Cerrar la ventana del portal cautivo, el portal de configuración seguirá activo",
  "Salir del portal de configuración, el portal se cerrará",
  "Reiniciar el dispositivo",
  "Borrar la configuración WiFi y reiniciar el dispositivo. El dispositivo no se reconectará a una red hasta que se introduzcan nuevos datos de configuración WiFi."
);
#else
const char HTTP_HELP[]             PROGMEM = "";
#endif

const char HTTP_UPDATE[] PROGMEM = WM_HTML_UPDATE(
  "Subir nuevo firmware",
  "Actualizar",
  "* Puede no funcionar dentro del portal cautivo, abrir en el navegador http://192.168.4.1"
);
const char HTTP_UPDATE_FAIL[] PROGMEM = WM_HTML_UPDATE_FAIL("¡Actualización fallida!", "Reinicie el dispositivo e inténtelo de nuevo");
const char HTTP_UPDATE_SUCCESS[] PROGMEM = WM_HTML_UPDATE_SUCCESS("Actualización correcta.", "El dispositivo se está reiniciando...");

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
const char HTTP_INFO_chiprev[]    PROGMEM = WM_HTML_INFO("Rev. del chip");
const char HTTP_INFO_lastreset[]  PROGMEM = WM_HTML_INFO_LASTRESET("Motivo del último reinicio");
const char HTTP_INFO_aphost[]     PROGMEM = WM_HTML_INFO("Hostname del punto de acceso");
const char HTTP_INFO_psrsize[]    PROGMEM = WM_HTML_INFO_BYTES("Tamaño de PSRAM");
const char HTTP_INFO_temp[]       PROGMEM = WM_HTML_INFO_TEMP("Temperatura");
const char HTTP_INFO_hall[]       PROGMEM = WM_HTML_INFO("Hall");

const char HTTP_INFO_memsketch[]  PROGMEM = WM_HTML_INFO_MEMSKETCH("Memoria - Tamaño del sketch", "Usado / Total bytes");
const char HTTP_INFO_freeheap[]   PROGMEM = WM_HTML_INFO_BYTES_AVAIL("Memoria - Heap libre");
const char HTTP_INFO_uptime[]     PROGMEM = WM_HTML_INFO_UPTIME("Tiempo activo", "min", "seg");
const char HTTP_INFO_chipid[]     PROGMEM = WM_HTML_INFO("ID del chip");
const char HTTP_INFO_idesize[]    PROGMEM = WM_HTML_INFO_BYTES("Tamaño de flash");
const char HTTP_INFO_sdkver[]     PROGMEM = WM_HTML_INFO("Versión del SDK");
const char HTTP_INFO_cpufreq[]    PROGMEM = WM_HTML_INFO_MHZ("Frecuencia de CPU");
const char HTTP_INFO_apip[]       PROGMEM = WM_HTML_INFO("IP del punto de acceso");
const char HTTP_INFO_apmac[]      PROGMEM = WM_HTML_INFO("MAC del punto de acceso");
const char HTTP_INFO_apssid[]     PROGMEM = WM_HTML_INFO("SSID del punto de acceso");
const char HTTP_INFO_apbssid[]    PROGMEM = WM_HTML_INFO("BSSID");
const char HTTP_INFO_stassid[]    PROGMEM = WM_HTML_INFO("SSID de la estación");
const char HTTP_INFO_staip[]      PROGMEM = WM_HTML_INFO("IP de la estación");
const char HTTP_INFO_stagw[]      PROGMEM = WM_HTML_INFO("Puerta de enlace de la estación");
const char HTTP_INFO_stasub[]     PROGMEM = WM_HTML_INFO("Subred de la estación");
const char HTTP_INFO_dnss[]       PROGMEM = WM_HTML_INFO("Servidor DNS");
const char HTTP_INFO_host[]       PROGMEM = WM_HTML_INFO("Hostname");
const char HTTP_INFO_stamac[]     PROGMEM = WM_HTML_INFO("MAC de la estación");
const char HTTP_INFO_conx[]       PROGMEM = WM_HTML_INFO("Conectado");
const char HTTP_INFO_autoconx[]   PROGMEM = WM_HTML_INFO("Conexión automática");

const char HTTP_INFO_aboutver[]     PROGMEM = WM_HTML_INFO("WiFiManager");
const char HTTP_INFO_aboutarduino[] PROGMEM = WM_HTML_INFO("Arduino");
const char HTTP_INFO_aboutsdk[]     PROGMEM = WM_HTML_INFO("ESP-SDK/IDF");
const char HTTP_INFO_aboutdate[]    PROGMEM = WM_HTML_INFO("Fecha de compilación");

const char S_brand[]              PROGMEM = "WiFiManager";
const char S_debugPrefix[]        PROGMEM = "*wm:";
const char S_y[]                  PROGMEM = "Sí";
const char S_n[]                  PROGMEM = "No";
const char S_enable[]             PROGMEM = "Activado";
const char S_disable[]            PROGMEM = "Desactivado";
const char S_GET[]                PROGMEM = "GET";
const char S_POST[]               PROGMEM = "POST";
const char S_NA[]                 PROGMEM = "Desconocido";
const char S_passph[]             PROGMEM = "********";
const char S_titlewifisaved[]     PROGMEM = "Credenciales guardadas";
const char S_titlewifisettings[]  PROGMEM = "Ajustes guardados";
const char S_titlewifi[]          PROGMEM = "Configurar ESP";
const char S_titleinfo[]          PROGMEM = "Info";
const char S_titleparam[]         PROGMEM = "Configuración";
const char S_titleparamsaved[]    PROGMEM = "Configuración guardada";
const char S_titleexit[]          PROGMEM = "Salir";
const char S_titlereset[]         PROGMEM = "Reiniciar";
const char S_titleerase[]         PROGMEM = "Borrar";
const char S_titleclose[]         PROGMEM = "Cerrar";
const char S_options[]            PROGMEM = "opciones";
const char S_nonetworks[]         PROGMEM = "No se encontraron redes. Actualice para buscar de nuevo.";
const char S_staticip[]           PROGMEM = "IP estática";
const char S_staticgw[]           PROGMEM = "Puerta de enlace estática";
const char S_staticdns[]          PROGMEM = "DNS estático";
const char S_subnet[]             PROGMEM = "Subred";
const char S_exiting[]            PROGMEM = "Saliendo";
const char S_resetting[]          PROGMEM = "El módulo se reiniciará en unos segundos.";
const char S_closing[]            PROGMEM = "Puede cerrar la página, el portal seguirá funcionando";
const char S_error[]              PROGMEM = "Se ha producido un error";
const char S_notfound[]           PROGMEM = "Archivo no encontrado\n\n";
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
