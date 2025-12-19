export module aoc.day02;

import std;
import util.types;
import util.parse;
import util.parallel;

export namespace day02 {

struct Day02
{
    static constexpr s64 number = 2;
    static constexpr sv  name{"Day 02: Gift Shop"};

    using id_range = pair<s64, s64>;
    using parsed   = vec<id_range>;

    static parsed parse (const sv in)
    {
        parsed out;
        for (const auto id_range : util::parse::split(in, ","))
        {
            const auto begin_and_end
                = util::parse::split(id_range, "-") | util::parse::to_numbers();
            out.emplace_back(begin_and_end[0], begin_and_end[1]);
        }

        return out;
    }

    static s64 sum_of_invalid_ids_from_range (const pair<s64, s64> id_range)
    {
        s64 sum_of_invalid_ids{0};
        for (s64 id = id_range.first; id <= id_range.second; id++)
        {
            const auto id_str{std::to_string(id)};
            if (id_str.size() % 2)
                continue;

            const auto first_half{id_str.substr(0, id_str.size() / 2)};
            const auto second_half{id_str.substr(id_str.size() / 2, id_str.size())};
            sum_of_invalid_ids += (first_half == second_half) * id;
        }
        return sum_of_invalid_ids;
    }

    static bool does_string_only_consist_of_sequence (const sv string, const sv sequence)
    {
        const u64 seq_len{sequence.size()};
        if (string.size() % seq_len)
            return false;

        for (u64 pos = seq_len; pos <= string.size() - seq_len; pos += seq_len)
            if (string.substr(0, seq_len) != string.substr(pos, seq_len))
                return false;

        return true;
    }

    static bool is_really_invalid (const s64 id)
    {
        const auto id_str{std::to_string(id)};
        for (u64 seq_end = 1; seq_end < id_str.size() / 2 + 1; seq_end++)
            if (does_string_only_consist_of_sequence(id_str, id_str.substr(0, seq_end)))
                return true;
        return false;
    }

    static s64 sum_of_really_invalid_ids_from_range (const pair<s64, s64> id_range)
    {
        s64 sum_of_invalid_ids{0};
        for (s64 id = id_range.first; id <= id_range.second; id++)
            sum_of_invalid_ids += is_really_invalid(id) * id;

        return sum_of_invalid_ids;
    }

    static s64 part1 (const parsed& list_of_id_ranges)
    {
        return util::parallel::sum_async(list_of_id_ranges, &Day02::sum_of_invalid_ids_from_range);
    }

    static s64 part2 (const parsed& list_of_id_ranges)
    {
        return util::parallel::sum_async(list_of_id_ranges,
                                         &Day02::sum_of_really_invalid_ids_from_range);
    }

    static constexpr pair<s64, s64> expected ()
    {
        return {30323879646, 43872163557};
    }
};
} // namespace day02
