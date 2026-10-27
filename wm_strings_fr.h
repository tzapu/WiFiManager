/**
 * wm_strings_fr.h
 * French strings for
 * WiFiManager, a library for the ESP32 platform
 * for configuration of WiFi credentials using a Captive Portal
 *
 * Only translatable words belong in this file. All markup, css and js lives in
 * wm_html.h and is pulled in through the WM_HTML_* macros.
 *
 * @author Creator tzapu
 * @author tablatronix
 * @version 0.0.1
 * @translated AByGCreation
 * @license MIT
 */

#ifndef _WM_STRINGS_FR_H_
#define _WM_STRINGS_FR_H_


#ifndef WIFI_MANAGER_OVERRIDE_STRINGS
// !!! ABOVE WILL NOT WORK if you define in your sketch, must be build flag, if anyone one knows how to order includes to be able to do this it would be neat.. I have seen it done..

// strings files must include a consts file and the shared html skeleton!
#include "wm_consts.h" // include constants, tokens, routes
#include "wm_html.h"   // include shared html/css/js and WM_HTML_* templates

const char WM_LANGUAGE[] PROGMEM = "fr-FR"; // i18n lang code

const char HTTP_HEAD_START[]       PROGMEM = WM_HTML_HEAD_START("fr");

const char * const HTTP_PORTAL_MENU[] PROGMEM = {
  WM_HTML_MENU_ITEM("/wifi", "Configurer WiFi"),            // MENU_WIFI
  WM_HTML_MENU_ITEM("/0wifi", "Configurer WiFi (sans scan)"), // MENU_WIFINOSCAN
  WM_HTML_MENU_ITEM_S("/info", "Info"),                     // MENU_INFO
  WM_HTML_MENU_ITEM("/param", "Configuration"),             // MENU_PARAM
  WM_HTML_MENU_ITEM_S("/close", "Fermer"),                  // MENU_CLOSE
  WM_HTML_MENU_ITEM("/restart", "Redémarrer"),              // MENU_RESTART
  WM_HTML_MENU_ITEM_S("/exit", "Quitter"),                  // MENU_EXIT
  WM_HTML_MENU_ITEM_D("/erase", "Effacer"),                 // MENU_ERASE
  WM_HTML_MENU_ITEM("/update", "Mettre à jour"),            // MENU_UPDATE
  "<hr><br/>"                                               // MENU_SEP
};

const char HTTP_FORM_WIFI[]        PROGMEM = WM_HTML_FORM_WIFI("Mot de passe", "Voir le mot de passe");
const char HTTP_FORM_END[]         PROGMEM = WM_HTML_FORM_END("Enregistrer");

const char HTTP_SCAN_LINK[]        PROGMEM = WM_HTML_SCAN_LINK("Rafraichir");
const char HTTP_SAVED[]            PROGMEM = WM_HTML_SAVED("Enregistrer l'accès<br/>L'ESP essai de se connecter au réseau.<br/>Si échec, merci de vous reconnecter à nouveau.");
const char HTTP_PARAMSAVED[]       PROGMEM = WM_HTML_PARAMSAVED("Enregistré<br/>");
const char HTTP_ERASEBTN[]         PROGMEM = WM_HTML_ERASEBTN("Effacer la configuration WiFi");
const char HTTP_UPDATEBTN[]        PROGMEM = WM_HTML_UPDATEBTN("Mettre à jour");
const char HTTP_BACKBTN[]          PROGMEM = WM_HTML_BACKBTN("Retour");

const char HTTP_STATUS_ON[]        PROGMEM = WM_HTML_STATUS_ON("Connecté", "à", "avec IP");
const char HTTP_STATUS_OFF[]       PROGMEM = WM_HTML_STATUS_OFF("Non connecté", "à"); // {c=class} {v=ssid} {r=status_off}
const char HTTP_STATUS_OFFPW[]     PROGMEM = "<br/>Échec d'authentification"; // STATION_WRONG_PASSWORD,  no eps32
const char HTTP_STATUS_OFFNOAP[]   PROGMEM = "<br/>Point d'accès introuvable";   // WL_NO_SSID_AVAIL
const char HTTP_STATUS_OFFFAIL[]   PROGMEM = "<br/>Impossible de se connecter"; // WL_CONNECT_FAILED
const char HTTP_STATUS_NONE[]      PROGMEM = WM_HTML_STATUS_NONE("Aucune borne sélectionnée");

#ifndef WM_NOHELP
const char HTTP_HELP[]             PROGMEM = WM_HTML_HELP(
  "Pages disponibles", "Page", "Fonctions",
  "Menu.",
  "Afficher les résultats de l'analyse WiFi et entrer dans la configuration WiFi.(/0wifi noscan)",
  "Sauvegarde les informations de configuration WiFi et configure l'appareil. Veuillez entrer vos informations de connexion",
  "Parametres",
  "Informations",
  "Mise à jour OTA",
  "Fermer la fenêtre contextuelle du portail captif, le portail de configuration reste actif.",
  "Quitter le portail de configuration, le portail de configuration se ferme",
  "Redémarrer l'appareil",
  "Effacer la configuration WiFi et redémarrer l'appareil. L'appareil ne se reconnectera pas à un réseau tant que de nouvelles données de configuration WiFi n'auront pas été saisies."
);
#else
const char HTTP_HELP[]             PROGMEM = "";
#endif

const char HTTP_UPDATE[] PROGMEM = WM_HTML_UPDATE(
  "Charger le nouveau firmware",
  "Mise à jour",
  "* Peut ne pas fonctionner à l'intérieur du portail captif, ouvrir dans le navigateur http://192.168.4.1"
);
const char HTTP_UPDATE_FAIL[] PROGMEM = WM_HTML_UPDATE_FAIL("Echec de la mise à jour !", "Redémarrer l'appareil et réessayer");
const char HTTP_UPDATE_SUCCESS[] PROGMEM = WM_HTML_UPDATE_SUCCESS("Mise à jour réussie.", "L'appareil redémarre maintenant...");

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
const char HTTP_INFO_chiprev[]    PROGMEM = WM_HTML_INFO("Version Puce");
const char HTTP_INFO_lastreset[]  PROGMEM = WM_HTML_INFO_LASTRESET("Motif de la dernière réinitialisation");
const char HTTP_INFO_aphost[]     PROGMEM = WM_HTML_INFO("Nom du point d'accès");
const char HTTP_INFO_psrsize[]    PROGMEM = WM_HTML_INFO_BYTES("Taille PSRAM");
const char HTTP_INFO_temp[]       PROGMEM = WM_HTML_INFO_TEMP("Temperature");
const char HTTP_INFO_hall[]       PROGMEM = WM_HTML_INFO("Hall");

const char HTTP_INFO_memsketch[]  PROGMEM = WM_HTML_INFO_MEMSKETCH("Mémoire - Taille du programme", "Utilisée / Total");
const char HTTP_INFO_freeheap[]   PROGMEM = WM_HTML_INFO_BYTES_AVAIL("Mémoire - Tampon");
const char HTTP_INFO_uptime[]     PROGMEM = WM_HTML_INFO_UPTIME("Temps de fonctionnement", "mins", "secs");
const char HTTP_INFO_chipid[]     PROGMEM = WM_HTML_INFO("ID de l'ESP");
const char HTTP_INFO_idesize[]    PROGMEM = WM_HTML_INFO_BYTES("Taille mémoire Flash");
const char HTTP_INFO_sdkver[]     PROGMEM = WM_HTML_INFO("Version SDK");
const char HTTP_INFO_cpufreq[]    PROGMEM = WM_HTML_INFO_MHZ("Fréquence processeur");
const char HTTP_INFO_apip[]       PROGMEM = WM_HTML_INFO("IP du point d'accès");
const char HTTP_INFO_apmac[]      PROGMEM = WM_HTML_INFO("Adresse MAC du point d'accès");
const char HTTP_INFO_apssid[]     PROGMEM = WM_HTML_INFO("SSID du point d'accès");
const char HTTP_INFO_apbssid[]    PROGMEM = WM_HTML_INFO("BSSID");
const char HTTP_INFO_stassid[]    PROGMEM = WM_HTML_INFO("Identifiant réseau WiFi");
const char HTTP_INFO_staip[]      PROGMEM = WM_HTML_INFO("Adresse IP");
const char HTTP_INFO_stagw[]      PROGMEM = WM_HTML_INFO("Passerelle");
const char HTTP_INFO_stasub[]     PROGMEM = WM_HTML_INFO("Masque de sous-réseau");
const char HTTP_INFO_dnss[]       PROGMEM = WM_HTML_INFO("Serveur DNS");
const char HTTP_INFO_host[]       PROGMEM = WM_HTML_INFO("Nom d'hôte");
const char HTTP_INFO_stamac[]     PROGMEM = WM_HTML_INFO("Adresse MAC");
const char HTTP_INFO_conx[]       PROGMEM = WM_HTML_INFO("Connecté");
const char HTTP_INFO_autoconx[]   PROGMEM = WM_HTML_INFO("Autoconnect");

const char HTTP_INFO_aboutver[]     PROGMEM = WM_HTML_INFO("WiFiManager");
const char HTTP_INFO_aboutarduino[] PROGMEM = WM_HTML_INFO("Arduino");
const char HTTP_INFO_aboutsdk[]     PROGMEM = WM_HTML_INFO("ESP-SDK/IDF");
const char HTTP_INFO_aboutdate[]    PROGMEM = WM_HTML_INFO("Date de compilation");

const char S_brand[]              PROGMEM = "WiFiManager";
const char S_debugPrefix[]        PROGMEM = "*wm:";
const char S_y[]                  PROGMEM = "Oui";
const char S_n[]                  PROGMEM = "Non";
const char S_enable[]             PROGMEM = "Activé";
const char S_disable[]            PROGMEM = "Desactivé";
const char S_GET[]                PROGMEM = "GET";
const char S_POST[]               PROGMEM = "POST";
const char S_NA[]                 PROGMEM = "Inconnue";
const char S_passph[]             PROGMEM = "********";
const char S_titlewifisaved[]     PROGMEM = "Identifiants sauvegardés";
const char S_titlewifisettings[]  PROGMEM = "Paramètres enregistrés";
const char S_titlewifi[]          PROGMEM = "Configuration ESP";
const char S_titleinfo[]          PROGMEM = "Info";
const char S_titleparam[]         PROGMEM = "Configuration";
const char S_titleparamsaved[]    PROGMEM = "Configuration sauvegardée";
const char S_titleexit[]          PROGMEM = "Quitter";
const char S_titlereset[]         PROGMEM = "Réinitialiser";
const char S_titleerase[]         PROGMEM = "Effacer";
const char S_titleclose[]         PROGMEM = "Fermer";
const char S_options[]            PROGMEM = "options";
const char S_nonetworks[]         PROGMEM = "Aucun réseau trouvé. Rafraîchir pour scanner à nouveau.";
const char S_staticip[]           PROGMEM = "IP Statique";
const char S_staticgw[]           PROGMEM = "Passerelle";
const char S_staticdns[]          PROGMEM = "DNS";
const char S_subnet[]             PROGMEM = "Sous réseau";
const char S_exiting[]            PROGMEM = "Sortie";
const char S_resetting[]          PROGMEM = "Le module redémarrera dans quelques secondes.";
const char S_closing[]            PROGMEM = "Vous pouvez fermer la page, le portail continuera de fonctionner";
const char S_error[]              PROGMEM = "Une erreur est survenue";
const char S_notfound[]           PROGMEM = "Fichier introuvable\n\n";
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
