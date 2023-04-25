Release Notes  {#release_notes}
=============

## General
- [Discussions](https://github.com/bobwolff68/LVGLPlusPlus/discussions) area on Github is a great way to get involved or get answers.

## Release Notes Follow:

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
