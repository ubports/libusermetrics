
set(USERMETRICS_OUTPUT_HEADERS
InfographicModelPrivate.h
qvariantlistmodel.h
)

set(USERMETRICS_OUTPUT_SOURCES
InfographicModel.cpp
InfographicModelPrivate.cpp
qvariantlistmodel.cpp
)

add_library(usermetrics-output SHARED ${USERMETRICS_OUTPUT_SOURCES})

#set_target_properties(hud PROPERTIES
#  VERSION ${API_VERSION}.0.0
#  SOVERSION ${ABI_VERSION}
#  OUTPUT_NAME "hud"
#)

qt5_use_modules(usermetrics-output Core)

#target_link_libraries(usermetrics-output
#
#)