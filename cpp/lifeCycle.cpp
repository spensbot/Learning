#include "LifeCycleLogger.h"

int main() {

  stm::LifeCycleLogger::shouldLog = true;

  // Default Constructor
  stm::LifeCycleLogger l1("Foo");

  //Copy construct
  auto l2 = stm::LifeCycleLogger(l1);
  //Move Construct
  auto l3 = stm::LifeCycleLogger(std::move(l2));
  //Move Construct
  auto l4 = stm::LifeCycleLogger(stm::LifeCycleLogger("Bar"));
  
  //Copy Assign
  l1 = l4;
  //Move Assign
  l2 = std::move(l4);

  std::cout << std::endl;
  stm::LifeCycleLogger::printInfo();
  std::cout << std::endl;

  return 0;
}