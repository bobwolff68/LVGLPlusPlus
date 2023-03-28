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
    void setFontSize(uint8_t points);
    void setFont(lv_font_t* pF);
    void setBGColor(lv_color16_t color16);
    virtual void setText(const char* pText);
    void setTextColor(lv_color_t newColor);
    virtual void setAdjText(const char* pText, lv_coord_t x_ofs=-10000, lv_coord_t y_ofs=-10000);
//    void setAdjTextColor(lv_color_t newColor);
    virtual void onClicked() { };
    virtual void internalOnClicked() { };
    virtual void onValueChanged() { };
    virtual void internalOnValueChanged() { };
    void setFriendlyName(const char* pName);
    std::string getFriendlyName() { return friendlyName; };
    std::string getObjType() { return objType; };
    const char* whoAmI(void);
    // Lambda Callback setups - to allow many objects to not require subclassing
    void setCallbackOnClicked(std::function<void()> cbF) {
        assert(!cbOnClicked);
        assert(cbF);
        cbOnClicked = cbF;
    };
    void setCallbackOnValueChanged(std::function<void()> cbF) {
        assert(!cbOnValueChanged);
        assert(cbF);
        cbOnValueChanged = cbF;
    };

protected:
    lv_obj_t* label;
    lv_obj_t* adjLabel; // For items that have a label 'nearby'
    std::function<void()> cbOnClicked=nullptr; // Start without a lambda callback
    std::function<void()> cbOnValueChanged=nullptr; // Start without a lambda callback
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
