project(parser)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(PUBLIC_HEADERS
	include/parser/ast_nodes.hpp
	include/parser/parser_interface.hpp
)
set(PRIVATE_HEADERS
	src/parser.hpp
)
set(SOURCES
	src/parser.cpp
	src/ast_nodes.cpp
)
include_directories(include)
add_library(parser STATIC ${SOURCES} ${PRIVATE_HEADERS} ${PUBLIC_HEADERS})

target_link_libraries(parser lexer)
target_include_directories(parser PUBLIC include/)