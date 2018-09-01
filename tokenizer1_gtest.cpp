#include "tokenizer1.hpp"
#include "gtest/gtest.h"
#include "gmock/gmock.h"

using namespace std::literals::string_view_literals;

struct tokenizer1_test : ::testing::Test {
  struct mock_callback {
    MOCK_METHOD1(string_token, void(std::string_view));
  };
  mock_callback callback_;
  tokenizer1<mock_callback> tokenizer_{callback_};
};

TEST_F(tokenizer1_test, hello) {
  EXPECT_CALL(callback_, string_token("hello"sv)).Times(1);
  tokenizer_("hello");
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
