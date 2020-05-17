
set(
	USERMETRICSSERVICE_UNIT_TESTS_SRC
	TestAuthentication.cpp
	TestUserMetricsService.cpp
)

add_executable(
	usermetricsservice-unit-tests
	${USERMETRICSSERVICE_UNIT_TESTS_SRC}
)

target_link_libraries(
	usermetricsservice-unit-tests
	-pthread
	usermetricsservice
	usermetrics-test-utils
	${QTDBUSTEST_LIBRARIES}
	${GTEST_LIBRARIES}
	${GMOCK_LIBRARIES}
)

add_test(
	usermetricsservice-unit-tests
	usermetricsservice-unit-tests
)
