project(interpreter)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(PUBLIC_HEADERS
    include/interpreter/config.hpp
    include/interpreter/interpreter_interface.hpp
)
set(PRIVATE_HEADERS
    src/interpreter.hpp
)
set(SOURCES
    src/interpreter.cpp
)

include_directories(include)
add_library(interpreter STATIC ${SOURCES} ${PRIVATE_HEADERS} ${PUBLIC_HEADERS})

target_link_libraries(interpreter parser)
target_include_directories(interpreter PUBLIC include/)