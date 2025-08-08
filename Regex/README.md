# Regex Engine

A high-performance **C++ library** for parsing and matching **regular expressions**, featuring both **Nondeterministic Finite Automaton (NFA)** and **Deterministic Finite Automaton (DFA)** engines.  
It supports a wide range of regex features — from basic literals to advanced constructs — and is designed with **modularity**, **extensibility**, and **robust error handling** in mind.

---

## ✨ Features

- **Rich regex syntax**:
  - Literals: `a`, `b`, …
  - Dot (`.`) – matches any character except newline.
  - Character classes: `[abc]`, `[^abc]`.
  - Alternation: `a|b`.
  - Concatenation: `ab`.
  - Quantifiers: `*` (Kleene star), `+` (one or more), `?` (optional).
  - Groups: `(ab)`.
  - Anchors: `^` (start), `$` (end).
- **Dual matching engines**:
  - **NFA** – flexible and lightweight.
  - **DFA** – optimized for speed after preprocessing.
- **Detailed error reporting**:
  - Pinpoints the exact position of syntax errors.
- **Modular architecture**:
  - Parsing → AST → NFA/DFA conversion → Matching are cleanly separated.
- **Exception safety**:
  - Clear, consistent exception handling via `regex_error`.

---

## 📦 Installation

### 1. Clone the Repository
```bash
git clone https://github.com/yourusername/regex-engine.git
cd regex-engine
```

### 2. Requirements
- C++17-compatible compiler (g++, clang++, MSVC).
- Standard C++ library only — **no external dependencies**.

### 3. Build with CMake
**`CMakeLists.txt` example:**
```cmake
cmake_minimum_required(VERSION 3.10)
project(RegexEngine)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB SOURCES "src/*.cpp")
include_directories(include)

add_library(regex_engine STATIC ${SOURCES})
```

**Compile:**
```bash
mkdir build && cd build
cmake ..
make
```

### 4. Integrate into Your Project
- Copy `include/` to your project.
- Link against `regex_engine`.

---

## 🚀 Usage

```cpp
#include <regex/regex.hpp>
#include <iostream>

int main() {
    // Create a regex object using the DFA engine
    regex::regex re("a(b|c)*d", regex::engine::dfa);

    // Input to test
    std::string input = "abcd";

    // Perform the match
    auto match = re.match(input);

    if (match.is_matched()) {
        std::cout << "Match found from position "
                  << match.get_start() << " to "
                  << match.get_end() << std::endl;
    } else {
        std::cout << "No match found" << std::endl;
    }

    return 0;
}
```

---

## 🛠 Error Handling

Invalid patterns throw a `regex::regex_error` with a clear message and error position:

```cpp
try {
    regex::regex re("[a-z", regex::engine::nfa); // Missing ']'
} catch (const regex::regex_error& e) {
    std::cerr << "Error: " << e.what()
              << " at position " << e.get_position()
              << std::endl;
}
```

---

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
│   ├── ast.cpp
│   ├── engine_dfa.cpp
│   ├── engine_nfa.cpp
│   ├── grammar.cpp
│   ├── match.cpp
│   ├── matchers.cpp
│   ├── regex.cpp
│   └── conversion_builder.cpp
└── README.md
```

---

## 🤝 Contributing

Contributions are welcome!  
Please:
1. Open an **issue** for bugs or feature requests.
2. Submit **pull requests** for improvements.

---

## 📜 License

**MIT License**  
Copyright © 2025

Permission is hereby granted, free of charge, to any person obtaining a copy of this software…  
(Full license text here.)
