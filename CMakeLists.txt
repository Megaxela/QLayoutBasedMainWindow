cmake_minimum_required(VERSION 3.8)
project(QLayoutBasedMainWindow)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
    src/Windows/QLayoutMainWindow.cpp
    src/Dialogs/LayoutSavingDialog.cpp
)

set(QOBJECT_HEADERS
    include/Windows/QLayoutMainWindow.h
    include/Dialogs/LayoutSavingDialog.h
)

set(UI_FILES
    ui/layout_saving_dialog.ui
)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt5Core    CONFIG REQUIRED)
find_package(Qt5Widgets CONFIG REQUIRED)
find_package(Qt5Gui     CONFIG REQUIRED)

qt5_add_resources(RESOURCE_ADDED ${QT_RESOURCES})

qt5_wrap_cpp(UI_SOURCE
    ${QOBJECT_HEADERS}
)

qt5_wrap_ui(
    UI_HDRS
    ${UI_FILES}
)

add_library(QLayoutBasedMainWindow STATIC
    ${UI_HDRS}
    ${UI_SOURCE}
    ${MAIN_FILE}
    ${SOURCE_FILES}
    ${INCLUDE_FILES}
    ${UNMANAGED_FILES}
    ${RESOURCE_ADDED}
    ${QOBJECT_HEADERS}
)

target_include_directories(QLayoutBasedMainWindow PUBLIC
    include
)

target_compile_options(QLayoutBasedMainWindow
    PRIVATE
        -ansi
        -pedantic
        -Wall
        -Wextra
        -Weffc++
        -Woverloaded-virtual
        -Winit-self
        -Wunreachable-code
)

target_link_libraries(QLayoutBasedMainWindow
    Qt5::Core
    Qt5::Widgets
    Qt5::Gui
)