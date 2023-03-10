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
#include <vector>
#include <string>

lvppDropdown::lvppDropdown(const char* fName, const char* pOptions, lv_obj_t* parent) : lvppBase(fName, "DROPDOWN") {
    objParent = parent ? parent : lv_scr_act();
    createObj(lv_dropdown_create(objParent));
    setOptions(pOptions);
}

lvppDropdown::~lvppDropdown() {

}

void lvppDropdown::setOptions(const char* pOptions)
{
    if (pOptions) {
        lv_dropdown_set_options(obj, pOptions);
    }
}

void lvppDropdown::clearOptions(void) {
    lv_dropdown_clear_options(obj);
}

void lvppDropdown::setOptions(std::vector<std::string> &options) {
    uint32_t qty=0;
    clearOptions();

    for (auto it: options) {
        lv_dropdown_add_option(obj, it.c_str(), qty++);
    }
}

// LV_DIR_BOTTOM, LV_DIR_TOP, LV_DIR_LEFT, LV_DIR_RIGHT
void lvppDropdown::setDropdownDirection(lv_dir_t dropDirection) {
    lv_dropdown_set_dir(obj, dropDirection);
}

void lvppDropdown::setCurrentIndex(uint16_t curInd) {
    if (curInd < lv_dropdown_get_option_cnt(obj)) {
        lv_dropdown_set_selected(obj, curInd);
        internalOnValueChanged();
        onValueChanged();
    }
}