#include <bitset>
#include <cstddef>

template <typename K, typename V, std::size_t S>
struct LinTable {
   struct Item {
      K k;
      V v;
      size_t next;
   };
   std::bitset<S> slots_used;
   Item table[S+ 1];
   LinTable() {
      table[S].next = S;
   }

   V &operator[](size_t k) {
      auto idx = k % S;
      while (slots_used[idx] && table[idx].k != k) {
         idx = (idx + 1) % S;
      }
      auto &cell = table[idx];
      if (!slots_used[idx]) {
         // Add this to the list of keys we are managing.
         cell.next = table[S].next;
         table[S].next = idx;
         cell.k = k;
         cell.v = {};
         slots_used[idx] = true;
      }
      return cell.v;
   }

   struct Iterator {
      const LinTable &t;
      size_t idx;
      Iterator(const LinTable &t, size_t idx) noexcept : t{t}, idx { idx } {}
      std::pair<K, std::reference_wrapper<const V>> operator *() const noexcept { return { t.table[idx].k, t.table[idx].v } ; }
      Iterator &operator++() noexcept { idx = t.table[idx].next; return *this; }
      bool operator == (const Iterator &rhs) const noexcept { return idx == rhs.idx; }
      bool operator != (const Iterator &rhs) const  noexcept{ return idx != rhs.idx; }
   };
   Iterator begin() { return Iterator{ *this, table[S].next }; }
   Iterator end() { return Iterator{ *this, S }; };
   void clear() {
      slots_used.reset();
      table[S].next = S;
   }
};
