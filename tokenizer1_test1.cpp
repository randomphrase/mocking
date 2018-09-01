#define BOOST_TEST_MODULE mocking
#include <boost/test/unit_test.hpp>
#include "tokenizer1.hpp"

BOOST_AUTO_TEST_CASE(tokenizer_test_manual_mock) {
  struct {
    int calls = 0;
    void string_token(std::string_view seen) {
      ++calls;
      BOOST_TEST(seen == "hello");
    }
  } callback;
  tokenizer1 tok {callback};
  tok("hello");
  BOOST_TEST(callback.calls == 1);
}
