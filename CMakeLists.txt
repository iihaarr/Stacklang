cmake_minimum_required(VERSION 3.12)
project(lang)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin)

enable_testing()
add_subdirectory(lib/googletest)

add_subdirectory(tests)
add_executable(lang main.cpp)

target_link_libraries(lang gtest gtest_main)
