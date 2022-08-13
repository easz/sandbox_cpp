#include <functional>
#include <tuple>

#include "main.h"
/*
 *
 * https://www.learncpp.com/cpp-tutorial/function-pointers/
 * https://isocpp.org/wiki/faq/pointers-to-members
 * https://en.cppreference.com/w/cpp/utility/functional/invoke
 * https://en.cppreference.com/w/cpp/utility/functional/mem_fn
 */

namespace FunctionPointer {

// free function
int func(std::string, double d = 3.21) { return d * 100; }

// member function
struct Klass {
  static std::string func() { return "static"; }
  std::string func(int i) const { return std::to_string(i); }
};


int main() {
  /* NOTE: C++ implicit dereferencing of a function pointer */
  CHECK_VALUE("func()", func("100x", 1.23), 123);
  CHECK_VALUE("(*fnc)()", (*func)("100x", 1.23), 123);
  CHECK_VALUE("func()", func("default"), 321);  // deafult parameter is applied

  // a function pointer type `fnPtr` which can be assigned with function having
  // specific signature here we initiate it with nullptr.
  int (*fnPtr)(std::string, double) = nullptr;
  fnPtr = &func;
  CHECK_VALUE("func()", fnPtr("100x", 1.23), 123);
  CHECK_VALUE("(*fnc)()", (*fnPtr)("100x", 1.23), 123);
  /*
   * ERROR:
   * default parameters cannot be resolved when making a function call
   * with a function pointer
   */
  // fnPtr("missing param");

  /* NOTE: C++ implicit dereferencing of a function pointer */
  fnPtr = func;
  CHECK_VALUE("func()", fnPtr("100x", 1.23), 123);
  CHECK_VALUE("(*fnc)()", (*fnPtr)("100x", 1.23), 123);

  // std::function to wrap the function
  std::function<int(std::string, double)> fnWrapper;
  fnWrapper = func;
  CHECK_VALUE("std::function", fnWrapper("100x", 1.23), 123);

  // `PtrStaticMemFn` is a type of function pointer to a static member function
  using PtrStaticMemFn = std::string (*)();
  PtrStaticMemFn psmf = &Klass::func;
  CHECK_VALUE("static", psmf(), "static");

  //  `PtrMemFn` is a type of function pointer to a member function
  using PtrMemFn = std::string (Klass::*)(int) const;
  PtrMemFn pmf = &Klass::func;
  /* NOTE: invocation with an object instance */
  Klass obj;
  CHECK_VALUE("member", (obj.*pmf)(666), "666");                  // primitive
  CHECK_VALUE("std::invoke", std::invoke(pmf, obj, 666), "666");  // std::invoke
  /*
   * NOTE: explicit type specification or cast is required for overloaded
   * functions.
   */
  auto memfn = std::mem_fn<std::string(int) const>(&Klass::func);
  CHECK_VALUE("std::mem_fn", memfn(obj, 777), "777");
  memfn = std::mem_fn(static_cast<PtrMemFn>(&Klass::func));
  CHECK_VALUE("std::mem_fn", memfn(obj, 888), "888");

  return 0;
}
}  // namespace FunctionPointer