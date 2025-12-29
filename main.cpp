import std;
import util.console;
import util.types;
import aoc.runner;
import aoc.days;
import aoc.io;

int
main (int argc, char** argv)
{
    using namespace aoc;

    bool single{false};
    s64 selected_day{0};
    if (argc >= 2)
    {
        const char* arg = argv[1];
        auto [ptr, ec] = std::from_chars(arg, arg + std::strlen(arg), selected_day);
        if (ec == std::errc{})
            single = true;
    }

    for (auto& r : aoc::registry)
    {
        if (single && r.day != selected_day)
            continue;

        auto input  = io::read_file(input_path_for(r.day));
        auto result = r.run_text(input);
        console::print_color(
            std::format("{}: p1={} p2={} [{:.3f}ms parse, {:.3f}ms p1, {:.3f}ms p2]\n",
                        r.name,
                        result.part1,
                        result.part2,
                        result.ms_parse,
                        result.ms_p1,
                        result.ms_p2));
    }
}
