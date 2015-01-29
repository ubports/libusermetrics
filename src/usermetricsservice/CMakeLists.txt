
set(USERMETRICSSERVICE_SOURCES
	database/DataSet.cpp
	database/DataSource.cpp
	database/UserData.cpp
	Authentication.cpp
	DBusDataSet.cpp
	DBusDataSource.cpp
	DBusUserData.cpp
	DBusUserMetrics.cpp
	TranslationLocatorImpl.cpp
)

qt5_add_dbus_adaptor(
	USERMETRICSSERVICE_SOURCES
	"${DATA_DIR}/com.canonical.UserMetrics.xml"
	usermetricsservice/DBusUserMetrics.h
	UserMetricsService::DBusUserMetrics
	UserMetricsAdaptor
)

qt5_add_dbus_adaptor(
	USERMETRICSSERVICE_SOURCES
	"${DATA_DIR}/com.canonical.usermetrics.DataSource.xml"
	usermetricsservice/DBusDataSource.h
	UserMetricsService::DBusDataSource
	DataSourceAdaptor
)

qt5_add_dbus_adaptor(
	USERMETRICSSERVICE_SOURCES
	"${DATA_DIR}/com.canonical.usermetrics.UserData.xml"
	usermetricsservice/DBusUserData.h
	UserMetricsService::DBusUserData
	UserDataAdaptor
)

qt5_add_dbus_adaptor(
	USERMETRICSSERVICE_SOURCES
	"${DATA_DIR}/com.canonical.usermetrics.DataSet.xml"
	usermetricsservice/DBusDataSet.h
	UserMetricsService::DBusDataSet
	DataSetAdaptor
)

add_library(
	usermetricsservice
	STATIC
	${USERMETRICSSERVICE_SOURCES}
)

qt5_use_modules(
	usermetricsservice
	Core
	DBus
	Sql
)

target_link_libraries(
	usermetricsservice
	usermetricscommon
	${QDJANGO_DB_LDFLAGS}
	${APPARMOR_LDFLAGS}
	${GLIB2_LDFLAGS}
	${GOBJECT2_LDFLAGS}
	${CLICK_LDFLAGS}
)

############################
# Service executable
############################

add_executable(
	usermetricsservice-bin
	main.cpp
)

set_target_properties(
	usermetricsservice-bin
	PROPERTIES
	OUTPUT_NAME "usermetricsservice"
)

qt5_use_modules(
	usermetricsservice-bin
	Core
)

target_link_libraries(
	usermetricsservice-bin
	usermetricsservice
)

#########################
# Installation
#########################

install(
  TARGETS usermetricsservice-bin
  RUNTIME DESTINATION "${CMAKE_INSTALL_FULL_LIBEXECDIR}/libusermetrics"
)