#include <iostream>
#include "LifeCycleLogger.h"

class Test {
public:
    Test() {
        std::cout << "Constructed" << std::endl;
    }

    Test(const Test& other) {
        std::cout << "Copy Constructed" << std::endl;
    }

    Test& operator= (const Test& other) {
        std::cout << "Copy Assigned" << std::endl;
        return *this;
    }

    static Test& Get() {
        static Test instance = Load();
        return instance;
    }

    void Print() {
        std::cout << "I'm a singleton" << std::endl;
    }

private:
    static Test Load() {
        std::cout << "Singleton Loaded" << std::endl;
        Test test;
        return test;
    }

};

int main() {
    
    Test::Get().Print();

    std::cin.ignore();

    Test::Get().Print();

    return 0;
}