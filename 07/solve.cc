#include "aoc.h"
using Int = unsigned long long;
struct Eqn {
   Int total;
   std::vector<Int> terms;
   Eqn(std::string_view sv) {
      total = aoc::parsetoken<std::string_view, Int>(sv, ": ");
      while (sv.size() != 0)
         terms.push_back(aoc::parsetoken<std::string_view, Int>(sv));
   }
   template <typename T> bool is_solvable(Int accum, auto iter) const noexcept {
      return iter == terms.end() ? accum == total : T::template is_solvable<T>(*this, accum, iter);
   }
   template <typename T> Int value() const noexcept { return is_solvable<T>(terms[0], terms.begin() + 1) ? total : 0; }
};

struct Equations {
   std::vector<Eqn> eqns;
   Equations(std::istream &is) {
      for (std::string line; getline(is, line);)
         eqns.emplace_back(line);
   }
   template <typename T> Int solve() const noexcept {
      return std::accumulate(eqns.begin(), eqns.end(), 0ULL,
            [](Int term, const Eqn &eqn) { return term + eqn.value<T>(); });
   }
};

struct Part1 : Equations {
   Part1(std::istream &is) : Equations(is) {}
   template <typename T> static bool is_solvable(const Eqn &eqn, Int accum, auto iter) noexcept {
      return eqn.is_solvable<T>(accum + *iter, iter + 1) || eqn.is_solvable<T>(accum * *iter, iter + 1);
   }
};

struct Part2 : Equations {
   Part2(std::istream &is) : Equations(is) {}
   static Int concat(Int l, Int r) noexcept {
      for (Int digits = r; digits; digits /= 10)
         l *= 10;
      return l+r;
   }
   template<typename T> static bool is_solvable(const Eqn &eqn, Int accum, auto iter) noexcept {
      return (Part1::is_solvable<T>(eqn, accum, iter) || eqn.is_solvable<T>(concat(accum, *iter), iter + 1));
   }
};
aoc::Case part1("part1", [](std::istream &is, std::ostream &os) {os << Part1( is ).solve<Part1>();});
aoc::Case part2("part2", [](std::istream &is, std::ostream &os) {os << Part2( is ).solve<Part2>();});
aoc::Case parse("parse", [](std::istream &is, std::ostream &) {Equations { is };});
