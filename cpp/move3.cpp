#include <iostream>
#include <string.h>
#include <memory>

class String
{
public:
  //Constructor
  String(const char* _data){
    std::cout << "Constructed" << std::endl;
    size = strlen(_data);
    copyData(_data, size);
  }
  //Copy Constructor
  String(const String& in) : size(in.size){
    std::cout << "Copy Constructed" << std::endl;
    copyData(in.data, in.size);
  }
  //Move Constructor
  String(String&& in) : size(in.size){
    std::cout << "Move Constructed" << std::endl;
    data = in.data;
    in.data = nullptr;
    in.size = 0;
  }

  // Assignment Operator
  String& operator=(const String& in){
    std::cout << "Copy Assignment" << std::endl;
    size = in.size;
    copyData(in.data, in.size);
    return *this;
  }
  // Move Assignment Operator
  String& operator=(String&& in){
    if (&in != this){
      std::cout << "Move Assignment" << std::endl;
      data = in.data;
      in.data = nullptr;
      in.size = 0;
    }
    return *this;
  }

  ~String(){
    delete[] data;
  }
  
  void print(){
    std::cout << data << std::endl;
  }

private:
  // std::unique_ptr<char> data;
  char* data;
  size_t size;

  void copyData(const char* src, size_t size){
    data = new char[size + 1];
    memcpy(data, src, size);
    data[size] = '\0';
  }
};

class Entity
{
public:
  Entity(const String& _name) : name(_name){}
  Entity(String&& _name) : name(std::move(_name)){}

  void print(){
    name.print();
  }

private:
  String name;
};


int main(){
  // Entity e1("Spenser"); //same as e2(String("two"))
  // Entity e2(std::move(String("Jacob")));
  // Entity e3(e1);
  // Entity e4(std::move(e2));
  // std::cout << std::endl;

  // e1.print(); // Still printable, because the data was copied, not moved.
  // // e2.print(); // Segmentation fault. e2's data has been plundered.
  // e3.print();
  // e4.print();

  String s1 = "one";
  String s2 = s1;
  String s3 = std::move(s2);

  s2 = "two";
  s3 = s2;
  s2 = std::move(s1);

  // s1.print();
  s2.print();
  s3.print();

  return 1;
}