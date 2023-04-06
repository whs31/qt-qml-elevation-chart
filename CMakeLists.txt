cmake_minimum_required(VERSION 3.13)

project(GLCharts VERSION 1.1 LANGUAGES CXX)
set(LIBRARY_COMPILE FALSE)

if(NOT LIBRARY_COMPILE)
	set(CMAKE_AUTOUIC ON)
	set(CMAKE_AUTOMOC ON)
	set(CMAKE_AUTORCC ON)

	set(CMAKE_CXX_STANDARD 17)
	set(CMAKE_CXX_STANDARD_REQUIRED ON)
endif()

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets Core)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets QuickWidgets)

include_directories(src)
include_directories(include)

set (EXAMPLES
    # EXAMPLES
    examples/main.cpp
    examples/example.cpp
    examples/example.h
    examples/example.ui
    examples/example.qrc
)
set (PROJECT_SOURCES
	# QML|JS
	js/charts.qrc

	# API
	include/charts/elevationwidget.hpp
	include/charts/geopoint.hpp

	# SOURCE BASE
	src/elevationwidget_p.hpp src/elevationwidget.cpp

		# DECLARATIVE ITEMS
		src/qdeclitems/cdeclarativepolyline.hpp src/qdeclitems/cdeclarativepolyline.cpp
		src/qdeclitems/cdeclarativepolygon.hpp src/qdeclitems/cdeclarativepolygon.cpp

		# GLSL & SCENE GRAPH
		src/scenegraph/glsl/glsl.qrc
		src/scenegraph/glpolygonshader.hpp
)

if(LIBRARY_COMPILE)
	add_library(${PROJECT_NAME} ${PROJECT_SOURCES})
else()
        add_executable(${PROJECT_NAME} ${PROJECT_SOURCES} ${EXAMPLES})
endif()

target_include_directories(${PROJECT_NAME} PUBLIC src, include)
target_link_libraries(${PROJECT_NAME} Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::QuickWidgets)

add_subdirectory(submodule)


execute_process(COMMAND git describe --tags --always WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} OUTPUT_VARIABLE GIT_VERSION OUTPUT_STRIP_TRAILING_WHITESPACE)
execute_process(COMMAND ${CMAKE_COMMAND} -E env TZ=UTC git log -1 --format=%cd --date=format-local:"%d.%m.%Y %H:%M:%S" WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} OUTPUT_VARIABLE GIT_SOURCE_DATE OUTPUT_STRIP_TRAILING_WHITESPACE)
string(TIMESTAMP PROJECT_BUILD_DATE "%d.%m.%Y %H:%M:%S" UTC)
add_definitions(-DPROJECT_NAME="${PROJECT_NAME}" -DPROJECT_VERSION="${GIT_VERSION}" -DPROJECT_SOURCE_DATE=${GIT_SOURCE_DATE} -DPROJECT_BUILD_DATE="${PROJECT_BUILD_DATE}")
