
# Check for google test and build it locally
set(
 GTEST_ROOT_DIR
  "/usr/src/gtest" # Default value, adjustable by user with e.g., ccmake
  CACHE
  PATH
  "Path to Google test srcs"
)

set(
  GTEST_INCLUDE_DIR
  "/usr/include/gtest"
  CACHE
  PATH
  "Path to Google tests include"
)

find_path(GTEST_INCLUDE_DIR gtest/gtest.h)
if (GTEST_INCLUDE_DIR)
	set(GTEST_FOUND true)
	#FIXME - hardcoded is bad!
  add_subdirectory(
    ${GTEST_ROOT_DIR}
    gtest
  )
endif(GTEST_INCLUDE_DIR)

set(GTEST_INCLUDE_DIRS ${GTEST_INCLUDE_DIR})
set(GTEST_LIBRARIES gtest)
set(GTEST_MAIN_LIBRARIES gtest_main)
set(GTEST_BOTH_LIBRARIES ${GTEST_LIBRARIES} ${GTEST_MAIN_LIBRARIES})

