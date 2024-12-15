#include "aoc.h"
#include <boost/container/small_vector.hpp>

namespace {

enum Direction { UP = '^', DOWN='v', LEFT='<', RIGHT='>' };

using Scalar = uint8_t;

struct Point {
   Scalar row{};
   Scalar col{};
   Point operator+ (const Point &rhs) const noexcept { return { uint8_t(row + rhs.row), uint8_t(col + rhs.col) }; }
   Point operator* (const Scalar s) const noexcept { return { uint8_t(row * s), uint8_t(col * s) }; }
   Point &operator += (const Point &rhs) noexcept { row += rhs.row; col += rhs.col; return *this;  }
};

constexpr Point velocity(Direction d) noexcept {
   switch (d) {
      case UP: return {Scalar(-1), 0};
      case DOWN: return {1, 0};
      case LEFT: return {0, Scalar(-1)};
      case RIGHT: return {0, 1};
      default: __builtin_unreachable();
   }
}

template <typename T> using Vec = boost::container::small_vector<T, 60>;

struct Part {
   Point robot;
   using Row = Vec<char>;
   Vec<Row> map;
   Vec<Direction> path;

   Part(std::istream &is) : Part{ is, [](const char c, Row &row) { row.push_back(c); } } {}

   template <typename Put> Part(std::istream &is, Put &&put) noexcept {
      for (std::string line; std::getline(is, line) && line != ""; ) {
         map.emplace_back();
         Vec<char> &row = map.back();
         for (char c : line) {
            switch (c) {
               case '@':
                  robot = Point{Scalar(map.size() - 1), Scalar(row.size())};
                  [[fallthrough]];
               default:
                  put(c, row);
                  break;
            }
         }
      }
      for (int c; (c = is.rdbuf()->sbumpc()) != EOF; ) {
         switch (c) {
            case '\n': break;
            default: path.push_back(Direction(c)); break;
         }
      }
   }

   char &at(Point p) { return map[p.row][p.col]; }
   char at(Point p) const { return map[p.row][p.col]; }

   template <char c> unsigned score() const {
      unsigned total = 0;
      for (Scalar row = 0; row < map.size(); ++row)
         for (Scalar col = 0; col < map[row].size(); ++col)
            if (at(Point{row, col}) == c)
               total += unsigned(row) * 100 + col;
      return total;
   }

   template <char c, typename RealPart> unsigned solve(std::ostream &) noexcept {
      for (Direction d : path)
         if (static_cast<RealPart*>(this)->move(robot, d))
            robot += velocity(d);
      return score<c>();
   }
};

struct Part1 : Part {
   Part1(std::istream &is) : Part{ is } {}
   bool move(Point p, Direction d) {
      Point next = p + velocity(d);
      char &c = at(next);
      switch (c) {
         case '#':
            return false;
         case 'O': case '@':
            if (!move(next, d))
               return false;
            [[fallthrough]];
         case '.':
            std::swap(c, at(p));
            return true;
         default:
            abort();
      }
   }
};

struct Part2 : Part {
   static void put(char c, Row &r) {
      switch (c) {
         case '#':
            r.push_back('#');
            r.push_back('#');
            break;
         case 'O':
            r.push_back('[');
            r.push_back(']');
            break;
         case '@':
            r.push_back('@');
            r.push_back('.');
            break;
         case '.':
            r.push_back('.');
            r.push_back('.');
            break;
      }
   }

   Part2(std::istream &is) : Part{ is, put }{ }

   bool move_lr(Point p, Direction d) {
      auto v = velocity(d);
      char &c = at(p + v);
      switch (c) {
         case ']': case '[':
            if (!move_lr(p+v, d))
               return false;
            [[fallthrough]];
         case '.':
            std::swap(at(p+v), at(p));
            return true;
         case '#':
            return false;
         default:
            abort();
      }
   }

   template<bool trial_run> bool move_ud(Point p, Direction d) {
      auto v = velocity(d);
      auto next = p + v;
      char &c = at(next);
      switch (c) {
         case ']':
            if (move_ud<trial_run>(next, d) && move_ud<trial_run>(next + velocity(LEFT), d))
               goto move;
            return false;
         case '[':
            if (move_ud<trial_run>(next, d) && move_ud<trial_run>(next + velocity(RIGHT), d))
               goto move;
            return false;
         case '#':
            return false;
         case '.':
            goto move;
         default:
            abort();
      }
move:
      if constexpr (!trial_run)
         std::swap(at(next), at(p));
      return true;
   }

   bool move(Point p, Direction d) {
      switch (d) {
         case LEFT: case RIGHT: return move_lr(p, d);
         case UP: case DOWN: return move_ud<true>(p, d) && move_ud<false>(p, d);
         default: __builtin_unreachable();
      }
   }
};

aoc::Case part1{"part1", [](std::istream &is, std::ostream &os) {os << Part1(is).solve<'O', Part1>(os); } };
aoc::Case part2{"part2", [](std::istream &is, std::ostream &os) {os << Part2(is).solve<'[', Part2>(os); } };
aoc::Case parse{"parse-only", [](std::istream &is, std::ostream &) { Part{is}; } };
}
