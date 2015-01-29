project(libusermetrics CXX)
cmake_minimum_required(VERSION 2.8.9)

set(API_VERSION 1)
set(ABI_VERSION 1)

set(GETTEXT_PACKAGE libusermetrics)
add_definitions(
	-DGETTEXT_PACKAGE="${GETTEXT_PACKAGE}"
)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake" "${CMAKE_MODULE_PATH}")

find_package(PkgConfig REQUIRED)

find_package(Qt5Core REQUIRED)
include_directories(${Qt5Core_INCLUDE_DIRS})

find_package(Qt5Gui REQUIRED)
include_directories(${Qt5Gui_INCLUDE_DIRS})

find_package(Qt5DBus COMPONENTS Qt5DBusMacros REQUIRED)
include_directories(${Qt5DBus_INCLUDE_DIRS})

find_package(Qt5Sql REQUIRED)
include_directories(${Qt5Sql_INCLUDE_DIRS})

find_package(Qt5XmlPatterns REQUIRED)
include_directories(${Qt5XmlPatterns_INCLUDE_DIRS})

pkg_check_modules(APPARMOR REQUIRED libapparmor REQUIRED)
include_directories(${APPARMOR_INCLUDE_DIRS})

pkg_check_modules(GSETTINGS_QT REQUIRED gsettings-qt REQUIRED)
include_directories(${GSETTINGS_QT_INCLUDE_DIRS})

find_package(Qt5Test REQUIRED)
include_directories(${Qt5Test_INCLUDE_DIRS})

find_package(Qt5Qml REQUIRED)
include_directories(${Qt5Qml_INCLUDE_DIRS})

pkg_check_modules(GLIB2 REQUIRED glib-2.0)
include_directories(${GLIB2_INCLUDE_DIRS})

pkg_check_modules(GOBJECT2 REQUIRED gobject-2.0)
include_directories(${GOBJECT2_INCLUDE_DIRS})

pkg_check_modules(CLICK REQUIRED click-0.4 REQUIRED)
include_directories(${CLICK_INCLUDE_DIRS})

pkg_check_modules(QDJANGO_DB REQUIRED qdjango-db REQUIRED)
include_directories(${QDJANGO_DB_INCLUDE_DIRS})

pkg_check_modules(QTDBUSTEST REQUIRED libqtdbustest-1 REQUIRED)
include_directories(${QTDBUSTEST_INCLUDE_DIRS})

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(DATA_DIR "${CMAKE_CURRENT_SOURCE_DIR}/data")

set(INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")
set(INCLUDE_BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}/include")
include_directories(${INCLUDE_DIR})
include_directories(${INCLUDE_BINARY_DIR})

set(SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(SOURCE_BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}/src")


include(GNUInstallDirs)
include(Coverage)

# Workaround for libexecdir on debian
if (EXISTS "/etc/debian_version") 
  set(CMAKE_INSTALL_LIBEXECDIR ${CMAKE_INSTALL_LIBDIR})
  set(CMAKE_INSTALL_FULL_LIBEXECDIR "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBEXECDIR}")
endif()

add_definitions(
	-std=c++11
	-fno-permissive
	-pedantic
	-Wall
	-Wextra
	-DLOCALEDIR="${CMAKE_INSTALL_FULL_DATADIR}/locale"
)

set(DOXYFILE_SOURCE_DIR "include")
set(DOXYFILE_EXTRA_SOURCES "doc")
include(UseDoxygen)
if(DOXYGEN_FOUND AND BUILD_DOXYGEN)
	install(
		DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/doc/html
		DESTINATION ${CMAKE_INSTALL_PREFIX}/share/doc/libusermetrics-doc/
	)
endif()

add_subdirectory("po")
add_subdirectory("src")
add_subdirectory("data")

enable_testing()
add_subdirectory(tests)

ADD_CUSTOM_TARGET(
	check
	${CMAKE_CTEST_COMMAND} --force-new-ctest-process --output-on-failure
)
