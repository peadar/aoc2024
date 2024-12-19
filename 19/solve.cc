#include "aoc.h"
#include "lintable.h"
namespace {

unsigned trie_idx( char c) {
   switch (c) {
      case 'r': return 0;
      case 'g': return 1;
      case 'u': return 2;
      case 'b': return 3;
      case 'w': return 4;
      default: abort();
   }
}

struct TrieNode;
struct NodePtr : std::unique_ptr<TrieNode> { inline NodePtr(); };
struct TrieNode {
   bool istok{false};
   std::array<unsigned, 5> arcs{};
};

NodePtr::NodePtr() : std::unique_ptr<TrieNode>(new TrieNode) {}

struct Trie {
   std::vector<TrieNode>  nodes;
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

unsigned long solve_pattern(const Trie &flags, std::string_view remaining_pattern) {
   using Paths = LinTable<unsigned, unsigned long long, 32>;
   Paths a, b, *in = &a, *out = &b;
   (*in)[0] = 1;
   for (char c : remaining_pattern) {
      for (const auto [ node, count ] : *in ) {
         unsigned nexti = flags.nodes[node].arcs[trie_idx(c)];
         if (nexti != 0) {
            auto &next = flags.nodes[nexti];
            (*out)[nexti]+=count;
            if (next.istok)
               (*out)[0]+=count;
         }
      }
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
