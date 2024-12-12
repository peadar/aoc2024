#include "aoc.h"
#include "2d.h"
namespace {

enum Direction { NORTH, EAST, SOUTH, WEST };
using P = aoc::Point<unsigned>;

const P direction_velocities[] = {
   { unsigned(-1), 0 }, { 0, 1 }, { 1, 0 }, { 0, unsigned(-1) }
};

template <typename Plot> struct Farm {
   std::vector<std::vector<Plot>> land;
   Farm(std::istream &is) {
      for (std::string line; getline(is, line); ) {
         land.emplace_back();
         auto &row = land.back();
         std::transform(line.begin(), line.end(),
               std::back_inserter(row), [](const char c) { return Plot(c); });
      }
   }
   Plot &at(const P &p) { return land[p.row][p.col]; }
   bool contains(const P &p) { return p.row < land.size() && p.col < land[p.row].size(); }
};

struct PlotP1 {
   char plant;
   unsigned char visited:1 { false };
   PlotP1(char plant) : plant(plant){}
};

struct Part1 : Farm<PlotP1> {
   Part1( std::istream &is) : Farm{is} {}
   std::pair<unsigned, unsigned> traverse_region(const P &pos) {
      auto &plot = at(pos);
      if (plot.visited)
         return { 0, 0 };
      plot.visited = 1;
      unsigned perimiter = 0;
      unsigned area = 1;
      for (auto dir : direction_velocities) {
         P nextpos = pos + dir;
         if (!contains(nextpos)) {
            ++perimiter;
            continue;
         }
         auto &nextplot = at(nextpos);
         if (nextplot.plant != plot.plant) {
            ++perimiter;
            continue;
         }
         auto [ narea, nperim ] = traverse_region( nextpos );
         area += narea;
         perimiter += nperim;
      }
      return {area, perimiter};
   }

   unsigned solve() {
      unsigned long total = 0;
      for (unsigned row = 0; row < land.size(); ++row) {
         for (unsigned col = 0; col < land[row].size(); ++col) {
            P point = {row, col};
            auto [area, perimiter] = traverse_region(point);
            if (area)
               total += area * perimiter;
         }
      }
      return total;
   }
};
aoc::Case part1{ "part1", [](std::istream &is, std::ostream &os) { os << Part1{is}.solve(); }};

struct PlotP2 {
   char plant;
   unsigned char borders{};
   unsigned region_id{std::numeric_limits<unsigned>::max()};
   PlotP2(char plant) : plant(plant){}
};

struct Part2 : Farm<PlotP2> {
   using Plot = PlotP2;
   struct Region {
      unsigned area{};
      unsigned walls{};
   };
   std::vector<Region> regions;
   Part2(std::istream &is) : Farm{is} {}

   unsigned traverse_region(const P &pos, size_t region_id) {
      auto &plot = at(pos);
      if (plot.region_id != std::numeric_limits<unsigned>::max())
         return 0;
      plot.region_id = region_id;
      unsigned area = 1;
      for (auto dir : { NORTH, EAST, SOUTH, WEST } ) {
         P nextpos = pos + direction_velocities[ dir ];
         if (!contains(nextpos)) {
            plot.borders |= 1 << dir;
            continue;
         }
         auto &nextplot = at(nextpos);
         if (nextplot.plant != plot.plant) {
            plot.borders |= 1 << dir;
            continue;
         }
         area += traverse_region( nextpos, region_id );
      }
      return area;
   }

   unsigned solve() {
      for (unsigned row = 0; row < land.size(); ++row)
         for (unsigned col = 0; col < land[row].size(); ++col)
            if (auto area = traverse_region({row,col}, regions.size()); area != 0)
               regions.push_back({.area=area});

      auto iterate = [this]( const Plot *&prev, unsigned row, unsigned col, Direction d) {
         const auto *cur = &at({row, col});
         if (cur->borders & (1<<d) &&
               (prev == nullptr ||  prev->region_id != cur->region_id || (prev->borders & 1 << d) == 0 ))
            regions[cur->region_id].walls++;
         prev = cur;
      };

      // do the east-west rows, checking for contiguous walls.
      for (unsigned row = 0; row < land.size(); ++row) {
         for (Direction d: {NORTH, SOUTH}) {
            const Plot *prev = nullptr;
            for (unsigned col = 0; col < land[0].size(); ++col)
               iterate(prev, row, col, d);
         }
      }

      // do the north-south cols, checking for contiguous walls.
      for (unsigned col = 0; col < land[0].size(); ++col) {
         for (Direction d: {EAST, WEST}) {
            const Plot *prev = nullptr;
            for (unsigned row = 0; row < land.size(); ++row)
               iterate(prev, row, col, d);
         }
      }

      return std::accumulate(regions.begin(), regions.end(), 0UL,
            [](unsigned long l, const Region &r) { return r.walls * r.area + l; });
   }
};
aoc::Case part2{ "part2", [](std::istream &is, std::ostream &os) { os << Part2{is}.solve(); }};
}
