#include <functional>
#include <iostream>
#include <memory>
#include <string>

#include "main.h"

/*
 * 'Type Erasure' with static polymorphism or duck-typing
 *
 * `Wrapper` holds a pointer of a adapter `Model` which implements a `Concept`
 * and redirects call to actual objects.
 *
 * The example here defines a `Wrapper` to hold functors (i.e. a static variant
 * to `std::function`)
 */

namespace TypeErasure {

class Wrapper {
 protected:
  struct Concept {
    virtual ~Concept() = default;
    virtual std::string operator()() = 0;
  };

  template <typename T>
  struct Model : Concept {
    /*
     * NOTE:
     * This universal constructor will be also used for copy construction.
     * https://ericniebler.com/2013/08/07/universal-references-and-the-copy-constructo/
     */
    template <typename TT>
    Model(TT&& t) : _model(std::forward<TT>(t)) {}
    std::string operator()() override { return _model(); }

   private:
    T _model;
  };

 public:
  template <typename T>
  Wrapper(T&& obj)
      : _wrapper(std::make_shared<Model<T>>(std::forward<T>(obj))) {}

  std::string operator()() { return (*_wrapper)(); }

 private:
  std::shared_ptr<Concept> _wrapper;
};

int main() {
  struct Klass {
    std::string memberFunc() const { return "member const function called"; }
    std::string memberFunc() {
      return "another none-const member function called";
    }
  } klass;

  auto lambda1 = []() -> std::string { return "lambda called"; };
  auto function1 = Wrapper(lambda1);

  /* captured lambda variable is const */
  auto function2 = Wrapper([klass]() { return klass.memberFunc(); });

  /* bind none-const member function explicitly */
  auto function3 = Wrapper(std::function<std::string()>(std::bind(
      static_cast<std::string (Klass::*)()>(&Klass::memberFunc), &klass)));

  CHECK_VALUE("lambda", function1(), "lambda called");
  CHECK_VALUE("const member function", function2(),
              "member const function called");
  CHECK_VALUE("none-const member function", function3(),
              "another none-const member function called");

  return 0;
}

}  // namespace TypeErasure
