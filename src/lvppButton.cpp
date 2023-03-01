#include "lvpp.h"

lvppButton::lvppButton(const char* fName, const char* pText, lv_obj_t* parent) : lvppBase(fName, "BUTTON") {
    objParent = parent ? parent : lv_scr_act();
    createObj(lv_btn_create(objParent));

    lv_style_set_pad_all(&style_obj, 5);
//    lv_style_set_bg_color(&style_obj, lv_color_hex(0xf1f1f1));  // lv_color_hex(0xeeeeee));
    lv_obj_add_style(obj, &style_obj, 0);

    label = lv_label_create(obj);

    if (pText) {
        lv_label_set_text(label, pText);
    }
}

lvppButton::~lvppButton() {
    if (label) {
      lv_obj_del_async(label);
      label = nullptr;
    }
}

void lvppButton::setText(const char* pText) {
    if (pText) {
        lv_label_set_text(label, pText);
    }
}

void lvppButton::setTextColor(lv_color_t newColor) {
    if (label) {
        lv_style_set_text_color(&style_obj, newColor);
        lv_obj_add_style(label, &style_obj, 0);
    }
}

lvppCycleButton::lvppCycleButton(const char* fName, lv_obj_t* parent) : lvppButton(fName, NULL, parent) {
    options.clear();
    currentIndex = 0;
    quantity = 0;
}

lvppCycleButton::~lvppCycleButton() {

}

void lvppCycleButton::clearOptions(void) {
    options.clear();
    quantity = 0;
}

void lvppCycleButton::addOption(const char* pText) {
    if (pText) {
        options.push_back(pText);
        quantity++;
    }
}

void lvppCycleButton::addOptionsFromVector(std::vector<std::string> &inOptions) {
    options = inOptions;
    currentIndex = 0;
    quantity = inOptions.size();

    setText(options[currentIndex].c_str());
 }

void lvppCycleButton::internalOnClicked() {
    currentIndex++;
    if (currentIndex >= quantity)
        currentIndex = 0;
    
    setText(options[currentIndex].c_str());
}

////////////////////////
////////////////////////
////////////////////////

lvppFullImageToggleButton::lvppFullImageToggleButton(const char* fName, lv_obj_t* parent) : lvppBase(fName, "FULLIMGTOGGLEBUTTON") {
    objParent = parent ? parent : lv_scr_act();
    createObj(lv_btn_create(objParent));
    lv_obj_add_flag(obj, LV_OBJ_FLAG_CHECKABLE);
//    lv_obj_set_size(btn1, 160+26, 47+16);

    bIsChecked = false;

    pImage = lv_img_create(obj);
}

lvppFullImageToggleButton::lvppFullImageToggleButton(const char* fName, lv_img_dsc_t uncheckedImg, lv_img_dsc_t checkedImg, lv_obj_t* parent) 
  : lvppBase(fName, "FULLIMGTOGGLEBUTTON") {
    objParent = parent ? parent : lv_scr_act();
    createObj(lv_btn_create(objParent));
    lv_obj_add_flag(obj, LV_OBJ_FLAG_CHECKABLE);
//    lv_obj_set_size(btn1, 160+26, 47+16);

    imgReleased = uncheckedImg;
    imgPressed  = checkedImg;

    bIsChecked = false;

    pImage = lv_img_create(obj);
    lv_img_set_src(pImage, &imgReleased);
}

lvppFullImageToggleButton::~lvppFullImageToggleButton() {
    if (pImage) {
        lv_obj_del(pImage);
        pImage = nullptr;
    }
}

void lvppFullImageToggleButton::setImageSourceUnChecked(const lv_img_dsc_t img) {
    imgReleased = img;
}

void lvppFullImageToggleButton::setImageSourceChecked(const lv_img_dsc_t img) {
    imgPressed = img;
}

void lvppFullImageToggleButton::onValueChanged() {
    if (lv_obj_get_state(obj) & LV_STATE_CHECKED) {
        lv_img_set_src(pImage, &imgPressed);
        printf("    IS CHECKED\n");
        bIsChecked = true;
        onButtonChecked();
    }
    else {
        lv_img_set_src(pImage, &imgReleased);
        printf("    IS UN-CHECKED\n");
        bIsChecked = false;
        onButtonUnChecked();
    }
}

bool lvppFullImageToggleButton::getCheckedState(void) {
    return bIsChecked;
}

void lvppFullImageToggleButton::setCheckedState(bool bSetChecked) {
    bIsChecked = bSetChecked;
    if (bIsChecked) {
        lv_obj_add_state(obj, LV_STATE_CHECKED);   /*Make the chekbox checked*/
        lv_img_set_src(pImage, &imgPressed);
        onButtonChecked();
    }
    else {
        lv_obj_clear_state(obj, LV_STATE_CHECKED); /*MAke the checkbox unchecked*/
        lv_img_set_src(pImage, &imgReleased);
        onButtonUnChecked();
    }
}
