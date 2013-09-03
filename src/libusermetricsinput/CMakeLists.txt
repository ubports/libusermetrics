
set(EXPORT_FILE_NAME UserMetricsInputExport.h)

FILE(
	GLOB USERMETRICSINPUT_PUBLIC_HEADERS
	RELATIVE "${INCLUDE_DIR}/libusermetricsinput"
	"${INCLUDE_DIR}/libusermetricsinput/*.h"
)

list(
	APPEND USERMETRICSINPUT_PUBLIC_HEADERS
	"${CMAKE_CURRENT_BINARY_DIR}/${EXPORT_FILE_NAME}"
)

set(USERMETRICSINPUT_SOURCES
	Metric.cpp
	MetricImpl.cpp
	MetricManager.cpp
	MetricManagerImpl.cpp
	MetricManagerImpl.cpp
	MetricUpdate.cpp
	MetricUpdateImpl.cpp
	usermetricsinput.cpp
)

set(USERMETRICS_INPUT_DEPENDENCIES
	Core
	DBus
)

# Static library version of libusermetricsinput

add_library(
	usermetricsinput-static
	STATIC
	${USERMETRICSINPUT_SOURCES}
)

qt5_use_modules(
	usermetricsinput-static
	${USERMETRICS_INPUT_DEPENDENCIES}
)

target_link_libraries(
	usermetricsinput-static
	usermetricscommon
	${Gettext_LIBRARIES}
)

set_target_properties(
	usermetricsinput-static
	PROPERTIES
	INPUT_NAME usermetricsinput
)

# Shared library version of libusermetricsinput

add_library(
	usermetricsinput
	SHARED
	${USERMETRICSINPUT_SOURCES}
)

generate_export_header(
	usermetricsinput
	EXPORT_FILE_NAME ${EXPORT_FILE_NAME}
)

configure_file(
	"${CMAKE_CURRENT_BINARY_DIR}/${EXPORT_FILE_NAME}"
	"${INCLUDE_BINARY_DIR}/libusermetricsinput/${EXPORT_FILE_NAME}"
)

qt5_use_modules(
	usermetricsinput
	${USERMETRICS_INPUT_DEPENDENCIES}
)

target_link_libraries(
	usermetricsinput
	usermetricscommon
)

set_target_properties(
	usermetricsinput
	PROPERTIES
	VERSION ${API_VERSION}.0.0
	SOVERSION ${ABI_VERSION}
	PUBLIC_HEADER "${USERMETRICSINPUT_PUBLIC_HEADERS}"
)

install(
  TARGETS usermetricsinput
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  PUBLIC_HEADER DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/libusermetrics-${API_VERSION}/libusermetricsinput"
)

# Package config

set(LIBUSERMETRICSINPUT_PC "libusermetricsinput-${API_VERSION}.pc")
configure_file("libusermetricsinput.pc.in" "${CMAKE_CURRENT_BINARY_DIR}/${LIBUSERMETRICSINPUT_PC}" @ONLY)

install(
  FILES "${CMAKE_CURRENT_BINARY_DIR}/${LIBUSERMETRICSINPUT_PC}"
  DESTINATION "${CMAKE_INSTALL_LIBDIR}/pkgconfig"
)

##########################
# Command line interface
##########################

add_executable(
	usermetricsinput-bin
	main.cpp
)

set_target_properties(
	usermetricsinput-bin
	PROPERTIES
	OUTPUT_NAME "usermetricsinput"
)

install(
  TARGETS usermetricsinput-bin
  RUNTIME DESTINATION bin
)

qt5_use_modules(
	usermetricsinput-bin
	Core
)

target_link_libraries(
	usermetricsinput-bin
	usermetricsinput
)

##########################
# Increment CLI
##########################

add_executable(
	usermetricsinput-increment
	main-increment.cpp
)

install(
  TARGETS usermetricsinput-increment
  RUNTIME DESTINATION bin
)

qt5_use_modules(
	usermetricsinput-increment
	Core
)

target_link_libraries(
	usermetricsinput-increment
	usermetricsinput
)

