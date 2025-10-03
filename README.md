# English-Arabic Dictionary System

[![C++17](https://img.shields.io/badge/C%2B%2B-17-blue.svg)](https://isocpp.org/)
[![CMake](https://img.shields.io/badge/CMake-3.16%2B-green.svg)](https://cmake.org/)
[![Platform](https://img.shields.io/badge/Platform-Windows%20%7C%20Linux%20%7C%20macOS-lightgrey.svg)](https://github.com)

A high-performance console-based English-Arabic dictionary application built in C++17. Features bidirectional translation, intelligent autocomplete using Trie data structure, and persistent UTF-8 storage.

## ✨ Features

- **Bidirectional Translation**: English ↔ Arabic word lookup
- **Multiple Meanings**: Support for multiple definitions per word
- **Smart Autocomplete**: Trie-based prefix matching for fast word suggestions
- **Persistent Storage**: UTF-8 file storage with BOM support for Arabic text
- **Cross-Platform**: Compatible with Windows, Linux, and macOS
- **Clean Architecture**: Well-structured codebase demonstrating data structures and algorithms

## 🚀 Quick Start

### Prerequisites

- C++17 compatible compiler (GCC 7+, Clang 5+, MSVC 2017+)
- CMake 3.16 or higher
- UTF-8 capable terminal

### Installation

1. **Clone the repository**
   ```bash
   git clone <repository-url>
   cd dictionary-system
   ```

2. **Build the project**
   ```bash
   mkdir build && cd build
   cmake ..
   cmake --build .
   ```

3. **Run the application**
   ```bash
   # Windows
   .\DictionarySystem.exe
   
   # Linux/macOS
   ./DictionarySystem
   ```

### CLion Setup

1. Open the project folder in CLion
2. Allow CMake to configure automatically
3. Click the **Run** button or press `Ctrl+F5`

## 📖 Usage Guide

### Main Menu Options

```
========================================
     ENGLISH-ARABIC DICTIONARY
========================================
 1. Search word
 2. Arabic to English
 3. Autocomplete (prefix)
 4. Add new word
 5. Add meaning to word
 6. Delete meaning/word
 7. Statistics
 0. Exit
========================================
```

### Example Operations

#### Adding a New Word
```
>> 4
English word: computer
Meaning: Electronic device for processing data
Arabic (optional): حاسوب
Word added!
```

#### Searching by English Word
```
>> 1
Word: book
Word: book
Meanings:
 [0] Written work
     Arabic: كتاب
```

#### Arabic to English Translation
```
>> 2
Arabic word: مرحبا
Arabic: مرحبا
English words:
 1. hello
```

#### Autocomplete Feature
```
>> 3
Prefix: co
Found 2 words:
 1. computer
 2. console
```

## 🛠️ Technical Architecture

### Core Components

- **Dictionary Class**: Main container managing word storage and operations
- **Trie Implementation**: Efficient prefix-based search with O(m) complexity
- **UTF-8 File I/O**: Persistent storage with Arabic text support
- **Bidirectional Indexing**: Hash-based reverse lookup for Arabic→English

### Performance Characteristics

| Operation | Time Complexity | Description |
|-----------|----------------|-------------|
| Word Search | O(1) average | Hash-based lookup |
| Prefix Search | O(m + k) | m = prefix length, k = results |
| Word Insertion | O(m) | m = word length |
| Arabic Lookup | O(1) average | Reverse index mapping |

### File Structure

```
├── main.cpp                    # Console interface and user interaction
├── enhanced_dictionary.h       # Dictionary class declaration
├── enhanced_dictionary.cpp     # Dictionary implementation
├── advanced_trie.h            # Trie data structure declaration  
├── advanced_trie.cpp          # Trie implementation
├── CMakeLists.txt             # Build configuration
└── README.md                  # Project documentation
```

## 🌐 Arabic Text Support

### Windows Setup

1. **Use Windows Terminal** (recommended) or PowerShell
2. **Set UTF-8 code page**:
   ```cmd
   chcp 65001
   ```
3. **Configure font**: Select Consolas, Cascadia Mono, or another Arabic-compatible font

### Linux/macOS Setup

Most modern terminals support UTF-8 by default. Ensure your terminal font includes Arabic glyphs.

### Input Methods

- **Copy-Paste**: Most reliable method for Arabic text input
- **Keyboard Input**: Works after proper UTF-8 setup
- **Built-in Examples**: Pre-loaded Arabic translations available

## 💾 Data Persistence

- **Storage Format**: UTF-8 encoded text file with BOM
- **File Location**: `dictionary.txt` in the application directory  
- **Format Structure**: `word|meaning1|arabic1|meaning2|arabic2|...`
- **Automatic Backup**: Data is saved after each modification

## 🔧 Development

### Building from Source

```bash
# Debug build
cmake -DCMAKE_BUILD_TYPE=Debug ..
cmake --build .

# Release build  
cmake -DCMAKE_BUILD_TYPE=Release ..
cmake --build .
```

### Code Style

- Modern C++17 features and best practices
- Clear separation of concerns
- Comprehensive error handling
- Memory-safe smart pointer usage

## ❓ Troubleshooting

### Common Issues

**Menu not appearing in IDE console**
- Press Enter once to trigger display
- Ensure "Emulate terminal in output console" is enabled

**Arabic text displaying as question marks**
- Verify UTF-8 console setup (`chcp 65001` on Windows)
- Use a font that supports Arabic glyphs
- Try running in Windows Terminal instead of CMD

**Build errors**
- Ensure C++17 compiler compatibility
- Check CMake version (3.16+ required)
- Clean build directory and retry

## 📊 Project Statistics

- **Language**: C++17
- **Lines of Code**: ~600
- **Files**: 6 source files
- **Data Structures**: Trie, Hash Table, Vector
- **Built-in Vocabulary**: 10+ words
- **Features**: 7 core operations

## 🎯 Educational Value

This project demonstrates:
- **Data Structures**: Trie implementation for efficient string operations
- **Algorithm Design**: Hash-based indexing and search optimization  
- **Memory Management**: Modern C++ memory safety practices
- **File I/O**: UTF-8 text processing and persistence
- **Cross-platform Development**: Multi-OS compatibility

## 📄 License

This project is developed for educational and demonstration purposes. Feel free to use and modify according to your needs.

---

**Built with ❤️ using modern C++17**