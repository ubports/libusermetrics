
FILE(
	GLOB USERMETRICSOUTPUT_PUBLIC_HEADERS
	RELATIVE "${INCLUDE_DIR}/libusermetricsoutput"
	"${INCLUDE_DIR}/libusermetricsoutput/*.h"
)

set(USERMETRICSOUTPUT_SOURCES
	ColorTheme.cpp
	ColorThemeImpl.cpp
	ColorThemeProvider.cpp
	GSettingsColorThemeProvider.cpp
	DataSet.cpp
	DataSource.cpp
	SyncedDataSet.cpp
	SyncedDataSource.cpp
	SyncedUserMetricsStore.cpp
	SyncedUserData.cpp
	UserData.cpp
	UserMetrics.cpp
	UserMetricsImpl.cpp
	UserMetricsStore.cpp
	qvariantlistmodel.cpp
)

set(USERMETRICS_OUTPUT_DEPENDENCIES
	Qt5::Core
	Qt5::Gui
	Qt5::DBus
	Qt5::XmlPatterns
)

# Static library version of libusermetricsoutput

add_library(
	usermetricsoutput-static
	STATIC
	${USERMETRICSOUTPUT_SOURCES}
)

target_link_libraries(
	usermetricsoutput-static
	${USERMETRICS_OUTPUT_DEPENDENCIES}
	usermetricscommon
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

target_link_libraries(
	usermetricsoutput
	${USERMETRICS_OUTPUT_DEPENDENCIES}
	usermetricscommon
	${GSETTINGS_QT_LIBRARIES}
	${Gettext_LIBRARIES}
)

set_target_properties(
	usermetricsoutput
	PROPERTIES
	VERSION ${API_VERSION}.0.0
	SOVERSION ${ABI_VERSION}
	PUBLIC_HEADER "${USERMETRICSOUTPUT_PUBLIC_HEADERS}"
)

install(
  TARGETS usermetricsoutput
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  PUBLIC_HEADER DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/libusermetrics-${API_VERSION}/libusermetricsoutput"
)

# Package config

set(LIBUSERMETRICSOUTPUT_PC "libusermetricsoutput-${API_VERSION}.pc")
configure_file("libusermetricsoutput.pc.in" "${CMAKE_CURRENT_BINARY_DIR}/${LIBUSERMETRICSOUTPUT_PC}" @ONLY)

install(
  FILES "${CMAKE_CURRENT_BINARY_DIR}/${LIBUSERMETRICSOUTPUT_PC}"
  DESTINATION "${CMAKE_INSTALL_LIBDIR}/pkgconfig"
)
