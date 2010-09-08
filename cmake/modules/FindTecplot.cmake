# Find Tecplot
# Finds the Tecplot libraries and include directory needed for an add-on.
# The critical variable is TECPLOT_INSTALL_DIR. Once that is set, this
# module will find with include directory and libraries within the
# install directory.
#
# Typical usage:
#   find_package (Tecplot REQUIRED)
#   include_directories ("${TECPLOT_INCLUDE_DIR}")
#   add_library (myaddon main.cpp)
#   target_link_libraries (myaddon
#       libtec
#       tptoolbox
#       )

function (set_tecplot_install_dir)
    set (TP_2009_HOME $ENV{TEC_360_2009})
    if (TP_2009_HOME)
        set (TECPLOT_INSTALL_DIR_TEMP $ENV{TEC_360_2009})
    endif ()

    set (TP_2010_HOME $ENV{TEC_360_2010})
    if (TP_2010_HOME)
        set (TECPLOT_INSTALL_DIR_TEMP $ENV{TEC_360_2010})
    endif ()

    if (EXISTS ${TECPLOT_INSTALL_DIR_TEMP})
        set (TECPLOT_INSTALL_DIR ${TECPLOT_INSTALL_DIR_TEMP} CACHE PATH "Tecplot installation directory")
    else ()
        message (FATAL_ERROR "TECPLOT_INSTALL_DIR does not exist. Please set it to the location of the Tecplot installation directory.")
    endif ()
endfunction ()

set_tecplot_install_dir ()

# Detect if user changed Tecplot installation directories. If so, we need to
# re-execute the searches for libraries and paths by re-setting the cache
# variables.
if (TECPLOT_INSTALL_DIR_LAST)
    string (COMPARE NOTEQUAL "${TECPLOT_INSTALL_DIR_LAST}" "${TECPLOT_INSTALL_DIR}" TECPLOT_INSTALL_DIR_CHANGED)
endif ()

if (TECPLOT_INSTALL_DIR_CHANGED)
    # Reset all cache variables so that find_path and find_library
    # will re-execute the search
    message (STATUS "Tecplot installation directory changed from ${TECPLOT_INSTALL_DIR_LAST} to ${TECPLOT_INSTALL_DIR}.")
    set (TECPLOT_INCLUDE_DIR TECPLOT_INCLUDE_DIR-NOTFOUND)
    set (TECPLOT_LIBTEC_LIB TECPLOT_LIBTEC_LIB-NOTFOUND)
    set (TECPLOT_TOOLBOX_LIB TECPLOT_TOOLBOX_LIB-NOTFOUND)
    set (TECPLOT_LIBTEC_LIB_DEBUG TECPLOT_LIBTEC_LIB_DEBUG-NOTFOUND)
    set (TECPLOT_TOOLBOX_LIB_DEBUG TECPLOT_TOOLBOX_LIB_DEBUG-NOTFOUND)
endif ()
set (TECPLOT_INSTALL_DIR_LAST "${TECPLOT_INSTALL_DIR}" CACHE INTERNAL "" FORCE)

find_path (TECPLOT_INCLUDE_DIR
           "TECADDON.h"
           HINTS "${TECPLOT_INSTALL_DIR}"
           PATH_SUFFIXES include
           NO_DEFAULT_PATH)
find_library (TECPLOT_LIBTEC_LIB
              NAMES libtec libtec.so
              HINTS "${TECPLOT_INSTALL_DIR}"
              PATH_SUFFIXES bin lib
              NO_DEFAULT_PATH)
find_library (TECPLOT_TOOLBOX_LIB "tptoolbox"
              HINTS "${TECPLOT_INSTALL_DIR}"
              PATH_SUFFIXES bin lib
              NO_DEFAULT_PATH)

# Create an imported target for libtec and tptoolbox
add_library (libtec UNKNOWN IMPORTED)
add_library (tptoolbox UNKNOWN IMPORTED)
set_target_properties (libtec
    PROPERTIES
    IMPORTED_LOCATION ${TECPLOT_LIBTEC_LIB}
)
set_target_properties (tptoolbox
    PROPERTIES
    IMPORTED_LOCATION ${TECPLOT_TOOLBOX_LIB}
)

if (WIN32)
    # Find the debug libraries, if available
    find_library (TECPLOT_LIBTEC_LIB_DEBUG "libtec"
                  HINTS "${TECPLOT_INSTALL_DIR}"
                  PATH_SUFFIXES debug bin lib
                  NO_DEFAULT_PATH)
    find_library (TECPLOT_TOOLBOX_LIB_DEBUG "tptoolboxd"
                  HINTS "${TECPLOT_INSTALL_DIR}"
                  PATH_SUFFIXES bin lib
                  NO_DEFAULT_PATH)
    set_target_properties (libtec
        PROPERTIES
        IMPORTED_LOCATION_DEBUG ${TECPLOT_LIBTEC_LIB_DEBUG}
        )
    set_target_properties (tptoolbox
        PROPERTIES
        IMPORTED_LOCATION_DEBUG ${TECPLOT_TOOLBOX_LIB_DEBUG}
        )
endif ()

# User normally only cares about TECPLOT_INSTALL_DIR
mark_as_advanced (
    TECPLOT_INCLUDE_DIR
    TECPLOT_LIBTEC_LIB
    TECPLOT_LIBTEC_LIB_DEBUG
    TECPLOT_TOOLBOX_LIB
    TECPLOT_TOOLBOX_LIB_DEBUG
)
