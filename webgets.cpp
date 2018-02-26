/**************************************************************
   WiFiManager is a library for the ESP8266/Arduino platform
   (https://github.com/esp8266/Arduino) to enable easy
   configuration and reconfiguration of WiFi credentials using a Captive Portal
   inspired by:
   http://www.esp8266.com/viewtopic.php?f=29&t=2520
   https://github.com/chriscook8/esp-arduino-apboot
   https://github.com/esp8266/Arduino/tree/master/libraries/DNSServer/examples/CaptivePortalAdvanced
   This is an extension to the original build from AlexT
   This extension is provided by Thoma HauC https://github.com/Thoma-HauC
   It is design to offer more flexibility in form entries handle.
   Licensed under MIT license
 **************************************************************/
#include "webgets.h"

#include <list>

// constructor for common ancestor of webgets
web_parameter::web_parameter(const char * id, const char * place_holder) : _id(id), _place_holder(place_holder), _custom_HTML("")
{
}

// constructor for common ancestor of webgets
web_parameter::web_parameter(const char * id, const char * place_holder, const char * custom_HTML) : _id(id), _place_holder(place_holder), _custom_HTML(custom_HTML)
{
}

// method that return ID
const char * web_parameter::get_ID()
{
  return _id;
}

// method that return place holder
const char * web_parameter::get_place_holder()
{
  return _place_holder;
}

// method that custom HTML
const char * web_parameter::get_custom_HTML()
{
  return _custom_HTML;
}

// constructor for text box webget
text_box_parameter::text_box_parameter(const char * id, const char * place_holder, char * default_value, int length) : web_parameter(id, place_holder), _value(String(default_value)), _max_length(length)
{
}

// constructor for text box webget
text_box_parameter::text_box_parameter(const char * id, const char * place_holder, char * default_value, int length, const char * custom_HTML) : web_parameter(id, place_holder, custom_HTML), _value(default_value), _max_length(length)
{
}

// method to clear object data
void text_box_parameter::clear_value()
{
  _value = "";
}

// method that returns the maximum field length
int text_box_parameter::get_value_length()
{
  return _max_length;
}

// method that returns the object value
String text_box_parameter::get_value()
{
  return _value;
}

// method that returns the HTML code for the field
String text_box_parameter::get_render()
{
  String temp = "<br/><input type='text' id='{i}' name='{n}' maxlength={l} placeholder='{p}' value='{v}'{c}>";

  temp.replace("{i}", get_ID());
  temp.replace("{n}", get_ID());
  temp.replace("{l}", String(get_value_length()));
  temp.replace("{p}", get_place_holder());
  temp.replace("{v}", get_value());
  temp.replace("{c}", get_custom_HTML());
  return temp;
}

// method that updates the object data
void text_box_parameter::set_from_server(std::unique_ptr<ESP8266WebServer>& server)
{
  _value = server->arg(get_ID());
}

// constructor for password box webget
password_box_parameter::password_box_parameter(const char * id, const char * place_holder, char * buffer, int length) : text_box_parameter(id, place_holder, buffer, length)
{
}

// constructor for password box webget
password_box_parameter::password_box_parameter(const char * id, const char * place_holder, char * buffer, int length, const char * custom_HTML) : text_box_parameter(id, place_holder, buffer, length, custom_HTML)
{
}

// method that returns the HTML code for the field
String password_box_parameter::get_render()
{
  String temp = "<br/><input style='width:80%' type='password' id='{i}' name='{n}' maxlength={l} placeholder='{p}'{c}><img style='position:relative;top:8px;left:8px;' id='eye_{i}' onClick=\"sh('eye_{i}','{i}')\" ontouchstart=\"sh('eye_{i}','{i}')\" src=''><script>document.getElementById('eye_{i}').src=ep;</script>";

  temp.replace("{i}", get_ID());
  temp.replace("{n}", get_ID());
  temp.replace("{p}", get_place_holder());
  temp.replace("{l}", String(get_value_length()));
  temp.replace("{c}", get_custom_HTML());
  return temp;
}

// method that updates the object data
void password_box_parameter::set_from_server(std::unique_ptr<ESP8266WebServer>& server)
{
  _value = server->arg(get_ID());
}

// constructor for check box webget
check_box_parameter::check_box_parameter(const char * id, const char * place_holder, bool default_value) : web_parameter(id, place_holder), _value(default_value)
{
}

// constructor for check box webget
check_box_parameter::check_box_parameter(const char * id, const char * place_holder, bool default_value, const char * custom_HTML) : web_parameter(id, place_holder, custom_HTML), _value(default_value)
{
}

// method to clear object data
void check_box_parameter::clear_value()
{
  _value = false;
}

// method that returns the HTML code for the field
String check_box_parameter::get_render()
{
  String temp = "<br/><label for='{f}'>{p}</label><input type='checkbox' id='{i}' name='{n}' value='checked'{v}{c}>";

  temp.replace("{i}", get_ID());
  temp.replace("{n}", get_ID());
  temp.replace("{f}", get_ID());
  temp.replace("{p}", get_place_holder());
  temp.replace("{v}", get_value() ? " checked" : "");
  temp.replace("{c}", get_custom_HTML());
  return temp;
}

// method that returns the object value
bool check_box_parameter::get_value()
{
  return _value;
}

// method that updates the object data
void check_box_parameter::set_from_server(std::unique_ptr<ESP8266WebServer>& server)
{
  _value = (server->arg(get_ID()) == "checked");
}

// constructor for radio field webget
radio_field_parameter::radio_field_parameter(const char * id, const char * place_holder, int default_value) : web_parameter(id, place_holder), _value(default_value), _index_max(0)
{
}

// constructor for radio field webget
radio_field_parameter::radio_field_parameter(const char * id, const char * place_holder, int default_value, const char * custom_HTML) : web_parameter(id, place_holder, custom_HTML), _value(default_value), _index_max(0)
{
}

// method to clear object data
void radio_field_parameter::clear_value()
{
  _value = -1;
}

// method that returns the HTML code for the field
String radio_field_parameter::get_render()
{
  String head = "<br/><fieldset><legend>{p}</legend>";
  String entry = "<label for='{f}'>{p}</label><input type='radio' id='{i}' name='{n}' value='{o}'{v}>";
  String tail = "</fieldset{c}>";
  String fields = "";

  head.replace("{p}", get_place_holder());
  for (std::list<field_entry>::iterator it = options.begin(); it != options.end(); ++it)
  {
    String temp = entry;

    temp.replace("{f}", it->id);
    temp.replace("{i}", it->id);
    temp.replace("{o}", it->id);
    temp.replace("{n}", get_ID());
    temp.replace("{p}", it->place_holder);
    temp.replace("{v}", (get_value() == it->index) ? " checked" : "");

    fields += temp;
  }

  tail.replace("{c}", get_custom_HTML());

  return head + fields + tail;
}

// method that adds an entry in the radio field
void radio_field_parameter::add_choice(const char * id, const char * place_holder)
{
  field_entry entry;

  entry.id = id;
  entry.place_holder = place_holder;
  entry.index = _index_max;
  options.push_back(entry);
  _index_max++;
}

// method that return the object value
int radio_field_parameter::get_value()
{
  return _value;
}

// method that updates the object data
void radio_field_parameter::set_from_server(std::unique_ptr<ESP8266WebServer>& server)
{
  for (std::list<field_entry>::iterator it = options.begin(); it != options.end(); ++it)
  {
    if (0 == strcmp(it->id, server->arg(get_ID()).c_str()))
    {
      _value = it->index;
      return;
    }
  }
  _value = -1;
}
