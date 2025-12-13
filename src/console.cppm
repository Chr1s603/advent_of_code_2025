// util/console.cppm
export module util.console;

import std;

import util.types;

export namespace console { // exported namespace
enum class color
{
    Default = 39,
    Red     = 31,
    Green   = 32,
    Yellow  = 33,
    Cyan    = 36
};

// Print string with ANSI color
inline void
print_color (sv msg, color fg = color::Default)
{
    std::print("\033[{}m{}\033[0m", static_cast<s32>(fg), msg);
}

// Generic warning (yellow)
template <typename T1, typename T2>
inline void
warn (sv context, T1 got, T2 expected)
{
    print_color(std::format("Warning: {} Got: {}, expected: {}\n", context, got, expected),
                color::Yellow);
}

// Generic info (green by default)
template <typename T>
inline void
info (sv context, T value, color fg = color::Green)
{
    print_color(std::format("{}: {}\n", context, value), fg);
}

// Generic success (cyan)
template <typename T>
inline void
success (sv context, T value)
{
    info(context, value, color::Cyan);
}

}
