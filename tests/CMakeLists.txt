# Build with system gmock and embedded gtest
set (GMOCK_INCLUDE_DIR "/usr/include/gmock/include" CACHE PATH "gmock source include directory")
set (GMOCK_SOURCE_DIR "/usr/src/gmock" CACHE PATH "gmock source directory")
set (GTEST_INCLUDE_DIR "${GMOCK_SOURCE_DIR}/gtest/include" CACHE PATH "gtest source include directory")

add_subdirectory(${GMOCK_SOURCE_DIR} "${CMAKE_CURRENT_BINARY_DIR}/gmock")
include_directories (
  ${GMOCK_INCLUDE_DIR}
  ${GTEST_INCLUDE_DIR}
)

set(GTEST_LIBRARIES gtest)
set(GTEST_MAIN_LIBRARIES gtest_main)
set(GMOCK_LIBRARIES gmock gmock_main)

find_package("Valgrind" REQUIRED)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")

add_definitions(-DDBUS_SYSTEM_CONFIG_FILE="${CMAKE_CURRENT_SOURCE_DIR}/data/system.conf")
add_definitions(-DDBUS_SESSION_CONFIG_FILE="${CMAKE_CURRENT_SOURCE_DIR}/data/session.conf")

add_definitions(-DUSERMETRICSSERVICE_BINARY="${SOURCE_BINARY_DIR}/usermetricsservice/usermetricsservice")

add_subdirectory(integration)
add_subdirectory(unit)
add_subdirectory(testutils)
