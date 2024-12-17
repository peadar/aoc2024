#include "aoc.h"

namespace {

enum Direction { NORTH, EAST, SOUTH, WEST };

using Scalar = unsigned;
using Int = unsigned long;
constexpr unsigned long MAX = std::numeric_limits<unsigned long>::max();

struct Point {
   Scalar row{}, col{};
   Point operator+ (const Point &rhs) const noexcept {
      return { uint8_t(row + rhs.row), uint8_t(col + rhs.col) };
   }
   auto operator <=> (const Point &rhs) const = default;
};

struct Cell {
   char c; // can kill this when done debugging.
   unsigned wall:1;
   std::bitset<4> visited;
   Int costs[4] = { MAX, MAX, MAX, MAX };
   Cell(char c) : c(c), wall(c == '#') {}
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
constexpr Direction rotate(Direction d) noexcept {
   return Direction((d + 1) % 4);
}

[[maybe_unused]]
std::ostream & operator << (std::ostream &os, Direction d) {
   switch (d) {
      case NORTH: return os << "NORTH";
      case SOUTH: return os << "SOUTH";
      case EAST: return os << "EAST";
      case WEST: return os << "WEST";
      default: __builtin_unreachable();
   }
}

struct Node {
   unsigned long cost { MAX };
   Direction d{};
   Point pos{};
   auto operator <=> (const Node &rhs) const = default;
};

struct Part {
   std::vector<std::vector<Cell>> map;
   Point s, e;

   Cell & at(Point p) {
      return map[p.row][p.col];
   }

   Int dijkstra() {
      std::set<Node> Q;
      Q.insert({.cost=0, .d=EAST, .pos = s});
      while (Q.begin()->pos != e) {
         auto head = Q.begin();
         auto item = *head;
         Q.erase(head);
         at(item.pos).visited[item.d] = true; // we now have the shortest path to this item.
         //std::cout << "cost to row=" << item.pos.row << ", col=" << item.pos.col << " in direction " << item.d << " is " << item.cost <<"\n";

         auto try_add = [&](Point p, Direction d, Int c) {
            auto &cell = at(p);
            if (!cell.wall && !cell.visited[d]  && cell.costs[d] > c) {
               Q.insert({.cost=c, .d=d, .pos=p });
            }
         };

         Direction d = item.d;
         try_add(item.pos + velocity(d), d, item.cost + 1);
         d = rotate(d);
         try_add(item.pos, d, item.cost + 1000);
         d = rotate(d);
         try_add(item.pos, d, item.cost + 2000);
         d = rotate(d);
         try_add(item.pos, d, item.cost + 1000);
         assert(!Q.empty());
      }
      return Q.begin()->cost;
   }

   Part(std::istream &is) noexcept {
      unsigned row = 0;
      for (std::string line; getline(is, line); ) {
         map.emplace_back();
         unsigned col = 0;
         for (auto c : line) {
            map.back().emplace_back(c);
            switch (c) {
               case 'S':
                  s = { row, col };
                  break;
               case 'E':
                  e = { row, col };
                  break;
            }
            ++col;
         }
         ++row;
      }
   }
};

struct Part1 : Part {
   Part1(std::istream &is) noexcept : Part(is) { }
   Int solve(std::ostream &) {
      return dijkstra();
   }
};

aoc::Case part1{"part1", [](std::istream &is, std::ostream &os) { os << Part1(is).solve(os); } };

}
