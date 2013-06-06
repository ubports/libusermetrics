
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")

set(
	USERMETRICSOUTPUT_TESTS_SRC
	TestInfographicModel.cpp
)

add_executable(
	usermetricsoutput-tests
	${USERMETRICSOUTPUT_TESTS_SRC}
)

target_link_libraries(
	usermetricsoutput-tests
	usermetricsoutput
	${GTEST_BOTH_LIBRARIES}
)

add_test(
	usermetricsoutput-tests
	usermetricsoutput-tests
)
