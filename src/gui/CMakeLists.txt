cmake_minimum_required (VERSION 2.8)

set (GUI_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src/gui")

set (GUI_HEADER_FILES
    ${GUI_SOURCE_DIR}/GUIDEFS.h
    ${GUI_SOURCE_DIR}/ZoneSelectDialog.h
    ${GUI_SOURCE_DIR}/DialogDropper.h
)

set (GUI_SOURCE_FILES
    ${GUI_SOURCE_DIR}/ZoneSelectDialog.cpp
    ${GUI_SOURCE_DIR}/DialogDropper.cpp
    ${GUI_SOURCE_DIR}/guicb.cpp
    ${GUI_SOURCE_DIR}/guidefs.cpp
)

list (APPEND HEADER_FILES ${GUI_HEADER_FILES})
list (APPEND SOURCE_FILES ${GUI_SOURCE_FILES})

source_group (gui FILES ${GUI_HEADER_FILES} ${GUI_SOURCE_FILES})
