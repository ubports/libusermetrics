find_package(DoxygenBuilder)

add_doxygen(
    qtdoc
    PROJECT_NAME
        "libusermetrics"
    INPUT
        "${CMAKE_SOURCE_DIR}/include"
        "${CMAKE_CURRENT_SOURCE_DIR}/pages"
    EXAMPLE_PATH
        "${CMAKE_SOURCE_DIR}/examples/"
    STRIP_FROM_PATH
        "${CMAKE_SOURCE_DIR}/include"
    STRIP_FROM_INC_PATH
        "${CMAKE_SOURCE_DIR}/include"
    EXCLUDE_PATTERNS
        */internal/*
    EXCLUDE_SYMBOLS
        *::internal*
        *::Priv
    INSTALL
        ${CMAKE_INSTALL_DATAROOTDIR}/doc/libusermetrics-doc
    XML
    ALL
)

