cmake_minimum_required(VERSION 3.8)
project(QLayoutBasedMainWindow)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "-ansi -pedantic -Wabi -Wall -Wextra -Weffc++ -Woverloaded-virtual -Winit-self -Wunreachable-code ")

set(INCLUDE_FILES)

set(SOURCE_FILES
        src/Windows/QLayoutMainWindow.cpp
        src/Dialogs/LayoutSavingDialog.cpp )

set(UNMANAGED_FILES
        )

set(QOBJECT_HEADERS
        include/Windows/QLayoutMainWindow.h
        include/Dialogs/LayoutSavingDialog.h)

set(UI_FILES
        ui/layout_saving_dialog.ui)

set(MAIN_FILE )

include_directories(
        include
)

#------------------------
# Setuping Qt
#------------------------

set(CMAKE_INCLUDE_CURRENT_DIR ON)
find_package( Qt5Core REQUIRED )
find_package( Qt5Widgets REQUIRED )
find_package( Qt5Gui REQUIRED )
qt5_add_resources(RESOURCE_ADDED ${QT_RESOURCES})

qt5_wrap_cpp(UI_SOURCE
        ${QOBJECT_HEADERS}
        )

qt5_wrap_ui(
        UI_HDRS
        ${UI_FILES}
)

#------------------------
# Adding to executable
#------------------------

add_executable(QLayoutBasedMainWindow
        ${UI_HDRS}
        ${UI_SOURCE}
        ${MAIN_FILE}
        ${SOURCE_FILES}
        ${INCLUDE_FILES}
        ${UNMANAGED_FILES}
        ${RESOURCE_ADDED}
        ${QOBJECT_HEADERS}
        )

qt5_use_modules(QLayoutBasedMainWindow Core Widgets Gui)
