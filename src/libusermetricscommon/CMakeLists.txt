
set(USERMETRICS_COMMON_SOURCES
	DateFactory.cpp
	DateFactoryImpl.cpp
)

set(USERMETRICS_COMMON_DEPENDENCIES
	Core
)

add_library(
	usermetricscommon
	STATIC
	${USERMETRICS_COMMON_SOURCES}
)

qt5_use_modules(
	usermetricscommon
	${USERMETRICS_COMMON_DEPENDENCIES}
)

