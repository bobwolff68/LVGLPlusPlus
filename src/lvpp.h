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

/**
 * @file lvpp.h
 * @brief Main header file for all non-base object definitions.
 * 
 */

/** \mainpage LVGLPlusPlus aspires to put a low-cost C++ wrapper around the LVGL library.
 *            It will not support every feature of LVGL as the feature-breadth of the
 *            full library is extensive and growing. However, it also does not preclude
 *            the co-mingling of this C++ wrapper usage and the underlying library.
 *            The user is free to get handles to pointers and use them directly in lv_* calls.
 *
 * \section intro_sec Introduction
 *
 * 
*/

/**
 * @class lvppScreen
 * @brief The lvppScreen is a non - lvppBase object for organizing the use of multiple 
 *        screens in a project.
 * 
 * The purpose is to aid in organized grouping, control, use of widgets that are all 
 * on the same screen. The notion of a screen is identical to that of lvgl. As such, 
 * the root of this class holds an lv_obj_t poitner to the screen.
 * The concept is to have the lvppScreen object created, then create widgets that
 * are added to the lvppScreen with AddObject(). Now some interaction can be had with
 * those objects by either getting the object pointer via findObj() utilizing the
 * object name given to the widget at instantiation time, or, in some cases, actually
 * changing the value of the object via setObjValue(), again using the object name
 * and giving a value. The plan is to expand the setObjValue() to encompass several
 * types of objects and also have a getter too.
*/
/// @todo Add full compliment of object types to setObjValue
/// @todo Add a getter similar to setObj* as getObj*()
class lvppScreen { 
public:
/**
 * @brief Construct a new lvpp Screen object. The default constructor will allocate a new
 *        screen object and will use this screen object as the 'parent' screen to any objects
 *        which are added via AddObject()
 * 
 * @param _pInitScreen The user can pass in an already allocated screen from lvgl if desired.
 */
    lvppScreen(lv_obj_t* _pInitScreen=nullptr);
    ~lvppScreen() { if (pScreen) lv_obj_del(pScreen); };
/**
 * @brief Add any lvppBase object pointer to the screen to keep track of.
 *
 * When an lvppBase derived object is created, the first parameter given to the creation
 * is an object name. This object name will be used in finding/locating and performing any
 * action on that object inside the screen. The purpose here is to allow the user to not
 * have to keep track of every single object pointer but only need to keep track of the
 * names of the objects that will need to be changed or used. In this way, the lvppScreen
 * pointers are the only main objects to keep track of along with the object names.
 *  
 * @param pObj Pointer to an lvppBase derived object to have the screen keep track of.
 *
 */
    void AddObject(lvppBase* pObj);
/**
 * @brief Get a pointer to the LVGL lv_obj_t screen object.
 * 
 * @return lv_obj_t* pointer to an LVGL base object.
 */
    lv_obj_t* GetScreen() { return pScreen; };
    //
    // LV_SCR_LOAD_ANIM_OUT_RIGHT
/**
 * @brief Load/Activate this screen either with or without animation based on parameters given.
 * 
 * If ActivateScreen() is called with no parameters, then the underlying load screen call is
 * lv_scr_load(). The two optional parameters give the user the ability to animate the new
 * screen utilizing the lv_scr_load_anim() which allows an animation 'time' as well as an
 * animation 'type'. 
 * 
 * @param anim_time Number of milliseconds spent doing the animated load of the screen.
 * @param anim The animation type. This is one of many LV_SCR_LOAD_ANIM_[OVER|MOVE|FADE|OUT]_[LEFT|RIGHT|TOP|BOTTOM]
 */
    void ActivateScreen(uint32_t anim_time=0, lv_scr_load_anim_t anim=LV_SCR_LOAD_ANIM_NONE);
/**
 * @brief Defeats the automatic use of scrollbars by LVGL when certain drawing operations might cause them to appear.
 * 
 * When certain drawing operations take place near or beyond the X/Y borders, LVGL will make
 * the decision to show scroll bars left/right or up/down. For instance, when drawing using
 * lvppLine and a line width greater than 1, drawing to the edge of the screen will cause
 * scroll bars to appear. This function will defeat that behavior and allow such drawing
 * to the edge of the screen.
 * 
 */
    void disableScrollBars();
/**
 * @brief Obtain a pointer to an object which was prior added to the lvppScreen by AddObject().
 * 
 * lvppBase derived objects all have an object name at their creation. This name is used to
 * allow lvppScreen to 'find' an object and pass back the base pointer to the caller. It is up
 * to the caller to know what type of object this is and appropriately cast it to the non-lvppBase
 * type for further use. lvppBase does also have dynamic functions to request what 'type' an
 * object is to make this easier in the absence of RTTI / dynamic_cast<> functionality.
 * 
 * @param pName Pointer to the object name being requested.
 * @return lvppBase* A pointer to the object found or nullptr when the object name was not
 *         in the list of objects added to lvppScreen.
 */
    lvppBase* findObj(const char* pName);
/**
 * @brief Set the value of the object objName to val if found.
 * 
 * @param objName Name of the object we're looking for in the list of objects in the lvppScreen.
 * @param val Value to utilize in 'setting' the value once the object is found.
 * @return If the object is found, returns true. Else false.
 */
    bool setObjValue(const char* objName, int16_t val);
/**
 * @todo Function incomplete and needs implementation.
 * 
 */
    void setObjText(std::string str);
protected:
    lv_obj_t* pScreen=nullptr; ///< The actual underlying LVGL screen pointer.
    std::vector<lvppBase*> objects; ///< Data structure used to hold all of the object pointers.
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

class lvppRoller : public lvppBase {
public:
    lvppRoller(const char* fName, const char* pOptions=nullptr, lv_obj_t* parent=nullptr);
    void addOptions(const char* pOptions);
    void addOptions(std::vector<std::string> &options);
    void clearOptions(void);
    u_int16_t getCurrentIndex() { return lv_dropdown_get_selected(obj); };
    void setCurrentIndex(uint16_t curInd);
    void getCurrentText(char* selStr, uint8_t selStrLen) { return lv_dropdown_get_selected_str(obj, selStr, selStrLen); };
protected:
    // TODO: Add a vector of pair<int,string> and a map<int,int> indexToID and map<int,int> IDToIndex
    //       Then we can have a simple 'addOptions' where the user only can get the index or value out.
    //       But they can also addOptions(vector<pair<int,string>>) which then utilizes the map entries too.
    //       Might want to sort the options list prior to mapping everything?
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

