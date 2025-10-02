# English-Arabic Dictionary System

A high-performance C++ dictionary application featuring bilingual support, multiple meanings per word, and persistent storage.

![Language](https://img.shields.io/badge/Language-C%2B%2B17-blue)
![Platform](https://img.shields.io/badge/Platform-Windows%20%7C%20Linux-lightgrey)
![License](https://img.shields.io/badge/License-MIT-green)

## 🌟 Features

- **Bilingual Support**: English words with Arabic translations (🇬🇧 ↔️ 🇸🇦)
- **Multiple Meanings**: Store unlimited definitions per word
- **Advanced Search Capabilities**: 
  - Exact word lookup
  - Prefix autocomplete
  - Reverse meaning search (find words by definition)
- **Professional Data Structures**: Trie, Hash Table, and Vector implementations
- **Persistent Storage**: Automatically saves user-added words to file
- **Fast Performance**: Optimized with caching and efficient algorithms
- **Thread-Safe**: Mutex protection for concurrent operations
- **UTF-8 Support**: Full Unicode handling for Arabic text

## 📋 Table of Contents

- [Installation](#-installation)
- [Usage](#-usage)
- [Project Structure](#-project-structure)
- [Building](#-building)
- [Features Detail](#-features-detail)
- [Contributing](#-contributing)
- [License](#-license)
- [Contact](#-contact)

## 🚀 Installation

### Prerequisites

- **C++17 compiler** (GCC 7+, Clang 5+, MSVC 2017+)
- **CMake** 3.10 or higher
- **Git**

### Clone the Repository

```bash
git clone https://github.com/YOUR_USERNAME/english-arabic-dictionary.git
cd english-arabic-dictionary
```

### Build Instructions

#### Windows (CLion/MinGW)

```bash
mkdir build
cd build
cmake ..
cmake --build .
```

#### Linux/Mac

```bash
mkdir build
cd build
cmake ..
make
./DictionarySystem
```

## 💻 Usage

Run the application:

```bash
./DictionarySystem
```

### Menu Options

```
==========================================================
          ENGLISH-ARABIC DICTIONARY SYSTEM
==========================================================

  1. Search for word
  2. Autocomplete (prefix search)
  3. Get Arabic translation
  4. Search by meaning

  5. Add new word
  6. Add meaning to existing word
  7. Delete word/meaning

  8. Show statistics
  0. Exit
```

### Example Usage

**Adding a new word:**
```
Enter word: computer
Enter meaning: Electronic device for processing data
Enter type: noun
Enter Arabic: الحاسوب
✓ Word added successfully!
```

**Searching:**
```
Enter word: cat
Result: Small domesticated carnivorous mammal
Arabic: القطة
Type: noun
```

**Autocomplete:**
```
Enter prefix: comp
Results:
  1. computer
  2. compile
  3. complete
  4. compare
```

## 📁 Project Structure

```
english-arabic-dictionary/
├── main.cpp                    # Main application entry point
├── enhanced_dictionary.h       # Dictionary class declaration
├── enhanced_dictionary.cpp     # Dictionary implementation
├── advanced_trie.h            # Trie data structure header
├── advanced_trie.cpp          # Trie implementation
├── CMakeLists.txt             # Build configuration
├── README.md                  # This file
├── .gitignore                 # Git ignore rules
└── user_words.txt             # Saved user words (auto-generated)
```

## 🏗️ Building

### CMake Configuration

The project uses CMake for cross-platform building:

```cmake
cmake_minimum_required(VERSION 3.10)
project(DictionarySystem)

set(CMAKE_CXX_STANDARD 17)

add_executable(DictionarySystem
    main.cpp
    enhanced_dictionary.cpp
    advanced_trie.cpp
)
```

### Compile Options

- **Debug Build**: `cmake -DCMAKE_BUILD_TYPE=Debug ..`
- **Release Build**: `cmake -DCMAKE_BUILD_TYPE=Release ..`
- **With Optimizations**: `cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_FLAGS="-O3" ..`

## ✨ Features Detail

### Data Structures

1. **Trie (Prefix Tree)**: 
   - Fast O(m) prefix-based search where m = word length
   - Efficient autocomplete functionality
   - Memory-optimized node structure

2. **Hash Table**: 
   - O(1) average lookup time for exact matches
   - Collision handling with chaining
   - Dynamic resizing

3. **Vector**: 
   - Dynamic storage for word list
   - Fast iteration for meaning search

### Multi-Meaning Support

Each word entry can store:
- Multiple definitions (unlimited)
- Multiple Arabic translations
- Part of speech tags (noun, verb, adjective, adverb, etc.)
- Usage frequency data
- Custom metadata

### Persistence Layer

- User-added words automatically saved to `user_words.txt`
- Automatic loading on application startup
- Built-in dictionary with 70+ common words
- File format: Pipe-delimited for easy parsing
- Crash-safe: Saves after each modification

### Performance Features

- **LRU Caching**: Recently searched words cached for instant retrieval
- **Thread Safety**: Mutex protection for all data modifications
- **Memory Optimization**: String interning and efficient cleanup
- **Lazy Loading**: Dictionary loaded only when needed

## 🎓 Educational Value

This project demonstrates:
- ✅ Advanced C++ programming (C++17)
- ✅ Data structure implementation (Trie, Hash Table)
- ✅ Algorithm optimization (caching, search)
- ✅ File I/O and persistence
- ✅ Unicode/UTF-8 handling for Arabic text
- ✅ Object-oriented design principles
- ✅ Memory management and RAII
- ✅ Cross-platform development
- ✅ Threading and synchronization
- ✅ Software architecture patterns

### Technical Highlights

- **Design Pattern**: Singleton-like dictionary instance
- **Memory Safety**: Smart pointers, RAII, no memory leaks
- **Error Handling**: Graceful failure with user feedback
- **Code Quality**: Clean code, meaningful names, comments
- **Testing**: Robust input validation

## 🤝 Contributing

Contributions are welcome! Here's how you can help:

### Ways to Contribute
- 🐛 Report bugs
- 💡 Suggest new features
- 📝 Improve documentation
- 🌍 Add more languages
- ⚡ Optimize performance

### Development Process

1. Fork the repository
2. Create your feature branch (`git checkout -b feature/AmazingFeature`)
3. Commit your changes (`git commit -m 'Add some AmazingFeature'`)
4. Push to the branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request

### Code Style
- Follow C++ Core Guidelines
- Use meaningful variable names
- Add comments for complex logic
- Write clean, readable code

## 📝 License

This project is licensed under the **MIT License** - see below for details:

### MIT License

```
MIT License

Copyright (c) 2025

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
```

### What does MIT License mean?

✅ **Commercial use** - Use for commercial projects  
✅ **Modification** - Modify the code freely  
✅ **Distribution** - Share with others  
✅ **Private use** - Use privately  
✅ **No Liability** - Author not responsible for damages  

This is one of the most permissive and business-friendly open source licenses.

## 📞 Contact

### Developer Information

**Email**: [fohklo@gmail.com](mailto:fohklo@gmail.com)  
**Phone**: +20 106 825 7061  
**GitHub**: [@YOUR_USERNAME](https://github.com/YOUR_USERNAME)  
**LinkedIn**: [Your LinkedIn Profile](https://linkedin.com/in/YOUR_PROFILE)

### Get in Touch

- 📧 For questions: fohklo@gmail.com
- 🐛 For bug reports: Use GitHub Issues
- 💼 For business inquiries: fohklo@gmail.com
- 📱 WhatsApp: +20 106 825 7061

Feel free to reach out for:
- Technical questions
- Collaboration opportunities
- Feature requests
- Bug reports
- General feedback

## 🙏 Acknowledgments

- Built as a portfolio demonstration project
- Implements concepts from Data Structures & Algorithms
- Inspired by professional dictionary applications
- Developed using modern C++ best practices

## 🌟 Project Stats

- **Language**: C++17
- **Lines of Code**: ~1500+
- **Files**: 6 source files
- **Data Structures**: 3 (Trie, Hash, Vector)
- **Built-in Words**: 70+
- **Features**: 8 core operations
- **Platform**: Cross-platform (Windows/Linux/Mac)

## 📊 Performance Metrics

- **Search Time**: O(m) where m = word length
- **Insert Time**: O(m)
- **Autocomplete**: O(n) where n = matches
- **Memory**: ~1MB for 1000 words
- **Cache Hit Rate**: ~60-80% for typical usage

## 🚀 Future Enhancements

- [ ] Web interface (REST API)
- [ ] Mobile application
- [ ] More languages (French, Spanish, etc.)
- [ ] Voice search
- [ ] Offline mobile dictionary
- [ ] Browser extension
- [ ] Word pronunciation (audio)
- [ ] Example sentences
- [ ] Word etymology
- [ ] Flashcard quiz mode

## 📚 Resources

- [C++ Reference](https://en.cppreference.com/)
- [CMake Documentation](https://cmake.org/documentation/)
- [Trie Data Structure](https://en.wikipedia.org/wiki/Trie)
- [Hash Tables](https://en.wikipedia.org/wiki/Hash_table)

---

<div align="center">

### ⭐ Star this repo if you find it helpful!

**Made with ❤️ for learning and sharing knowledge**

[Report Bug](https://github.com/YOUR_USERNAME/english-arabic-dictionary/issues) • 
[Request Feature](https://github.com/YOUR_USERNAME/english-arabic-dictionary/issues) • 
[Documentation](https://github.com/YOUR_USERNAME/english-arabic-dictionary/wiki)

</div>

---

### 📄 Quick Links

- [Installation Guide](#-installation)
- [Usage Examples](#-usage)
- [API Documentation](docs/API.md)
- [Contributing Guidelines](#-contributing)
- [License](#-license)
- [Contact Developer](#-contact)

### 🏆 Achievements

![GitHub stars](https://img.shields.io/github/stars/YOUR_USERNAME/english-arabic-dictionary?style=social)
![GitHub forks](https://img.shields.io/github/forks/YOUR_USERNAME/english-arabic-dictionary?style=social)
![GitHub watchers](https://img.shields.io/github/watchers/YOUR_USERNAME/english-arabic-dictionary?style=social)

---

**© 2025 | Free and Open Source | MIT License**
