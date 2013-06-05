project(libusermetrics CXX)
cmake_minimum_required(VERSION 2.7.8)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake" "${CMAKE_MODULE_PATH}")

find_package(Qt5Core REQUIRED)

#find_package(Qt5DBus COMPONENTS Qt5DBusMacros REQUIRED)
#set(TEST_SOURCES)
#qt5_add_dbus_adaptor(TEST_SOURCES com.canonical.hud.xml hud.h HudService)

include_directories(${Qt5DBus_INCLUDE_DIRS})

set(INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")
set(SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")

include_directories(${INCLUDE_DIR})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fvisibility=hidden")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fno-permissive -pedantic -Wall -Wextra")

add_subdirectory("include")
add_subdirectory("src")

add_subdirectory(tests)
