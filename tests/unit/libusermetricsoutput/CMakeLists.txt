
set(
	USERMETRICSOUTPUT_UNIT_TESTS_SRC
	QModelListener.cpp
	TestColorThemeImpl.cpp
	TestQVariantListModel.cpp
	TestUserMetricsImpl.cpp
	TestSyncedUserMetricsStore.cpp
)

add_executable(
	usermetricsoutput-unit-tests
	${USERMETRICSOUTPUT_UNIT_TESTS_SRC}
)

target_link_libraries(
	usermetricsoutput-unit-tests
	usermetricsoutput-static
	usermetrics-test-utils
	${GTEST_LIBRARIES}
	${GMOCK_BOTH_LIBRARIES}
)

add_valgrind_test(
	usermetricsoutput-unit-tests
	usermetricsoutput-unit-tests
)
