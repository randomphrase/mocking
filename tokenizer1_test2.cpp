#define BOOST_TEST_MODULE mocking
#include <boost/test/unit_test.hpp>
#include "tokenizer1.hpp"
#include "mocking.hpp"

struct tokenizer1_fixture {
  struct mock_callback {
    std::function<void(std::string_view)> string_token;
  };

  mock_callback callback_;
  tokenizer1<mock_callback> tokenizer_{callback_};
};

BOOST_FIXTURE_TEST_SUITE(tokenizer1_test, tokenizer1_fixture)

BOOST_AUTO_TEST_CASE(not_called) {
  tokenizer_("   ");
}

BOOST_AUTO_TEST_CASE(not_mocked) {
  BOOST_CHECK_THROW(tokenizer_("boom"), std::bad_function_call);
}

BOOST_AUTO_TEST_CASE(hello_counted) {
  int calls = 0;
  callback_.string_token = [&] (std::string_view seen) {
    ++calls;
    BOOST_TEST(seen == "hello");
  };
  tokenizer_("   hello    ");
  BOOST_TEST(calls == 1);
}

BOOST_AUTO_TEST_CASE(hello_expect_calls) {
  callback_.string_token = expect_calls(1, [] (std::string_view seen) {
      BOOST_TEST(seen == "hello");
    });
  tokenizer_("hello");
}

BOOST_AUTO_TEST_CASE(hello_expect_calls_fail, *boost::unit_test::expected_failures(1)) {
  callback_.string_token = expect_calls(1, [] (std::string_view) {});
  tokenizer_("");
}

BOOST_AUTO_TEST_CASE(hello_world_1) {
  auto expect_world = expect_calls(1, [&] (std::string_view seen) {
      BOOST_TEST(seen == "world");
    });
  auto expect_hello = expect_calls(1, [&] (std::string_view seen) {
      BOOST_TEST(seen == "hello");
      callback_.string_token = expect_world;
    });
  callback_.string_token = expect_hello;
  tokenizer_("hello world");
}

BOOST_AUTO_TEST_CASE(hello_world_2) {
  const char* exp_toks[] = { "hello", "world" };
  callback_.string_token = expect_calls(2, [&,exp=exp_toks] (std::string_view seen) mutable {
      BOOST_REQUIRE(exp != std::end(exp_toks));
      BOOST_TEST(seen == *exp++);
    });
  tokenizer_("hello world");
}

BOOST_AUTO_TEST_SUITE_END()
