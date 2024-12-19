#include "aoc.h"

namespace {
enum Direction { NORTH, EAST, SOUTH, WEST };

using Scalar = unsigned;
using Int = unsigned short;
constexpr Int MAX = std::numeric_limits<Int>::max();

struct Point {
   Scalar row{}, col{};
   Point operator+ (const Point &rhs) const noexcept { return { Scalar(row + rhs.row), Scalar(col + rhs.col) }; }
   auto operator <=> (const Point &rhs) const = default;
};

struct Cell {
   unsigned short wall:1{};
   unsigned short epoch:15{};
   Int cost;
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

struct Node {
   unsigned long cost { MAX };
   Point pos{};
   auto operator <=> (const Node &rhs) const = default;
};

static constexpr Scalar ROWS = 71;
static constexpr Scalar COLS = 71;

struct MemorySpace {
   std::vector<Cell> map;
   unsigned age{};
   Point s{0,0}, e{ROWS-1, COLS-1};

   Cell & at(Point p) { return map[p.row * COLS + p.col]; }
   std::vector<Point> walls;
   const Cell & at(Point p) const { return map[p.row * COLS + p.col]; }

   Int dijkstra(unsigned maxage) {
      for (auto &cell : map)
         cell.cost = MAX;
      std::set<Node> Q;
      Q.insert({.cost=0, .pos = {0,0}});
      while (Q.begin()->pos != e) {
         auto head = Q.begin();
         auto item = *head;
         Q.erase(head);
         auto try_add = [&](Point p, Int c) {
            if (p.row >= ROWS || p.col >= COLS)
               return;
            auto &cell = at(p);
            if ((cell.wall && cell.epoch < maxage) || cell.cost < c )
               return;
            cell.cost = c;
            Q.insert({.cost=c, .pos=p });
         };
         try_add(item.pos + velocity(NORTH), item.cost + 1);
         try_add(item.pos + velocity(SOUTH), item.cost + 1);
         try_add(item.pos + velocity(EAST), item.cost + 1);
         try_add(item.pos + velocity(WEST), item.cost + 1);
         if (Q.empty())
            return MAX;
      }
      return Q.begin()->cost;
   }

   MemorySpace(std::istream &is) noexcept : map ( ROWS * COLS ) {
      for (std::string line; getline(is, line); ) {
         std::string_view v = line;
         Scalar col = aoc::parsetoken<int>(v, ",");
         Scalar row = aoc::parsetoken<int>(v, "\n");
         walls.emplace_back(row, col);
         auto &cell = at({row, col});
         cell.wall = true;
         cell.epoch = age++;
      }
   }
};
aoc::Case case1{"part1", [](std::istream &is, std::ostream &os) { os << MemorySpace(is).dijkstra(1024); } };

struct Part2 : MemorySpace {
   Part2( std::istream &is ) : MemorySpace(is) {}
   void part2(std::ostream &os) {
      auto res = std::ranges::partition_point( std::ranges::iota_view(unsigned(0), age) , [this] ( unsigned value ) { auto v = dijkstra(value); return v != MAX; });
      os << walls[*res - 1].col << "," << walls[*res - 1].row << "\n";
   }
};
aoc::Case case2{"part2", [](std::istream &is, std::ostream &os) { Part2(is).part2(os); } };
}
