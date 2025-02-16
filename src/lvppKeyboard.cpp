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

/** @file lvppKeyboard.cpp
 * @brief All keyboard related lvpp class implementations.
*/

lvppKeyboard::lvppKeyboard(const char* fName, lv_obj_t* parent) : lvppBase(fName, "KEYBOARD") {
    objParent = parent ? parent : lv_scr_act();
    obj = nullptr;
}

void lvppKeyboard::enableKeyboard(bool bEnable) {
    if (bEnable && !obj) {
       createObj(lv_keyboard_create(lv_scr_act()));
//        createObj(lv_keyboard_create(objParent));
    }
    else {
        if (obj) {
            lv_obj_remove_event_cb(obj, NULL);
            lv_obj_del(obj);
            obj = nullptr;
        }
    }
}

void lvppKeyboard::eventHandler(lv_event_t* event) {
    lv_obj_t *ta;
    lv_event_code_t code = lv_event_get_code(event);
    lv_obj_t* targobj = lv_event_get_target(event);

    switch(code) {
        case LV_EVENT_CLICKED:
            printf("Keyboard Clicked\n");
            break;
        case LV_EVENT_READY:
        case LV_EVENT_CANCEL:
            if (obj) {
                ta = lv_keyboard_get_textarea(obj);
                if (ta) {
                    lv_obj_clear_state(ta, LV_STATE_FOCUSED);
                    lv_indev_reset(NULL, ta);   /*To forget the last clicked object to make it focusable again*/
                }
            }
            enableKeyboard(false);
        default:
            break;
    }
}
