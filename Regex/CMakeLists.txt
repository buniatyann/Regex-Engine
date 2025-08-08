cmake_minimum_required(VERSION 3.10)

project(regex_engine)

# Use C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable warnings and optimizations
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -O3 -fPIC")

# Include directories
include_directories(include)

# Source files
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
    main.cpp
)

# Define the executable
add_executable(regex_engine ${SOURCES})
