export module aoc.day12;

import std;
import util.types;
import util.parse;
import util.parallel;

using namespace util::parse;

export namespace day12 {

struct Day12
{
    static constexpr s64 number = 12;
    static constexpr sv  name{"Day 12: Christmas Tree Farm"};

    enum class roate_t
    {
        rotate_0deg,
        rotate_90deg,
        rotate_180deg,
        rotate_270deg
    };
    struct region
    {
        s64       width, height;
        vec<bool> occupied;
        vec<s64>  present_occurences;
    };
    struct present
    {
        vec<vec<bool>>              local_occupy;
        arr<vec<pair<s64, s64>>, 4> occupied_cells;

        void prepare_rotations ()
        {
            const s64 h{s64(local_occupy.size())};
            const s64 w{s64(local_occupy[0].size())};

            // rotate_0deg
            for (s64 y = 0; y < h; y++)
                for (s64 x = 0; x < w; x++)
                    if (local_occupy[static_cast<u64>(y)][static_cast<u64>(x)])
                        occupied_cells[0].emplace_back(x, y);

            // rotate_90deg
            for (s64 y = 0; y < h; y++)
                for (s64 x = 0; x < w; x++)
                    if (local_occupy[static_cast<u64>(y)][static_cast<u64>(x)])
                        occupied_cells[1].emplace_back(h - 1 - y, x);

            // rotate_180deg
            for (s64 y = 0; y < h; y++)
                for (s64 x = 0; x < w; x++)
                    if (local_occupy[static_cast<u64>(y)][static_cast<u64>(x)])
                        occupied_cells[2].emplace_back(w - 1 - x, h - 1 - y);

            // rotate_270deg
            for (s64 y = 0; y < h; y++)
                for (s64 x = 0; x < w; x++)
                    if (local_occupy[static_cast<u64>(y)][static_cast<u64>(x)])
                        occupied_cells[3].emplace_back(y, w - 1 - x);
        }

        s64 get_local_width (roate_t r) const
        {
            switch (r)
            {
                case roate_t::rotate_0deg:
                    return s64(local_occupy[0].size());
                case roate_t::rotate_90deg:
                    return s64(local_occupy.size());
                case roate_t::rotate_180deg:
                    return s64(local_occupy[0].size());
                case roate_t::rotate_270deg:
                    return s64(local_occupy.size());
            }
            std::unreachable();
        }

        s64 get_local_height (roate_t r) const
        {
            switch (r)
            {
                case roate_t::rotate_0deg:
                    return s64(local_occupy.size());
                case roate_t::rotate_90deg:
                    return s64(local_occupy[0].size());
                case roate_t::rotate_180deg:
                    return s64(local_occupy.size());
                case roate_t::rotate_270deg:
                    return s64(local_occupy[0].size());
            }
            std::unreachable();
        }
    };
    using regions  = vec<region>;
    using presents = vec<present>;
    using parsed   = pair<presents, regions>;

    static parsed parse (const sv in)
    {
        const auto input_blocks{split(in, "\n\n")};

        presents presents;
        for (u64 present = 0; present < input_blocks.size() - 1; present++)
        {
            const auto     p_str{split(input_blocks[present], "\n")};
            vec<vec<bool>> shape(p_str.size() - 1);
            for (u64 row = 0; row < p_str.size() - 1; row++)
            {
                const auto row_str{p_str[row + 1]};
                shape[row].reserve(row_str.size());
                for (const auto& c : row_str)
                    shape[row].emplace_back(c == '#');
            }

            presents.emplace_back(shape);
        }
        for (auto& p : presents)
            p.prepare_rotations();

        const auto region_strs{split(input_blocks.back(), "\n")};
        regions    regions;
        regions.reserve(region_strs.size());
        for (u64 region = 0; region < region_strs.size(); region++)
        {
            const auto region_dimensions_and_present_occurences_str{
                util::parse::split(region_strs[region], ": ")};

            const auto region_dimensions_str{region_dimensions_and_present_occurences_str[0]};
            const auto present_occurences_str{region_dimensions_and_present_occurences_str[1]};

            const auto region_dimension_strs{split(region_dimensions_str, "x")};
            const auto present_occurence_strs{split(present_occurences_str, " ")};

            struct region r;
            r.width    = to_number(region_dimension_strs[0]);
            r.height   = to_number(region_dimension_strs[1]);
            r.occupied = vec<bool>(u64(r.width * r.height), false);
            for (const auto& occurence : present_occurence_strs)
                r.present_occurences.emplace_back(to_number(occurence));
            regions.emplace_back(std::move(r));
        }

        return {presents, regions};
    }

    static bool
    add_present (cs64 x, cs64 y, cu64 rot_idx, const present& p, region& region, vec<s64>& changed)
    {
        const s64 w{p.get_local_width(static_cast<roate_t>(rot_idx))};
        const s64 h{p.get_local_height(static_cast<roate_t>(rot_idx))};
        if (x + w > region.width || y + h > region.height)
            return false;

        changed.clear();
        for (const auto& [dx, dy] : p.occupied_cells[rot_idx])
        {
            cs64 idx = (y + dy) * region.width + (x + dx);
            if (region.occupied[static_cast<u64>(idx)])
                return false;
            changed.push_back(idx);
        }
        for (cs64 idx : changed)
            region.occupied[static_cast<u64>(idx)] = true;
        return true;
    }

    static bool
    try_place_present (const presents& ps, cu64 p_idx, cs64 current_occurence, region& r)
    {
        if (p_idx == ps.size())
            return true;
        if (current_occurence >= r.present_occurences[p_idx])
            return try_place_present(ps, p_idx + 1, 0, r);

        vec<s64> changed;
        for (s64 x = 0; x < r.width; x++)
            for (s64 y = 0; y < r.height; y++)
                for (u64 rot = 0; rot < 4; rot++)
                    if (add_present(x, y, rot, ps[p_idx], r, changed))
                    {
                        if (try_place_present(ps, p_idx, current_occurence + 1, r))
                            return true;
                        for (cs64 idx : changed)
                            r.occupied[static_cast<u64>(idx)] = false;
                    }
        return false;
    }

    static s64 present_area (const present& p)
    {
        s64 area{0};
        for (const auto& row : p.local_occupy)
            for (const bool b : row)
                area += b;
        return area;
    }

    static s64 part1 (const parsed& in)
    {
        return util::parallel::sum_async(in.second, [&] (const region& r) -> s64 {
            s64 required_area{0};
            for (u64 i = 0; i < in.first.size(); ++i)
                required_area += present_area(in.first[i]) * r.present_occurences[i];
            if (required_area > r.width * r.height)
                return 0;

            region current{r};
            return try_place_present(in.first, 0, 0, current) ? 1 : 0;
        });
    }

    static s64 part2 (const parsed&)
    {
        return 0;
    }

    static constexpr pair<s64, s64> expected ()
    {
        return {534, 0};
    }
};

} // namespace day12
