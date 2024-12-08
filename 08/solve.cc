#include "aoc.h"

template <typename Scalar> struct Point {
   Scalar row{};
   Scalar col{};
   Point operator + (const Point &rhs) const noexcept { return { row + rhs.row, col + rhs.col }; }
   Point &operator += (const Point &rhs) noexcept {
      row += rhs.row;
      col += rhs.col;
      return *this;
   }
   Point operator - (const Point &rhs) const noexcept { return { row - rhs.row, col - rhs.col }; }
   Point operator - () const noexcept { return { -row, -col }; }
   Point operator * (Scalar m) const noexcept { return { row * m, col * m }; }
   Point abs () const { return { std::abs(row), std::abs(col) }; }
   auto operator <=> (const Point &rhs) const noexcept = default;
   bool operator == (const Point &rhs) const noexcept = default;
   unsigned magnitude() const noexcept { return std::abs(row) + std::abs(col); }
};

namespace std {
template <typename T> struct hash<Point<T>> {
   size_t operator() (const Point<T> &p) const noexcept { return p.row * 65537 + p.col; }
};
}

template <typename Scalar> struct Box {
   using P = Point<Scalar>;
   std::pair<P, P> extent {};
   Box(P a, P b) noexcept : extent { a, b } {}
   Box() noexcept : extent {} {}
   bool contains (const P &p) const noexcept {
      return p.row >= extent.first.row && p.col >= extent.first.col &&
         p.row < extent.second.row && p.col < extent.second.col;
   }
};

using Int = int;
using P = Point<Int>;
using Ps = std::unordered_set<P>;
struct Input {
   std::unordered_map<char, Ps> points;
   Box<Int> dimensions{};
   Input(std::istream &is) {
      P curpos;
      for (std::string line; getline(is, line);) {
         curpos.col = 0;
         for (char c : line) {
            if (c != '.')
               points[c].insert(curpos);
            ++curpos.col;
         }
         ++curpos.row;
      }
      dimensions = { {0,0}, curpos };
   }
};

struct Part1 {
   const Input &input;
   Part1(const Input &input) : input(input) { }
   Int solve() const noexcept {
      Ps antinodes;
      for (auto &[c, cpoints] : input.points)
         for (const P &a : cpoints)
            for (const P &b : cpoints) {
               if (&a == &b)
                  continue;
               auto anti = b + (b - a);
               if (input.dimensions.contains(anti))
                  antinodes.insert(anti);
            }
      return antinodes.size();
   }
};

struct Part2 {
   const Input &input;
   Part2(const Input &input) : input(input) { }
   Int solve() const noexcept {
      Ps antinodes;
      for (auto &[c, cpoints] : input.points)
         for (auto ia = cpoints.begin(); ia != cpoints.end() ; ++ia)
            for (auto ib = cpoints.begin(); ib != cpoints.end() ; ++ib) {
               if (ia == ib)
                  continue;
               auto diff = *ib - *ia;
               for (auto p = *ia + diff; input.dimensions.contains(p); p += diff)
                  antinodes.insert(p);
            }
      return antinodes.size();
   }
};

aoc::Case part1("part1", [](std::istream &is, std::ostream &os) {os << Part1{Input{is}}.solve();});
aoc::Case part2("part2", [](std::istream &is, std::ostream &os) {os << Part2{Input{is}}.solve();});
aoc::Case both("both", [](std::istream &is, std::ostream &os) {
      Input i{is};
      os << "part1: " << Part1{i}.solve() << ", part2: " << Part2{i}.solve(); });
aoc::Case parse("parse", [](std::istream &is, std::ostream &) {Input{is};});
