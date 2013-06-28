
set(
	USERMETRICSINPUT_INTEGRATION_TESTS_SRC
	TestMetricManager.cpp
)

add_executable(
	usermetricsinput-integration-tests
	${USERMETRICSINPUT_INTEGRATION_TESTS_SRC}
)

target_link_libraries(
	usermetricsinput-integration-tests
	usermetricsinput
	usermetrics-test-utils
	${GTEST_LIBRARIES}
	${GMOCK_LIBRARIES}
)

add_test(
	usermetricsinput-integration-tests
	usermetricsinput-integration-tests
)
