#include "aoc.h"
namespace {

using Scalar = int;
using Cost = unsigned short;

struct Cell {
   static constexpr Cost MAXCOST = std::numeric_limits<Cost>::max() - 1;
   static constexpr Cost WALL = std::numeric_limits<Cost>::max();
   Cost cost;
   Cell( char c ) : cost{ c == '#' ? WALL :MAXCOST} {}
   bool wall() const { return cost == WALL; }
};

struct Point {
   Scalar row{}, col{};
   Point operator+ (const Point &rhs) const noexcept { return { Scalar(row + rhs.row), Scalar(col + rhs.col) }; }
   auto operator <=> (const Point &) const = default;
   unsigned mag() const { return abs(row) + abs(col); }
};

enum Direction { NORTH, EAST, SOUTH, WEST };
constexpr Point velocities[] = { {Scalar(-1), 0}, {0, 1}, {1, 0}, {0, Scalar(-1)} };

struct Maze {
   std::vector<Cell> cells;
   Scalar cols{}, rows{};
   Point START, END;
   bool contains(Point p) { return p.row >= 0 && p.row < rows && p.col >= 0 && p.col < cols; }
   Cell &at(Point p) { return cells[p.row * cols + p.col]; }
   const Cell &at(Point p) const { return cells[p.row * cols + p.col]; }
   // our maze has a single direction, so we just have to find the first 
   std::vector<Point> pathfind() {
      std::vector<Point> path;
      path.push_back(START);
      at(START).cost = 0;
      for (size_t pos = 0; path[pos] != END; ++pos) {
         for ( auto d : { NORTH, SOUTH, EAST, WEST} ) {
            Point &from = path[pos];
            const Cell &existing = at(from);
            auto to = from + velocities[d];
            if (!contains(to))
               continue;
            auto &cell = at(to);
            if (cell.wall() || cell.cost <= existing.cost + 1)
               continue;
            cell.cost = existing.cost + 1;
            path.push_back(to);
         }
         assert(path.size() > pos);
      }
      return path;
   }
   Maze(std::istream &is) noexcept {
      cells.reserve(141 * 141);
      rows = 0;
      for (std::string line; getline(is, line); ) {
         std::string_view v = line;
         cols = 0;
         for (auto c : v) {
            switch (c) {
               case 'S': START = Point{ rows, cols }; break;
               case 'E': END = Point{ rows, cols }; break;
            }
            cells.emplace_back(c);
            ++cols;
         }
         ++rows;
      }
   }
};

void solve(std::istream &is, std::ostream &os, int maxdistance, int threshold) {
   Maze maze(is);
   std::vector<Point> path = maze.pathfind();

   unsigned long total = 0;

   for (const auto &start : path) {
      for (int rowdelta = -maxdistance; rowdelta <= maxdistance; ++rowdelta) {
         for (int coldelta = -maxdistance + abs(rowdelta); coldelta <= maxdistance - abs(rowdelta); ++coldelta) {
            Point delta = {rowdelta, coldelta};
            Point end = start + delta;
            if (!maze.contains(end))
               continue;
            const Cell &startcell = maze.at(start);
            const Cell &endcell = maze.at(end);
            if (!endcell.wall() && endcell.cost > startcell.cost + delta.mag() &&
                  endcell.cost - startcell.cost - delta.mag() >= threshold)
               total++;
         }
      }
   }
   os << "\n" << total;
}

aoc::Case P1{"part1", [](auto &is, auto &os) { solve(is, os, 2, 100); }};
aoc::Case P2{"part2", [](auto &is, auto &os) { solve(is, os, 20, 100); }};
aoc::Case EG{"eg", [](auto &is, auto &os) { solve(is, os, 20, 50); }};
}
