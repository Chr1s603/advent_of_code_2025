cmake_minimum_required(VERSION 4.0)

# ===========================================
# Project
# ===========================================
project(advent_of_code_2025
        LANGUAGES CXX
        DESCRIPTION "Advent of Code 2025 using C++26 modules")

# ===========================================
# Language & tooling
# ===========================================
set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Default build type for single-config generators
if(NOT CMAKE_CONFIGURATION_TYPES AND NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

# ===========================================
# Warnings & build flags
# ===========================================
set(AOC_WARNINGS
    -Wall -Wextra -Wpedantic
    -Wconversion -Wsign-conversion -Wshadow
    -Wnon-virtual-dtor -Wold-style-cast -Woverloaded-virtual
    -Wnull-dereference -Wdouble-promotion -Wformat=2
    -Wimplicit-fallthrough -Wundef -Wheader-hygiene
    -Wmissing-declarations -Wexit-time-destructors -Wglobal-constructors
    -Wweak-vtables -Wimplicit-fallthrough -Wcast-align
    -Wswitch-enum -Wfloat-equal -Wcomma
)

set(AOC_BUILD_FLAGS
    $<$<CONFIG:Debug>:-O0 -g -fno-omit-frame-pointer -DDEBUG>
    $<$<CONFIG:Release>:-O3 -DNDEBUG>
)

set(AOC_COMMON_OPTIONS
    ${AOC_WARNINGS}
    ${AOC_BUILD_FLAGS}
    -stdlib=libc++
    -Wno-reserved-module-identifier
)

# ===========================================
# Precompile libc++ standard module per config
# ===========================================
set(STD_PCM "${CMAKE_BINARY_DIR}/std.pcm")
add_custom_command(
    OUTPUT ${STD_PCM}
    COMMAND ${CMAKE_CXX_COMPILER}
        -std=c++26
        -stdlib=libc++
        -I/usr/include/c++/v1
        --precompile /usr/share/libc++/v1/std.cppm
        -o ${STD_PCM}
    COMMENT "Precompiling libc++ std module"
)

add_custom_target(precompile_std ALL DEPENDS ${STD_PCM})

# ===========================================
# Common library
# ===========================================
add_library(aoc_common SHARED)
add_dependencies(aoc_common precompile_std)
target_compile_options(aoc_common PRIVATE
    ${AOC_COMMON_OPTIONS}
    $<$<COMPILE_LANGUAGE:CXX>:-fmodule-file=std=${STD_PCM}>
)
target_link_libraries(aoc_common PRIVATE c++)
set_target_properties(aoc_common PROPERTIES CXX_STANDARD 26)

target_sources(aoc_common
    PUBLIC
    FILE_SET aoc_common_modules TYPE CXX_MODULES FILES
        src/types.cppm
        src/parallel.cppm
        src/console.cppm
        src/enum.cppm
        src/parse.cppm
        src/runner.cppm
        src/io.cppm
)

# ===========================================
# Discover day modules
# ===========================================
file(GLOB AOC_DAY_MODULES CONFIGURE_DEPENDS "src/days/day_*.cppm")

add_library(aoc_days SHARED)
add_dependencies(aoc_days precompile_std)
target_compile_options(aoc_days PRIVATE
    ${AOC_COMMON_OPTIONS}
    $<$<COMPILE_LANGUAGE:CXX>:-fmodule-file=std=${STD_PCM}>
)
target_link_libraries(aoc_days PRIVATE aoc_common)
set_target_properties(aoc_days PROPERTIES CXX_STANDARD 26)

foreach(day_module IN LISTS AOC_DAY_MODULES)
    get_filename_component(day_name ${day_module} NAME_WE)
    target_sources(aoc_days
        PUBLIC
        FILE_SET "day_${day_name}_module" TYPE CXX_MODULES FILES
            ${day_module}
    )
endforeach()

# ===========================================
# Main executable
# ===========================================
add_executable(advent_of_code_2025 main.cpp)
add_dependencies(advent_of_code_2025 precompile_std)
target_compile_options(advent_of_code_2025 PRIVATE
    ${AOC_COMMON_OPTIONS}
    $<$<COMPILE_LANGUAGE:CXX>:-fmodule-file=std=${STD_PCM}>
)
target_link_libraries(advent_of_code_2025 PRIVATE aoc_common aoc_days c++)
set_target_properties(advent_of_code_2025 PROPERTIES CXX_STANDARD 26)

target_sources(advent_of_code_2025
    PUBLIC
    FILE_SET advent_modules TYPE CXX_MODULES FILES
        src/days.cppm
)

# ===========================================
# Analyzer sources
# ===========================================

# Start with main.cpp
set(AOC_ANALYZE_SOURCES
    main.cpp
)

# Add all project module interfaces
file(GLOB_RECURSE AOC_MODULE_SOURCES
    CONFIGURE_DEPENDS
    src/*.cppm
)
list(APPEND AOC_ANALYZE_SOURCES ${AOC_MODULE_SOURCES})

# ===========================================
# Prebuilt module directory
# ===========================================
set(AOC_PCM_DIR "${CMAKE_BINARY_DIR}/pcm")
file(MAKE_DIRECTORY ${AOC_PCM_DIR})
file(GLOB_RECURSE AOC_ALL_PCMS
    CONFIGURE_DEPENDS
    "${CMAKE_BINARY_DIR}/CMakeFiles/aoc_common.dir/*.pcm"
    "${CMAKE_BINARY_DIR}/CMakeFiles/aoc_days.dir/*.pcm"
    "${CMAKE_BINARY_DIR}/CMakeFiles/advent_of_code_2025.dir/*.pcm"
)

add_custom_target(copy_pcm ALL
    DEPENDS aoc_common aoc_days advent_of_code_2025
    COMMAND ${CMAKE_COMMAND} -E make_directory "${AOC_PCM_DIR}"
    COMMAND ${CMAKE_COMMAND} -E copy ${AOC_ALL_PCMS} "${AOC_PCM_DIR}"
)

# ===========================================
# Clang static analyzer target (always produces HTML)
# ===========================================
set(AOC_ANALYZE_DIR "${CMAKE_BINARY_DIR}/analyze-html")
file(MAKE_DIRECTORY "${AOC_ANALYZE_DIR}")

# Collect all project sources with absolute paths
set(AOC_ANALYZE_SOURCES
    ${CMAKE_SOURCE_DIR}/main.cpp
)
file(GLOB_RECURSE AOC_MODULE_SOURCES
    CONFIGURE_DEPENDS
    ${CMAKE_SOURCE_DIR}/src/*.cppm
)
list(APPEND AOC_ANALYZE_SOURCES ${AOC_MODULE_SOURCES})

add_custom_target(analyze
    DEPENDS copy_pcm precompile_std "${DUMMY_ANALYZE_SRC}" "${DUMMY_ALPHA_SRC}"
    COMMENT "Running clang static analyzer (HTML output)"

    COMMAND ${CMAKE_COMMAND} -E rm -rf "${AOC_ANALYZE_DIR}"
    COMMAND ${CMAKE_COMMAND} -E make_directory "${AOC_ANALYZE_DIR}"

    COMMAND scan-build
        --use-analyzer=${CMAKE_CXX_COMPILER}
        --status-bugs
        -o "${AOC_ANALYZE_DIR}"
        ${CMAKE_CXX_COMPILER}
            -std=c++26
            -stdlib=libc++
            -I/usr/include/c++/v1
            -fmodule-file=std=${STD_PCM}
            -fprebuilt-module-path=${AOC_PCM_DIR}
            -Wall -Wextra -Wpedantic
            ${AOC_ANALYZE_SOURCES}
            "${DUMMY_ANALYZE_SRC}"
            "${DUMMY_ALPHA_SRC}"
)
