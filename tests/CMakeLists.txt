include(FindGMock)

find_package("Valgrind" REQUIRED)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}
                    ${GMOCK_INCLUDE_DIR}
                    ${GTEST_INCLUDE_DIR}
)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")

add_definitions(-DDBUS_SYSTEM_CONFIG_FILE="${CMAKE_CURRENT_SOURCE_DIR}/data/system.conf")
add_definitions(-DDBUS_SESSION_CONFIG_FILE="${CMAKE_CURRENT_SOURCE_DIR}/data/session.conf")

add_definitions(-DUSERMETRICSSERVICE_BINARY="${SOURCE_BINARY_DIR}/usermetricsservice/usermetricsservice")

add_definitions(-DTEST_LOCALEDIR="${CMAKE_CURRENT_BINARY_DIR}/data")

add_subdirectory(data)
add_subdirectory(integration)
add_subdirectory(unit)
add_subdirectory(testutils)
