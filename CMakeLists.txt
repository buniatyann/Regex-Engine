cmake_minimum_required(VERSION 3.10)

project(regex_engine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# warnings and optimizations
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -O3")

# Include directories
include_directories(include)

# Source files for core library
set(SOURCES
    src/ast/ast.cpp
    src/match/match.cpp
    src/match/matchers.cpp
    src/grammar/grammar.cpp
    src/regex/conversion_builder.cpp
    src/engine/engine_nfa.cpp
    src/engine/engine_dfa.cpp
    src/regex/regex.cpp
    src/regex/regex_error.cpp
)

# Source files for MVC components
set(MVC_SOURCES
    src/cli/cli_parser.cpp
    src/model/grep_model.cpp
    src/view/grep_view.cpp
    src/controller/grep_controller.cpp
)

# Build static library
add_library(regex_engine STATIC ${SOURCES})

# Build executable (grep-like tool)
add_executable(regex_grep main.cpp ${MVC_SOURCES})
target_link_libraries(regex_grep regex_engine)

# Installation rules (for library and headers)
install(TARGETS regex_engine
    ARCHIVE DESTINATION lib
)
install(DIRECTORY include/
    DESTINATION include
    FILES_MATCHING PATTERN "*.hpp"
)