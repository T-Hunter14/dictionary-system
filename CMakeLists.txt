cmake_minimum_required(VERSION 3.20)
project(DictionarySystem VERSION 1.0.0)

# C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Compiler flags
if(MSVC)
    add_compile_options(/W4 /O2 /GL)
    add_link_options(/LTCG)
else()
    add_compile_options(-Wall -Wextra -O3)
    if(CMAKE_BUILD_TYPE STREQUAL "Release")
        add_compile_options(-march=native -flto)
        add_link_options(-flto)
    endif()
endif()

# Source files
set(SOURCES
        main.cpp
        enhanced_dictionary.cpp
        advanced_trie.cpp
)

# Executable
add_executable(DictionarySystem ${SOURCES})

# Windows console UTF-8 support
if(WIN32)
    target_compile_definitions(DictionarySystem PRIVATE _WIN32_WINNT=0x0A00)
endif()

# Convenience run target
add_custom_target(run
        COMMAND DictionarySystem
        DEPENDS DictionarySystem
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)

message(STATUS "Build: cmake -DCMAKE_BUILD_TYPE=Release .. && make")
message(STATUS "Run:   ./DictionarySystem")