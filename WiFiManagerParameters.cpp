/**
 * --------------------------------------------------------------------------------
 *  WiFiManagerParameter
 * --------------------------------------------------------------------------------
**/

#include "WiFiManagerParameters.h"

WiFiManagerParameterBase::WiFiManagerParameterBase()
{
    _id = NULL;
    _label = NULL;
    _length = 1;
    _value = NULL;
}

WiFiManagerParameterBase::WiFiManagerParameterBase(const char *id, const char *label, const char *defaultValue, int length)
{
    _id = id;
    _label = label;
    setValue(defaultValue, length);
}

WiFiManagerParameterBase::~WiFiManagerParameterBase()
{
    if (_value != NULL)
    {
        delete[] _value;
    }
    _length = 0; // setting length 0, ideally the entire parameter should be removed, or added to wifimanager scope so it follows
}

void WiFiManagerParameterBase::setValue(const char *defaultValue, int length)
{
    if (!_id)
    {
        // Serial.println("cannot set value of this parameter");
        return;
    }

    // if(strlen(defaultValue) > length){
    //   // Serial.println("defaultValue length mismatch");
    //   // return false; //@todo bail
    // }

    _length = length;
    _value = new char[_length + 1];
    memset(_value, 0, _length + 1); // explicit null

    if (defaultValue != NULL)
    {
        strncpy(_value, defaultValue, _length);
    }
}

const char *WiFiManagerParameterBase::getValue() const
{
    // Serial.println(printf("Address of _value is %p\n", (void *)_value));
    return _value;
}
const char *WiFiManagerParameterBase::getID() const
{
    return _id;
}
const char *WiFiManagerParameterBase::getPlaceholder() const
{
    return _label;
}
const char *WiFiManagerParameterBase::getLabel() const
{
    return _label;
}
int WiFiManagerParameterBase::getValueLength() const
{
    return _length;
}

WiFiManagerParameter::WiFiManagerParameter()
{
    WiFiManagerParameter("");
}

WiFiManagerParameter::WiFiManagerParameter(const char *custom) : WiFiManagerParameterBase()
{
    _labelPlacement = WFM_LABEL_BEFORE;
    _customHTML = custom;
}

WiFiManagerParameter::WiFiManagerParameter(const char *id, const char *label) : WiFiManagerParameterBase(id, label, "", 1)
{
    _labelPlacement = WFM_LABEL_BEFORE;
    _customHTML = "";
}

WiFiManagerParameter::WiFiManagerParameter(const char *id, const char *label, const char *defaultValue, int length) : WiFiManagerParameterBase(id, label, defaultValue, length)
{
    _labelPlacement = WFM_LABEL_BEFORE;
    _customHTML = "";
}

WiFiManagerParameter::WiFiManagerParameter(const char *id, const char *label, const char *defaultValue, int length, const char *custom) : WiFiManagerParameterBase(id, label, defaultValue, length)
{
    _labelPlacement = WFM_LABEL_BEFORE;
    _customHTML = custom;
}

WiFiManagerParameter::WiFiManagerParameter(const char *id, const char *label, const char *defaultValue, int length, const char *custom, int labelPlacement) : WiFiManagerParameterBase(id, label, defaultValue, length)
{

    _labelPlacement = labelPlacement;
}

// void WiFiManagerParameter::init(const char *id, const char *label, const char *defaultValue, int length, const char *custom, int labelPlacement)
// {
//     _id = id;
//     _label = label;
//     _labelPlacement = labelPlacement;
//     _customHTML = custom;
//     setValue(defaultValue, length);
// }

// @note debug is not available in wmparameter class

int WiFiManagerParameter::getLabelPlacement() const
{
    return _labelPlacement;
}
const char *WiFiManagerParameter::getCustomHTML() const
{
    return _customHTML;
}

String WiFiManagerParameter::getHTML()
{
    char valLength[5];

    String pitem;
    switch (getLabelPlacement())
    {
    case WFM_LABEL_BEFORE:
        pitem = FPSTR(HTTP_FORM_LABEL);
        pitem += getHTMLTemplate();
        break;
    case WFM_LABEL_AFTER:
        pitem = getHTMLTemplate();
        pitem += FPSTR(HTTP_FORM_LABEL);
        break;
    default:
        // WFM_NO_LABEL
        pitem = getHTMLTemplate();
        break;
    }

    // Input templating
    // "<br/><input id='{i}' name='{n}' maxlength='{l}' value='{v}' {c}>";
    // if no ID use customhtml for item, else generate from param string
    if (getID() != NULL)
    {
        pitem.replace(FPSTR(T_t), getLabel()); // T_T label

        pitem.replace(FPSTR(T_i), getID()); // T_i id name
        pitem.replace(FPSTR(T_n), getID()); // T_n id name alias
        snprintf(valLength, 5, "%d", getValueLength());
        pitem.replace(FPSTR(T_l), valLength);       // T_l value length
        pitem.replace(FPSTR(T_v), getValue());      // T_v value
        pitem.replace(FPSTR(T_c), getCustomHTML()); // T_c meant for additional attributes, not html, but can stuff
    }
    else
    {
        pitem = getCustomHTML();
    }
    return pitem;
}

String WiFiManagerParameter::getHTMLTemplate()
{
    return FPSTR(HTTP_FORM_PARAM);
}

bool WiFiManagerParameter::addOption(WiFiManagerParameterOption *p)
{

    // check param id is valid, unless null
    if (p->getID())
    {
        for (size_t i = 0; i < strlen(p->getID()); i++)
        {
            if (!(isAlphaNumeric(p->getID()[i])) && !(p->getID()[i] == '_'))
            {
#ifdef WM_DEBUG_LEVEL
                DEBUG_WM(DEBUG_ERROR, F("[ERROR] parameter IDs can only contain alpha numeric chars"));
#endif
                return false;
            }
        }
    }

    // init params if never malloc
    if (_options == NULL)
    {
#ifdef WM_DEBUG_LEVEL
        DEBUG_WM(DEBUG_DEV, F("allocating params bytes:"), _max_params * sizeof(WiFiManagerParameter *));
#endif
        _options = (WiFiManagerParameterOption **)malloc(_max_options * sizeof(WiFiManagerParameterOption *));
    }

    // resize the params array by increment of WIFI_MANAGER_MAX_OPTIONS
    if (_optionsCount == _max_options)
    {
        _max_options += WIFI_MANAGER_MAX_OPTIONS;
#ifdef WM_DEBUG_LEVEL
        DEBUG_WM(DEBUG_DEV, F("Updated _max_options:"), _max_options);
        DEBUG_WM(DEBUG_DEV, F("re-allocating options bytes:"), _max_options * sizeof(WiFiManagerParameterOption *));
#endif
        WiFiManagerParameterOption **new_options = (WiFiManagerParameterOption **)realloc(_options, _max_options * sizeof(WiFiManagerParameterOption *));
#ifdef WM_DEBUG_LEVEL
// DEBUG_WM(WIFI_MANAGER_MAX_OPTIONS);
// DEBUG_WM(_optionsCount);
// DEBUG_WM(_max_options);
#endif
        if (new_options != NULL)
        {
            _options = new_options;
        }
        else
        {
#ifdef WM_DEBUG_LEVEL
            DEBUG_WM(DEBUG_ERROR, F("[ERROR] failed to realloc params, size not increased!"));
#endif
            return false;
        }
    }

    _options[_optionsCount] = p;
    _optionsCount++;

#ifdef WM_DEBUG_LEVEL
    DEBUG_WM(DEBUG_VERBOSE, F("Added Parameter:"), p->getID());
#endif
    return true;
}

WiFiManagerParameterPassword::WiFiManagerParameterPassword() : WiFiManagerParameter()
{
}

WiFiManagerParameterPassword::WiFiManagerParameterPassword(const char *id, const char *label) : WiFiManagerParameter(id, label)
{
}
WiFiManagerParameterPassword::WiFiManagerParameterPassword(const char *id, const char *label, const char *defaultValue, int length) : WiFiManagerParameter(id, label, defaultValue, length)
{
}
WiFiManagerParameterPassword::WiFiManagerParameterPassword(const char *id, const char *label, const char *defaultValue, int length, int labelPlacement) : WiFiManagerParameter(id, label, defaultValue, length, "", labelPlacement)
{
}
String WiFiManagerParameterPassword::getHTMLTemplate()
{
    return FPSTR(HTTP_FORM_PARAM_PASS);
}

WiFiManagerParameterSelect::WiFiManagerParameterSelect() : WiFiManagerParameter()
{
}

WiFiManagerParameterSelect::WiFiManagerParameterSelect(const char *id, const char *label) : WiFiManagerParameter(id, label)
{
}
WiFiManagerParameterSelect::WiFiManagerParameterSelect(const char *id, const char *label, const char *defaultValue, int length) : WiFiManagerParameter(id, label, defaultValue, length)
{
}
WiFiManagerParameterSelect::WiFiManagerParameterSelect(const char *id, const char *label, const char *defaultValue, int length, int labelPlacement) : WiFiManagerParameter(id, label, defaultValue, length, "", labelPlacement)
{
}
String WiFiManagerParameterSelect::getHTMLTemplate()
{
    return FPSTR(HTTP_FORM_PARAM_SELECT);
}

String WiFiManagerParameterSelect::getHTML()
{
    String pitem;
    switch (getLabelPlacement())
    {
    case WFM_LABEL_BEFORE:
        pitem = FPSTR(HTTP_FORM_LABEL);
        pitem += getHTMLTemplate();
        break;
    case WFM_LABEL_AFTER:
        pitem = getHTMLTemplate();
        pitem += FPSTR(HTTP_FORM_LABEL);
        break;
    default:
        // WFM_NO_LABEL
        pitem = getHTMLTemplate();
        break;
    }

    // Input templating
    // "<br/><input id='{i}' name='{n}' maxlength='{l}' value='{v}' {c}>";
    // if no ID use customhtml for item, else generate from param string
    String optionhtml;

    if (_optionsCount > 0)
    {
        // add the extra parameters to the form
        for (int i = 0; i < _optionsCount; i++)
        {
            if (_options[i] == NULL || _options[i]->getValueLength() == 0)
            {
                break;
            }
            String optiontemp = FPSTR(HTTP_FORM_PARAM_SELECT_OPTION);
            optiontemp.replace(FPSTR(T_l), _options[i]->getLabel()); // T_T label
            optiontemp.replace(FPSTR(T_v), _options[i]->getValue()); // T_T label
            if (strcmp(_options[i]->getValue(), getValue()) == 0)
            {
                optiontemp.replace(FPSTR(T_s), FPSTR(HTTP_FORM_PARAM_CHECKED)); // T_T label
            }
            optionhtml += optiontemp;
        }
    }

    if (getID() != NULL)
    {
        pitem.replace(FPSTR(T_t), getLabel()); // T_T label

        pitem.replace(FPSTR(T_i), getID());    // T_i id name
        pitem.replace(FPSTR(T_n), getID());    // T_n id name alias
        pitem.replace(FPSTR(T_o), optionhtml); // T_n id name alias
    }
    else
    {
        pitem = getCustomHTML();
    }
    return pitem;
}

WiFiManagerParameterCheckBox::WiFiManagerParameterCheckBox() : WiFiManagerParameter()
{
}
WiFiManagerParameterCheckBox::WiFiManagerParameterCheckBox(const char *id, const char *label) : WiFiManagerParameter(id, label, "true", 4)
{
}
WiFiManagerParameterCheckBox::WiFiManagerParameterCheckBox(const char *id, const char *label, bool checked) : WiFiManagerParameter(id, label, "true", 4)
{
    _checked = checked;
    if (checked)
    {
        setValue("true", 4);
    }
}

WiFiManagerParameterCheckBox::WiFiManagerParameterCheckBox(const char *id, const char *label, bool checked, int labelPlacement) : WiFiManagerParameter(id, label, "true", 4, "", labelPlacement)
{
    _checked = checked;
}
String WiFiManagerParameterCheckBox::getHTMLTemplate()
{
    return FPSTR(HTTP_FORM_PARAM_CHECKBOX);
}

String WiFiManagerParameterCheckBox::getHTML()
{
    String pitem;
    switch (getLabelPlacement())
    {
    case WFM_LABEL_BEFORE:
        pitem = FPSTR(HTTP_FORM_LABEL);
        pitem += getHTMLTemplate();
        break;
    case WFM_LABEL_AFTER:
        pitem = getHTMLTemplate();
        pitem += FPSTR(HTTP_FORM_LABEL);
        break;
    default:
        // WFM_NO_LABEL
        pitem = getHTMLTemplate();
        break;
    }

    // Input templating
    // "<br/><input id='{i}' name='{n}' maxlength='{l}' value='{v}' {c}>";
    // if no ID use customhtml for item, else generate from param string
    if (getID() != NULL)
    {
        pitem.replace(FPSTR(T_t), getLabel()); // T_T label

        pitem.replace(FPSTR(T_i), getID()); // T_i id name
        pitem.replace(FPSTR(T_n), getID()); // T_n id name alias
        // snprintf(valLength, 5, "%d", getValueLength());
        if (_checked)
        {
            pitem.replace(FPSTR(T_C), FPSTR(HTTP_FORM_PARAM_CHECKED)); // T_T label
        }
        else
        {
            pitem.replace(FPSTR(T_C), ""); // T_T label
        }

        pitem.replace(FPSTR(T_v), getValue());      // T_v value
        pitem.replace(FPSTR(T_c), getCustomHTML()); // T_c meant for additional attributes, not html, but can stuff
    }
    else
    {
        pitem = getCustomHTML();
    }
    return pitem;
}

bool WiFiManagerParameterCheckBox::isChecked()
{

    if (strcmp(getValue(), "true") == 0)
    {
        _checked = true;
    }
    else
    {
        _checked = false;
    }
    return _checked;
}

WiFiManagerParameterOption::WiFiManagerParameterOption()
{
}

WiFiManagerParameterOption::WiFiManagerParameterOption(const char *id, const char *label, const char *defaultValue, int length) : WiFiManagerParameterBase(id, label, defaultValue, length)
{
}

void WiFiManagerParameterOption::init(const char *id, const char *label, const char *defaultValue, int length)
{
    _id = id;
    _label = label;
    setValue(defaultValue, length);
}

void WiFiManagerParameterOption::setValue(const char *defaultValue, int length)
{
    if (!_id)
    {
        // Serial.println("cannot set value of this parameter");
        return;
    }

    _length = length;
    _value = new char[_length + 1];
    memset(_value, 0, _length + 1); // explicit null

    if (defaultValue != NULL)
    {
        strncpy(_value, defaultValue, _length);
    }
}

WiFiManagerParameterRadio::WiFiManagerParameterRadio() : WiFiManagerParameter()
{
}

WiFiManagerParameterRadio::WiFiManagerParameterRadio(const char *id, const char *label) : WiFiManagerParameter(id, label)
{
}
WiFiManagerParameterRadio::WiFiManagerParameterRadio(const char *id, const char *label, const char *defaultValue, int length) : WiFiManagerParameter(id, label, defaultValue, length)
{
}
WiFiManagerParameterRadio::WiFiManagerParameterRadio(const char *id, const char *label, const char *defaultValue, int length, int labelPlacement) : WiFiManagerParameter(id, label, defaultValue, length, "", labelPlacement)
{
}
String WiFiManagerParameterRadio::getHTMLTemplate()
{
    return FPSTR(HTTP_FORM_PARAM_SELECT);
}

String WiFiManagerParameterRadio::getHTML()
{

    // Input templating
    // "<br/><input id='{i}' name='{n}' maxlength='{l}' value='{v}' {c}>";
    // if no ID use customhtml for item, else generate from param string
    String optionhtml;

    if (_optionsCount > 0)
    {
        // add the extra parameters to the form
        for (int i = 0; i < _optionsCount; i++)
        {
            if (_options[i] == NULL || _options[i]->getValueLength() == 0)
            {
                break;
            }
            String optiontemp = FPSTR(HTTP_FORM_PARAM_RADIO);
            //"<br/><input type='radio' id='{i}' name='{n}' value='{ov}'><label for='{on}'>'{ol}'</label>";

            optiontemp.replace(FPSTR(T_i), getID());                  // T_T id
            optiontemp.replace(FPSTR(T_n), getID());                  // T_T id
            optiontemp.replace(FPSTR(T_ov), _options[i]->getValue()); // T_T label
            optiontemp.replace(FPSTR(T_on), _options[i]->getID());    // T_T label
            optiontemp.replace(FPSTR(T_ol), _options[i]->getLabel()); // T_T label

            if (strcmp(_options[i]->getValue(), getValue()) == 0)
            {
                optiontemp.replace(FPSTR(T_C), FPSTR(HTTP_FORM_PARAM_CHECKED)); // T_T label
            }
            else
            {
                optiontemp.replace(FPSTR(T_C), ""); // T_T label
            }
            optionhtml += optiontemp;
        }
    }

    String pitem;

    switch (getLabelPlacement())
    {
    case WFM_LABEL_BEFORE:
        pitem = FPSTR(HTTP_FORM_LABEL);
        pitem += optionhtml;
        break;
    case WFM_LABEL_AFTER:
        pitem = optionhtml;
        pitem += FPSTR(HTTP_FORM_LABEL);
        break;
    default:
        // WFM_NO_LABEL
        pitem = optionhtml;
        break;
    }

    if (getID() != NULL)
    {
        pitem.replace(FPSTR(T_t), getLabel()); // T_T label

        pitem.replace(FPSTR(T_i), getID());    // T_i id name
        pitem.replace(FPSTR(T_n), getID());    // T_n id name alias
        pitem.replace(FPSTR(T_o), optionhtml); // T_n id name alias
    }
    else
    {
        pitem = getCustomHTML();
    }
    return pitem;
}

WiFiManagerParameterSpacer::WiFiManagerParameterSpacer() : WiFiManagerParameter()
{
}

WiFiManagerParameterSpacer::WiFiManagerParameterSpacer(const char *id, const char *label) : WiFiManagerParameter(id, label)
{
}

String WiFiManagerParameterSpacer::getHTMLTemplate()
{
    return FPSTR(HTTP_FORM_PARAM_SPACER);
}

String WiFiManagerParameterSpacer::getHTML()
{
    String pitem;

    pitem = getHTMLTemplate();
    pitem += FPSTR(HTTP_FORM_LABEL);
    pitem += FPSTR(HTTP_BR);

    // Input templating
    // "<br/><input id='{i}' name='{n}' maxlength='{l}' value='{v}' {c}>";
    // if no ID use customhtml for item, else generate from param string
    if (getID() != NULL)
    {
        pitem.replace(FPSTR(T_t), getLabel()); // T_T label
        pitem.replace(FPSTR(T_i), getID());    // T_i id name
    }
    return pitem;
}
