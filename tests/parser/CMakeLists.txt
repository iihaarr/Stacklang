project(parser_test)

set(SOURCES
    parser_test.cpp
)

add_executable(parser_test ${SOURCES})

target_link_libraries(parser_test gtest gtest_main parser)