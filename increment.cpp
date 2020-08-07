#include<iostream>

int main() {

  int testInt = 1;

  // Outputs testInt initial value: 1
  std::cout << std::endl << testInt << std::endl; // 1

  // testInt++ returns testInt value (1) then increments it (to 2).
  std::cout << testInt++ << std::endl; // 1

  // ++testInt increments the value (to 3) then return it (3).
  std::cout << ++testInt << std::endl;

  return 0;
}