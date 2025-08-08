cmake_minimum_required(VERSION 3.10)

project(regex_engine)

# Use C++17 (sufficient for the provided code)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable warnings and optimizations
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -O3")

# Include directories
include_directories(include)

# Source files
set(SOURCES
    src/ast.cpp
    src/match.cpp
    src/matchers.cpp
    src/grammar.cpp
    src/conversion_builder.cpp
    src/engine_nfa.cpp
    src/engine_dfa.cpp
    src/regex.cpp
    src/regex_error.cpp
)

# Build static library
add_library(regex_engine STATIC ${SOURCES})

# Optionally build executable if main.cpp exists
if(EXISTS "${CMAKE_SOURCE_DIR}/main.cpp")
    add_executable(regex_engine_exe main.cpp)
    target_link_libraries(regex_engine_exe regex_engine)
endif()

# Installation rules (for library and headers)
install(TARGETS regex_engine
    ARCHIVE DESTINATION lib
)
install(DIRECTORY include/
    DESTINATION include
    FILES_MATCHING PATTERN "*.hpp"
)