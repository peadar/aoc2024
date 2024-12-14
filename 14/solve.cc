#include "aoc.h"
#include "aoc.h"
namespace {
using Int = long;

struct Point {
   Int x{}, y{};
   Point operator + (const Point &rhs) const noexcept { return { x + rhs.x, y + rhs.y }; }
};
constexpr Int XSIZE = 101;
constexpr Int YSIZE = 103;

struct Robot {
   Point location, velocity;
};

std::istream &operator>>(std::istream &is, Robot &robot) {
   std::string line;
   std::getline(is, line);
   std::string_view rest = line;
   if (!is) {
      is.setstate(std::ios::failbit);
      return is;
   }
   aoc::parsetoken<aoc::Ignore>(rest, "p=");
   robot.location.x=aoc::parsetoken<Int>(rest, ",");
   robot.location.y=aoc::parsetoken<Int>(rest, " v=");
   robot.velocity.x=aoc::parsetoken<Int>(rest, ",");
   robot.velocity.y=aoc::parsetoken<Int>(rest, "\n");
   return is;
}

Point move_robot(const Robot &robot, unsigned iters) {
   Point v = robot.velocity;
   if (v.x < 0) v.x = XSIZE + v.x;
   if (v.y < 0) v.y = YSIZE + v.y;
   return { (robot.location.x + v.x * iters) % XSIZE, (robot.location.y + v.y * iters) % YSIZE };
}

std::array<unsigned, 4> quads(const std::vector<Robot> &robots, unsigned iterations) {
   std::array<unsigned, 4> quads{};
   for (auto &r : robots) {
      auto p = move_robot(r, iterations);
      if (p.x == XSIZE/2 || p.y == YSIZE/2)
         continue;
      ++quads[(p.x < XSIZE/2 ? 0 : 1) + (p.y < YSIZE/2 ? 0 : 2 )];
   }
   return quads;
};

void part1(std::istream &is, std::ostream &os) {
   std::vector<Robot> robots;
   std::copy(std::istream_iterator<Robot>(is), std::istream_iterator<Robot>(), std::back_inserter(robots));
   auto q  = quads(robots, 100);
   os << std::accumulate(q.begin(), q.end(), 1UL, std::multiplies{});
}
aoc::Case case1{ "part1", part1 };

void display(std::ostream &os, const std::vector<Robot> &robots, int iters, unsigned quad) {
   unsigned pic[XSIZE][YSIZE] {};
   for (const auto &r : robots) {
      auto p = move_robot(r, iters);
      ++pic[p.x][p.y];
   }
   for (size_t y = 0; y < YSIZE/2; ++y) {
      for (size_t x = 0; x < XSIZE/2; ++x) {
         const char elts[] = " 123456789X";
         auto count = pic[x + XSIZE/2 *(quad % 2)][y + YSIZE/2 * (quad / 2)];
         os.rdbuf()->sputc(count < sizeof elts  ? elts[count] : 'X');
      }
      os.rdbuf()->sputc('\n');
   }
   os << iters << "\n";
}

void part2(std::istream &is, std::ostream &os) {
   std::vector<Robot> robots;
   std::copy(std::istream_iterator<Robot>(is), std::istream_iterator<Robot>(), std::back_inserter(robots));
   unsigned best_guess_score = 0;
   unsigned best_guess = 0;
   unsigned best_quad = 0;
   for (unsigned i = 0; i < XSIZE * YSIZE; ++i) {
      unsigned low = std::numeric_limits<unsigned>::max();
      unsigned high = 0;
      auto qs = quads(robots, i);
      for (auto q : qs) {
         low = std::min(q, low);
         high = std::max(q, high);
      }
      unsigned score = high - low;
      if (score > best_guess_score) {
         best_guess_score = score;
         best_guess = i;
         best_quad = std::max_element(qs.begin(), qs.end()) - qs.begin();
      }
   }
   display(os, robots, best_guess, best_quad );
}
aoc::Case case2{ "part2", part2 };
}
