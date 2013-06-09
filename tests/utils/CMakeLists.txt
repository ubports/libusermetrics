
set(
	USERMETRICS_TEST_UTILS_SRC
	QStringPrinter.cpp
	MockSignalReceiver.cpp
)

add_library(
	usermetricsoutput-test-utils
	SHARED
	${USERMETRICS_TEST_UTILS_SRC}
)

qt5_use_modules(
	usermetricsoutput-test-utils
	Core
)
