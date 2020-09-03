#include <iostream>

using namespace std;

class Rect
{
public:
  Rect(int w, int h) : width(w), height(h){
    update();
  }

  void print() {
    cout << width << " X " << height << " = " << area << endl;
  }

  bool operator==(const Rect& rect){
    if (this->width == rect.width && this->height == rect.height) return true;
    return false;
  }

  Rect operator+(const Rect& rect){
    int newW = this->width + rect.width;
    int newH = this->height + rect.height;
    return Rect(newW, newH);
  }

  Rect operator+(int add){
    return Rect( this->width + add, this->height + add );
  }

private: 
  int width;
  int height;
  int area;

  void update(){
    area = width * height;
  }
};

int main() {
  Rect rect(3,4);
  rect.print();
  Rect rect2(1,2);
  (rect + rect2).print();
  (rect + 2).print();
  (rect + 5).print();

  Rect rect3(3,4);
  cout << (rect == rect2) << endl;
  cout << (rect == rect3) << endl;

  return 1;
}