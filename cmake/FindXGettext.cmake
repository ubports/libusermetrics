
find_package(Gettext REQUIRED)

find_program(XGETTEXT_EXECUTABLE xgettext)

if(XGETTEXT_EXECUTABLE)
	execute_process(
		COMMAND ${XGETTEXT_EXECUTABLE} --version
		OUTPUT_VARIABLE xgettext_version
		ERROR_QUIET
		OUTPUT_STRIP_TRAILING_WHITESPACE)
	if (xgettext_version MATCHES "^xgettext \\(.*\\) [0-9]")
		string(
			REGEX REPLACE "^xgettext \\([^\\)]*\\) ([0-9\\.]+[^ \n]*).*" "\\1"
			XGETTEXT_VERSION_STRING "${xgettext_version}"
		)
	endif()
	unset(xgettext_version)
endif()

include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(
	XGettext
	REQUIRED_VARS XGETTEXT_EXECUTABLE
	VERSION_VAR XGETTEXT_VERSION_STRING
)

function(XGETTEXT_CREATE_POT_FILE _potFile)
	set(_options ALL QT CPP)
	set(_oneValueArgs ADD_COMMENTS KEYWORD PACKAGE_NAME COPYRIGHT_HOLDER WORKING_DIRECTORY)
	set(_multiValueArgs INPUT)

	CMAKE_PARSE_ARGUMENTS(_ARG "${_options}" "${_oneValueArgs}" "${_multiValueArgs}" ${ARGN})
	
	set(_QT "")
	if(_ARG_QT)
		set(_QT "--qt")
	endif()
	
	set(_CPP "")
	if(_ARG_CPP)
		set(_CPP "--c++")
	endif()
	
	set(_KEYWORD "")
	if(_ARG_KEYWORD)
		set(_KEYWORD --keyword="${_ARG_KEYWORD}")
	endif()
	
	set(_ADD_COMMENTS "")
	if(_ARG_ADD_COMMENTS)
		set(_ADD_COMMENTS --add-comments="${_ARG_ADD_COMMENTS}")
	endif()
	
	set(_PACKAGE_NAME "")
	if(_ARG_PACKAGE_NAME)
		set(_PACKAGE_NAME --package-name="${_ARG_PACKAGE_NAME}")
	endif()
	
	set(_COPYRIGHT_HOLDER "")
	if(_ARG_COPYRIGHT_HOLDER)
		set(_COPYRIGHT_HOLDER --copyright-holder="${_ARG_COPYRIGHT_HOLDER}")
	endif()

	add_custom_command(
		OUTPUT "${_potFile}"
		COMMAND ${XGETTEXT_EXECUTABLE} --output="${_potFile}" ${_KEYWORD} ${_PACKAGE_NAME} ${_COPYRIGHT_HOLDER} ${_QT} ${_CPP} ${_ADD_COMMENTS} ${_ARG_INPUT}
		WORKING_DIRECTORY ${_ARG_WORKING_DIRECTORY}
	)
	
	_GETTEXT_GET_UNIQUE_TARGET_NAME(_potFile _uniqueTargetName)
	
	if(_ARG_ALL)
		add_custom_target(${_uniqueTargetName} ALL DEPENDS ${_potFile})
	else()
		add_custom_target(${_uniqueTargetName} DEPENDS ${_potFile})
	endif()

endfunction()
