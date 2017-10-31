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
#ifndef Webgets_h
#define Webgets_h

#include <list>

#include <ESP8266WebServer.h>

// common ancestor of webgets
class web_parameter
{
  private:
    const char * _id;
    const char * _place_holder;
    const char * _custom_HTML;
  protected:
    web_parameter(const char * id, const char * place_holder);
    web_parameter(const char * id, const char * place_holder, const char * custom_HTML);
  public:
    virtual void clear_value();
    const char * get_ID();
    const char * get_place_holder();
    const char * get_custom_HTML();
    virtual String get_render();
    virtual void set_from_server(std::unique_ptr<ESP8266WebServer>& server);
};

// text box class
class text_box_parameter : public web_parameter
{
  protected:
    int _max_length;
    String _value;
  public:
    text_box_parameter(const char * id, const char * place_holder, char * default_value, int length);
    text_box_parameter(const char * id, const char * place_holder, char * default_value, int length, const char * custom_HTML);
    void clear_value();
    int get_value_length();
    String get_value();
    String get_render();
    void set_from_server(std::unique_ptr<ESP8266WebServer>& server);
};

// password box class
class password_box_parameter : public text_box_parameter
{
  public:
    password_box_parameter(const char * id, const char * place_holder, char * buffer, int length);
    password_box_parameter(const char * id, const char * place_holder, char * buffer, int length, const char * custom_HTML);
    String get_render();
    void set_from_server(std::unique_ptr<ESP8266WebServer>& server);
};

// check box class
class check_box_parameter : public web_parameter
{
  private:
    bool _value;
  public:
    check_box_parameter(const char * id, const char * place_holder, bool default_value);
    check_box_parameter(const char * id, const char * place_holder, bool default_value, const char * custom_HTML);
    void clear_value();
    bool get_value();
    String get_render();
    void set_from_server(std::unique_ptr<ESP8266WebServer>& server);
};

// storage struct for radio field choices
typedef struct {
  const char * id;
  const char * place_holder;
  int index;
} field_entry;

// radio field class
class radio_field_parameter : public web_parameter
{
  private:
    int _value;
    int _index_max;
    std::list<field_entry> options;
  public:
    radio_field_parameter(const char * id, const char * place_holder, int default_value);
    radio_field_parameter(const char * id, const char * place_holder, int default_value, const char * custom_HTML);
    void clear_value();
    void add_choice(const char * id, const char * place_holder);
    int get_value();
    String get_render();
    void set_from_server(std::unique_ptr<ESP8266WebServer>& server);
};
 
#endif /* Webgets_h */
