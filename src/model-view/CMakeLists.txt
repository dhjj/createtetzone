cmake_minimum_required (VERSION 2.8)
project (model-view)

set (HEADER_FILES
    ListModelAbstract.h
    ListModelGenericInterface.h
    ListModelInterface.h
    ListModelObserverInterface.h
    ListModelPreMapped.h
    ListModelViewFwd.h
    ListModelViewTypedefs.h
    ListMultiSelectionModel.h
    ListSelectionModelAbstract.h
    ListSelectionModelInterface.h
    ListSelectionModelObserverInterface.h
    ListSingleSelectionModel.h
    ListView.h
    ListViewInterface.h
    StateChangeListener.h
    ZoneListModel.h
)

set (SOURCE_FILES
    ListMultiSelectionModel.cpp
    ListSelectionModelAbstract.cpp
    ListSingleSelectionModel.cpp
    ListView.cpp
    StateChangeListener.cpp
    ZoneListModel.cpp
)

add_library (${PROJECT_NAME}
    STATIC
    ${HEADER_FILES}
    ${SOURCE_FILES}
)

target_link_libraries (${PROJECT_NAME}
    ${TECPLOT_LIBS}
)

if (BUILD_TESTS)
    add_subdirectory(unittest)
endif ()
