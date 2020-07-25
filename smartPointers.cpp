#include <array>
#include <memory>
#include <vector>
#include <iostream>
#include <string>

class Test
{
public:
  Test(){
    heapArray = new int[50];
    smartHeapArray = std::unique_ptr<int[]>(new int[50]);
    id = nextId;
    nextId++;
    std::cout << "Constructed: " << id << std::endl;
  }
  ~Test(){
    delete[] heapArray;
    std::cout << "Destructed: " << id << std::endl;
  }

  static inline int nextId = 1;
  int id;
  int stackArray[50];
  int* heapArray;
  std::unique_ptr<int[]> smartHeapArray;
};

class Entity
{
public:

  Entity(){
    p = new Test();
    smartP2.reset(p);
    smartP = std::make_unique<Test>();
    //smartP3 = smartP2; cannot create a copy of a unique_ptr
  }

  Test test;
  Test* p;
  std::unique_ptr<Test> smartP2;
  std::unique_ptr<Test> smartP;
  std::unique_ptr<Test> smartP3;
};

class SharedEntity
{
public:
  SharedEntity() {
    p = new Test();
    smartP1.reset(p);
    printCount();
    smartP2 = smartP1; // We can create copies of a shared_ptr
    printCount();
    smartP3 = smartP1;
    printCount();
  }

  void printCount(){
    std::cout << "Ref Count: " << smartP1.use_count() << std::endl;
  }

  Test* p;
  std::shared_ptr<Test> smartP1;
  std::shared_ptr<Test> smartP2;
  std::shared_ptr<Test> smartP3;
};


int main(){
  {
    std::cout << "UNIQUE PTR EXAMPLE: " << std::endl;
    Entity entity; //Stack allocated
  }
  
  {
    std::cout << std::endl << "SHARED PTR EXAMPLE: " << std::endl;
    SharedEntity* shared = new SharedEntity(); //Heap allocated
    delete shared; //Needs to be manually deleted
  }

  {
    std::cout << std::endl << "SHARED PTR EXAMPLE 2: " << std::endl;
    auto shared = std::make_unique<SharedEntity>(); //Heap allocated object
  } // Automatically deleted when the unique_ptr "shared" goes out of scope


}