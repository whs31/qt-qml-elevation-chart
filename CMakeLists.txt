cmake_minimum_required(VERSION 3.13)

project(ElevationChart VERSION 1.1 LANGUAGES CXX)
set(LIBRARY_COMPILE FALSE)

set(COMPANY "Radar-MMS")
set(COPYRIGHT "Copyright (c) 2023 ${COMPANY}. All rights reserved.")
set(IDENTIFIER "${PROJECT_NAME}")

if(NOT LIBRARY_COMPILE)
    set(CMAKE_AUTOUIC ON)
    set(CMAKE_AUTOMOC ON)
    set(CMAKE_AUTORCC ON)
    set(CMAKE_CXX_STANDARD 17)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
endif()

find_package(QT NAMES Qt6 Qt5 COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS 
    Core
    Quick
    Positioning
    QuickControls2
)

include_directories(src)
include_directories(include)

set(EXAMPLES
    examples/main.c++
    examples/examples.qrc
)

set(PROJECT_SOURCES
    qml/qml.qrc
    src/elevationchart.h src/elevationchart.c++
)

add_subdirectory(libs/LPVL)

if(LIBRARY_COMPILE)
    add_library(${PROJECT_NAME} ${PROJECT_SOURCES})
else()
    add_executable(${PROJECT_NAME} ${PROJECT_SOURCES} ${EXAMPLES})
endif()

target_include_directories(${PROJECT_NAME} PUBLIC src, include)
target_link_libraries(${PROJECT_NAME} 
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Quick
    Qt${QT_VERSION_MAJOR}::Positioning
    Qt${QT_VERSION_MAJOR}::QuickControls2
    LPVL
)

add_definitions(
    -DPROJECT_NAME="${PROJECT_NAME}"
    -DPROJECT_VERSION="${PROJECT_VERSION}"
    -DPROJECT_COMPANY="${COMPANY}"
)

message("${PROJECT_NAME} version ${PROJECT_VERSION} compiled successfully")
