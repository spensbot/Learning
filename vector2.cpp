#include <iostream>
#include <memory>
#include "Vector.h"
#include "LifeCycleLogger.h"

template <typename T>
void print(stm::Vector<T>& vect) {
  std::cout << std::endl;
  for (auto i=0 ; i<vect.getSize() ; i++){
    std::cout << vect[i] << std::endl;
  }
}

int main() {
  stm::Vector<int> test;

  test.reserve(3);

  test.push(1);
  test.push(2);
  test.push(100);
  test.push(43);
  test.push(-12);

  std::cout << test.pop() << std::endl;
  std::cout << test.pop() << std::endl;

  print(test);

  // stm::Vector<stm::LifeCycleLogger> loggerTest;

  // loggerTest.push(stm::LifeCycleLogger());



  int testInt = 1;
  std::cout << std::endl << testInt << std::endl;

  //Returns testInt value
  std::cout << testInt++ << std::endl;

  std::cout << ++testInt << std::endl;

  return 0;
}