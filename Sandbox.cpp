#include <array>
#include <string_view>

#include "main.h"

namespace Sandbox {

using sv = std::string_view;

struct Thing {
  static constexpr auto Apple = sv("Apple");
  static constexpr auto Kiwi = sv("Kiwi");
  static constexpr auto Cat = sv("Cat");

  static constexpr std::array Fruit = {Apple, Kiwi};
  static constexpr std::array Animal = {Cat};
};

constexpr bool isFruit(const std::string_view str) {
  for (auto v : Thing::Fruit) {
    if (str == v) return true;
  }
  return false;
}

constexpr int addOne(const int& n) { return n+1; }

int main() {
  CHECK_VALUE("Apple", Thing::Apple, "Apple");
  CHECK_VALUE("Cat", Thing::Cat, "Cat");
  static_assert(isFruit("Apple"));
  static_assert(!isFruit("Cat"));

  constexpr int val = addOne(4);
  int val2 = addOne(val);

  return 0;
}

}  // namespace Sandbox
