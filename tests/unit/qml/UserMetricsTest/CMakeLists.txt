set(testplugin_SRCS
    TestPlugin.cpp
    DBusQuery.cpp
    MetricInfo.cpp
    )

set(testplugin_HDRS
    TestPlugin.h
    DBusQuery.h
    MetricInfo.h
    )

add_library(usermetrics-qml-test SHARED ${testplugin_SRCS} ${testplugin_HDRS})

target_link_libraries(
        usermetrics-qml-test
        Qt5::Core Qt5::Qml Qt5::Quick
        Qt5::Test Qt5::DBus Qt5::QuickTest
        usermetricsinput-static
        usermetrics-test-utils
        ${GTEST_LIBRARIES}
        ${GMOCK_LIBRARIES}
        )

# copy qmldir file to build dir
configure_file(
	qmldir
	qmldir
)
