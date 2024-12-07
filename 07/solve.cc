#include "aoc.h"

struct Parse {
   using Int = unsigned long long;
   struct Eqn {
      Int total;
      std::vector<Int> terms;
      Eqn(std::string_view sv) {
         total = aoc::parsetoken<std::string_view, Int>(sv, ": ");
         while (sv.size())
            terms.push_back( aoc::parsetoken<std::string_view, Int>(sv) );
      }
   };
   std::vector<Eqn> eqns;
   Parse(std::istream &is) noexcept {
      eqns.reserve(1000);
      for (std::string line; getline(is, line);)
         eqns.emplace_back(line);
   }
   template <typename Accumulate> Int accumulate(Accumulate exprval) const noexcept {
      return std::accumulate( eqns.begin(), eqns.end(), 0ULL, [exprval]( Int term, const Eqn &eqn) {
            return term + exprval(eqn);} );
   }
};

struct Part1 : Parse {
   Part1( std::istream &is) : Parse( is ) {}
   static Int total_if_solvable( const Eqn &eqn ) {
      unsigned combos = 1 << (eqn.terms.size() - 1);
      for (unsigned combo = 0; combo < combos; ++combo) {
         auto start = eqn.terms.begin();
         Int sofar = *start++;
         auto end = eqn.terms.end();
         for (unsigned mask = combo; start != end && sofar <= eqn.total; mask >>= 1) {
            Int rhs = *start++;
            if ((mask & 1) != 0)
               sofar *= rhs;
            else
               sofar += rhs;
         }
         if (sofar == eqn.total)
            return sofar;
      }
      return 0;
   }
   Int solve() const noexcept { return accumulate(total_if_solvable) ; }
};

struct Part2 : Parse {
   Part2(std::istream &is) : Parse(is) {}
   static Int concat(Int l, Int r) noexcept {
      for (Int digits = r; digits; digits /= 10)
         l *= 10;
      return l + r;
   }
   static Int total_if_solvable( const Eqn &eqn ) noexcept {
      auto p1 = Part1::total_if_solvable(eqn);
      if (p1) // removing all the combos that don't require "concat" first helps a lot.
         return p1;
      unsigned combos = unsigned(pow(3, eqn.terms.size() - 1));
      for (unsigned combo = 0; combo < combos; ++combo) {
         auto start = eqn.terms.begin();
         Int sofar = *start++;
         auto end = eqn.terms.end();
         for (unsigned mask = combo; start != end && sofar <= eqn.total; mask /= 3) {
            Int rhs = *start++;
            switch (mask % 3) {
               // XXX: these should all be checked for overflow.
               case 0: sofar *= rhs; break;
               case 1: sofar += rhs; break;
               case 2: sofar = concat(sofar, rhs); break;
            }
         }
         if (sofar == eqn.total)
            return sofar;
      }
      return 0;
   }
   Int solve() const noexcept { return accumulate(total_if_solvable) ; }
};

aoc::Case part1("part1", [](std::istream &is, std::ostream &os) {os << Part1( is ).solve();});
aoc::Case part2("part2", [](std::istream &is, std::ostream &os) {os << Part2( is ).solve();});
aoc::Case parse("parse", [](std::istream &is, std::ostream &) {Parse { is };});
