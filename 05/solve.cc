#include "aoc.h"
#include <stdint.h>

namespace {
using Bits = __uint128_t;
using IntVec = std::vector<unsigned>;

struct Parse {
   Bits befores[101] {};
   std::vector<IntVec> updates;
   std::istream &is;
   Parse(std::istream &is) : is(is) {
      for (std::string line; getline(is, line) && line != ""; ) {
         size_t before{}, after{};
         std::from_chars(line.data(), line.data() + 2, before);
         std::from_chars(line.data() + 3, line.data() + 5, after);
         befores[before] |= Bits(1) << after;
      }
      IntVec update;
      for (std::string line; getline(is, line); ) {
         update.clear();
         update.reserve(100);
         std::string_view rest = line;
         std::string_view numstr;
         while (rest.size()) {
            std::tie(numstr, rest) = aoc::token(rest, std::string_view(","));
            unsigned i;
            std::from_chars(numstr.data(), numstr.data() + numstr.size(), i);
            update.push_back(i);
         }
         updates.push_back(std::move(update));
      }
   }
   unsigned update_val(const IntVec &update) {
      Bits seen(0);
      for (auto page : update) {
         if (befores[page] & seen) 
            return 0;
         seen |= Bits(1) << page;
      }
      return update[update.size()/2];
   }
   unsigned solve() { return 0; }
};

struct Part1 : Parse {

   Part1( std::istream &is) : Parse( is ) {}

   unsigned solve() {
      return std::accumulate(updates.begin(), updates.end(),
            0, [this](unsigned acc, const auto &update) { return update_val(update) + acc; } );
   }
};

struct Part2 : Parse {
   Part2(std::istream &is) : Parse(is) {}

   unsigned update_val(IntVec &update) {
      unsigned swaps{};
      for (size_t i = 0; i < update.size() - 1; ++i) {
         for (size_t j = i+1; j < update.size(); ++j) {
            if (befores[update[j]] & (Bits(1) << update[i])) {
               // item at position j must come before item at position i - swap em.
               std::swap(update[i], update[j]);
               ++swaps;
            }
         }
      }
      return swaps ? update[update.size()/2] : 0;
   }

   unsigned solve() {
      return std::accumulate(updates.begin(), updates.end(),
            0, [this](unsigned acc, auto &update) { return update_val(update) + acc; } );
   }
};

}


aoc::Case part1("part1", [](std::istream &is, std::ostream &os) {os << Part1( is ).solve();});
aoc::Case part2("part2", [](std::istream &is, std::ostream &os) {os << Part2( is ).solve();});
aoc::Case parse("parse", [](std::istream &is, std::ostream &) {Parse{ is };});
