#include "aoc.h"
namespace {
struct Part1 {
   std::array<std::vector<unsigned>, 2> input;
   struct const_iterator {
      std::vector<unsigned>::const_iterator its[2];
      std::pair<unsigned, unsigned> operator *() const { return { *its[0], *its[1] }; }
      const_iterator operator++() { ++its[0], ++its[1]; return *this; }
      auto operator <=>(const const_iterator &) const = default;
   };
   const_iterator begin() const { return { { input[0].begin(), input[1].begin() } }; }
   const_iterator end() const { return { { input[0].end(), input[1].end() } }; }
   Part1(std::istream &in) {
      for ( unsigned l, r; (in >> l >> r), in; ) {
         input[0].push_back(l);
         input[1].push_back(r);
      }
      for (auto &vec : input)
         std::sort(vec.begin(), vec.end());
   }
   unsigned solve() const {
      return std::accumulate( begin(), end(), 0,
            [](unsigned acc, const auto &elt) {
            return acc + (elt.first > elt.second ? elt.first - elt.second : elt.second - elt.first ); });
   }
};
struct Part2 {
   std::array<std::map<unsigned, unsigned>, 2> counts;
   Part2(std::istream &in) {
      for ( unsigned l, r; (in >> l >> r), in; )
         ++counts[0][l], ++counts[1][r];
   }
   unsigned solve() {
      return std::accumulate(counts[0].begin(), counts[0].end(), 0,
            [this](unsigned acc, const auto &lhs) { return acc + lhs.first * lhs.second * counts[1][lhs.first]; });
   }
};
}
aoc::Case part1("part1", [](std::istream &is, std::ostream &os) { os << Part1( is ).solve(); });
aoc::Case part2("part2", [](std::istream &is, std::ostream &os) { os << Part2( is ).solve(); });
