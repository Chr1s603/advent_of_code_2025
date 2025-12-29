export module aoc.day03;

import std;
import util.parse;
import util.types;
import aoc.day_common;

export namespace day {

template <>
struct Day<3>
{
    static constexpr sv name{"Day 03: Lobby"};

    using bank   = vec<s64>;
    using parsed = vec<bank>;

    static parsed parse (const sv in)
    {
        const auto all_banks{util::parse::split(in, "\n")};

        parsed out;
        out.reserve(all_banks.size());

        for (const auto current_bank : all_banks)
        {
            bank bank;
            bank.reserve(current_bank.size());

            for (auto voltage : current_bank)
                bank.emplace_back(util::parse::to_number(voltage));
            out.emplace_back(bank);
        }

        return out;
    }

    static s64
    get_best_idx_for_bank (const bank& b, cs64 battery_idx, cs64 battery_count, cs64 start_idx)
    {
        cs64 elements_needed_for_lower_orders{battery_count - battery_idx};
        cs64 elements_to_take{static_cast<s64>(b.size()) - elements_needed_for_lower_orders};

        auto it = std::max_element(b.begin() + start_idx, b.begin() + elements_to_take);
        return std::distance(b.begin(), it);
    }

    static s64 get_max_joltage_of_bank (const bank& b, cs64 max_batteries_per_bank)
    {
        s64 max_joltage{0};
        s64 place_value{1};

        for (s64 i = 0; i < max_batteries_per_bank; ++i)
            place_value *= 10; // NOTE: Avoids nasty casts w std::pow which is double only

        s64 last_orders_idx{0};
        for (s64 order = 0; order <= max_batteries_per_bank; ++order)
        {
            using index_t = bank::size_type;

            index_t best_idx_of_this_order{static_cast<index_t>(
                get_best_idx_for_bank(b, order, max_batteries_per_bank, last_orders_idx))};

            last_orders_idx = static_cast<s64>(best_idx_of_this_order) + 1;

            max_joltage += b[best_idx_of_this_order] * place_value;
            place_value /= 10;
        }

        return max_joltage;
    }

    static s64 get_max_joltage (const parsed& banks, cs64 max_batteries_per_bank)
    {
        vec<std::future<s64>> tasks;
        tasks.reserve(banks.size());

        for (const auto& current_bank : banks)
        {
            tasks.emplace_back(
                std::async(std::launch::async, [&current_bank, max_batteries_per_bank] () {
                    return get_max_joltage_of_bank(current_bank, max_batteries_per_bank);
                }));
        }

        s64 total_output_joltage{0};
        for (auto& f : tasks)
            total_output_joltage += f.get();

        return total_output_joltage;
    }

    static s64 part1 (const parsed& in)
    {
        return get_max_joltage(in, 1);
    }

    static s64 part2 (const parsed& in)
    {
        return get_max_joltage(in, 11);
    }

    static constexpr pair<s64, s64> expected ()
    {
        return {17031, 168575096286051};
    }
};

}
