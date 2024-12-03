#include "aoc.h"
namespace {

enum {
   wait_m,
   wait_u,
   wait_l,
   wait_lparen,
   wait_firstint,
   wait_secondint,
};

struct Part1 {
   long first{}, second{}, total{};

   Part1(std::istream &in) {
      for (auto state = wait_m;; ) {
         char c = in.get();
         if (!in)
            break;
         switch (state) {
            case wait_m:
               if (c == 'm')
                  state = wait_u;
               break;
            case wait_u:
               if (c == 'u')
                  state = wait_l;
               else
                  state = wait_m;
               break;
            case wait_l:
               if (c == 'l')
                  state = wait_lparen;
               else
                  state = wait_m;;
               break;
            case wait_lparen:
               if (c == '(') {
                  first = 0;
                  state = wait_firstint;
               } else {
                  state = wait_m;;
               }
               break;
            case wait_firstint:
               if (isdigit(c)) {
                  first = first * 10 + (c - '0');
               } else if (c == ',') {
                  second = 0;
                  state = wait_secondint;
               } else {
                  state = wait_m;
               }
               break;
            case wait_secondint:
               if (isdigit(c)) {
                  second = second * 10 + (c - '0');
               } else {
                  if (c == ')')
                     total += first * second;
                  state = wait_m;
               }
               break;
         }
      }

   }
   unsigned solve() const {
      return total;
   }
};
struct Part2 {
   Part2(std::istream &) {
   }
   unsigned solve() {
      return 0;
   }
};

}

aoc::Case part1("part1", [](std::istream &is, std::ostream &os) { os << Part1( is ).solve(); });
aoc::Case part2("part2", [](std::istream &is, std::ostream &os) { os << Part2( is ).solve(); });
