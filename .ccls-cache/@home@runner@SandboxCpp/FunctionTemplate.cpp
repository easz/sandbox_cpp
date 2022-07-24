#include "main.h"

namespace FunctionTemplate {

class Klazz {
public:
  // ERROR:
  // template <typename T> Klazz() {}

  template <typename T> Klazz(T t) {}

  template <typename T> static void func() { T a = T(); }
};

void test() {
  // auto c = Klazz(); // ERROR
  auto c = Klazz(13);
  Klazz::func<int>();
}

} // namespace FunctionTemplate
