#include <iostream>
#include <memory>
#include <vector>
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

template <typename T>
void printAll(std::vector<T>& vect) {
  std::cout << "-----" << std::endl;
  for (auto i=0 ; i<vect.size() ; i++){
    std::cout << vect[i] << std::endl;
  }
  std::cout << "-----" << std::endl;
}

int main() {

  stm::LifeCycleLogger::shouldLog = true;

  stm::Vector<stm::LifeCycleLogger> loggers;
  // std::vector<stm::LifeCycleLogger> loggers;

  loggers.reserve(4);

  loggers.push_back(stm::LifeCycleLogger("Spenser"));
  loggers.push_back(stm::LifeCycleLogger("asdf"));
  loggers.push_back(stm::LifeCycleLogger("Jacob"));
  loggers.push_back(stm::LifeCycleLogger("Juan"));

  // loggers.emplace_back("Spenser");
  // loggers.emplace_back("asdf");
  // loggers.emplace_back("Jacob");
  // loggers.emplace_back("Juan");

  printAll(loggers);

  stm::LifeCycleLogger::printInfo();

  return 0;
}