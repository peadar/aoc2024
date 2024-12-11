#include "aoc.h"
#include "lintable.h"
namespace {

using Int = unsigned long;
using Ints = std::vector<Int>;

Ints parse(std::istream &in) {
   Ints sequence;
   for (Int i;;) {
      in >> i;
      if (!in)
         break;
      sequence.push_back(i);
   }
   return sequence;
};

constexpr inline unsigned digit_count(Int i) noexcept {
   unsigned result = 1;
   for (Int compare = 10;; compare *=10, result++)
      if (compare > i)
         return result;
}

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
         return pow10(half) * pow10(count - half);
      }
   }
}

void permute_one( Int i, auto yield ) {
   if (i == 0) {
      yield(1);
   } else {
      unsigned digits = digit_count(i);
      if (digits % 2 == 0) {
         Int denom = pow(10, double( digits ) / 2 );
         yield( i / denom );
         yield( i % denom );
      } else {
         yield( i * 2024 );
      }
   }
}

template <template <typename, typename> typename MapType>
Int permute(Ints &&seq, size_t iters) noexcept {
   MapType<Int, Int> a, *in = &a, b, *out = &b;
   for (auto i: seq)
      (*in)[i]++;
   for (size_t iter = 0; iter < iters; ++iter) {
      for (auto key_and_count : *in)
         permute_one( key_and_count.first, [&] (Int outk) { (*out)[outk] += key_and_count.second; });
      std::swap(out, in);
      out->clear();
   }
   return std::accumulate( in->begin(), in->end(), 0ULL, [](Int acc, auto k) { return acc + k.second; } );
}
}

template <typename K, typename V> using SystemMap = std::unordered_map<K, V>;
template <typename K, typename V> using QuickTable = LinTable<K, V, 65537>;

aoc::Case part1{ "part1", [](std::istream &is, std::ostream &os) {
   os << permute<SystemMap>(parse(is), 25 );
}};

aoc::Case part2{ "part2", [](std::istream &is, std::ostream &os) {
   os << permute<SystemMap>( parse(is), 75 );
}};

aoc::Case part1b{ "part1-better", [](std::istream &is, std::ostream &os) {
   os << permute<QuickTable>(parse(is), 25 );
}};

aoc::Case part2b{ "part2-better", [](std::istream &is, std::ostream &os) {
   os << permute<QuickTable>( parse(is), 75 );
}};
