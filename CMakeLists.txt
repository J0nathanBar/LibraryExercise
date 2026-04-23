cmake_minimum_required(VERSION 4.2)
project(LibraryExercise)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_COMPILE_WARNING_AS_ERROR ON)
add_executable(LibraryExercise
        src/main.cpp
        src/Book.cpp
        src/BookLibrary.cpp
        src/TextUI.cpp
)
target_include_directories(LibraryExercise PRIVATE include)
target_compile_options(LibraryExercise PRIVATE -Wall -Wextra -Wpedantic -Wshadow -Wunused -Wconversion -Wuninitialized -Wsign-conversion)
