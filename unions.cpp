#include <iostream>

struct Point {
  int x, y;
};

struct Line {
  union {
    struct {
      int x1, y1, x2, y2;
    };
    struct {
      Point p1, p2;
    };
  };

  void print() {
    std::cout << "x1: " << x1 << " y1: " << y1 << " x2: " << x2 << " y2: " << y2 << "\n";
    std::cout << "p1.x: " << p1.x << " p1.y: " << p1.y << " p2.x: " << p2.x << " p2.y: " << p2.y << std::endl;
  }
};

int main() 
{
  Line line {1, 2 ,3, 4};

  line.print();

  line.x1 = 10;
  line.p2 = {123, 456};

  line.print();

  return 1;
}