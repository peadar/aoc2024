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

struct Key {
   Point pos;
   Direction d;
   auto operator <=> (const Key &rhs) const = default;
};

struct Node {
   unsigned long cost { MAX };
   Key key;
   auto operator <=> (const Node &rhs) const = default;
};

struct Part2 {
   std::vector<std::vector<Cell>> map;
   std::vector<std::vector<std::array<std::vector<Key>, 4>>> paths;
   Point s, e;

   Cell & at(Point p) {
      return map[p.row][p.col];
   }

   void visit(Key k, std::set<Point> &best_points) {
      best_points.insert(k.pos);
      if (k.pos != s) {
         for (auto &k2 : paths[k.pos.row][k.pos.col][k.d])
            visit(k2, best_points);
      }
   }

   Int dijkstra() {
      std::set<Node> Q;
      Q.insert({.cost=0, .key={.pos = s, .d=EAST}});
      while (!Q.empty()) {
         auto head = Q.begin();
         auto item = *head;
         Q.erase(head);

         auto try_add = [&](Key to, Int c, Key prev) {
            auto &cell = at(to.pos);
            if (!cell.wall) {
               if (cell.costs[to.d] == c) {
                  paths[to.pos.row][to.pos.col][to.d].push_back(prev);
               } else if (cell.costs[to.d] > c) {
                  paths[to.pos.row][to.pos.col][to.d].clear();
                  paths[to.pos.row][to.pos.col][to.d].push_back(prev);
                  Q.insert({.cost=c, .key=to});
                  cell.costs[to.d] = c;
               }
            }
         };

         Direction d = item.key.d;
         try_add(Key{item.key.pos + velocity(d), d}, item.cost + 1, item.key);
         d = rotate(d);
         try_add({item.key.pos, d}, item.cost + 1000, item.key);
         d = rotate(d);
         try_add({item.key.pos, d}, item.cost + 2000, item.key);
         d = rotate(d);
         try_add({item.key.pos, d}, item.cost + 1000, item.key);
      }
      auto ecell = at(e);
      auto ebest = *std::min_element( ecell.costs, ecell.costs + 4 );
      std::set<Point> best_points;

      for (auto dir : { NORTH, SOUTH, EAST, WEST })
         if (ecell.costs[dir] == ebest)
            visit({e, dir}, best_points);
      return best_points.size();
   }

   Part2(std::istream &is) noexcept {
      unsigned row = 0;
      for (std::string line; getline(is, line); ) {
         map.emplace_back();
         paths.emplace_back();
         unsigned col = 0;
         for (auto c : line) {
            map.back().emplace_back(c);
            paths.back().emplace_back();
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
   Int solve(std::ostream &) {
      return dijkstra();
   }
};


aoc::Case part2{"part2", [](std::istream &is, std::ostream &os) { os << Part2(is).solve(os); } };

}
