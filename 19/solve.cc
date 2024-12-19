#include "aoc.h"
namespace {
struct TrieNode;
struct NodePtr : std::unique_ptr<TrieNode> { inline NodePtr(); };
struct TrieNode {
   std::unordered_map<char, NodePtr> arcs;
   bool istok{false};
};
NodePtr::NodePtr() : std::unique_ptr<TrieNode>(new TrieNode) {}

struct Trie {
   TrieNode root;
   void insert(std::string_view sv) noexcept {
      TrieNode *cur = &root;
      for (char c : sv)
         cur = cur->arcs[c].get();
      cur->istok = true;
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

unsigned long solve_pattern(const Trie &flags, std::string_view remaining_pattern) {
   using Paths = std::unordered_map<const TrieNode *, unsigned long long>;
   Paths in;
   in[&flags.root] = 1;
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
   return std::accumulate(in.begin(), in.end(), 0ULL, [](unsigned long acc, auto item) {
         return item.first->istok ? acc + item.second : acc; });
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
