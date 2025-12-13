export module aoc.runner;

import std;
import util.console;
import util.types;

export namespace aoc {

struct run_result
{
    s64 day;
    s64 part1;
    s64 part2;
    f64 ms_parse;
    f64 ms_p1;
    f64 ms_p2;
};

using day_function = run_result (*)(sv);

struct runner
{
    s64          day{};
    sv           name{};
    day_function run_text{};
};

struct timer
{
    std::chrono::steady_clock::time_point t{std::chrono::steady_clock::now()};

    f64 split_ms ()
    {
        const auto now = std::chrono::steady_clock::now();
        const auto ms  = std::chrono::duration<double, std::milli>(now - t).count();
        t              = now;
        return ms;
    }
};

template <typename Day>
    requires requires(Day d, sv text) {
        { Day::parse(text) };
        { Day::part1(Day::parse(text)) };
        { Day::part2(Day::parse(text)) };
        { Day::expected() };
        { Day::name } -> std::convertible_to<std::string_view>;
    }
run_result
run_text_impl (sv text)
{
    timer tm;

    const auto parsed   = Day::parse(text);
    cf64       ms_parse = tm.split_ms();

    cs64 p1    = static_cast<s64>(Day::part1(parsed));
    cf64 ms_p1 = tm.split_ms();

    cs64 p2    = static_cast<s64>(Day::part2(parsed));
    cf64 ms_p2 = tm.split_ms();

    const auto [exp_p1, exp_p2] = Day::expected();
    if (p1 != exp_p1)
        console::warn(std::format("Day {} Part 1 incorrect", Day::number), p1, exp_p1);

    if (p2 != exp_p2)
        console::warn(std::format("Day {} Part 2 incorrect", Day::number), p2, exp_p2);

    return {Day::number, p1, p2, ms_parse, ms_p1, ms_p2};
}

template <typename Day>
    requires requires(Day d, sv text) {
        { Day::parse(text) };
        { Day::part1(Day::parse(text)) };
        { Day::part2(Day::parse(text)) };
        { Day::expected() };
        { Day::name } -> std::convertible_to<std::string_view>;
    }
consteval runner
make_runner ()
{
    return runner{Day::number, Day::name, &run_text_impl<Day>};
}

inline str
input_path_for (cs64 day, sv base_dir = "../inputs")
{
    auto path = std::format("{}/day_{:02}.txt", base_dir, day);
    if (!std::filesystem::exists(path))
        console::warn("Input file missing", path, "exists");

    return path;
}

}
