/**
 * wm_strings_multiwifi.h
 * strings for multi wifi (build flag WM_MULTIWIFI)
 * define WM_MULTIWIFI_OVERRIDE_STRINGS and provide your own to translate
 *
 * @license MIT
 */

#ifndef _WM_STRINGS_MULTIWIFI_H_
#define _WM_STRINGS_MULTIWIFI_H_

#ifndef WM_MULTIWIFI_OVERRIDE_STRINGS

const char R_wifidel[]            PROGMEM = "/wifidel";

#ifndef WM_MULTIWIFI_NOUI
// {n} count {m} max {l} rows
const char HTTP_MW_LIST[]         PROGMEM = "<h3>Saved networks ({n}/{m})</h3><div class='mw'>{l}</div><hr><br/>";
// {v} ssid html, {V} ssid attr, {t} tags
const char HTTP_MW_ITEM[]         PROGMEM = "<form method='POST' action='/wifidel'><a href='#p' onclick='c(this)' data-ssid='{V}'>{v}</a><span class='l'></span> {t}<button name='s' value='{V}' class='D' title='Forget'>&#x2715;</button></form>";
const char HTTP_MW_TAG_HIDDEN[]   PROGMEM = "<small>hidden</small> ";
const char HTTP_MW_TAG_UNVER[]    PROGMEM = "<small title='never connected'>?</small> ";
const char HTTP_MW_TAG_FULL[]     PROGMEM = "<div class='msg D'>List is full, forget a network to add another</div>";
const char HTTP_MW_HIDDEN[]       PROGMEM = "<input type='checkbox' id='h' name='h' value='1'> <label for='h'>Hidden network</label><br/>";
const char HTTP_MW_STYLE[]        PROGMEM = "<style>.mw form{margin:0;padding:3px 0}.mw button{width:auto;float:right;line-height:1.4rem;font-size:1rem;padding:0 10px}</style>";
#endif

const char HTTP_MW_FULL[]         PROGMEM = "<div class='msg D'><strong>Not saved</strong><br/>Saved network list is full, forget a network first</div>";

#endif

#endif
