
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

qt5_use_modules(
	usermetrics-test-utils
	Core
	Gui
	DBus
	Test
)

target_link_libraries(
	usermetrics-test-utils
	${QTDBUSTEST_LIBRARIES}
)