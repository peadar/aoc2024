#include "aoc.h"
#include "lintable.h"
namespace {
template <typename T>
auto parse(std::istream &in) {
   std::vector<T> sequence;
   for (T i;;) {
      in >> i;
      if (!in)
         break;
      sequence.push_back(i);
   }
   return sequence;
};

template <typename Int>
constexpr unsigned digit_count(Int i) noexcept {
   unsigned result = 1;
   for (Int compare = 10;; compare *= 10, result++)
      if (compare > i)
         return result;
}

template <typename Int>
constexpr Int pow10(unsigned count) noexcept {
   switch (count) { // this measures margiinally faster than a loop multiplying
                    // by 10 each time. Additional cases beyond 4 don't help.
      case 0: return 1;
      case 1: return 10;
      case 2: return 100;
      case 3: return 1000;
      case 4: return 10000;
      default: {
         Int half = count / 2;
         return pow10<Int>(half) * pow10<Int>(count - half);
      }
   }
}

template <typename Int>
constexpr void stone(Int i, auto yield) noexcept {
   if (i == 0) {
      yield(1);
   } else if (unsigned digits = digit_count(i); digits % 2 == 0) {
      Int denom = pow10<Int>(digits / 2);
      yield(i / denom);
      yield(i % denom);
   } else {
      yield(i * 2024);
   }
}

template <typename Map, size_t times, typename Ints>
constexpr Map::mapped_type blink(const Ints seq) noexcept {
   Map a, *in = &a, b, *out = &b;
   for (auto i: seq)
      (*in)[i]++;
   for (size_t iter = 0; iter < times; ++iter) {
      for (auto key_and_count : *in)
         stone(key_and_count.first, [key_and_count, out] (Map::mapped_type outk) constexpr {
               (*out)[outk] += key_and_count.second; });
      std::swap(out, in);
      out->clear();
   }
   return std::accumulate(in->begin(), in->end(), 0ULL,
         [](typename Map::mapped_type acc, auto k) { return acc + k.second; });
}

template <typename Map, size_t times>
void partN(std::istream &is, std::ostream &os) {
   os << blink<Map, times>(parse<typename Map::mapped_type>(is));
}

using Int = unsigned long;
using Linmap = LinTable<Int, Int, 65537>;

aoc::Case part1{ "part1", partN<std::unordered_map<Int, Int>, 25>};
aoc::Case part2{ "part2", partN<std::unordered_map<Int, Int>, 75>};
aoc::Case part1b{ "part1-alt", partN<Linmap, 25>};
aoc::Case part2b{ "part2-alt", partN<Linmap, 75>};

constexpr std::array <Int, 2> const_eg { 125, 17 };
aoc::Case part1c{ "part1-const", [](std::istream &, std::ostream &os) { os << blink<Linmap, 25>(const_eg); } };
aoc::Case part2c{ "part2-const", [](std::istream &, std::ostream &os) { os << blink<Linmap, 75>(const_eg); } };
}
