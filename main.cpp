import std;
import aoc.runner;
import aoc.days;
import aoc.io;

int main(int argc, char** argv) {
    using namespace aoc;

    if (argc == 2) {
        int day = std::stoi(argv[1]);
        auto it = std::find_if(aoc::registry.begin(), aoc::registry.end(),
                               [day](auto& r){ return r.day == day; });
        if (it == aoc::registry.end()) {
            std::cerr << "No such day: " << day << "\n";
            return 1;
        }

        auto input = io::read_file(input_path_for(day));
        auto result = it->run_text(input);
        std::println("Day {:02d}: p1={} p2={} [{:.3f}ms parse, {:.3f}ms p1, {:.3f}ms p2]",
                     day, result.part1, result.part2, result.ms_parse, result.ms_p1, result.ms_p2);
    } else {
        for (auto& r : aoc::registry) {
            auto input = io::read_file(input_path_for(r.day));
            auto result = r.run_text(input);
            std::println("Day {:02d}: p1={} p2={} [{:.3f}ms parse, {:.3f}ms p1, {:.3f}ms p2]",
                         r.day, result.part1, result.part2, result.ms_parse, result.ms_p1, result.ms_p2);
        }
    }
}
