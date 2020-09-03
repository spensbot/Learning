#include <iostream>
#include <string>
#include <memory>
#include <fstream>

using namespace std;

class Node
{
private:
  int asciiToIndex(char ascii){
    return ascii;
  }
  char indexToAscii(int index){
    return index;
  }

public:
  Node() {
    // cout << "Node Created" << endl;
    for (int i=0 ; i<length ; i++){
      children[i] = nullptr;
    }
  }
  ~Node() {
    // cout << "Node Destroyed" << endl;
  }
  Node* getChild(char letter){
    int index = asciiToIndex(letter);
    return children[index];
  }
  void setChild(char letter){
    int index = asciiToIndex(letter);
    children[index] = new Node();
  }
  static const int length=127;
  Node* children[length];
  bool isWord = false;
};

class Tree
{
public:
  Tree() {
    cout << "Tree Created" << endl;
    head = new Node();
  }
  ~Tree() {
    destroyAllChildren(head);
    cout << "Destroyed " << nodeCount << " nodes" << endl;
    cout << "Tree Destroyed" << endl;
  }
  void put(string str){
    auto p = head;
    Node* child;
    for (int i=0 ; i<str.length() ; i++){
      char letter = str[i];
      child = p -> getChild(letter);
      if (child == nullptr) {
        p -> setChild(letter);
        nodeCount++;
      }
      p = p -> getChild(letter);
    }
    p->isWord = true;
  }

  bool find(string str){
    auto p = head;
    Node* child;
    for (int i=0 ; i<str.length() ; i++){
      char letter = str[i];
      p = p->getChild(letter);
      if (p == nullptr) return false;
    }
    return p->isWord;
  }

  private: 
  size_t nodeCount = 0;
  Node* head = nullptr;
  void destroyAllChildren(Node* node){
    for (int i=0 ; i<127 ; i++) {
      auto child = node->getChild(i);
      if (child != nullptr) {
        destroyAllChildren(child);
      }
    }
    //At this point, all child nodes should be destroyed.
    delete node;
  }
};

void addToDictionary(string&& fileName, Tree& dictionary){
  ifstream fileReader;
  fileReader.open(fileName);
  if (fileReader.is_open()){
    string word;
    int lines = 0;
    while(getline(fileReader, word)){ //read data from file object and put it into string.
        dictionary.put(word);
        lines++;
        if (lines % 10000 == 0){
          // cout << "Added " << lines << " lines!" << endl;
        }
    }
  }
  fileReader.close();
}

void spellCheck(string&& str, Tree& dictionary){
  bool isInDict = dictionary.find(str);
  cout << str;
  if (isInDict) {
    cout << " correct" << endl;
  } else {
    cout << " WRONG" << endl;
  }
}

class ArrayDictionary 
{
public:
  ArrayDictionary(string fileName){
    dict = new string[maxWords];
    for (int i=0 ; i<maxWords ; i++){
      dict[i] = nullptr;
    }
    // //Populate array from file
    // ifstream fileReader;
    // fileReader.open(fileName);
    // if (fileReader.is_open()){
    //   string word;
    //   int line = 0;
    //   while(getline(fileReader, word) && line < maxWords){ //read data from file object and put it into string.
    //       dict[line] = word;
    //       line++;
    //   }
    // }
    // fileReader.close();
  }

  // bool search(string&& word){
  //   int comparisons = 0;
  //   for (int i=0 ; i<maxWords ; i++){
  //     comparisons++;
  //     if (word == dict[i]){
  //       cout << word << " found | " << comparisons << " comparisons" << endl;
  //       return true;
  //     }
  //   }
  //   cout << word << ": not found | " << comparisons << " comparisons" << endl;
  //   return false;
  // }

  // bool binarySearch(string&& word, string* array){
  //   return true;
  // }

private:
  static const int maxWords = 500000;
  static const int maxLetters = 50;
  string* dict = nullptr;
};


int main()
{
  // Tree dictionary;
  // addToDictionary(string("words.txt"), dictionary);
  // spellCheck("hello", dictionary);
  // spellCheck("asdfa", dictionary);
  // spellCheck("steven", dictionary);
  // spellCheck("Run", dictionary);
  // spellCheck("run", dictionary);
  // spellCheck("prestidigitation", dictionary);
  // spellCheck("school", dictionary);
  // spellCheck("aren't", dictionary);
  // spellCheck("ain't", dictionary);
  // spellCheck("spell", dictionary);
  // spellCheck("Spell", dictionary);
  // spellCheck("geoffrey", dictionary);

  ArrayDictionary dict("words.txt");
  // dict.search("hello");
  // dict.search("Hello");
}