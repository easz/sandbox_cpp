#include <memory>
#include <string>

#include "main.h"

/*
 * Type of Polymorphism:
 *
 * - Compile time
 *   - overloading
 *   - type erasure with temoplate (See: TypeErasure.cpp)
 * - Run time
 *   - virtual function through inheritance
 */

namespace Polymorphism {
struct Base {
  virtual ~Base() = default;
  virtual std::string method() const { return "Base::method"; }
};

struct Derived : Base {
  virtual std::string method() const override { return "Derived::method"; }
};

int main() {
  // virtual function with pointer
  std::unique_ptr<Base> pBase = std::make_unique<Base>();
  std::unique_ptr<Base> pDerived = std::make_unique<Derived>();
  CHECK_VALUE("base", pBase->method(), "Base::method");
  CHECK_VALUE("derived", pDerived->method(), "Derived::method");

  // virtual function with reference
  const Base& rBase = Base();
  const Base& rDerived = Derived();
  CHECK_VALUE("base", rBase.method(), "Base::method");
  CHECK_VALUE("derived", rDerived.method(), "Derived::method");

  return 0;
}
}  // namespace Polymorphism