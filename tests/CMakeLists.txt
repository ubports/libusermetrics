set(CMAKE_AUTOMOC OFF)
include(FindGMock)
set(CMAKE_AUTOMOC ON)

find_package("Valgrind" REQUIRED)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${GMOCK_INCLUDE_DIRS})
include_directories(${GTEST_INCLUDE_DIRS})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")

add_definitions(-DUSERMETRICSSERVICE_BINARY="${SOURCE_BINARY_DIR}/usermetricsservice/usermetricsservice")

add_definitions(-DTEST_LOCALEDIR="${CMAKE_CURRENT_BINARY_DIR}/data")
add_definitions(-DTEST_DATADIR="${CMAKE_CURRENT_SOURCE_DIR}/data")

add_definitions(-DUSERMETRICS_TEST_COLOR_BASEDIR="${DATA_DIR}/themes")

add_subdirectory(data)
add_subdirectory(integration)
add_subdirectory(unit)
add_subdirectory(testutils)
