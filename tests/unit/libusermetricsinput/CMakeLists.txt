
set(
	USERMETRICSINPUT_UNIT_TESTS_SRC
	TestMetricImpl.cpp
	TestMetricManagerImpl.cpp
	TestMetricUpdateImpl.cpp
)

add_executable(
	usermetricsinput-unit-tests
	${USERMETRICSINPUT_UNIT_TESTS_SRC}
)

target_link_libraries(
	usermetricsinput-unit-tests
	usermetricsinput-static
	usermetrics-test-utils
	${GTEST_LIBRARIES}
	${GMOCK_BOTH_LIBRARIES}
)

add_valgrind_test(
	usermetricsinput-unit-tests
	usermetricsinput-unit-tests
)
