
set(
	USERMETRICS_TEST_UTILS_SRC
	DBusTest.cpp
	QColorPrinter.cpp
	QStringPrinter.cpp
	QVariantPrinter.cpp
	QVariantListPrinter.cpp
	main.cpp
)

add_library(
	usermetrics-test-utils
	SHARED
	${USERMETRICS_TEST_UTILS_SRC}
)

target_link_libraries(
	usermetrics-test-utils
	Qt5::Core
	Qt5::Gui
	Qt5::DBus
	Qt5::Test
	${QTDBUSTEST_LIBRARIES}
)
