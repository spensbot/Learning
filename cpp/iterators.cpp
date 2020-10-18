#include <vector>
#include <string>
#include <iostream>
#include <list>

using namespace std;

template <typename T>
void print(T item) {cout << item << endl;}

int main() {
    vector<string> strings;
    list<string> stringList;

    strings.reserve(5);
    strings.push_back("One");
    strings.push_back("tooo");
    strings.push_back("3!");
    strings.push_back("__");

    for (int index=0 ; index<strings.size() ; index++) {
        print(strings[index]);
    }

    for (vector<string>::iterator iter=strings.begin() ; iter!=strings.end() ; iter++) {
        print(*iter);
    }

    for (auto& str : strings){
        print(str);
    }

    stringList.push_back("One");
    stringList.push_back("tooo");
    stringList.push_back("3!");
    stringList.push_back("__");

    stringList.push_front("0");
    stringList.reverse(); // Linked lists can be reversed

    // print(stringList[3]);   Cannot index into linked list

    size_t n = 0;
    for (auto iter=stringList.begin() ; iter!=stringList.end() ; iter++) {
        print(*iter);
        if (n == 3){
            stringList.insert(iter, "Sneaky Snake"); // Linked lists can insert elements into the middle
        }
        n++;
    }

    for (auto& str : stringList) {
        print(str);
    }

    auto iter = stringList.begin();
    print(*iter);
    print(*(++iter));
    print(*(--(--stringList.end())));
    // print(*(iter + 2)); // Linked list iterators cannot be incremented by more than 1.

    print(*(strings.begin() + 2)); // Vectors iterators can be incremented by as many elements as needed.
}