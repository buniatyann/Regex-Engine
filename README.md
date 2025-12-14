# Regex Engine

A high-performance **C++ library** for parsing and matching **regular expressions**, architected with **object-oriented design patterns** and a modular pipeline for efficient parsing, AST construction, NFA/DFA conversion, and matching. This project demonstrates **software engineering best practices** including the **Strategy Pattern** (dual NFA/DFA engines), **Builder Pattern** (AST to NFA conversion), **State Machine** (CLI parser), and **MVC Pattern** (grep tool). Built with **extensibility**, **robust error handling**, and **clean architecture** in mind.

## Table of Contents

- [Features](#features)
- [Design Patterns & Architecture](#design-patterns--architecture)
- [Installation](#installation)
- [Usage](#usage)
  - [Core Library Examples](#core-library-examples)
  - [Grep Tool Usage](#grep-tool-usage)
- [Project Structure](#project-structure)
- [Error Handling](#error-handling)
- [Contributing](#contributing)
- [License](#license)

## Features

### Core Regex Engine

- **Rich regex syntax**: Comprehensive pattern matching support
  - Literals: `a`, `b`, …
  - Dot (`.`) – matches any character except newline
  - Character classes: `[abc]`, `[^abc]` (negated)
  - Alternation: `a|b`
  - Concatenation: `ab`
  - Quantifiers: `*` (Kleene star), `+` (one or more), `?` (optional)
  - Groups: `(ab)`
  - Anchors: `^` (start), `$` (end) for precise string boundary matching

- **Dual matching engines** (Strategy Pattern):
  - **NFA**: Lightweight with faster construction, ideal for small inputs or dynamic patterns using epsilon transitions
  - **DFA**: Optimized for high-speed matching after preprocessing, suitable for large inputs or repeated matches

- **Multi-stage pipeline architecture**:
  - **Parsing**: Recursive descent parser converting patterns to AST
  - **AST Construction**: Tree representation of regex structure
  - **NFA Conversion**: Thompson's construction algorithm (Builder Pattern)
  - **DFA Optimization**: Subset construction for deterministic matching
  - **Matching**: Configurable engine selection

### Grep-Like CLI Tool

- **MVC Architecture**: Clean separation of concerns
  - **Model**: Pattern matching and file I/O logic
  - **View**: ANSI-colored output formatting
  - **Controller**: Application flow orchestration

- **State Machine CLI Parser**: Robust command-line argument processing
  - States: `start` → `reading_option` → `reading_pattern` → `reading_file` → `done/error`
  - Comprehensive option support: `-i`, `-n`, `-v`, `-c`, `-l`, `-r`, `--dfa`

- **Advanced search capabilities**:
  - Case-insensitive matching
  - Line number display
  - Inverted matching
  - Match counting
  - Recursive directory traversal
  - Filename-only output

## Design Patterns & Architecture

This project showcases multiple software design patterns working in harmony:

### 1. Strategy Pattern (Dual Engine Architecture)

The regex engine supports two interchangeable matching strategies:

```cpp
// Choose NFA strategy for lightweight, one-time matches
regex::regex re_nfa("pattern", regex::engine::nfa);

// Choose DFA strategy for optimized, repeated matches
regex::regex re_dfa("pattern", regex::engine::dfa);
```

**Benefits**: Allows runtime selection of matching algorithm based on use case without changing client code.

### 2. Builder Pattern (NFA Construction)

The `conversion::builder` class constructs complex NFA state machines from AST nodes using Thompson's construction:

```cpp
conversion::builder builder;
auto nfa = builder.build(ast_root);  // Builds NFA from AST
```

**Benefits**: Separates complex NFA construction logic from AST representation, making both components independently maintainable.

### 3. State Machine Pattern (CLI Parser)

The CLI argument parser implements a formal state machine with well-defined transitions:

```
[start] → [reading_option] → [reading_pattern] → [reading_file] → [done/error]
```

**Benefits**: Provides predictable, testable argument parsing with clear error handling at each state.

### 4. Model-View-Controller (MVC) Pattern

The grep tool demonstrates classic MVC separation:

- **Model** (`grep_model`): Encapsulates business logic, file operations, pattern matching
- **View** (`grep_view`): Handles all presentation logic, ANSI colors, formatting
- **Controller** (`grep_controller`): Coordinates Model and View, handles application flow

**Benefits**: Each component has a single responsibility, making the codebase maintainable and testable.

### 5. Composite Pattern (AST Structure)

The Abstract Syntax Tree uses composite nodes where each node can contain child nodes:

```cpp
ast::node_ptr concat_node;  // Parent node
concat_node->set_children({literal_a, literal_b});  // Child nodes
```

**Benefits**: Allows uniform treatment of simple and complex regex patterns.

### 6. Visitor Pattern Potential (AST Traversal)

While not explicitly implemented, the AST structure is designed for visitor pattern application in the conversion builder:

```cpp
// Builder visits each AST node type and constructs appropriate NFA fragments
auto fragment = build_node(ast_node);  // Recursive visitation
```

### 7. Pipeline Architecture

The entire regex processing follows a clear pipeline pattern:

```
Input Pattern → Parser → AST → Builder → NFA → (Optional) DFA → Matcher → Result
```

**Benefits**: Each stage is independent and can be tested, debugged, or optimized separately.

## Installation

### 1. Clone the Repository

```bash
git clone https://github.com/buniatyann/Regex-Engine.git
cd Regex-Engine
```

### 2. Requirements

- **C++17-compatible compiler**:
  - Linux/macOS: g++ ≥ 7, clang++ ≥ 5
  - Windows: MSVC (Visual Studio 2017 or later), MinGW (g++ ≥ 7)
- **CMake** ≥ 3.10
- **Standard C++ library only** — no external dependencies
- **Supported platforms**: Linux, macOS, Windows

### 3. Build with CMake

#### Linux/macOS:

```bash
mkdir build && cd build
cmake ..
make
sudo make install  # Optional: install system-wide
```

This produces:
- `libregex_engine.a` – Static library for the core regex engine
- `regex_grep` – Grep-like command-line tool

#### Windows (MinGW):

```bash
mkdir build && cd build
cmake -G "MinGW Makefiles" ..
cmake --build .
```

#### Windows (Visual Studio):

```bash
mkdir build && cd build
cmake -G "Visual Studio 16 2019" -A x64 ..
cmake --build . --config Release
```

To install system-wide (requires admin privileges):
```bash
cmake --install .
```

### 4. Integrate into Your Project

**Option 1**: Use installed library
```cmake
find_library(REGEX_ENGINE regex_engine HINTS /usr/local/lib)
target_link_libraries(your_target ${REGEX_ENGINE})
```

**Option 2**: Copy headers and link
```bash
# Copy headers to your project
cp -r include/ /your/project/include/

# Link against the library
g++ your_code.cpp -L/usr/local/lib -lregex_engine -o your_app
```

## Usage

### Core Library Examples

#### Example 1: NFA Matching with Strategy Pattern

```cpp
#include <regex/regex.hpp>
#include <iostream>

int main() {
    // Strategy Pattern: Choose NFA engine
    regex::regex re("^[0-9]+$", regex::engine::nfa);

    std::string input = "12345";
    auto match = re.match(input);

    if (match.is_matched()) {
        std::cout << "Match from " << match.get_start()
                  << " to " << match.get_end() << std::endl;
    } else {
        std::cout << "No match found" << std::endl;
    }

    return 0;
}
```

#### Example 2: DFA Matching for Performance

```cpp
#include <regex/regex.hpp>
#include <iostream>

int main() {
    // Strategy Pattern: Choose DFA engine for optimized matching
    regex::regex re("a(b|c)*d", regex::engine::dfa);

    std::string input = "abbbcd";
    auto match = re.match(input);

    if (match.is_matched()) {
        std::cout << "DFA Match successful!" << std::endl;
    }

    return 0;
}
```

#### Example 3: Error Handling

```cpp
#include <regex/regex.hpp>
#include <iostream>

int main() {
    try {
        // Invalid pattern - missing closing bracket
        regex::regex re("[a-z", regex::engine::nfa);
    } catch (const regex::regex_error& e) {
        std::cerr << "Pattern error: " << e.what()
                  << " at position " << e.get_position() << std::endl;
    }

    return 0;
}
```

### Grep Tool Usage

The `regex_grep` tool demonstrates MVC pattern and state machine CLI parsing:

#### Basic Usage

```bash
# Search for pattern in files
./build/regex_grep "error" logfile.txt

# Case-insensitive search with line numbers
./build/regex_grep -i -n "warning" *.log

# Search with alternation (remember to group!)
./build/regex_grep "(error|warning|fatal)" system.log
```

#### Advanced Options

```bash
# Recursive directory search
./build/regex_grep -r "TODO" src/

# Count matches only
./build/regex_grep -c "function" code.cpp

# Show only filenames with matches
./build/regex_grep -l "bug" *.txt

# Invert match (show non-matching lines)
./build/regex_grep -v "debug" output.log

# Use DFA engine for better performance
./build/regex_grep --dfa "pattern" largefile.txt
```

#### Combined Options

```bash
# Case-insensitive recursive search with line numbers
./build/regex_grep -i -n -r "fixme" project/

# Count matches in all files recursively
./build/regex_grep -c -r "error" /var/log/
```

#### Help & Version

```bash
# Display help
./build/regex_grep --help

# Show version
./build/regex_grep --version
```

## Project Structure

```
regex-engine/
├── include/                      # Public headers
│   ├── ast/
│   │   └── ast.hpp              # AST nodes (Composite Pattern)
│   ├── engine/
│   │   ├── engine_dfa.hpp       # DFA strategy implementation
│   │   └── engine_nfa.hpp       # NFA strategy implementation
│   ├── grammar/
│   │   └── grammar.hpp          # Recursive descent parser
│   ├── match/
│   │   ├── match.hpp            # Match result representation
│   │   └── matchers.hpp         # Character matcher abstraction
│   ├── regex/
│   │   ├── conversion_builder.hpp # NFA Builder Pattern
│   │   ├── regex.hpp            # Main API (Strategy context)
│   │   └── regex_error.hpp      # Exception handling
│   ├── cli/
│   │   └── cli_parser.hpp       # CLI State Machine
│   ├── model/
│   │   └── grep_model.hpp       # MVC Model layer
│   ├── view/
│   │   └── grep_view.hpp        # MVC View layer
│   └── controller/
│       └── grep_controller.hpp  # MVC Controller layer
│
├── src/                          # Implementation files
│   ├── ast/
│   │   └── ast.cpp              # AST implementation
│   ├── engine/
│   │   ├── engine_dfa.cpp       # DFA implementation (subset construction)
│   │   └── engine_nfa.cpp       # NFA implementation (Thompson's construction)
│   ├── grammar/
│   │   └── grammar.cpp          # Parser implementation
│   ├── match/
│   │   ├── match.cpp            # Match result implementation
│   │   └── matchers.cpp         # Matcher implementations
│   ├── regex/
│   │   ├── conversion_builder.cpp # Builder implementation
│   │   ├── regex.cpp            # Main API implementation
│   │   └── regex_error.cpp      # Error handling implementation
│   ├── cli/
│   │   └── cli_parser.cpp       # State machine implementation
│   ├── model/
│   │   └── grep_model.cpp       # Model implementation
│   ├── view/
│   │   └── grep_view.cpp        # View implementation (ANSI colors)
│   └── controller/
│       └── grep_controller.cpp  # Controller implementation
│
├── main.cpp                      # Grep tool entry point
├── CMakeLists.txt               # Build configuration
└── README.md                    # This file
```

### Architecture Diagram

```
┌─────────────────────────────────────────────────────────────┐
│                     Regex Engine Core                       │
├─────────────────────────────────────────────────────────────┤
│  Pattern → Parser → AST → Builder → NFA → DFA → Matcher    │
│           (RD)    (Comp)  (Builder) (Thompson) (Subset)     │
└─────────────────────────────────────────────────────────────┘
                              ↓
┌─────────────────────────────────────────────────────────────┐
│                    Grep Tool (MVC)                          │
├─────────────────────────────────────────────────────────────┤
│  Controller ─→ CLI Parser (State Machine)                  │
│       ↓              ↓                                       │
│     Model ←─────── Options                                  │
│       ↓                                                      │
│  Regex Engine (Strategy: NFA/DFA)                          │
│       ↓                                                      │
│     View (ANSI Output)                                      │
└─────────────────────────────────────────────────────────────┘
```

## Error Handling

The library uses exception-based error handling with detailed error messages:

```cpp
try {
    regex::regex re("[unclosed", regex::engine::nfa);
} catch (const regex::regex_error& e) {
    // Provides exact error position for debugging
    std::cerr << "Error: " << e.what()
              << " at position " << e.get_position() << std::endl;
    // Output: "Error: Expected ']' at position 9"
}
```

**Error types**:
- Syntax errors (unclosed brackets, invalid escape sequences)
- Invalid quantifiers
- Unexpected characters
- Malformed character classes

## Performance Characteristics

| Engine | Construction Time | Matching Speed | Memory Usage | Best For |
|--------|------------------|----------------|--------------|----------|
| NFA    | Fast (O(n))     | Moderate       | Low          | One-time matches, dynamic patterns |
| DFA    | Slow (O(2^n) worst) | Very Fast   | High         | Repeated matches, large inputs |

**Recommendation**: Use NFA by default, switch to DFA when matching the same pattern against many inputs.

## Contributing

Contributions are welcome! This project is an excellent opportunity to practice:
- **Design patterns** in real-world scenarios
- **Compiler theory** (parsing, AST, automata)
- **Software architecture** (MVC, separation of concerns)
- **C++ best practices** (RAII, smart pointers, const correctness)

Please:
1. Open an **issue** for bugs or feature requests
2. Submit **pull requests** with:
   - Clear descriptions of changes
   - Explanation of design patterns used
   - Test cases demonstrating functionality
   - Updated documentation

## License

This project is licensed under the [MIT License](https://opensource.org/licenses/MIT). See the `LICENSE` file for details.

---
