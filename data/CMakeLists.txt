include("UseGSettings")

###########################
# Settings
###########################

add_schema("com.canonical.UserMetrics.gschema.xml")

install(
	DIRECTORY
		libusermetrics
	DESTINATION
		${CMAKE_INSTALL_DATADIR}
)

###########################
# Policy
###########################

set(
	DBUSCONFDIR
	/etc/dbus-1/system.d
)

install(
	FILES
	"com.canonical.UserMetrics.conf"
	DESTINATION ${DBUSCONFDIR}
)

###########################
# Dbus Interfaces
###########################

set(
	DBUSIFACEDIR
	"${CMAKE_INSTALL_DATADIR}/dbus-1/interfaces/"
)

install(
	FILES
	com.canonical.usermetrics.DataSet.xml
	com.canonical.usermetrics.DataSource.xml
	com.canonical.usermetrics.UserData.xml
	com.canonical.UserMetrics.xml
	DESTINATION ${DBUSIFACEDIR}
)

###########################
# Dbus Services
###########################

set(
	DBUSSERVICEDIR
	"${CMAKE_INSTALL_DATADIR}/dbus-1/system-services/"
)

set(USERMETRICS_SERVICE
	"${CMAKE_CURRENT_BINARY_DIR}/com.canonical.UserMetrics.service"
)

configure_file(
	"com.canonical.UserMetrics.service.in"
	${USERMETRICS_SERVICE}
	@ONLY
)

install(
	FILES
	${USERMETRICS_SERVICE}
	DESTINATION ${DBUSSERVICEDIR}
)
