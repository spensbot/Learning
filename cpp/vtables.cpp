#include "pch.h"

void print(std::string str) {
    std::cout << str << std::endl;
}

class Base {
public:
    Base() { // <-- constructor cannot be virtual. Before construction, there is no v-table.
        print("Base Constructor");
    }
    ~Base() {
        print("Base Destructor");
    }
    virtual void test() {
        print("Base test");
    }
};

class Derived : public Base {
public:
    Derived()
    : Base() { // <-- Base constructor can be explicitly called like this
        print("Derived Constructor");
    }
    ~Derived() {
        print("Derived Destructor");
    }
    void test() override {
        Base::test(); // <-- You may call overwritten base classes like this.
        print("Derived test");
    }
};

int main() {

    Base* p_derived = new Derived();
    p_derived->test();
    delete p_derived; // <-- If the base destructor is not marked virtual, then the derived classes destructor won't be called. Only base destructor will be called.
    // When the destructor is marked virtual, the compiler creates a v-table for the base class and the derived class with references to the correct virtual functions (including desctructors).
    // Then the instance is allocated with a pointer to it's v-table. So even though the pointer is of type Base, it knows to run v-table functions from the derived class.

    std::cin.ignore();
    Derived derived;
    derived.test();
}