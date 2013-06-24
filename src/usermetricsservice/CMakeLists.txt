
set(USERMETRICSSERVICE_SOURCES
	#DataSet.cpp
	DataSource.cpp
	#UserData.cpp
	DBusDataSet.cpp
	DBusDataSource.cpp
	DBusUserData.cpp
	DBusUserMetrics.cpp
	main.cpp
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
	"${DATA_DIR}/com.canonical.UserMetrics.DataSource.xml"
	usermetricsservice/DBusDataSource.h
	UserMetricsService::DBusDataSource
	DataSourceAdaptor
)

add_executable(
	usermetricsservice
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
	${QDJANGO_DB_LIBRARIES}
)
