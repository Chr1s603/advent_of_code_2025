export module aoc.days;

import std;

import util.types;

import aoc.runner;
import aoc.day_common;
import aoc.day01;
import aoc.day02;
import aoc.day03;
import aoc.day04;
import aoc.day05;
import aoc.day06;
import aoc.day07;
import aoc.day08;
import aoc.day09;
import aoc.day10;
import aoc.day11;
import aoc.day12;

export namespace aoc {

template <std::size_t... Is>
consteval arr<runner, sizeof...(Is)>
make_registry_from_idx (std::index_sequence<Is...>)
{
    arr<runner, sizeof...(Is)> res{};
    ((res[Is] = make_runner<day::Day<Is + 1>>(static_cast<s64>(Is + 1))), ...);
    return res;
}

inline constexpr auto registry = make_registry_from_idx(std::make_index_sequence<12>{});

}
