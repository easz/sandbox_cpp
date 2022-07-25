#include "main.h"

#include <iostream>

#define RUN_TEST(ns)                                \
  {                                                 \
    std::cout << "Test: '" #ns << "'" << std::endl; \
    ns::main();                                     \
  }

int main() {
  RUN_TEST(FunctionTemplate);
  RUN_TEST(TypeErasure);
}