
#set(USERMETRICSOUTPUT_HEADERS
#InfographicModelPrivate.h
#InfographicModelImpl.h
#qvariantlistmodel.h
#)

set(USERMETRICSOUTPUT_SOURCES
InfographicColorTheme.cpp
InfographicColorThemeImpl.cpp
InfographicData.cpp
InfographicModel.cpp
InfographicModelImpl.cpp
InfographicModelPrivate.cpp
qvariantlistmodel.cpp
)

add_library(
	usermetricsoutput
	SHARED
	${USERMETRICSOUTPUT_SOURCES}
)

#set_target_properties(
#	usermetricsoutput
#  VERSION ${API_VERSION}.0.0
#  SOVERSION ${ABI_VERSION}
#)

qt5_use_modules(
	usermetricsoutput
	Core
	Gui
)
