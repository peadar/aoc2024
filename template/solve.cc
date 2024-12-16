#include "aoc.h"

namespace {

enum Direction { NORTH = '^', SOUTH='v', EAST='<', WEST='>' };

using Scalar = uint8_t;
using Int = unsigned long;

struct Point {
   Scalar row{}, col{};
   Point operator+ (const Point &rhs) const noexcept { return { uint8_t(row + rhs.row), uint8_t(col + rhs.col) }; }
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
   Part(std::istream &) noexcept { }
};

struct Part1 : Part {
   Part1(std::istream &is) noexcept : Part(is) { }
   Int solve(std::ostream &) { return 0; }
};

aoc::Case part1{"part1", [](std::istream &is, std::ostream &os) { Part1(is).solve(os); } };


struct Part2 : Part {
   Part2(std::istream &is) noexcept : Part(is) { }
   Int solve(std::ostream &) { return 0; }
};

aoc::Case part2{"part2", [](std::istream &is, std::ostream &os) { Part2(is).solve(os); } };

}
