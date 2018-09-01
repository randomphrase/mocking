#pragma once

#include <string_view>
#include <charconv>

template <typename callback_t>
struct tokenizer2 {
  callback_t& callback_;
  tokenizer2(callback_t& c) : callback_{c} {}

  void operator()(std::string_view input) const {
    const char ws[] = " \t\n";
    size_t a{}, b{};
    do {
      a = input.find_first_not_of(ws, b);
      if (a == input.npos) return;
      b = input.find_first_of(ws, a);
      auto tok = input.substr(a, b == input.npos? b : b - a);
      if (int i; std::from_chars(tok.begin(), tok.end(), i).ec == std::errc{}) {
        callback_.int_token(i);
      } else {
        callback_.string_token(tok);
      }
    } while (b != input.npos);
  }
};

