#include <Arduino.h>
#include <WiFi.h>
#include <WiFiManager.h>

WiFiManager wm; // global wm instance

WiFiManagerParameter string_param;            // global param ( for non blocking w params )
WiFiManagerParameterPassword password_param;  // global param ( for non blocking w params )
WiFiManagerParameterCheckBox checkbox1_param; // global param ( for non blocking w params )
WiFiManagerParameterCheckBox checkbox2_param; // global param ( for non blocking w params )
WiFiManagerParameterSelect select_param;      // global param ( for non blocking w params )
WiFiManagerParameterRadio radio_param;        // global param ( for non blocking w params )
WiFiManagerParameterSpacer spacer_1;          // global param ( for non blocking w params )

WiFiManagerParameterOption select_option1; // global param ( for non blocking w params )
WiFiManagerParameterOption select_option2; // global param ( for non blocking w params )
WiFiManagerParameterOption select_option3; // global param ( for non blocking w params )
WiFiManagerParameterOption select_option4; // global param ( for non blocking w params )

WiFiManagerParameterOption radio_option1; // global param ( for non blocking w params )
WiFiManagerParameterOption radio_option2; // global param ( for non blocking w params )
WiFiManagerParameterOption radio_option3; // global param ( for non blocking w params )
WiFiManagerParameterOption radio_option4; // global param ( for non blocking w params )

String getParam(String name)
{
    //read parameter from server, for customhmtl input
    String value;
    if (wm.server->hasArg(name))
    {
        value = wm.server->arg(name);
    }
    return value;
}

void saveParamCallback()
{
    printf("\nSaving...\n");
    printf("string_param: %s\n", getParam("string_param").c_str());
    printf("password_param: %s\n", getParam("password_param").c_str());
    printf("checkbox1_param: %s\n", getParam("checkbox1_param").c_str());
    Serial.println(checkbox1_param.isChecked()); //Alternative option
    printf("checkbox2_param: %s\n", getParam("checkbox2_param").c_str());
    Serial.println(checkbox2_param.isChecked()); //Alternative option
    printf("select_param: %s\n", getParam("select_param").c_str());
    printf("checkbox_param2: %s\n", getParam("checkbox_param2").c_str());
    printf("radio_param: %s\n", getParam("radio_param").c_str());
}

void setup()
{
    Serial.begin(115200);
    printf("\nBooting...\n");

    new (&string_param) WiFiManagerParameter("string_param", "String Param", "default_val", 40);
    new (&password_param) WiFiManagerParameterPassword("password_param", "Password Param", "default_val", 40);
    new (&checkbox1_param) WiFiManagerParameterCheckBox("checkbox1_param", "Checkbox Param");
    new (&checkbox2_param) WiFiManagerParameterCheckBox("checkbox2_param", "Checkbox Param", true);
    new (&select_param) WiFiManagerParameterSelect("select_param", "Select Param", "option_1", 40);
    new (&radio_param) WiFiManagerParameterRadio("radio_param", "Radio Param", "option_2", 40);

    new (&select_option1) WiFiManagerParameterOption("select_option1", "Option 1", "option_1", 8);
    new (&select_option2) WiFiManagerParameterOption("select_option2", "Option 2", "option_2", 8);
    new (&select_option3) WiFiManagerParameterOption("select_option3", "Option 3", "option_3", 8);
    new (&select_option4) WiFiManagerParameterOption("select_option4", "Option 4", "option_4", 8);

    new (&radio_option1) WiFiManagerParameterOption("radio_option1", "Option 1", "option_1", 8);
    new (&radio_option2) WiFiManagerParameterOption("radio_option2", "Option 2", "option_2", 8);
    new (&radio_option3) WiFiManagerParameterOption("radio_option3", "Option 3", "option_3", 8);
    new (&radio_option4) WiFiManagerParameterOption("radio_option4", "Option 4", "option_4", 8);

    new (&spacer_1) WiFiManagerParameterSpacer("spacer1", "Next part");

    select_param.addOption(&select_option1);
    select_param.addOption(&select_option2);
    select_param.addOption(&select_option3);
    select_param.addOption(&select_option4);

    radio_param.addOption(&radio_option1);
    radio_param.addOption(&radio_option2);
    radio_param.addOption(&radio_option3);
    radio_param.addOption(&radio_option4);

    wm.addParameter(&string_param);
    wm.addParameter(&password_param);
    wm.addParameter(&checkbox1_param);
    wm.addParameter(&checkbox2_param);
    wm.addParameter(&spacer_1);
    wm.addParameter(&select_param);
    wm.addParameter(&radio_param);
    wm.setSaveParamsCallback(saveParamCallback);
    std::vector<const char *> menu = {"wifi", "info", "param", "sep", "erase", "update", "restart", "exit"};
    wm.setMenu(menu);

    // set dark theme
    // wm.setClass("invert");

    wm.setConfigPortalTimeout(30);   // auto close configportal after n seconds
    wm.setCaptivePortalEnable(true); // disable captive portal redirection
    wm.setAPClientCheck(true);       // avoid timeout if client connected to softap

    // wifi scan settings
    // wm.setRemoveDuplicateAPs(false); // do not remove duplicate ap names (true)
    // wm.setMinimumSignalQuality(20);  // set min RSSI (percentage) to show in scans, null = 8%
    // wm.setShowInfoErase(false);      // do not show erase button on info page
    // wm.setScanDispPerc(true);       // show RSSI as percentage not graph icons

    // wm.setBreakAfterConfig(true);   // always exit configportal even if wifi save fails

    bool connect_result;
    connect_result = wm.autoConnect("AutoConnectAP", "password"); // password protected ap

    if (!connect_result)
    {
        printf("Failed to connect or hit timeout\n");
        // ESP.restart();
    }
    else
    {
        //if you get here you have connected to the WiFi
        printf("connected...yeey :)\n");
        wm.startWebPortal();
    }

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        printf("Connecting to WiFi..\n");
    }
    printf("Connected to the WiFi network\n");
}

void loop()
{
    // put your main code here, to run repeatedly:
    wm.process();
}
