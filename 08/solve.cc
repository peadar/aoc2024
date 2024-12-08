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
   size_t operator() (const Point<T> &p) const noexcept { return p.row * 50 + p.col; }
};
}
using Int = int;
using P = Point<Int>;
using PointSet = std::unordered_set<P>;
struct Input {
   std::unordered_map<char, PointSet> points;
   P size{};
   Input(std::istream &is) {
      for (std::string line; getline(is, line);) {
         size.col = 0;
         for (char c : line) {
            if (c != '.')
               points[c].insert(size);
            ++size.col;
         }
         ++size.row;
      }
   }
};
struct Part1 : Input {
   Part1(std::istream &is) : Input(is) { }
   Int solve() const noexcept {
      PointSet antinodes;
      for (auto &[c, cpoints] : points)
         for (const P &a : cpoints)
            for (const P &b : cpoints) {
               if (a == b)
                  continue;
               auto anti = b + (b - a);
               if (anti.row >= 0 && anti.col >= 0 && anti.row < size.row && anti.col < size.col)
                  antinodes.insert(anti);
            }
      return antinodes.size();
   }
};
struct Part2 : Input {
   Part2(std::istream &is) : Input(is) {}
   Int solve() const noexcept {
      PointSet antinodes;
      for (auto &[c, cpoints] : points)
         for (auto ia = cpoints.begin(); ia != cpoints.end() ; ++ia)
            for (auto ib = cpoints.begin(); ib != cpoints.end() ; ++ib) {
               if (ia == ib)
                  continue;
               auto diff = *ib - *ia;
               for (auto p = *ia; p.row < size.row && p.col < size.col && p.row >= 0 && p.col >= 0; p += diff)
                  antinodes.insert(p);
            }
      return antinodes.size();
   }
};
aoc::Case part1("part1", [](std::istream &is, std::ostream &os) {os << Part1{is}.solve();});
aoc::Case part2("part2", [](std::istream &is, std::ostream &os) {os << Part2{is}.solve();});
aoc::Case parse("parse", [](std::istream &is, std::ostream &) {Input{is};});
