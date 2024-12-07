#include "aoc.h"

namespace {

using Int = unsigned long long;

#if 0
// this is signficantly slower than the raw array below.
constexpr unsigned long long pow10(size_t n) { return (n == 0) ? 1 : 10 * pow10(n - 1); }
template <size_t Count, size_t ... Indices> Int tenst[Count] = {pow10(Indices)...};
constexpr auto tens = tenst<20, 20>;
#else
static constexpr Int tens[] = {
      1ULL,
      10ULL,
      100ULL,
      1000ULL,
      10000ULL,
      100000ULL,
      1000000ULL,
      10000000ULL,
      100000000ULL,
      1000000000ULL,
      10000000000ULL,
      100000000000ULL,
      1000000000000ULL,
      10000000000000ULL,
      100000000000000ULL,
      1000000000000000ULL,
      10000000000000000ULL,
      100000000000000000ULL,
      1000000000000000000ULL,
      10000000000000000000ULL,
};
#endif

struct Parse {
   struct Eqn {
      Int total;
      std::vector<Int> terms;
      Eqn(std::string_view sv) {
         if (!aoc::parsetoken<std::string_view, Int>(sv, total, ":"))
            throw std::logic_error("parse failed");
         sv = sv.substr(1);
         while (sv.size()) {
            Int i;
            if (!aoc::parsetoken(sv, i))
               throw std::logic_error("parse failed");
            terms.push_back(i);
         }
      }
   };
   std::vector<Eqn> eqns;

   Parse(std::istream &is) noexcept {
      for (std::string line; getline(is, line); ) {
         eqns.emplace_back(line);
      }
   }
   template <typename Accumulate>
   Int solve(std::ostream &, Accumulate exprval) const noexcept {
      return std::accumulate( eqns.begin(), eqns.end(), 0ULL, [exprval]( Int term, const Eqn &eqn) {
            return term + exprval(eqn);} );
   }
   Int solve(std::ostream &os) const noexcept {
      return solve(os, [](auto &) { return 0; }) ;
   }


};

struct Part1 : Parse {
   Part1( std::istream &is) : Parse( is ) {}
   static Int total_if_solvable( const Eqn &eqn ) {
      Int limit = 1 << (eqn.terms.size() - 1);
      for (Int i = 0; i < limit; ++i) {
         auto start = eqn.terms.begin();
         Int thistot = *start++;
         auto end = eqn.terms.end();
         for (Int mask = i;  (start != end); mask >>= 1) {
            if (mask & 1)
               thistot *= *start++;
            else
               thistot += *start++;
         }
         if (thistot == eqn.total)
            return thistot;
      }
      return 0;
   }
   Int solve(std::ostream &os) const noexcept { return Parse::solve(os, total_if_solvable) ; }
};

struct Part2 : Parse {
   Part2(std::istream &is) : Parse(is) {}
   static int digits(Int value) {
      Int ceiling = 9;
      int dig;
      for (dig = 1; value > ceiling && dig < 20; ++dig)
         ceiling = ceiling * 10 + 9;
      return dig;
   }

   static Int concat(Int l, Int r) {
      int rdigits = digits(r);
      return l * tens[rdigits] + r;
   }

   static Int total_if_solvable( const Eqn &eqn ) {
      Int combos = int(pow(3, eqn.terms.size() - 1));
      for (Int i = 0; i < combos; ++i) {
         auto start = eqn.terms.begin();
         Int thistot = *start++;
         auto end = eqn.terms.end();
         for (Int combo = i;  (start != end); combo /= 3) {
            switch (combo % 3) {
               // XXX: these should all be checked for overflow.
               case 0: thistot *= *start++; break;
               case 1: thistot += *start++; break;
               case 2: thistot = concat(thistot, *start++); break;
            }
            if (thistot > eqn.total)
               break;
         }
         if (thistot == eqn.total)
            return thistot;
      }
      return 0;
   }
   Int solve(std::ostream &os) const noexcept { return Parse::solve(os, total_if_solvable) ; }

};
}

aoc::Case part1("part1", [](std::istream &is, std::ostream &os) {os << Part1( is ).solve(std::cout);});
aoc::Case part2("part2", [](std::istream &is, std::ostream &os) {os << Part2( is ).solve(std::cout);});
aoc::Case parse("parse", [](std::istream &is, std::ostream &os) {Parse{ is }.solve(os);});
