#include <functional>
#include <iostream>
#include <memory>
#include <vector>

#include "main.h"

namespace TypeErasure {

/*
 * 'Type Erasure' as static polymorphism with templates
 */

class Task {
 protected:
  struct ICallable {
    virtual ~ICallable() = default;
    virtual void operator()() = 0;
  };

  template <typename T>
  struct Callable : ICallable {
    /*
     * Note:
     * This universal constructor will be also used for copy construction.
     * https://ericniebler.com/2013/08/07/universal-references-and-the-copy-constructo/
     */
    template <typename TT>
    Callable(TT&& t) : _callable(std::forward<TT>(t)) {}
    void operator()() override { _callable(); }

   private:
    T _callable;
  };

 public:
  template <typename T>
  Task(T&& obj) : _task(std::make_shared<Callable<T>>(std::forward<T>(obj))) {}

  void operator()() { (*_task)(); }

 private:
  std::shared_ptr<ICallable> _task;
};

int main() {
  auto lambda1 = []() { std::cout << "lambda called" << std::endl; };

  struct Klass {
    void f1() const {
      std::cout << "member const function called" << std::endl;
    }
    void f2() {
      std::cout << "another none-const member function called" << std::endl;
    }
  } klass;

  auto task1 = Task(lambda1);
  const auto task2 = Task([klass]() { klass.f1(); });

  auto tasks = std::vector<Task>();
  tasks.push_back(task1);
  tasks.push_back(task2);
  tasks.push_back(Task(std::function<void()>(std::bind(&Klass::f2, &klass))));

  for (auto& task : tasks) {
    task();
  }

  return 0;
}

}  // namespace TypeErasure
