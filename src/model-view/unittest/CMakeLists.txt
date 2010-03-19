cmake_minimum_required (VERSION 2.8)
project (model-view_tests)

set (HEADER_FILES
    GMockInclude.h
    ../ZoneListModel.h
    StateChangeEventMock.h
    StateChangeNotifierMock.h
    ../StateChangeListener.h
    ListModelObserverMock.h
)

set (SOURCE_FILES
    ZoneListModelTest.cpp
    ../ZoneListModel.cpp
    ../StateChangeListener.cpp
)

# Compiler definitions we need to add:
# - _TPTOOLBOX_DLLAPI because we're including toolbox headers
#   but not linking against tptoolbox lib
# - AMTEC_INTERNAL_MAKELIBTEC needed because we're including TecUtil headers
#   but not linking against libtec
get_directory_property (
    COMPILE_DEFINITIONS COMPILE_DEFINITIONS
)
list (APPEND
    COMPILE_DEFINITIONS
    _TPTOOLBOX_DLLAPI
    AMTEC_INTERNAL_MAKELIBTEC
)
set_directory_properties (
    PROPERTIES
    COMPILE_DEFINITIONS "${COMPILE_DEFINITIONS}"
)

add_executable(${PROJECT_NAME}
    ${HEADER_FILES}
    ${SOURCE_FILES}
)

target_link_libraries(${PROJECT_NAME}
    ${GMOCK_BOTH_LIBRARIES}
)

# Add a rule so that when this unittest builds, it also runs.
get_target_property (UNITTEST_EXECUTABLE
    ${PROJECT_NAME}
    LOCATION
)
add_custom_command (TARGET ${PROJECT_NAME}
    POST_BUILD
    COMMAND ${UNITTEST_EXECUTABLE} --gtest_print_time
)