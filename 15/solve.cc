#include "aoc.h"

namespace {

enum Direction { UP = '^', DOWN='v', LEFT='<', RIGHT='>' };

using Scalar = uint8_t;

struct Point {
   Scalar row{}, col{};
   Point operator+ (const Point &rhs) const noexcept { return { uint8_t(row + rhs.row), uint8_t(col + rhs.col) }; }
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

struct Part {
   Point robot;
   using Row = std::vector<char>;
   std::vector<Row> map;
   std::vector<Direction> path;

   Part(std::istream &is) : Part{ is, [](const char c, Row &row) { row.push_back(c); } } {}

   template <typename Put> Part(std::istream &is, Put &&put) noexcept {
      for (std::string line; std::getline(is, line) && line != ""; ) {
         map.emplace_back();
         auto &row = map.back();
         for (char c : line) {
            if (c == '@')
               robot = {Scalar(map.size() - 1), Scalar(row.size())};
            put(c, row);
         }
      }
      for (int c; (c = is.rdbuf()->sbumpc()) != EOF; )
         if (c != '\n')
            path.push_back(Direction(c));
   }

   char &at(Point p) { return map[p.row][p.col]; }
   char at(Point p) const { return map[p.row][p.col]; }

   template <char c> unsigned score() const {
      unsigned total = 0;
      for (Scalar row = 0; row < map.size(); ++row)
         for (Scalar col = 0; col < map[row].size(); ++col)
            if (at({row, col}) == c)
               total += unsigned(row) * 100 + col;
      return total;
   }

   template <char c, typename RealPart> unsigned solve(std::ostream &) noexcept {
      for (Direction d : path)
         if (static_cast<RealPart*>(this)->move(robot, d))
            robot = robot + velocity(d);
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
            break;
         case '.':
         default:
            break;
      }
      std::swap(c, at(p));
      return true;
   }
};
aoc::Case part1{"part1", [](std::istream &is, std::ostream &os) {os << Part1(is).solve<'O', Part1>(os); } };

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
         default:
            r.push_back('.');
            r.push_back('.');
            break;
      }
   }

   Part2(std::istream &is) : Part{ is, put }{ }

   bool move_lr(Point p, Direction d) {
      Point next = p + velocity(d);
      char &c = at(next);
      switch (c) {
         case ']': case '[':
            if (!move_lr(next, d))
               return false;
            break;
         case '.':
            break;
         case '#': default:
            return false;
      }
      std::swap(c, at(p));
      return true;
   }

   template<bool trial_run> bool move_ud(Point p, Direction d) {
      auto next = p + velocity(d);
      char &c = at(next);
      switch (c) {
         case ']':
            if (!move_ud<trial_run>(next, d) || !move_ud<trial_run>(next + velocity(LEFT), d))
               return false;
            break;
         case '[':
            if (!move_ud<trial_run>(next, d) || !move_ud<trial_run>(next + velocity(RIGHT), d))
               return false;
         case '.':
            break;
         case '#': default:
            return false;
      }
      if constexpr (!trial_run)
         std::swap(c, at(p));
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
aoc::Case part2{"part2", [](std::istream &is, std::ostream &os) {os << Part2(is).solve<'[', Part2>(os); } };
}
