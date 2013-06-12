project(libusermetrics CXX)
cmake_minimum_required(VERSION 2.8.9)

set(API_VERSION 1)
set(ABI_VERSION 1)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake" "${CMAKE_MODULE_PATH}")

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

include(GNUInstallDirs)
include(Coverage)

set(DOXYFILE_SOURCE_DIR "include")
include(UseDoxygen)
if(DOXYGEN_FOUND AND BUILD_DOXYGEN)
	install(
		DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/doc/html
		DESTINATION ${CMAKE_INSTALL_PREFIX}/share/doc/libusermetrics-doc/
	)
endif()

add_subdirectory("src")

enable_testing()
add_subdirectory(tests)


ADD_CUSTOM_TARGET(
	test_verbose
	${CMAKE_CTEST_COMMAND} --force-new-ctest-process --output-on-failure
)
