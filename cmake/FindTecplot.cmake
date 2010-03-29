set (TECPLOT_INSTALL_DIR $ENV{TEC_360_2009} CACHE PATH "Tecplot installation directory")

if (NOT EXISTS ${TECPLOT_INSTALL_DIR})
    message (FATAL_ERROR "TECPLOT_INSTALL_DIR does not exist.")
endif ()

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

if (WIN32)
    find_library (TECPLOT_LIBTEC_LIB_DEBUG "libtec"
                  HINTS "${TECPLOT_INSTALL_DIR}"
                  PATH_SUFFIXES debug bin lib
                  NO_DEFAULT_PATH)
    find_library (TECPLOT_TOOLBOX_LIB_DEBUG "tptoolboxd"
                  HINTS "${TECPLOT_INSTALL_DIR}"
                  PATH_SUFFIXES bin lib
                  NO_DEFAULT_PATH)
    set (TECPLOT_LIBS
        debug ${TECPLOT_LIBTEC_LIB_DEBUG}
        optimized ${TECPLOT_LIBTEC_LIB}
        debug ${TECPLOT_TOOLBOX_LIB_DEBUG}
        optimized ${TECPLOT_TOOLBOX_LIB}
    )
else ()
    set (TECPLOT_LIBS
        ${TECPLOT_LIBTEC_LIB}
        ${TECPLOT_TOOLBOX_LIB}
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
