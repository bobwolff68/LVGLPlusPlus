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

static    lv_style_t style_status;

/** @file lvppTextarea.cpp
 * @brief Implementation of the textarea support for displaying text
*/

lvppTextarea::lvppTextarea(const char* fName, const char* pText, lv_obj_t* parent) : lvppBase(fName, "TEXTAREA") {
    objParent = parent ? parent : lv_scr_act();
    createObj(lv_textarea_create(objParent));

    if (pText) {
        setText(pText);
    }
}

void lvppTextarea::setText(const char* pText, const char* pPlaceholder) {
    if (pText) {
        lv_textarea_set_text(obj, pText);
    }

    if (pPlaceholder) {
        lv_textarea_set_placeholder_text(obj, pPlaceholder);
    }
}

const char * lvppTextarea::getText() {
    return lv_textarea_get_text(obj);
}

/** @todo I think this add_style needs to go away and an 'invalidate' should be used instead.
 * 
*/
void lvppTextarea::setTextColor(lv_color_t newColor) {
    lv_style_set_text_color(&style_obj, newColor);
    lv_obj_add_style(obj, &style_obj, 0);
}

void lvppTextarea::eventHandler(lv_event_t* event) {
    lv_event_code_t code = lv_event_get_code(event);

    lv_obj_t * ta = lv_event_get_target(event);
    if (!ta) {
        printf("lvppTextarea::eventHandler - no target for event.\n");
    }

    switch(code) {
        case LV_EVENT_FOCUSED:
            if (pKB) {
                pKB->enableKeyboard();
                lv_keyboard_set_textarea(pKB->getObj(), ta);
            }
//failed            lv_obj_scroll_to_view_recursive(ta, LV_ANIM_ON);
            break;
        case LV_EVENT_DEFOCUSED:
            if (pKB) {
                pKB->enableKeyboard(false);
            }
            break;
        default:
            break;
    }
}
