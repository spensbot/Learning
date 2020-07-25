#include <iostream>
#include "timer.h"
#include <vector>

using namespace std;

template <typename T>
class Node 
{
public:
  Node(const T& _data) : data(_data) {
    count = 1;
  }

  Node(T&& _data) : data(std::move(_data)) {
    count = 1;
  }

  ~Node(){
    delete left;
    delete right;
  }

  void addChild(T&& childData){
    if (childData > data){
      if (right == nullptr){
        right = new Node<T>(childData);
      } else {
        right->addChild(std::move(childData));
      }
    }
    else if (childData < data){
      if (left == nullptr){
        left = new Node<T>(childData);
      } else {
        left->addChild(std::move(childData));
      }
    }
    else {
      count++;
    }
  }

  void addChild(Node<T>* child){
    if (child->data > data){
      if (right == nullptr){
        right = child;
      } else {
        right->addChild(child);
      }
    }
    else if (child->data < data){
      if (left == nullptr){
        left = child;
      } else {
        left->addChild(child);
      }
    }
    else {
      count += child->count;
    }
  }

  int find(const T& searchData){
    if (searchData > data){
      if (right == nullptr){
        return 0;
      } else {
        return right->find(searchData);
      }
    } else if (searchData < data){
      if (left == nullptr){
        return 0;
      } else {
        return left->find(searchData);
      }
    } else {
      return count;
    }
  }

  void print(){
    if (left != nullptr){
      left->print();
    }
    cout << data << ": " << count << " | ";
    if (right != nullptr){
      right->print();
    }
  }

  int getDepth(){
    int leftDepth = 1;
    int rightDepth = 1;
    if (left != nullptr){
      leftDepth += left->getDepth();
    }
    if (right != nullptr){
      rightDepth += right->getDepth();
    }
    if (leftDepth > rightDepth){
      return leftDepth;
    } else {
      return rightDepth;
    }
  }

  int countNodes(){
    int count = 1;
    if (left != nullptr){
      count += left->countNodes();
    }
    if (right != nullptr){
      count += right->countNodes();
    }
    return count;
  }

  int getMinDepth(){
    int nodeCount = countNodes();
    int depth = 0;
    int possibleNodes = 0;
    int nodesAtThisDepth = 1;
    while (possibleNodes < nodeCount){
      depth++;
      possibleNodes += nodesAtThisDepth;
      nodesAtThisDepth *= 2;
    }
    return depth;
  }

  void appendLeftToRight(vector<Node<T>*>& list){
    if (left != nullptr){
      left->appendLeftToRight(list);
    }
    list.push_back(this);
    if (right != nullptr){
      right->appendLeftToRight(list);
    }
  }

  T data;
  int count;
  Node<T>* left = nullptr;
  Node<T>* right = nullptr;
};


template <typename T>
class BinaryTree
{
public:
  ~BinaryTree(){
    delete root;
  }

  void populateRandom(int n, int max){
    for (int i=0 ; i<n ; i++){
      add(rand() % max);
    }
  }

  void add(T&& data){
    if (root == nullptr){
      root = new Node<T>(std::move(data));
    } else {
      root->addChild(std::move(data));
    }
  }

  void add(Node<T>* node){
    if (root == nullptr){ 
      root = node;
    } else {
      root->addChild(node);
    }
  }

  // Recursively adds all nodes in the list to the tree.
  // Assuming the list is sorted, this will create a balanced tree with minimum depth.
  void add(vector<Node<T>*>& sortedList, int start, int end){
    
    //check if the section to add contains at least one element
    if (end >= start) {
      //Add the element in the middle of the section
      int length = end - start + 1;
      int centerIndex = start + length / 2;
      add(sortedList[centerIndex]);
      //Then recursively 
      add(sortedList, start, centerIndex - 1);
      add(sortedList, centerIndex + 1, end);
    }
  }

  int find(const T& data){
    if (root == nullptr){
      return 0;
    } else {
      return root->find(data);
    }
  }

  void rebase(){
    //Create an empty list of node pointers
    vector<Node<T>*> sortedList;

    //Size the list of node pointers to hold all nodes in the tree
    int nodeCount = root->countNodes();
    sortedList.reserve(nodeCount);

    //Add each node in the tree in order
    root->appendLeftToRight(sortedList);
    for (Node<T>* item : sortedList){
      //Clear the children of each node, since we are going to rebuild the tree
      item->left = nullptr;
      item->right = nullptr;
    }

    root = nullptr;

    add(sortedList, 0, sortedList.size() - 1);
  }

  void printInfo(){
    if (root != nullptr){
      cout << "Depth: " << root->getDepth() << endl;
      cout << "Nodes: " << root->countNodes() << endl;
      cout << "Min Possible Depth: " << root->getMinDepth() << endl;
    } else {
      cout << "The Tree is Empty!" << endl;
    }
  }

  void print(){
    root->print();
    cout << endl;
  }

private:
  Node<T>* root = nullptr;
};


int main(){

// VALIDITY TESTING
  BinaryTree<int> binaryTree;
  int max = 10;
  binaryTree.populateRandom(max, max);
  binaryTree.add(5);
  binaryTree.add(15);
  // binaryTree.print();


// STRESS TESTING
  BinaryTree<int> testTree;
  max = 1000000;
  
  cout << "Creating binary tree with 1 million random numbers" << endl;
  Timer::testCount = 1;
  Timer::test([&testTree, max](){
    testTree.populateRandom(max, max);
  });

  cout << endl;
  testTree.printInfo();
  cout << "Searching for a random number 1million times" << endl;
  Timer::testCount = 1000000;
  int findSum = 0;
  Timer::test([&findSum, &testTree, max](){
    findSum += testTree.find(rand() % max);
  });
  cout << "Average find: " << (float)findSum / (float)Timer::testCount << endl;

  cout << endl << endl << "Rebasing..." << endl;
  Timer::testCount = 1;
  Timer::test([&testTree](){
    testTree.rebase();
  });
  testTree.printInfo();

  cout << "Searching for a random number 1million times" << endl;
  findSum = 0;
  Timer::testCount = 1000000;
  Timer::test([&findSum, &testTree, max](){
    findSum += testTree.find(rand() % max);
  });
  cout << "Average find: " << (float)findSum / (float)Timer::testCount << endl;

  return 1;
}