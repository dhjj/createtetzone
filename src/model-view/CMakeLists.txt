cmake_minimum_required (VERSION 2.8)
project (model-view)

set (SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}")

set (HEADER_FILES
    ${SOURCE_DIR}/ListModelAbstract.h
    ${SOURCE_DIR}/ListModelGenericInterface.h
    ${SOURCE_DIR}/ListModelInterface.h
    ${SOURCE_DIR}/ListModelObserverInterface.h
    ${SOURCE_DIR}/ListModelPreMapped.h
    ${SOURCE_DIR}/ListModelViewFwd.h
    ${SOURCE_DIR}/ListModelViewTypedefs.h
    ${SOURCE_DIR}/ListMultiSelectionModel.h
    ${SOURCE_DIR}/ListSelectionModelAbstract.h
    ${SOURCE_DIR}/ListSelectionModelInterface.h
    ${SOURCE_DIR}/ListSelectionModelObserverInterface.h
    ${SOURCE_DIR}/ListSingleSelectionModel.h
    ${SOURCE_DIR}/ListView.h
    ${SOURCE_DIR}/ListViewInterface.h
    ${SOURCE_DIR}/StateChangeListener.h
    ${SOURCE_DIR}/ZoneListModel.h
)

set (SOURCE_FILES
    ${SOURCE_DIR}/ListMultiSelectionModel.cpp
    ${SOURCE_DIR}/ListSelectionModelAbstract.cpp
    ${SOURCE_DIR}/ListSingleSelectionModel.cpp
    ${SOURCE_DIR}/ListView.cpp
    ${SOURCE_DIR}/StateChangeListener.cpp
    ${SOURCE_DIR}/ZoneListModel.cpp
)

# Use boost to resolve the tr1 library. This buys us a couple of
# things:
# 1. TR1 on platforms that don'e have it (i.e. VS2005)
# 2. A cross-platform include style (e.g. gcc uses non-standard
#    #include <tr1/memory>)
find_package(Boost REQUIRED)
set (Boost_INCLUDE_DIRS
    ${Boost_INCLUDE_DIRS}/boost/tr1/tr1
    ${Boost_INCLUDE_DIRS}
)
include_directories(${Boost_INCLUDE_DIRS})

if (UNIX)
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -fPIC")
    add_definitions(-DLINUX)
    if (CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64")
        add_definitions(-DLINUX64)
    endif ()
endif ()

# Set warning level to 4 (the highest) on Windows
if (WIN32)
      set (CMAKE_CXX_WARNING_LEVEL 4)
      if (CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
            string (REGEX REPLACE "/W[0-4]" "/W4"
                    CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
      else ()
            set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
      endif ()
endif ()

set (CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
message (STATUS "CMake module path is ${CMAKE_MODULE_PATH}")
find_package (Tecplot REQUIRED)

add_library (${PROJECT_NAME}
    STATIC
    ${HEADER_FILES}
    ${SOURCE_FILES}
)

target_link_libraries (${PROJECT_NAME}
    ${TECPLOT_LIBS}
)

include_directories(
    "${SOURCE_DIR}"
    "${TECPLOT_INCLUDE_DIR}"
)

add_subdirectory(unittest)
