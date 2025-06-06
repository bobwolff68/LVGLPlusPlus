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

#include <algorithm>

/** @file lvppBase.cpp
 * @brief Implementation of the base class for the library - lvppBase.
*/

#define LV_EVENT_UNKNOWN "EVENT_UNKNOWN:"

/**
 * @brief Uncommenting the definition of LOG_EVENTS will make for a noisy logging of all events.
 *        This can be useful if things feel upside down when you're adding features or something.
 * 
 */
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
            LV_LOG_ERROR("lvppBase::setFontSize() - font size not available. Error.\n");
            throw;
            return;
    }

    lv_obj_add_style(obj, &style_obj, LV_PART_MAIN | LV_STATE_DEFAULT);
}

void lvppBase::setFont(const lv_font_t* pF) {
    lv_style_set_text_font(&style_obj, pF);
    lv_obj_add_style(obj, &style_obj, LV_PART_MAIN | LV_STATE_DEFAULT);
}

/** @todo I think this add_style needs to go away and an 'invalidate' should be used instead.
 * 
*/
void lvppBase::setBGColor(lv_color_t color) {
    lv_obj_set_style_bg_color(obj, color, LV_PART_MAIN);
//    lv_style_set_bg_color(&style_obj, color);
//    lv_style_set_bg_opa(&style_obj, LV_OPA_100);
//    lv_obj_add_style(obj, &style_obj, LV_PART_MAIN | LV_STATE_DEFAULT);
    if (label) {
//        printf(":%s:setVGColor - setting label background color now.\n", whoAmI());
        lv_obj_set_style_bg_color(label, color, 0);
        lv_obj_set_style_bg_opa(label, LV_OPA_100, 0);
    }
}

void lvppBase::setColorGradient(lv_color_t col1, lv_color_t col2, lv_grad_dir_t direction) {
    lv_style_selector_t sel;
    std::string ot = getObjType();

    if (ot=="BAR" || ot=="SLIDER" || ot=="ARC") {
        sel = LV_PART_INDICATOR;
    }
    else {
        sel = LV_PART_MAIN;
    }
    lv_obj_set_style_bg_color(obj, col1, sel);
    lv_obj_set_style_bg_grad_color(obj, col2, sel);
    lv_obj_set_style_bg_grad_dir(obj, direction, sel);
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

void lvppBase::createLabel() {
//    printf(":%s:createLabel - entry.\n", whoAmI());

    if (!label) {
        label = lv_label_create(obj);
    }
}

void lvppBase::createAdjLabel() {
    if (!adjLabel) {
        adjLabel = lv_label_create(objParent);
    }
}

void lvppBase::align(lv_align_t align, lv_coord_t x_ofs, lv_coord_t y_ofs)
{
    lv_obj_align(obj, align, x_ofs, y_ofs);
}

void lvppBase::setText(const char* pText) {
    createLabel();
    if (pText) {
        lv_label_set_text(label, pText);
    }
    else {
        lv_label_set_text(label, "");
    }
}

std::string lvppBase::getText() {
    std::string st = "";
    if (label) {
        st = lv_label_get_text(label);
    }

    return st;
}

void lvppBase::setTextAlign(lv_align_t align, lv_coord_t xoff, lv_coord_t yoff)
{
    createLabel();
    lv_obj_align(label, align, xoff, yoff);
}

void lvppBase::setTextColor(lv_color_t newColor) {
    if (label) {
        lv_style_set_text_color(&style_obj, newColor);
        lv_obj_add_style(label, &style_obj, 0);
        lv_obj_set_style_text_color(label, newColor, 0);
    }
}

void lvppBase::setLabelJustificationAlignment(lv_text_align_t _align) {
    if (label) {
        lv_obj_set_style_text_align(label, _align, 0);
    }
}

void lvppBase::setLabelColorizationEnabled(bool bEnable) {
    if (label) {
        lv_label_set_recolor(label, bEnable);
    }
}

void lvppBase::setAdjText(const char* pText, lv_coord_t x_ofs, lv_coord_t y_ofs) {
    createAdjLabel();
    if (pText) {
        lv_label_set_text(adjLabel, pText);
    }
    if (x_ofs!=-10000 && y_ofs!=-10000) {
        lv_obj_align_to(adjLabel, obj, LV_ALIGN_CENTER, x_ofs, y_ofs);
    }
}

void lvppBase::setAdjTextColor(lv_color_t newColor) {
    createAdjLabel();
    if (adjLabel) {
        lv_obj_set_style_text_color(adjLabel, newColor, 0);
    }
}

void lvppBase::setAdjBGColor(lv_color_t color) {
    createAdjLabel();
    if (adjLabel) {
        lv_obj_set_style_bg_color(adjLabel, color, 0);
        lv_obj_set_style_bg_opa(adjLabel, LV_OPA_100, 0);
    }
}

void lvppBase::setAdjColorGradient(lv_color_t col1, lv_color_t col2, lv_grad_dir_t direction) {
    createAdjLabel();
    if (adjLabel) {
        lv_obj_set_style_bg_color(adjLabel, col1, LV_PART_MAIN);
        lv_obj_set_style_bg_grad_color(adjLabel, col2, LV_PART_MAIN);
        lv_obj_set_style_bg_grad_dir(adjLabel, direction, LV_PART_MAIN);
    }
}

void lvppBase::setAdjFont(const lv_font_t* pF) {
    createAdjLabel();
    if (adjLabel) {
        lv_obj_set_style_text_font(adjLabel, pF, 0);
    }
}

void lvppBase::setAdjJustificationAlignment(lv_text_align_t _align) {
    createAdjLabel();
    if (adjLabel) {
        lv_obj_set_style_text_align(adjLabel, _align, 0);
    }
}

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
//            printf("%s: CALLING onClicked()...\n", whoAmI());
            internalOnClicked();
            onClicked();
            if (cbOnClicked)
                cbOnClicked();
            break;
        case LV_EVENT_VALUE_CHANGED:
//            printf("%s: CALLING onValueChanged()...\n", whoAmI());
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
    if (label && getObjType()!="LABEL") {
        lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);  // If the size of the object changed, the text will be wrong - so let's just center it as default.
    }
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

void lvppBase::setNewParent(lv_obj_t* pNewParent) {
    objParent = pNewParent;
    // Main object needs this new pointer.
    lv_obj_set_parent(obj, pNewParent);
    // Label's parent is 'obj' so don't reset that.

    // If adjlabel exists, set its parent here
    // This is due to the fact that adjLabel's parent is not the main object
    // so that it can be moved outside the boundaries of the main object.
    if (adjLabel) {
        lv_obj_set_parent(adjLabel, pNewParent);
    }
}

//
//
// l v p p B a s e W i t h V a l u e
//
//

lvppBaseWithValue::lvppBaseWithValue(const char* fName, const char* oType) : lvppBase(fName, oType) {
    valueLabel = nullptr;
    valueLabelFormat = "%d";
    curValue = 0;

    min=0;
    max=100;
}

/** @todo TECHDEBT - should allow values outside range, but UI only shows inside range.
 *        However, this will impact the 'percentage' function too and likely needs to become getUIPercentage
 * 
*/
void lvppBaseWithValue::setValue(int16_t value, bool animate)
{
    if (value >= min && value <= max) {
        curValue = value;
        baseSetter(value, animate);
        lv_event_send(obj, LV_EVENT_VALUE_CHANGED, NULL);
    }
}

void lvppBaseWithValue::createValueLabel() {
    if (!valueLabel) {
        valueLabel = lv_label_create(objParent);
    }
}

void lvppBaseWithValue::enableValueLabel(lv_coord_t xoff, lv_coord_t yoff, lv_align_t alignment) {
    createValueLabel();
    lv_obj_align_to(valueLabel, obj, alignment, xoff, yoff);
}

void lvppBaseWithValue::setValueLabelFont(const lv_font_t* pF) {
    if (!valueLabel) {
        enableValueLabel(0,0);
    }

    lv_obj_set_style_text_font(valueLabel, pF, 0);
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

    lv_obj_set_style_text_color(valueLabel, newColor, 0);
    lv_obj_invalidate(valueLabel);
}

void lvppBaseWithValue::setValueLabelBGColor(lv_color_t newColor) {
    if (!valueLabel) {
        enableValueLabel(0,0);
    }

    lv_obj_set_style_bg_color(valueLabel, newColor, 0);
    lv_obj_set_style_bg_opa(valueLabel, LV_OPA_100, 0);
    lv_obj_invalidate(valueLabel);
}

void lvppBaseWithValue::setValueLabelColorGradient(lv_color_t col1, lv_color_t col2, lv_grad_dir_t direction) {
    if (!valueLabel) {
        enableValueLabel(0,0);
    }

    lv_obj_set_style_bg_color(valueLabel, col1, LV_PART_MAIN);
    lv_obj_set_style_bg_grad_color(valueLabel, col2, LV_PART_MAIN);
    lv_obj_set_style_bg_grad_dir(valueLabel, direction, LV_PART_MAIN);
}

void lvppBaseWithValue::setValueLabelJustificationAlignment(lv_text_align_t _align) {
    if (!valueLabel) {
        enableValueLabel(0,0);
    }

    lv_obj_set_style_text_align(valueLabel, _align, 0);
    lv_obj_invalidate(valueLabel);
}

void lvppBaseWithValue::internalOnValueChanged() {
    curValue = baseGetter();

    if (valueLabel) {
        lv_label_set_text_fmt(valueLabel, valueLabelFormat.c_str(), curValue);
        lv_obj_invalidate(valueLabel);
    }
}

void lvppBaseWithValue::setNewParent(lv_obj_t* pNewParent) {
    // If valuelabel exists, set its parent
    if (valueLabel) {
        lv_obj_set_parent(valueLabel, pNewParent);
    }

    // Now call the parent.
    lvppBase::setNewParent(pNewParent);
}

//
//
// l v p p O p t i o n s
//
//

void lvppOptions::setOptions(const char* pOpts) {
    std::string currentLine;
    const char* currentChar = pOpts;

    if (!pOpts) {
        LV_LOG_WARN("lvppOptions::setOptions - argument is nullptr. Not setting.\n");
        return;
    }

    clearOptions();

    // Rip through the pOpts string and add strings ending in \n to options.
    while (*currentChar != '\0') {
        if (*currentChar == '\n') {
            // When we get to a newline...put the current line in options.
            options.push_back(currentLine);
            currentLine.clear();
        } else {
            // Add character to the end of the current line until we get a newline
            currentLine += *currentChar;
        }
        currentChar++;
    }
    
    // Add the last line if it doesn't end with '\n'
    if (!currentLine.empty()) {
        options.push_back(currentLine);
    }

    // Set all of the ID values to the index value since they aren't being used otherwise here.
    for (unsigned int i=0; i<options.size(); i++) {
        idList.push_back((uint64_t)i);
    }

    lvOptionSetter(getNewlineSepOptions());
}

void lvppOptions::setOptionsWithIDs(std::vector<std::pair<std::string, uint64_t>>& valIDs) {
    options.clear();
//    printf("setOptionsWithIDs: Pair-size inbound: %lu\n", valIDs.size());
    for (unsigned int i=0; i<valIDs.size(); i++) {
        options.push_back(valIDs[i].first);
        idList.push_back(valIDs[i].second);
//        printf("  Adding: [%llu] : %s\n", idList[i], options[i].c_str());
    }
//    printf("setOptionsWithIDs after add:\n");
//    printList();
    lvOptionSetter(getNewlineSepOptions());
}

void lvppOptions::setOptions(std::vector<std::string>& _opts) {
    options = _opts;

    // Set all of the ID values to the index value since they aren't being used otherwise here.
    for (unsigned int i=0; i<options.size(); i++) {
        idList.push_back((uint64_t)i);
    }

    lvOptionSetter(getNewlineSepOptions());
}

void lvppOptions::addOption(const char* pOpt) {
    options.push_back(pOpt);
    idList.push_back((uint64_t)(options.size()-1));

    lvOptionSetter(getNewlineSepOptions());
}

void lvppOptions::addOption(std::string& _opt) {
    options.push_back(_opt);
    idList.push_back((uint64_t)(options.size()-1));

    lvOptionSetter(getNewlineSepOptions());
}

void lvppOptions::addOptionWithID(const char* pOpt, uint64_t id) {
    options.push_back(pOpt);
    idList.push_back((uint64_t)id);

    lvOptionSetter(getNewlineSepOptions());
}

void lvppOptions::printList(void) {
    printf("options list has %lu entries. idList has %lu entries.\n", options.size(), idList.size());
    for (unsigned int i=0;i<options.size();i++) {
        bool useID = idList.size() > i;
        printf("  [%lld] : %s\n", useID ? (int64_t)idList[i] : -1, options[i].c_str());
    }
}

void lvppOptions::addOptionWithID(std::string& _opt, uint64_t id) {
    options.push_back(_opt);
    idList.push_back((uint64_t)id);

    lvOptionSetter(getNewlineSepOptions());
}

void lvppOptions::clearOptions() {
    options.clear();
    idList.clear();

    lvOptionSetter(getNewlineSepOptions());
}

uint64_t lvppOptions::getSelectedID() {
    return idList[lvOptionGetIndex()];
}

bool lvppOptions::setSelectedID(uint64_t _val) {
    // Need to find the index of the input _val
    auto it = std::find(idList.begin(), idList.end(), _val);
    if (it == idList.end())
        return false;
    
    uint16_t ind = it - idList.begin();
    lvOptionSetIndex(ind);
    return true;
}

const char* lvppOptions::getNewlineSepOptions() {
    bool first=true;
    oneString = "";

    for (const auto& it: options) {
        // Prevents final entry from having \n concatenated.
        if (!first) {
            oneString += '\n';
        }
        oneString += it;
        first = false;
    }

    return oneString.c_str();
}
