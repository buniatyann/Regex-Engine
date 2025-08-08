# Regex Engine

A high-performance **C++ library** for parsing and matching **regular expressions**, designed with a modular architecture for efficient parsing, AST construction, NFA/DFA conversion, and matching. It features both **Nondeterministic Finite Automaton (NFA)** and **Deterministic Finite Automaton (DFA)** engines, offering flexibility for lightweight matching (NFA) or optimized performance (DFA). Built with **extensibility** and **robust error handling** in mind.

## ✨ Features

- **Rich regex syntax**: Supports a wide range of constructs for flexible pattern matching:
  - Literals: `a`, `b`, …
  - Dot (`.`) – matches any character except newline.
  - Character classes: `[abc]`, `[^abc]` (negated).
  - Alternation: `a|b`.
  - Concatenation: `ab`.
  - Quantifiers: `*` (Kleene star), `+` (one or more), `?` (optional).
  - Groups: `(ab)`.
  - Anchors: `^` (start), `$` (end) for precise string boundary matching.
- **Dual matching engines**: 
  - **NFA**: Lightweight with faster construction, ideal for small inputs or dynamic patterns using epsilon transitions.
  - **DFA**: Optimized for high-speed matching after preprocessing, suitable for large inputs or repeated matches.
- **Pattern parsing and processing**: Parses regex patterns into an Abstract Syntax Tree (AST), converts to NFA, and optionally to DFA for efficient matching.
- **Robust error handling**: Detects syntax errors with detailed messages and exact positions via `regex::regex_error`.
- **Modular architecture**: Cleanly separates parsing, AST construction, NFA/DFA conversion, and matching for extensibility.
- **Use cases**: Ideal for text processing, input validation (e.g., email addresses), search functionality, and lexical analysis in compilers or tools.

## 📦 Installation

### 1. Clone the Repository
```bash
git clone https://github.com/yourusername/regex-engine.git
cd regex-engine
```

### 2. Requirements
- C++17-compatible compiler:
  - Linux/macOS: g++ ≥ 7, clang++ ≥ 5.
  - Windows: MSVC (Visual Studio 2017 or later), MinGW (g++ ≥ 7).
- CMake ≥ 3.10.
- Standard C++ library only — **no external dependencies**.
- Supported platforms: Linux, macOS, Windows.

### 3. Build with CMake

#### Compile on Linux/macOS:
```bash
mkdir build && cd build
cmake ..
make
sudo make install
```

#### Compile on Windows:
Using **MinGW**:
```bash
mkdir build && cd build
cmake -G "MinGW Makefiles" ..
cmake --build .
```

Using **Visual Studio** (e.g., Visual Studio 2019):
```bash
mkdir build && cd build
cmake -G "Visual Studio 16 2019" -A x64 ..
cmake --build . --config Release
```

To install system-wide on Windows (optional, requires admin privileges):
```bash
cmake --install .
```

### 4. Integrate into Your Project
- Copy `include/` to your project or use the installed headers (`/usr/local/include` on Unix, or `C:\Program Files\regex_engine\include` on Windows).
- Link against `regex_engine` (e.g., `-lregex_engine` in `/usr/local/lib` or equivalent).

Example for linking in another CMake project:
```cmake
find_library(REGEX_ENGINE regex_engine HINTS /usr/local/lib)
target_link_libraries(your_target ${REGEX_ENGINE})
```

## 🚀 Usage

The main interface is the `regex::regex` class, which supports both NFA and DFA engines. The `match::match` class returns match results (success, start, and end positions).

### Example 1: DFA Matching
```cpp
#include <regex/regex.hpp>
#include <iostream>

int main() {
    regex::regex re("a(b|c)*d", regex::engine::dfa);
    std::string input = "abcd";
    auto match = re.match(input);
    if (match.is_matched()) {
        std::cout << "DFA Match from " << match.get_start() << " to " << match.get_end() << std::endl;
    } else {
        std::cout << "No match found" << std::endl;
    }
    return 0;
}
```

### Example 2: NFA Matching with Anchors
```cpp
#include <regex/regex.hpp>
#include <iostream>

int main() {
    regex::regex re("^[0-9]+$", regex::engine::nfa);
    std::string input = "123";
    auto match = re.match(input);
    if (match.is_matched()) {
        std::cout << "NFA Match from " << match.get_start() << " to " << match.get_end() << std::endl;
    } else {
        std::cout << "No match found" << std::endl;
    }
    return 0;
}
```

## 🛠 Error Handling

Invalid patterns throw a `regex::regex_error`:
```cpp
try {
    regex::regex re("[a-z", regex::engine::nfa); // Missing ']'
} catch (const regex::regex_error& e) {
    std::cerr << "Error: " << e.what() << " at position " << e.get_position() << std::endl;
}
```

## 📂 Project Structure

```
regex-engine/
├── include/
│   ├── ast/
│   │   └── ast.hpp              # AST structure for regex patterns
│   ├── engine/
│   │   ├── engine_dfa.hpp       # DFA engine interface
│   │   └── engine_nfa.hpp       # NFA engine interface
│   ├── grammar/
│   │   └── grammar.hpp          # Regex parser
│   ├── match/
│   │   ├── match.hpp            # Match result representation
│   │   └── matchers.hpp         # Match component classes
│   ├── regex/
│   │   ├── conversion_builder.hpp # AST → NFA conversion
│   │   ├── regex.hpp            # Main regex interface
│   │   └── regex_error.hpp      # Exception handling
├── src/
│   ├── ast/                      
│   │   └── ast.cpp              # AST node implementation
│   ├── engine/
│   │   ├── engine_dfa.cpp       # DFA engine implementation
│   │   └── engine_nfa.cpp       # NFA engine implementation
│   ├── grammar/
│   │   └── grammar.cpp          # Regex parser implementation
│   ├── match/
│   │   ├── match.cpp            # Match result implementation
│   │   └── matchers.cpp         # Matcher implementations
│   ├── regex/
│   │   ├── conversion_builder.cpp    # AST to NFA conversion implementation
│   │   ├── regex.cpp            # Main regex class implementation
│   │   └── regex_error.cpp      # Exception handling implementation
├── CMakeLists.txt               # Build configuration
└── README.md                    # Project documentation
```

## 🤝 Contributing

Contributions are welcome! Please:
1. Open an **issue** for bugs or feature requests.
2. Submit **pull requests** with clear descriptions of changes.

## 📜 License

**MIT License**  
Copyright © 2025 [Your Name]

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
