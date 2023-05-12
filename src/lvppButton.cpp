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
#include "lvpp.h"

/** @file lvppButton.cpp
 * @brief All button related lvpp class implementations.
*/

lvppButton::lvppButton(const char* fName, const char* pText, lv_obj_t* parent) : lvppBase(fName, "BUTTON") {
    objParent = parent ? parent : lv_scr_act();
    createObj(lv_btn_create(objParent));

    lv_style_set_pad_all(&style_obj, 5);
//    lv_style_set_bg_color(&style_obj, lv_color_hex(0xf1f1f1));  // lv_color_hex(0xeeeeee));
    lv_obj_add_style(obj, &style_obj, 0);

    if (pText) {
        setText(pText);
    }
}


lvppCycleButton::lvppCycleButton(const char* fName, lv_obj_t* parent) : lvppButton(fName, NULL, parent) {
    clearOptions();
}

void lvppCycleButton::clearOptions(void) {
    options.clear();
    currentIndex = 0;
}

void lvppCycleButton::setOptions(const char* pText) {
    if (pText) {
        currentIndex = 0;
        lvppOptions::setOptions(pText);
    }
    else {
        clearOptions();
    }
}

void lvppCycleButton::setOptions(std::vector<std::string> &inOptions) {
    currentIndex = 0;
    lvppOptions::setOptions(inOptions);
 }

void lvppCycleButton::internalOnClicked() {
    currentIndex++;
    if (currentIndex >= options.size())
        currentIndex = 0;
    
    setText(options[currentIndex].c_str());
}

////////////////////////
////////////////////////
////////////////////////

lvppFullImageToggleButton::lvppFullImageToggleButton(const char* fName, lv_obj_t* parent) : lvppBase(fName, "FULLIMGTOGGLEBUTTON") {
    objParent = parent ? parent : lv_scr_act();
    createObj(lv_btn_create(objParent));
    lv_obj_add_flag(obj, LV_OBJ_FLAG_CHECKABLE);
//    lv_obj_set_size(btn1, 160+26, 47+16);

    bIsChecked = false;

    pImage = lv_img_create(obj);
}

lvppFullImageToggleButton::lvppFullImageToggleButton(const char* fName, lv_img_dsc_t uncheckedImg, lv_img_dsc_t checkedImg, lv_obj_t* parent) 
  : lvppBase(fName, "FULLIMGTOGGLEBUTTON") {
    objParent = parent ? parent : lv_scr_act();
    createObj(lv_btn_create(objParent));
    lv_obj_add_flag(obj, LV_OBJ_FLAG_CHECKABLE);
//    lv_obj_set_size(btn1, 160+26, 47+16);

    imgReleased = uncheckedImg;
    imgPressed  = checkedImg;

    bIsChecked = false;

    pImage = lv_img_create(obj);
    lv_img_set_src(pImage, &imgReleased);
}

lvppFullImageToggleButton::~lvppFullImageToggleButton() {
    if (pImage) {
        lv_obj_del(pImage);
        pImage = nullptr;
    }
}

void lvppFullImageToggleButton::setImageSourceUnChecked(const lv_img_dsc_t img) {
    imgReleased = img;
}

void lvppFullImageToggleButton::setImageSourceChecked(const lv_img_dsc_t img) {
    imgPressed = img;
}

void lvppFullImageToggleButton::onValueChanged() {
    if (lv_obj_get_state(obj) & LV_STATE_CHECKED) {
        lv_img_set_src(pImage, &imgPressed);
        printf("    IS CHECKED\n");
        bIsChecked = true;
        onButtonChecked();
    }
    else {
        lv_img_set_src(pImage, &imgReleased);
        printf("    IS UN-CHECKED\n");
        bIsChecked = false;
        onButtonUnChecked();
    }
}

bool lvppFullImageToggleButton::getCheckedState(void) {
    return bIsChecked;
}

void lvppFullImageToggleButton::setCheckedState(bool bSetChecked) {
    bIsChecked = bSetChecked;
    if (bIsChecked) {
        lv_obj_add_state(obj, LV_STATE_CHECKED);   /*Make the chekbox checked*/
        lv_img_set_src(pImage, &imgPressed);
        onButtonChecked();
    }
    else {
        lv_obj_clear_state(obj, LV_STATE_CHECKED); /*MAke the checkbox unchecked*/
        lv_img_set_src(pImage, &imgReleased);
        onButtonUnChecked();
    }
}

////////////////////////
////////////////////////
////////////////////////

lvppSwitch::lvppSwitch(const char* fName, lv_obj_t* parent) : lvppBase(fName, "SWITCH") {
    objParent = parent ? parent : lv_scr_act();
    createObj(lv_switch_create(objParent));
}

void lvppSwitch::setEnabled(bool bEnable) {
    if (bEnable) {
        lv_obj_clear_state(obj, LV_STATE_DISABLED);
    }
    else {
        lv_obj_add_state(obj, LV_STATE_DISABLED);
    }
}

void lvppSwitch::setCheckedState(bool bChecked) {
    if (bChecked) {
        lv_obj_add_state(obj, LV_STATE_CHECKED);
    }
    else {
        lv_obj_clear_state(obj, LV_STATE_CHECKED);
    }
}

bool lvppSwitch::getCheckedState() {
    return lv_obj_has_state(obj, LV_STATE_CHECKED);
}
