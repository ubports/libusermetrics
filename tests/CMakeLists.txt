set(CMAKE_AUTOMOC OFF)
find_package(GMock)
set(CMAKE_AUTOMOC ON)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${GMOCK_INCLUDE_DIRS})
include_directories(${GTEST_INCLUDE_DIRS})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")

add_definitions(-DUSERMETRICSSERVICE_BINARY="${SOURCE_BINARY_DIR}/usermetricsservice/usermetricsservice")

add_definitions(-DTEST_LOCALEDIR="${CMAKE_CURRENT_BINARY_DIR}/data")
add_definitions(-DTEST_DATADIR="${CMAKE_CURRENT_SOURCE_DIR}/data")

add_definitions(-DDATA_DIR="${DATA_DIR}")

add_subdirectory(data)
add_subdirectory(integration)
add_subdirectory(unit)
add_subdirectory(testutils)
