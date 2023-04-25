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

/**
 * @file lvppScreen.cpp
 * @brief Implementation of lvppScreen class.
 */

///
///
/// lvppScreen
///
///

lvppScreen::lvppScreen(lv_obj_t* _pInitScreen) {
    if (!_pInitScreen)
        pScreen = lv_obj_create(NULL); 
    else
        pScreen = _pInitScreen;
    assert(pScreen);

    pPriorScreen=nullptr;
}

void lvppScreen::disableScrollBars() {
    lv_obj_clear_flag(pScreen, LV_OBJ_FLAG_SCROLLABLE);
}

void lvppScreen::activateScreen(uint32_t anim_time, lv_scr_load_anim_t anim_type) {    
    pPriorScreen = lv_scr_act();
    if (anim_time==0 && anim_type==LV_SCR_LOAD_ANIM_NONE) {
        lv_scr_load(pScreen);
    }
    else {
        lv_scr_load_anim(pScreen, anim_type, anim_time, 0, false);
    }
}

void lvppScreen::activatePriorScreen() {
    if (pPriorScreen) {
        lv_scr_load(pPriorScreen);
        pPriorScreen=nullptr;
    }
}

void lvppScreen::addObject(lvppBase* pObj) {
    assert(pObj);
    objects.push_back(pObj);
    lv_obj_set_parent(pObj->getObj(), pScreen);
}

lvppBase* lvppScreen::findObj(const char* pName) {
    for(auto it: objects) {
//        printf("lvppScreen:findObj - looking at: %s\n", it->getFriendlyName().c_str());
        if (!strcmp(it->getFriendlyName().c_str(), pName))
            return it;
    }

    return nullptr;
}

///@todo Support more than just bar and slider for value setting. Get clear on what widgets
///      should bet settable.
///@todo Create a 'getter' version as well.
///@todo Create a setObj() possibly for boolean types like buttons?? OR just use 1/0 as the 'val'?
bool lvppScreen::setObjValue(const char* objName, int16_t val) {
    lvppBase* pB = findObj(objName);
    std::string type=pB->getObjType();
    if (pB) {
        if (type=="BAR") {
            lvppBar* pBar = (lvppBar*)pB;
            if (!pBar)
                throw std::bad_cast();
            else
                pBar->setValue(val);
        }
        else if (type=="SLIDER") {
            lvppSlider* pSlider = (lvppSlider*)pB;
            if (!pSlider)
                throw std::bad_cast();
            else
                pSlider->setValue(val);
        }
        return true;
    }
    else {
        return false;
    }
}
