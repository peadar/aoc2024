#include "aoc.h"

namespace {
struct Parse {
   enum {
      X = 0,
      M = 1,
      A = 2,
      S = 3
   };
   std::vector<uint8_t> grid;
   size_t cols{};
   size_t rows{};
   size_t log2cols{};

   void addline(std::string &&val) {
         ++rows;
         /*
         int mod =0;
         uint8_t field = 0;
         for (auto c : val) {
            switch (c) {
               case 'X': field = (field << 2 | X);
               case 'M': field = (field << 2 | M);
               case 'A': field = (field << 2 | A);
               case 'S': field = (field << 2 | S);
               default: throw std::logic_error("unexpected char in input");

            }
         }
         */
         std::copy(val.begin(), val.end(), back_inserter(grid));
         /*
         for (size_t i = cols; i < 1U << log2cols; ++i)
            grid.push_back(0);
            */
   }

   Parse( std::istream &is) {
      std::string line;
      if (!getline(is, line)) {
         throw std::logic_error("input is empty?");
      }
      cols = line.size();
      for (log2cols = 0; 1U << log2cols < cols; log2cols++)
         ;
      grid.reserve((1U<<log2cols) * cols);
      addline(std::move(line));
      for (std::string line; getline(is, line); ) {
         addline(std::move(line));
      }
   }
   int solve() {
      return 0;
   }
   char at(size_t row, size_t col) {
      return grid[(row *cols) + col];
   }
};

struct Part1 : Parse {
   Part1( std::istream &is) : Parse( is ) {}
   int solve() {
      int count = 0;
      for (size_t row = 0; row < rows; ++row) {
         for (size_t col = 0; col < cols; ++col) {
            for (int dr = -1; dr <= 1; ++dr) {
               for (int dc = -1; dc <= 1; ++dc) {
                  if (dc == 0 && dr == 0)
                     continue; // no progress in either direction.
                  size_t wordrow = row, wordcol = col;
                  for (auto c : {'X','M','A','S'}) {
                     if (wordrow >= rows || wordcol >= cols)
                        goto nope;
                     if (at(wordrow,wordcol) != c)
                        goto nope;
                     wordrow += dr;
                     wordcol += dc;
                  }
                  count++;
nope:;
               }
            }
         }
      }
      return count;
   }
};

struct Part2 : Parse {
   Part2(std::istream &is) : Parse(is) {}
   bool word(char a, char b) {
      return (a == 'M' && b == 'S' ) || ( a == 'S' && b == 'M' );
   }
   int solve() {
      int count = 0;
      for (size_t row = 1; row < rows - 1; ++row) {
         for (size_t col = 1; col < cols - 1; ++col) {
            if (at(row,col) == 'A' && word(at(row-1,col-1), at(row+1,col+1)) && word(at(row-1,col+1), at(row+1,col-1))) {
                  ++count;
            }
         }
      }
      return count;
   }
};

}


aoc::Case part1("part1", [](std::istream &is, std::ostream &os) {os << Part1( is ).solve();});
aoc::Case part2("part2", [](std::istream &is, std::ostream &os) {os << Part2( is ).solve();});
aoc::Case parse("parse", [](std::istream &is, std::ostream &os) {os << Parse( is ).solve();});
