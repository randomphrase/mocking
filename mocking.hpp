#pragma once

#include <functional>
// clang can't compile libstdc++'s implementation of this
#if __has_include(<experimental/source_location>)  && !defined(__clang__)
#include <experimental/source_location>
#endif
#include <boost/test/test_tools.hpp>

#if __cpp_lib_experimental_source_location
using source_location = std::experimental::source_location;
#else
struct no_source_location {
  static no_source_location current() { return {}; }
  no_source_location() = default;
  const char* file_name() const { return "unknown"; }
  int line() const { return 0; }
};
using source_location = no_source_location;
#endif

template <typename decorator_t, typename callable_t>
struct decorated_callable {
  decorator_t decorator_;
  callable_t callable_;

  template <typename D, typename C>
  decorated_callable& operator= (const decorated_callable<D, C>& other) {
    decorator_ = other.decorator_;
    callable_ = other.callable_;
    return *this;
  }
  template <typename D, typename C>
  decorated_callable& operator= (decorated_callable<D, C>&& other) {
    decorator_ = std::move(other.decorator_);
    callable_ = std::move(other.callable_);
    return *this;
  }

  auto& decorator() const { return decorator_; }

  template <typename ... args_t>
  auto operator() (args_t&& ... args) {
    decorator_(std::forward<args_t>(args)...);
    return callable_(std::forward<args_t>(args)...);
  }
};

class call_count_checker {
  struct state {
    state(source_location&& location, std::optional<unsigned> expected)
      : location_{std::move(location)}
      , expected_{expected}
    {}
    ~state() {
      if (expected_) {
        BOOST_TEST(*expected_ == calls_, "Function defined at " << location_.file_name() << ':' << location_.line()
                   << " expected " << *expected_ << " calls, " << calls_ << " seen");
      }
    }
    const source_location location_;
    const std::optional<unsigned> expected_;
    unsigned calls_ {};
  };
  std::shared_ptr<state> state_;
public:
  call_count_checker(source_location&& location = source_location::current())
    : state_{std::make_shared<state>(std::move(location), std::nullopt)}
  {}
  call_count_checker(unsigned expected, source_location&& location
                     = source_location::current())
    : state_{std::make_shared<state>(std::move(location), expected)}
  {}
  call_count_checker(const call_count_checker&) = default;
  call_count_checker(call_count_checker&&) = default;
  call_count_checker& operator= (const call_count_checker&) = default;
  call_count_checker& operator= (call_count_checker&&) = default;

  unsigned current_count() const { return state_->calls_; }

  template <typename ... Args>
  void operator() (Args&& ...) const {
    state_->calls_ += 1;
  }
};

template <typename fn>
decorated_callable<call_count_checker, fn>
expect_calls(unsigned expected, fn&& f,
             source_location&& location = source_location::current()) {
  return {{expected, std::move(location)}, std::forward<fn>(f)};
}

template <typename>
struct counted_function;

template <typename Ret, typename ... Args>
struct counted_function<Ret(Args...)>
  : decorated_callable<call_count_checker, std::function<Ret(Args...)>>
{
  using base_t = decorated_callable<call_count_checker, std::function<Ret(Args...)>>;

  using base_t::decorated_callable;
  using base_t::operator=;
  using base_t::decorator;

  auto current_count() const { return decorator().current_count(); }
};
