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