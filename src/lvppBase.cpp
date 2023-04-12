// Copyright 2023 Robert M. Wolff (bob dot wolff 68 at gmail dot com)
//
// Redistribution and use in source and binary forms, with or without modification, 
// are permitted provided that the following conditions are met:
//
// 1. Redistributions of source code must retain the above copyright notice, this 
// list of conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright notice, 
// this list of conditions and the following disclaimer in the documentation and/or 
// other materials provided with the distribution.
//
// 3. Neither the name of the copyright holder nor the names of its contributors 
// may be used to endorse or promote products derived from this software without 
// specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND 
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE 
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE 
// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL 
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR 
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER 
// CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, 
// OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
#include "lvppBase.h"

#define LV_EVENT_UNKNOWN "EVENT_UNKNOWN:"
// Noisy logging of all known events
//#define LOG_EVENTS

bool lvppBase::bEventNamesInitComplete = false;

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

  cbOnClicked = nullptr;
  cbOnValueChanged = nullptr;
  obj = nullptr;
  objParent = nullptr;
  label = nullptr;
  adjLabel = nullptr;
  friendlyName = "";
  setObjType("lvppBase");

  if (fName)
    setFriendlyName(fName);

  if (oType)
    setObjType(oType);

  lv_style_init(&style_obj);
}

lvppBase::~lvppBase() {
    if (adjLabel) {
      lv_obj_del_async(adjLabel);
      adjLabel = nullptr;
    }

    if (label) {
      lv_obj_del_async(label);
      label = nullptr;
    }

    if (obj) {
      lv_obj_del_async(obj);
      obj = nullptr;
    }
}

void lvppBase::align(lv_align_t align, lv_coord_t x_ofs, lv_coord_t y_ofs)
{
    lv_obj_align(obj, align, x_ofs, y_ofs);
}

void lvppBase::setText(const char* pText) {
    if (!label) {
        label = lv_label_create(obj);
    }
    if (pText) {
        lv_label_set_text(label, pText);
    }
}

void lvppBase::setTextAlign(lv_align_t align, lv_coord_t xoff, lv_coord_t yoff)
{
    if (!label) {
        label = lv_label_create(obj);
    }
    lv_obj_align(label, align, xoff, yoff);
}

void lvppBase::setTextColor(lv_color_t newColor) {
    if (label) {
        lv_style_set_text_color(&style_obj, newColor);
        lv_obj_add_style(label, &style_obj, 0);
    }
}

void lvppBase::setAdjText(const char* pText, lv_coord_t x_ofs, lv_coord_t y_ofs) {
    if (!adjLabel) {
        adjLabel = lv_label_create(objParent);
    }
    if (pText) {
        lv_label_set_text(adjLabel, pText);
    }
    if (x_ofs!=-10000 && y_ofs!=-10000) {
        lv_obj_align_to(adjLabel, obj, LV_ALIGN_CENTER, x_ofs, y_ofs);
    }
}

// void lvppBase::setAdjTextColor(lv_color_t newColor) {
//     if (adjLabel) {
//         lv_style_set_text_color(&style_obj, newColor);
//         lv_obj_add_style(adjLabel, &style_obj, 0);
//     }
// }

void lvppBase::createObj(lv_obj_t* o) {
    if (!o)
        throw;
    
    obj = o;
    lv_obj_add_style(obj, &style_obj, 0);

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
            if (cbOnClicked)
                cbOnClicked();
            break;
        case LV_EVENT_VALUE_CHANGED:
            printf("%s: CALLING onValueChanged()...\n", whoAmI());
            internalOnValueChanged();
            onValueChanged();
            if (cbOnValueChanged)
                cbOnValueChanged();
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

lvppBaseWithValue::lvppBaseWithValue(const char* fName, const char* oType) : lvppBase(fName, oType) {
    valueLabel = nullptr;
    valueLabelFormat = "%d";
    curValue = 0;
    lv_style_init(&style_value_obj);

    min=0;
    max=100;
}

void lvppBaseWithValue::enableValueLabel(lv_coord_t xoff, lv_coord_t yoff, lv_align_t alignment) {
    if (!valueLabel) {
        valueLabel = lv_label_create(objParent);
        lv_obj_add_style(valueLabel, &style_value_obj, 0);
    }

    lv_obj_align_to(valueLabel, obj, alignment, xoff, yoff);
}

void lvppBaseWithValue::setValueLabelFont(const lv_font_t* pF) {
    if (!valueLabel) {
        enableValueLabel(0,0);
    }

    lv_style_set_text_font(&style_value_obj, pF);
    lv_obj_invalidate(valueLabel);
}

void lvppBaseWithValue::setValueLabelFormat(const char* fmt) {
    if (fmt) {
        valueLabelFormat = fmt;
    }
}

void lvppBaseWithValue::setValueLabelColor(lv_color_t newColor) {
    if (!valueLabel) {
        enableValueLabel(0,0);
    }

    lv_style_set_text_color(&style_value_obj, newColor);
    lv_obj_invalidate(valueLabel);
}

void lvppBaseWithValue::internalOnValueChanged() {
    if (valueLabel) {
        lv_label_set_text_fmt(valueLabel, valueLabelFormat.c_str(), curValue);
        lv_obj_invalidate(valueLabel);
    }
}

