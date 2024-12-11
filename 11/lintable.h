#include <cstddef>
#include <functional>

// A dumb, but malloc-free hash table.
// You do not want to copy this, either the code, nor the runtime objects.
template <size_t sz, typename holder=unsigned long>
struct Bitset {
   static constexpr size_t holder_bits = sizeof (holder) * 8; // CHAR_BIT
   static constexpr size_t holder_count = (sz + (holder_bits - 1)) / holder_bits;
   holder contents[holder_count] {};
   constexpr size_t index(size_t bit) const noexcept { return bit / holder_bits; }
   constexpr size_t bitno(size_t bit) const noexcept { return bit % holder_bits; }
   constexpr holder mask(size_t bit) const noexcept { return holder(1) << bitno(bit); }
   constexpr holder &holder_at(size_t bit) noexcept { return contents[index(bit)]; }
   constexpr void set(size_t bit) noexcept { holder_at(bit) |= mask(bit); }
   constexpr void clear(size_t bit) noexcept { holder_at(bit) &= ~mask(bit); }
   constexpr bool get(size_t bit) const noexcept { return (contents[index(bit)] & mask(bit)) != 0; }
   constexpr void reset() noexcept {
      for (size_t i = 0; i < holder_count; ++i)
         contents[i] = 0;
   }
};

template <typename K, typename V, std::size_t S>
struct LinTable {
   using key_type = K;
   using mapped_type = V;
   struct Item {
      // It's not required at runtime to initialize these, but we also want it
      // to work for constexpr.
      K k{};
      V v{};
      size_t next_slot{};
   };
   Bitset<S> slots_used{};
   Item slots[S];
   size_t first_slot{S};

   constexpr V &operator[](size_t k) noexcept {
      auto idx = k % S;
      while (slots_used.get(idx) && slots[idx].k != k)
         idx = (idx + 1) % S;
      auto &cell = slots[idx];
      if (!slots_used.get(idx)) {
         // Add this to the list of keys we are managing.
         cell.next_slot = first_slot;
         first_slot = idx;
         cell.k = k;
         cell.v = {};
         slots_used.set(idx);
      }
      return cell.v;
   }
   using value_type = std::pair<K, std::reference_wrapper<const V>>;

   struct Iterator {
      const LinTable &t;
      size_t idx;
      constexpr Iterator(const LinTable &t, size_t idx) noexcept : t{t}, idx { idx } {}
      constexpr value_type operator *() const noexcept { return { t.slots[idx].k, t.slots[idx].v }; }
      constexpr Iterator &operator++() noexcept { idx = t.slots[idx].next_slot; return *this; }
      constexpr bool operator == (const Iterator &rhs) const noexcept { return idx == rhs.idx; }
      constexpr bool operator != (const Iterator &rhs) const noexcept{ return idx != rhs.idx; }
   };

   constexpr Iterator begin() const noexcept { return Iterator{ *this, first_slot }; }
   constexpr Iterator end() const noexcept { return Iterator{ *this, S }; };
   constexpr LinTable() {}
   constexpr ~LinTable() = default;

#if 1
   constexpr LinTable(const LinTable &rhs) = delete;
#else
   // unstested, but probably better than copying the array by memcpy.
   LinTable(const LinTable &rhs) {
      slots_used = rhs.slots_used;
      for (size_t i = rhs.slots[S].next_slot; i != S; i = rhs.slots[i].next_slot) {
         slots[i] = rhs.slots[i];
      }
   }
#endif
   constexpr void clear() {
      slots_used.reset();
      first_slot = S;
   }
   int f() { return 32; }
};
