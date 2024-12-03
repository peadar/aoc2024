#include "aoc.h"
namespace {
struct tokmask {
   // We deal with up to 8-character tokens. The problem uses up to 5, for "don't"
   // but we also encode each argument as a \1 in the token so we can count the
   // args as well as the name of the token (a primitive form of
   // name-mangling). We still need at most 5 chars, as only "mul" takes args,
   // and it takes 2, and is thus parsed as 'mul\1\1' in the input.
   uint64_t data{};
   uint64_t mask{};
   constexpr bool operator == ( uint64_t tok ) const { return data == (mask & tok); }
   consteval tokmask(std::string_view tok) {
      for (auto c : tok) {
         data = (data << 8) | (uint64_t(c) & 0xff);
         mask = (mask << 8) | 0xff;
      }
   }
};

struct Parser {
   using Integer = unsigned; // the integer type we calculate with
   enum State {         // States:
      start,            // restart the parser (clear current token, continue to "ident")
      ident,            // expecting first/next char of an identifier (or '(' to indicate call invocation)
      firstarg,         // expecting the first char in the first arg (or ')' for 0 arg)
      secondarg,      // expecting the first digit in the second or successive arg.
      argrest,          // expecting the second or subsequent digit in an arg (or , or ) to terminate the arg)
   };

   static const int maxargs = 2; // max args we need to deal with.
   Integer args[maxargs] {}; // arguments to the instruction
   Integer total {}; // accumulator for the answer.

   template <typename Strategy>
   Parser(std::istream &in, Strategy strategy) {
      unsigned argidx{}; // current argument being calculated.
      int c;
      uint64_t token{};
      std::streambuf * rb = in.rdbuf();

      for (auto state = start; c=rb->sbumpc(), c!=EOF; ) {
         switch (state) {
            case start:
restart:
               token = 0;
               state = ident;
               // fallthrough
            case ident:
               switch (c) {
                  default:
                     token = (token << 8) | c;
                     break;
                  case '(':
                     argidx = -1;
                     state = firstarg;
                     break;
               }
               break;
            case firstarg:
               switch (c) {
                  case '0'...'9':
                     args[++argidx] = c - '0';
                     token = (token << 8) | 1;
                     state = argrest;
                     break;
                  case ')':
                     strategy(*this, token);
                     state = start;
                     break;
                  default:
                     state = start;
                     goto restart; // this is marginally, but measurably, faster than rb->sungetc(); continue;
               }
               break;
            case secondarg: // same as previous state, but closing bracket not allowed (as it follows a comma )
               switch (c) {
                  case '0'...'9':
                     args[++argidx] = c - '0';
                     token = (token << 8) | 1;
                     state = argrest;
                     break;
                  default:
                     state = start;
                     goto restart;
               }
               break;

            case argrest:
               switch (c) {
                  case '0'...'9':
                     args[argidx] = args[argidx] * 10 + c - '0';
                     break;
                  case ',':
                     if (argidx == maxargs - 1) [[unlikely]] {
                        state = start; // too many args - restart parsing.
                     } else {
                        state = secondarg; // move to second (or subsequent?) arg.
                     }
                     break;
                  case ')':
                     strategy(*this, token);
                     state = start;
                     break;
                  default:
                     state = start;
                     goto restart;
               }
               break;
         }
      }
   }
   constexpr Integer solve() const {
      return total;
   }
};

constinit tokmask mul_tok( "mul\1\1" ), do_tok( "do" ), dont_tok( "don't" );

struct Part1Strategy {
   constexpr void operator()(Parser &p, uint64_t token) {
      if (mul_tok == token)
         p.total += p.args[0] * p.args[1];
   }
};

struct Part2Strategy {
   bool enabled{ true };
   constexpr void operator()(Parser &p, uint64_t token) {
      if (mul_tok == token) {
         if (enabled)
            p.total += p.args[0] * p.args[1];
      } else if (dont_tok == token) {
         enabled = false;
      } else if (do_tok == token) {
         enabled = true;
      }
   }
};

}
aoc::Case part1("part1", [](std::istream &is, std::ostream &os) {os << Parser( is, Part1Strategy() ).solve();});
aoc::Case part2("part2", [](std::istream &is, std::ostream &os) {os << Parser( is, Part2Strategy() ).solve();});
