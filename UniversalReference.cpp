#include <string>
#include <vector>

#include "main.h"

/*
 *
 * 1. type deduction:
 *   - reference-ness is ignore at first
 *   - lvalue of type `T` is deducted to lvalue reference `T&`
 *   - rvalue of type `T` is deducted to just type `T`
 * 2. reference collapse
 *   - rvalue ref to rvalue ref -> rvalue reference
 *   - otherwise lvalue reference
 *
 * https://isocpp.org/blog/2012/11/universal-references-in-c11-scott-meyers#FurtherInformation
 */

namespace UniversalReference {

struct Klass {
  /* Ctor */
  Klass() { constructed = "[C0] Klass()"; }

  /*
   * Ctor with && which is a universal reference.
   */
  template <typename T>
  Klass(T&&) {
    constructed = "[C1] Klass(T&&)";
  }

  /*
   * WARNING:
   * Actually it is !discouraged! to overload a universal function (i.e. the
   * above one)
   *
   * The above constructor `Klass(T&&)` will be choosen over this function for
   * copy construction.˝
   *
   * This constructor is supposed to accept a rvalue.
   */
  template <typename T>
  Klass(const T&&) {
    constructed = "[C3] Klass(const T&&)";
  }

  /*
   * NOTE:
   * && here is rvalue reference!
   */
  template <typename T>
  Klass(std::vector<T>&& t) {
    constructed = "[C4] Klass(std::vector<T>&&)";
  }

  std::string constructed = "";
};

/* type deduction in context of `typedef` */
template <typename T>
struct A {
  typedef T&& type;
  typedef const T&& ctype;
};

int main() {
  //
  // template instantiation
  //

  auto o1 = Klass();
  CHECK_VALUE("default ctor", o1.constructed, "[C0] Klass()");

  o1 = Klass("string type");
  CHECK_VALUE("universal ctor accepts rval", o1.constructed, "[C1] Klass(T&&)");
  int num = 42;
  o1 = Klass(num);
  CHECK_VALUE("universal ctor accepts lval", o1.constructed, "[C1] Klass(T&&)");

  auto o2 = o1;
  CHECK_VALUE("universal ctor is called for copy instead of copy ctor!",
              o2.constructed, "[C1] Klass(T&&)");

  o1 = Klass(std::vector<int>{});
  CHECK_VALUE("rval ref of a vector1", o1.constructed,
              "[C4] Klass(std::vector<T>&&)");

  /*
   * NOTE:
   * Klass(std::vector<T>&&) is NOT universal reference but just rvalue
   * reference, therefore it does not accept lvalue; The constructor
   * `Klass(T&&)` is called where T is `std::vector<int>`
   */
  auto vec = std::vector<int>{};
  o1 = Klass(vec);
  CHECK_VALUE("rval ref of a vector2", o1.constructed, "[C1] Klass(T&&)");

  //
  // `auto` keyword
  //

  // auto&& is a universal reference which can take rvalue
  auto&& c1 = Klass();
  CHECK("rval ref", (std::is_same_v<decltype(c1), Klass&&>));
  CHECK("rval ref", std::is_rvalue_reference_v<decltype(c1)>);
  CHECK("rval ref", !std::is_const_v<std::remove_reference_t<decltype(c1)>>);
  /*
   * NOTE:
   * If T is a reference type then is_const<T>::value is always false.
   * Therefore we remove reference before checking const-ness.
   */

  // auto&& is a universal reference which can take rvalue
  auto&& c2 = c1;
  CHECK("lval ref", (std::is_same_v<decltype(c2), Klass&>));
  CHECK("lval ref", std::is_lvalue_reference_v<decltype(c2)>);
  CHECK("lval ref", !std::is_const_v<std::remove_reference_t<decltype(c2)>>);

  // const auto&& is a const rvalue reference
  const auto&& c3 = Klass();
  CHECK("const rval ref", (std::is_same_v<decltype(c3), const Klass&&>));
  CHECK("const rval ref", std::is_rvalue_reference_v<decltype(c3)>);
  CHECK("const rval ref",
        std::is_const_v<std::remove_reference_t<decltype(c3)>>);
  /* ERROR: it cannot bind lvalue */
  // const auto&& c4 = c1;

  /* NOTE: `const T&` extends rvalue lifetime */
  const auto& c6 = Klass();
  CHECK("const T&", (std::is_same_v<decltype(c6), const Klass&>));
  CHECK("const T&", std::is_reference_v<decltype(c6)>);
  /* ERROR: it is invalid */
  // auto& c7 = Klass();

  //
  // `typedef`
  //

  CHECK("T &&", (std::is_same_v<A<int>::type, int&&>));
  CHECK("T&& &&", (std::is_same_v<A<int&&>::type, int&&>));
  CHECK("T& &&", (std::is_same_v<A<int&>::type, int&>));
  CHECK("const T &&", (std::is_same_v<A<const int>::type, const int&&>));
  CHECK("const T&& &&", (std::is_same_v<A<const int&&>::type, const int&&>));
  CHECK("const T& &&", (std::is_same_v<A<const int&>::type, const int&>));

  return 0;
}

}  // namespace UniversalReference