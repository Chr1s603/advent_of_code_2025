export module aoc.day09;

import std;
import util.types;
import util.parse;
import util.enumerable;

using namespace util::enumerable;

export namespace day09 {

struct Day09
{
    static constexpr s64 number = 9;
    static constexpr sv  name{"Day 09: Movie Theater"};

    using position      = pair<s64, s64>;
    using red_positions = vec<position>;
    using row_span      = pair<s64, s64>;
    using row_spans     = std::map<s64, vec<row_span>>;

    static red_positions parse (const sv in)
    {
        const auto positions_string{util::parse::split(in, "\n")};

        red_positions result;
        for (const auto& position_str : positions_string)
        {
            const auto x_y_str{util::parse::split(position_str, ",")};
            result.emplace_back(util::parse::to_number(x_y_str[0]),
                                util::parse::to_number(x_y_str[1]));
        }

        return result;
    }

    static s64 part1 (const red_positions& red_positions)
    {
        s64 largest_rect_area{0};
        for (u64 a = 0; a < red_positions.size(); ++a)
            for (u64 b = a + 1; b < red_positions.size(); ++b)
            {
                cs64 dist_x{std::abs(red_positions[a].first - red_positions[b].first)};
                cs64 dist_y{std::abs(red_positions[a].second - red_positions[b].second)};
                largest_rect_area = std::max(largest_rect_area, (dist_x + 1) * (dist_y + 1));
            }

        return largest_rect_area;
    }

    static row_spans get_green_spans (const red_positions& red_positions)
    {
        row_spans result;
        cu64      red_pos_count{red_positions.size()};

        // Add edges of the polygon to the result
        for (u64 i = 0; i < red_pos_count; ++i)
        {
            const auto& a{red_positions[i]};
            const auto& b{red_positions[(i + 1) % red_pos_count]}; // Wrap

            if (x(a) == x(b)) // Vertical line
            {
                cs64 y_start{std::min(y(a), y(b))};
                cs64 y_end{std::max(y(a), y(b))};
                for (s64 y = y_start; y <= y_end; ++y)
                    result[y].push_back({x(a), x(a)}); // Single-column span
            }
            else if (y(a) == y(b)) // horizontal line
            {
                cs64 x_start{std::min(x(a), x(b))};
                cs64 x_end{std::max(x(a), x(b))};
                result[y(a)].push_back({x_start, x_end}); // Horizontal span
            }
        }

        // Determine vertical range of the polygon
        s64 min_y{y(red_positions.front())};
        s64 max_y{y(red_positions.front())};
        for (auto [x, y] : red_positions)
        {
            min_y = std::min(min_y, y);
            max_y = std::max(max_y, y);
        }

        // Fill interior spans using scanline method
        for (s64 y_idx = min_y + 1; y_idx <= max_y - 1; ++y_idx)
        {
            vec<s64> intersections;

            // Find intersection points with polygon edges for current row
            for (u64 i = 0; i < red_pos_count; ++i)
            {
                const auto& a{red_positions[i]};
                const auto& b{red_positions[(i + 1) % red_pos_count]};
                if (y(a) == y(b)) // Skip horizontal edges
                    continue;

                cs64 y_start{std::min(y(a), y(b))};
                cs64 y_end{std::max(y(a), y(b))};
                if (y_start < y_idx && y_idx <= y_end)
                    intersections.push_back(a.first);
            }

            if (intersections.size() < 2)
                continue;

            std::sort(intersections.begin(), intersections.end());

            // Fill spans between pairs of intersections
            for (u64 i = 0; i + 1 < intersections.size(); i += 2)
                result[y_idx].push_back({intersections[i] + 1, intersections[i + 1] - 1});
        }

        return result;
    }

    static bool in_green_spans (s64 x, s64 y, const row_spans& spans)
    {
        const auto it{spans.find(y)};
        if (it == spans.end())
            return false;

        for (const auto& [x_start, x_end] : it->second)
            if (x >= x_start && x <= x_end)
                return true;

        return false;
    }

    static bool all_edges_green (
        const row_spans& green_spans, cs64 x_start, cs64 x_end, cs64 y_start, cs64 y_end)
    {
        bool all_green = true;

        // Top & bottom edges
        for (s64 x = x_start; x <= x_end && all_green; x++)
        {
            if (!in_green_spans(x, y_start, green_spans))
                all_green = false;
            if (!in_green_spans(x, y_end, green_spans))
                all_green = false;
        }

        // Left & right edges
        for (s64 y = y_start + 1; y <= y_end - 1 && all_green; y++)
        {
            if (!in_green_spans(x_start, y, green_spans))
                all_green = false;
            if (!in_green_spans(x_end, y, green_spans))
                all_green = false;
        }

        return all_green;
    }

    static s64 part2 (const red_positions& red_positions)
    {
        const auto green_rects{get_green_spans(red_positions)};
        auto       worker = [&] (u64 start_idx, u64 end_idx) -> s64 {
            s64 local_max_rect_area{0};
            for (u64 a_idx = start_idx; a_idx < end_idx; ++a_idx)
                for (u64 b_idx = a_idx + 1; b_idx < red_positions.size(); ++b_idx)
                {
                    const auto a{red_positions[a_idx]};
                    const auto b{red_positions[b_idx]};

                    cs64 x_start{std::min(x(a), x(b))};
                    cs64 x_end{std::max(x(a), x(b))};
                    cs64 y_start{std::min(y(a), y(b))};
                    cs64 y_end{std::max(y(a), y(b))};

                    cs64 area{(x_end - x_start + 1) * (y_end - y_start + 1)};
                    if (area < local_max_rect_area) // Skip early BEFORE search
                        continue;

                    if (all_edges_green(green_rects, x_start, x_end, y_start, y_end))
                        local_max_rect_area = std::max(local_max_rect_area, area);
                }

            return local_max_rect_area;
        };

        // Launch async workers
        const u64             num_workers{std::thread::hardware_concurrency()};
        vec<std::future<s64>> futures;
        vec<u64>              starts(num_workers + 1);
        for (u64 t = 0; t <= num_workers; ++t)
            starts[t] = t * red_positions.size() / num_workers;
        for (u64 t = 0; t < num_workers; ++t)
            futures.push_back(std::async(std::launch::async, worker, starts[t], starts[t + 1]));

        // Reduce the local maxima
        s64 global_max = 0;
        for (auto& f : futures)
            global_max = std::max(global_max, f.get());

        return global_max;
    }

    static constexpr pair<s64, s64> expected ()
    {
        return {4735222687, 1569262188};
    }
};

} // namespace day09
