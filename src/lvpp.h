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

/** @file lvpp.h
 * @brief All derived classes for LVGL based class/widgets. This is the file you want to include.
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
 * @brief If you've activated this screen via activateScreen(), then this method will activate
 *        the screen which was active _prior_ to calling activateScreen(). If activation has not
 *        yet occured, then this function will have no effect.
 * 
 */
    void activatePriorScreen();
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
    lv_obj_t* pPriorScreen;     ///< Holder for prior screen when activateScreen() is called. Used in activatePriorScreen()
    std::vector<lvppBase*> objects; ///< Data structure used to hold all of the object pointers.
};

/** @class lvppSwitch
 * @brief On/Off switch widget
 * 
 * Switch which is either on or off and is either enabled or disabled.
 * 
 */
class lvppSwitch : public lvppBase {
public:
/**
 * @brief Construct a new lvpp Switch object with a friendly name and optionally a parent object.
 * 
 * @param fName friendly name
 * @param parent The lv_obj_t parent (optional)
 */
    lvppSwitch(const char* fName, lv_obj_t* parent=nullptr);
/**
 * @brief Set the switch to be enabled or disabled
 * 
 * @param bEnable defaults to true. False makes it a disabled widget unable to be changed by the user.
 */
    void setEnabled(bool bEnable=true);
/**
 * @brief Set the Checked State either on or off.
 * 
 * @param bChecked defaults to true. False turns the switch 'off'. True turns it to 'on' position.
 */
    void setCheckedState(bool bChecked=true);
/**
 * @brief Get the Checked State of the switch
 * 
 * @return true if switch is in the 'on' position
 * @return false if the switch is in the 'off' position
 */
    bool getCheckedState();
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
class lvppCycleButton : public lvppButton, public lvppOptions {
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
    void setOptions(const char* pOption);
/**
 * @brief Add button options using a vector of strings
 * 
 * @param inOptions Uses std::vector<std::string> reference to pass all the options.
 */
    void setOptions(std::vector<std::string> &inOptions);
/**
 * @brief Get the Current Index of which option is the active label on the button.
 * 
 * @return uint16_t zero to n-1 value representing the current label/button state.
 */
    uint16_t getSelectedIndex() { return currentIndex; };
/**
 * @brief Get the Current Text of the button label.
 * 
 * @return const char* current text of the label
 */
    const char* getSelectedText() { return options[currentIndex].c_str(); };
    void internalOnClicked();
protected:
/**
 * @brief This is the implementation of the pure virtual in lvppOptions. It is responsible
 *        for doing the actual class-specific 'set' - in this case just setting the button text
 *        to the option corresponding to the currentIndex.
 * 
 * @param pOpts newline-separated list of options. Unused for lvppCycleButton.
 */
    void lvOptionSetter(const char* pOpts) { setText(options[currentIndex].c_str()); };
/**
 * @brief This is the implementation of the pure virtual in lvppOptions. It is responsible
 *        for doing the actual class-specific 'get' - in this case just the currentIndex.
 * 
 */
    virtual uint16_t lvOptionGetIndex() { return currentIndex; };
    uint16_t currentIndex;              ///< Current value of which option is active on the button label.
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
 * @param img a pointer to an image descriptor. This is what the LVGL image converter tool produces in a .c file.
 */
    void setImage(const lv_img_dsc_t* pImg);
/**
 * @brief Set the Rotation angle of the image about the setPivot point.
 * 
 * @param rotTenthsOfDegrees Given in tenths of degrees - so 0-3600 for 360 degrees.
 */
    void setRotation(int16_t rotTenthsOfDegrees);
/**
 * @brief Set the Pivot Point for the image. Rotations will happen about this point.
 * 
 * @param xPivot, yPivot The coordinates of the pivot point of the images.
 */
    void setPivotPoint(lv_coord_t xPivot, lv_coord_t yPivot);
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
 * @param pText Starting text for the label. Can be changed subsequently. Default nullptr.
 * @param parent If provided, the parent of the object. This is a real LVGL lv_obj_t pointer
 */
    lvppLabel(const char* fName, const char* pText=nullptr, lv_obj_t* parent=nullptr);
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
 *        Utilizes multiple inheritance of lvppBase and lvppOptions.
 * 
 */
class lvppDropdown : public lvppBase, lvppOptions {
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
 * @brief Get the Index of the selected option.
 * 
 * @return uint16_t Value from 0-(n-1) of the selected option.
 */
    uint16_t getSelectedIndex() { return (uint16_t)lv_dropdown_get_selected(obj); };
/**
 * @brief Set the selected option manually/programmatically.
 * 
 * @param curInd Value from 0-(n-1) of the desired option.
 */
    void setSelectedIndex(uint16_t curInd);
/**
 * @brief Get the Text value of the currently selected option
 * 
 * @param selStr A pointer to a string where the current option string will be copied.
 * @param selStrLen The size of the selStr location to ensure a long option value won't overrun selStr memory.
 *                  Be sure to use the size of selStr MINUS one here to ensure you can fit a null terminator.
 */
    void getSelectedText(char* selStr, uint8_t selStrLen) { lv_dropdown_get_selected_str(obj, selStr, selStrLen); };
protected:
/**
 * @brief This is the implementation of the pure virtual in lvppOptions. It is responsible
 *        for doing the actual class-specific 'set' - in this case lv_dropdown_set_options().
 * 
 * @param pOpts newline-separated list of options.
 */
    void lvOptionSetter(const char* pOpts);
/**
 * @brief This is the implementation of the pure virtual in lvppOptions. It is responsible
 *        for doing the actual class-specific 'get' - in this case lv_dropdown_get_selected().
 * 
 */
    virtual uint16_t lvOptionGetIndex();
};

/**
 * @brief Construct a roller list widget for option selection
 * 
 */
class lvppRoller : public lvppBase, lvppOptions {
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
 * @brief Get the Index of the currently selected option.
 * 
 * @return uint16_t Value from 0-(n-1) of the selected option.
 */
    uint16_t getSelectedIndex() { return lv_roller_get_selected(obj); };
/**
 * @brief Set the selected option manually/programmatically.
 * 
 * @param curInd Value from 0-(n-1) of the desired option.
 */
    void setSelectedIndex(uint16_t curInd);
/**
 * @brief Get the Text value of the currently selected option
 * 
 * @param selStr A pointer to a string where the current option string will be copied.
 * @param selStrLen The size of the selStr location to ensure a long option value won't overrun selStr memory.
 *                  Be sure to use the size of selStr MINUS one here to ensure you can fit a null terminator.
 */
    void getSelectedText(char* selStr, uint8_t selStrLen) { lv_roller_get_selected_str(obj, selStr, selStrLen); };
protected:
/**
 * @brief This is the implementation of the pure virtual in lvppOptions. It is responsible
 *        for doing the actual class-specific 'set' - in this case lv_roller_set_options().
 * 
 * @param pOpts newline-separated list of options.
 */
    virtual void lvOptionSetter(const char* pOpts);
/**
 * @brief This is the implementation of the pure virtual in lvppOptions. It is responsible
 *        for doing the actual class-specific 'get' - in this case lv_roller_get_selected().
 * 
 */
    virtual uint16_t lvOptionGetIndex();
    // TODO: Add a vector of pair<int,string> and a map<int,int> indexToID and map<int,int> IDToIndex
    //       Then we can have a simple 'addOptions' where the user only can get the index or value out.
    //       But they can also addOptions(vector<pair<int,string>>) which then utilizes the map entries too.
    //       Might want to sort the options list prior to mapping everything?
};

/**
 * @brief Construct a canvas which uses FULL_COLOR rather than indexed color methods.
 * 
 * Canvas drawing is very powerful in LVGL, but only when a full color canvas is available.
 * In many smaller CPUs, full color isn't an option due to the sheer size of needing to have
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
/**
 * @brief Construct a new lvpp Canvas Full Color object
 * 
 * @param fName Internal object name. This is generally used in findObj() or lvppScreen
 * @param x,y Top-left starting point of the canvas on the display. 
 * @param w,h Width and height of the canvas referenced from x and y.
 * @param providedBuffer If provided, a non-null value will cause the constructor to _not_
 *                       allocate its own buffer. This is helpful when a series of screens
 *                       intend to share a common canvas with no drawing changes between them.
 * @param parent If provided, the parent of the object. This is a real LVGL lv_obj_t pointer
 */
    lvppCanvasFullColor(const char* fName, lv_coord_t x, lv_coord_t y, lv_coord_t w, lv_coord_t h, lv_color_t* providedBuffer=nullptr, lv_obj_t* parent=nullptr);
    virtual ~lvppCanvasFullColor();
/**
 * @brief Set the background color of the canvas.
 * 
 * @param bgColor The color to be used in painting the background. This is an lv_color_t from LVGL.
 */
    void setbgColor(lv_color_t bgColor);
/**
 * @brief Draw a single pixel on to the canvas.
 * 
 * @param x X location
 * @param y Y location
 * @param color Color to be drawn.
 */
    void drawPixel(lv_coord_t x, lv_coord_t y, lv_color_t color);
/**
 * @brief Draw a rectangle on the canvas
 * 
 * @param x,y Starting point of the rectangle
 * @param w,h Width and height of the rectangle
 * @param borderThickness Thickness of the rectangle in pixels.
 * @param borderColor Color of the rectangle's border lines.
 * @param fillColor Color to fill in the rectangle.
 * @param radius The outside radius for a non-square set of corners.
 * @param opa Opacity if opacity is enabled for your configuration.
 */
    void drawRect(lv_coord_t x, lv_coord_t y, lv_coord_t w, lv_coord_t h, 
        lv_coord_t borderThickness, lv_color_t borderColor, lv_color_t fillColor,  uint16_t radius, lv_opa_t opa);
/**
 * @brief Draw a line on the canvas
 * 
 * @param x1,y1 Starting point of the line to be drawn.
 * @param x2,y2 Ending point of the line to be drawn.
 * @param width Width of the line to be drawn in pixels.
 * @param color Color of the line to be drawn.
 */
    void drawLine(lv_coord_t x1, lv_coord_t y1, lv_coord_t x2, lv_coord_t y2, lv_coord_t width, lv_color_t color);
/**
 * @brief Draw a text label onto the canvas
 * 
 * @param x,y Starting point for the drawing of the text
 * @param maxW Maximum width in pixels of the drawing.
 * @param color Color of the text to be drawn
 * @param pText Text to be drawn
 * 
 * @todo This likely needs more thought. It takes no account for font sizes at a minimum.
 *       Not enough consideration has been given to _how_ this would be used quite yet.
 */
    void drawLabel(lv_coord_t x, lv_coord_t y, lv_coord_t maxW, lv_color_t color, const char* pText);
protected:
    lv_draw_rect_dsc_t*  pDscRect;      //< LVGL attribute for rectangle drawing
    lv_draw_line_dsc_t*  pDscLine;      //< LVGL attribute for line drawing
    lv_draw_label_dsc_t* pDscLabel;     //< LVGL attribute for label drawing
    lv_point_t twoPoints[2];            //< Supporting the two-point line draw
    lv_color_t* pBuffer;                //< Location of canvas color buffer if allocated internally.
    lv_coord_t width, height;   //< Width and Height of this canvas.
};

/**
 * @brief Construct a canvas which uses INDEXED color rather than FULL color methods.
 * 
 * Canvas drawing is very powerful in LVGL, but only when a full color canvas is available.
 * In many smaller CPUs, full color isn't an option due to the sheer size of needing to have
 * a buffer which is width * height * 4-bytes plus a bit more. LVGL has some great facilities
 * around drawing rectangles, lines, and labels with really nice options like rounded ends, 
 * line thicknesses, and other style attributes. Many of these functions simply do not work
 * in the indexed color world.
 * 
 * The underlying LVGL library allows for pixel drawing using an lv_color_t where the '.full'
 * member is not actually a color but instead is an index based on how many colors are in the
 * indexed color set. For instance, if you've chosen a 4-bit indexed color set, then the indexes
 * would run from 0-15 and it is up to the user to "set up" the indexed color set (palette)
 * prior to using any of those indexed values.
 * 
 * In lvppCanvasIndexed, some of the mystery and intricacy is taken out of the drawing and also
 * the class supports drawing of lines and rectangles in a more limited fashion than the native
 * full color canvas LVGL drawing routines.
 * 
 * To alleviate the complexity of keeping track of which color corresponds to which index value,
 * this class allows the user to utilize either lv_color_t values or index values to do the
 * drawing functions. The internals of the class work out the translation. This is accomplished
 * by the user adding colors and/or palettes to the object through addColorToIndex() and 
 * addPaletteToIndex(). These colors are then mapped to index values internally which then allows
 * the user to work in "colors" rather than indexes to colors, if they wish. 
 * 
 * While it was the original intention to have users utilize only _colors_ in this class, it was
 * revamped to allow drawing by index as well. It is up to the user to keep track of what color
 * is which index. And all of the drawing functions which are draw*Indexed() do take an lv_color_t
 * but this is done in the same manner that LVGL intends it. The ".full" member of that color
 * is actually the _index value_ and not a true color. While I didn't particularly like the
 * type conflict between a real color and an index, I felt it important to lean, again, to being
 * more similar to LVGL concepts than to make my own.
 * 
 * NOTE: If the user tries to draw using an lv_color_t color which is _not_ already defined in
 *       the indexed color set via addColorToIndex() or addPaletteToIndex(), the drawing requested
 *       will _not_ take place and there will be an `LV_LOG_WARN()` message created to note that
 *       the requested color was not in the indexed color set.
 * 
 * NOTE: The primative drawPixel() is likely 'more expensive' than drawLine() and drawRect()
 *       functions for situations where more than a few pixels are to be drawn. This is because
 *       the drawing is invalidated after every pixel draw in the primative case while in the
 *       case of lines and rects, the invalidation is only done once after all of the pixels are
 *       'laid down' in the drawing area. This is only a performance warning and not a functional
 *       one.
 */
class lvppCanvasIndexed : public lvppBase {
public:
/**
 * @brief Construct a new lvpp Canvas Full Color object
 * 
 * @param fName Internal object name. This is generally used in findObj() or lvppScreen
 * @param x,y Top-left starting point of the canvas on the display. 
 * @param w,h Width and height of the canvas referenced from x and y.
 * @param colorDepth How many _bits_ of color depth shall be used for this canvas. Valid values
 *                   would be 1, 2, 4, or 8. For instance, a 4-bit color depth would have 16 colors
 *                   available in the indexed color set. NOTE: If the user passes in a providedBuffer,
 *                   it is incumbent upon that user to ensure this colorDepth and the size of that
 *                   buffer are not in conflict with each other or _bad things_ could occur.
 * @param providedBuffer If provided, a non-null value will cause the constructor to _not_
 *                       allocate its own buffer. This is helpful when a series of screens
 *                       intend to share a common canvas with no drawing changes between them.
 * @param parent If provided, the parent of the object. This is a real LVGL lv_obj_t pointer
 */
    lvppCanvasIndexed(const char* fName, lv_coord_t x, lv_coord_t y, lv_coord_t w, lv_coord_t h, uint8_t colorDepth, lv_color_t* providedBuffer=nullptr, lv_obj_t* parent=nullptr);
    virtual ~lvppCanvasIndexed();
/**
 * @brief Add a single color to the indexed color set
 * 
 * @param col Color to add to the index.
 * @return true If there was room to add a new color or if the color was already in the index.
 * @return false If the index is already full based on the indexed color depth of the canvas.
 */
    bool addColorToIndex(lv_color_t col);
/**
 * @brief Add a full palette of color to the indexed color set. This is 10 colors.
 * 
 * The concept of palette used here is that of LVGL. I felt the palette concept was a powerful
 * one and wanted to support it here, easily. Adding a color palette will effectively add
 * (4) darker palette colors, (1) main palette color, and (5) lighter palette colors.
 * These figures come from the lv_palette_darken(), lv_palette_main() and lv_palette_lighten()
 * functions.
 * 
 * @param pal Palette (not color) to add to the index. This is generally done by the use of
 *            LV_PALETTE_BLUE, LV_PALETTE_TEAL, LV_PALETTE_BROWN, etc.
 * @return true If there was room to add a new color or if the color was already in the index.
 * @return false If the index is already full based on the indexed color depth of the canvas.
 */
    bool addPaletteToIndex(lv_palette_t pal);
/**
 * @brief Remove all colors from the indexed color set.
 * 
 */
    void clearColorIndex();
/**
 * @brief Get the Index of a single Color.
 * 
 * @param col Color being requested for translation
 * @param ind Reference to the index which will be returned (if successful)
 * @return true If the color col is found.
 * @return false If the color col is not found in the indexed color set.
 */
    bool getIndexFromColor(lv_color_t col, lv_color_t& ind);
/**
 * @brief Set the background color of the canvas.
 * 
 * @param bgColor The color to be used in painting the background. This is an lv_color_t from LVGL.
 */
    void setbgColor(lv_color_t bgColor);
/**
 * @brief Set the backgrond color from an _index_ color
 * 
 * @param bgColorIndex The _index_ of the color to use. This is _not a true color_ but only an index.
 */
    void setbgColorByIndex(lv_color_t bgColorIndex);
/**
 * @brief Draw a single pixel on to the canvas.
 * 
 * @param x X location
 * @param y Y location
 * @param color Color to be drawn.
 */
    void drawPixel(lv_coord_t x, lv_coord_t y, lv_color_t color);
/**
 * @brief Draw a single pixel on to the canvas using an _index_ color.
 * 
 * @param x X location
 * @param y Y location
 * @param colorIndex The _index_ of the color to use. This is _not a true color_ but only an index.
 */
    void drawPixelByIndex(lv_coord_t x, lv_coord_t y, lv_color_t colorIndex);
/**
 * @brief Draw a verical line on the canvas
 * 
 * @param x1,y1 Starting point of the line to be drawn.
 * @param h Height of the line to be drawn.
 * @param color Color of the line to be drawn.
 */
    void drawLineVert(lv_coord_t x1, lv_coord_t y1, lv_coord_t h, lv_color_t color);
/**
 * @brief Draw a verical line on the canvas using an _index_ color.
 * 
 * @param x1,y1 Starting point of the line to be drawn.
 * @param h Height of the line to be drawn.
 * @param indexCol The _index_ of the color to use. This is _not a true color_ but only an index.
 */
    void drawLineVertByIndex(lv_coord_t x1, lv_coord_t y1, lv_coord_t h, lv_color_t indexCol);
/**
 * @brief Draw a horizontal line on the canvas
 * 
 * @param x1,y1 Starting point of the line to be drawn.
 * @param w Width of the line to be drawn.
 * @param color Color of the line to be drawn.
 */
    void drawLineHoriz(lv_coord_t x1, lv_coord_t y1, lv_coord_t w, lv_color_t color);
/**
 * @brief Draw a horizontal line on the canvas using an _index_ color.
 * 
 * @param x1,y1 Starting point of the line to be drawn.
 * @param w Width of the line to be drawn.
 * @param indexCol The _index_ of the color to use. This is _not a true color_ but only an index.
 */
    void drawLineHorizByIndex(lv_coord_t x1, lv_coord_t y1, lv_coord_t w, lv_color_t indexCol);
/**
 * @brief Draw a rectangle on the canvas without filling in the rectangle.
 * 
 * @param x1,y1 Starting point of the rectangle
 * @param w,h Width and height of the rectangle
 * @param borderColor Color of the rectangle's border lines.
 */
    void drawRectWithoutFill(lv_coord_t x1, lv_coord_t y1, lv_coord_t w, lv_coord_t h, 
        lv_color_t borderColor);
/**
 * @brief Draw a rectangle on the canvas without filling in the rectangle using an _index_ color.
 * 
 * @param x1,y1 Starting point of the rectangle
 * @param w,h Width and height of the rectangle
 * @param borderColorInd The _index_ of the color to use. This is _not a true color_ but only an index.
 */
    void drawRectWithoutFillByIndex(lv_coord_t x1, lv_coord_t y1, lv_coord_t w, lv_coord_t h, 
        lv_color_t borderColorInd);
/**
 * @brief Draw a rectangle on the canvas and fill in the center of the rectangle.
 * 
 * @param x1,y1 Starting point of the rectangle
 * @param w,h Width and height of the rectangle
 * @param borderColor Color of the rectangle's border lines.
 * @param fillColor Color to fill in the rectangle.
 */
    void drawRectWithFill(lv_coord_t x1, lv_coord_t y1, lv_coord_t w, lv_coord_t h, 
        lv_color_t borderColor, lv_color_t fillColor);
/**
 * @brief Draw a rectangle on the canvas and fill in the center of the rectangle. This uses _index_ colors.
 * 
 * @param x1,y1 Starting point of the rectangle
 * @param w,h Width and height of the rectangle
 * @param borderColorInd The _index_ of the color to use. This is _not a true color_ but only an index.
 * @param fillColorInd The _index_ of the color to use. This is _not a true color_ but only an index.
 */
    void drawRectWithFillByIndex(lv_coord_t x1, lv_coord_t y1, lv_coord_t w, lv_coord_t h, 
        lv_color_t borderColorInd, lv_color_t fillColorInd);
/**
 * @brief Draw a _centered_ rectangle on the canvas. Will have xBorder/yBorder around it.
 * 
 * @param xBorder,yBorder The border size on each x-side and each y-side of the rectangle
 * @param borderColor Color of the rectangle's border lines.
 */
    void drawCenteredRectWithoutFill(lv_coord_t xBorder, lv_coord_t yBorder, lv_color_t borderColor);
/**
 * @brief Draw a _centered_ rectangle on the canvas.
 *        Will have xBorder/yBorder around it. This uses _index_ colors.
 * 
 * @param xBorder,yBorder The border size on each x-side and each y-side of the rectangle
 * @param borderColorInd The _index_ of the color to use. This is _not a true color_ but only an index.
 */
    void drawCenteredRectWithoutFillByIndex(lv_coord_t xBorder, lv_coord_t yBorder, lv_color_t borderColorInd);
/**
 * @brief Draw a _centered_ rectangle on the canvas and fill in the rectangle. Will have xBorder/yBorder around it.
 * 
 * @param xBorder,yBorder The border size on each x-side and each y-side of the rectangle
 * @param borderColor Color of the rectangle's border lines.
 * @param fillColor Color to fill in the rectangle.
 */
    void drawCenteredRectWithFill(lv_coord_t xBorder, lv_coord_t yBorder, 
        lv_color_t borderColor, lv_color_t fillColor);
/**
 * @brief Draw a _centered_ rectangle on the canvas and fill in the rectangle. 
 *        Will have xBorder/yBorder around it. This uses _index_ colors.
 * 
 * @param xBorder,yBorder The border size on each x-side and each y-side of the rectangle
 * @param borderColorInd The _index_ of the color to use. This is _not a true color_ but only an index.
 * @param fillColorInd The _index_ of the color to use. This is _not a true color_ but only an index.
 */
    void drawCenteredRectWithFillByIndex(lv_coord_t xBorder, lv_coord_t yBorder, 
        lv_color_t borderColorInd, lv_color_t fillColorInd);
protected:
    uint16_t maxColorIndexesAllowed;    ///< Number of color indexes available based on the color depth at creation time.
    uint8_t colorIndexesUsed;           ///< How many colors have presently been used in the indexed color set.
///@todo This use of LV_COLOR_DEPTH is believed to be correct at least for 8 and 16, but is this _complete_ ?
#if LV_COLOR_DEPTH == 16
    std::map<uint16_t, uint8_t> colorToIndex;
#elif LV_COLOR_DEPTH == 8
    std::map<uint8_t, uint8_t> colorToIndex;
#endif
    lv_color_t* pBuffer;    //< Location of canvas color buffer if allocated internally.
    lv_coord_t width, height;   //< Width and Height of this canvas.
};
