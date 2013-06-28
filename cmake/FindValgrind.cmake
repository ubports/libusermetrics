
option(
  ENABLE_MEMCHECK_OPTION
  "If set to ON, enables automatic creation of memcheck targets"
  OFF
)

find_program(
	VALGRIND_PROGRAM
	NAMES valgrind
)

if(VALGRIND_PROGRAM)
	set(VALGRIND_PROGRAM_OPTIONS "--suppressions=${CMAKE_SOURCE_DIR}/tests/data/valgrind.suppression" "--error-exitcode=1" "--leak-check=full" "--quiet")
endif()

find_package_handle_standard_args(
	VALGRIND DEFAULT_MSG
	VALGRIND_PROGRAM
)

function(add_valgrind_test NAME EXECUTABLE)
	if(ENABLE_MEMCHECK_OPTION)
		add_test(${NAME} ${VALGRIND_PROGRAM} ${VALGRIND_PROGRAM_OPTIONS} "${CMAKE_CURRENT_BINARY_DIR}/${EXECUTABLE}")
	else()
		add_test(${NAME} ${EXECUTABLE})
	endif()
endfunction()
