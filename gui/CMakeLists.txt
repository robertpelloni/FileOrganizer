
if (NOT FO_BUILD_GUI)
    return()
endif()

find_package(Qt6 COMPONENTS Widgets REQUIRED)

set(GUI_SOURCES
    main.cpp
    mainwindow.cpp
    mainwindow.h
)

add_executable(fo_gui ${GUI_SOURCES})

target_link_libraries(fo_gui PRIVATE Qt6::Widgets fo_core)

# Copy resources if needed
# qt_add_resources(...)
