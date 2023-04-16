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
 * @class lvppScreen
 * @brief The lvppScreen is a non - lvppBase object for organizing the use of multiple 
 *        screens in a project.
 * 
 * The purpose is to aid in organized grouping, control, use of widgets that are all 
 * on the same screen. The notion of a screen is identical to that of lvgl. As such, 
 * the root of this class holds an lv_obj_t poitner to the screen.
 * The concept is to have the lvppScreen object created, then create widgets that
 * are added to the lvppScreen with addObject(). Now some interaction can be had with
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
 *        which are added via addObject()
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
    void addObject(lvppBase* pObj);
/**
 * @brief Get a pointer to the LVGL lv_obj_t screen object.
 * 
 * @return lv_obj_t* pointer to an LVGL base object.
 */
    lv_obj_t* getScreen() { return pScreen; };
    //
    // LV_SCR_LOAD_ANIM_OUT_RIGHT
/**
 * @brief Load/Activate this screen either with or without animation based on parameters given.
 * 
 * If activateScreen() is called with no parameters, then the underlying load screen call is
 * lv_scr_load(). The two optional parameters give the user the ability to animate the new
 * screen utilizing the lv_scr_load_anim() which allows an animation 'time' as well as an
 * animation 'type'. 
 * 
 * @param anim_time Number of milliseconds spent doing the animated load of the screen.
 * @param anim The animation type. This is one of many LV_SCR_LOAD_ANIM_[OVER|MOVE|FADE|OUT]_[LEFT|RIGHT|TOP|BOTTOM]
 */
    void activateScreen(uint32_t anim_time=0, lv_scr_load_anim_t anim=LV_SCR_LOAD_ANIM_NONE);
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
 * @brief Obtain a pointer to an object which was prior added to the lvppScreen by addObject().
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

/** @class lvppButton
 * @brief Basic button class
 * 
 * LVGL button which is auto-sized, initially, based on the pText of the button label.
 * 
 */
class lvppButton : public lvppBase {
public:
/**
 * @brief Construct a new lvpp Button object
 * 
 * @param fName Button's internal name. This is generally used in findObj() or lvppScreen
 * @param pText This is the starting text of the button's label, if provided.
 * @param parent If provided, the parent of the button object. This is a real LVGL lv_obj_t pointer
 */
    lvppButton(const char* fName, const char* pText=nullptr, lv_obj_t* parent=nullptr);
};

//
// Button has multiple "values"
// Each click cycles forward through the list of values.
//
/** @class lvppCycleButton
 * @brief Derived from lvppButton, this is a button who's clicks cycle through a list of options.
 * 
 * Options can be added in bulk by char* or vector<string> and the label cycles based on its current state.
 * Current index or value/text can be obtained at any time.
 * 
 */
class lvppCycleButton : public lvppButton {
public:
/**
 * @brief Construct a new lvpp Cycle Button object
 * 
 * @param fName Button's internal name. This is generally used in findObj() or lvppScreen
 * @param parent If provided, the parent of the button object. This is a real LVGL lv_obj_t pointer
 */
    lvppCycleButton(const char* fName, lv_obj_t* parent=nullptr);
/**
 * @brief Remove all options from the current options list.
 * 
 */
    void clearOptions(void);
/**
 * @brief Add button options using a single char* with options separated by `\n`
 * 
 * @param pOption Pointer to a character string of options separated by `\n`
 */
    void addOptions(const char* pOption);
/**
 * @brief Add button options using a vector of strings
 * 
 * @param inOptions Uses std::vector<std::string> reference to pass all the options.
 */
    void addOptions(std::vector<std::string> &inOptions);
/**
 * @brief Get the Current Index of which option is the active label on the button.
 * 
 * @return u_int16_t zero to n-1 value representing the current label/button state.
 */
    u_int16_t getCurrentIndex() { return currentIndex; };
/**
 * @brief Get the Current Text of the button label.
 * 
 * @return const char* current text of the label
 */
    const char* getCurrentText() { return options[currentIndex].c_str(); };
    void internalOnClicked();
protected:
    std::vector<std::string> options;   ///< Internal representation of the options for the cycling button.
    uint16_t currentIndex;              ///< Current value of which option is active on the button label.
    uint16_t quantity;                  ///< Total number of options.
};

/** @class lvppFullImageToggleButton
 * @brief Button which is all image based and simply toggles on/off between the images.
 * 
 */
class lvppFullImageToggleButton : public lvppBase {
public:
/**
 * @brief Construct a new lvpp Full Image Toggle Button without providing either image
 * 
 * @param fName Button's internal name. This is generally used in findObj() or lvppScreen
 * @param parent If provided, the parent of the button object. This is a real LVGL lv_obj_t pointer
 */
    lvppFullImageToggleButton(const char* fName, lv_obj_t* parent=nullptr);
/**
 * @brief Construct a new lvpp Full Image Toggle Button providing both images
 * 
 * @param fName Button's internal name. This is generally used in findObj() or lvppScreen
 * @param uncheckedImg This is a pre-prepared lv_img_dsc_t pointing to the deselected/unchecked image
 * @param checkedImg This is a pre-prepared lv_img_dsc_t pointing to the selected/checked image
 * @param parent If provided, the parent of the button object. This is a real LVGL lv_obj_t pointer
 */
    lvppFullImageToggleButton(const char* fName, lv_img_dsc_t uncheckedImg, lv_img_dsc_t checkedImg, lv_obj_t* parent=nullptr);
    ~lvppFullImageToggleButton();
/**
 * @brief Set the Image Source for the unchecked image
 * 
 * @param img This is a pre-prepared lv_img_dsc_t pointing to the deselected/unchecked image
 */
    void setImageSourceUnChecked(const lv_img_dsc_t img);
/**
 * @brief Set the Image Source for the checked image
 * 
 * @param img This is a pre-prepared lv_img_dsc_t pointing to the selected/checked image
 */
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

/**
 * @brief Object for loading and displaying an image on a screen
 * 
 */
class lvppImage : public lvppBase {
public:
/**
 * @brief Construct a new lvpp Image object
 * 
 * @param fName Internal object name. This is generally used in findObj() or lvppScreen
 * @param parent If provided, the parent of the object. This is a real LVGL lv_obj_t pointer
 */
    lvppImage(const char* fName, lv_obj_t* parent=nullptr);
    ~lvppImage();
/**
 * @brief Set the Image to be displayed. This is a pre-prepared lv_img_dsc_t directly from LVGL.
 * 
 * @param img 
 */
    void setImage(lv_img_dsc_t img);
};

/**
 * @brief Enable the display of a text item
 * 
 */
class lvppLabel : public lvppBase {
public:
/**
 * @brief Construct a new lvpp Label object
 * 
 * @param fName Internal object name. This is generally used in findObj() or lvppScreen
 * @param pText Starting text for the label. Can be changed subsequently.
 * @param parent If provided, the parent of the object. This is a real LVGL lv_obj_t pointer
 */
    lvppLabel(const char* fName, const char* pText, lv_obj_t* parent=nullptr);
/**
 * @brief Set the Text value to display
 * 
 * @param pText pointer to a char string for the new value/contents of the label.
 */
    virtual void setText(const char* pText);
/**
 * @brief Set the Text Color of the label (not the background)
 * 
 * @param newColor This is the new color to be used for the label. It is an LVGL lv_color_t construct.
 *        Examples would be lv_color_black() or lv_palette_main(LV_COLOR_BLUE) or custom created colors.
 */
    void setTextColor(lv_color_t newColor);
};

/**
 * @brief Create a 'bar' widget from LVGL and allow range and value to be used.
 * 
 * This is based not on lvppBase but on lvppBaseWithValue as are a few other widget types.
 * 
 */
class lvppBar : public lvppBaseWithValue {
public:
/**
 * @brief Construct a new lvpp Bar object in the same way as most widget creations.
 * 
 * @param fName Internal object name. This is generally used in findObj() or lvppScreen
 * @param parent If provided, the parent of the object. This is a real LVGL lv_obj_t pointer
 */
    lvppBar(const char* fName, lv_obj_t* parent=nullptr);
/**
 * @brief Set the Range of your new bar widget
 * 
 * @param range_min Minimum value for the bar
 * @param range_max Maximum value for the bar
 */
    void setRange(int16_t range_min, int16_t range_max);
/**
 * @brief Set the Value of your bar
 * 
 * @param value New value for the bar widget
 * @param animate Whether or not to animate from current to new position. Default is true.
 */
    void setValue(int16_t value, bool animate=true);
};

/**
 * @brief Construct a new Slider widget (similar to a bar in LVGL parlance).
 * 
 */
class lvppSlider : public lvppBaseWithValue {
public:
/**
 * @brief Construct a new lvpp Slider widget
 * 
 * @param fName Internal object name. This is generally used in findObj() or lvppScreen
 * @param parent If provided, the parent of the object. This is a real LVGL lv_obj_t pointer
 */
    lvppSlider(const char* fName, lv_obj_t* parent=nullptr);
/**
 * @brief Set the Range of your new slider widget
 * 
 * @param range_min Minimum value for the slider
 * @param range_max Maximum value for the slider
 */
    void setRange(int16_t range_min, int16_t range_max);
/**
 * @brief Set the Value of your slider
 * 
 * @param value New value for the slider widget
 * @param animate Whether or not to animate from current to new position. Default is true.
 */
    void setValue(int16_t value, bool animate=true);
};

/**
 * @brief Create an Arc widget - also based on lvppBaseWithValue like Bar and Slider
 * 
 */
class lvppArc : public lvppBaseWithValue {
public:
/**
 * @brief Construct a new lvpp Arc widget
 * 
 * @param fName Internal object name. This is generally used in findObj() or lvppScreen
 * @param parent If provided, the parent of the object. This is a real LVGL lv_obj_t pointer
 */
    lvppArc(const char* fName, lv_obj_t* parent=nullptr);
/**
 * @brief Set the Arc Color - can be useful for custom values/ranges corresponding to different visual representations.
 * 
 * @param newColor The new color to use represented in LVGP lv_color_t typedef.
 */
    void setArcColor(lv_color_t newColor);
/**
 * @brief Set the Range of your new arc widget
 * 
 * @param range_min Minimum value for the arc
 * @param range_max Maximum value for the arc
 */
    void setRange(int16_t range_min, int16_t range_max);
/**
 * @brief Set the Value of your arc
 * 
 * @param value New value for the arc widget
 * @param animate Whether or not to animate from current to new position. Default is true.
 */
    void setValue(int16_t value, bool animate=true);
/**
 * @brief Set the Arc Rotation And Sweep amounts.
 * 
 * The arc's rotation is about setting what '0 degrees' is visually. The default rotation
 * sets '0' to be at 3-o'clock on a clock (yes this seems odd and I thought about changing
 * it to be 12-o'clock but felt this would confuse folks who know LVGL concepts). So, an
 * arc which has '0' at the bottom would be rotated by 90 degrees (from 3-o'clock to 6-o'clock)
 * 
 * The arc is a circle which is either "full" (sweep=0-360) or it is an open/broken circle
 * which has a sweep encompassing angles greater than 0 and less than 360. These angles are
 * only the visual representation of the arc and are not about the _value_ and _range_. For
 * example, an arc which is 3/4 of a circle could have a sweep of 0-270 or it could be 10-280.
 * It seems it would be typical to rotate an arc by an amount to where you want the lowest
 * value to start and to always make the sweep start at zero. But this isn't required. One
 * could rotate 90 degrees and have a sweep that starts at 15 degrees which would effectly
 * be the same as rotating by 90+15 and starting the sweep at 0.
 * 
 * @param rot Angle to rotate from the 3-o'clock position clockwise.
 * @param startAngle Sweep of the arc should start at this angle offset from the rotation amount.
 * @param endAngle Sweep of the arc should end at this angle offset from the rotation amount.
 */
    void setArcRotationAndSweep(uint16_t rot, uint16_t startAngle=361, uint16_t endAngle=361);
};

/**
 * @brief Support for drop-down lists and choosing from a dropdown.
 * 
 */
class lvppDropdown : public lvppBase {
public:
/**
 * @brief Construct a new lvpp Dropdown object and optionially give its starting options list.
 * 
 * @param fName Internal object name. This is generally used in findObj() or lvppScreen
 * @param pOptions Optional char pointer with a list of options which are separated by `\n`
 * @param parent If provided, the parent of the object. This is a real LVGL lv_obj_t pointer
 */
    lvppDropdown(const char* fName, const char* pOptions=nullptr, lv_obj_t* parent=nullptr);
/**
 * @brief sets the complete list of options by way of a char pointer with options separated by `\n`
 * 
 * @param pOptions pointer to a string of options separated by `\n`
 */
    void setOptions(const char* pOptions);
/**
 * @brief sets the complete list of options by way of a reference to a vector of std::string values
 * 
 * @param options A reference to a std::vector of std::string values.
 */
    void setOptions(std::vector<std::string> &options);
/**
 * @brief Removes all options from the dropdown.
 * 
 */
    void clearOptions(void);
/**
 * @brief Set the Dropdown Direction. Depending on the screen location of the drop-down,
 *        it may be desirable to have the dropdown "open" above rather than below the widget location.
 * 
 * @param dropDirection An lv_dir_t value such as LV_DIR_BOTTOM, LV_DIR_TOP.
 */
    void setDropdownDirection(lv_dir_t dropDirection);
/**
 * @brief Get the Current Index of the selected option.
 * 
 * @return u_int16_t Value from 0-(n-1) of the selected option.
 */
    u_int16_t getCurrentIndex() { return lv_dropdown_get_selected(obj); };
/**
 * @brief Set the Current selected option manually/programmatically.
 * 
 * @param curInd Value from 0-(n-1) of the desired option.
 */
    void setCurrentIndex(uint16_t curInd);
/**
 * @brief Get the Current Text value of the currently selected option
 * 
 * @param selStr A pointer to a string where the current option string will be copied.
 * @param selStrLen The size of the selStr location to ensure a long option value won't overrun selStr memory.
 *                  Be sure to use the size of selStr MINUS one here to ensure you can fit a null terminator.
 */
    void getCurrentText(char* selStr, uint8_t selStrLen) { return lv_dropdown_get_selected_str(obj, selStr, selStrLen); };
protected:
};

/**
 * @brief Construct a roller list widget for option selection
 * 
 */
class lvppRoller : public lvppBase {
public:
/**
 * @brief Construct a new lvpp Roller object
 * 
 * @param fName Internal object name. This is generally used in findObj() or lvppScreen
 * @param pOptions Optional char pointer with a list of options which are separated by `\n`
 * @param parent If provided, the parent of the object. This is a real LVGL lv_obj_t pointer
 */
    lvppRoller(const char* fName, const char* pOptions=nullptr, lv_obj_t* parent=nullptr);
/**
 * @brief sets the complete list of options by way of a char pointer with options separated by `\n`
 * 
 * @param pOptions pointer to a string of options separated by `\n`
 */
    void setOptions(const char* pOptions);
/**
 * @brief sets the complete list of options by way of a reference to a vector of std::string values
 * 
 * @param options A reference to a std::vector of std::string values.
 */
    void setOptions(std::vector<std::string> &options);
/**
 * @brief Removes all options from the roller.
 * 
 */
    void clearOptions(void);
/**
 * @brief Get the Current Index of the selected option.
 * 
 * @return u_int16_t Value from 0-(n-1) of the selected option.
 */
    u_int16_t getCurrentIndex() { return lv_roller_get_selected(obj); };
/**
 * @brief Set the Current selected option manually/programmatically.
 * 
 * @param curInd Value from 0-(n-1) of the desired option.
 */
    void setCurrentIndex(uint16_t curInd);
/**
 * @brief Get the Current Text value of the currently selected option
 * 
 * @param selStr A pointer to a string where the current option string will be copied.
 * @param selStrLen The size of the selStr location to ensure a long option value won't overrun selStr memory.
 *                  Be sure to use the size of selStr MINUS one here to ensure you can fit a null terminator.
 */
    void getCurrentText(char* selStr, uint8_t selStrLen) { return lv_roller_get_selected_str(obj, selStr, selStrLen); };
protected:
    // TODO: Add a vector of pair<int,string> and a map<int,int> indexToID and map<int,int> IDToIndex
    //       Then we can have a simple 'addOptions' where the user only can get the index or value out.
    //       But they can also addOptions(vector<pair<int,string>>) which then utilizes the map entries too.
    //       Might want to sort the options list prior to mapping everything?
};

/**
 * @brief Construct a canvas which uses FULL_COLOR rather than indexed color methods.
 * 
 * Canvas drawing is very powerful in LVGL, but only when a full color canvas is available.
 * In many smaller micros, full color isn't an option due to the sheer size of needing to have
 * a buffer which is width * height * 4-bytes plus a bit more. LVGL has some great facilities
 * around drawing rectangles, lines, and labels with really nice options like rounded ends, 
 * line thicknesses, and other style attributes. Many of these functions simply do not work
 * in the indexed color world. So, this class implements the full color functions while the
 * lvppCanvasIndexed implements a stripped down version of these functions within the reduced
 * color set. See lvppCanvasIndexed for more detail on what's provided and how it works.
 * 
 */
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

