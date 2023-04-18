#pragma once

// This file is only used for Doxygen documentation at the higher level of the library.

/**
 * @file lvpp_mainpage.h
 * @brief Main documentation page for Doxygen purposes.
 * 
 */

/** \mainpage LVGLPlusPlus High-level Goals
 * 
 * LVGLPlusPlus aspires to put a low-cost C++ wrapper around the LVGL library.
 * It will not support every feature of LVGL as the feature-breadth of the
 * full library is extensive and growing. However, it also does not preclude
 * the co-mingling of this C++ wrapper usage and the underlying library.
 * The user is free to get handles to pointers and use them directly in lv_* calls.
 * 
 * ## Goals:
 * - Lightweight - no heavy use of memory allocations nor heavy std library use. 
 *   - Std library usage is kept pretty much to std::vector, std::string, and std::map
 * - Lower the entry-level 'cost' for new users to be able to utilize this amazing underlying library.
 * - Handle the most common use cases
 *   - Keeps the library smaller
 *   - Makes less work to implement it to a steady state
 *   - Keeps test coverage more managable if there is only one or two maintainers
 * - Hide the intricacies of LVGL behind common methods amongst many of the widget types.
 *   - The lv_* API is large and requires some study to understand. Commong methods amongst
 *     several widget types keeps the learning curve lower and the bugs less prevalent.
 *
 * ### Release Notes
 * - [Release Notes found here](#release_notes) 
 *
 * \section intro_sec Introduction
 * 
 * I started using LVGL for a hardware design that utilized an ILI9341 TFT touch display.
 * LVGL seemed like the right answer to my needs, but it felt like a learning curve that
 * was fairly steep. Examples were great, but I was failing to understand some of the 
 * subtleties that mattered. Ultimately LVGLPlusPlus was borne out of my own learnings
 * in using the library and wanting to be able to consistently utilize it without tripping
 * up over some of the details.
 * 
 * Philosophy - support the biggest common use cases
 *  - Create a common set of modifier functions that work regardless of what type of widget
 *  - Decomplexify drawing for most cases
 *  - Allow use of the underlying lvgl functionality for advanced use cases.
 * 
 * \section included_sec What's included (today) - and what's not
 * 
 * As of the first public release in Spring 2023, LVGLPlusPlus, at a high level, has the
 * following included:
 * - lvppBase - A hopefully fairly well designed base class upon which all other widgets sit.
 *   It is the intended base gateway to LVGL functionality that both hides some of the details
 *   and yet also allows more advanced users access to the underlying object pointers, for instance.
 * - lvppBaseWithValues - A derived class from lvppBase which is useful for several LVGL widgets
 *   such as Bar, Arc, and Slider. 
 * - lvppButton - This handles the Button and a CycledButton concept.
 * - lvppFullImageToggleButton - Button who's state relates directly to a pair of images.
 * - lvppImage - supporting the placement of images on screen
 * - lvppLabel - supporting the placement and modification of labels
 * - lvppDropdown - A dropdown list manager widget
 * - lvppRoller - A widget for choosing amongst a list which scrolls or rolls on the display.
 * - lvppCanvasFullColor and lvppCanvasIndexed - these two classes support the canvas drawing
 *   functionality while hiding some of the more detailed descriptor typedefs/structs and 
 *   filling them out. Additionally, given that many smaller CPUs lack the memory to have their
 *   nice TFT display in full color simply due to memory constraints, the Indexed version of 
 *   the class provides much of the full color functionality but using an indexed color set.
 *   It hopes to help the user in not having to manage their own list of indexed color values
 *   and instead handles these internally.
 * 
 * While more functions and widgets are coming, I wanted to get this version documented and published
 * to not delay the use of it by folks who might want to pick up and use it.
 * 
 * Some prioritized widgets that will come sooner than later are:
 * - [Checkbox](https://docs.lvgl.io/8.3/widgets/core/checkbox.html)
 * - [Line](https://docs.lvgl.io/8.3/widgets/core/line.html)
 * - [Switch](https://docs.lvgl.io/8.3/widgets/core/switch.html)
 * - [Text Area](https://docs.lvgl.io/8.3/widgets/core/textarea.html)
 * 
 * Secondary priorities at this point include:
 * - [Button matrix](https://docs.lvgl.io/8.3/widgets/core/btnmatrix.html)
 * - [Table](https://docs.lvgl.io/8.3/widgets/core/table.html)
 * - The whole list of [Extra Widgets](https://docs.lvgl.io/8.3/widgets/extra/index.html)
 *
 * \section started_sec Getting Started
 * 
 * ## How do I use this thing, anyway?
 * 
 * The project is intended, for most people, to be used from PlatformIO as a library.
 * The library mechanism inside PlatformIO is a beautiful thing. You can go searching
 * for libraries by keyword and get lots of results and then make a decision of 
 * which makes sense for you. Adding it to your project can be done from the "GUI"
 * or it can also simply be added to the platformio.ini file in the `libs` section.
 * 
 * To help facilitate "how to use it", the below mentioned sample projects do include
 * LVGLPlusPlus in exactly this manner. Check out their platformio.ini file or copy
 * the project and use it as a starting space and hack-n-slay from there.
 * 
 * My personal setup is on a Mac M1/ARM running VSCode with PlatformIO. I _think_ the
 * only other additions to my system are around the installation of SDL2 in order to
 * support the native simulator build of the project. I'd love to have someone
 * volunteer to step through that installation and write down any intricacies they
 * encounter so I can get it documented (or they could document it!). The problem
 * I face is that my setup already works and I didn't write down what all I did to
 * get things in place. I do know I used `brew` to install SDL2, at least.
 * 
 * ## Samples
 * 
 * I've produced two distinct projects as starting points. The two end results on screen
 * look identical, but the underlying difference around threading makes for a few notable
 * differences and I figure folks will gravitate to one or the other. And I didn't want
 * to overly complicate an example with having "both" mechanisms in place as that kinda
 * defeats the purpose of an example being easily digestable. 
 * 
 * The two samples are:
 * - [Sample Without threading](https://github.com/bobwolff68/LVGLPlusPlus-NonThreadedSample) 
 *   This is the simpler of the two, but I feel it is the one that
 *   gives fewer abilities to expand and grow.
 * - [Sample With threading](https://github.com/bobwolff68/LVGLPlusPlus-ThreadedSample) 
 *   This version requires the use of my RoboTask threading class and
 *   a mutex due to LVGL not being threadsafe, currently. While this adds some degree
 *   of complexity, it also allows for your program to be much more thread-oriented
 *   without being concerned about which items are interacting with LVGL and which
 *   are not.
 * 
 * Both of the sample projects are currently buildable on the simulator for Mac as
 * well as for ESP32 and a TFT ILI9341 display being driven by TFT_eSPI. This is
 * a limited setup for a sample, to be sure, but it's what I have currently so it's
 * what I've been able to put forth. I'd love help from someone to expand these
 * samples to Windows and to other micros, displays, and display drawing libraries
 * to help round things out and make it even more useful to folks starting out
 * from wherever they're starting.
 * 
 * \section pattern_sec Thoughts on Usage
 * 
 * ## Startup
 * The simulator startup is different from that of an ESP32 running Arduino framework.
 * Also, the simulator requires certain SDL2 oriented startup items to get things
 * kicked off. So, the two startups are completely different. I won't go into grave
 * detail on the "how" and the "what" exactly, here, but I'll cover the broad brush
 * strokes and get feedback for what all is really needed in the details. I hope
 * the following is enough bread crumbs to help out. And note, these items are
 * strictly outside of the scope of the LVGLPlusPlus library - I am just hoping to
 * help fulfill that goal of lowering the bar for getting something rolling using
 * the library. 
 * 
 * Much of the startup code to bind my use of TFT_eSPI to the project and the use
 * of the simulator / native build came from LVGL's Github as there is a
 * [PlatformIO example](https://github.com/lvgl/lv_platformio) which was an excellent 
 * starting point for me and where I found the app_hal.* examples.
 * 
 * ## ESP32 / TFT_eSPI
 * 
 * While the app_hal.c file is instructional, I think the better way to fully
 * understand the startup and binding with ESP32 and TFT_eSPI would come from
 * looking at my [Sample LVGL Project](https://github.com/bobwolff68/LVGLPlusPlus-NonThreadedSample)
 * 
 * ## Use of Screen(s) and Widget(s)
 * While you are more than welcome to derive your own classes from the lvpp* classes
 * in order to fully encapsulate them and to have your own callback handlers in your
 * class, I wanted to give a simpler and slicker way to both instantiate and use these
 * classes as simpler objects. I wanted to give the user the ability to control the
 * instance of the object through methods and use lambda functions for callbacks.
 * Additionally I wanted to simplify the grouping and control over setting values
 * without having to keep track of all the pointers to all the objects.
 * 
 * With this foreward, the use pattern I most like (today) is seen in the sample
 * projects primarily in [Widget.cpp](https://github.com/bobwolff68/LVGLPlusPlus-ThreadedSample/blob/main/src/Widgets.cpp)
 * and in [TheBrain.cpp](https://github.com/bobwolff68/LVGLPlusPlus-ThreadedSample/blob/main/src/TheBrain.cpp#L68)
 * 
 * - Instantiate an lvppScreen() object. Let's call this your main screen.
 *   @code pScreenMain = new lvppScreen(lv_scr_act()); @endcode 
 * - Now instantiate an lvppButton() object. Give it a name, some text, and then
 *   place it on the screen and finally give a callback for when it is clicked.
 *   @code
 *     static lvppButton setupButton("Setup", LV_SYMBOL_SETTINGS" Setup");
 *     setupButton.align(LV_ALIGN_BOTTOM_RIGHT, -3, -3);
 *     setupButton.setCallbackOnClicked([]() -> void {
 *         // Time to load the setup screen.
 *         if (pScreenSetup) {
 *             pScreenSetup->ActivateScreen(500, LV_SCR_LOAD_ANIM_OVER_LEFT);
 *         }
 *     });
 *   @endcode 
 * - Next - take this button and add its address to the `pScreenMain` object for later use.
 * @code pScreenMain->AddObject(&setupButton); @endcode
 * - These snips came directly from the referened `Widgets.cpp` file above. There are more
 *   complex examples of an lvppBar object with ranges and size and values and another lvppButton,
 *   lvppCycleButton, and lvppDropdown.
 * - Let's show the addition of the lvppBar object here along with its size, color change, etc.
 * @code 
 *   static lvppBar fullnessBar("H2OLevel");
 *   fullnessBar.setSize(13, 80);
 *   fullnessBar.align(LV_ALIGN_TOP_RIGHT, -19, 20);
 *   fullnessBar.setRange(0,100);
 *   fullnessBar.setValueLabelFormat("%d%%");
 *   fullnessBar.enableValueLabel(3, 17, LV_ALIGN_BOTTOM_MID);
 *   fullnessBar.setValue(20);
 *   fullnessBar.setAdjText("Water", 0, -50);
 *
 *   pScreenMain->AddObject(&fullnessBar);
 * @endcode 
 * - If you've created this lvppBar which was named "H2OLevel", now from TheBrain.cpp, you can
 *   set it's value by only knowing the screen pointer and the name of the object so long as 
 *   you did do the AddObject() so that the lvppScreen knows about that object.
 * @code pScreenMain->setObjValue("H2OLevel", fullPercentage); @endcode 
 * - Pretty slick, right? I hope folks agree.
 * 
 * 
 * \section todo_sec High level to-do-list
 * 
 * This is a rather unstructured list of items I'm needing to address:
 * - Add a few images to the main page - logo? links to lvgl with logo ; sample screenshot
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
 *   and may still have misunderstandings buried in them. Fair warning...
 * - Find a contributor with a Windows machine who is willing and able to:
 *   - Take the Mac native simulation configuration and translate that to Windows.
 *   - Test against that configuration.
 *   - Document any pre-requisite dependencies that had to be installed in the environment to make it work.
 * - Build a test-jig program which exercises each of the lvpp* classes
 *   - Possibly utilize lvppScreen to make each 'unit test' into a screen
 *   - Show use of adjacent labels, labels, values, options, different drop-directions, adding
 *     full and individual options to selection lists, etc.
 *   - Find someone to document the requirements for the test jig.
 *   - Find someone to implement the test jig as I think it'll require more time than I (currently) have.
 * - Get feedback from anyone who is using the library on a non-ILI9341 display or using a different
 *   drawing support method than TFT_eSPI from Bodmer.
 * - Better or more complete handling of what font sizes are handled by setFontSize() without making
 *   assumptions of what fonts were baked into the build and which were not.
 *   - Would be great to calculate or measure what font sizes "cost" in terms of build size to give
 *     users a good sense of what levers they are pulling and how much each lever costs.
 * - Go through and clean up any printf() items and think about LV_INFO or LV_WARN in its place.
 * - The design of lvppFullImageToggleButton may be overly complicated or 'manual' as I vaguely remember
 *   seeing an lv_create_ construct which might be an ImageButton already. Maybe refactor here.
 * 
 * \section about_sec About Me
 * 
 * I'm an engineer who started my career, professionally, in 1991. I'm likely the 'old guy' in the room.
 * My hair color proves it. I've been involved in projects ranging from digital hardware design to
 * programmable logic design as well as writing the software that compiled those complex PLDs. My
 * software career has been centered largely around digital video in various forms on PC platforms,
 * consumer platforms, and more recently fully embedded work on micros from Arduino to ESP32 based
 * designs (both hardware and software). Passions of mine revolve mostly around Adventure motorcycling
 * and Burning Man with a sustainability slant.
 * 
 * I found LVGL after I had implemented my own multi-threaded graphics subsystem for my own
 * ESP32-based pandemic "mental health project". It worked and worked well, but the graphics 
 * were not pretty as I'm definitely not an artist. When I discovered LVGL, I had embarked
 * on a new ESP32 based design and I dove into implementing the graphics portion for the
 * new project entirely with LVGL. I was blown away at what it could do and do very well, but
 * I was also struck by not fully (or quickly) understanding how styles and some of the handles
 * worked. My project was working but it felt "clunky" and disorganized. I started writing my C++
 * wrapper for LVGL borne out of this sense of my own disorganization and wanting to simplify my
 * own interfaces to the library underneath. I'm happy to open-source this project and hope that it
 * makes some projects get built quicker and with fewer issues or errors.
 * 
*/
