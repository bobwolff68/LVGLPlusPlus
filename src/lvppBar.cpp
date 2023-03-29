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

lvppBar::lvppBar(const char* fName, lv_obj_t* parent) : lvppBase(fName, "BAR") {
    objParent = parent ? parent : lv_scr_act();
    createObj(lv_bar_create(objParent));

    valueLabel = nullptr;
    valueLabelFormat = "%d";

    min=0;
    max=100;
    setValue(0);
}

void lvppBar::enableValueLabel(lv_coord_t xoff, lv_coord_t yoff, lv_align_t alignment) {
    if (!valueLabel) {
        valueLabel = lv_label_create(objParent);
    }

    lv_obj_align_to(valueLabel, obj, alignment, xoff, yoff);
}

void lvppBar::setValue(int16_t value, bool animate)
{
    if (value >= min && value <= max) {
        curValue = value;
        lv_bar_set_value(obj, value, animate ? LV_ANIM_ON : LV_ANIM_OFF);
    }

    // Even if the value was out of range, let's call the downstream folks expecting us.
    internalOnValueChanged();
    onValueChanged();
}

void lvppBar::setRange(int16_t range_min, int16_t range_max)
{
    min=range_min;
    max=range_max;

    lv_bar_set_range(obj, range_min, range_max);
}

void lvppBar::internalOnValueChanged() {
    if (valueLabel) {
        lv_label_set_text_fmt(valueLabel, valueLabelFormat.c_str(), curValue);
    }
}

void lvppBar::setValueLabelFormat(const char* fmt) {
    if (fmt) {
        valueLabelFormat = fmt;
    }
}


/////////////////////////////
//
//
//
/////////////////////////////

lvppSlider::lvppSlider(const char* fName, lv_obj_t* parent) : lvppBase(fName, "SLIDER") {
    objParent = parent ? parent : lv_scr_act();
    createObj(lv_slider_create(objParent));

    valueLabel = nullptr;
    valueLabelFormat = "%d";

    min=0;
    max=100;
    setValue(0);
}

void lvppSlider::enableValueLabel(lv_coord_t xoff, lv_coord_t yoff, lv_align_t alignment) {
    if (!valueLabel) {
        valueLabel = lv_label_create(objParent);
    }

    lv_obj_align_to(valueLabel, obj, alignment, xoff, yoff);
}

void lvppSlider::setValue(int16_t value, bool animate)
{
    if (value >= min && value <= max) {
        curValue = value;
        lv_bar_set_value(obj, value, animate ? LV_ANIM_ON : LV_ANIM_OFF);
    }

    // Even if the value was out of range, let's call the downstream folks expecting us.
    internalOnValueChanged();
    onValueChanged();
}

void lvppSlider::setRange(int16_t range_min, int16_t range_max)
{
    min=range_min;
    max=range_max;

    lv_bar_set_range(obj, range_min, range_max);
}

void lvppSlider::internalOnValueChanged() {
    curValue = lv_slider_get_value(obj);

    if (valueLabel) {
        lv_label_set_text_fmt(valueLabel, valueLabelFormat.c_str(), curValue);
    }
}

void lvppSlider::setValueLabelFormat(const char* fmt) {
    if (fmt) {
        valueLabelFormat = fmt;
    }
}