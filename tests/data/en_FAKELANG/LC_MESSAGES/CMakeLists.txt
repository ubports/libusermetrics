
find_package(Gettext REQUIRED)

add_custom_command(
	OUTPUT
		foo.mo
	COMMAND
		${GETTEXT_MSGFMT_EXECUTABLE}
		--check
		--output-file foo.mo
		"${CMAKE_CURRENT_SOURCE_DIR}/foo.po"
)

add_custom_target(foo-po-file
	ALL
	DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/foo.mo"
)