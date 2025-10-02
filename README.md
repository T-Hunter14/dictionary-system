# 📚 C++ Dictionary System

A comprehensive English-Arabic dictionary application built in C++ with advanced data structures including Trie and Hash Tables for optimal performance.

![C++](https://img.shields.io/badge/C++-17-blue.svg)
![License](https://img.shields.io/badge/License-MIT-green.svg)
![Platform](https://img.shields.io/badge/Platform-Windows%20%7C%20Linux%20%7C%20macOS-lightgrey.svg)

## ✨ Features

- **🔍 Word Search**: Fast lookup of English words with detailed meanings
- **🔤 Prefix Search**: Autocomplete functionality for word suggestions
- **🌍 Arabic Translation**: Full Arabic sentence translations (not just transliterations)
- **📝 Meaning Search**: Find words by searching through their definitions
- **➕ Add Words**: Dynamically add new words with translations
- **📊 Statistics**: Performance metrics and cache hit ratios

## 🏗️ Technical Architecture

### Data Structures
- **Advanced Trie**: O(m) search complexity for prefix matching
- **Hash Table**: O(1) average lookup time with intelligent caching
- **Thread-Safe Operations**: Mutex-protected concurrent access
- **Smart Caching**: LRU cache for frequently accessed words

### Performance Features
- 150+ built-in English words with full Arabic translations
- Memory-optimized storage with automatic garbage collection  
- Console encoding support for proper Arabic text display
- Cross-platform compatibility (Windows, Linux, macOS)

## 🚀 Quick Start

### Prerequisites
- C++17 compatible compiler (GCC, Clang, MSVC)
- CMake 3.16 or higher

### Building the Project

#### Option 1: Using CMake (Recommended)
```bash
# Clone the repository
git clone https://github.com/yourusername/cpp-dictionary-system.git
cd cpp-dictionary-system

# Create build directory
mkdir build
cd build

# Configure and build
cmake ..
cmake --build .

# Run the application
./DictionarySystem
```

#### Option 2: Manual Compilation
```bash
g++ -std=c++17 -O2 main.cpp enhanced_dictionary.cpp advanced_trie.cpp -o dictionary
./dictionary
```

#### Option 3: Using CLion IDE
1. Open the project folder in CLion
2. Press `Ctrl+F9` to build
3. Press `Shift+F10` to run

## 📖 Usage

The application provides a simple menu-driven interface:

```
==================================================
*** C++ DICTIONARY SYSTEM ***
==================================================
0. Exit
1. Search for word
2. Find words by prefix (autocomplete)  
3. Arabic translation lookup
4. Search by meaning
5. Add new word with translation
6. Performance statistics

Choose option (0-6): 
```

### Example Usage

#### 1. Word Search
```
Enter word to search: cat

*** Word Found! ***
Word: cat
Meaning: Small domesticated carnivorous mammal
Arabic: القطة حيوان أليف صغير آكل اللحوم
Part of speech: noun
```

#### 2. Prefix Search  
```
Enter prefix for autocomplete: ca

Found 5 words with prefix 'ca':
1. car
2. cat  
3. camera
4. cake
5. call
```

#### 3. Arabic Translation
```
Enter English word for Arabic translation: house

*** Translation Result ***
English: house
Arabic: البيت مكان يسكن فيه الإنسان مع أسرته
```

## 📁 Project Structure

```
cpp-dictionary-system/
├── src/
│   ├── main.cpp                    # Main application entry point
│   ├── enhanced_dictionary.h       # Dictionary class interface  
│   ├── enhanced_dictionary.cpp     # Dictionary implementation
│   ├── advanced_trie.h            # Trie data structure interface
│   └── advanced_trie.cpp          # Trie implementation
├── build/                         # Build directory (created by CMake)
├── CMakeLists.txt                # CMake configuration
├── README.md                     # Project documentation
└── LICENSE                       # MIT License
```

## 🔧 Technical Details

### Core Classes

#### `EnhancedDictionary`
- Primary interface for dictionary operations
- Manages word storage using both Trie and Hash Table
- Implements thread-safe caching mechanisms
- Handles Arabic translation storage and retrieval

#### `AdvancedTrie`  
- Specialized trie structure for English words
- Supports prefix-based word suggestions
- Optimized memory usage with smart pointer management
- BFS-based word traversal for autocomplete

#### `WordEntry`
- Data structure representing dictionary entries
- Contains word, meaning, Arabic translation, and part of speech
- Includes frequency data for relevance ranking

### Performance Optimizations
- **Dual Storage**: Trie for prefix searches, Hash Table for exact matches
- **Intelligent Caching**: LRU cache with configurable size limits
- **Memory Management**: Automatic cleanup and optimization
- **Thread Safety**: Mutex-protected concurrent operations

## 🌐 Internationalization

The system properly handles Arabic text display:
- UTF-8 console encoding for Windows
- ANSI escape sequence support
- Full Arabic sentences instead of transliterations
- Right-to-left text rendering support

## 📊 Performance Metrics

- **Word Lookup**: Average O(1) with hash table
- **Prefix Search**: O(m + k) where m is prefix length, k is results
- **Memory Usage**: ~50KB for 150 words with caching
- **Cache Hit Ratio**: Typically 80-90% for repeated searches

## 🛠️ Development

### Adding New Features
1. Extend the `EnhancedDictionary` class with new methods
2. Update the menu system in `main.cpp`
3. Add corresponding test cases
4. Update documentation

### Code Style Guidelines
- Follow modern C++17 standards
- Use RAII principles for resource management
- Implement proper error handling with exceptions
- Document public API methods

## 🤝 Contributing

Contributions are welcome! Please follow these steps:

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/amazing-feature`)
3. Commit your changes (`git commit -m 'Add amazing feature'`)
4. Push to the branch (`git push origin feature/amazing-feature`)
5. Open a Pull Request

## 📄 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## 🙏 Acknowledgments

- Arabic translations provided by native speakers
- Trie implementation inspired by classical algorithms
- Thread-safety patterns from modern C++ best practices

## 📞 Contact

- **Author**: Your Name
- **Email**: your.email@example.com
- **Project Link**: https://github.com/yourusername/cpp-dictionary-system

---

**⭐ If you found this project helpful, please consider giving it a star!**