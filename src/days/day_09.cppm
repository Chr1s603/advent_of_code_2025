export module aoc.day09;

import std;
import util.types;

export namespace day09 {

struct Day09
{
    static constexpr s64 number = 9;
    static constexpr sv  name{"Day 09: Example"};

    using Pair   = pair<s64, s64>;
    using Parsed = vec<Pair>;

    static Parsed parse (sv s)
    {
        return {};
    }

    static s64 part1 (const Parsed& in)
    {
        return 0;
    }

    static s64 part2 (const Parsed& in)
    {
        return 0;
    }

    static constexpr pair<s64, s64> expected ()
    {
        return {0, 0};
    }
};

} // namespace day09
