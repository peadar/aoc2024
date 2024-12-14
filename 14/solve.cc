#include "aoc.h"
#include "aoc.h"
namespace {
using Int = long;

struct Point {
   Int x{}, y{};
   Point operator + (const Point &rhs) const noexcept { return { x + rhs.x, y + rhs.y }; }
};
struct Robot { Point location, velocity; };
constexpr Int XSIZE = 101;
constexpr Int YSIZE = 103;

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

void display(std::ostream &os, const std::array<std::array<unsigned, XSIZE>, YSIZE> &pic) {
   for (int y = 0; y < YSIZE - 3; y += 4) {
      for (int x = 0; x < XSIZE - 3; x += 2) {
         unsigned braille = 0;
         braille |= (pic[y][x] ? 1 : 0)  << 0;
         braille |= (pic[y+1][x] ? 1 : 0)  << 1;
         braille |= (pic[y+2][x] ? 1 : 0)  << 2;

         braille |= (pic[y][x+1] ? 1 : 0)  << 3;
         braille |= (pic[y+1][x+1] ? 1 : 0) << 4;
         braille |= (pic[y+2][x+1] ? 1 : 0)  << 5;

         braille |= (pic[y+3][x] ? 1 : 0) << 6;
         braille |= (pic[y+3][x+1] ? 1 : 0) << 7;
         os << aoc::Utf8( braille | 0x2800 );
      }
      os << "\n";
   }
}

unsigned tree(const std::vector<Robot> &robots) {
   unsigned best_guess_score = 0;
   unsigned best_guess = 0;
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
      }
   }
   return best_guess;
}

void part2(std::istream &is, std::ostream &os) {
   std::vector<Robot> robots;
   std::copy(std::istream_iterator<Robot>(is), std::istream_iterator<Robot>(), std::back_inserter(robots));
   os << tree( robots );
}

void animate(std::istream &is, std::ostream &os) {
   os << "\n";
   std::vector<Robot> robots;
   std::copy(std::istream_iterator<Robot>(is), std::istream_iterator<Robot>(), std::back_inserter(robots));
   int target = tree(robots);
   auto framerate = std::chrono::nanoseconds(16'666'667); // That's 1/60th sec.
   auto until = std::chrono::high_resolution_clock::now();
   for (size_t iteration = 0; iteration < robots.size(); ++iteration) {
      std::array<std::array<unsigned, XSIZE>, YSIZE> pic{};
      unsigned robotnum = 0;
      for (const auto &r : robots) {
         auto p = move_robot(r, robotnum <= iteration ? target : 0 );
         ++pic[p.y][p.x];
         ++robotnum;
      }
      std::cout << "\033[H\033[2J\033[3J";
      display(os, pic);
      std::cout.flush();

      until += framerate;
      std::this_thread::sleep_until(until);
   }
}

void testchart(std::istream &is, std::ostream &os) {
   std::cout << "\n";
   std::array<std::array<unsigned, XSIZE>, YSIZE> pic{};
   for (int x = 0; x < XSIZE; x += 2) {
      for (int y = 0; y < YSIZE; y += 1) {
         pic[x][y] = 1;
         pic[x+1][y] = 0;
      }
   }
   display(os, pic);
   std::cout.flush();
}

aoc::Case case3{ "animate", animate };
aoc::Case case1{ "part1", part1 };
aoc::Case case2{ "part2", part2 };
}
