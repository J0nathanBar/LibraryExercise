# Book Library

A thread-safe in-memory book management system built with C++20.

## Features
- **Concurrent Access:** Multiple subscribers can borrow, return, and lookup books simultaneously without blocking each other.
- **Asynchronous Capacity Management:** A background worker automatically sweeps and removes the least popular books when capacity is exceeded.
- **$O(1)$ Removals:** Core storage utilizes a contiguous array with swap-and-pop mechanics for lightning-fast memory management.
- **Standard Library Operations:** Add, remove, and query the current status of the entire library.
- **Borrowed Book Protection:** Borrowed books cannot be removed from the library. how can you remove something that isn't there

## Building and Running

This project uses CMake and compiles with strict warnings (`-Wall -Wextra -Wpedantic -Werror`) to ensure strict memory and type safety.

```bash
mkdir build
cd build
cmake ..
make
./LibraryExercise