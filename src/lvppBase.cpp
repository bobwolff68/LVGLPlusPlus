#include "lvppBase.h"

#define LV_EVENT_UNKNOWN "EVENT_UNKNOWN:"
// Noisy logging of all known events
//#define LOG_EVENTS

bool lvppBase::bEventNamesInitComplete = false;

void lvppBase::align(lv_align_t align, lv_coord_t x_ofs, lv_coord_t y_ofs)
{
    lv_obj_align(obj, align, x_ofs, y_ofs);
}

/**************************
 * 
 * Available point sizes are dictated by the LV_FONT* items which are enabled at compile-time.
 * In platform.ini...build_flags...
 *   -D LV_FONT_MONTSERRAT_22=1
 * 
 * By default, we are compiling "in" all even point sizes from 8 to 24
 * 
 * ****************/

void lvppBase::setFontSize(uint8_t points) {
    lv_font_t F;

    switch(points) {
        case 8:
            lv_style_set_text_font(&style_obj, &lv_font_montserrat_8);
            break;
        case 10:
            lv_style_set_text_font(&style_obj, &lv_font_montserrat_10);
            break;
        case 12:
            lv_style_set_text_font(&style_obj, &lv_font_montserrat_12);
            break;
        case 14:
            lv_style_set_text_font(&style_obj, &lv_font_montserrat_14);
            break;
        case 16:
            lv_style_set_text_font(&style_obj, &lv_font_montserrat_16);
            break;
        case 18:
            lv_style_set_text_font(&style_obj, &lv_font_montserrat_18);
            break;
        case 20:
            lv_style_set_text_font(&style_obj, &lv_font_montserrat_20);
            break;
        case 22:
            lv_style_set_text_font(&style_obj, &lv_font_montserrat_22);
            break;
        case 24:
            lv_style_set_text_font(&style_obj, &lv_font_montserrat_24);
            break;
        default:
            throw;
            return;
    }

    lv_obj_add_style(obj, &style_obj, LV_PART_MAIN | LV_STATE_DEFAULT);
}

void lvppBase::setFont(lv_font_t* pF) {
    lv_style_set_text_font(&style_obj, pF);
    lv_obj_add_style(obj, &style_obj, LV_PART_MAIN | LV_STATE_DEFAULT);
}

void lvppBase::setBGColor(lv_color16_t color16) {
    lv_style_set_bg_color(&style_obj, color16);
//    lv_style_set_bg_color(&style_status, lv_palette_main(LV_PALETTE_BLUE)); // lv_color_hex(0x115588));
    lv_obj_add_style(obj, &style_obj, LV_PART_MAIN | LV_STATE_DEFAULT);
}

lvppBase::lvppBase(const char* fName, const char* oType) {
  if (!bEventNamesInitComplete)
    initEventNames();

  obj = nullptr;
  objParent = nullptr;
  friendlyName = "";
  setObjType("lvppBase");

  if (fName)
    setFriendlyName(fName);

  if (oType)
    setObjType(oType);

  lv_style_init(&style_obj);
}

lvppBase::~lvppBase() {
    if (obj) {
      lv_obj_del_async(obj);
      obj = nullptr;
    }
}

void lvppBase::createObj(lv_obj_t* o) {
    if (!o)
        throw;
    
    obj = o;
    lv_obj_set_user_data(obj, this);

    // Handle all event callbacks
    lv_obj_add_event_cb(obj, &lvppBase::lvCallback, LV_EVENT_ALL, this);
}

//
// Static member to handle all inbound callback events.
// Used to "route" event handling to the appropriate C++ object
//
void lvppBase::lvCallback(lv_event_t* event) {
  lv_event_code_t code = lv_event_get_code(event);
  lvppBase* pBase = (lvppBase*)lv_event_get_user_data(event);
  assert(pBase);

  pBase->baseEventHandler(event);
}

void lvppBase::baseEventHandler(lv_event_t* event) {
    lv_event_code_t code = lv_event_get_code(event);

#ifdef LOG_EVENTS
    if (!isUnknownCode(code)) {
        if (lv_event_get_current_target(event) != obj)
            printf("NOT_CURRENT_OBJECT\n");
        else
            printf("CURRENT_OBJ: ");

        printf("baseEventHandler for %s: %s\n", whoAmI(), getEventName(lv_event_get_code(event)));
    }
#endif

    switch(code) {
        case LV_EVENT_CLICKED:
            printf("%s: CALLING onClicked()...\n", whoAmI());
            internalOnClicked();
            onClicked();
            break;
        case LV_EVENT_VALUE_CHANGED:
            printf("%s: CALLING onValueChanged()...\n", whoAmI());
            internalOnValueChanged();
            onValueChanged();
            break;
        default:
            // Call derived class
            eventHandler(event);
            break;
    }
}

void lvppBase::setSize(lv_coord_t width, lv_coord_t height) {
    lv_obj_set_size(obj, width, height);
}

void lvppBase::setFriendlyName(const char* pName) {
    if (pName) {
        friendlyName = pName;
    }
}

const char* lvppBase::whoAmI(void) {
    static std::string ts;

    if (friendlyName != "")
        ts = friendlyName+"("+objType+")";
    else
        ts = "*("+objType+")";
    
    return ts.c_str();
}


void lvppBase::initEventNames(void) {
    for (int i=0; i<_LV_EVENT_LAST; i++)
        eventNames[i] = (char*)LV_EVENT_UNKNOWN;
    
    eventNames[LV_EVENT_PRESSED] = "LV_EVENT_PRESSED";
    eventNames[LV_EVENT_PRESSING] = "LV_EVENT_PRESSING";
    eventNames[LV_EVENT_PRESS_LOST] = "LV_EVENT_PRESS_LOST";
    eventNames[LV_EVENT_SHORT_CLICKED] = "LV_EVENT_SHORT_CLICKED";
    eventNames[LV_EVENT_LONG_PRESSED] = "LV_EVENT_LONG_PRESSED";
    eventNames[LV_EVENT_LONG_PRESSED_REPEAT] = "LV_EVENT_LONG_PRESSED_REPEAT";
    eventNames[LV_EVENT_CLICKED] = "LV_EVENT_CLICKED";
    eventNames[LV_EVENT_RELEASED] = "LV_EVENT_RELEASED";
    eventNames[LV_EVENT_SCROLL_BEGIN] = "LV_EVENT_SCROLL_BEGIN";
    eventNames[LV_EVENT_SCROLL_END] = "LV_EVENT_SCROLL_END";
    eventNames[LV_EVENT_SCROLL] = "LV_EVENT_SCROLL";
    eventNames[LV_EVENT_GESTURE] = "LV_EVENT_GESTURE";
    eventNames[LV_EVENT_KEY] = "LV_EVENT_KEY";
    eventNames[LV_EVENT_FOCUSED] = "LV_EVENT_FOCUSED";
    eventNames[LV_EVENT_DEFOCUSED] = "LV_EVENT_DEFOCUSED";
    eventNames[LV_EVENT_LEAVE] = "LV_EVENT_LEAVE";

    eventNames[LV_EVENT_VALUE_CHANGED] = "LV_EVENT_VALUE_CHANGED";
    eventNames[LV_EVENT_INSERT] = "LV_EVENT_INSERT";
    eventNames[LV_EVENT_REFRESH] = "LV_EVENT_REFRESH";
    eventNames[LV_EVENT_READY] = "LV_EVENT_READY";
    eventNames[LV_EVENT_CANCEL] = "LV_EVENT_CANCEL";

    eventNames[LV_EVENT_DELETE] = "LV_EVENT_DELETE";
    eventNames[LV_EVENT_SIZE_CHANGED] = "LV_EVENT_SIZE_CHANGED";
    eventNames[LV_EVENT_STYLE_CHANGED] = "LV_EVENT_STYLE_CHANGED";
    eventNames[LV_EVENT_LAYOUT_CHANGED] = "LV_EVENT_LAYOUT_CHANGED";

}

bool lvppBase::isUnknownCode(lv_event_code_t code) {
    return eventNames[code]==LV_EVENT_UNKNOWN;
}

const char* lvppBase::getEventName(lv_event_code_t code) {
    static char res[30];

    if (code >= _LV_EVENT_LAST) {
        snprintf(res, 30, "EVENT_OUT_OF_RANGE:%d", (int)code);
        return (const char*)res;
    }

    if (eventNames[code]==LV_EVENT_UNKNOWN) {
        snprintf(res, 30, "%s%d", LV_EVENT_UNKNOWN, (int)code);
        return (const char*)res;
    }

    bEventNamesInitComplete = false;
    return (const char*)eventNames[code].c_str();
}