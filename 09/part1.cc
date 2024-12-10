#include "aoc.h"
namespace {
using Total = unsigned long;
using Datum = unsigned char;
struct Input {
   std::vector<Datum> data;
   Input(std::istream &is) {
      for ( int c; (c = is.rdbuf()->sbumpc() ) != EOF;) {
         switch (c) {
            case '\n': break;
            case '0'...'9': data.push_back(c - '0'); break;
            default: abort(); break;
         }
      }
   }
};

void advance(Total &total, unsigned &pos, unsigned id, unsigned count) noexcept {
   total += id * ( count * pos + count * (count - 1) / 2 );
   pos += count;
}

Total solve(const Input &input) noexcept {
   assert(input.data.size() != 0);
   auto head = input.data.begin();
   auto tail = input.data.end();
   if (input.data.size() % 2 == 0) // ignore empty space at the end of the set
      --tail;
   unsigned endfile_sizeleft = 0;
   unsigned endfile_id = (tail - head) / 2 + 1;
   Total total{};
   for (unsigned cur_fileid = 0, pos = 0;;) {
      advance(total, pos, cur_fileid++, *head++);
      for (unsigned freechunk = head == tail ? std::numeric_limits<unsigned>::max() : *head++; freechunk != 0;) {
         if (endfile_sizeleft == 0) {
            if (head == tail)
               return total;
            endfile_sizeleft = *--tail;
            --endfile_id;
            if (!input.data.empty()) // remove trailing free block.
               --tail;
         }
         unsigned chunk = std::min(endfile_sizeleft, freechunk);
         advance(total, pos, endfile_id, chunk);
         freechunk -= chunk;
         endfile_sizeleft -= chunk;
      }
   }
}
}

aoc::Case part1("part1", [](std::istream &is, std::ostream &os) { os << solve(Input{is}); });
