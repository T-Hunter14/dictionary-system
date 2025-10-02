cmake_minimum_required(VERSION 3.16)
project(DictionarySystem VERSION 1.0.0)

# Set C++17 standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Compiler optimizations
if(MSVC)
    add_compile_options(/W4 /O2)
else()
    add_compile_options(-Wall -Wextra -O3)
endif()

# Find threading support
find_package(Threads REQUIRED)

# Source files
set(SOURCES
        main.cpp
        enhanced_dictionary.cpp
        advanced_trie.cpp
)

# Create executable
add_executable(DictionarySystem ${SOURCES})

# Link threading library
target_link_libraries(DictionarySystem PRIVATE Threads::Threads)

# Windows console support
if(WIN32)
    target_compile_definitions(DictionarySystem PRIVATE _WIN32_WINNT=0x0A00)
endif()

# Custom run target
add_custom_target(run
        COMMAND DictionarySystem
        DEPENDS DictionarySystem
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)

message(STATUS "Build: cmake .. && make")
message(STATUS "Run: ./DictionarySystem")