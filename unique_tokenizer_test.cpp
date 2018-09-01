#define BOOST_TEST_MODULE mocking
#include <boost/test/unit_test.hpp>
#include "unique_tokenizer.hpp"
#include "mocking.hpp"

using namespace std::literals::string_view_literals;

struct unique_tokenizer_fixture {
  struct callback {
    std::function<void(std::string_view)> string_token;
  } callback_;
  unique_tokenizer<callback> tokenizer_{callback_};
};

BOOST_FIXTURE_TEST_SUITE(unique_tokenizer_test, unique_tokenizer_fixture)

BOOST_AUTO_TEST_CASE(hello) {
  callback_.string_token = expect_calls(1, [] (std::string_view tok) {
      BOOST_TEST(tok == "hello");
    });
  tokenizer_("hello");
}

BOOST_AUTO_TEST_CASE(hello_hello_hello) {
  callback_.string_token = expect_calls(1, [] (std::string_view tok) {
      BOOST_TEST(tok == "hello");
    });
  tokenizer_("hello hello hello");
}

BOOST_AUTO_TEST_CASE(hello_world) {
  std::set expected { "hello"sv, "world"sv };
  callback_.string_token = expect_calls(2, [&] (std::string_view tok) {
      BOOST_TEST(expected.erase(tok) == 1, tok << " not expected");
    });
  tokenizer_("hello world");
}

BOOST_AUTO_TEST_SUITE_END()
