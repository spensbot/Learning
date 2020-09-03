#include "pch.h"

std::pair<std::string, int> getPerson(){
  return {"Spenser", 28};
}

std::tuple<int, float, double> getValue(){
  int r = std::rand();
  return {r, float(r), double(r)};
}

int main()
{
  auto person = getPerson();
  std::cout << person.first << " | " << person.second << std::endl;

  auto [name, age] = getPerson();
  std::cout << name << " | " << age << std::endl;

  auto value = getValue();
  std::cout << std::get<0>(value) << " | " << std::get<1>(value) << " | " << std::get<2>(value) << std::endl;

  int valInt;
  float valFloat;
  double valDouble;
  std::tie(valInt, valFloat, valDouble) = getValue();
  std::cout << valInt << " | " << valFloat << " | " << valDouble << std::endl;

  auto [vInt, vFloat, vDouble] = getValue();
  std::cout << vInt << " | " << vFloat << " | " << vDouble << std::endl;
}