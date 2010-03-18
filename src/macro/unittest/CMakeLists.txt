cmake_minimum_required (VERSION 2.8)
project (macro_tests)

set (SOURCE_FILES
    ../MacroCommandParser.h
    ../MacroCommandParser.cpp
    ../../utilities/Error.h
    ../../utilities/Error.cpp
    GMockInclude.h
    MacroCommandParserTest.cpp
)

add_executable(${PROJECT_NAME}
    ${SOURCE_FILES}
)

target_link_libraries(${PROJECT_NAME}
    ${GMOCK_BOTH_LIBRARIES}
    ${Boost_LIBRARIES}
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
