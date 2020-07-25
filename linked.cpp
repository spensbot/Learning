#include <iostream>
#include <string>
#include <memory>

using namespace std;

class Node
{
public:
  Node(string d) : data(d) {
    cout << "Node Created" << endl;
  }
  ~Node() { cout << "Node Destroyed" << endl; }
  string data;
  Node* next = nullptr;
};

class LinkedList
{
public:
  LinkedList() { cout << "List Created" << endl; }

  ~LinkedList() {
    auto p=head;
    auto pLast=p;
    while (p->next != nullptr) {
      p = p->next;
      delete pLast;
      pLast = p;
    }
    delete p;
    cout << "List Destroyed" << endl;
  }
  
  void push(string obj){
    auto newNode = new Node(obj);
    if (head == nullptr) {
      head = newNode;
    } else {
      auto p = head;
      while (p->next != nullptr){
        p = p->next;
      }
      p->next = newNode;
    }
    size++;
  }

  string& get(size_t index){
    if(index >= size) throw;

    auto p = head;
    for (int i=0; i<index ; i++){
      p = p->next;
    }
    return p->data;
  }

  void printAll(){
    auto p = head;
    while (p != nullptr) {
      cout << p->data << endl;
      p = p->next;
    }
  }

  void reverse(){
    Node* pLast = nullptr;
    Node* pCurrent = head;
    Node* pNext = nullptr;

    //Return if the list is empty
    if(pCurrent == nullptr) return;

    while(pCurrent->next != nullptr){
      pNext = pCurrent->next;

      pCurrent->next = pLast;

      pLast = pCurrent;
      pCurrent = pNext;
    }

    //Last element reached (no more next) this will be the new head.
    pCurrent->next = pLast;
    head = pCurrent;
  }

  Node* head = nullptr;
  size_t size = 0;
};

int main() { 
  auto list = LinkedList();
  // list.push(string("Hello"));
  // list.push(string("Goodbye"));
  // list.push(string("OK"));
  // list.push(string("Whatevs"));
  list.push(string("a"));
  list.push(string("b"));
  list.push(string("c"));
  list.push(string("d"));
  list.push(string("e"));

  list.printAll();

  list.reverse();

  list.printAll();
}
