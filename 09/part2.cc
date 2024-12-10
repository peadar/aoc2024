#include "aoc.h"
namespace {
struct File  { unsigned id; unsigned offset; unsigned size; };

struct Input {
   std::vector<File> files; // offset->file
   std::map<unsigned, unsigned> freespace; // freespace, key is *end* of the space.
   Input(std::istream &is) {
      unsigned type = 0;
      unsigned offset = 0;
      unsigned id = 0;
      for ( int c; (c = is.rdbuf()->sbumpc() ) != EOF;) {
         switch (c) {
            case '\n': break;
            case '0'...'9': {
               unsigned size = c - '0';
               switch (type++ % 2) {
                  case 0: // file
                     files.push_back( {id++, offset, size} );
                     break;
                  case 1: // free space.
                     freespace[offset + size] = size;
                     break;
               }
               offset += size;
               break;
            }
            default: abort(); break;
         }
      }
   }
};

unsigned long solve(Input &&input) noexcept {
   for (auto ifile = input.files.rbegin(); ifile != input.files.rend(); ++ifile) {
      for (auto ifree = input.freespace.begin(); ifree != input.freespace.end(); ++ifree) {
         auto &[offset, space_avail] = *ifree;
         if (offset - space_avail >= ifile->offset)
            break;
         if (space_avail >= ifile->size) {
            ifile->offset = offset - space_avail;
            space_avail -= ifile->size;
            if (space_avail == 0)
               input.freespace.erase(ifree);
            break;
         }
      }
   }
   unsigned long total = 0;
   for (auto &file : input.files)
      total += file.id * ( file.size * file.offset + file.size * (file.size - 1) / 2 );
   return total;
}
}
aoc::Case part2("part2", [](std::istream &is, std::ostream &os) { os << solve(Input{is});});
