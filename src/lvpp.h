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
#pragma once

#ifdef LV_LVGL_H_INCLUDE_SIMPLE
#include "lvgl.h"
#else
#include "lvgl/lvgl.h"
#endif

#ifdef USE_SDL
#include "app_hal.h"
#endif

#include "lvppBase.h"

#include <vector>
#include <string>
#include <map>

class lvppScreen { 
public:
    lvppScreen(lv_obj_t* _pInitScreen=nullptr);
    ~lvppScreen() { if (pScreen) lv_obj_del(pScreen); };
    void AddObject(lvppBase* pObj);
    lv_obj_t* GetScreen() { return pScreen; };
    //
    // LV_SCR_LOAD_ANIM_OUT_RIGHT
    void ActivateScreen(uint32_t anim_time=500, lv_scr_load_anim_t anim=LV_SCR_LOAD_ANIM_NONE);
    lvppBase* findObj(const char* pName);
    void setObjValue(const char* objName, int16_t val);
    void setObjValue(bool b);
    void setObjText(std::string str);
protected:
    lv_obj_t* pScreen=nullptr;
    std::vector<lvppBase*> objects;
};


class lvppButton : public lvppBase {
public:
    lvppButton(const char* fName, const char* pText=nullptr, lv_obj_t* parent=nullptr);
};

//
// Button has multiple "values"
// Each click cycles forward through the list of values.
//
class lvppCycleButton : public lvppButton {
public:
    lvppCycleButton(const char* fName, lv_obj_t* parent=nullptr);
    void clearOptions(void);
    void addOptions(const char* pOption);
    void addOptions(std::vector<std::string> &inOptions);
    u_int16_t getCurrentIndex() { return currentIndex; };
    const char* getCurrentText() { return options[currentIndex].c_str(); };
    void internalOnClicked();
protected:
    std::vector<std::string> options;
    uint16_t currentIndex;
    uint16_t quantity;
};

class lvppFullImageToggleButton : public lvppBase {
public:
    lvppFullImageToggleButton(const char* fName, lv_obj_t* parent=nullptr);
    lvppFullImageToggleButton(const char* fName, lv_img_dsc_t uncheckedImg, lv_img_dsc_t checkedImg, lv_obj_t* parent=nullptr);
    ~lvppFullImageToggleButton();
    void setImageSourceUnChecked(const lv_img_dsc_t img);
    void setImageSourceChecked(const lv_img_dsc_t img);
    virtual void onValueChanged();
    virtual void onButtonChecked() { };
    virtual void onButtonUnChecked() { };
    bool getCheckedState(void);
    void setCheckedState(bool bSetChecked=true);
protected:
    lv_img_dsc_t imgReleased;
    lv_img_dsc_t imgPressed;
    lv_obj_t* pImage;
    bool bIsChecked;
};

class lvppImage : public lvppBase {
public:
    lvppImage(const char* fName, lv_obj_t* parent=nullptr);
    ~lvppImage();
    void setImage(lv_img_dsc_t img);
};

class lvppLabel : public lvppBase {
public:
    lvppLabel(const char* fName, const char* pText, lv_obj_t* parent=nullptr);
    virtual void setText(const char* pText);
    void setTextColor(lv_color_t newColor);
};

class lvppBar : public lvppBaseWithValue {
public:
    lvppBar(const char* fName, lv_obj_t* parent=nullptr);
    void setRange(int16_t range_min, int16_t range_max);
    void setValue(int16_t value, bool animate=true);
};

class lvppSlider : public lvppBaseWithValue {
public:
    lvppSlider(const char* fName, lv_obj_t* parent=nullptr);
    void setRange(int16_t range_min, int16_t range_max);
    void setValue(int16_t value, bool animate=true);
};

class lvppArc : public lvppBaseWithValue {
public:
    lvppArc(const char* fName, lv_obj_t* parent=nullptr);
    void setArcColor(lv_color_t newColor);
    void setRange(int16_t range_min, int16_t range_max);
    void setValue(int16_t value, bool animate=true);
    void setArcRotationAndSweep(uint16_t rot, uint16_t startAngle=361, uint16_t endAngle=361);
};

class lvppDropdown : public lvppBase {
public:
    lvppDropdown(const char* fName, const char* pOptions=nullptr, lv_obj_t* parent=nullptr);
    void addOptions(const char* pOptions);
    void addOptions(std::vector<std::string> &options);
    void clearOptions(void);
    void setDropdownDirection(lv_dir_t dropDirection);
    u_int16_t getCurrentIndex() { return lv_dropdown_get_selected(obj); };
    void setCurrentIndex(uint16_t curInd);
    void getCurrentText(char* selStr, uint8_t selStrLen) { return lv_dropdown_get_selected_str(obj, selStr, selStrLen); };
protected:
};

class lvppCanvasFullColor : public lvppBase {
public:
    lvppCanvasFullColor(const char* fName, lv_coord_t x, lv_coord_t y, lv_coord_t w, lv_coord_t h, lv_color_t* providedBuffer=nullptr, lv_obj_t* parent=nullptr);
    virtual ~lvppCanvasFullColor();

    void setbgColor(lv_color_t bgColor);
    void drawPixel(lv_coord_t x, lv_coord_t y, lv_color_t color);
    void drawRect(lv_coord_t x, lv_coord_t y, lv_coord_t w, lv_coord_t h, 
        lv_coord_t borderThickness, lv_color_t borderColor, lv_color_t fillColor,  uint16_t radius, lv_opa_t opa);
    void drawLine(lv_coord_t x1, lv_coord_t y1, lv_coord_t x2, lv_coord_t y2, lv_coord_t width, lv_color_t color);
    void drawLabel(lv_coord_t x, lv_coord_t y, lv_coord_t maxW, lv_color_t color, const char* pText);
protected:
    lv_draw_rect_dsc_t*  pDscRect;
    lv_draw_line_dsc_t*  pDscLine;
    lv_draw_label_dsc_t* pDscLabel;
    lv_point_t twoPoints[2];
    lv_color_t* pBuffer;
};

class lvppCanvasIndexed : public lvppBase {
public:
    lvppCanvasIndexed(const char* fName, lv_coord_t x, lv_coord_t y, lv_coord_t w, lv_coord_t h, uint8_t colorDepth, lv_color_t* providedBuffer=nullptr, lv_obj_t* parent=nullptr);
    virtual ~lvppCanvasIndexed();
    bool addColorToIndex(lv_color_t col);
    bool addPaletteToIndex(lv_palette_t pal);
    void clearColorIndex();
    bool getIndexFromColor(lv_color_t col, lv_color_t& ind);
    void setbgColor(lv_color_t bgColor);
    void setbgColorByIndex(lv_color_t bgColorIndex);
    void drawPixel(lv_coord_t x, lv_coord_t y, lv_color_t color);
    void drawPixelByIndex(lv_coord_t x, lv_coord_t y, lv_color_t colorIndex);
    void drawLineVert(lv_coord_t x1, lv_coord_t y1, lv_coord_t h, lv_color_t color);
    void drawLineVertByIndex(lv_coord_t x1, lv_coord_t y1, lv_coord_t h, lv_color_t indexCol);
    void drawLineHoriz(lv_coord_t x1, lv_coord_t y1, lv_coord_t w, lv_color_t color);
    void drawLineHorizByIndex(lv_coord_t x1, lv_coord_t y1, lv_coord_t w, lv_color_t indexCol);
    void drawRectWithoutFill(lv_coord_t x1, lv_coord_t y1, lv_coord_t w, lv_coord_t h, 
        lv_color_t borderColor);
    void drawRectWithoutFillByIndex(lv_coord_t x1, lv_coord_t y1, lv_coord_t w, lv_coord_t h, 
        lv_color_t borderColorInd);
    void drawRectWithFill(lv_coord_t x1, lv_coord_t y1, lv_coord_t w, lv_coord_t h, 
        lv_color_t borderColor, lv_color_t fillColor);
    void drawRectWithFillByIndex(lv_coord_t x1, lv_coord_t y1, lv_coord_t w, lv_coord_t h, 
        lv_color_t borderColorInd, lv_color_t fillColorInd);
protected:
    uint16_t maxColorIndexesAllowed;
    uint8_t colorIndexesUsed;
#if LV_COLOR_DEPTH == 16
    std::map<uint16_t, uint8_t> colorToIndex;
#elif LV_COLOR_DEPTH == 8
    std::map<uint8_t, uint8_t> colorToIndex;
#endif
    lv_color_t* pBuffer;
};

