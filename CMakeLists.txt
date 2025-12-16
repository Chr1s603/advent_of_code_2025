cmake_minimum_required(VERSION 4.0)
project(advent_of_code_2025 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# ------------------------
# Precompile libc++ std module
# ------------------------
set(STD_PCM "${CMAKE_BINARY_DIR}/std.pcm")

add_custom_command(
    OUTPUT ${STD_PCM}
    COMMAND ${CMAKE_CXX_COMPILER}
        -std=c++26 -stdlib=libc++
        -I/usr/include/c++/v1
        --precompile /usr/share/libc++/v1/std.cppm
        -o ${STD_PCM}
    COMMENT "Precompiling libc++ std module"
)
add_custom_target(precompile_std DEPENDS ${STD_PCM})


# ------------------------
# Common compile options
# ------------------------
set(COMMON_COMPILE_OPTIONS
        -Wall -Wextra -Wpedantic
        -stdlib=libc++
        -fmodule-file=std=${STD_PCM}
        -Wno-reserved-module-identifier
)

# ------------------------
# Common library for runner/io
# ------------------------
add_library(aoc_common SHARED)
add_dependencies(aoc_common precompile_std)
target_compile_options(aoc_common PRIVATE ${COMMON_COMPILE_OPTIONS})
set_target_properties(aoc_common PROPERTIES CXX_STANDARD 26)
target_sources(aoc_common
        PUBLIC
        FILE_SET aoc_common_modules TYPE CXX_MODULES FILES
        "${CMAKE_CURRENT_SOURCE_DIR}/src/types.cppm"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/console.cppm"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/enum.cppm"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/parse.cppm"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/runner.cppm"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/io.cppm")

# ------------------------
# Function to add day modules
# ------------------------
set(AOC_DAY_TARGETS "")
function(add_aoc_day day_number)
    set(day_target "day${day_number}_target")
    set(day_source "${CMAKE_CURRENT_SOURCE_DIR}/src/days/day_${day_number}.cppm")

    add_library(${day_target} SHARED)
    add_dependencies(${day_target} precompile_std)
    target_compile_options(${day_target} PRIVATE ${COMMON_COMPILE_OPTIONS})
    target_link_libraries(${day_target} PRIVATE aoc_common)
    set_target_properties(${day_target} PROPERTIES CXX_STANDARD 26)
    target_sources(${day_target}
            PUBLIC
            FILE_SET "${day_target}_modules" TYPE CXX_MODULES FILES
            ${day_source})

    set(AOC_DAY_TARGETS "${AOC_DAY_TARGETS}" "${day_target}" PARENT_SCOPE)
endfunction()

# ------------------------
# Add day modules
# ------------------------
add_aoc_day(01)
add_aoc_day(02)
add_aoc_day(03)
add_aoc_day(04)
add_aoc_day(05)
add_aoc_day(06)
add_aoc_day(07)
add_aoc_day(08)
add_aoc_day(09)
add_aoc_day(10)
add_aoc_day(11)
add_aoc_day(12)

# ------------------------
# Main executable
# ------------------------
add_executable(advent_of_code_2025 "${CMAKE_CURRENT_SOURCE_DIR}/main.cpp" )
target_link_libraries(advent_of_code_2025 PRIVATE aoc_common ${AOC_DAY_TARGETS} c++)
add_dependencies(advent_of_code_2025 precompile_std)
target_compile_options(advent_of_code_2025 PRIVATE
        ${COMMON_COMPILE_OPTIONS}
        -fprebuilt-module-path=${CMAKE_BINARY_DIR}
        -fprebuilt-module-path=${CMAKE_BINARY_DIR}/CMakeFiles/aoc_common.dir/src
        -fprebuilt-module-path=${CMAKE_BINARY_DIR}/CMakeFiles/day01_target.dir/src/days
)
target_sources(advent_of_code_2025
        PUBLIC
        FILE_SET advent_of_code_2025_modules TYPE CXX_MODULES FILES
        "${CMAKE_CURRENT_SOURCE_DIR}/src/days.cppm")
