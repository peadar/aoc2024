#include "aoc.h"
#include <cassert>
#include <map>
#include <optional>

// We could just use a sequence of pairs of <string>->int, and search it. But
// get some practice, and implement a trie for the lookups.
struct Node {
   std::map<char, Node> arcs;
   std::optional<int> value;
};

template <typename Value>
struct Trie {
   Node root;
   void insert(std::string_view text, Value value);
   Trie(std::initializer_list<std::tuple<std::string_view, Value>> strings);
   template<typename B, typename E> constexpr std::optional<Value> find(B b, E e) const noexcept;
};

template <typename Value>
void
Trie<Value>::insert(std::string_view s, Value value) {
   Node *n = &root;
   for (auto i = s.rbegin(); i != s.rend(); ++i)
      n = &n->arcs[*i];
   n->value = value;
}

template<typename Value> template<typename B, typename E>
constexpr
std::optional<Value>
Trie<Value>::find(B cur, E end) const noexcept {
   const Node *n = &root;
   for (; cur != end; ++cur) {
      auto child = n->arcs.find(*cur);
      if (child == n->arcs.end())
         return std::nullopt;
      n = &child->second;
      if (n->value)
         return n->value;
   }
   return std::nullopt;
}

template <typename Value>
Trie<Value>::Trie(std::initializer_list<std::tuple<std::string_view, Value>> items) {
   for (auto &[n,v] : items)
      insert(n, v);
}

Trie<int> numbers {
   { "zero", 0 }, { "one", 1 }, { "two", 2 }, { "three", 3 }, { "four", 4 },
   { "five", 5 }, { "six", 6 }, { "seven", 7 }, { "eight", 8 }, { "nine", 9 },
   { "0", 0 }, { "1", 1 }, { "2", 2 }, { "3", 3 }, { "4", 4 },
   { "5", 5 }, { "6", 6 }, { "7", 7 }, { "8", 8 }, { "9", 9 },
};
