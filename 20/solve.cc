#include "aoc.h"
namespace {

using Scalar = int;
using Cost = int;
template <typename T> using Vec = std::vector<T>;

struct Cell {
   static constexpr Cost MAXCOST = std::numeric_limits<Cost>::max();
   static constexpr Cost WALL = -1; // less than any other cost, so looks visited.
   Cost cost;
   Cell( char c ) : cost{ c == '#' ? WALL :MAXCOST} {}
};

struct Point {
   Scalar row_, col_;
   Point operator+ (const Point &rhs) const noexcept {
      return { Scalar(row() + rhs.row()), Scalar(col() + rhs.col()) }; }
   auto operator <=> (const Point &) const = default;
   int mag() const { return abs(row()) + abs(col()); }
   Scalar row() const { return row_; }
   Scalar col() const { return col_; }
};

enum Direction { NORTH, EAST, SOUTH, WEST };
constexpr Point velocities[] = { {Scalar(-1), 0}, {0, 1}, {1, 0}, {0, Scalar(-1)} };

struct Maze {
   Vec<Cell> cells;
   Scalar cols{}, rows{};
   Point START, END;
   bool contains(Point p) { return p.row() >= 0 && p.row() < rows && p.col() >= 0 && p.col() < cols; }
   Cell &at(Point p) { return cells[p.row() * cols + p.col()]; }
   const Cell &at(Point p) const { return cells[p.row() * cols + p.col()]; }
   // our maze has a single direction, so we just have to find the first 
   template <typename V>
   void pathfind(V &path) {
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
            if (cell.cost <= existing.cost + 1)
               continue;
            cell.cost = existing.cost + 1;
            path.push_back(to);
         }
         assert(path.size() > pos);
      }
   }
   Maze(const Maze &) = delete;
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

template <int maxdistance, int threshold>
void solve(std::istream &is, std::ostream &os) {
   Maze maze(is);
   Vec<Point> path;
   maze.pathfind(path);
   int total = 0;
   for (const auto &start : path) {
      int minrowd = std::max(-maxdistance, -start.row());
      int maxrowd = std::min(maxdistance, maze.rows - start.row() - 1);
      const Cell &startcell = maze.at(start);
      for (int rowdelta = minrowd; rowdelta <= maxrowd; ++rowdelta) {
         int mincold = std::max( -maxdistance + abs(rowdelta), -start.col());
         int maxcold = std::min( maxdistance - abs(rowdelta), maze.cols - start.col() - 1);
         for (int coldelta = mincold; coldelta <= maxcold; ++coldelta) {
            Point delta = {rowdelta, coldelta};
            if (maze.at(start + delta).cost - startcell.cost - delta.mag() >= threshold)
               total++;
         }
      }
   }
   os << "\n" << total;
}

aoc::Case P1{"part1", [](auto &is, auto &os) { solve<2,100>(is, os); }};
aoc::Case P2{"part2", [](auto &is, auto &os) { solve<20,100>(is, os); }};
}
