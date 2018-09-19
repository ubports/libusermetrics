
set(USERMETRICS_COMMON_SOURCES
	DateFactory.cpp
	DateFactoryImpl.cpp
	DBusPaths.cpp
	Localisation.cpp
)

set(USERMETRICS_COMMON_DEPENDENCIES
	Qt5::Core
)

qt5_add_dbus_interface(
	USERMETRICS_COMMON_SOURCES
	"${DATA_DIR}/com.canonical.UserMetrics.xml"
	UserMetricsInterface
)

qt5_add_dbus_interface(
	USERMETRICS_COMMON_SOURCES
	"${DATA_DIR}/com.canonical.usermetrics.DataSource.xml"
	DataSourceInterface
)

qt5_add_dbus_interface(
	USERMETRICS_COMMON_SOURCES
	"${DATA_DIR}/com.canonical.usermetrics.UserData.xml"
	UserDataInterface
)

qt5_add_dbus_interface(
	USERMETRICS_COMMON_SOURCES
	"${DATA_DIR}/com.canonical.usermetrics.DataSet.xml"
	DataSetInterface
)

add_library(
	usermetricscommon
	STATIC
	${USERMETRICS_COMMON_SOURCES}
)

target_link_libraries(
	usermetricscommon
	${USERMETRICS_COMMON_DEPENDENCIES}
)

