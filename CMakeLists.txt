add_library(lvglcpp
    # ./src/app_hal.c
    ./src/lvppBase.cpp
    # ./src/lvppUtils.cpp
    ./src/lvppLabel.cpp
    ./src/lvppButton.cpp
    ./src/lvppImage.cpp
    ./src/lvppScreen.cpp
    ./src/lvppDropdown.cpp
    ./src/lvppKeyboard.cpp
    ./src/lvppTextarea.cpp
    ./src/lvppArc.cpp
    ./src/lvppBar.cpp
    ./src/lvppCanvas.cpp
    # ./src/app_hal.h
    # ./src/lvppBase.h
    # ./src/lvpp.h
    # ./src/lvpp_mainpage.h
    # ./src/lvppUtils.h
)

target_link_libraries(lvglcpp
    pico_stdlib
    lvgl
)

target_include_directories(lvglcpp PUBLIC
    .
    ./src
)
