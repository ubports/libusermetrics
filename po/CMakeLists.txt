include(UseXGettext)

add_translations_directory(${GETTEXT_PACKAGE})

add_translations_catalog(
    GETTEXT_PACKAGE ${GETTEXT_PACKAGE}
    COPYRIGHT_HOLDER "Canonical Ltd."
    SOURCE_DIRECTORIES "${CMAKE_SOURCE_DIR}/src"
)

