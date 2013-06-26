
set(
	USERMETRICSSERVICE_UNIT_TESTS_SRC
	TestUserMetricsService.cpp
)

add_executable(
	usermetricsservice-unit-tests
	${USERMETRICSSERVICE_UNIT_TESTS_SRC}
)

target_link_libraries(
	usermetricsservice-unit-tests
	usermetricsservice
	${GTEST_BOTH_LIBRARIES}
)

add_valgrind_test(
	usermetricsservice-unit-tests
	usermetricsservice-unit-tests
)
