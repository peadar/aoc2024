#include "aoc.h"

namespace {

enum Direction { UP, RIGHT, DOWN, LEFT };

struct Point {
   unsigned row{};
   unsigned col{};
   Point operator + (const Point &rhs) const noexcept { return { row + rhs.row, col + rhs.col }; }
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

constexpr Direction rotate(Direction d) noexcept { return Direction((d + 1) % 4); }

constexpr Point velocity(Direction d) noexcept {
   switch (d) {
      case UP: return {unsigned(-1), 0};
      case DOWN: return {1, 0};
      case LEFT: return {0, unsigned(-1)};
      case RIGHT: return {0, 1};
      default: __builtin_unreachable();
   }
}

struct Guard {
   Point pos;
   Direction dir;
};

struct Parse {
   Guard guard_start;
   std::vector<std::vector<char>> walls; // faster than the vector<bool> mess. Could probably do better.

   Parse(std::istream &is) noexcept {
      walls.reserve(140);
      for (std::string line; std::getline(is, line); ) {
         std::vector<char> wall;
         wall.reserve(140);
         for (char c : line) {
            switch (c) {
            case '#':
               wall.push_back(true);
               break;
            case 'v': case '<': case '^': case '>':
               guard_start.pos = Point(walls.size(), wall.size());
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
         walls.push_back(std::move(wall));
      }
   }
   unsigned solve() { return 0; }


   // calculate if the guard escapes the configuration.
   template <typename Acceptor> bool escape_path(Acceptor acc) const noexcept {
      Guard guard = guard_start;
      std::vector<std::vector<unsigned char>> visited;
      visited.resize(walls.size());
      for (auto &vr : visited)
         vr.resize(walls[0].size());
      visited[guard_start.pos.row][guard_start.pos.col] = (1 << guard.dir);
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
         if (visitcell & (1<<guard.dir)) {
            // Already been here, travelling in this direction. That's a loop.
            return false;
         }
         if (visitcell == 0) {
            acc( nextpos );
         }
         visitcell |= (1 << guard.dir);
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
