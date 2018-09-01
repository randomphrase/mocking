#pragma once

#include <string_view>

template <typename callback_t>
struct tokenizer1 {
  callback_t& callback_;
  tokenizer1(callback_t& callback) : callback_{callback} {}

  void operator()(std::string_view input) const {
    const char ws[] = " \t\n";
    size_t a{}, b{};
    do {
      a = input.find_first_not_of(ws, b);
      if (a == input.npos) return;
      b = input.find_first_of(ws, a);
      auto tok = input.substr(a, b == input.npos? b : b - a);
      // call callback here!
      callback_.string_token(tok);
    } while (b != input.npos);
  }
};

