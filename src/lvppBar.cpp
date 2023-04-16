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

/** @file lvppBar.cpp
 * @brief Implementation for Bar and Bar-like widgets (Bar and Slider)
*/


lvppBar::lvppBar(const char* fName, lv_obj_t* parent) : lvppBaseWithValue(fName, "BAR") {
    objParent = parent ? parent : lv_scr_act();
    createObj(lv_bar_create(objParent));

    setValue(0);
}

void lvppBar::setValue(int16_t value, bool animate)
{
    if (value >= min && value <= max) {
        curValue = value;
        lv_bar_set_value(obj, value, animate ? LV_ANIM_ON : LV_ANIM_OFF);
        lv_event_send(obj, LV_EVENT_VALUE_CHANGED, NULL);
    }
}

void lvppBar::setRange(int16_t range_min, int16_t range_max)
{
    min=range_min;
    max=range_max;

    lv_bar_set_range(obj, range_min, range_max);
}


/////////////////////////////
//
//
//
/////////////////////////////

lvppSlider::lvppSlider(const char* fName, lv_obj_t* parent) : lvppBaseWithValue(fName, "SLIDER") {
    objParent = parent ? parent : lv_scr_act();
    createObj(lv_slider_create(objParent));

    setValue(0);
}

void lvppSlider::setValue(int16_t value, bool animate)
{
    if (value >= min && value <= max) {
        curValue = value;
        lv_bar_set_value(obj, value, animate ? LV_ANIM_ON : LV_ANIM_OFF);
        lv_event_send(obj, LV_EVENT_VALUE_CHANGED, NULL);
    }
}

void lvppSlider::setRange(int16_t range_min, int16_t range_max)
{
    min=range_min;
    max=range_max;

    lv_bar_set_range(obj, range_min, range_max);
}
