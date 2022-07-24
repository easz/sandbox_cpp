#include "main.h"

#include <iostream>
#include <list>
#include <memory>

namespace TypeErasure {

class Task {
public:

  struct ICallable {
    virtual ~ICallable() = default;
    virtual void call() = 0;
  };

  template <typename F> class Callable : ICallable {
  public:
    /*
     * The constructor has to have its own template argument list
     * so that '&&' cam be treated as universal reference.
     */
    template <typename FF>
    explicit Callable(FF&& f) : _callable(std::forward<FF>(f)) {}

    virtual void call() override { _callable(); }

  private:
    F _callable;
  };

  /*
   * deduction guide 
   */
  template <typename FF> Callable(FF&& f) -> Callable<FF>;

  template <typename T>
  Task(T &&t) : _task(std::make_shared<Callable<T>>(std::forward<T>(t))) {}
  void execute() {}

private:
  std::shared_ptr<ICallable> _task;
};

int main() {
  auto lambda = []() { std::cout << "lambda call" << std::endl; };
  Task::Callable(lambda).call();
  Task::Callable(std::move(lambda)).call();

  struct Klass {
    void call() { std::cout << "XXX call" << std::endl; }
    void call() const { std::cout << "XXX call const" << std::endl; }
  } x;
  /*
   * lam
   */
  Task::Callable([x]() mutable { x.call(); }).call();
  Task::Callable([x]() { x.call(); }).call();

  return 0;
}

} // namespace TypeErasure