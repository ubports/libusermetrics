project(libusermetrics CXX)
cmake_minimum_required(VERSION 2.8.9)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake" "${CMAKE_MODULE_PATH}")

include(GenerateExportHeader)

find_package(Qt5Core REQUIRED)
include_directories(${Qt5Core_INCLUDE_DIRS})

find_package(Qt5Gui REQUIRED)
include_directories(${Qt5Gui_INCLUDE_DIRS})

#find_package(Qt5DBus COMPONENTS Qt5DBusMacros REQUIRED)
#set(TEST_SOURCES)
#qt5_add_dbus_adaptor(TEST_SOURCES com.canonical.hud.xml hud.h HudService)
#include_directories(${Qt5DBus_INCLUDE_DIRS})

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")
include_directories(${INCLUDE_DIR})

set(SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fno-permissive -pedantic -Wall -Wextra")

add_subdirectory("src")

enable_testing()
add_subdirectory(tests)
