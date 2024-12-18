#include "aoc.h"

namespace {
using Int = unsigned long;
using WORD = unsigned char; // our 3-bit word fits in this.
struct CPU {
   enum Opcode { adv, bxl, bst, jnz, bxc, out, bdv, cdv };
   Int PC{}, A{}, B{}, C{};

   CPU(std::istream &is) {
      std::string line;
      std::getline(is, line);
      std::string_view rest = line;
      aoc::parsetoken<aoc::Ignore>(rest, "Register A: ");
      A = aoc::parsetoken<Int>(rest, "\n");
      std::getline(is, line);
      rest = line;
      aoc::parsetoken<aoc::Ignore>(rest, "Register B: ");
      B = aoc::parsetoken<Int>(rest, "\n");
      std::getline(is, line);
      rest = line;
      aoc::parsetoken<aoc::Ignore>(rest, "Register C: ");
      C = aoc::parsetoken<Int>(rest, "\n");
   }

   Int comboval(WORD opand) {
      switch (opand) {
         case 0 ... 3: return opand;
         case 4: return A;
         case 5: return B;
         case 6: return C;
         default: abort();
      }
   }

   template <typename Out> void run(const std::vector<WORD> &program, Out output) noexcept {
      while (PC != program.size()) {
         auto opcode = Opcode(program[PC++]);
         WORD arg = program[PC++];
         switch (opcode) {
            case adv: A >>=  comboval(arg);
               break;
            case bxl:
               B ^= arg;
               break;
            case bst:
               B = comboval(arg) % 8;
               break;
            case jnz:
               if (A != 0)
                  PC = arg;
               break;
            case bxc:
               B ^= C;
               break;
            case out:
               output(comboval(arg) % 8);
               break;
            case bdv:
               B = A / (1 << comboval(arg));
               break;
            case cdv:
               C = A / (1 << comboval(arg));
               break;
         }
      }
   }
};

struct State { // the input file.
   CPU cpu;
   std::vector<WORD> program;

   State(std::istream &is) noexcept : cpu(is) {
      std::string line;
      std::getline(is, line); // blank line
      std::getline(is, line); // the program itself.
      std::string_view rest = line;
      for (aoc::parsetoken<aoc::Ignore>(rest, "Program: "); rest != "";) {
         auto c = aoc::parsetoken<unsigned int>(rest, ",");
         program.push_back(static_cast<unsigned char>(c));
      }
   }
};

struct Part1 {
   State machine;

   Part1(std::istream &is) noexcept : machine(is) {}
   void solve(std::ostream &os) {
      int count = 0;
      machine.cpu.run(machine.program, [&] (Int w) { os << (count++ != 0 ? "," : "") << w; });
   }
};

aoc::Case part1{"part1", [](std::istream &is, std::ostream &os) { Part1(is).solve(os); } };

struct Part2 {
   State machine;

   Part2(std::istream &is) noexcept : machine(is) { }
   std::vector<WORD> run_with_A(Int aval) {
      CPU copy = machine.cpu;
      copy.A = aval;
      std::vector<WORD> output;
      copy.run(machine.program, [&] (Int w) { output.push_back(w); return true; });
      return output;
   }

   Int find_prefix(Int generator, unsigned progress) {
      for (Int i = 0; i < 8; ++i) {
         Int candidate_A = generator | i;
         auto out = run_with_A(candidate_A);
         if (out.size() == progress && std::equal(out.begin(), out.end(), machine.program.end() - progress)) {
            if (progress == machine.program.size())
               return candidate_A;
            if (Int soln = find_prefix(candidate_A << 3, progress + 1); soln != 0)
               return soln;
         }
      }
      return 0;
   }
   void solve(std::ostream &os) { os << find_prefix(0, 1); }
};
aoc::Case part2{"part2", [](std::istream &is, std::ostream &os) { Part2(is).solve(os); } };
}
