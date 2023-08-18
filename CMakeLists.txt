cmake_minimum_required(VERSION 3.15)

project(ElevationChart LANGUAGES CXX)

set(COMPANY "Radar-MMS")
set(COPYRIGHT "Copyright (c) 2023 ${COMPANY}. All rights reserved.")
set(IDENTIFIER "${PROJECT_NAME}")

option(MAKE_LIBRARY
    "Exclude examples from build and make static library."
    OFF
)

if(NOT MAKE_LIBRARY)
    set(CMAKE_CXX_STANDARD 17)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)

    set(CMAKE_AUTORCC ON)
    set(CMAKE_AUTOMOC ON)
endif()

find_package(QT NAMES Qt5 COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS 
    Core
    Quick
    Positioning
    Widgets
    QuickControls2
    Concurrent
)

include_directories(src/c++ include examples)

file(GLOB_RECURSE EC_CORE src/c++/*)
file(GLOB_RECURSE EC_CORE_INCLUDE include/*)
file(GLOB_RECURSE EC_EXAMPLES examples/*)

add_subdirectory(libs/LPVL)
add_subdirectory(libs/DEM)

if(MAKE_LIBRARY)
    message("Making library...")
    set(SRC ${EC_CORE} ${EC_CORE_INCLUDE} resources/elevation-chart-rc.qrc src/qml/elevation-chart.qrc src/js/elevation-chart-js.qrc)
else()
    message("Making test executable...")
    set(SRC ${EC_CORE} ${EC_CORE_INCLUDE} ${EC_EXAMPLES} resources/elevation-chart-rc.qrc src/qml/elevation-chart.qrc src/js/elevation-chart-js.qrc)
endif()

if(MAKE_LIBRARY)
    add_library(${PROJECT_NAME} ${SRC})
else()
    add_executable(${PROJECT_NAME} ${SRC})
endif()

target_link_libraries(${PROJECT_NAME} 
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Quick
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Positioning
    Qt${QT_VERSION_MAJOR}::QuickControls2
    Qt${QT_VERSION_MAJOR}::Concurrent
    LPVL
    DEM
)

target_include_directories(${PROJECT_NAME} PUBLIC include src)

execute_process(COMMAND git remote update WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
execute_process(COMMAND git describe --tags --abbrev=0 WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} OUTPUT_VARIABLE GIT_VERSION OUTPUT_STRIP_TRAILING_WHITESPACE)

add_definitions(
    -DPROJECT_NAME="${PROJECT_NAME}"
    -DPROJECT_VERSION="${GIT_VERSION}"
    -DPROJECT_COMPANY="${COMPANY}"
)

message("${PROJECT_NAME} version ${PROJECT_VERSION} compiled successfully")
