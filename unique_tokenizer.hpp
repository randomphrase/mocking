#pragma once

#include "tokenizer1.hpp"
#include <unordered_set>

/// calls callback with each unique token
template <typename callback_t>
struct unique_tokenizer {
  callback_t& callback_;

  void operator()(std::string_view input) const {
    struct {
      std::unordered_set<std::string_view> tokens_;
      void string_token(std::string_view word) {
        tokens_.insert(word);
      }
    } uniqifier;
    tokenizer1 tok {uniqifier};
    tok(input);
    for (const auto& t : uniqifier.tokens_) {
      callback_.string_token(t);
    }
  }
};

