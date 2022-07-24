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

  template<typename F>
  class Callable : ICallable {
  public:
    template <typename FF>
    explicit Callable(FF&& f) : _callable(std::forward<FF>(f)) {}

    virtual void call() override { _callable(); }
  private:
    F _callable;
  };

  /* deduction guide */
  template <typename FF> Callable(FF&& f) -> Callable<FF>;

  template<typename T>
  Task(T&& t) : _task(std::make_shared<Callable<T>>(std::forward<T>(t))) {}
  void execute() {}
    
private:
  std::shared_ptr<ICallable> _task;
};

int main() {
  
  struct Klass {
    void call() {
      std::cout << "XXX call" << std::endl;
    }
  };

  auto lambda = [](){ std::cout << "lambda call" << std::endl; };
  
  auto c1 = Task::Callable(lambda); 
  c1.call();

  auto x = Klass();
  auto c2 = Task::Callable([&x](){x.call();}); 
  c2.call();
  
  return 0;
}

} // namespace TypeErasure