export module aoc.day03;

import std;
import util.parse;
import util.types;

export namespace day03 {

struct Day03
{
    static constexpr s64 number = 3;
    static constexpr sv  name{"Day 03: Lobby"};

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

    static u64
    get_best_idx_for_bank (const bank& b, cu64 battery_idx, cu64 battery_count, cu64 start_idx)
    {
        const u64 elements_needed_for_lower_orders = battery_count - battery_idx;
        const u64 elements_to_take                 = b.size() - elements_needed_for_lower_orders;

        auto it = std::max_element(b.begin() + start_idx, b.begin() + elements_to_take);
        return std::distance(b.begin(), it);
    }

    static s64 get_max_joltage_of_bank (const bank& b, cu64 max_batteries_per_bank)
    {
        s64 max_joltage{0};
        u64 last_orders_idx{0};
        for (u64 order = 0; order <= max_batteries_per_bank; order++)
        {
            cu64 best_idx_of_this_order{
                get_best_idx_for_bank(b, order, max_batteries_per_bank, last_orders_idx)};
            last_orders_idx = best_idx_of_this_order + 1;
            max_joltage += b[best_idx_of_this_order] * std::pow(10, max_batteries_per_bank - order);
        }

        return max_joltage;
    }

    static s64 get_max_joltage (const parsed& banks, cu64 max_batteries_per_bank)
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

} // namespace day03
