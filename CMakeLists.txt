cmake_minimum_required (VERSION 2.8)
project (createtetzone)

set (BUILD_TESTS FALSE CACHE BOOL "Include unit tests in the build.")

set (CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake;${CMAKE_MODULE_PATH}")

find_package (Tecplot REQUIRED)
include_directories ("${TECPLOT_INCLUDE_DIR}")

# Tecplot installer for some versions did not include some toolbox
# headers, so we include a version of these in our source
include_directories (src/missing-tecplot-headers)
include_directories (src/missing-tecplot-headers/toolbox)

find_package(VTK REQUIRED)
include(${VTK_USE_FILE})

set (Boost_USE_STATIC_LIBS TRUE)
find_package(Boost REQUIRED COMPONENTS program_options)
# Use boost to resolve the tr1 library. This buys us a couple of
# things:
# 1. TR1 on platforms that don'e have it (i.e. VS2005)
# 2. A cross-platform include style (e.g. gcc uses non-standard
#    #include <tr1/memory>)
set (Boost_INCLUDE_DIRS
    ${Boost_INCLUDE_DIRS}/boost/tr1/tr1
    ${Boost_INCLUDE_DIRS}
)
include_directories(${Boost_INCLUDE_DIRS})

if (BUILD_TESTS)
    set (GTEST_ROOT $ENV{GTEST_ROOT} CACHE PATH "Google Test root directory.")
    set (CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/../../../cmake;${CMAKE_MODULE_PATH}")
    find_package (GTest REQUIRED)
    find_package (GMock REQUIRED)
    include_directories (${GTEST_INCLUDE_DIRS})
    include_directories (${GMOCK_INCLUDE_DIRS})

    if (MSVC80)
        # Can't mix tr1::tuple from boost with gtest's own tr1::tuple
        add_definitions (
            -DGTEST_HAS_TR1_TUPLE=1
            -DGTEST_USE_OWN_TR1_TUPLE=0
        )
    endif ()
endif ()

if (UNIX)
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -fPIC -fno-strict-aliasing")
    add_definitions(-DLINUX)
    if (CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64")
        add_definitions(-DLINUX64)
    endif ()
endif ()

if (WIN32)
      # Set warning level to 4 (the highest) on Windows
      set (CMAKE_CXX_WARNING_LEVEL 4)
      if (CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
            string (REGEX REPLACE "/W[0-4]" "/W4"
                    CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
      else ()
            set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
      endif ()
      # Enable multi-process compilation
      set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")
endif ()

include (src/CMakeLists.txt)
include (src/gui/CMakeLists.txt)
include (src/utilities/CMakeLists.txt)
include (src/macro/CMakeLists.txt)

add_library (${PROJECT_NAME}
    SHARED
    ${HEADER_FILES}
    ${SOURCE_FILES}
)

target_link_libraries (${PROJECT_NAME}
    ${TECPLOT_LIBS}
    vtkGraphics
    model-view
    ${Boost_LIBRARIES}
)

add_subdirectory (src/model-view)
if (BUILD_TESTS)
    add_subdirectory (src/macro/unittest)
endif ()

if (MSVC)
    # Set up a default .user file that contains the debugging setup
    set (SYSTEM_NAME $ENV{USERDOMAIN})
    set (USER_NAME $ENV{USERNAME})

    # Only do this if one doesn't already exist
    set (USER_FILE ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.vcproj.${SYSTEM_NAME}.${USER_NAME}.user)
    if (NOT EXISTS "${USER_FILE}")
        message (STATUS "Creating ${USER_FILE} with default debug settings")
        # These are the configuration variables
        if (CMAKE_GENERATOR MATCHES "Win64")
            set (USERFILE_PLATFORM "x64")
        else ()
            set (USERFILE_PLATFORM "Win32")
        endif ()
        find_program (USERFILE_DEBUG_COMMAND tec360.exe
                      HINTS "${TECPLOT_INSTALL_DIR}"
                      PATH_SUFFIXES bin
                      NO_DEFAULT_PATH)

        set (USER_FILE ${PROJECT_NAME}.vcproj.${SYSTEM_NAME}.${USER_NAME}.user)
        configure_file (cmake/User2005.in ${CMAKE_CURRENT_BINARY_DIR}/${USER_FILE})
    endif ()
endif ()
