export module aoc.day04;

import std;
import util.parse;
import util.types;

export namespace day04 {

struct Day04
{
    static constexpr s64 number = 4;
    static constexpr sv  name{"Day 04: Printing Department"};

    enum class type
    {
        paper,
        none
    };
    using parsed = vec<vec<type>>;

    static parsed parse (const sv in)
    {
        auto get_type = [] (const char c) {
            if (c == '.')
                return type::none;
            return type::paper;
        };

        parsed result;

        const auto rows{util::parse::split(in, "\n")};
        result.reserve(rows.size());

        for (u64 y = 0; y < rows.size(); y++)
        {
            const auto input_row{rows[y]};
            vec<type>  row;
            row.reserve(input_row.size());
            for (u64 x = 0; x < input_row.size(); x++)
                row.push_back(get_type(input_row[x]));
            result.emplace_back(row);
        }

        return result;
    }

    static type get_type (const parsed& map, cs64 x, cs64 y)
    {
        cs64 height{static_cast<s64>(map.size())};
        cs64 width{static_cast<s64>(map[0].size())};
        if (x < 0 || x >= width || y < 0 || y >= height)
            return type::none;
        return map[y][x];
    }

    static void set_type (parsed& map, cs64 x, cs64 y, const type t)
    {
        map[y][x] = t;
    }

    static u64 get_sourrounding_paper_count (const parsed& map, cs64 x, cs64 y)
    {
        u64 paper_count{0};
        for (s64 offset_x = -1; offset_x <= 1; offset_x++)
            for (s64 offset_y = -1; offset_y <= 1; offset_y++)
                if (!(offset_x == 0 && offset_y == 0))
                    paper_count += get_type(map, x + offset_x, y + offset_y) == type::paper;

        return paper_count;
    }

    static s64 try_remove_papers (parsed& map_to_change)
    {
        const parsed original{map_to_change};
        cs64         height{static_cast<s64>(map_to_change.size())};
        cs64         width{static_cast<s64>(map_to_change[0].size())};

        s64 removed_papers{0};
        for (s64 x = 0; x < width; x++)
            for (s64 y = 0; y < height; y++)
                if (get_type(original, x, y) == type::paper)
                    if (get_sourrounding_paper_count(original, x, y) < 4)
                    {
                        removed_papers++;
                        set_type(map_to_change, x, y, type::none);
                    }

        return removed_papers;
    }

    static s64 part1 (const parsed& input_map)
    {
        auto local_copy{input_map};
        return try_remove_papers(local_copy);
    }

    static s64 part2 (const parsed& input_map)
    {
        auto local_copy{input_map};
        s64  removed_papers{0};
        for (;;)
        {
            cs64 removed = try_remove_papers(local_copy);
            if (removed == 0)
                break;
            removed_papers += removed;
        }

        return removed_papers;
    }

    static constexpr pair<s64, s64> expected ()
    {
        return {1428, 8936};
    }
};

} // namespace day04
