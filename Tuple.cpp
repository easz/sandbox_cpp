#include <iostream>
#include <tuple>

#include "main.h"

/*
 * https://aherrmann.github.io/programming/2016/02/28/unpacking-tuples-in-cpp14/
 * https://en.cppreference.com/w/cpp/language/fold
 * https://blog.tartanllama.xyz/exploding-tuples-fold-expressions/
 */

namespace Tuple {

template <typename... Args>
constexpr size_t args_size(Args... args) {
  return sizeof...(Args);
}

template <typename... Args, size_t... Is>
constexpr auto fisrt_of_impl(const std::tuple<Args...>& t, std::index_sequence<Is...>) {
  return std::make_tuple(std::get<Is>(t)...);
}

template <size_t N, typename... Args>
constexpr auto first_of(const std::tuple<Args...>& t) {
  static_assert(N <= sizeof...(Args), "N too large");
  return fisrt_of_impl(t, std::make_index_sequence<N>());
}

//


int main() {
  const auto t = std::make_tuple(1, 2.0, "3");
  CHECK_VALUE("args_size", args_size(1, 2.0, "3"), 3);
  CHECK("first_of", (first_of<3>(t) == t));
  CHECK("first_of", (first_of<2>(t) == std::make_tuple(1, 2.0)));
  CHECK("first_of", (first_of<0>(t) == std::make_tuple()));


  return 0;
}

}  // namespace Tuple
