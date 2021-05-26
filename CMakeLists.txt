cmake_minimum_required(VERSION 3.1.0)

project(plotter VERSION 1.0.0 LANGUAGES CXX)

##################SOURCE FILES##############################

set(SOURCE_FILES
    main.cpp
    qcustomplot.cpp
    mainwindow.cpp
    main.cpp
    )

set(HEADER_FILES
    mainwindow.h
    qcustomplot.h)

set(UI_FILES
    mainwindow.ui)

##################CMAKE CONFIGURATION#######################

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

find_package(Qt5 COMPONENTS Core Widgets Gui REQUIRED)

set(QT_LIBS Qt5::Widgets Qt5::Core)

add_executable(Plotter
    mainwindow.ui
    mainwindow.cpp
    main.cpp

)

target_link_libraries(Plotter ${QT_LIBS})
