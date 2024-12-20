#include <boost/container/small_vector.hpp>
#include "aoc.h"
#include "lintable.h"
namespace {

// We store the arcs of our trie as indexes into a vector, and we only have 5
// potential keys, so we can use an array with an entry for each possible arc.
// Using indexes everywhere instead of pointers makes it possible to avoid
// dynamically allocating nodes. There are no branches back to the root node,
// so 0 is a useable sentinel for "no entry"
//
inline unsigned trie_idx( char c) {
   switch (c) {
      case 'r': return 0;
      case 'g': return 1;
      case 'u': return 2;
      case 'b': return 3;
      case 'w': return 4;
      default: __builtin_unreachable();
   }
}

template <typename T, size_t S>
//using Vec = std::vector<T>; // boost::container::small_vector<T, S>;
using Vec = boost::container::small_vector<T, S>;

// Each node in the trie is a set of arcs out of the trie (some with zero), and
// a flag to say if the node represents a terminal token
struct TrieNode {
   bool istok{false};
   std::array<unsigned short, 5> arcs{};
};

struct Trie {
   std::vector<TrieNode> nodes; // all arcs and node indexes are relative to the base of this vector
   Trie() {
      nodes.reserve(2048);
   }
   void insert(std::string_view sv) noexcept {
      nodes.emplace_back(); // the root node.
      unsigned cur = 0;
      for (char c : sv) {
         unsigned next = nodes[cur].arcs[trie_idx(c)];
         if (next == 0) {
            next = nodes.size();
            nodes.emplace_back();
            nodes[cur].arcs[trie_idx(c)] = next;
         }
         cur = next;
      }
      nodes[cur].istok = true;
   }
};

struct Input {
   Trie flags;
   std::vector<std::string> patterns;
   Input(std::istream &is) noexcept {
      std::string line;
      getline(is, line);
      for (std::string_view sv = line; sv != ""; )
         flags.insert(aoc::parsetoken<std::string_view>(sv, ", "));
      std::getline(is, line);
      while (std::getline(is, line))
         patterns.push_back(line);
   }
};

inline unsigned long solve_pattern(const Trie &flags, std::string_view remaining_pattern) {
   /*
    * Start off with a set containing a single pair with the root node of the
    * trie, and a count of one.
    * For each input character:
    *    for each node, count in the input set
    *       traverse the arc for node, creating an entry in the output set { next(node), count }
    *       If there is no valid transition, the count for that node is dropped.
    *       If the new node represents a "token", then add that to the count of new root nodes.
    *    add the root node to the output set with the accumulated count for the new root node.
    *
    * At each step along the input, the input set's node/count paris track the
    * number of possible paths up to that point that would be at that node in
    * the trie.  When we're done, any count on a node in the final set
    * representing a token node represents a disting possible combination.
    */
   using Pvec = Vec<std::pair<unsigned, unsigned long>, 10>;
   Pvec a, b, *in = &a, *out = &b;
   in->emplace_back(0, 1);
   for (char c : remaining_pattern) {
      unsigned long new_roots = 0;
      for (const auto &[index, count]: *in ) {
         if (unsigned nexti = flags.nodes[index].arcs[trie_idx(c)]; nexti != 0) {
            out->emplace_back(nexti, count);
            if (flags.nodes[nexti].istok)
               new_roots += count;
         }
      }
      out->emplace_back(0, new_roots);
      std::swap(in, out);
      out->clear();
   }
   return std::accumulate(in->begin(), in->end(), 0ULL, [&](unsigned long acc, auto item) {
         return flags.nodes[item.first].istok ? acc + item.second : acc; });
}

void part1(std::istream &in, std::ostream &out) {
   Input data(in);
   out << std::accumulate(data.patterns.begin(), data.patterns.end(), 0ULL,
         [&data](unsigned long acc, const std::string &s) { return solve_pattern(data.flags, s) ? acc + 1 : acc; } );
}
aoc::Case case1{"part1", part1};

void part2(std::istream &in, std::ostream &out) {
   Input data(in);
   out << std::accumulate(data.patterns.begin(), data.patterns.end(), 0ULL,
         [&data](unsigned long acc, const std::string &s) { return solve_pattern(data.flags, s) + acc; } );
}
aoc::Case case2{"part2", part2};
}
