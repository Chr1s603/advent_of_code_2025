export module aoc.day05;

import std;
import util.parse;
import util.types;
import aoc.day_common;

export namespace day {

template <>
struct Day<5>
{
    static constexpr sv name{"Day 05: Cafeteria"};

    using range = pair<s64, s64>;
    struct database
    {
        vec<range> fresh_ingredient_id_ranges;
        vec<s64>   avail_ingredient_ids;
    };

    static void merge_range (vec<range>& db, range r)
    {
        vec<u64> colliding;
        for (u64 i = 0; i < db.size(); ++i)
            if (const auto& entry = db[i]; !(entry.second < r.first || entry.first > r.second))
                colliding.push_back(i);

        for (cu64 idx : colliding)
            r = {std::min(r.first, db[idx].first), std::max(r.second, db[idx].second)};

        for (u64 i = colliding.size(); i-- > 0;)
            db.erase(db.begin() + static_cast<s64>(colliding[i]));

        db.emplace_back(r);
    }

    static database parse (sv in)
    {
        const auto fresh_ranges_and_avail{util::parse::split(in, "\n\n")};
        const auto fresh_ranges{util::parse::split(fresh_ranges_and_avail[0], "\n")};
        const auto avail{util::parse::split(fresh_ranges_and_avail[1], "\n")};

        database result;

        result.fresh_ingredient_id_ranges.reserve(fresh_ranges.size());
        for (auto& range : fresh_ranges)
        {
            const auto begin_and_end{util::parse::split(range, "-")};
            cs64       begin{util::parse::to_number(begin_and_end[0])};
            cs64       end{util::parse::to_number(begin_and_end[1])};
            merge_range(result.fresh_ingredient_id_ranges, pair<s64, s64>{begin, end});
        }

        result.avail_ingredient_ids.reserve(avail.size());
        for (auto& id_str : avail)
        {
            cs64 id{util::parse::to_number(id_str)};
            result.avail_ingredient_ids.emplace_back(id);
        }

        return result;
    }

    static bool is_fresh (const vec<range>& fresh_ranges, cs64 id)
    {
        for (const auto range : fresh_ranges)
            if (id >= range.first && id <= range.second)
                return true;
        return false;
    }

    static s64 part1 (const database& in)
    {
        s64 avail_fresh_ingredients{0};
        for (const auto id : in.avail_ingredient_ids)
            avail_fresh_ingredients += is_fresh(in.fresh_ingredient_id_ranges, id);

        return avail_fresh_ingredients;
    }

    static s64 part2 (const database& in)
    {
        s64 fresh_ingredients{0};
        for (const auto range : in.fresh_ingredient_id_ranges)
            fresh_ingredients += range.second - range.first + 1;

        return fresh_ingredients;
    }

    static constexpr pair<s64, s64> expected ()
    {
        return {758, 343143696885053};
    }
};

}
