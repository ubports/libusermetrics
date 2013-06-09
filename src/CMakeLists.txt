
include(GenerateExportHeader)

include_directories(${SOURCE_DIR})

add_compiler_export_flags()

add_subdirectory("libusermetricsinput")
add_subdirectory("libusermetricsoutput")
