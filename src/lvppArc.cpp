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

lvppArc::lvppArc(const char* fName, lv_obj_t* parent) : lvppBaseWithValue(fName, "ARC") {
    objParent = parent ? parent : lv_scr_act();
    createObj(lv_arc_create(objParent));

    setRange(0, 100);
    // Default 'start' is at 3-oclock position as '0 degrees'
    // This rotates 'start' to the 6-oclock position and
    // sets the total arc to be 270 degrees from that point.
    setArcRotationAndSweep(90, 0, 270);
    setValue(50);
}

void lvppArc::setArcColor(lv_color_t newColor) {
    lv_style_set_arc_color(&style_obj, newColor);
    lv_obj_invalidate(obj);
}

void lvppArc::setArcRotationAndSweep(uint16_t rot, uint16_t startAngle, uint16_t endAngle) {
    assert(rot>=0 && rot<=360);
    assert(startAngle>=0 && startAngle <= 361);
    assert(endAngle>=0 && endAngle <= 361);

    lv_arc_set_rotation(obj, rot); 
    if (startAngle != 361 && endAngle != 361) {
        lv_arc_set_bg_angles(obj, startAngle, endAngle);
    }    
}

void lvppArc::setRange(int16_t range_min, int16_t range_max)
{
    lv_arc_set_range(obj, range_min, range_max);
}
