
set(
	USERMETRICS_OUTPUT_TESTS_SRC
	foo.cpp
)

add_executable(
	usermetrics-output-tests
	${USERMETRICS_OUTPUT_TESTS_SRC}
)

target_link_libraries(
	usermetrics-output-tests
	usermetrics-output
	${GTEST_BOTH_LIBRARIES}
)

add_test(
	usermetrics-output-tests
	usermetrics-output-tests
)
