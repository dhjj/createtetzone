cmake_minimum_required (VERSION 2.8)
project (model-view_tests)

set (SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}")

set (HEADER_FILES
    ${SOURCE_DIR}/GMockInclude.h
    ${SOURCE_DIR}/../ZoneListModel.h
    ${SOURCE_DIR}/StateChangeEventMock.h
    ${SOURCE_DIR}/StateChangeNotifierMock.h
    ${SOURCE_DIR}/../StateChangeListener.h
    ${SOURCE_DIR}/ListModelObserverMock.h
)

set (SOURCE_FILES
    ${SOURCE_DIR}/ZoneListModelTest.cpp
    ${SOURCE_DIR}/../ZoneListModel.cpp
    ${SOURCE_DIR}/../StateChangeListener.cpp
)

set (GTEST_ROOT $ENV{GTEST_ROOT} CACHE PATH "Google Test root directory.")
find_package (GTest REQUIRED)

set (CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
message (STATUS "CMake module path is ${CMAKE_MODULE_PATH}")
find_package (GMock REQUIRED)

include_directories(${GTEST_INCLUDE_DIRS})
#message (STATUS "GMOCK_INCLUDE_DIRS are ${GMOCK_INCLUDE_DIRS}")
include_directories(${GMOCK_INCLUDE_DIRS})

add_definitions (
    # _TPTOOLBOX_DLLAPI needed because we're including toolbox headers
    # but not linking against tptoolbox lib
    -D_TPTOOLBOX_DLLAPI
    # AMTEC_INTERNAL_MAKELIBTEC needed because we're including TecUtil headers
    # but not linking against libtec
    -DAMTEC_INTERNAL_MAKELIBTEC
)

if (MSVC80)
    # Can't mix tr1::tuple from boost with gtest's own tr1::tuple
    add_definitions (
        -DGTEST_HAS_TR1_TUPLE=1
        -DGTEST_USE_OWN_TR1_TUPLE=0
    )
endif ()

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