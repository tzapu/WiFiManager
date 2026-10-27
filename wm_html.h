/**
 * wm_html.h
 * language independent html/css/js skeleton for
 * WiFiManager, a library for the ESP32 platform
 * for configuration of WiFi credentials using a Captive Portal
 *
 * This file is the single source of truth for all markup, styling and script
 * that does not need translating. Language files (wm_strings_xx.h) must only
 * contain words, and build their markup through the WM_HTML_* macros below.
 *
 * Everything here is compile time string literal concatenation, so there is no
 * runtime cost compared to the old per-language duplicated constants.
 *
 * Overrides:
 *   WM_CSS_CUSTOM  - define to supply your own HTTP_STYLE (see below)
 *   WM_NOHELP      - define to compile out the help table
 *
 * @author Creator tzapu
 * @author tablatronix
 * @version 0.0.0
 * @license MIT
 */

#ifndef _WM_HTML_H_
#define _WM_HTML_H_

// ---------------------------------------------------------------------------
// 100% language independent constants
// ---------------------------------------------------------------------------

const char HTTP_SCRIPT[]           PROGMEM = "<script>function c(l){"
"document.getElementById('s').value=l.getAttribute('data-ssid')||l.innerText||l.textContent;"
"p = l.nextElementSibling.classList.contains('l');"
"document.getElementById('p').disabled = !p;"
"if(p)document.getElementById('p').focus();};"
"function f() {var x = document.getElementById('p');x.type==='password'?x.type='text':x.type='password';}"
"</script>"; // @todo add button states, disable on click , show ack , spinner etc

const char HTTP_HEAD_END[]         PROGMEM = "</head><body class='{c}'><div class='wrap'>"; // {c} = _bodyclass
const char HTTP_ROOT_MAIN[]        PROGMEM = "<h1>{t}</h1><h3>{v}</h3>";
const char HTTP_END[]              PROGMEM = "</div></body></html>";
const char HTTP_BR[]               PROGMEM = "<br/>";

// dead weight kept for api compatibility, was never populated
const char HTTP_PORTAL_OPTIONS[]   PROGMEM = "";

// wifi scan list
const char HTTP_ITEM_QI[]          PROGMEM = "<div role='img' aria-label='{r}%' title='{r}%' class='q q-{q} {i} {h}'></div>"; // rssi icons
const char HTTP_ITEM_QP[]          PROGMEM = "<div class='q {h}'>{r}%</div>"; // rssi percentage {h} = hidden showperc pref
const char HTTP_ITEM[]             PROGMEM = "<div class='ap'><a href='#p' onclick='c(this)' data-ssid='{V}'>{v}</a>{qi}{qp}</div>"; // {q} = HTTP_ITEM_QI, {r} = HTTP_ITEM_QP

// forms
const char HTTP_FORM_START[]       PROGMEM = "<form method='POST' action='{v}'>";
const char HTTP_FORM_WIFI_END[]    PROGMEM = "";
const char HTTP_FORM_STATIC_HEAD[] PROGMEM = "<hr><br/>";
const char HTTP_FORM_LABEL[]       PROGMEM = "<label for='{i}'>{t}</label>";
const char HTTP_FORM_PARAM_HEAD[]  PROGMEM = "<hr><br/>";
const char HTTP_FORM_PARAM[]       PROGMEM = "<br/><input id='{i}' name='{n}' maxlength='{l}' value='{v}' {c}>\n"; // do not remove newline!

// info page
const char HTTP_INFO_esphead[]    PROGMEM = "<h3>esp32</h3><hr><dl>";
const char HTTP_INFO_wifihead[]   PROGMEM = "<br/><h3>WiFi</h3><hr>";
const char HTTP_INFO_memsmeter[]  PROGMEM = "<br/><progress value='{1}' max='{2}'></progress></dd>";

// ---------------------------------------------------------------------------
// markup templates, language files pass only the words
// ---------------------------------------------------------------------------

// <head> ... {v} is replaced with the page title
#define WM_HTML_HEAD_START(lang) \
  "<!DOCTYPE html><html lang='" lang "'><head>" \
  "<meta name='format-detection' content='telephone=no'>" \
  "<meta charset='UTF-8'>" \
  "<meta name='viewport' content='width=device-width,initial-scale=1,user-scalable=no'>" \
  "<meta name='theme-color' content='#1a73e8'>" \
  "<title>{v}</title>"

// portal menu entries, route is a literal like "/wifi"
#define WM_HTML_MENU_ITEM(route, label) \
  "<form action='" route "' method='get'><button>" label "</button></form>\n"
#define WM_HTML_MENU_ITEM_S(route, label) \
  "<form action='" route "' method='get'><button class='S'>" label "</button></form>\n"
#define WM_HTML_MENU_ITEM_D(route, label) \
  "<form action='" route "' method='get'><button class='D'>" label "</button></form>\n"

// wifi credentials form, {v}/{p} are ssid/password placeholders
#define WM_HTML_FORM_WIFI(lab_pass, lab_show) \
  "<label for='s'>SSID</label>" \
  "<input id='s' name='s' maxlength='32' autocomplete='off' autocorrect='off' autocapitalize='none' placeholder='{v}'>" \
  "<label for='p'>" lab_pass "</label>" \
  "<input id='p' name='p' maxlength='64' type='password' autocomplete='current-password' placeholder='{p}'>" \
  "<div class='chk'><input type='checkbox' id='showpass' onclick='f()'>" \
  "<label for='showpass'>" lab_show "</label></div>"

#define WM_HTML_FORM_END(lab_save) \
  "<button type='submit'>" lab_save "</button></form>"

#define WM_HTML_SCAN_LINK(lab_refresh) \
  "<form action='/wifi?refresh=1' method='POST'><button class='S' name='refresh' value='1'>" lab_refresh "</button></form>"

#define WM_HTML_SAVED(text)      "<div class='msg'>" text "</div>"
#define WM_HTML_PARAMSAVED(text) "<div class='msg S'>" text "</div>"
#define WM_HTML_STATUS_NONE(text) "<div class='msg'>" text "</div>"

#define WM_HTML_ERASEBTN(label)  "<form action='/erase' method='get'><button class='D'>" label "</button></form>"
#define WM_HTML_UPDATEBTN(label) "<form action='/update' method='get'><button class='S'>" label "</button></form>"
#define WM_HTML_BACKBTN(label)   "<hr><form action='/' method='get'><button class='S'>" label "</button></form>"

// {c=class} {v=ssid} {r=status_off}
#define WM_HTML_STATUS_ON(conn, to, withip) \
  "<div class='msg S'><strong>" conn "</strong> " to " {v}<br/><em><small>" withip " {i}</small></em></div>"
#define WM_HTML_STATUS_OFF(notconn, to) \
  "<div class='msg {c}'><strong>" notconn "</strong> " to " {v}{r}</div>"

// ota update page
#define WM_HTML_UPDATE(text, btn, note) \
  text \
  "<form method='POST' action='u' enctype='multipart/form-data' onchange=\"(function(el){document.getElementById('uploadbin').style.display = el.value=='' ? 'none' : 'initial';})(this)\">" \
  "<input type='file' name='update' accept='.bin,application/octet-stream'>" \
  "<button id='uploadbin' type='submit' class='h D'>" btn "</button></form>" \
  "<small><a href='http://192.168.4.1/update' target='_blank'>" note "</a></small>"

#define WM_HTML_UPDATE_FAIL(title, text) \
  "<div class='msg D'><strong>" title "</strong><br/>" text "</div>"
#define WM_HTML_UPDATE_SUCCESS(title, text) \
  "<div class='msg S'><strong>" title "</strong><br/>" text "</div>"

// info page rows
#define WM_HTML_INFO(label)          "<dt>" label "</dt><dd>{1}</dd>"
#define WM_HTML_INFO_BYTES(label)    "<dt>" label "</dt><dd>{1} bytes</dd>"
#define WM_HTML_INFO_BYTES_AVAIL(label) "<dt>" label "</dt><dd>{1} bytes available</dd>"
#define WM_HTML_INFO_MHZ(label)      "<dt>" label "</dt><dd>{1}MHz</dd>"
#define WM_HTML_INFO_UPTIME(label, mins, secs) "<dt>" label "</dt><dd>{1} " mins " {2} " secs "</dd>"
#define WM_HTML_INFO_MEMSKETCH(label, sub) "<dt>" label "</dt><dd>" sub "<br/>{1} / {2}"
#define WM_HTML_INFO_TEMP(label)     "<dt>" label "</dt><dd>{1} C&deg; / {2} F&deg;</dd>"
#define WM_HTML_INFO_LASTRESET(label) "<dt>" label "</dt><dd>CPU0: {1}<br/>CPU1: {2}</dd>"

// help table, 13 words, markup lives here only
#define WM_HTML_HELP(title, th_page, th_func, d_root, d_wifi, d_wifisave, d_param, d_info, d_u, d_close, d_exit, d_restart, d_erase) \
  "<h3>" title "</h3><hr>" \
  "<table class='table'><thead><tr><th>" th_page "</th><th>" th_func "</th></tr></thead><tbody>" \
  "<tr><td><a href='/'>/</a></td><td>" d_root "</td></tr>" \
  "<tr><td><a href='/wifi'>/wifi</a></td><td>" d_wifi "</td></tr>" \
  "<tr><td><a href='/wifisave'>/wifisave</a></td><td>" d_wifisave "</td></tr>" \
  "<tr><td><a href='/param'>/param</a></td><td>" d_param "</td></tr>" \
  "<tr><td><a href='/info'>/info</a></td><td>" d_info "</td></tr>" \
  "<tr><td><a href='/u'>/u</a></td><td>" d_u "</td></tr>" \
  "<tr><td><a href='/close'>/close</a></td><td>" d_close "</td></tr>" \
  "<tr><td>/exit</td><td>" d_exit "</td></tr>" \
  "<tr><td>/restart</td><td>" d_restart "</td></tr>" \
  "<tr><td>/erase</td><td>" d_erase "</td></tr>" \
  "</tbody></table>" \
  "<p/>Github <a href='https://github.com/mrcodetastic/WiFiManager_ESP32'>https://github.com/mrcodetastic/WiFiManager_ESP32</a>."

// ---------------------------------------------------------------------------
// stylesheet
// define WM_CSS_CUSTOM before including this file to provide your own
// HTTP_STYLE, or use WiFiManager::setCustomHeadElement() to append overrides
// ---------------------------------------------------------------------------
#ifndef WM_CSS_CUSTOM
const char HTTP_STYLE[]            PROGMEM = "<style>"
// theme tokens
":root{--bg:#f4f6f9;--card:#fff;--fg:#101418;--mut:#5b6472;--bd:#dde2ea;--pri:#1a73e8;--pri2:#1558b0;--ok:#1e7f3f;--err:#c62828;--r:10px}"
"*,*::before,*::after{box-sizing:border-box}"
"body{margin:0;padding:16px;background:var(--bg);color:var(--fg);"
"font:16px/1.5 system-ui,-apple-system,'Segoe UI',Roboto,Helvetica,Arial,sans-serif;-webkit-text-size-adjust:100%}"
".wrap{max-width:480px;margin:0 auto;padding:20px;background:var(--card);border:1px solid var(--bd);border-radius:16px;"
"box-shadow:0 1px 2px rgba(16,20,24,.06),0 8px 24px rgba(16,20,24,.06)}"
"h1{margin:0 0 4px;font-size:1.3rem;line-height:1.25;text-align:center}"
"h3{margin:0 0 18px;font-size:.9rem;font-weight:500;color:var(--mut);text-align:center}"
"hr{border:0;border-top:1px solid var(--bd);margin:18px 0}"
"a{color:var(--pri);font-weight:600;text-decoration:none}a:hover{text-decoration:underline}"
// fields
"label{display:block;margin:14px 0 6px;font-size:.78rem;font-weight:700;letter-spacing:.04em;text-transform:uppercase;color:var(--mut)}"
"input,select{display:block;width:100%;min-height:44px;padding:10px 12px;font:inherit;color:inherit;background:var(--card);border:1px solid var(--bd);border-radius:var(--r)}"
"input:focus,select:focus{outline:0;border-color:var(--pri);box-shadow:0 0 0 3px rgba(26,115,232,.2)}"
"input[type=checkbox],input[type=radio]{display:inline-block;width:20px;height:20px;min-height:0;margin:0 10px 0 0;padding:0;accent-color:var(--pri);vertical-align:text-bottom}"
".chk{margin:14px 0}.chk label{display:inline-block;margin:0;font-size:.9rem;font-weight:500;letter-spacing:0;text-transform:none;color:var(--fg)}"
"input[type=file]{padding:12px;border:1px dashed var(--bd);background:var(--bg)}"
// buttons
"button{cursor:pointer;width:100%;min-height:44px;padding:10px 16px;font:inherit;font-weight:600;color:#fff;border:0;border-radius:var(--r);"
"background:var(--pri);transition:background .15s ease,transform .06s ease,box-shadow .15s ease}"
"button:hover{background:var(--pri2)}button:active{transform:scale(.99)}button:focus-visible{box-shadow:0 0 0 3px rgba(26,115,232,.35)}"
"button.S{background:transparent;color:var(--pri);border:1px solid var(--bd)}button.S:hover{background:rgba(26,115,232,.08)}"
"button.D{background:var(--err)}button.D:hover{background:#9e1f1f}"
"form{margin:0 0 12px}form button[type=submit]{margin-top:18px}"
// access point rows
".ap{display:flex;align-items:center;gap:12px;padding:12px 2px;border-bottom:1px solid var(--bd)}"
".ap:last-child{border-bottom:0}.ap:hover a{color:var(--pri)}"
".ap a{flex:1;min-width:0;overflow:hidden;text-overflow:ellipsis;white-space:nowrap;color:var(--fg);font-weight:600}"
".q{float:none;flex:0 0 auto;display:flex;align-items:center;justify-content:flex-end;margin:0;padding:0 0 0 6px;height:16px;min-width:38px;text-align:right}"
".ql .q{float:none}"
// quality icons
".q.q-0:after{background-position-x:0}.q.q-1:after{background-position-x:-16px}.q.q-2:after{background-position-x:-32px}.q.q-3:after{background-position-x:-48px}.q.q-4:after{background-position-x:-64px}.q.l:before{background-position-x:-80px;padding-right:5px}"
".q:after,.q:before{content:'';width:16px;height:16px;display:inline-block;background-repeat:no-repeat;background-position:16px 0;"
"background-image:url('data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAGAAAAAQCAMAAADeZIrLAAAAJFBMVEX///8AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAADHJj5lAAAAC3RSTlMAIjN3iJmqu8zd7vF8pzcAAABsSURBVHja7Y1BCsAwCASNSVo3/v+/BUEiXnIoXkoX5jAQMxTHzK9cVSnvDxwD8bFx8PhZ9q8FmghXBhqA1faxk92PsxvRc2CCCFdhQCbRkLoAQ3q/wWUBqG35ZxtVzW4Ed6LngPyBU2CobdIDQ5oPWI5nCUwAAAAASUVORK5CYII=')}"
// icons @2x media query (32px rescaled)
"@media (-webkit-min-device-pixel-ratio:2),(min-resolution:192dpi){.q:before,.q:after{"
"background-image:url('data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAALwAAAAgCAMAAACfM+KhAAAALVBMVEX///8AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAADAOrOgAAAADnRSTlMAESIzRGZ3iJmqu8zd7gKjCLQAAACmSURBVHgB7dDBCoMwEEXRmKlVY3L//3NLhyzqIqSUggy8uxnhCR5Mo8xLt+14aZ7wwgsvvPA/ofv9+44334UXXngvb6XsFhO/VoC2RsSv9J7x8BnYLW+AjT56ud/uePMdb7IP8Bsc/e7h8Cfk912ghsNXWPpDC4hvN+D1560A1QPORyh84VKLjjdvfPFm++i9EWq0348XXnjhhT+4dIbCW+WjZim9AKk4UZMnnCEuAAAAAElFTkSuQmCC');"
"background-size:95px 16px}}"
// msg callouts
".msg{padding:14px 16px;margin:16px 0;border-radius:var(--r);font-size:.95rem;background:rgba(26,115,232,.08);border-left:4px solid var(--pri)}"
".msg strong{display:block;margin-bottom:2px}"
".msg.S{background:rgba(30,127,63,.10);border-left-color:var(--ok)}"
".msg.D{background:rgba(198,40,40,.10);border-left-color:var(--err)}"
".msg.P{background:rgba(26,115,232,.08);border-left-color:var(--pri)}"
// info lists and tables
"dl{margin:0}dt{margin-top:12px;font-size:.75rem;font-weight:700;letter-spacing:.04em;text-transform:uppercase;color:var(--mut)}"
"dd{margin:2px 0 0;word-break:break-word}"
"table{width:100%;border-collapse:collapse;font-size:.92rem}"
"th,td{padding:8px 6px;text-align:left;border-bottom:1px solid var(--bd);vertical-align:top}"
"th{font-size:.75rem;text-transform:uppercase;letter-spacing:.04em;color:var(--mut)}"
"progress{width:100%;height:10px;border:0;border-radius:99px;background:#e7ebf1;overflow:hidden}"
"progress::-webkit-progress-bar{background:#e7ebf1}progress::-webkit-progress-value{background:var(--pri);border-radius:99px}"
"progress::-moz-progress-bar{background:var(--pri)}"
".h{display:none}:disabled{opacity:.5;cursor:not-allowed}"
// dark theme, explicit via setDarkMode() and opt in automatic via body.auto
"body.invert{--bg:#0f1216;--card:#181d24;--fg:#e7ebf0;--mut:#98a2b0;--bd:#2a313a;--pri:#4c9bf5;--pri2:#3b86dd}"
"body.invert input,body.invert select{background:#10141a}body.invert .msg{background:#20262f}"
"body.invert progress,body.invert progress::-webkit-progress-bar{background:#2a313a}"
"body.invert .q[role=img]{-webkit-filter:invert(1);filter:invert(1)}"
"@media (prefers-color-scheme:dark){"
"body.auto{--bg:#0f1216;--card:#181d24;--fg:#e7ebf0;--mut:#98a2b0;--bd:#2a313a;--pri:#4c9bf5;--pri2:#3b86dd}"
"body.auto input,body.auto select{background:#10141a}body.auto .msg{background:#20262f}}"
"</style>";
#endif // WM_CSS_CUSTOM

#endif // _WM_HTML_H_
