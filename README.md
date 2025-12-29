# Advent of Code 2025 — C++26 solutions

This repo contains my C++ solutions for the Advent of Code 2025.
The idea was to get my feet wet with C++ modules, but as I found out, support is still very experimental...

**Prerequisites:** Linux, Clang with C++ modules support (tested with Clang 21.1.6), Ninja generator (required for module builds), and libc++ (to precompile std module for `import std` support),

**Quick start**

Use the provided CMake presets (they use the Ninja generator, Unix makefiles don't like modules...):

```bash
git clone <repo>
git submodule update --init --recursive
mkdir build
cd build
cmake .. --preset Release
cmake --build .

# Run all days
./advent_of_code_2025

# Run a single day (numeric argument)
./advent_of_code_2025 03
```

**Project layout**
- `src/` — module-based day implementations and utilities.
- `inputs/` — problem inputs named `day_01.txt`, `day_02.txt`, …
- `build/` — out-of-tree CMake build artifacts.

**Notes**
- This project requires a precompiled `std.pcm` (see `CMakeLists.txt` for how the build locates it). The included `CMakePresets.json` is configured to use the Ninja generator which is necessary for C++ module builds.
- The runner accepts an optional numeric argument to run a single day (e.g., `03`). Without an argument it runs all registered days.
- There is a analyzer target to get some static checks running. As of now, ClangTidy does not work with modules, so the workaround using direct compiler invocation is used (see `CMakeLists.txt`)

