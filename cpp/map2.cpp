#include <iostream>
#include <map>
#include <vector>

class Test {
public:
    std::string name;
    std::string bio;
    int instantiationCount;

    // // DEFAULT CONSTRUCTOR
    // Test() : name("Default Name"), bio("Default Bio") {
    //     std::cout << "Default Constructor" << std::endl;
    // }
    // OVERWRITTEN CONSTRUCTOR
    Test(std::string _name, std::string _bio) : name(_name), bio(_bio), instantiationCount(0) {
        std::cout << instantiationCount++ << "  Constructor" << std::endl;
    }
    // COPY CONSTRUCTOR
    Test(const Test& other) : name(other.name), bio(other.bio), instantiationCount(other.instantiationCount) {
        std::cout << instantiationCount++ << "  Copy Constructor" << std::endl;
    }
    // COPY ASSIGNMENT OPERATOR
    Test& operator=(const Test& other) {
        name = other.name;
        bio = other.bio;
        instantiationCount = other.instantiationCount;
        std::cout << instantiationCount++ << "  Copy Assignment" << std::endl;
        return *this;
    }

    void print() {
        std::cout << "     " << name << ": " << bio << std::endl;
    }
};

class TestBetter {
public:
    std::string name;
    std::string bio;

    // CONSTRUCTOR
    TestBetter(std::string _name, std::string _bio) : name(_name), bio(_bio) {
        std::cout << "  Constructor" << std::endl;
    }
    // COPY CONSTRUCTOR
    TestBetter(const TestBetter& other) : name(other.name), bio(other.bio) {
        std::cout << "  Copy Constructor" << std::endl;
    }
    // COPY ASSIGNMENT OPERATOR
    TestBetter& operator=(const TestBetter& other) {
        std::cout << "  Copy Assignment" << std::endl;
        name = other.name;
        bio = other.bio;
        return *this;
    }
    // MOVE CONSTRUCTOR
    TestBetter(TestBetter&& other) {
        name = std::move(other.name);
        bio = std::move(other.bio);
        std::cout << "    Move Constructor" << std::endl;
    }
    // MOVE ASSIGNMENT OPERATOR
    TestBetter& operator=(TestBetter&& other) {
        std::cout << "    Move Assignment" << std::endl;
        name = std::move(other.name);
        bio = std::move(other.bio);
        return *this;
    }

    void print() {
        std::cout << "    " << name << ": " << bio << std::endl;
    }
};

int main() {
    // std::map<int, Test> users;

    // users[1] = Test("Spenser", "I'm cool");
    // std::cin.ignore();
    // users.try_emplace(2, "Jason", "Hey guys, I'm Jason");
    // std::cin.ignore();
    // for (auto& [id, user] : users) {
    //     std::cout << id << ")" << user.name << ": " << user.bio << std::endl;
    // }

    // std::cin.ignore();
    // std::vector<Test> usersVect;
    // usersVect.emplace_back("Jason", "Whatever, dude");
    // std::cin.ignore();
    // usersVect.push_back(Test("Jason", "Whatever, dude"));
    // std::cin.ignore();
    // for (auto& user : usersVect) {
    //     std::cout << user.name << ": " << user.bio << std::endl;
    // }

//  Level I
    std::cout << "LEVEL I" << std::endl;
    std::vector<Test> tests;
    tests.push_back(Test("Spenser", "hellz yeah"));
    tests.push_back(Test("Guy", "oh baby"));
    for (auto test : tests) {
        test.print();
    }
    std::cin.ignore();

//  Level II
    std::cout << std::endl << "LEVEL II" << std::endl;
    std::vector<TestBetter> tests2;
    tests2.emplace_back("Jack", "oh baby");
    tests2.emplace_back("Spenser", "whatevs");
    tests2.push_back(TestBetter("Spenser", "whatevs"));
    for (auto& test : tests2) {
        test.print();
    }
    std::cin.ignore();

}