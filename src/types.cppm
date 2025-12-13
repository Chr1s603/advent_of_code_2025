export module util.types;

import std;

// STD library

export using str = std::string;
export using sv  = std::string_view;

export template <typename T, std::size_t L>
using arr = std::array<T, L>;

export template <typename T>
using vec = std::vector<T>;

export template <typename T1, typename T2>
using pair = std::pair<T1, T2>;

// Signed integers

export using s8  = std::int8_t;
export using s16 = std::int16_t;
export using s32 = std::int32_t;
export using s64 = std::int64_t;

export using cs8  = const s8;
export using cs16 = const s16;
export using cs32 = const s32;
export using cs64 = const s64;

export using vs8  = volatile s8;
export using vs16 = volatile s16;
export using vs32 = volatile s32;
export using vs64 = volatile s64;

export using cvs8  = const volatile s8;
export using cvs16 = const volatile s16;
export using cvs32 = const volatile s32;
export using cvs64 = const volatile s64;

// Unsigned integers

export using u8  = std::uint8_t;
export using u16 = std::uint16_t;
export using u32 = std::uint32_t;
export using u64 = std::uint64_t;

export using cu8  = const u8;
export using cu16 = const u16;
export using cu32 = const u32;
export using cu64 = const u64;

export using vu8  = volatile u8;
export using vu16 = volatile u16;
export using vu32 = volatile u32;
export using vu64 = volatile u64;

export using cvu8  = const volatile u8;
export using cvu16 = const volatile u16;
export using cvu32 = const volatile u32;
export using cvu64 = const volatile u64;

// Floating-point

export using f32  = float;
export using f64  = double;
export using f128 = long double;

export using cf32  = const f32;
export using cf64  = const f64;
export using cf128 = const f128;

export using vf32  = volatile f32;
export using vf64  = volatile f64;
export using vf128 = volatile f128;

export using cvf32  = const volatile f32;
export using cvf64  = const volatile f64;
export using cvf128 = const volatile f128;

// Sizes

export using usize = std::size_t;
export using isize = std::ptrdiff_t;
