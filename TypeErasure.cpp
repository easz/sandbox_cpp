#include <iostream>
#include <memory>
#include <vector>

#include "main.h"

namespace TypeErasure {

class Object {
 protected:
  struct Concept {
    virtual ~Concept() {}
    virtual void operator()() const = 0;
  };

  template <typename T>
  struct Model : Concept {
    Model(const T& t) : _model(t) {}
    void operator()() const override { _model(); }

   private:
    T _model;
  };

 public:
  template <typename T>
  Object(T&& obj) : _object(std::make_unique<Model<T>>(std::forward<T>(obj))) {}

  void operator()() const { (*_object)(); }

 private:
  std::unique_ptr<const Concept> _object;
};

int main() {
  auto lambda1 = []() { std::cout << "lamda called" << std::endl; };
  auto obj1 = Object(lambda1);
  obj1();

  struct Klass {
    void operator()() const { std::cout << "Klass called" << std::endl; }
  } klass;

  auto lambda2 = [&klass]() { klass(); };
  auto obj2 = Object(lambda2);
  auto vec = std::vector<Object>();
  vec.push_back(std::move(obj2));
  vec.push_back(Object(std::move(lambda2)));
  for (auto v : vec) {
    v();
  }

  return 0;
}

}  // namespace TypeErasure
