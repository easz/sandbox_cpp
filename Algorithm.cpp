#include <algorithm>
#include <functional>
#include <iterator>
#include <vector>

#include "main.h"

/*
 */

/**
 * std::max_element
 * std::distance
*/
template <typename T>
auto index_of_max(const std::vector<T>& v) {
  const auto iter_to_max = std::max_element(v.begin(), v.end());
  return std::distance(v.begin(), iter_to_max);
}

/**
 * std::adjacent_find
 * std::not_equal_to
*/
template <typename T>
bool all_equal(const std::vector<T>& v) {
  return std::adjacent_find(v.begin(), v.end(), std::not_equal_to<T>()) ==
         v.end();
}


namespace Algorithm {

//


int main() {
  const std::vector<int> numbers = {0, 2, 3, 1, 13, 5};

  const auto idx_to_max = index_of_max(numbers);
  CHECK_VALUE("index_of_max", idx_to_max, 4);

  CHECK("all_equal", !all_equal(numbers));
  CHECK("all_equal", all_equal<int>({1, 1, 1}));

  const std::vector<int> another = {0, 2, 3, 4, 5, 6};
  auto pair_to_mismatch = std::mismatch(numbers.begin(), numbers.end(), another.begin());
  CHECK_VALUE("mismatch", 1, *pair_to_mismatch.first);
  CHECK_VALUE("mismatch", 4, *pair_to_mismatch.second);

  return 0;
}

}  // namespace Algorithm
