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
      for (std::string line; getline(is, line);)
         eqns.emplace_back(line);
   }
   template <typename Accumulate> Int accumulate(Accumulate exprval) const noexcept {
      return std::accumulate( eqns.begin(), eqns.end(), 0ULL, [exprval]( Int term, const Eqn &eqn) {
            return term + exprval(eqn);} );
   }
   template <typename T> static Int total_if_solvable( const Eqn &eqn ) noexcept {
      return total_if_solvable_r<T>( eqn, eqn.terms[0], 1) ? eqn.total : 0;
   }

   template <typename T> static bool total_if_solvable_r( const Eqn &eqn, Int accum, unsigned idx ) noexcept {
      if (idx == eqn.terms.size())
         return accum == eqn.total;
      return T::template total_if_solvable_part<T>( eqn, accum, idx);
   }
};

struct Part1 : Parse {
   Part1( std::istream &is) : Parse( is ) {}
   template <typename T> static bool total_if_solvable_part( const Eqn &eqn, Int accum, unsigned idx ) noexcept {
      return total_if_solvable_r<T>( eqn, accum + eqn.terms[idx], idx + 1) ||
         total_if_solvable_r<T>( eqn, accum * eqn.terms[idx], idx + 1);
   }
   Int solve() const noexcept { return accumulate(total_if_solvable<Part1>) ; }
};

struct Part2 : Parse {
   Part2(std::istream &is) : Parse(is) {}
   static Int concat(Int l, Int r) noexcept {
      for (Int digits = r; digits; digits /= 10)
         l *= 10;
      return l + r;
   }
   template<typename T>
   static bool total_if_solvable_part( const Eqn &eqn, Int accum, unsigned idx ) noexcept {
      return (Part1::total_if_solvable_part<T>(eqn, accum, idx) ||
            total_if_solvable_r<T>(eqn, concat(accum, eqn.terms[idx]), idx + 1));
   }
   Int solve() const noexcept { return accumulate(total_if_solvable<Part2>) ; }
};

aoc::Case part1("part1", [](std::istream &is, std::ostream &os) {os << Part1( is ).solve();});
aoc::Case part2("part2", [](std::istream &is, std::ostream &os) {os << Part2( is ).solve();});
aoc::Case parse("parse", [](std::istream &is, std::ostream &) {Parse { is };});
