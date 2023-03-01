#include "lvpp.h"

lvppArc::lvppArc(const char* fName, lv_obj_t* parent) : lvppBase(fName, "ARC") {
    objParent = parent ? parent : lv_scr_act();
    createObj(lv_arc_create(objParent));

    lv_arc_set_range(obj, 0, 100);
    // Default orientation and such
    lv_arc_set_rotation(obj, 250);
    lv_arc_set_bg_angles(obj, 0, 260);
    lv_arc_set_value(obj, 140);
    setValue(50);
}

lvppArc::~lvppArc() {

}

void lvppArc::setValue(int16_t value, bool animate)
{
    lv_arc_set_value(obj, value);
    curValue = value;
}

void lvppArc::setRange(int16_t range_min, int16_t range_max)
{
    lv_arc_set_range(obj, range_min, range_max);
}
