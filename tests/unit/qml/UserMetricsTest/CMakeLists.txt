set(testplugin_SRCS
    testplugin.cpp
    dbusquery.cpp
    )

set(testplugin_HDRS
    testplugin.h
    dbusquery.h
    )

add_library(usermetrics-qml-test SHARED ${testplugin_SRCS} ${testplugin_HDRS})
qt5_use_modules(usermetrics-qml-test Core Qml Quick Test DBus QuickTest)

target_link_libraries(
        usermetrics-qml-test
        usermetricsinput-static
        usermetrics-test-utils
        ${GTEST_LIBRARIES}
        ${GMOCK_LIBRARIES}
        )

# copy qmldir file to build dir
file(COPY qmldir DESTINATION ${CMAKE_BINARY_DIR}/tests/unit/qml/UserMetricsTest)
