#include "../aoc.h"
namespace {

struct Parser {
   using Integer = int; // the integer type we calculate with. Everything fits in 'int'
   enum State {
      ident,            // epxecting first/next char of an identifier.
      firstarg,         // expecting the first char in the first arg.
      notfirstarg,      // expecting the first digit in the second or successive arg.
      argrest,          // expecting the second or subsequent digit in an arg.
   };

   static const int maxargs = 2;
   Integer total {}; // accumulator for the answer.
   Integer args[maxargs]; // arguments to the instruction
   int argidx {}; // current argument being calculated.
   int toknext {}; // next element in tokbuf to update.
   bool enabled { true };
   char tokbuf[5]{}; // last N characters. enough to store 'don't', our longest ident.

   // Check if the token buffer ends with 'tok'
   bool tokcmp(std::string_view tok) {
      size_t off = toknext + sizeof tokbuf - 1;
      for (auto e = tok.size();  e-- != 0; )
         if (tokbuf[off-- % sizeof tokbuf] != tok[e])
            return false;
      return true;
   }

   template <typename Strategy>
   Parser(std::istream &in, const Strategy strategy) {
      char c;
      auto rb = in.rdbuf();
      for (auto state = ident; c=rb->sbumpc(), c!=EOF; ) {
         switch (state) {
            case ident:
ident:
               switch (c) {
                  case '(':
                     argidx = -1;
                     state = firstarg;
                     break;
                  default:
                     tokbuf[toknext++ % (sizeof tokbuf)] = c;
                     break;
               }
               break;

            case firstarg:
               switch (c) {
                  case ')':
                     strategy(*this);
                     state = ident;
                     break;
                  case '0'...'9':
                     args[++argidx] = c - '0';
                     state = argrest;
                     break;
                  default:
                     state = ident;
                     goto ident;
               }
               break;

            case notfirstarg:
               switch (c) {
                  case '0'...'9':
                     args[++argidx] = c - '0';
                     state = argrest;
                     break;
                  default:
                     state = ident;
                     goto ident;
               }
               break;

            case argrest:
               switch (c) {
                  case '0'...'9':
                     args[argidx] = args[argidx] * 10 + c - '0';
                     break;
                  case ',':
                     if (argidx == maxargs - 1) [[unlikely]]
                        state = ident;
                     else
                        state = notfirstarg;
                     break;
                  case ')':
                     strategy(*this);
                     state = ident;
                     break;
                  default:
                     state = ident;
                     goto ident;
               }
               break;
         }
      }
   }
   long solve() const {
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
      } else if (p.enabled && p.argidx == 1 && p.tokcmp("mul")) {
         p.total += p.args[0] * p.args[1];
      }
   }
};

}

aoc::Case part1("part1", [](std::istream &is, std::ostream &os) {
      os << Parser( is, Part1Strategy() ).solve(); });
aoc::Case part2("part2", [](std::istream &is, std::ostream &os) {
      os << Parser( is, Part2Strategy() ).solve(); });
