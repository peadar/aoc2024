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
    if (split == std::string::npos)
        return { line, "" };
    else
        return { line.substr(0, split), line.substr(split + sep.size()) };
}

template <typename String, typename Value>
Value
parsetoken(String &line, const std::string_view sep = " ") {
   String tok;
   std::tie(tok, line) = token(line, sep);
   Value value;
   auto fcr = std::from_chars( tok.data(), tok.data() + tok.size(), value );
   if (fcr.ptr == tok.data())
      throw std::logic_error("parse failed");
   return value;
}

}
