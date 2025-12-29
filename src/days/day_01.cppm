export module aoc.day01;

import std;
import util.enumerable;
import util.parse;
import util.types;
import aoc.day_common;

export namespace day {

template <>
struct Day<1>
{
    static constexpr sv name{"Day 01: Secret Entrance"};

    enum class direction
    {
        left,
        right
    };
    static direction parse (const char c)
    {
        if (c == 'L')
            return direction::left;
        return direction::right;
    }

    using rotation = pair<direction, s64>;
    using parsed   = vec<rotation>;

    static parsed parse (sv input)
    {
        const auto input_lines{util::parse::split(input, "\n")};

        parsed result;
        result.reserve(input_lines.size());
        for (const auto& line : input_lines)
        {
            if (line.size() < 2)
                continue;

            const auto direction{parse(line[0])};

            s64 ticks{};
            std::from_chars(line.data() + 1, line.data() + line.size(), ticks);

            result.push_back(rotation{direction, ticks});
        }

        return result;
    }

    static s64 part1 (const parsed& sequence_of_rotations)
    {
        s64 password{0};
        s64 dial_pos{50};

        for (auto [dir, dist] : sequence_of_rotations)
        {
            if (dir == direction::left)
                dial_pos += dist;
            else
                dial_pos -= dist;

            // Clamp the rotation to [0,99]. Loop needed as dist can be > 100!
            while (dial_pos < 0)
                dial_pos += 100;
            while (dial_pos > 99)
                dial_pos -= 100;

            if (dial_pos == 0) // Position after this rotation is 0
                password++;
        }

        return password;
    }

    static s64 part2 (const parsed& sequence_of_rotations)
    {
        s64 password_method_0x434C49434B{0};
        s64 dial_pos = 50;

        for (auto [dir, dist] : sequence_of_rotations)
        {
            for (s64 i = 0; i < dist; i++)
            {
                if (dir == direction::left)
                    dial_pos = (dial_pos + 99) % 100;
                else
                    dial_pos = (dial_pos + 1) % 100;

                if (dial_pos == 0) // Position during execution of this rotation is 0
                    password_method_0x434C49434B++;
            }
        }

        return password_method_0x434C49434B;
    }

    static constexpr pair<s64, s64> expected ()
    {
        return {1078, 6412};
    }
};

}
