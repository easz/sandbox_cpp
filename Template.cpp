#include <tuple>

#include "main.h"

/**
 * https://en.cppreference.com/w/cpp/language/templates
 * - class template
 * - function tempplate
 * - alias template
 * - variable template
 * - concepts
 *
 */

namespace Template {

/**
 * silly `size_of` to get the length of template argument. I,e, `sizeof...` *
 */
template<class L> struct size_of;

template<template<class...> class L, class... E>
struct size_of<L<E...>> : std::integral_constant<size_t, sizeof...(E)>
{};

template<typename T>
struct eval;

template<template<typename...> class TT, typename... Rest>
struct eval<TT<Rest...>> {};

template<typename T1, typename T2> struct E;

int main() {
  typedef std::tuple<int, double, char> TupleType;
  CHECK_VALUE("sizeof", size_of<TupleType>::value, 3);
  CHECK_VALUE("sizeof", size_of<TupleType>(), 3);
  /* conversion function: `operator value_type() const` */
  CHECK_VALUE("sizeof", static_cast<size_t>(size_of<TupleType>()), 3);

  typedef std::variant<int, double> VariantType;
  CHECK_VALUE("sizeof", size_of<VariantType>::value, 2);

  eval<E<int, float>> eE;
  return 0;
}

}  // namespace Template
