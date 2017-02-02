#if defined LANG_DE 
const char _HTTP_HEAD_0[] PROGMEM = "<!DOCTYPE html> <html lang=en> <head> <meta name=viewport content=\"width=device-width,initial-scale=1,user-scalable=no\"/> <title>";
const char _HTTP_HEAD_1[] PROGMEM = "</title>";
#define HTTP_HEAD(v) \
	 (String() + (FPSTR(_HTTP_HEAD_0)) + (v) + (FPSTR(_HTTP_HEAD_1)))
const char _HTTP_STYLE[] PROGMEM = "<style>.c{text-align:center}div,input{padding:5px;font-size:1em}input{width:95%}body{text-align:center;font-family:verdana}button{border:0;border-radius:.3rem;background-color:#1fa3ec;color:#fff;line-height:2.4rem;font-size:1.2rem;width:100%}.q{float:right;width:64px;text-align:right}.l{background:url(data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAACAAAAAgCAMAAABEpIrGAAAALVBMVEX///8EBwfBwsLw8PAzNjaCg4NTVVUjJiZDRUUUFxdiZGSho6OSk5Pg4eFydHTCjaf3AAAAZElEQVQ4je2NSw7AIAhEBamKn97/uMXEGBvozkWb9C2Zx4xzWykBhFAeYp9gkLyZE0zIMno9n4g19hmdY39scwqVkOXaxph0ZCXQcqxSpgQpONa59wkRDOL93eAXvimwlbPbwwVAegLS1HGfZAAAAABJRU5ErkJggg==) no-repeat left center;background-size:1em}</style>";
#define HTTP_STYLE (FPSTR(_HTTP_STYLE))
const char _HTTP_SCRIPT[] PROGMEM = "<script>function c(e){document.getElementById(\"s\").value=e.innerText||e.textContent,document.getElementById(\"p\").focus()}</script>";
#define HTTP_SCRIPT (FPSTR(_HTTP_SCRIPT))
const char _HTTP_HEAD_END[] PROGMEM = "</head> <body> <div style=text-align:left;display:inline-block;min-width:260px>";
#define HTTP_HEAD_END (FPSTR(_HTTP_HEAD_END))
const char _HTTP_PORTAL_OPTIONS[] PROGMEM = "<form action=/wifi method=get> <button>WiFi konfigurieren</button> </form><br/> <form action=/0wifi method=get> <button>WiFi konfigurieren (kein Scan)</button> </form><br/> <form action=/i method=get> <button>Info</button> </form><br/> <form action=/r method=post> <button>Zur&uuml;cksetzen</button> </form>";
#define HTTP_PORTAL_OPTIONS (FPSTR(_HTTP_PORTAL_OPTIONS))
const char _HTTP_ITEM_0[] PROGMEM = "<div> <a href=#p onclick=c(this)>";
const char _HTTP_ITEM_1[] PROGMEM = "</a>&nbsp; <span class=\"q";
const char _HTTP_ITEM_2[] PROGMEM = "\">";
const char _HTTP_ITEM_3[] PROGMEM = "%</span> </div>";
#define HTTP_ITEM(v, i, r) \
	 (String() + (FPSTR(_HTTP_ITEM_0)) + (v) + (FPSTR(_HTTP_ITEM_1)) + (i) + (FPSTR(_HTTP_ITEM_2)) + (r) + (FPSTR(_HTTP_ITEM_3)))
const char _HTTP_FORM_START[] PROGMEM = "<form method=get action=wifisave> <input id=s name=s length=32 placeholder=SSID> <br/> <input id=p name=p length=64 type=password placeholder=Passwort> <br/>";
#define HTTP_FORM_START (FPSTR(_HTTP_FORM_START))
const char _HTTP_FORM_PARAM_0[] PROGMEM = "<br/> <input id=";
const char _HTTP_FORM_PARAM_1[] PROGMEM = "name=";
const char _HTTP_FORM_PARAM_2[] PROGMEM = "length=";
const char _HTTP_FORM_PARAM_3[] PROGMEM = "placeholder=";
const char _HTTP_FORM_PARAM_4[] PROGMEM = "value=";
const char _HTTP_FORM_PARAM_5[] PROGMEM = "";
const char _HTTP_FORM_PARAM_6[] PROGMEM = ">";
#define HTTP_FORM_PARAM(i, n, l, p, v, c) \
	 (String() + (FPSTR(_HTTP_FORM_PARAM_0)) + (i) + (FPSTR(_HTTP_FORM_PARAM_1)) + (n) + (FPSTR(_HTTP_FORM_PARAM_2)) + (l) + (FPSTR(_HTTP_FORM_PARAM_3)) + (p) + (FPSTR(_HTTP_FORM_PARAM_4)) + (v) + (FPSTR(_HTTP_FORM_PARAM_5)) + (c) + (FPSTR(_HTTP_FORM_PARAM_6)))
const char _HTTP_FORM_END[] PROGMEM = "<br/> <button type=submit>speichern</button> </form>";
#define HTTP_FORM_END (FPSTR(_HTTP_FORM_END))
const char _HTTP_SCAN_LINK[] PROGMEM = "<br/> <div class=c> <a href=/wifi>scannen</a> </div>";
#define HTTP_SCAN_LINK (FPSTR(_HTTP_SCAN_LINK))
const char _HTTP_SAVED[] PROGMEM = "<div>Zugangsdaten gespeichert <br/>Versuche den ESP mit dem Netzwerk zu verbinden. <br/>Wenn es fehlschl&auml;gt, verbinde Dich wieder mit dem AP und versuche es erneut. </div>";
#define HTTP_SAVED (FPSTR(_HTTP_SAVED))
const char _HTTP_INFO_0[] PROGMEM = "<dl> <dt>Chip ID</dt> <dd>";
const char _HTTP_INFO_1[] PROGMEM = "</dd> <dt>Flash Chip ID</dt> <dd>";
const char _HTTP_INFO_2[] PROGMEM = "</dd> <dt>IDE Flash Gr&ouml;&szlig;e</dt> <dd>";
const char _HTTP_INFO_3[] PROGMEM = "bytes</dd> <dt>Echte Flash Gr&ouml;&szlig;e</dt> <dd>";
const char _HTTP_INFO_4[] PROGMEM = "bytes</dd> <dt>Soft AP IP</dt> <dd>";
const char _HTTP_INFO_5[] PROGMEM = "</dd> <dt>Soft AP MAC</dt> <dd>";
const char _HTTP_INFO_6[] PROGMEM = "</dd> <dt>Station MAC</dt> <dd>";
const char _HTTP_INFO_7[] PROGMEM = "</dd> </dl>";
#define HTTP_INFO(chipid, flashid, idesize, realsize, softip, softmac, smac) \
	 (String() + (FPSTR(_HTTP_INFO_0)) + (chipid) + (FPSTR(_HTTP_INFO_1)) + (flashid) + (FPSTR(_HTTP_INFO_2)) + (idesize) + (FPSTR(_HTTP_INFO_3)) + (realsize) + (FPSTR(_HTTP_INFO_4)) + (softip) + (FPSTR(_HTTP_INFO_5)) + (softmac) + (FPSTR(_HTTP_INFO_6)) + (smac) + (FPSTR(_HTTP_INFO_7)))
const char _HTTP_END[] PROGMEM = "</div> </body> </html>";
#define HTTP_END (FPSTR(_HTTP_END))
const char _TITLE_OPTIONS[] PROGMEM = "Optionen";
#define TITLE_OPTIONS (FPSTR(_TITLE_OPTIONS))
const char _TITLE_CONFIG[] PROGMEM = "ESP konfigureren";
#define TITLE_CONFIG (FPSTR(_TITLE_CONFIG))
const char _TITLE_RESET[] PROGMEM = "Zur&uuml;cksetzen";
#define TITLE_RESET (FPSTR(_TITLE_RESET))
const char _TITLE_INFO[] PROGMEM = "Informationen";
#define TITLE_INFO (FPSTR(_TITLE_INFO))
const char _TITLE_SAVED[] PROGMEM = "Zugangsdaten gespeichert";
#define TITLE_SAVED (FPSTR(_TITLE_SAVED))
const char _MESSAGE_NONETWORKS[] PROGMEM = "Keine Netzwerke gefunden. Neu laden um erneut zu scannen.";
#define MESSAGE_NONETWORKS (FPSTR(_MESSAGE_NONETWORKS))
const char _MESSAGE_RESET[] PROGMEM = "Modul wird in ein paar Sekunden zur&uuml;ckgesetzt.";
#define MESSAGE_RESET (FPSTR(_MESSAGE_RESET))
const char _MESSAGE_NOTFOUND[] PROGMEM = "Datei nicht gefunden";
#define MESSAGE_NOTFOUND (FPSTR(_MESSAGE_NOTFOUND))
const char _MESSAGE_NOTFOUND_URI[] PROGMEM = "URI";
#define MESSAGE_NOTFOUND_URI (FPSTR(_MESSAGE_NOTFOUND_URI))
const char _MESSAGE_NOTFOUND_METHOD[] PROGMEM = "Methode";
#define MESSAGE_NOTFOUND_METHOD (FPSTR(_MESSAGE_NOTFOUND_METHOD))
const char _MESSAGE_NOTFOUND_ARGS[] PROGMEM = "Argumente";
#define MESSAGE_NOTFOUND_ARGS (FPSTR(_MESSAGE_NOTFOUND_ARGS))
const char _OPTION_STATICIP[] PROGMEM = "Statische IP";
#define OPTION_STATICIP (FPSTR(_OPTION_STATICIP))
const char _OPTION_STATICGW[] PROGMEM = "Statisches Gateway";
#define OPTION_STATICGW (FPSTR(_OPTION_STATICGW))
const char _OPTION_STATICSN[] PROGMEM = "Subnetz";
#define OPTION_STATICSN (FPSTR(_OPTION_STATICSN))
#else 
const char _HTTP_HEAD_0[] PROGMEM = "<!DOCTYPE html> <html lang=en> <head> <meta name=viewport content=\"width=device-width,initial-scale=1,user-scalable=no\"/> <title>";
const char _HTTP_HEAD_1[] PROGMEM = "</title>";
#define HTTP_HEAD(v) \
	 (String() + (FPSTR(_HTTP_HEAD_0)) + (v) + (FPSTR(_HTTP_HEAD_1)))
const char _HTTP_STYLE[] PROGMEM = "<style>.c{text-align:center}div,input{padding:5px;font-size:1em}input{width:95%}body{text-align:center;font-family:verdana}button{border:0;border-radius:.3rem;background-color:#1fa3ec;color:#fff;line-height:2.4rem;font-size:1.2rem;width:100%}.q{float:right;width:64px;text-align:right}.l{background:url(data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAACAAAAAgCAMAAABEpIrGAAAALVBMVEX///8EBwfBwsLw8PAzNjaCg4NTVVUjJiZDRUUUFxdiZGSho6OSk5Pg4eFydHTCjaf3AAAAZElEQVQ4je2NSw7AIAhEBamKn97/uMXEGBvozkWb9C2Zx4xzWykBhFAeYp9gkLyZE0zIMno9n4g19hmdY39scwqVkOXaxph0ZCXQcqxSpgQpONa59wkRDOL93eAXvimwlbPbwwVAegLS1HGfZAAAAABJRU5ErkJggg==) no-repeat left center;background-size:1em}</style>";
#define HTTP_STYLE (FPSTR(_HTTP_STYLE))
const char _HTTP_SCRIPT[] PROGMEM = "<script>function c(e){document.getElementById(\"s\").value=e.innerText||e.textContent,document.getElementById(\"p\").focus()}</script>";
#define HTTP_SCRIPT (FPSTR(_HTTP_SCRIPT))
const char _HTTP_HEAD_END[] PROGMEM = "</head> <body> <div style=text-align:left;display:inline-block;min-width:260px>";
#define HTTP_HEAD_END (FPSTR(_HTTP_HEAD_END))
const char _HTTP_PORTAL_OPTIONS[] PROGMEM = "<form action=/wifi method=get> <button>Configure WiFi</button> </form><br/> <form action=/0wifi method=get> <button>Configure WiFi (No Scan)</button> </form><br/> <form action=/i method=get> <button>Info</button> </form><br/> <form action=/r method=post> <button>Reset</button> </form>";
#define HTTP_PORTAL_OPTIONS (FPSTR(_HTTP_PORTAL_OPTIONS))
const char _HTTP_ITEM_0[] PROGMEM = "<div> <a href=#p onclick=c(this)>";
const char _HTTP_ITEM_1[] PROGMEM = "</a>&nbsp; <span class=\"q";
const char _HTTP_ITEM_2[] PROGMEM = "\">";
const char _HTTP_ITEM_3[] PROGMEM = "%</span> </div>";
#define HTTP_ITEM(v, i, r) \
	 (String() + (FPSTR(_HTTP_ITEM_0)) + (v) + (FPSTR(_HTTP_ITEM_1)) + (i) + (FPSTR(_HTTP_ITEM_2)) + (r) + (FPSTR(_HTTP_ITEM_3)))
const char _HTTP_FORM_START[] PROGMEM = "<form method=get action=wifisave> <input id=s name=s length=32 placeholder=SSID> <br/> <input id=p name=p length=64 type=password placeholder=password> <br/>";
#define HTTP_FORM_START (FPSTR(_HTTP_FORM_START))
const char _HTTP_FORM_PARAM_0[] PROGMEM = "<br/> <input id=";
const char _HTTP_FORM_PARAM_1[] PROGMEM = "name=";
const char _HTTP_FORM_PARAM_2[] PROGMEM = "length=";
const char _HTTP_FORM_PARAM_3[] PROGMEM = "placeholder=";
const char _HTTP_FORM_PARAM_4[] PROGMEM = "value=";
const char _HTTP_FORM_PARAM_5[] PROGMEM = "";
const char _HTTP_FORM_PARAM_6[] PROGMEM = ">";
#define HTTP_FORM_PARAM(i, n, l, p, v, c) \
	 (String() + (FPSTR(_HTTP_FORM_PARAM_0)) + (i) + (FPSTR(_HTTP_FORM_PARAM_1)) + (n) + (FPSTR(_HTTP_FORM_PARAM_2)) + (l) + (FPSTR(_HTTP_FORM_PARAM_3)) + (p) + (FPSTR(_HTTP_FORM_PARAM_4)) + (v) + (FPSTR(_HTTP_FORM_PARAM_5)) + (c) + (FPSTR(_HTTP_FORM_PARAM_6)))
const char _HTTP_FORM_END[] PROGMEM = "<br/> <button type=submit>save</button> </form>";
#define HTTP_FORM_END (FPSTR(_HTTP_FORM_END))
const char _HTTP_SCAN_LINK[] PROGMEM = "<br/> <div class=c> <a href=/wifi>Scan</a> </div>";
#define HTTP_SCAN_LINK (FPSTR(_HTTP_SCAN_LINK))
const char _HTTP_SAVED[] PROGMEM = "<div>Credentials Saved <br/>Trying to connect ESP to network. <br/>If it fails reconnect to AP to try again. </div>";
#define HTTP_SAVED (FPSTR(_HTTP_SAVED))
const char _HTTP_INFO_0[] PROGMEM = "<dl> <dt>Chip ID</dt> <dd>";
const char _HTTP_INFO_1[] PROGMEM = "</dd> <dt>Flash Chip ID</dt> <dd>";
const char _HTTP_INFO_2[] PROGMEM = "</dd> <dt>IDE Flash Size</dt> <dd>";
const char _HTTP_INFO_3[] PROGMEM = "bytes</dd> <dt>Real Flash Size</dt> <dd>";
const char _HTTP_INFO_4[] PROGMEM = "bytes</dd> <dt>Soft AP IP</dt> <dd>";
const char _HTTP_INFO_5[] PROGMEM = "</dd> <dt>Soft AP MAC</dt> <dd>";
const char _HTTP_INFO_6[] PROGMEM = "</dd> <dt>Station MAC</dt> <dd>";
const char _HTTP_INFO_7[] PROGMEM = "</dd> </dl>";
#define HTTP_INFO(chipid, flashid, idesize, realsize, softip, softmac, smac) \
	 (String() + (FPSTR(_HTTP_INFO_0)) + (chipid) + (FPSTR(_HTTP_INFO_1)) + (flashid) + (FPSTR(_HTTP_INFO_2)) + (idesize) + (FPSTR(_HTTP_INFO_3)) + (realsize) + (FPSTR(_HTTP_INFO_4)) + (softip) + (FPSTR(_HTTP_INFO_5)) + (softmac) + (FPSTR(_HTTP_INFO_6)) + (smac) + (FPSTR(_HTTP_INFO_7)))
const char _HTTP_END[] PROGMEM = "</div> </body> </html>";
#define HTTP_END (FPSTR(_HTTP_END))
const char _TITLE_OPTIONS[] PROGMEM = "Options";
#define TITLE_OPTIONS (FPSTR(_TITLE_OPTIONS))
const char _TITLE_CONFIG[] PROGMEM = "Config ESP";
#define TITLE_CONFIG (FPSTR(_TITLE_CONFIG))
const char _TITLE_RESET[] PROGMEM = "Reset";
#define TITLE_RESET (FPSTR(_TITLE_RESET))
const char _TITLE_INFO[] PROGMEM = "Info";
#define TITLE_INFO (FPSTR(_TITLE_INFO))
const char _TITLE_SAVED[] PROGMEM = "Credentials Saved";
#define TITLE_SAVED (FPSTR(_TITLE_SAVED))
const char _MESSAGE_NONETWORKS[] PROGMEM = "No networks found. Refresh to scan again.";
#define MESSAGE_NONETWORKS (FPSTR(_MESSAGE_NONETWORKS))
const char _MESSAGE_RESET[] PROGMEM = "Module will reset in a few seconds.";
#define MESSAGE_RESET (FPSTR(_MESSAGE_RESET))
const char _MESSAGE_NOTFOUND[] PROGMEM = "File Not Found";
#define MESSAGE_NOTFOUND (FPSTR(_MESSAGE_NOTFOUND))
const char _MESSAGE_NOTFOUND_URI[] PROGMEM = "URI";
#define MESSAGE_NOTFOUND_URI (FPSTR(_MESSAGE_NOTFOUND_URI))
const char _MESSAGE_NOTFOUND_METHOD[] PROGMEM = "Method";
#define MESSAGE_NOTFOUND_METHOD (FPSTR(_MESSAGE_NOTFOUND_METHOD))
const char _MESSAGE_NOTFOUND_ARGS[] PROGMEM = "Arguments";
#define MESSAGE_NOTFOUND_ARGS (FPSTR(_MESSAGE_NOTFOUND_ARGS))
const char _OPTION_STATICIP[] PROGMEM = "Static IP";
#define OPTION_STATICIP (FPSTR(_OPTION_STATICIP))
const char _OPTION_STATICGW[] PROGMEM = "Static Gateway";
#define OPTION_STATICGW (FPSTR(_OPTION_STATICGW))
const char _OPTION_STATICSN[] PROGMEM = "Subnet";
#define OPTION_STATICSN (FPSTR(_OPTION_STATICSN))
#endif