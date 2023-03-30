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
}

void lvppScreen::ActivateScreen(uint32_t anim_time, lv_scr_load_anim_t anim_type) {    
    lv_scr_load_anim(pScreen, anim_type, anim_time, 0, false);
}

void lvppScreen::AddObject(lvppBase* pObj) {
    assert(pObj);
    objects.push_back(pObj);
    lv_obj_set_parent(pObj->getObj(), pScreen);
}

lvppBase* lvppScreen::findObj(const char* pName) {
    for(auto it: objects) {
        if (!strcmp(it->getFriendlyName().c_str(), pName))
            return it;
    }

    return nullptr;
}

void lvppScreen::setObjValue(const char* objName, int16_t val) {
    lvppBase* pB = findObj(objName);
    std::string type=pB->getObjType();
    if (pB) {
        if (type=="BAR") {
            lvppBar* pBar = dynamic_cast<lvppBar*>(pB);
            if (!pBar)
                throw std::bad_cast();
            else
                pBar->setValue(val);
        }
        else if (type=="SLIDER") {
            lvppSlider* pSlider = dynamic_cast<lvppSlider*>(pB);
            if (!pSlider)
                throw std::bad_cast();
            else
                pSlider->setValue(val);
        }

    }
}