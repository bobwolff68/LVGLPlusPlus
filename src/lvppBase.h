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
#ifndef _LVPPBASE_H
#define _LVPPBASE_H

#include <string>
#include <functional>

#ifdef LV_LVGL_H_INCLUDE_SIMPLE
#include "lvgl.h"
#else
#include "lvgl/lvgl.h"
#endif

/** @file lvppBase.h
 * @brief Base class for LVGLPlusPlus widgets (lvppBase)
 * 
 */

static std::string eventNames[_LV_EVENT_LAST];  ///< Place to store textual versions of event types.

/**
 * @brief lvppBase is the root of the C++ library LVGLPlusPlus. It strives to create consistent
 *        ways to manipulate a variety of LVGL-based widgets in a consistent manner - hiding the
 *        details away from the user where possible.
 * 
 * The topmost concepts for lvppBase are:
 * - Having a handle to an LVGL object 'obj' which gets created (and named) for each widget.
 * - Event handling for each widget is handled by the class and callbacks are used to give
 *   the user the ability to take action on changes or events.
 * - Lambda functions are used in some cases to allow users to utilize the widget class
 *   without having to derive their own class from it for the sole purpose of a callback.
 * - A main 'label' is defined for all base widgets.
 * - An options 'adjacent label' can be used to enable a separate label that acts (often)
 *   as a label for the item. Think of a dropdown list that needs a nearby label to tell
 *   the user what this dropdown is intended for.
 * - Font setting
 * - Background color setting
 * - Size and location/alignment of the widget
 * - Setting of text for the main label as well as ancillary labels.
 * 
 */
class lvppBase {
public:
/**
 * @brief Construct a new lvpp Base object
 * 
 * @param fName Internal object name. This is generally used in findObj() or lvppScreen
 * @param oType The type of the object being created. This is primarily internal, but
 *              can be used to know the category/type of the object and can be obtained
 *              via getObjType() if needed.
 */
    lvppBase(const char* fName, const char* oType);
    ~lvppBase();
/**
 * @brief Create the handlers and store the newly created object.
 * 
 * The pattern here is that any derived widget type will, in its constructor, call createObj()
 * with a pointer to the appropriate LVGL object type. For instance
 *          createObj(lv_dropdown_create(objParent));
 * This effectively make it the responsibility of the derived class to create the right _type_
 * of object and pass it to createObj() so that the base class can hang on to that handle and
 * then setup event callbacks for the object.
 * 
 * @param o The lv_obj_t pointer created by the derived class.
 */
    void createObj(lv_obj_t* o);        ///< Might this be better named storeNewObj() - but lots of refacotring to do if so.
/**
 * @brief Get the Event Name from a given event code. Translate it to text, essentially.
 * 
 * @param code An lv_event_code_t type from LVGL.
 * @return const char* Pointer to a string with the textual description of the code type.
 */
    static const char* getEventName(lv_event_code_t code);
/**
 * @brief Check to see if an event code is valid or not. Utility function.
 * 
 * @param code Event code from LVGL
 * @return If the event code has a valid lookup, returns true.
 */
    static bool isUnknownCode(lv_event_code_t code);
/**
 * @brief Get the Obj object
 * 
 * @return lv_obj_t* Returns the LVGL lv_obj_t for the actual widget/object. 
 *         Great for more advanced users who may wish to manipulate the object
 *         more fully outside of the lvpp* class library.
 */
    lv_obj_t* getObj(void) { return obj; };
/**
 * @brief Set the Size object. Most of the time, LVGL does a great job of setting a sane size,
 *        but manipulation of things like the text or the font size can cause this to no longer
 *        be a good size. It is a common use pattern to instantiate a widget and then use
 *        setSize() and align() to place them in your user interface as needed.
 * 
 * @param width New width of the widget.
 * @param height New height of the widget.
 */
    void setSize(lv_coord_t width, lv_coord_t height);
/**
 * @brief Change the x/y location of the object.
 * 
 * @param align This is an LVGL notion of alignment type. It is a very rich concept of
 *              alignment that does not simply use x/y screen coordinates but uses the
 *              x/y as an offset from some other 'anchor'. In concrete terms, this is
 *              one of LV_ALIGN_CENTER, LV_ALIGN_TOP_MID, LV_ALIGN_BOTTON_RIGHT, etc.
 * @param x_ofs The x offset from the align point.
 * @param y_ofs The y offset from the align point.
 */
    void align(lv_align_t align, lv_coord_t x_ofs, lv_coord_t y_ofs);
/**
 * @brief Set the Font Size for the object in simple 'points'.
 * 
 * NOTE: This makes easy work of changing font sizes, but its downfall is that the size
 *       can be given for font sizes that don't exist for several reasons. Be forewarned.
 * 
 * @param points The desired point size for the object.
 * @todo Possibly return a bool for errant point values or for requests of point sizes that weren't compiled in?
 * 
 */
    void setFontSize(uint8_t points);
/**
 * @brief Set the Font to be used. This is a more complete/complex version of setFontSize().
 *        The fact that the font pointer is required at least means the user knows the font exists.
 * 
 * @param pF A pointer to a valid lv_font_t to be used for the object.
 */
    void setFont(lv_font_t* pF);
/**
 * @brief Set the background color of the object
 * 
 * @param color lv_color_t color to be used for the background.
 */
    void setBGColor(lv_color_t color);
/**
 * @brief Set the Text for the primary text label. For a button, for instance, this is the text on top of the button.
 * 
 * @param pText char pointer to the text
 */
    virtual void setText(const char* pText);
/**
 * @brief Set the Text Alignment. This allows the object's primary label to be moved from its 'center' position.
 * 
 * @param align This is an LVGL notion of alignment type. It is a very rich concept of
 *              alignment that does not simply use x/y screen coordinates but uses the
 *              x/y as an offset from some other 'anchor'. In concrete terms, this is
 *              one of LV_ALIGN_CENTER, LV_ALIGN_TOP_MID, LV_ALIGN_BOTTON_RIGHT, etc.
 * @param xoff The x offset from the align point.
 * @param yoff The y offset from the align point.
 */
    void setTextAlign(lv_align_t align, lv_coord_t xoff, lv_coord_t yoff);
/**
 * @brief Set the Text Color of the primary label object
 * 
 * @param newColor An lv_color_t for the desired color to use.
 */
    void setTextColor(lv_color_t newColor);
/**
 * @brief Set the text for the adjacent text label.
 *        This label is not enabled by default. Setting this text will create and enable the
 *        adjacent label object. The label's position can be set, optionally, as well.
 * 
 * @param pText Pointer to the text to use.
 * @param x_ofs X offset from the center of the main object.
 * @param y_ofs Y offset from the center of the main object.
 */
    virtual void setAdjText(const char* pText, lv_coord_t x_ofs=-10000, lv_coord_t y_ofs=-10000);
//    void setAdjTextColor(lv_color_t newColor);
/**
 * @brief Callback that derived objects can use to get notification when the object is clicked.
 * 
 */
    virtual void onClicked() { };
/**
 * @brief Internal version of the onClicked() callback which is used for other derived internal
 *        objects in the library.
 * 
 */
    virtual void internalOnClicked() { };
/**
 * @brief Callback that derived objects can use to get notification when the value changes of an object.
 * 
 */
    virtual void onValueChanged() { };
/**
 * @brief Internal version of the onValueChanged() callback which is used for other derived internal
 *        objects in the library.
 * 
 */
    virtual void internalOnValueChanged() { };
/**
 * @brief Set the Friendly Name of the object. This is the name given in the constructor's fName
 *        parameter and is unlikely to be changed. However, it can be via this method.
 * 
 * @param pName pointer to a new friendly name.
 */
    void setFriendlyName(const char* pName);
/**
 * @brief Get the Friendly Name of the object
 * 
 * @return std::string of the internal friendly name of the object.
 */
    std::string getFriendlyName() { return friendlyName; };
/**
 * @brief Get the Obj Type
 * 
 * @return std::string of the internal object type. This is what is given in the constructor
 *         as oType. It can be used to determine the type of an object without the need for
 *         RTTI library support and dynamic_cast<> or other such RTTI support methodologies.
 */
    std::string getObjType() { return objType; };
/**
 * @brief Utility useful for dumping the friendly name and object type if needed.
 * 
 * @return const char* returned `friendlyName(objectType)`
 */
    const char* whoAmI(void);
/**
 * @brief Uses a lambda callback for when an object is clicked.
 * 
 * This method allows users to instantiate most objects without deriving their own class
 * from them. They can instantiate the object, set its font, size, alignment, and text all
 * using the built in methods and they can receive a callback through the use of this
 * lambda callback methodology.
 * 
 * @param cbF This is a std::function<> lambda defining the callback 'inline'.
 */
    void setCallbackOnClicked(std::function<void()> cbF) {
        assert(!cbOnClicked);
        assert(cbF);
        cbOnClicked = cbF;
    };
/**
 * @brief Uses a lambda callback for when an object value changes.
 * 
 * This method allows users to instantiate most objects without deriving their own class
 * from them. They can instantiate the object, set its font, size, alignment, and text all
 * using the built in methods and they can receive a callback through the use of this
 * lambda callback methodology.
 * 
 * @param cbF This is a std::function<> lambda defining the callback 'inline'.
 */
    void setCallbackOnValueChanged(std::function<void()> cbF) {
        assert(!cbOnValueChanged);
        assert(cbF);
        cbOnValueChanged = cbF;
    };

protected:
    lv_obj_t* label;    ///< Primary label.
    lv_obj_t* adjLabel; ///< For items that have a label 'nearby' (adjacent label)
    std::function<void()> cbOnClicked=nullptr;      ///< Start without a lambda callback
    std::function<void()> cbOnValueChanged=nullptr; ///< Start without a lambda callback
    lv_obj_t* obj;      ///< The LVGL object that was created for this widget
    lv_obj_t* objParent;    ///< Any parent object (following same principle as LVGL here)
    std::string objType;
    std::string friendlyName;
    lv_style_t style_obj;   ///< Main style object - unsure if I'm handling all of the style bit correctly.
    void setObjType(const char* pType) { objType = pType; };

/**
 * @brief Main event handler loop for all events. This handler makes calls for onClicked() and onValueChanged()
 * 
 * @param event LVGL lv_event_t type event to be handled.
 */
    void baseEventHandler(lv_event_t* event);
/**
 * @brief Any events _not_ handled by baseEventHandler come here. Can be overridden by a derived class.
 * 
 * @param event LVGL lv_event_t type event to be handled.
 */
    virtual void eventHandler(lv_event_t* event) { };
/**
 * @brief Static member to handle all inbound callback events.
 *        Used to "route" event handling to the appropriate C++ object
 * 
 * @param event An LVGL lv_event_t event object pointer to be handled.
 */
    static void lvCallback(lv_event_t* event);
/**
 * @brief One-time initialization of the event names array.
 * 
 */
    static void initEventNames(void);

    static bool bEventNamesInitComplete;    ///< Status of the event names table to avoid re-processing.
};

/**
 * @brief Derivation from lvppBase. Those widgets which have an inherant 'value' to show are based on 
 *        this slight modified version of lvppBase which includes a value label and an associated style object.
 * 
 */
class lvppBaseWithValue : public lvppBase {
public:
/**
 * @brief Construct a new lvpp object based on lvppBase which has an associated value.
 * 
 * @param fName Internal object name. This is generally used in findObj() or lvppScreen
 * @param oType The type of the object being created. This is primarily internal, but
 *              can be used to know the category/type of the object and can be obtained
 *              via getObjType() if needed.
 * 
 */
    lvppBaseWithValue(const char* fName, const char* oType);
/**
 * @brief Set the Value associated with the object. This is a pure virtual function and
 *        must be overridden and implemented by the derived class. When implementing your
 *        own derived class, be sure to not only set `curValue = value`, but also to send
 *        an event to trigger the update notification via:
 *              `lv_event_send(obj, LV_EVENT_VALUE_CHANGED, NULL);`
 * 
 * One more note - the class has a 'min' and a 'max' that is expected to keep the value within
 * those value bounds. It is the responsibility of this function, as written by the derived
 * class, to enforce that min and max - and to decide whether to set curValue to 'max' in the
 * case where value is greater than max or to simply ignore the value altogether due to it being
 * out of the specified range.
 * 
 * @param value The integer value to be set.
 * @param animate If there is animation involved, we decide it in this parameter.
 */
    virtual void setValue(int16_t value, bool animate) = 0;
/**
 * @brief Get the Value from the object.
 * 
 * @return int16_t The current value is returned.
 */
    int16_t getValue(void) { return curValue; };
/**
 * @brief In the case where a label is desired to be shown for the value, enable it and align it here.
 * 
 * @param xoff X offset of label
 * @param yoff Y offset of label
 * @param alignment Defaults to a 'center' alignment but other LV_ALIGN_* values can be experimented with.
 */
    void enableValueLabel(lv_coord_t xoff, lv_coord_t yoff, lv_align_t alignment=LV_ALIGN_CENTER);
/**
 * @brief Set the Value Label Format. This 'format' allows the label to be more than simply a number.
 *        The format needs a '%d' in there somewhere (ala printf style) but this allows you to have
 *        text before the number and even a '%' after - for instance "Completeness: %d%%".
 *        When the value changes, the class will automatically apply this format with the new value.
 * 
 * @param fmt A printf-style format to be used when values change in the object.
 */
    void setValueLabelFormat(const char* fmt);
/**
 * @brief Set the Value Label Color object
 * 
 * @param newColor The lv_color_t of the desired color for the value label.
 */
    void setValueLabelColor(lv_color_t newColor);
/**
 * @brief Set the Value Label Font
 * 
 * @param pF A pointer to the font to be used. This is a lv_font_t pointer.
 */
    void setValueLabelFont(const lv_font_t* pF);
/**
 * @brief Set the Value Range allowed. NOTE: This enforcement needs to happen in the derived setValue() function.
 * 
 * @param _min Minimum value allowed.
 * @param _max Maximum value allowed.
 */
    void setValueRange(int16_t _min, int16_t _max) { min = _min; max = _max; };
protected:
    void internalOnValueChanged();  ///< Internal handler for value change. Does the formatting as well.
    int16_t curValue;               ///< The value we're keeping and handling in this class.
    lv_obj_t* valueLabel;           ///< The label that receives the formatted value if the label is enabled.
    std::string valueLabelFormat;   ///< The format to use when the value changes.
    lv_style_t style_value_obj;     ///< The style of the label.
    int16_t min, max;               ///< The allowable range of the value.
};

#endif
