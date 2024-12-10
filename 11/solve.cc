#include "aoc.h"
namespace {

using Int = unsigned long;
using Ints = std::vector<Int>;
using IntMap = std::unordered_map<Int, Int>;

struct State {
   Ints sequence;
   State(std::istream &is) {
      for (Int i;;) {
         is >> i;
         if (!is)
            break;
         sequence.push_back(i);
      }
   }
};

constexpr unsigned digit_count(Int i) {
   unsigned result = 1;
   for (Int compare = 10;; compare *=10, result++)
      if (compare > i)
         return result;
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
   IntMap a, b;
   IntMap *in, *out;
   for (auto i: seq)
      a[i]++;
   in = &a;
   out = &b;
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
   os << permute( State{is}.sequence, 25 );
}};

aoc::Case part2{ "part2", [](std::istream &is, std::ostream &os) {
   os << permute( State{is}.sequence, 75 );
}};
