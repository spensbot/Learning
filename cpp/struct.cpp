#include <string>
#include <iostream>

using namespace std;

struct Vec2D
{
  int x,y;

  void print(){
    cout << "X: " << x << " Y: " << y << endl;
  }

  Vec2D operator+(Vec2D other) {
    int newX = other.x + this->x;
    int newY = other.y + this->y;
    return {newX, newY};
  }

  bool operator==(Vec2D other) {
    if (other.x == this->x && other.y == this->y) {
      return true;
    }
    return false;
  }

};

int main() {
  Vec2D v1 = {1,3};
  Vec2D v2 = {3,2};

  auto v3 = v1 + v2;

  Vec2D v4 = Vec2D(v3); //L-value reference copy constructor
  Vec2D v5 = Vec2D({1,5}); //R-value reference copy constructor
  Vec2D v6 = Vec2D(); //Default constructor
  Vec2D v7 = {4, 10}; //Implicit constructor

  v1.print();
  v2.print();
  v3.print();

  cout << (v3 == v4) << endl;
  cout << (v1 == v2) << endl;

  return 1;
}