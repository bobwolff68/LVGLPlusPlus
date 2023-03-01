#include "lvpp.h"

static    lv_style_t style_status;

lvppLabel::lvppLabel(const char* fName, const char* pText, lv_obj_t* parent) : lvppBase(fName, "LABEL") {
    objParent = parent ? parent : lv_scr_act();
    createObj(lv_label_create(objParent));

    if (pText) {
        lv_label_set_text(obj, pText);
    }

}

lvppLabel::~lvppLabel() {
}

void lvppLabel::setText(const char* pText) {
    if (pText) {
        lv_label_set_text(obj, pText);
    }
}

void lvppLabel::setTextColor(lv_color_t newColor) {
    lv_style_set_text_color(&style_obj, newColor);
    lv_obj_add_style(obj, &style_obj, 0);
}
