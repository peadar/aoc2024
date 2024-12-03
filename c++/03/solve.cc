#include "aoc.h"
namespace {

struct Part1 {
   enum {
      wait_m,
      wait_u,
      wait_l,
      wait_lparen,
      wait_firstint,
      wait_secondint,
   };

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
   enum {
      wait_oparen,
      wait_argstart,
      wait_argcont,
   };
   long args[2];
   long argidx {};
   long total {};
   char tokbuf[5]; // enough to store 'don't'
   int toknext {};
   bool enabled { true };


   bool tokcmp(const char *p) {
      size_t off = toknext + sizeof tokbuf - 1;
      for (auto e = p + strlen(p);  e-- != p;) {
         if (tokbuf[off-- % sizeof tokbuf] != *e)
            return false;
      }
      return true;
   }

   void execute() {
      if (tokcmp("don't")) {
         enabled = false;
      } else if (tokcmp("do")) {
         enabled = true;
      } else if (tokcmp("mul")) {
         if (argidx == 1) {
            total += args[0] * args[1];
         }
      }
   }

   Part2(std::istream &in) {
      for (auto state = wait_oparen;; ) {
         char c = in.get();
         if (!in)
            break;
         switch (state) {
            case wait_oparen:
               if (c == '(') {
                  argidx = -1;
                  state = wait_argstart;
               } else {
                  tokbuf[toknext++ % (sizeof tokbuf)] = c;
               }
               break;

            case wait_argstart:
               if (isdigit(c)) {
                  args[++argidx] = 0;
                  state = wait_argcont;
                  goto argcont;
               } else {
                  if ( c == ')' && argidx == -1)
                     execute();
                  state = wait_oparen;
               }
               break;
argcont:
            case wait_argcont:
               if (isdigit(c)) {
                  args[argidx] = args[argidx] * 10 + c - '0';
               } else if (c == ',') {
                  state = wait_argstart;
               } else {
                  if (c == ')')
                     execute();
                  argidx = -1;
                  state = wait_oparen;
               }
               break;
         }
      }
   }
   unsigned solve() const {
      return total;
   }
};


}

aoc::Case part1("part1", [](std::istream &is, std::ostream &os) { os << Part1( is ).solve(); });
aoc::Case part2("part2", [](std::istream &is, std::ostream &os) { os << Part2( is ).solve(); });
