cmake_minimum_required(VERSION 3.5)

project(3dprojections VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(3dprojections
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        matdisplay.h matdisplay.cpp
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET 3dprojections APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(3dprojections SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(3dprojections
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(3dprojections PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.3dprojections)
endif()
set_target_properties(3dprojections PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS 3dprojections
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(3dprojections)
endif()

# SOLUÇÃO PARA O ERRO:
# /home/caiotavares/Documents/unesp/cg/build-3dprojections-Desktop_Qt_6_3_2_GCC_64bit-Debug/3dprojections_autogen/include/ui_mainwindow.h:19: error: matdisplay.h: No such file or directory
# In file included from /home/caiotavares/Documents/unesp/cg/3dprojections/mainwindow.cpp:2:
# /home/caiotavares/Documents/unesp/cg/build-3dprojections-Desktop_Qt_6_3_2_GCC_64bit-Debug/3dprojections_autogen/include/./ui_mainwindow.h:19:10: fatal error: matdisplay.h: No such file or directory
#    19 | #include <matdisplay.h>
#       |          ^~~~~~~~~~~~~~
target_include_directories(3dprojections PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
