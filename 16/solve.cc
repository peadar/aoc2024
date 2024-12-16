#include "aoc.h"

namespace {

enum Direction { NORTH = '^', SOUTH='v', EAST='<', WEST='>' };

using Scalar = uint8_t;
using Int = unsigned long;

struct Point {
   Scalar row{}, col{};
   Point operator+ (const Point &rhs) const noexcept { return { uint8_t(row + rhs.row), uint8_t(col + rhs.col) }; }
};

struct Cell {
   unsigned wall:1;
   Cell(char c) : wall(c == '#') {}
};

constexpr Point velocity(Direction d) noexcept {
   switch (d) {
      case NORTH: return {Scalar(-1), 0};
      case SOUTH: return {1, 0};
      case EAST: return {0, 1};
      case WEST: return {0, Scalar(-1)};
      default: __builtin_unreachable();
   }
}

struct Part {
   std::vector<std::vector<Cell>> map;
   Part(std::istream &is) noexcept {
      for (std::string line; getline(is, line); ) {
         map.emplace_back();
         std::copy(line.begin(), line.end(), std::back_inserter(map.back()) );
      }
   }
};

struct Part1 : Part {
   Part1(std::istream &is) noexcept : Part(is) { }
   Int solve(std::ostream &) { return 0; }
};

aoc::Case part1{"part1", [](std::istream &is, std::ostream &os) { Part1(is).solve(os); } };

}
