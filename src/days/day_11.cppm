export module aoc.day11;

import std;
import util.types;
import util.parse;
import util.enumerable;
import aoc.day_common;

export namespace day {

template <>
struct Day<11>
{
    static constexpr sv name{"Day 11: Reactor"};

    struct device
    {
        sv      id;
        vec<sv> outs;
    };
    using device_list = std::unordered_map<sv, vec<sv>>;
    using parsed      = device_list;

    static parsed parse (const sv in)
    {
        const auto device_strings{util::parse::split(in, "\n")};

        device_list result;
        result.reserve(device_strings.size());

        for (const auto& device_string : device_strings)
        {
            const auto device_and_output_strings{util::parse::split(device_string, ": ")};
            const auto device_id{device_and_output_strings[0]};
            const auto out_strs{util::parse::split(device_and_output_strings[1], " ")};
            result.emplace(device_id, out_strs);
        }

        return result;
    }

    struct cache_key
    {
        sv   node;
        bool visited_fft;
        bool visited_dac;

        bool operator==(const cache_key& other) const noexcept
        {
            return node == other.node && visited_fft == other.visited_fft
                   && visited_dac == other.visited_dac;
        }
    };

    struct cache_key_hash
    {
        u64 operator()(const cache_key& k) const noexcept
        {
            u64 h{std::hash<sv>{}(k.node)};
            h = h * 31 + std::hash<bool>{}(k.visited_fft);
            h = h * 31 + std::hash<bool>{}(k.visited_dac);
            return h;
        }
    };

    static s64
    get_total_path_count_from_server (const sv      node,
                                      const parsed& nodes,
                                      const bool    visited_fft,
                                      const bool    visited_dac,
                                      std::unordered_map<cache_key, s64, cache_key_hash>& cache)
    {
        const str target{"out"};
        if (node == target)
            return (visited_dac && visited_fft);

        bool is_fft_node{node == "fft"};
        bool is_dac_node{node == "dac"};

        s64 paths_of_subnodes{0};
        for (const auto& next_node : nodes.at(node))
        {
            cache_key key{next_node, visited_fft || is_fft_node, visited_dac || is_dac_node};

            if (!cache.contains(key))
            {
                cache.emplace(key,
                              get_total_path_count_from_server(
                                  next_node, nodes, key.visited_fft, key.visited_dac, cache));
            }

            paths_of_subnodes += cache.at(key);
        }

        return paths_of_subnodes;
    }

    static s64 part1 (const parsed& in)
    {
        std::unordered_map<cache_key, s64, cache_key_hash> cache;
        return get_total_path_count_from_server("you", in, true, true, cache);
    }

    static s64 part2 (const parsed& in)
    {
        std::unordered_map<cache_key, s64, cache_key_hash> cache;
        return get_total_path_count_from_server("svr", in, false, false, cache);
    }

    static constexpr pair<s64, s64> expected ()
    {
        return {753, 450854305019580};
    }
};

}
