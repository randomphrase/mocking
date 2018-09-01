#define BOOST_TEST_MODULE mocking
#include <boost/test/unit_test.hpp>
#include "tokenizer2.hpp"
#include "mocking.hpp"
#include <deque>

using namespace std::literals::string_view_literals;

struct tokenizer2_fixture {
  struct mock_callback {
    counted_function<void(std::string_view)> string_token;
    counted_function<void(int)> int_token;
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
  callback_.string_token = expect_calls(1, [] (std::string_view seen) {
      BOOST_TEST(seen == "hello");
    });
  callback_.int_token = expect_calls(1, [&] (int seen) {
      BOOST_TEST(callback_.string_token.current_count() == 1);
      BOOST_TEST(seen == 123);
    });
  tokenizer_("hello 123");
}

BOOST_AUTO_TEST_CASE(hello_123_tuple) {
  std::deque<std::variant<std::string_view, int>> exp {{ "hello"sv, 123 }};

  auto check_token = expect_calls(size(exp), [&] (auto seen) {
    auto* next_exp = std::get_if<decltype(seen)>(&exp.front());
    BOOST_REQUIRE(next_exp);
    BOOST_TEST(*next_exp == seen);
    exp.pop_front();
    });
  callback_.int_token = check_token;
  callback_.string_token = check_token;
  tokenizer_("hello 123");
}

BOOST_AUTO_TEST_SUITE_END()
