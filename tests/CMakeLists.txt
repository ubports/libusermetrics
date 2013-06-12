
set(CMAKE_AUTOMOC OFF)
find_package("LocalGTest" REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})
set(CMAKE_AUTOMOC ON)

find_package("GMock" REQUIRED)
include_directories(${GMOCK_INCLUDE_DIRS})

find_package("Valgrind" REQUIRED)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")

add_subdirectory(integration)
add_subdirectory(unit)
add_subdirectory(utils)
