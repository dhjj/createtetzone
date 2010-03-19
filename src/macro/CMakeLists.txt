cmake_minimum_required (VERSION 2.8)

set (MACRO_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src/macro")

set (MACRO_SOURCE_FILES
    ${MACRO_SOURCE_DIR}/ZoneListSerializer.h
    ${MACRO_SOURCE_DIR}/ZoneListSerializer.cpp
    ${MACRO_SOURCE_DIR}/MacroCommandParser.h
    ${MACRO_SOURCE_DIR}/MacroCommandParser.cpp
    ${MACRO_SOURCE_DIR}/MacroCommandGenerator.h
    ${MACRO_SOURCE_DIR}/MacroCommandGenerator.cpp
)

list (APPEND SOURCE_FILES ${MACRO_SOURCE_FILES})

source_group (macro FILES ${MACRO_HEADER_FILES} ${MACRO_SOURCE_FILES})
