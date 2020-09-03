#include <iostream>
#include <string.h>

using namespace std;

class String
{
public:
  String(const char* string)
  {
    printf("Created!\n");
    m_size = strlen(string);
    m_data = new char[m_size];
    memcpy(m_data, string, m_size);
  }

  String(const String& src)
  {
    printf("Copied\n");
    m_size = src.m_size;
    m_data = new char[m_size];
    memcpy(m_data, src.m_data, m_size);
  }

  String(String&& src)
  {
    printf("Moved\n");
    m_size = src.m_size;
    m_data = src.m_data;

    src.m_data = nullptr;
  }

  ~String()
  {
    printf("Destroyed\n");
    delete[] m_data;
  }

  void print(){
    for (size_t i=0 ; i<m_size ; i++)
    {
      cout << m_data[i];
    }
    cout << endl;
  }

  uint32_t length(){
    return m_size;
  }

private:
  char* m_data;
  uint32_t m_size;
};

class Test
{
public:
  Test(const String& name) : m_name(name)
  {
    printf("L value reference used\n");
  }

  Test(String&& name) : m_name(std::move(name))
  {
    printf("R Value reference used\n");
  }

  void print(){
    m_name.print();
  }

private:
  String m_name;
};

int main(){
  String str("Hello");
  Test test1(str);
  test1.print();
  
  printf("\n");
  
  Test test2("Hello");
  test2.print();
}









// // L-value reference. Only accepts l-values. Won't accept r-values unless const.
// void print(string& name){
//   cout << "l-value: " << name << endl;
// }

// // R-value reference override. Only accepts r-values
// void print(string&& name){
//   cout << "r-value: " << name << endl;
// }

// int main() {
//   string first = "Spenser ";
//   string last = "Saling";

//   string full = first + last;

//   print(full);
//   print(first + last);
//   print(string("Spenser"));
// }






// int getRValue(){
//   return 100;
// }

// int& getLValue(){
//   static int lValue = 999;
//   return lValue;
// }

// int main()
// {
//   int i = 10;
//   int a = i;

//   getLValue() = i;

//   cout << "i: " << i << endl;
//   cout << "a: " << a << endl;
//   cout << "R Value: " << getRValue() << endl;
//   cout << "L Value: " << getLValue() << endl;
// }


