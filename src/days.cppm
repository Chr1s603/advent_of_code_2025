export module aoc.days;

import std;

import util.types;

import aoc.runner;
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
inline constexpr arr<runner, 12> registry = {make_runner<day01::Day01>(),
                                             make_runner<day02::Day02>(),
                                             make_runner<day03::Day03>(),
                                             make_runner<day04::Day04>(),
                                             make_runner<day05::Day05>(),
                                             make_runner<day06::Day06>(),
                                             make_runner<day07::Day07>(),
                                             make_runner<day08::Day08>(),
                                             make_runner<day09::Day09>(),
                                             make_runner<day10::Day10>(),
                                             make_runner<day11::Day11>(),
                                             make_runner<day12::Day12>()};
}
