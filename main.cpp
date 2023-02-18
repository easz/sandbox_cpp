#include "main.h"

#include <array>
#include <vector>
#include <iostream>

int main() {
  RUN_TEST(FunctionPointer);
  RUN_TEST(UniversalReference);
  RUN_TEST(MostVexingParse);

  RUN_TEST(Polymorphism);
  RUN_TEST(TypeErasure);

  RUN_TEST(Template);
  RUN_TEST(TemplateFunction);

  RUN_TEST(Tuple);
  RUN_TEST(Algorithm);
  RUN_TEST(Sandbox);
  RUN_TEST(DesignPattern);
}