#include "aoc.h"
namespace {
using Int = long;
union Eqn {
   struct { Int a, b, c; };
   Int vals[3]{};
};

template <size_t adjust> struct WorkItem {
   Eqn eqns[2]{};
   Int solve();
};

template <size_t adjust> std::istream &operator >> (std::istream &is, WorkItem<adjust> &item) {
   if (is.peek() == EOF) {
      is.setstate(std::ios::failbit);
      return is;
   }
   std::string line;
   std::string_view tok;
   std::string_view rest;
   for (int i = 0; i < 2; ++i) {
      std::getline(is, line);
      rest = line;
      aoc::parsetoken<aoc::Ignore>(rest, " ");
      aoc::parsetoken<aoc::Ignore>(rest, ": ");
      aoc::parsetoken<aoc::Ignore>(rest, "+");
      item.eqns[0].vals[i] = aoc::parsetoken<Int>(rest, ", Y+");
      item.eqns[1].vals[i] = aoc::parsetoken<Int>(rest, "\n");
   }
   std::getline(is, line);
   rest = line;
   aoc::parsetoken<aoc::Ignore>(rest, ": X=");
   item.eqns[0].c = aoc::parsetoken<Int>(rest, ", Y=") + adjust;
   item.eqns[1].c = aoc::parsetoken<Int>(rest, "\n") + adjust;
   if (is.peek() == '\n')
      std::getline(is, line);
   return is;
}

template <size_t adjust> Int solve(std::istream &is) {
      return std::accumulate( std::istream_iterator<WorkItem<adjust>>(is),
            std::istream_iterator<WorkItem<adjust>>(), Int(0),
            [](Int acc, WorkItem<adjust> item) { return acc + item.solve(); });
}

template <size_t adjust> Int WorkItem<adjust>::solve() {
   // We have two equations in two unknowns. Solve for the first variable by
   // cancelling the second.  (I'm sure some matrices and linear algebra can do
   // thise way better - this is me tracing what I did on paper.
   Int tmp = eqns[0].b;
   Eqn tmpeqn = eqns[0];
   for (auto &i : eqns[0].vals)
      i *= eqns[1].b;
   for (auto &i : eqns[1].vals)
      i *= tmp;
   assert(eqns[0].b == eqns[1].b);
   Int c = eqns[0].c  - eqns[1].c;
   Int a = eqns[0].a - eqns[1].a;
   if (c % a != 0)
      return 0;
   Int atimes = c / a;
   Int btimes = tmpeqn.c - tmpeqn.a * atimes;
   if (btimes % tmpeqn.b != 0)
      return 0;
   btimes /= tmpeqn.b;
   return atimes * 3 + btimes;
}

aoc::Case part1{ "part1", [](std::istream &is, std::ostream &os) { os << solve<0>(is); }};
aoc::Case part2{ "part2", [](std::istream &is, std::ostream &os) { os << solve<10'000'000'000'000>(is); }};
}
