#include "pch.h"

std::optional<int> getBigInt() {
  int n = std::rand() % 1000;

  std::cout << n << std::endl;

  if (n > 500) return n;

  return {};
}

void printTest() {
  auto potentialBigInt = getBigInt();
  if (potentialBigInt.has_value()) {
    std::cout << potentialBigInt.value() << std::endl;
  } else {
    std::cout << "Optional Empty" << std::endl;
  }

  int verifiedBigInt = potentialBigInt.value_or(1000);
  std::cout << verifiedBigInt << std::endl << std::endl;
}

int main() {
  printTest();
  printTest();
  printTest();
  printTest();

  return 1;
}