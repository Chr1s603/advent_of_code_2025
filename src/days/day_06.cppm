export module aoc.day06;

import std;
import util.types;
import util.parse;
import aoc.day_common;

export namespace day {

template <>
struct Day<6>
{
    static constexpr sv name{"Day 06: Trash Compactor"};

    enum class op
    {
        add,
        mult,
    };
    struct equation
    {
        op       op;
        vec<str> number_strings;
    };
    using equation_list = vec<equation>;

    static pair<vec<u64>, vec<op>> obtain_chunk_widths_and_ops (const sv last_line)
    {
        vec<op>  ops;
        vec<u64> chunk_widths;
        ops.reserve(last_line.size());          // > actual size
        chunk_widths.reserve(last_line.size()); // > actual size

        u64 prev_pos{1};
        ops.emplace_back((last_line[0] == '*') ? op::mult : op::add);
        for (u64 idx = 1; idx < static_cast<u64>(last_line.size()); idx++)
        {
            char c = last_line[idx];
            if (c == '*' || c == '+')
            {
                u64 chunk_len{idx - prev_pos};
                chunk_widths.emplace_back(chunk_len);
                ops.emplace_back((c == '*') ? op::mult : op::add);
                prev_pos = idx + 1;
            }
        }

        cu64 final_chunk = static_cast<u64>(last_line.size()) - prev_pos;
        chunk_widths.push_back(final_chunk + 1);

        return {std::move(chunk_widths), std::move(ops)};
    }

    static equation_list parse (sv in)
    {
        const vec<sv> lines{util::parse::split(in, "\n")};
        const auto [chunk_widths, ops]{obtain_chunk_widths_and_ops(lines.back())};

        equation_list result(chunk_widths.size());
        for (u64 line_idx = 0; line_idx < lines.size() - 1; line_idx++)
        {
            u64       pos  = 0;
            const sv& line = lines[line_idx];

            for (u64 chunk_idx = 0; chunk_idx < chunk_widths.size(); chunk_idx++)
            {
                result[chunk_idx].op = ops[chunk_idx];

                u64 len = chunk_widths[chunk_idx];
                result[chunk_idx].number_strings.emplace_back(line.substr(pos, len));

                pos += chunk_widths[chunk_idx] + 1; // move to next chunk
            }
        }

        return result;
    }

    static s64 part1 (const equation_list& equations)
    {
        s64 sum_of_answers{0};
        for (const auto& e : equations)
        {
            auto nums  = e.number_strings | std::views::transform([] (const str& s) {
                            auto pos = s.find_first_not_of(' ');
                            return pos == str::npos ? 0ll : util::parse::to_number(s.substr(pos));
                        });
            auto op_fn = [&] (auto a, auto b) { return e.op == op::add ? a + b : a * b; };
            sum_of_answers
                += std::reduce(nums.begin(), nums.end(), e.op == op::add ? 0ll : 1ll, op_fn);
        }

        return sum_of_answers;
    }

    static s64 part2 (const equation_list& equations)
    {
        s64 sum_of_answers = 0;
        for (u64 eq_idx = 0; eq_idx < equations.size(); eq_idx++)
        {
            const auto& e{equations[eq_idx]};
            s64         line_result{(e.op == op::add) ? 0 : 1};

            cu64 num_digits{e.number_strings[0].size()};
            cu64 num_lines{e.number_strings.size()};

            for (u64 digit = 0; digit < num_digits; ++digit)
            {
                str num_str;
                for (u64 line = 0; line < num_lines; ++line)
                    if (char c = e.number_strings[line][digit]; c != ' ')
                        num_str += c;

                cs64 complete_number = num_str.empty() ? 0 : util::parse::to_number(num_str);
                if (e.op == op::add)
                    line_result += complete_number;
                else
                    line_result *= complete_number;
            }

            sum_of_answers += line_result;
        }

        return sum_of_answers;
    }

    static constexpr pair<s64, s64> expected ()
    {
        return {5873191732773, 11386445308378};
    }
};

}
