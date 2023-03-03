cmake_minimum_required(VERSION 3.13)

project(QPlot VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets Core)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets QuickWidgets)

include_directories(src)
include_directories(include)

set(PROJECT_SOURCES
	examples/main.cpp
	examples/example.cpp
	examples/example.h
	examples/example.ui
	examples/QPlot-example.qrc

	js/QPlot.qrc
	include/elevationwidget.hpp			src/elevationwidget.cpp			src/elevationwidget_p.hpp
	js/qplot-js.qrc
)

add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})

target_link_libraries(${PROJECT_NAME} Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::QuickWidgets)

add_subdirectory(submodule)
