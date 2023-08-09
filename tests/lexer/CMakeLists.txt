project(lexer_tests)

set(SOURCES
    lexer_test.cpp
)

add_executable(lexer_tests ${SOURCES})

target_link_libraries(lexer_tests gtest gtest_main lexer)