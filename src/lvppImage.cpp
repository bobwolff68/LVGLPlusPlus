#include "lvpp.h"

lvppImage::lvppImage(const char* fName, lv_obj_t* parent) : lvppBase(fName, "IMAGE") {
    objParent = parent ? parent : lv_scr_act();
    createObj(lv_img_create(objParent));
}

lvppImage::~lvppImage() {

}

void lvppImage::setImage(lv_img_dsc_t img) {
    lv_img_set_src(obj, &img);
}