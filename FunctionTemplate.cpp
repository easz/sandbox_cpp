#include "main.h"

namespace FunctionTemplate {

class Klazz {
public:
  /* 
    ERROR: constructor templates are named without using a function name, there is no way to provide an explicit template argument list for these function templates 
  */
  template <typename T> Klazz() {}

  template <typename T> 
  Klazz(T t) {}

  /* 
    Template argument deduction takes place after the function template name lookup.
    Template instantions are not overloaded
  */
  template <typename T> 
  void func() { auto a = T(); }
};

int main() {
  /* ERROR: there is no valid default constructor */
  // auto c = Klazz(); 
  
  /* ERROR: explicit template argument is not valid for construcor w/o argument */
  // auto c = Klazz<int>(); 
  
  auto c = Klazz("foo");
  c.func<int>();
  c.func<double>();

  return 0;
}

} // namespace FunctionTemplate
