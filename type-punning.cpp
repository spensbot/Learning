#include <iostream>

struct Cube {
  int x,y,z;

  void print() {
    std::cout << x << " | " << y << " | " << z << "\n";
  }
};

int main() 
{
  float a = 523.432;
  int b = *(int*)&a;
  int& c = *(int*)&a;

  std::cout << a << " | " << b << " | " << c << "\n";
  
  c = 1222237470;

  std::cout << a << " | " << b << " | " << c << "\n";

  Cube cube {3, 4, 5};

  cube.print();

  int& x = *(int*)&cube;
  int& z = *(int*)((char*)&cube + 8);

  std::cout << x << " | " << z << "\n";

  x = 10;
  z = 22;

  cube.print();

  return 1;
}