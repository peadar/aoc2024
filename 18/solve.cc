#include "aoc.h"
namespace {

using Scalar = unsigned;
using Cost = unsigned;
constexpr Cost MAXCOST = std::numeric_limits<Cost>::max();
static constexpr Scalar MAXROWS = 71;
static constexpr Scalar MAXCOLS = 71;

#if 0
using Point = std::complex<Scalar>;
constexpr Scalar row(const Point &p) noexcept { return p.imag(); }
constexpr Scalar col(const Point &p) noexcept { return p.real(); }
constexpr unsigned key(Point p) noexcept { return p.imag() * MAXCOLS + p.real(); }
constexpr Point make_point(Scalar row, Scalar col) noexcept { return { col, row }; }
#else
using Point = unsigned;
constexpr Scalar row(Point p) noexcept { return p / MAXCOLS; }
constexpr Scalar col(Point p) noexcept { return p % MAXCOLS; }
constexpr unsigned key(Point p) noexcept { return p; }
constexpr Point make_point(Scalar row, Scalar col) noexcept { return row * MAXCOLS + col; }
#endif

struct Cell { Cost cost{MAXCOST}; };
using Map = std::array<Cell, MAXCOLS * MAXROWS>;
Cell & at(Map &map, Point p) { return map[key(p)]; }

struct MemorySpace {
   Map weights;
   std::vector<Point> walls;

   static constexpr Point s { make_point(0,0) };
   static constexpr Point e { make_point( MAXROWS-1, MAXCOLS-1) };

   Cost bfs(Cost maxage) {
      Map map { weights };
      std::deque<Point> Q;
      at(map, make_point(0,0)).cost = 0;
      Q.push_back(make_point(0,0));
      while (*Q.begin() != e) {
         auto from = Q.front();
         Q.pop_front();
         auto try_add = [&](const Point &to, Cost c) {
            auto &cell = at(map, to);
            if (cell.cost < maxage)
               return;
            cell.cost = c;
            Q.push_back(to);
         };
         const Cell &existing = at(map, from);
         if (row(from) != 0)
            try_add(make_point(row(from) - 1, col(from)), existing.cost + 1);
         if (row(from) != MAXROWS - 1)
            try_add(make_point(row(from) + 1, col(from)), existing.cost + 1);
         if (col(from) != 0)
            try_add(make_point(row(from), col(from) - 1), existing.cost + 1);
         if (col(from) != MAXCOLS - 1)
            try_add(make_point(row(from), col(from) + 1), existing.cost + 1);
         if (Q.empty())
            return MAXCOST;
      }
      return at(map, *Q.begin()).cost;
   }

   MemorySpace(std::istream &is) noexcept {
      walls.reserve(4000);
      for (std::string line; getline(is, line); ) {
         std::string_view v = line;
         Scalar col = aoc::parsetoken<int>(v, ",");
         Scalar row = aoc::parsetoken<int>(v, "\n");
         auto p = make_point( row, col );
         auto &cell = at(weights, p);
         cell.cost = walls.size();
         walls.emplace_back(p);
      }
   }
};

aoc::Case case1{"part1", [](std::istream &is, std::ostream &os) { os << MemorySpace(is).bfs(1024); } };

struct Part2 : MemorySpace {
   Part2( std::istream &is ) : MemorySpace(is) {}
   void part2(std::ostream &os) {
      auto res = std::ranges::partition_point( std::ranges::iota_view(Cost(0), Cost(walls.size())) , [this] ( Cost v ) { return bfs(v) != MAXCOST; });
      os << col(walls[*res - 1]) << "," << row(walls[*res - 1]) << "\n";
   }
};
aoc::Case case2{"part2", [](std::istream &is, std::ostream &os) { Part2(is).part2(os); } };
}
