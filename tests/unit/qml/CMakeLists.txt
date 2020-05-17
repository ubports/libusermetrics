
add_executable(tst_QmlTests tst_QmlTests.cpp)
target_link_libraries(tst_QmlTests Qt5::Core Qt5::Qml Qt5::Quick Qt5::Test Qt5::QuickTest ${TPL_QT5_LIBRARIES})

add_test(tst_QmlTests
	${CMAKE_CURRENT_BINARY_DIR}/tst_QmlTests
	-input ${CMAKE_CURRENT_SOURCE_DIR}
	-import ${SOURCE_BINARY_DIR}/modules
)
set_tests_properties(tst_QmlTests PROPERTIES ENVIRONMENT "QT_QPA_PLATFORM=minimal")

add_subdirectory(UserMetricsTest)
