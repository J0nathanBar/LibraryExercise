cmake_minimum_required(VERSION 4.2)
project(LibraryExercise)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_COMPILE_WARNING_AS_ERROR ON)
add_executable(LibraryExercise main.cpp
        Book.cpp
        BookLibrary.cpp
        TextUI.cpp
        TextUI.hpp
)

target_compile_options(LibraryExercise PRIVATE -Wall -Wextra -Wpedantic -Wshadow -Wunused -Wconversion -Wuninitialized -Wsign-conversion)
