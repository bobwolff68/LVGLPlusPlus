#ifndef _LVPPBASE_H
#define _LVPPBASE_H

#include <string>

#ifdef LV_LVGL_H_INCLUDE_SIMPLE
#include "lvgl.h"
#else
#include "lvgl/lvgl.h"
#endif

static std::string eventNames[_LV_EVENT_LAST];

class lvppBase {
public:
    lvppBase(const char* fName, const char* oType);
    ~lvppBase();
    void createObj(lv_obj_t* o);
    static const char* getEventName(lv_event_code_t code);
    static bool isUnknownCode(lv_event_code_t code);
    lv_obj_t* getObj(void) { return obj; };
    void setSize(lv_coord_t width, lv_coord_t height);
    void align(lv_align_t align, lv_coord_t x_ofs, lv_coord_t y_ofs);
    virtual void onClicked() { };
    virtual void internalOnClicked() { };
    virtual void onValueChanged() { };
    virtual void internalOnValueChanged() { };
    void setFriendlyName(const char* pName);
    const char* whoAmI(void);
    void setFontSize(uint8_t points);
    void setFont(lv_font_t* pF);
    void setBGColor(lv_color16_t color16);

protected:
    lv_obj_t* obj;
    lv_obj_t* objParent;
    std::string objType;
    std::string friendlyName;
    lv_style_t style_obj;
    void setObjType(const char* pType) { objType = pType; };

    virtual void eventHandler(lv_event_t* event) { };
    void baseEventHandler(lv_event_t* event);
    static void lvCallback(lv_event_t* event);
    static void initEventNames(void);

    static bool bEventNamesInitComplete;
};

#endif
