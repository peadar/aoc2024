#include "aoc.h"
namespace {

using Scalar = int;
using Cost = unsigned;
constexpr Cost MAXCOST = std::numeric_limits<Cost>::max();

struct Cell {
   Cost cost{MAXCOST};
   unsigned wall : 1;
   Cell( char c ) : cost{MAXCOST}, wall{c == '#'} {}
};

enum Direction { NORTH, EAST, SOUTH, WEST };

enum Direction rotate_clockwise(Direction d, unsigned steps = 1) {
   return Direction((unsigned(d) + steps) % 4);
}

struct Point {
   Scalar row{}, col{};
   Point operator+ (const Point &rhs) const noexcept { return { row + rhs.row, col + rhs.col }; }
   auto operator <=> (const Point &) const = default;
};

std::ostream &operator << (std::ostream &os, const Point &p2) {
   return os << "[r=" << p2.row << ",c=" << p2.col << "]";
}

constexpr Point velocities[] = { {Scalar(-1), 0}, {0, 1}, {1, 0}, {0, Scalar(-1)} };

struct Maze {
   std::vector<Cell> cells;
   Scalar cols{}, rows{};
   Point START;
   Point END;

   bool contains(Point p) { return p.row >= 0 && p.row < rows && p.col >= 0 && p.col < cols; }
   Cell &at(Point p) { return cells[p.row * cols + p.col]; }
   const Cell &at(Point p) const { return cells[p.row * cols + p.col]; }

   // our maze has a single direction, so we just have to find the first 
   Cost pathfind(std::vector<Point> &path) {
      std::deque<Point> Q;
      Q.push_back(START);
      at(START).cost = 0;
      unsigned iterations;
      for (iterations = 0; Q.front() != END; ++iterations) {
         Point from = Q.front();
         Q.pop_front();
         path.push_back(from);

         const Cell &existing = at(from);
         for ( auto d : { NORTH, SOUTH, EAST, WEST} ) {
            auto to = from + velocities[d];
            if (!contains(to))
               continue;
            auto &cell = at(to);
            if (cell.wall || cell.cost <= existing.cost + 1)
               continue;
            cell.cost = existing.cost + 1;
            Q.push_back(to);
         }
         if (Q.empty())
            return MAXCOST;
      }
      return at(Q.front()).cost;
   }

   Maze(const Maze &) = default;
   Maze(Maze &&) = default;

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

std::ostream &operator<<(std::ostream &os, const Maze &maze) {
   for (Scalar row = 0; row < maze.rows; ++row) {
      for (Scalar col = 0; col < maze.cols; ++col) {
         Point p{row,col};
         if (maze.START == p )
            os << "[ST]";
         else if (maze.END == p ) {
            os << "[EN]";
         } else {
            const Cell &c = maze.at(p);
            if (c.wall)
               os << "####";
            else if (c.cost == MAXCOST)
               os << " .. ";
            else
               os << "." << std::setw(2) << c.cost << "." << std::setw(0);
         }
      }
      os << "\n";
   }
   return os;
}

void part1(std::istream &is, std::ostream &os) {
   Maze maze(is);
   std::vector<Point> path;
   unsigned orig = maze.pathfind(path);

   std::map<unsigned, std::set<std::pair<Point, Point>>> savings;
   unsigned total = 0;
   for (const auto &start : path) {
      for (auto delta : { Point{ 0, -2 }, { 0, 2 }, { 2, 0 }, { -2, 0 } }) {
         Point end = start + delta;
         if (!maze.contains(end))
            continue;
         Cost startcost = maze.at(start).cost;
         Cell endcell = maze.at(end);
         if (!endcell.wall && endcell.cost > startcost + 2) {
            Cost saving = endcell.cost - startcost - 2;
            savings[saving].insert({start, end});
            if (saving >= 100)
               ++total;
         }
      }
   }
   for (auto &[saving, count] : savings)
      os << "\nThere are " << count.size() << " cheats that save " << saving << " picoseconds.";
   os << "\n" << total;
}

aoc::Case P1{"part1", part1};
}
