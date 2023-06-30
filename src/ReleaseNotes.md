Release Notes  {#release_notes}
=============

## General
- [Discussions](https://github.com/bobwolff68/LVGLPlusPlus/discussions) area on Github is a great way to get involved or get answers.

## Release Notes Follow:

### Version 1.4.1
• Image rotation allowed for true color and true color with chroma. Warning issued for attempting rotation otherwise.
• Removed ‘defaults’ feature. Not well thought out enough. Too many unexpected consequences in current form.
• lvppBaseWithValue::
•   Added baseGetter() and baseSetter()
•   Added getValuePercentage() which adheres to min/max range.
• Fix: lvppOptions was not ‘public’ as base to lvppDropdown and lvppRoller

### Version 1.4.0 - Features, consistency, fixes
- lvppScreen - when doing 'addObj()', properly re-parents all objects to the screen.
- [lvppOptions](https://bobwolff68.github.io/LVGLPlusPlus/gh-pages/classlvpp_options.html) - widgets lvppDropdown, lvppRoller, and lvppCycleButton all inherit from lvppOptions now to gain total consistency on how options are added and managed in such widgets.
- lvppImage gained features and testing in the ability to set rotation amounts (for full color images), ties in properly to setSize() which will auto-scale the image to your desired size, and even sets the size properly if the user calls setImage _after_ calling setSize through a deferred width and height feature internally.
- [lvppSwitch](https://bobwolff68.github.io/LVGLPlusPlus/gh-pages/classlvpp_switch.html) - The On/Off click-switch UI element is now available. The user can setCheckedState(), getCheckedState, and setEnabled()

- Default items
  - setDefaultFont(), setDefaultBGColor(), setDefaultTextColor()
  - These new methods are static methods to lvppBase and can be used to aid in setting up some default choices which will get utilized as default any time a widget lvpp* class is created from that point forward. The most impactful one, IMHO, is the font. This allows widgets to no longer need individualized settings but instead a single call in the beginning.
  - These defaults caused a number of minor edits in lvppBase to support fully in object, label, adjacent label, and value labels.

- Color Gradients
  - lvppBase gained 3 variants for set*ColorGradient() which will paint a gradient between two colors at the 'ends' of the object. In the case of lvppBar, lvppSlider, and lvppArc, this means the _indicator_ itself while in the case of buttons and labels it is the main background which receives the gradient treatment.

- Consistency
  - addOptions, addOption, and setOptions are not consistently utilized
  - getCurrent* becomes getSelected* as _current_ isn't very descriptive, but _selected_ is about the user interface.
  - Arguments to methods were sometimes not _const_ and should have been. This will likely be more of these changes.
  - lvppBase
    - Setting attributes is now consistent amongst object, label, adjacent label, and value label. 
    - Text, TextColor, BGColor, Font, and JustificationAlignment are all suffixes for each of the types of items. For instance setTextColor(), setAdjTextColor(), and setValueLabelTextColor() all exist now.
    - JusitifcationAlignment allows centered, left-placed, or right-placed text labels. Super handy.
    - Getting consistent on using local styles for labels. Might make one more pass at this to use local styles on the main object itself, but that might defeat eventually granting users access to bring in their own styles or copy out a style.



### Version 1.3.5 - Adding a few quick feature items
- lvppScreen::activatePriorScreen() - After using activateScreen, the user can "pop" to the prior screen with this method.
- lvppLabel::lvppLabel() Constructor - can omit the initial text and will get set to ""
- lvppCanvasIndexed
  - Added a draw "centered" rectangle where all you have to give is the size of the border in the x-side and the y-side. Just a nice handy addition.
  - drawCenteredRectWithFill(), drawCenteredRectWithFillByIndex()
  - drawCenteredRectWithoutFill(), drawCenteredRectWithoutFillByIndex()
- lvppBase
  - getText() - returns the label text as a std::string
  - setLabelJustificationAlignment() - Want your label centered or right justified? Simple method for allowing the use of LV_TEXT_ALIGN_*
  - setLabelColorizationEnabled() - LVGL allows the use of #RRGGBB placed inline in the text of the label to provide colorization of the text. This method enables or disables that feature.
  - getLabelObj() - returns the 'label' object to allow more advanced usage patterns.

### Version 1.3.0 - First Public version documented and released
- April 18, 2023 - released as public repo.
- Fairly complete library as described at [LVGLPlusPlus Github Pages](https://bobwolff68.github.io/LVGLPlusPlus)
- Pre-test-jig state.

#### Pre-Version 1.3.0 - All internal and getting things squared up for public release.
