
set(USERMETRICSOUTPUT_HEADERS
InfographicModelPrivate.h
qvariantlistmodel.h
)

set(USERMETRICSOUTPUT_SOURCES
InfographicModel.cpp
InfographicModelPrivate.cpp
qvariantlistmodel.cpp
)

add_library(
	usermetricsoutput
	SHARED
	${USERMETRICSOUTPUT_SOURCES}
)

set_target_properties(
	usermetricsoutput
	PROPERTIES
	AUTOMOC_MOC_OPTIONS "-Ifoo"
#  VERSION ${API_VERSION}.0.0
#  SOVERSION ${ABI_VERSION}
)

qt5_use_modules(
	usermetricsoutput
	Core
	Gui
)

#target_link_libraries(usermetricsoutput
#
#)