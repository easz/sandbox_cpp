#include <cstdlib>
#include <iostream>

#define DECLARE_TEST(ns) \
  namespace ns {         \
  int main();            \
  }

#define RUN_TEST(ns)                        \
  do {                                      \
    std::cout << "Test: " #ns << std::endl; \
    ns::main();                             \
  } while (0, 0)

#define CHECK_VALUE(title, expected, actual)                          \
  do {                                                                \
    if ((expected) != (actual)) {                                     \
      std::cerr << "[FAILED] [" << title << "] "                      \
                << "expected: " << expected << ", actual: " << actual \
                << std::endl;                                         \
      exit(EXIT_FAILURE);                                             \
    }                                                                 \
  } while (0, 0)

#define CHECK(title, pred) CHECK_VALUE(title, pred, true)

#define CHECK_FAILED(title)                                 \
  do {                                                      \
    std::cerr << "[FAILED] [" << title << "]" << std::endl; \
    exit(EXIT_FAILURE);                                     \
  } while (0, 0)

DECLARE_TEST(FunctionPointer)
DECLARE_TEST(UniversalReference)
DECLARE_TEST(MostVexingParse)

DECLARE_TEST(Polymorphism)
DECLARE_TEST(TypeErasure)

DECLARE_TEST(Template)
DECLARE_TEST(TemplateFunction)

DECLARE_TEST(Tuple)
DECLARE_TEST(Algorithm)
DECLARE_TEST(Sandbox)
