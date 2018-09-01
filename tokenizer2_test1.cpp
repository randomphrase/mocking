#define BOOST_TEST_MODULE mocking
#include <boost/test/unit_test.hpp>
#include "tokenizer2.hpp"
#include "mocking.hpp"

struct tokenizer2_fixture {
  struct mock_callback {
    std::function<void(std::string_view)> string_token;
    std::function<void(int)> int_token;
  };

  mock_callback callback_;
  tokenizer2<mock_callback> tokenizer_{callback_};
};

BOOST_FIXTURE_TEST_SUITE(tokenizer2_test, tokenizer2_fixture)

BOOST_AUTO_TEST_CASE(hello_123) {
  callback_.string_token = expect_calls(1, [] (std::string_view seen) {
      BOOST_TEST(seen == "hello");
    });
  callback_.int_token = expect_calls(1, [] (int seen) {
      BOOST_TEST(seen == 123);
    });
  tokenizer_("hello 123");
}

BOOST_AUTO_TEST_CASE(hello_123_ordered) {
  auto string_calls = expect_calls(1, [] (std::string_view seen) {
      BOOST_TEST(seen == "hello");
    });
  callback_.string_token = string_calls;
  callback_.int_token = expect_calls(1, [&] (int seen) {
      BOOST_TEST(string_calls.decorator().current_count() == 1);
      BOOST_TEST(seen == 123);
    });
  tokenizer_("hello 123");
}

BOOST_AUTO_TEST_SUITE_END()
