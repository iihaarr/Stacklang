project(lexer)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(PUBLIC_HEADERS
        include/lexer/lexer_interface.hpp
        include/lexer/location.hpp
        include/lexer/token.hpp
)
set(PRIVATE_HEADERS
        src/lexer.hpp
)
set(SOURCES
        src/lexer.cpp
        src/token.cpp
)

include_directories(include)
add_library(lexer STATIC ${SOURCES} ${PRIVATE_HEADERS} ${PUBLIC_HEADERS})
target_include_directories(lexer PUBLIC include/)