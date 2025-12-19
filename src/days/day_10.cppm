export module aoc.day10;

import std;
import util.types;
import util.parallel;
import util.parse;

export namespace day10 {

struct Day10
{
    static constexpr s64 number = 10;
    static constexpr sv  name{"Day 10: Factory"};

    struct machine
    {
        str           indicator_lights_goal;
        vec<vec<s64>> button_wiring;
        vec<s64>      joltage_requirements;
    };
    using machines = vec<machine>;

    // Applies a single button press by toggling all affected lights
    static str get_lights_after_press (const sv& current_lights, const vec<s64>& button)
    {
        str result{current_lights};
        for (const auto& light_idx : button)
            result[light_idx] = result[light_idx] == '#' ? '.' : '#';
        return result;
    }

    static str parse_light_goal (const sv lights_str)
    {
        // Strips surrounding brackets from the light pattern
        return str(lights_str.substr(1, lights_str.size() - 2));
    }

    // Parses a list of integer tuples like "(0,2,3)" into vectors
    static vec<vec<s64>> parse_ints (const vec<sv> button_strs)
    {
        vec<vec<s64>> result;
        result.reserve(button_strs.size());
        for (const auto& num_strs : button_strs)
        {
            sv inner = num_strs.substr(1, num_strs.size() - 2); // Strips surrounding brackets

            vec<s64> b;
            if (!inner.empty())
            {
                const auto nums = util::parse::split(inner, ",");
                b.reserve(nums.size());

                for (const auto& n : nums)
                    b.emplace_back(util::parse::to_number(n));
            }

            result.emplace_back(std::move(b));
        }
        return result;
    }

    static machines parse (const sv in)
    {
        const auto machine_strings{util::parse::split(in, "\n")};

        machines result;
        result.reserve(machine_strings.size());

        for (const sv machine_string : machine_strings)
        {
            const auto components_strs{util::parse::split(machine_string, " ")};
            result.emplace_back(parse_light_goal(components_strs[0]),
                                parse_ints(vec<sv>(&components_strs[1],
                                                   &components_strs[components_strs.size() - 1])),
                                parse_ints({components_strs.back()})[0]);
        }

        return result;
    }

    // BFS over light states; buttons are edges, states are bitstrings
    static s64 get_fewest_press_combination (const machine& m)
    {
        const str  start(m.indicator_lights_goal.size(), '.');
        const str& target{m.indicator_lights_goal};

        std::queue<str>              frontier;
        std::unordered_map<str, s64> distances;

        frontier.push(start);
        distances[start] = 0;

        while (!frontier.empty())
        { // Visit bitstring in front of FIFO frontier
            const str cur{frontier.front()};
            frontier.pop();

            if (cur == target)
                return distances[cur];

            // Left to right BFS: Visit bitstrings next to cur
            for (const auto& button : m.button_wiring)
            {
                const str next{get_lights_after_press(cur, button)};
                if (!distances.contains(next))
                {
                    distances[next] = distances[cur] + 1;
                    frontier.push(next);
                }
            }
        }

        return -1; // Should never happen with valid input
    }

    static s64 part1 (const machines& machines)
    {
        return util::parallel::sum_async(machines, &Day10::get_fewest_press_combination);
    }

    // Recursive parity-based reduction:
    // Uses Part 1 logic to strip odd contributions, then divides the problem by 2
    // https://www.reddit.com/r/adventofcode/comments/1pk87hl/comment/ntp4njq/
    static s64 fewest_presses (const machine& m)
    {
        // Hash for joltage vectors to allow memoization
        using Key = vec<s64>;
        struct vec_hash
        {
            std::size_t operator()(const Key& v) const
            {
                std::size_t h = 0;
                for (auto x : v)
                    h = h * 31 + std::hash<s64>{}(x);
                return h;
            }
        };

        std::unordered_map<Key, s64, vec_hash> cache;

        // Enumerates all button subsets that reproduce a given parity pattern
        auto parity_patterns = [&] (const str& parity_lights) -> vec<vec<s64>> {
            vec<vec<s64>> patterns;
            const s64     n = m.button_wiring.size();

            // Each button is pressed at most once (mod 2)
            for (s64 mask = 0; mask < (1 << n); mask++)
            {
                str lights(m.joltage_requirements.size(), '.');
                for (s64 i = 0; i < n; i++)
                    if ((mask >> i) & 1)
                        lights = get_lights_after_press(lights, m.button_wiring[i]);
                if (lights == parity_lights)
                {
                    vec<s64> pressed;
                    for (s64 i = 0; i < n; i++)
                        if ((mask >> i) & 1)
                            pressed.push_back(i);
                    patterns.push_back(pressed);
                }
            }
            return patterns;
        };

        std::function<s64(Key)> dfs = [&] (Key jolts) -> s64 {
            if (cache.contains(jolts))
                return cache[jolts];

            // Base case: all counters are zero
            bool all_zero = true;
            for (s64 j : jolts)
                if (j != 0)
                {
                    all_zero = false;
                    break;
                }
            if (all_zero)
                return 0;

            // Convert target joltages to a parity pattern
            str parity;
            for (s64 j : jolts)
                parity += (j % 2) ? '#' : '.';

            s64  best     = std::numeric_limits<s64>::max();
            auto patterns = parity_patterns(parity);

            for (const auto& pressed : patterns)
            {
                Key  new_jolts = jolts;
                bool valid     = true;

                // Remove the odd (parity) contribution
                for (s64 i : pressed)
                {
                    for (s64 idx : m.button_wiring[i])
                    {
                        // Defensive check against malformed input
                        if (idx < 0 || idx >= s64(new_jolts.size()))
                        {
                            valid = false;
                            break;
                        }
                        new_jolts[idx]--;
                    }
                    if (!valid)
                        break;
                }

                if (!valid)
                    continue;

                // After removing parity, all values must be even to allow division by 2
                for (s64& x : new_jolts)
                {
                    if (x < 0 || x % 2 != 0)
                    {
                        valid = false;
                        break;
                    }
                    x /= 2;
                }
                if (!valid)
                    continue;

                s64 cost = dfs(new_jolts);
                if (cost != std::numeric_limits<s64>::max())
                    best = std::min(best, s64(2 * cost + pressed.size()));
            }

            return cache[jolts] = best;
        };

        return dfs(m.joltage_requirements);
    }

    static s64 part2 (const machines& machines)
    {
        return util::parallel::sum_async(machines, &Day10::fewest_presses);
    }

    static constexpr pair<s64, s64> expected ()
    {
        return {417, 16765};
    }
};

} // namespace day10
