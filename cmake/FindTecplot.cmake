set (TECPLOT_INSTALL_DIR TECPLOT_INSTALL_DIR-NOTFOUND CACHE PATH "Tecplot installation directory")

if (NOT EXISTS ${TECPLOT_INSTALL_DIR})
    message (FATAL_ERROR "TECPLOT_INSTALL_DIR does not exist.")
endif ()

find_path (TECPLOT_INCLUDE_DIR "TECADDON.h"
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
