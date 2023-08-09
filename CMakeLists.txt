cmake_minimum_required(VERSION 3.15)
project(Stacklang)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")

enable_testing()
add_subdirectory(lib/googletest)

add_subdirectory(src/lexer)

add_subdirectory(tests)
add_executable(Stacklang src/main.cpp)

target_link_libraries(Stacklang gtest gtest_main)
