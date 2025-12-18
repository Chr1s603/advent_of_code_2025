export module aoc.day08;

import std;
import util.types;
import util.parse;
import util.enumerable;

export namespace day08 {

struct Day08
{
    static constexpr s64 number = 8;
    static constexpr sv  name{"Day 08: Playground"};

    using position         = std::tuple<s64, s64, s64>;
    using junction_box_map = vec<position>;

    static junction_box_map parse (const sv in)
    {
        const auto position_strings{util::parse::split(in, "\n")};

        junction_box_map result;
        result.reserve(position_strings.size());

        for (const auto& position : position_strings)
        {
            const auto xyz_strings{util::parse::split(position, ",")};
            result.emplace_back(util::parse::to_number(xyz_strings[0]),
                                util::parse::to_number(xyz_strings[1]),
                                util::parse::to_number(xyz_strings[2]));
        }

        return result;
    }

    static double euclidean_distance (const position& a, const position& b)
    {
        using namespace util::enumerable;
        return std::hypot(x(a) - x(b), y(a) - y(b), z(a) - z(b));
    }

    static std::list<pair<double, pair<position, position>>>
    get_sorted_abs_distances (const junction_box_map& map)
    {
        using result_entry = pair<double, pair<position, position>>;
        list<result_entry> result;

        for (u64 a = 0; a < map.size(); ++a)
            for (u64 b = a + 1; b < map.size(); ++b)
                result.emplace_back(euclidean_distance(map[a], map[b]),
                                    pair<position, position>{map[a], map[b]});

        result.sort(
            [] (const result_entry& a, const result_entry& b) { return a.first < b.first; });

        return result;
    }

    static void connect_box_pair_to_circuits (const pair<position, position>& box_pair,
                                              list<list<position>>&           circuits)
    {
        const position a = box_pair.first;
        const position b = box_pair.second;

        auto a_occurence = circuits.end();
        auto b_occurence = circuits.end();
        for (auto circuit_it = circuits.begin(); circuit_it != circuits.end(); ++circuit_it)
        {
            if (std::find(circuit_it->begin(), circuit_it->end(), a) != circuit_it->end())
                a_occurence = circuit_it;
            if (std::find(circuit_it->begin(), circuit_it->end(), b) != circuit_it->end())
                b_occurence = circuit_it;
        }

        if (a_occurence != circuits.end() && b_occurence != circuits.end())
        {
            if (a_occurence != b_occurence) // different circuits -> merge
            {
                a_occurence->splice(a_occurence->end(), *b_occurence);
                circuits.erase(b_occurence);
            }
        }
        else if (a_occurence != circuits.end()) // only a found
            a_occurence->emplace_back(b);
        else if (b_occurence != circuits.end()) // only b found
            b_occurence->emplace_back(a);
        else // neither found
            circuits.emplace_back(list{a, b});
    }

    static s64 part1 (const junction_box_map& map)
    {
        auto sorted_abs_distances{get_sorted_abs_distances(map)};

        list<list<position>> circuits;
        for (u64 i = 0; i < 1000; i++)
        {
            auto closest_boxes{*sorted_abs_distances.begin()};
            connect_box_pair_to_circuits(closest_boxes.second, circuits);
            sorted_abs_distances.erase(sorted_abs_distances.begin());
        }

        s64 three_largest_circuit_sizes_multiplied{1};
        circuits.sort(
            [] (const list<position>& a, const list<position>& b) { return a.size() > b.size(); });
        for (s64 i = 0; i < 3; i++)
            three_largest_circuit_sizes_multiplied *= (std::next(circuits.begin(), i))->size();

        return three_largest_circuit_sizes_multiplied;
    }

    static s64 part2 (const junction_box_map& map)
    {
        auto sorted_abs_distances{get_sorted_abs_distances(map)};

        list<list<position>>     circuits;
        pair<position, position> last_merge_pair;
        while (true)
        {
            auto pair_to_connect = sorted_abs_distances.front().second;
            sorted_abs_distances.pop_front();

            // Before connecting, track total boxes in all circuits
            u64 total_before = 0;
            for (auto& c : circuits)
                total_before += c.size();

            connect_box_pair_to_circuits(pair_to_connect, circuits);

            // After connecting, track total boxes
            u64 total_after = 0;
            for (auto& c : circuits)
                total_after += c.size();

            // Only update last_merge_pair if new boxes were connected or circuits merged
            if (total_after > total_before)
                last_merge_pair = pair_to_connect;

            if (total_after == map.size())
                break;
        }

        return util::enumerable::x(last_merge_pair.first)
               * util::enumerable::x(last_merge_pair.second);
    }

    static constexpr pair<s64, s64> expected ()
    {
        return {123420, 673096646};
    }
};

} // namespace day08
