#pragma once
#include <bits/stdc++.h>

void timeit(std::function<void(void)>);

namespace aoc {

using Executor = std::function<void(std::istream &, std::ostream &)>;
struct Case  {
   Case (std::string_view name, Executor func);
};

template <typename String>
inline std::pair<String, String> token(const String line, const std::string_view sep = " ") {
    auto split = line.find(sep);
    return (split == std::string::npos) ?
       std::pair{ line, "" } :
       std::pair{ line.substr(0, split), line.substr(split + sep.size()) };
}

struct Ignore{};

}
namespace std {
inline std::from_chars_result from_chars(const char *, const char *e, aoc::Ignore &) {
   return std::from_chars_result{e, std::errc{}};
}
}
namespace aoc {

template <typename Value, typename String >
Value
parsetoken(String &line, const std::string_view sep = " ") {
   String tok;
   std::tie(tok, line) = token(line, sep);
   Value value{};
   auto fcr = std::from_chars( tok.data(), tok.data() + tok.size(), value );
   if (fcr.ptr == tok.data())
      throw std::logic_error("parse failed");
   return value;
}

template <typename T> typename T::value_type popfront(T &container) {
   typename T::value_type value = container.front();
   container.pop_front();
   return value;
}
template <typename T> typename T::value_type popback(T &container) {
   typename T::value_type value = container.back();
   container.pop_back();
   return value;
}

}
