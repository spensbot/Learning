#include <iostream>

using namespace std;

struct Test
{
  Test(){
    x = 1;
    y = 2;
    count++;
    zeroSmall();
    zeroBig();
    cout << "Constructed: " << count << " total" << endl;
  }
  Test(const Test& test){
    x = test.x;
    y = test.y;
    count++;
    cout << "L Value Constructor" << endl;
  }
  Test(Test&& test)
    : arrayBig(move(test.arrayBig))
  {
    x = test.x;
    y = test.y;
    cout << "R Value Constructor" << endl;
    test.arrayBig = nullptr;
  }

  ~Test(){
    count--;
    cout <<  "Destructed: " << count << " left" << endl;
    delete[] arrayBig;
  }

  void zeroSmall(){
    for (int i=0 ; i<10 ; i++){
      arraySmall[i] = 0;
    }
  }

  void zeroBig(){
    for (int i=0 ; i<100 ; i++){
      arrayBig[i] = 0;
    }
  }

  void incSmall(){
    int n = 0;
    for (int i=0 ; i<10 ; i++){
      arraySmall[i] = n;
      n++;
    }
  }

  void incBig(){
    int n = 0;
    for (int i=0 ; i<100 ; i++){
      arrayBig[i] = n;
      n++;
    }
  }

  int sumBig(){
    int sum = 0;
    for (int i=0 ; i<100 ; i++){
      sum += arrayBig[i];
    }
    return sum;
  }

  void print(){
    cout << x << " | " << y << endl;
    for (int i=0 ; i<10 ; i++){
      cout << arraySmall[i];
    }
    cout << endl << "Big sum: " << sumBig() << endl << endl;
  }

  int x, y;
  int arraySmall[10];
  int* arrayBig = new int[100];
  char bytes[1024];

  inline static int count = 0;
  
};

int main() {
  Test test1; //Default constructor
  test1.print();

  Test test2 = Test(); // Default constructor
  Test test3 = Test(test1); // copy constructor
  Test test4 = Test(std::move(Test())); // Move constructor
  Test test5 = Test(std::move(test1));
  test5.x = 99999;
  test5.y = 88888888;

  //test1.print();
  test3.print();
  test4.print();
  test5.print();

  cout << "stop" << endl;
  return 1;
}