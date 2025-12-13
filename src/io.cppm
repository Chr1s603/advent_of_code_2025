export module aoc.io;

import std;
import util.types;

export namespace aoc::io {

inline str
read_file (const str& path)
{
    std::ifstream f(path, std::ios::binary);
    if (!f)
        throw std::runtime_error("failed to open " + path);

    // Get length of string
    str s;
    f.seekg(0, std::ios::end);
    s.resize(static_cast<s64>(f.tellg()));

    // Read file into string
    f.seekg(0, std::ios::beg);
    f.read(s.data(), static_cast<std::streamsize>(s.size()));

    return s;
}

} // namespace aoc::io
