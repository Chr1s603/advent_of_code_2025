export module util.parse;

import std;
import util.types;

export namespace util::parse {

template <typename T = s64>
T
to_number (const sv token)
{
    T value{};
    std::from_chars(token.data(), token.data() + token.size(), value);
    return value;
}

template <typename T = s64>
auto
to_numbers ()
{
    return std::views::transform([] (std::string_view token) {
        T value{};
        std::from_chars(token.data(), token.data() + token.size(), value);
        return value;
    });
}

inline vec<std::string_view>
split (std::string_view s, std::string_view delimiter = " ")
{
    vec<std::string_view> out;
    if (delimiter.empty())
    {
        out.push_back(s);
        return out;
    }

    std::size_t pos = 0;
    while (pos <= s.size())
    {
        std::size_t next = s.find(delimiter, pos);
        if (next == std::string_view::npos)
        {
            out.emplace_back(s.substr(pos));
            break;
        }
        out.emplace_back(s.substr(pos, next - pos));
        pos = next + delimiter.size();
    }

    return out;
}

template <typename T>
inline vec<T>
parse_whitespace_numbers (sv s)
{
    vec<T>      out;
    const char* p = s.data();
    const char* e = s.data() + s.size();

    while (p < e)
    {
        while (p < e && std::isspace(static_cast<unsigned char>(*p)))
            ++p;
        if (p >= e)
            break;

        T value{};
        auto [q, ec] = std::from_chars(p, e, value);
        if (ec != std::errc{})
            break;
        p = q;

        out.push_back(value);
    }

    return out;
}

template <typename T>
inline vec<pair<T, T>>
parse_whitespace_pairs (sv s)
{
    vec<pair<T, T>> out;
    const char*     p = s.data();
    const char*     e = s.data() + s.size();

    while (p < e)
    {
        while (p < e && std::isspace(static_cast<unsigned char>(*p)))
            ++p;
        if (p >= e)
            break;

        T a{}, b{};
        auto [q1, ec1] = std::from_chars(p, e, a);
        if (ec1 != std::errc{})
            break;
        p = q1;

        while (p < e && std::isspace(static_cast<unsigned char>(*p)))
            ++p;
        if (p >= e)
            break;

        auto [q2, ec2] = std::from_chars(p, e, b);
        if (ec2 != std::errc{})
            break;
        p = q2;

        out.emplace_back(a, b);
    }

    return out;
}

}
