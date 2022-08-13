
#include <memory>
#include <vector>

#include "main.h"

/*
 * prefer `T obj;` or `T obj{};` over `T obj();` for object construction to avoid most vexing parse.
 */

namespace MostVexingParse {
struct NonCopyable {
  NonCopyable() = default;
  NonCopyable(const NonCopyable&) = delete;
  NonCopyable& operator=(const NonCopyable&) = delete;
  NonCopyable(NonCopyable&&) = default;
  NonCopyable& operator=(NonCopyable&&) = default;
};

int main() {
  std::vector<NonCopyable> v;
  NonCopyable nc0;
  NonCopyable nc1{};

  v.push_back(NonCopyable());
  v.push_back(std::move(nc0));
  v.push_back(std::move(nc1));

  /*
   * ERROR:
   * Most vexing parse.
   * Object construction with parentheses is treated as a function declaration
   * https://en.wikipedia.org/wiki/Most_vexing_parse
   *
   * The expression `nc2` is treated as a function object.
   */
  // NonCopyable nc2();
  // v.push_back(nc2);

  return 0;
}
}  // namespace MostVexingParse