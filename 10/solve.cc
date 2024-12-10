#include "aoc.h"
namespace {
template <typename Accumulator> struct Solve {
   std::vector<std::vector<unsigned char>> grid;
   Solve(std::istream &is) {
      for ( std::string line; getline(is, line); )
         std::transform(line.begin(), line.end(), std::back_inserter((grid.emplace_back(), grid.back())),
               [](auto from) { return from - '0'; });
   }
   void find_heads(unsigned row, unsigned col, unsigned expected, Accumulator &acc) const {
      if (row < grid.size() && col < grid[0].size() && grid[row][col] == expected) {
         if (expected == 9) {
            acc.visit(row, col);
         } else {
            static constexpr std::pair<int, int> dirs[] { { -1, 0 }, { 1, 0 }, { 0, -1 }, { 0, 1 } };
            for (const auto &[ dr, dc ] : dirs)
               find_heads(row + dr, col + dc, expected + 1, acc);
         }
      }
   }
   unsigned operator()() const {
      unsigned score = 0;
      for (unsigned row = 0; row < grid.size(); ++row)
         for (unsigned col = 0; col < grid[row].size(); ++col) {
            Accumulator accumulate;
            find_heads(row, col, 0, accumulate);
            score += accumulate.value();
         }
      return score;
   }
};
struct Part1 {
   std::set<std::pair<unsigned, unsigned>> summits;
   void visit(unsigned row, unsigned col) { summits.insert({row,col}); }
   unsigned value() const { return summits.size(); }
};
struct Part2 {
   unsigned total{};
   void visit(unsigned, unsigned) { ++total; }
   unsigned value() const { return total; }
};
}
aoc::Case part1{ "part1", [](std::istream &is, std::ostream &os) { os << Solve<Part1>{is}(); }};
aoc::Case part2{ "part2", [](std::istream &is, std::ostream &os) { os << Solve<Part2>{is}(); }};
