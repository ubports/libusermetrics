
find_package("LocalGTest" REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

add_subdirectory(libusermetricsinput)
add_subdirectory(libusermetricsoutput)
