#include "main.h"

int main() {
  RUN_TEST(FunctionTemplate);
  RUN_TEST(FunctionPointer);
  RUN_TEST(UniversalReference);
  RUN_TEST(MostVexingParse);

  RUN_TEST(Polymorphism);
  RUN_TEST(TypeErasure);
}