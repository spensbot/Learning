#include <iostream>
#include <memory>
#include "Vector.h"
#include "LifeCycleLogger.h"

template <typename T>
void printAll(stm::Vector<T>& vect) {
  std::cout << "-----" << std::endl;
  for (auto i=0 ; i<vect.getSize() ; i++){
    std::cout << vect[i] << std::endl;
  }
  std::cout << "-----" << std::endl;
}

int main() {
  // stm::Vector<int> test;

  // test.reserve(3);

  // test.push(1);
  // test.push(2);
  // test.push(100);
  // test.push(43);
  // test.push(-12);

  // std::cout << test.pop() << std::endl;
  // std::cout << test.pop() << std::endl;

  // print(test);

  stm::Vector<stm::LifeCycleLogger> loggers;

  auto logger1 = stm::LifeCycleLogger();
  stm::LifeCycleLogger logger2;
  logger2 = stm::LifeCycleLogger();
  auto loggerTemp = stm::LifeCycleLogger();
  auto logger3 = std::move(loggerTemp);

  loggers.push(logger1);
  loggers.push(logger2);
  loggers.push(logger3);
  loggers.push(stm::LifeCycleLogger());

  printAll(loggers);

  stm::LifeCycleLogger::printInfo();

  return 0;
}