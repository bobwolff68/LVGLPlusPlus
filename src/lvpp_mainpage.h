#pragma once

// This file is only used for Doxygen documentation at the higher level of the library.

/**
 * @file lvpp_mainpage.h
 * @brief Main documentation page for Doxygen purposes.
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
 * \section todo_sec High level to-do-list
 * 
 * - lvppDropdown and lvppRoller are similar enough that they should get a common base class
 *   for option choosing. This would consolidate much of the getting, setting, and then possibly
 *   _adding_ of options by virtue of keeping an internal vector of those options. This would also
 *   aid in being able to have a getter for the text value which doesn't have to worry about
 *   destination pointer memory overruns but could just pass back a string. In a related matter,
 *   lv_dropdown allows for adding singular options to the dropdown, but roller doesn't have the
 *   same ability. The consistency of these two widgets could be had by keeping the internal list,
 *   and then when a single 'add' is requested in a roller, the entire list could be 'set' again
 *   after the 'add'. Additionally the lists could be sorted alphabetically.
 * - There is a need for cleanup on how object styles are used and added and how objects are then
 *   invalidated after a change. This is still a fuzzy misunderstanding of LVGL for myself. While
 *   it has become clearer recently, some of these objects are still not 'clean' in that regard
 *   and may still have misunderstandings buried in them. #fairwarning
 * 
*/