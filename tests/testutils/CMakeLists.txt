
set(
	USERMETRICS_TEST_UTILS_SRC
	QColorPrinter.cpp
	QStringPrinter.cpp
	QVariantPrinter.cpp
	MockSignalReceiver.cpp
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
)
