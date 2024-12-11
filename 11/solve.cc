#include "aoc.h"
namespace {

using Int = unsigned long;
using Ints = std::vector<Int>;
using IntMap = std::unordered_map<Int, Int>;

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

Int permute(Ints &&seq, size_t iters) {
   IntMap a, *in=&a, b, *out=&b;
   for (auto i: seq)
      (*in)[i]++;
   for (size_t iter = 0; iter < iters; ++iter) {
      for (auto &[ink, incount] : *in)
         permute_one( ink, [&] (Int outk) { (*out)[outk] += incount; });
      std::swap(out, in);
      out->clear();
   }
   return std::accumulate( in->begin(), in->end(), 0ULL, [](Int acc, auto &k) { return acc + k.second; } );
}

}

aoc::Case part1{ "part1", [](std::istream &is, std::ostream &os) {
   os << permute(parse(is), 25 );
}};

aoc::Case part2{ "part2", [](std::istream &is, std::ostream &os) {
   os << permute( parse(is), 75 );
}};
