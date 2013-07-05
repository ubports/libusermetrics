cmake_minimum_required(VERSION 2.8.9)

find_package(XGettext REQUIRED)

set(
	POT_FILE
	"${CMAKE_CURRENT_SOURCE_DIR}/libusermetrics.pot"
)

file(
	GLOB PO_FILES
	*.po
)

FILE(
	GLOB_RECURSE SRC_FILES
	RELATIVE ${CMAKE_SOURCE_DIR}
	${SOURCE_DIR}/*.cpp
	${SOURCE_DIR}/*.c
	${SOURCE_DIR}/*.h
)

xgettext_create_pot_file(
	${POT_FILE}
	CPP
	QT
	INPUT ${SRC_FILES}
	WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
	ADD_COMMENTS "TRANSLATORS"
	KEYWORD "_"
	PACKAGE_NAME ${GETTEXT_PACKAGE}
	COPYRIGHT_HOLDER "Canonical Ltd."
)

add_custom_target(update-pot DEPENDS ${POT_FILE})

gettext_create_translations(
	${POT_FILE}
	ALL
	${PO_FILES}
)
