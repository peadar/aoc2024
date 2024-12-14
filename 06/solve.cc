#include "aoc.h"
#include <boost/container/small_vector.hpp>

namespace {

enum Direction { UP = 1 << 0, RIGHT = 1 << 1, DOWN = 1 << 2, LEFT = 1 << 3 };

using Scalar = uint8_t;

struct Point {
   Scalar row{};
   Scalar col{};
   Point operator + (const Point &rhs) const noexcept { return { uint8_t(row + rhs.row), uint8_t(col + rhs.col) }; }
   auto operator <=> (const Point &rhs) const noexcept = default;
   bool operator == (const Point &rhs) const noexcept = default;
};

}
template <> struct std::hash<Point> { size_t operator()(const Point &p) const noexcept { return p.row * 64 + p.col; } };

namespace {

constexpr Direction c2dir (char c) noexcept {
   switch (c) {
      case '^': return UP;
      case '>': return RIGHT;
      case 'v': return DOWN;
      case '<': return LEFT;
      default: __builtin_unreachable();
   }
}

constexpr Direction rotate(Direction d) noexcept {
   switch (d) {
      case UP: return RIGHT;
      case RIGHT: return DOWN;
      case DOWN: return LEFT;
      case LEFT: return UP;
      default: __builtin_unreachable();
   }
}

constexpr Point velocity(Direction d) noexcept {
   switch (d) {
      case UP: return {Scalar(-1), 0};
      case DOWN: return {1, 0};
      case LEFT: return {0, Scalar(-1)};
      case RIGHT: return {0, 1};
      default: __builtin_unreachable();
   }
}

struct Guard {
   Point pos;
   Direction dir;
};

template <typename T>
using Vec = boost::container::small_vector<T, 60>;

struct Parse {
   Guard guard_start;
   Vec<Vec<char>> walls;

   Parse(std::istream &is) noexcept {
      walls.reserve(130);
      for (std::string line; std::getline(is, line); ) {
         walls.emplace_back();
         Vec<char> &wall = walls.back();
         wall.reserve(131);
         for (char c : line) {
            switch (c) {
            case '#':
               wall.push_back(true);
               break;
            case 'v': case '<': case '^': case '>':
               guard_start.pos = Point{Scalar(walls.size()), Scalar(wall.size())};
               guard_start.dir = c2dir(c);
               wall.push_back(false);
               break;
            case '.':
               wall.push_back(false);
               break;
            default:
               abort();
            }
         }
      }
   }
   unsigned solve() { return 0; }

   // calculate if the guard escapes the configuration.
   template <typename Acceptor> bool escape_path(Acceptor acc) const noexcept {
      Guard guard = guard_start;
      Vec<Vec<unsigned char>> visited;
      visited.resize(walls.size());
      for (auto &vr : visited)
         vr.resize(walls[0].size());
      visited[guard_start.pos.row][guard_start.pos.col] = guard.dir;
      acc(guard_start.pos);
      for (;;) {
         auto nextpos = guard.pos + velocity(guard.dir);
         if (nextpos.row >= walls.size() || nextpos.col >= walls[0].size())
            break;
         bool iswall = walls[nextpos.row][nextpos.col];
         if (iswall) {
            guard.dir = rotate(guard.dir);
            continue;
         } 
         auto & visitcell = visited[nextpos.row][nextpos.col];
         if (visitcell & guard.dir) {
            // Already been here, travelling in this direction. That's a loop.
            return false;
         }
         if (visitcell == 0) {
            acc( nextpos );
         }
         visitcell |= guard.dir;
         guard.pos = nextpos;
      }
      return true;
   }
};

struct Part1 : Parse {
   Part1( std::istream &is) : Parse( is ) {}
   unsigned solve(std::ostream &) const noexcept {
      int count = 0;
      bool rc = escape_path([&count] (auto &) { ++count; });
      assert(rc);
      return count;
   }
};

struct Part2 : Parse {
   Part2(std::istream &is) : Parse(is) {}
   unsigned solve(std::ostream &) noexcept {
      std::unordered_set<Point> orig_path;
      bool must_escape = escape_path([&orig_path] (const Point &p) {orig_path.insert(p);});
      assert(must_escape);
      unsigned result {};
      for (auto &[row, col] : orig_path) {
         auto & c = walls[row][col];
         if (c)
            continue;
         if (row == guard_start.pos.row && col == guard_start.pos.col)
            continue;
         c = true;
         if (!escape_path([](auto &) {}))
            ++result;
         c = false;
      }
      return result;
   }
};
}

aoc::Case part1("part1", [](std::istream &is, std::ostream &os) {os << Part1( is ).solve(std::cout);});
aoc::Case part2("part2", [](std::istream &is, std::ostream &os) {os << Part2( is ).solve(std::cout);});
aoc::Case parse("parse", [](std::istream &is, std::ostream &) {Parse{ is };});
