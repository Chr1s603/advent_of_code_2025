export module aoc.day06;

import std;
import util.types;
import util.parse;

export namespace day06 {

struct Day06
{
    static constexpr s64 number = 6;
    static constexpr sv  name{"Day 06: Trash Compactor"};

    enum class op
    {
        add,
        mult,
    };
    struct equation
    {
        op       op;
        vec<s64> numbers;
    };
    using equation_list = vec<equation>;

    static equation_list parse (sv in)
    {
        const auto lines{util::parse::split(in, "\n")};

        equation_list result;
        result.resize(util::parse::split(lines[0], " ").size());

        for (u64 line_idx = 0; line_idx < lines.size() - 1; line_idx++)
        {
            const auto line{lines[line_idx]};
            const auto numbers{util::parse::split(line, " ")};
            u64        actual_number{0};
            for (u64 num_idx = 0; num_idx < numbers.size(); num_idx++)
                if (numbers[num_idx].size() >= 1)
                    result[actual_number++].numbers.emplace_back(
                        util::parse::to_number(numbers[num_idx]));
        }

        const auto last_line{lines[lines.size() - 1]};
        const auto ops{util::parse::split(last_line, " ")};
        u64        actual_op{0};
        for (u64 op_idx = 0; op_idx < ops.size(); op_idx++)
            if (ops[op_idx].size() >= 1)
                result[actual_op++].op = ops[op_idx] == "*" ? op::mult : op::add;

        return result;
    }

    static s64 part1 (const equation_list& equations)
    {
        s64 sum_of_answers{0};
        for (const auto& e : equations)
        {
            auto op_fn = [&] (auto a, auto b) { return e.op == op::add ? a + b : a * b; };
            sum_of_answers += std::reduce(
                e.numbers.cbegin(), e.numbers.cend(), e.op == op::add ? 0ll : 1ll, op_fn);
        }

        return sum_of_answers;
    }

    static s64 part2 (const equation_list& in)
    {
        return 0;
    }

    static constexpr pair<s64, s64> expected ()
    {
        return {5873191732773, 0};
    }
};

} // namespace day06
