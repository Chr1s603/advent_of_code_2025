export module util.enumerable;

import std;

export namespace util::enumerable {

template <std::size_t N, std::ranges::range R>
auto
get_nth_of_each_element (const R& range)
{
    using std::get;
    using Elem    = std::ranges::range_value_t<R>;
    using NthType = std::tuple_element_t<N, Elem>;

    auto view = range | std::views::transform([] (auto const& elem) { return get<N>(elem); });
    return std::vector<NthType>(view.begin(), view.end());
}

bool
contains (const auto& container, const auto& val)
{
    return std::find(container.begin(), container.end(), val) != container.end();
}

}
