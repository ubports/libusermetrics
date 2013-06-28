
set(
	USERMETRICSOUTPUT_INTEGRATION_TESTS_SRC
	TestUserMetrics.cpp
)

add_executable(
	usermetricsoutput-integration-tests
	${USERMETRICSOUTPUT_INTEGRATION_TESTS_SRC}
)

target_link_libraries(
	usermetricsoutput-integration-tests
	usermetricsoutput
	usermetrics-test-utils
	${GTEST_LIBRARIES}
	${GMOCK_LIBRARIES}
)

add_test(
	usermetricsoutput-integration-tests
	usermetricsoutput-integration-tests
)
