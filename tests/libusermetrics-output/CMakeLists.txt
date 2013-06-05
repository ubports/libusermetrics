
add_executable(usermetrics-output-tests foo.cpp)
target_link_libraries(
	usermetrics-output-tests
	usermetrics-output
	${GTEST_BOTH_LIBRARIES}
)

add_test(usermetrics-output-tests usermetrics-output-tests)
