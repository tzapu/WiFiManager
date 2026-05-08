/**
 * wm_strings_en.h
 * engligh strings for
 * WiFiManager, a library for the ESP8266/Arduino platform
 * for configuration of WiFi credentials using a Captive Portal
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

// strings files must include a consts file!
#include "wm_consts_en.h" // include constants, tokens, routes

const char WM_LANGUAGE[] PROGMEM = "en-US"; // i18n lang code

const char HTTP_HEAD_START[]       PROGMEM = "<!DOCTYPE html>"
"<html lang='en'><head>"
"<meta name='format-detection' content='telephone=no'>"
"<meta charset='UTF-8'>"
"<meta  name='viewport' content='width=device-width,initial-scale=1,user-scalable=no'/>"
"<title>{v}</title>";

const char HTTP_SCRIPT[]           PROGMEM = "<script>function c(l){"
"document.getElementById('s').value=l.getAttribute('data-ssid')||l.innerText||l.textContent;"
"p = l.nextElementSibling.classList.contains('l');"
"document.getElementById('p').disabled = !p;"
"if(p)document.getElementById('p').focus();};"
"function f() {var x=document.getElementById('p');var b=document.querySelector('.pw-btn');var m=x.classList.toggle('pw-masked');b.setAttribute('aria-label',m?'Show password':'Hide password');}"
"</script>"; // @todo add button states, disable on click , show ack , spinner etc


const char HTTP_HEAD_END[]         PROGMEM = "</head><body class='{c}'><div class='wrap'>"; // {c} = _bodyclass
// example of embedded logo, base64 encoded inline, No styling here
// const char HTTP_ROOT_MAIN[]        PROGMEM = "<img title=' alt=' src='data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAADAAAAAwCAYAAABXAvmHAAADQElEQVRoQ+2YjW0VQQyE7Q6gAkgFkAogFUAqgFQAVACpAKiAUAFQAaECQgWECggVGH1PPrRvn3dv9/YkFOksoUhhfzwz9ngvKrc89JbnLxuA/63gpsCmwCADWwkNEji8fVNgotDM7osI/x777x5l9F6JyB8R4eeVql4P0y8yNsjM7KGIPBORp558T04A+CwiH1UVUItiUQmZ2XMReSEiAFgjAPBeVS96D+sCYGaUx4cFbLfmhSpnqnrZuqEJgJnd8cQplVLciAgX//Cf0ToIeOB9wpmloLQAwpnVmAXgdf6pwjpJIz+XNoeZQQZlODV9vhc1Tuf6owrAk/8qIhFbJH7eI3eEzsvydQEICqBEkZwiALfF70HyHPpqScPV5HFjeFu476SkRA0AzOfy4hYwstj2ZkDgaphE7m6XqnoS7Q0BOPs/sw0kDROzjdXcCMFCNwzIy0EcRcOvBACfh4k0wgOmBX4xjfmk4DKTS31hgNWIKBCI8gdzogTgjYjQWFMw+o9LzJoZ63GUmjWm2wGDc7EvDDOj/1IVMIyD9SUAL0WEhpriRlXv5je5S+U1i2N88zdPuoVkeB+ls4SyxCoP3kVm9jsjpEsBLoOBNC5U9SwpGdakFkviuFP1keblATkTENTYcxkzgxTKOI3jyDxqLkQT87pMA++H3XvJBYtsNbBN6vuXq5S737WqHkW1VgMQNXJ0RshMqbbT33sJ5kpHWymzcJjNTeJIymJZtSQd9NHQHS1vodoFoTMkfbJzpRnLzB2vi6BZAJxWaCr+62BC+jzAxVJb3dmmiLzLwZhZNPE5e880Suo2AZgB8e8idxherqUPnT3brBDTlPxO3Z66rVwIwySXugdNd+5ejhqp/+NmgIwGX3Py3QBmlEi54KlwmjkOytQ+iJrLJj23S4GkOeecg8G091no737qvRRdzE+HLALQoMTBbJgBsCj5RSWUlUVJiZ4SOljb05eLFWgoJ5oY6yTyJp62D39jDANoKKcSocPJD5dQYzlFAFZJflUArgTPZKZwLXAnHmerfJquUkKZEgyzqOb5TuDt1P3nwxobqwPocZA11m4A1mBx5IxNgRH21ti7KbAGiyNn3HoF/gJ0w05A8xclpwAAAABJRU5ErkJggg==' /><h1>{v}</h1><h3>WiFiManager</h3>";
const char HTTP_ROOT_MAIN[]        PROGMEM = "<h1>{t}</h1><h3>{v}</h3>";

const char * const HTTP_PORTAL_MENU[] PROGMEM = {
"<form action='/wifi'    method='get'><button onclick=\"this.textContent='Scanning\u2026'\">Configure WiFi</button></form><br/>\n", // MENU_WIFI
"<form action='/0wifi'   method='get'><button>Configure WiFi (No scan)</button></form><br/>\n", // MENU_WIFINOSCAN
"<form action='/info'    method='get'><button>Info</button></form><br/>\n", // MENU_INFO
"<form action='/param'   method='get'><button>Setup</button></form><br/>\n",//MENU_PARAM
"<form action='/close'   method='get'><button>Close</button></form><br/>\n", // MENU_CLOSE
"<form action='/restart' method='get'><button>Restart</button></form><br/>\n",// MENU_RESTART
"<form action='/exit'    method='get'><button>Exit</button></form><br/>\n",  // MENU_EXIT
"<form action='/erase'   method='get'><button class='D'>Erase</button></form><br/>\n", // MENU_ERASE
"<form action='/update'  method='get'><button>Update</button></form><br/>\n",// MENU_UPDATE
"<hr><br/>" // MENU_SEP
};

// const char HTTP_PORTAL_OPTIONS[]   PROGMEM = strcat(HTTP_PORTAL_MENU[0] , HTTP_PORTAL_MENU[3] , HTTP_PORTAL_MENU[7]);
const char HTTP_PORTAL_OPTIONS[]   PROGMEM = "";
const char HTTP_ITEM_QI[]          PROGMEM = "<div role='img' aria-label='{r}%' title='{r}%' class='q q-{q} {i} {h}'></div>"; // rssi icons
const char HTTP_ITEM_QP[]          PROGMEM = "<div class='q {h}'>{r}%</div>"; // rssi percentage {h} = hidden showperc pref
const char HTTP_ITEM[]             PROGMEM = "<div><a href='#p' onclick='c(this)' data-ssid='{V}' class='wi'>{v}</a>{qi}{qp}</div>"; // {q} = HTTP_ITEM_QI, {r} = HTTP_ITEM_QP
// const char HTTP_ITEM[]            PROGMEM = "<div><a href='#p' onclick='c(this)'>{v}</a> {R} {r}% {q} {e}</div>"; // test all tokens

const char HTTP_FORM_START[]       PROGMEM = "<form method='POST' action='{v}'>";
const char HTTP_FORM_WIFI[]        PROGMEM = "<label for='s'>Network Name</label><input id='s' name='s' maxlength='32' autocorrect='off' autocapitalize='none' value='{v}' pattern='^[^!#;+\\/\\[\\]\"\\s].{0,31}$' title='Network name (1-32 chars, first character cannot be a space or !#;+/[]\")'><br/><label for='p'>Password</label><div class='pw-wrap'><input id='p' name='p' maxlength='64' type='text' class='pw-masked' value='{p}' autocomplete='off' pattern='^.{8,63}$' title='WiFi password must be between 8 and 63 characters (leave empty for open networks).'><button type='button' class='pw-btn' onclick='f()' aria-label='Show password'><svg xmlns='http://www.w3.org/2000/svg' viewBox='0 0 24 24' width='18' height='18' fill='currentColor'><path d='M12 4.5C7 4.5 2.73 7.61 1 12c1.73 4.39 6 7.5 11 7.5s9.27-3.11 11-7.5c-1.73-4.39-6-7.5-11-7.5zM12 17c-2.76 0-5-2.24-5-5s2.24-5 5-5 5 2.24 5 5-2.24 5-5 5zm0-8c-1.66 0-3 1.34-3 3s1.34 3 3 3 3-1.34 3-3-1.34-3-3-3z'/></svg></button></div><br/>";
const char HTTP_FORM_WIFI_END[]    PROGMEM = "";
const char HTTP_FORM_STATIC_HEAD[] PROGMEM = "<hr><br/>";
const char HTTP_FORM_END[]         PROGMEM = "<br/><button type='submit' class='btn-cta'>Save</button></form>";
const char HTTP_FORM_LABEL[]       PROGMEM = "<label for='{i}'>{t}</label>";
const char HTTP_FORM_PARAM_HEAD[]  PROGMEM = "<hr><br/>";
const char HTTP_FORM_PARAM[]       PROGMEM = "<br/><input id='{i}' name='{n}' maxlength='{l}' value='{v}' {c}>\n"; // do not remove newline!

const char HTTP_SCAN_LINK[]        PROGMEM = "<form class='rf' action='/wifi?refresh=1' method='POST'><button type='submit' class='rib' name='refresh' value='1' aria-label='Refresh networks' title='Refresh'><svg xmlns='http://www.w3.org/2000/svg' viewBox='0 0 24 24' width='18' height='18' fill='currentColor'><path d='M17.65 6.35C16.2 4.9 14.21 4 12 4c-4.42 0-7.99 3.58-7.99 8s3.57 8 7.99 8c3.73 0 6.84-2.55 7.73-6h-2.08c-.82 2.33-3.04 4-5.65 4-3.31 0-6-2.69-6-6s2.69-6 6-6c1.66 0 3.14.69 4.22 1.78L13 11h7V4l-2.35 2.35z'/></svg></button></form>";
const char HTTP_SAVED[]            PROGMEM = "<div class='msg'>Connecting to WiFi network...<br/>If the connection fails, come back to this page to try again.</div>";
const char HTTP_PARAMSAVED[]       PROGMEM = "<div class='msg S'>Saved<br/></div>";
// Provisioning mode save page: stays open and polls /status with JavaScript
// {svgC} = custom connecting SVG (replaced server-side), {svgS} = success SVG, {svgF} = failure SVG
const char HTTP_SAVED_PROVISIONING[] PROGMEM =
  "<div id='wm-svg-c' style='display:none;text-align:center;margin:8px 0'>{svgC}</div>"
  "<div id='wm-svg-s' style='display:none;text-align:center;margin:8px 0'>{svgS}</div>"
  "<div id='wm-svg-f' style='display:none;text-align:center;margin:8px 0'>{svgF}</div>"
  "<div class='msg' id='wm-prov-msg'><span class='sp'></span>&nbsp;Connecting&hellip;<br/><small id='wm-prov-status'>Please wait</small></div>"
  "<div id='wm-prov-btns'></div>"
  "<script>"
  "function wmSvg(id){"
    "['wm-svg-c','wm-svg-s','wm-svg-f'].forEach(function(i){"
      "var e=document.getElementById(i);if(e)e.style.display='none';"
    "});"
    "if(id){var e=document.getElementById(id);if(e&&e.innerHTML.trim()!='')e.style.display='';}"
  "}"
  "function wmPoll(){"
    "fetch('/status').then(function(r){return r.json();}).then(function(d){"
      "var m=document.getElementById('wm-prov-msg');"
      "var b=document.getElementById('wm-prov-btns');"
      "if(d.state==='connected'){"
        "wmSvg('wm-svg-s');"
        "m.className='msg S';"
        "m.innerHTML='<strong>Connected!</strong><br/><small>Network: <b>'+d.ssid+'</b><br/>IP: '+d.ip+(d.hostname?'&nbsp;&nbsp;'+d.hostname:'')+'</small>';"
        "var shut=d.apShutdownIn?'<br/><small>Setup mode closes in '+Math.ceil(d.apShutdownIn/1000)+'s</small>':'';"
        "m.innerHTML+=shut;"
        "b.innerHTML='<br/><form action=\"/exit\" method=\"get\"><button type=\"submit\">Close setup</button></form>';"
      "}else if(d.state==='failed'){"
        "wmSvg('wm-svg-f');"
        "var lr=d.lastResult||'';"
        "var title=lr==='wrong_password'?'Wrong password':lr==='not_found'?'Network not found':'Could not connect';"
        "var sub=lr==='wrong_password'?'Please re-enter the password for <b>'+d.ssid+'</b> and try again.':lr==='not_found'?'<b>'+d.ssid+'</b> was not found. Move closer and try again.':'Could not reach <b>'+d.ssid+'</b>. Check your settings and try again.';"
        "m.className='msg D';"
        "m.innerHTML='<strong>'+title+'</strong><br/><small>'+sub+'</small>';"
        "b.innerHTML='<br/><form action=\"/wifi\" method=\"get\"><button type=\"submit\">Change settings</button></form>';"
      "}else if(d.state==='connecting'){"
        "wmSvg('wm-svg-c');"
        "m.className='msg';"
        "m.innerHTML='<span class=\"sp\"></span>&nbsp;Connecting to <b>'+d.ssid+'</b>&hellip;<br/><small id=\"wm-prov-status\">'+new Date().toLocaleTimeString()+'</small>';"
        "b.innerHTML='';"
        "setTimeout(wmPoll,1500);"
      "}else{"
        "var s=document.getElementById('wm-prov-status');"
        "if(s)s.textContent=d.state;"
        "setTimeout(wmPoll,2000);"
      "}"
    "}).catch(function(){setTimeout(wmPoll,3000);});"
  "}"
  "wmSvg('wm-svg-c');"
  "setTimeout(wmPoll,800);"
  "</script>";
const char HTTP_END[]              PROGMEM = "</div></body></html>";
const char HTTP_ERASEBTN[]         PROGMEM = "<br/><form action='/erase' method='get'><button class='D'>Erase WiFi config</button></form>";
const char HTTP_UPDATEBTN[]        PROGMEM = "<br/><form action='/update' method='get'><button>Update</button></form>";
const char HTTP_BACKBTN[]          PROGMEM = "<hr><br/><form action='/' method='get'><button>Back</button></form>";

// Bottom navigation bar shown on WiFi-setup and info pages
const char HTTP_NAV_BOTTOM[]       PROGMEM =
  "<div class='nav'>"
    "<a href='/wifi'>"
      "<svg xmlns='http://www.w3.org/2000/svg' viewBox='0 0 24 24' width='22' height='22' fill='currentColor'><path d='M1 9l2 2c5.1-5.1 13.3-5.1 18.4 0l2-2C16.5 2.3 7.3 2.3 1 9zm8 8l3 3 3-3c-1.7-1.6-4.2-1.6-6 0zm-4-4l2 2c2.8-2.8 7.3-2.8 10.1 0l2-2C15.5 9.5 8.3 9.5 5 13z'/></svg>"
      "<span>WiFi</span>"
    "</a>"
    "<a href='/info'>"
      "<svg xmlns='http://www.w3.org/2000/svg' viewBox='0 0 24 24' width='22' height='22' fill='currentColor'><path d='M12 2C6.48 2 2 6.48 2 12s4.48 10 10 10 10-4.48 10-10S17.52 2 12 2zm1 15h-2v-6h2v6zm0-8h-2V7h2v2z'/></svg>"
      "<span>Info</span>"
    "</a>"
    "<a href='/restart'>"
      "<svg xmlns='http://www.w3.org/2000/svg' viewBox='0 0 24 24' width='22' height='22' fill='currentColor'><path d='M17.65 6.35C16.2 4.9 14.21 4 12 4c-4.42 0-7.99 3.58-7.99 8s3.57 8 7.99 8c3.73 0 6.84-2.55 7.73-6h-2.08c-.82 2.33-3.04 4-5.65 4-3.31 0-6-2.69-6-6s2.69-6 6-6c1.66 0 3.14.69 4.22 1.78L13 11h7V4l-2.35 2.35z'/></svg>"
      "<span>Restart</span>"
    "</a>"
  "</div>";

// Live status polling script – polls /status every 6 s and updates the element
// with id='wm-live-status'.  Shows a spinner immediately and reveals failure
// messages only after several polls so a brief disconnect is not alarming.
const char HTTP_STATUS_LIVE_SCRIPT[] PROGMEM =
  "<script>"
  "(function(){"
    "var c=0;"
    // highlight the password field (wrong-password error feedback)
    "function markPwErr(){var p=document.getElementById('p');if(p)p.classList.add('input-error');}"
    "function g(){"
      "fetch('/status').then(function(r){return r.json();})"
      ".then(function(d){"
        "c++;"
        "var e=document.getElementById('wm-live-status');"
        "if(!e)return;"
        "if(d.wlstatus==='connected'){"
          "var q=d.quality?' ('+d.quality+'%)':'';"
          "e.className='msg S';"
          "e.innerHTML='<strong>Connected</strong> to <b>'+d.ssid+'</b><br/><small>IP\u00a0'+d.ip+q+'</small>';"
        "}else if(c>1&&d.lastResult==='wrong_password'){"
          "e.className='msg D';"
          "e.innerHTML='<strong>Wrong password</strong><br/><small>Check the password for <b>'+d.ssid+'</b> and try again.</small>';"
          "markPwErr();"
        "}else if(c>1&&d.lastResult==='not_found'){"
          "e.className='msg D';"
          "e.innerHTML='<strong>Network not found</strong><br/><small><b>'+d.ssid+'</b> is not in range. Move closer or choose a different network.</small>';"
        "}else if(c>1&&(d.lastResult==='failed'||d.lastResult==='timeout')){"
          "e.className='msg D';"
          "e.innerHTML='<strong>Could not connect</strong><br/><small>Check settings for <b>'+d.ssid+'</b> and try again.</small>';"
        "}else if(d.state==='connecting'){"
          "e.className='msg';"
          "e.innerHTML='<span class=\"sp\"></span>&nbsp;Connecting to <b>'+d.ssid+'</b>\u2026';"
        "}else if(!d.ssid||d.ssid===''){"
          "e.className='msg';"
          "e.innerHTML='No WiFi network configured.';"
          "return;"
        "}else{"
          "e.className='msg';"
          "e.innerHTML='<span class=\"sp\"></span>&nbsp;Checking connection\u2026';"
        "}"
        "setTimeout(g,6000);"
      "})"
      ".catch(function(){setTimeout(g,9000);});"
    "}"
    // If the server-rendered banner already shows a wrong-password error, highlight
    // the password field immediately (before the spinner replaces the static content).
    "var e=document.getElementById('wm-live-status');"
    "if(e&&e.classList.contains('D')&&e.textContent.indexOf('Wrong')!==-1){markPwErr();}"
    // Replace static status with spinner, then start polling
    "if(e){e.className='msg';e.innerHTML='<span class=\"sp\"></span>&nbsp;Checking connection\u2026';}"
    "setTimeout(g,1500);"
  "})();"
  "</script>";

const char HTTP_STATUS_ON[]        PROGMEM = "<div class='msg S' id='wm-live-status'><strong>Connected</strong> to {v}<br/><em><small>IP: {i}</small></em></div>";
const char HTTP_STATUS_OFF[]       PROGMEM = "<div class='msg {c}' id='wm-live-status'><strong>Not connected</strong> to {v}{r}</div>"; // {c=class} {v=ssid} {r=status_off}
const char HTTP_STATUS_OFFPW[]     PROGMEM = "<br/>Wrong password"; // STATION_WRONG_PASSWORD
const char HTTP_STATUS_OFFNOAP[]   PROGMEM = "<br/>Network not found";   // WL_NO_SSID_AVAIL
const char HTTP_STATUS_OFFFAIL[]   PROGMEM = "<br/>Check your settings"; // WL_CONNECT_FAILED
const char HTTP_STATUS_NONE[]      PROGMEM = "<div class='msg' id='wm-live-status'>No WiFi network configured</div>";
const char HTTP_BR[]               PROGMEM = "<br/>";

const char HTTP_STYLE[]            PROGMEM = "<style>"
".c,body,h1,h3{text-align:center;font-family:-apple-system,BlinkMacSystemFont,'Segoe UI',Roboto,Helvetica,Arial,sans-serif}div,input,select{padding:5px;font-size:1em;margin:5px 0;box-sizing:border-box}"
"input,button,select,.msg{border-radius:.3rem;width: 100%}input[type=radio],input[type=checkbox]{width:auto}"
"button,input[type='button'],input[type='submit']{cursor:pointer;border:0;background-color:#1fa3ec;color:#fff;line-height:2.75rem;font-size:1.2rem;width:100%}"
"input[type='file']{border:1px solid #1fa3ec}"
".wrap {text-align:left;display:block;width:100%;max-width:500px;margin:0 auto}"
".footer {position: fixed; text-align: center; bottom: 0; width: 100%}"
// links
"a{color:#000;font-weight:700;text-decoration:none}a:hover{color:#1fa3ec;text-decoration:underline}"
// quality icons
".q{height:16px;margin:0;padding:0 5px;text-align:right;min-width:38px;float:right}.q.q-0:after{background-position-x:0}.q.q-1:after{background-position-x:-16px}.q.q-2:after{background-position-x:-32px}.q.q-3:after{background-position-x:-48px}.q.q-4:after{background-position-x:-64px}.q.l:before{background-position-x:-80px;padding-right:5px}.ql .q{float:left}.q:after,.q:before{content:'';width:16px;height:16px;display:inline-block;background-repeat:no-repeat;background-position: 16px 0;"
"background-image:url('data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAGAAAAAQCAMAAADeZIrLAAAAJFBMVEX///8AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAADHJj5lAAAAC3RSTlMAIjN3iJmqu8zd7vF8pzcAAABsSURBVHja7Y1BCsAwCASNSVo3/v+/BUEiXnIoXkoX5jAQMxTHzK9cVSnvDxwD8bFx8PhZ9q8FmghXBhqA1faxk92PsxvRc2CCCFdhQCbRkLoAQ3q/wWUBqG35ZxtVzW4Ed6LngPyBU2CobdIDQ5oPWI5nCUwAAAAASUVORK5CYII=');}"
// icons @2x media query (32px rescaled)
"@media (-webkit-min-device-pixel-ratio: 2),(min-resolution: 192dpi){.q:before,.q:after {"
"background-image:url('data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAALwAAAAgCAMAAACfM+KhAAAALVBMVEX///8AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAADAOrOgAAAADnRSTlMAESIzRGZ3iJmqu8zd7gKjCLQAAACmSURBVHgB7dDBCoMwEEXRmKlVY3L//3NLhyzqIqSUggy8uxnhCR5Mo8xLt+14aZ7wwgsvvPA/ofv9+44334UXXngvb6XsFhO/VoC2RsSv9J7x8BnYLW+AjT56ud/uePMdb7IP8Bsc/e7h8Cfk912ghsNXWPpDC4hvN+D1560A1QPORyh84VKLjjdvfPFm++i9EWq0348XXnjhhT+4dIbCW+WjZim9AKk4UZMnnCEuAAAAAElFTkSuQmCC');"
"background-size: 95px 16px;}}"
// msg callouts
".msg{padding:20px;margin:20px 0;border:1px solid #eee;border-left-width:5px;border-left-color:#777}.msg h4{margin-top:0;margin-bottom:5px}.msg.P{border-left-color:#1fa3ec}.msg.P h4{color:#1fa3ec}.msg.D{border-left-color:#dc3630}.msg.D h4{color:#dc3630}.msg.S{border-left-color: #5cb85c}.msg.S h4{color: #5cb85c}"
// highlight input when password is wrong
".input-error{border-color:#dc3630!important;box-shadow:0 0 0 3px rgba(220,54,48,.15)!important}"
// lists
"dt{font-weight:bold}dd{margin:0;padding:0 0 0.5em 0;min-height:12px}"
"td{vertical-align: top;}"
".h{display:none}"
"button{transition: 0s opacity;transition-delay: 3s;transition-duration: 0s;cursor: pointer}"
"button.D{background-color:#dc3630}"
"button:active{opacity:50% !important;cursor:wait;transition-delay: 0s}"
// spinner animation
".sp{display:inline-block;width:1em;height:1em;border:3px solid rgba(31,163,236,.3);border-top-color:#1fa3ec;border-radius:50%;animation:spin .8s linear infinite;vertical-align:middle}"
"@keyframes spin{to{transform:rotate(360deg)}}"
// bottom navigation bar
".nav{position:fixed;bottom:0;left:0;right:0;background:#fff;border-top:1px solid #ddd;padding:4px 5px;text-align:center;z-index:100;display:flex;justify-content:space-around}"
".nav a{color:#1fa3ec;margin:0;font-size:.65em;text-decoration:none;font-weight:bold;display:flex;flex-direction:column;align-items:center;padding:2px 12px}"
".nav a svg{display:block;margin:0 auto 2px}"
// add bottom padding to wrap so content is not hidden behind nav
".wrap{padding-bottom:60px}"
// sticky call-to-action button sits above the nav bar
".btn-cta{position:sticky;bottom:58px;z-index:10}"
// password eye-icon wrapper
".pw-wrap{position:relative;padding:0;margin:0}"
".pw-wrap>input{padding-right:40px}"
".pw-btn{position:absolute;right:2px;top:50%;transform:translateY(-50%);background:none;border:none;padding:6px;cursor:pointer;width:36px;line-height:1;color:#888}"
".pw-masked{-webkit-text-security:disc;-moz-text-security:disc;}"
// status + refresh icon flex row
".sh{display:flex;align-items:flex-start;gap:6px}"
".sh>.msg{flex:1;margin:5px 0}"
".rf{flex-shrink:0;margin:5px 0;width:auto}"
".rib{width:36px;height:36px;padding:7px;border-radius:.3rem;line-height:1}"
// scrollable wifi network list
".wl{max-height:40vh;overflow-y:auto;border:1px solid #eee;border-radius:.3rem;margin:5px 0}"
// make SSID anchor fill the whole row (left of quality icons) so the whole area is tappable
".wl>div>a.wi{flex:1;min-width:0;padding:4px 0;overflow:hidden;text-overflow:ellipsis;white-space:nowrap}"
// network list item: padding + hairline separator (like iOS / Premiere Pro)
".wl>div{padding:10px 12px;border-bottom:1px solid #eee;display:flex;align-items:center;justify-content:space-between}"
".wl>div:last-child{border-bottom:none}"
// empty/error message for network list
".nm{color:#888;text-align:center;padding:8px 0;margin:0}"
// invert
"body.invert{background-color:#060606;}"
"body.invert,body.invert a,body.invert h1 {color:#fff;}"
"body.invert .msg{color:#fff;background-color:#282828;border-top:1px solid #555;border-right:1px solid #555;border-bottom:1px solid #555;}"
"body.invert .q[role=img]{-webkit-filter:invert(1);filter:invert(1);}"
"body.invert .nav{background:#121212;border-top-color:#333}"
"body.invert .wl{border-color:#444}"
"body.invert .wl>div{border-bottom-color:#333}"
":disabled {opacity: 0.5;}"
// automatic OS dark mode
"@media(prefers-color-scheme:dark){"
  "body{background-color:#060606;color:#fff}"
  "a,h1{color:#fff}"
  ".msg{color:#fff;background-color:#282828;border-top:1px solid #555;border-right:1px solid #555;border-bottom:1px solid #555}"
  ".q[role=img]{-webkit-filter:invert(1);filter:invert(1)}"
  ".nav{background:#121212;border-top-color:#333}"
  ".wl{border-color:#444}"
  ".wl>div{border-bottom-color:#333}"
  "input,select{background-color:#1a1a1a;color:#fff;border:1px solid #444}"
  ".pw-btn{color:#aaa}"
  ".input-error{box-shadow:0 0 0 3px rgba(220,54,48,.3)!important}"
"}"
"</style>";

#ifndef WM_NOHELP
const char HTTP_HELP[]             PROGMEM =
 "<br/><h3>Available pages</h3><hr>"
 "<table class='table'>"
 "<thead><tr><th>Page</th><th>Function</th></tr></thead><tbody>"
 "<tr><td><a href='/'>/</a></td>"
 "<td>Menu page.</td></tr>"
 "<tr><td><a href='/wifi'>/wifi</a></td>"
 "<td>Show WiFi scan results and enter WiFi configuration.(/0wifi noscan)</td></tr>"
 "<tr><td><a href='/wifisave'>/wifisave</a></td>"
 "<td>Save WiFi configuration information and configure device. Needs variables supplied.</td></tr>"
 "<tr><td><a href='/param'>/param</a></td>"
 "<td>Parameter page</td></tr>"
 "<tr><td><a href='/info'>/info</a></td>"
 "<td>Information page</td></tr>"
 "<tr><td><a href='/u'>/u</a></td>"
 "<td>OTA Update</td></tr>"
 "<tr><td><a href='/close'>/close</a></td>"
 "<td>Close the captiveportal popup, config portal will remain active</td></tr>"
 "<tr><td>/exit</td>"
 "<td>Exit Config portal, config portal will close</td></tr>"
 "<tr><td>/restart</td>"
 "<td>Reboot the device</td></tr>"
 "<tr><td>/erase</td>"
 "<td>Erase WiFi configuration and reboot device. Device will not reconnect to a network until new WiFi configuration data is entered.</td></tr>"
 "</table>"
 "<p/>Github <a href='https://github.com/tzapu/WiFiManager'>https://github.com/tzapu/WiFiManager</a>.";
#else
const char HTTP_HELP[]             PROGMEM = "";
#endif

const char HTTP_UPDATE[] PROGMEM = "Upload new firmware<br/><form method='POST' action='u' enctype='multipart/form-data' onchange=\"(function(el){document.getElementById('uploadbin').style.display = el.value=='' ? 'none' : 'initial';})(this)\"><input type='file' name='update' accept='.bin,application/octet-stream'><button id='uploadbin' type='submit' class='h D'>Update</button></form><small><a href='http://192.168.4.1/update' target='_blank'>* May not function inside captive portal, open in browser http://192.168.4.1</a></small>";
const char HTTP_UPDATE_FAIL[] PROGMEM = "<div class='msg D'><strong>Update failed!</strong><Br/>Reboot device and try again</div>";
const char HTTP_UPDATE_SUCCESS[] PROGMEM = "<div class='msg S'><strong>Update successful.  </strong> <br/> Device rebooting now...</div>";

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

// Info html
// @todo remove html elements from progmem, repetetive strings
#ifdef ESP32
	const char HTTP_INFO_esphead[]    PROGMEM = "<h3>esp32</h3><hr><dl>";
	const char HTTP_INFO_chiprev[]    PROGMEM = "<dt>Chip rev</dt><dd>{1}</dd>";
  	const char HTTP_INFO_lastreset[]  PROGMEM = "<dt>Last reset reason</dt><dd>CPU0: {1}<br/>CPU1: {2}</dd>";
  	const char HTTP_INFO_aphost[]     PROGMEM = "<dt>Access point hostname</dt><dd>{1}</dd>";
    const char HTTP_INFO_psrsize[]    PROGMEM = "<dt>PSRAM Size</dt><dd>{1} bytes</dd>";
	const char HTTP_INFO_temp[]       PROGMEM = "<dt>Temperature</dt><dd>{1} C&deg; / {2} F&deg;</dd>";
    const char HTTP_INFO_hall[]       PROGMEM = "<dt>Hall</dt><dd>{1}</dd>";
#else
	const char HTTP_INFO_esphead[]    PROGMEM = "<h3>esp8266</h3><hr><dl>";
	const char HTTP_INFO_fchipid[]    PROGMEM = "<dt>Flash chip ID</dt><dd>{1}</dd>";
	const char HTTP_INFO_corever[]    PROGMEM = "<dt>Core version</dt><dd>{1}</dd>";
	const char HTTP_INFO_bootver[]    PROGMEM = "<dt>Boot version</dt><dd>{1}</dd>";
	const char HTTP_INFO_lastreset[]  PROGMEM = "<dt>Last reset reason</dt><dd>{1}</dd>";
	const char HTTP_INFO_flashsize[]  PROGMEM = "<dt>Real flash size</dt><dd>{1} bytes</dd>";
#endif

const char HTTP_INFO_memsmeter[]  PROGMEM = "<br/><progress value='{1}' max='{2}'></progress></dd>";
const char HTTP_INFO_memsketch[]  PROGMEM = "<dt>Memory - Sketch size</dt><dd>Used / Total bytes<br/>{1} / {2}";
const char HTTP_INFO_freeheap[]   PROGMEM = "<dt>Memory - Free heap</dt><dd>{1} bytes available</dd>";
const char HTTP_INFO_wifihead[]   PROGMEM = "<br/><h3>WiFi</h3><hr>";
const char HTTP_INFO_uptime[]     PROGMEM = "<dt>Uptime</dt><dd>{1} mins {2} secs</dd>";
const char HTTP_INFO_chipid[]     PROGMEM = "<dt>Chip ID</dt><dd>{1}</dd>";
const char HTTP_INFO_idesize[]    PROGMEM = "<dt>Flash size</dt><dd>{1} bytes</dd>";
const char HTTP_INFO_sdkver[]     PROGMEM = "<dt>SDK version</dt><dd>{1}</dd>";
const char HTTP_INFO_cpufreq[]    PROGMEM = "<dt>CPU frequency</dt><dd>{1}MHz</dd>";
const char HTTP_INFO_apip[]       PROGMEM = "<dt>Access point IP</dt><dd>{1}</dd>";
const char HTTP_INFO_apmac[]      PROGMEM = "<dt>Access point MAC</dt><dd>{1}</dd>";
const char HTTP_INFO_apssid[]     PROGMEM = "<dt>Access point SSID</dt><dd>{1}</dd>";
const char HTTP_INFO_apbssid[]    PROGMEM = "<dt>BSSID</dt><dd>{1}</dd>";
const char HTTP_INFO_stassid[]    PROGMEM = "<dt>Station SSID</dt><dd>{1}</dd>";
const char HTTP_INFO_staip[]      PROGMEM = "<dt>Station IP</dt><dd>{1}</dd>";
const char HTTP_INFO_stagw[]      PROGMEM = "<dt>Station gateway</dt><dd>{1}</dd>";
const char HTTP_INFO_stasub[]     PROGMEM = "<dt>Station subnet</dt><dd>{1}</dd>";
const char HTTP_INFO_dnss[]       PROGMEM = "<dt>DNS Server</dt><dd>{1}</dd>";
const char HTTP_INFO_host[]       PROGMEM = "<dt>Hostname</dt><dd>{1}</dd>";
const char HTTP_INFO_stamac[]     PROGMEM = "<dt>Station MAC</dt><dd>{1}</dd>";
const char HTTP_INFO_conx[]       PROGMEM = "<dt>Connected</dt><dd>{1}</dd>";
const char HTTP_INFO_autoconx[]   PROGMEM = "<dt>Autoconnect</dt><dd>{1}</dd>";

const char HTTP_INFO_aboutver[]     PROGMEM = "<dt>WiFiManager</dt><dd>{1}</dd>";
const char HTTP_INFO_aboutarduino[] PROGMEM = "<dt>Arduino</dt><dd>{1}</dd>";
const char HTTP_INFO_aboutsdk[]     PROGMEM = "<dt>ESP-SDK/IDF</dt><dd>{1}</dd>";
const char HTTP_INFO_aboutdate[]    PROGMEM = "<dt>Build date</dt><dd>{1}</dd>";

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
const char S_titlewifi[]          PROGMEM = "WiFi Setup";
const char S_titleinfo[]          PROGMEM = "Info";
const char S_titleparam[]         PROGMEM = "Setup";
const char S_titleparamsaved[]    PROGMEM = "Setup saved";
const char S_titleexit[]          PROGMEM = "Exit";
const char S_titlereset[]         PROGMEM = "Reset";
const char S_titleerase[]         PROGMEM = "Erase";
const char S_titleclose[]         PROGMEM = "Close";
const char S_options[]            PROGMEM = "options";
const char S_nonetworks[]         PROGMEM = "No networks found. Refresh to scan again.";
const char S_scanfailed[]         PROGMEM = "Scan failed. Tap refresh to try again.";
const char S_staticip[]           PROGMEM = "Static IP";
const char S_staticgw[]           PROGMEM = "Static gateway";
const char S_staticdns[]          PROGMEM = "Static DNS";
const char S_subnet[]             PROGMEM = "Subnet";
const char S_exiting[]            PROGMEM = "<div class='msg' style='text-align:center'><span class='sp'></span>&nbsp;<strong>Closing setup&hellip;</strong><br/><small style='color:#888'>You can close this window</small></div><script>setTimeout(function(){try{window.close();}catch(e){}},600);</script>";
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
#ifdef ESP8266
    const char S_ssidpre[]        PROGMEM = "ESP";
#elif defined(ESP32)
    const char S_ssidpre[]        PROGMEM = "ESP32";
#else
    const char S_ssidpre[]        PROGMEM = "WM";
#endif

// END WIFI_MANAGER_OVERRIDE_STRINGS
#endif

#endif
