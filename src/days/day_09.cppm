export module aoc.day09;

import std;
import util.types;
import util.parse;

export namespace day09 {

struct Day09
{
    static constexpr s64 number = 9;
    static constexpr sv  name{"Day 09: Movie Theater"};

    using position      = pair<s64, s64>;
    using red_positions = vec<position>;

    static red_positions parse (const sv in)
    {
        const auto positions_string{util::parse::split(in, "\n")};

        red_positions result;
        for (const auto& position_str : positions_string)
        {
            const auto x_y_str{util::parse::split(position_str, ",")};
            result.emplace_back(util::parse::to_number(x_y_str[0]),
                                util::parse::to_number(x_y_str[1]));
        }

        return result;
    }

    static s64 part1 (const red_positions& positions)
    {
        s64 largest_rect_area{0};
        for (u64 a = 0; a < positions.size(); ++a)
            for (u64 b = a + 1; b < positions.size(); ++b)
            {
                cs64 dist_x{std::abs(positions[a].first - positions[b].first)};
                cs64 dist_y{std::abs(positions[a].second - positions[b].second)};
                largest_rect_area = std::max(largest_rect_area, (dist_x + 1) * (dist_y + 1));
            }

        return largest_rect_area;
    }

    static s64 part2 (const red_positions& in)
    {
        return 0;
    }

    static constexpr pair<s64, s64> expected ()
    {
        return {4735222687, 0};
    }
};

} // namespace day09
