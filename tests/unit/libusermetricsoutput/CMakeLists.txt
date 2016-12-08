
set(
	USERMETRICSOUTPUT_UNIT_TESTS_SRC
	QModelListener.cpp
	TestColorThemeImpl.cpp
	TestDataSet.cpp
	TestGSettingsColorThemeProvider.cpp
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
	${GSETTINGS_QT_LIBRARIES}
	${GTEST_LIBRARIES}
	${GMOCK_LIBRARIES}
)

add_test(
	usermetricsoutput-unit-tests
	usermetricsoutput-unit-tests
)
