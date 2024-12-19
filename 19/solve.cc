#include "aoc.h"
namespace {
struct TrieNode {
   std::unordered_map<char, std::unique_ptr<TrieNode>> arcs;
   bool istok{false};
};

struct Trie {
   TrieNode root;
   void insert(std::string_view sv) {
      TrieNode *cur = &root;
      for (char c : sv) {
         auto &arc = cur->arcs[c];
         if (!arc) 
            arc = std::make_unique<TrieNode>();
         cur = arc.get();
      }
      cur->istok = true;
   }
};

struct Input {
   Trie flags;
   std::vector<std::string> patterns;
   Input(std::istream &is) noexcept {
      std::string line;
      getline(is, line);
      std::string_view sv = line;
      while (sv != "")
         flags.insert(aoc::parsetoken<std::string_view>(sv, ", "));
      std::getline(is, line);
      assert(line == "");
      while (std::getline(is, line))
         patterns.push_back(line);
   }
};

unsigned long long solve_pattern(const Trie &flags, std::string_view remaining_pattern) {
   using Paths = std::unordered_map<const TrieNode *, unsigned long long>;
   Paths in;
   in[&flags.root]++;
   for (char c : remaining_pattern) {
      Paths out;
      for (const auto [ node, count ] : in ) {
         auto nexti = node->arcs.find(c);
         if (nexti != node->arcs.end()) {
            auto next = nexti->second.get();
            out[next]+=count;
            if (next->istok)
               out[&flags.root]+=count;
         }
      }
      in = std::move(out);
   }
   unsigned long long count = 0;
   for (auto &[k,v] : in)
      if (k->istok)
         count += v;
   return count;
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
