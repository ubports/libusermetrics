
set(API_VERSION 1)
set(ABI_VERSION 1)

set(USERMETRICSOUTPUT_HEADERS
	ColorTheme.h
	ColorThemeImpl.h
	DataSet.h
	UserMetrics.h
	UserMetricsImpl.h
	qvariantlistmodel.h
)

set(USERMETRICSOUTPUT_SOURCES
	ColorTheme.cpp
	ColorThemeImpl.cpp
	DataSet.cpp
	DateFactory.cpp
	DateFactoryImpl.cpp
	UserMetrics.cpp
	UserMetricsImpl.cpp
	qvariantlistmodel.cpp
)

set(USERMETRICS_OUTPUT_DEPENDENCIES
	Core
	Gui
)

# Static library version of libusermetricsoutput

add_library(
	usermetricsoutput-static
	STATIC
	${USERMETRICSOUTPUT_SOURCES}
)

qt5_use_modules(
	usermetricsoutput-static
	${USERMETRICS_OUTPUT_DEPENDENCIES}
)

set_target_properties(
	usermetricsoutput-static
	PROPERTIES
	OUTPUT_NAME usermetricsoutput
)

# Shared library version of libusermetricsoutput

add_library(
	usermetricsoutput
	SHARED
	${USERMETRICSOUTPUT_SOURCES}
)

qt5_use_modules(
	usermetricsoutput
	${USERMETRICS_OUTPUT_DEPENDENCIES}
)

set_target_properties(
	usermetricsoutput
	PROPERTIES
	VERSION ${API_VERSION}.0.0
	SOVERSION ${ABI_VERSION}
)

install(
  TARGETS usermetricsoutput
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

# Package config

set(LIBUSERMETRICSOUTPUT_PC "libusermetricsoutput-${API_VERSION}.pc")
configure_file("libusermetricsoutput.pc.in" "${CMAKE_CURRENT_BINARY_DIR}/${LIBUSERMETRICSOUTPUT_PC}" @ONLY)

install(
  FILES "${CMAKE_CURRENT_BINARY_DIR}/${LIBUSERMETRICSOUTPUT_PC}"
  DESTINATION "${CMAKE_INSTALL_LIBDIR}/pkgconfig"
)
