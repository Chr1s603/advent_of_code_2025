export module util.parallel;

import std;
import util.types;

export namespace util::parallel {

template <typename Container, typename Func>
s64
sum_async (const Container& items, Func&& func)
{
    std::vector<std::future<s64>> futures;
    futures.reserve(items.size());

    for (const auto& item : items)
        futures.emplace_back(std::async(std::launch::async, func, std::cref(item)));

    s64 sum{0};
    for (auto& f : futures)
        sum += f.get();

    return sum;
}

}