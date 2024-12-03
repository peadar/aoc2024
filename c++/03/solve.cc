#include "../aoc.h"
#include <ctype.h>
namespace {

struct Parser {
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

   template <typename Strategy>
   Parser(std::istream &in, const Strategy strategy) {
      char c;
      for (auto state = wait_oparen; c=in.get(), in; ) {
         switch (state) {
            case wait_oparen:
oparen:
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
                     strategy(*this);
                  state = wait_oparen;
                  goto oparen;
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
                     strategy(*this);
                  argidx = -1;
                  state = wait_oparen;
                  goto oparen;
               }
               break;
         }
      }
   }
   unsigned solve() const {
      return total;
   }
};

struct Part1Strategy {
   void operator()(Parser &p) const {
      if (p.tokcmp("mul"))
         p.total += p.args[0] * p.args[1];
   }
};

struct Part2Strategy {
   void operator()(Parser &p) const {
      if (p.tokcmp("don't")) {
         p.enabled = false;
      } else if (p.tokcmp("do")) {
         p.enabled = true;
      } else if (p.tokcmp("mul")) {
         if (p.enabled && p.argidx == 1) {
            p.total += p.args[0] * p.args[1];
         }
      }
   }
};

}

aoc::Case part1("part1", [](std::istream &is, std::ostream &os) {
      os << Parser( is, Part1Strategy() ).solve(); });
aoc::Case part2("part2", [](std::istream &is, std::ostream &os) {
      os << Parser( is, Part2Strategy() ).solve(); });
