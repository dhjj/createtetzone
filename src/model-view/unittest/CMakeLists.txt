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