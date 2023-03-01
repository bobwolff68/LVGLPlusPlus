#include "lvpp.h"

lvppBar::lvppBar(const char* fName, lv_obj_t* parent) : lvppBase(fName, "BAR") {
    objParent = parent ? parent : lv_scr_act();
    createObj(lv_bar_create(objParent));

    extraLabel = nullptr;
    labelFormat = "%d";

    min=0;
    max=100;
    setValue(0);
}

lvppBar::~lvppBar() {

}

void lvppBar::enableLabel(lv_align_t alignment, lv_coord_t xoff, lv_coord_t yoff) {
    if (!extraLabel) {
        extraLabel = lv_label_create(objParent);
    }

    lv_obj_align_to(extraLabel, obj, alignment, xoff, yoff);
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
    if (extraLabel) {
        lv_label_set_text_fmt(extraLabel, labelFormat.c_str(), curValue);
    }
}

void lvppBar::setLabelFormat(const char* fmt) {
    if (fmt) {
        labelFormat = fmt;
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

    extraLabel = nullptr;
    labelFormat = "%d";

    min=0;
    max=100;
    setValue(0);
}

lvppSlider::~lvppSlider() {

}

void lvppSlider::enableLabel(lv_align_t alignment, lv_coord_t xoff, lv_coord_t yoff) {
    if (!extraLabel) {
        extraLabel = lv_label_create(objParent);
    }

    lv_obj_align_to(extraLabel, obj, alignment, xoff, yoff);
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

    if (extraLabel) {
        lv_label_set_text_fmt(extraLabel, labelFormat.c_str(), curValue);
    }
}

void lvppSlider::setLabelFormat(const char* fmt) {
    if (fmt) {
        labelFormat = fmt;
    }
}