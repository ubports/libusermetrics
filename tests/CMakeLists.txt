
find_package("LocalGTest" REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

include(CTest)
enable_testing()

add_subdirectory(libusermetrics-input)
add_subdirectory(libusermetrics-output)