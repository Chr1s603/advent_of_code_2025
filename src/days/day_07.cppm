export module aoc.day07;

import std;
import util.types;
import util.parse;
import util.enumerable;

export namespace day07 {

struct Day07
{
    static constexpr s64 number = 7;
    static constexpr sv  name{"Day 07: Laboratories"};

    static constexpr s64 no_cache{-1};
    enum class type
    {
        beam_start,
        beam,
        empty,
        splitter
    };
    using tachyon_manifold = vec<vec<type>>;

    static tachyon_manifold parse (const sv in)
    {
        const auto tachyon_rows{util::parse::split(in, "\n")};

        tachyon_manifold result;
        result.reserve(tachyon_rows.size());
        for (const auto& row : tachyon_rows)
        {
            vec<type> parsed_row;
            parsed_row.reserve(row.size());
            for (const auto& c : row)
                if (c == 'S')
                    parsed_row.emplace_back(type::beam_start);
                else if (c == '^')
                    parsed_row.emplace_back(type::splitter);
                else
                    parsed_row.emplace_back(type::empty);
            result.emplace_back(std::move(parsed_row));
        }

        return result;
    }

    static s64 count_timelines_memo (const tachyon_manifold& map,
                                     cu64                    row,
                                     cu64                    col,
                                     vec<vec<s64>>&          timeline_cache)
    {
        if (row >= map.size())
            return 1; // We are at the bottom - no further timeline split here

        if (col < 0 || col >= map[row].size())
            return 0; // We are out of the map - no further timeline split here

        if (timeline_cache[row][col] != -1)
            return timeline_cache[row][col];

        s64 result = 0;
        if (map[row][col] == type::splitter) // Timeline split here
            result = count_timelines_memo(map, row + 1, col - 1, timeline_cache)
                     + count_timelines_memo(map, row + 1, col + 1, timeline_cache);
        else
            result = count_timelines_memo(map, row + 1, col, timeline_cache);

        timeline_cache[row][col] = result;
        return result;
    }

    static s64 part1 (const tachyon_manifold& map)
    {
        vec<u64> beams_for_current_row;
        beams_for_current_row.reserve(map[0].size());

        s64 total_split_count{0};
        for (u64 row = 0; row < map.size(); row++)
        {
            vec<u64> beams_for_next_row;
            beams_for_next_row.reserve(map[0].size());

            for (u64 col = 0; col < map[row].size(); col++)
            {
                if (map[row][col] == type::beam_start)
                    beams_for_next_row.emplace_back(col);
                if (util::enumerable::contains(beams_for_current_row, col))
                {
                    if (map[row][col] == type::splitter)
                    {
                        total_split_count++;
                        if (col > 0)
                            beams_for_next_row.emplace_back(col - 1);
                        if (col < map[row].size() - 1)
                            beams_for_next_row.emplace_back(col + 1);
                    }
                    else
                        beams_for_next_row.emplace_back(col);
                }
            }

            std::swap(beams_for_current_row, beams_for_next_row);
        }

        return total_split_count;
    }

    static s64 part2 (const tachyon_manifold& map)
    {
        cu64 rows{map.size()};
        cu64 cols{map[0].size()};

        vec<vec<s64>> timeline_cache(rows, vec<s64>(cols, no_cache));
        for (u64 col = 0; col < cols; col++)
            if (map[0][col] == type::beam_start) // beam starts *below* the S cell
                return count_timelines_memo(map, 1, col, timeline_cache);

        return 0;
    }

    static constexpr pair<s64, s64> expected ()
    {
        return {1598, 4509723641302};
    }
};

} // namespace day07
