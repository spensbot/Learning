#include <iostream>
#include <cstdlib>
#include <memory>
#include <vector>

using namespace std;

int* getRandomNumberArray(int n){
  auto nums = new int[n];
  for (auto i=0 ; i<n ; i++){
    nums[i] = rand();
  }
  return nums;
}

vector<int> getRandomNumberVector(int n){
  vector<int> list;
  for (auto i=0 ; i<n ; i++){
    list.push_back(rand());
  }
  return list;
}

template <typename type>
void printArray(type* array, size_t length){
  for (auto i=0 ; i<length ; i++){
    cout << array[i] << endl;
  }
}

void printList(vector<int> list){
  for (auto i=0 ; i<list.size() ; i++){
    cout << list[i] << endl;
  }
}

void bubbleSort(int* array, int length){

  bool modified = true;

  size_t iterations = 0;

  while (modified) {
    modified = false;
    for (auto i=0 ; i < length-1 ; i++){
      int current = array[i];
      int next = array[i+1];
      if (next < current){
        array[i] = next;
        array[i+1] = current;
        modified = true;
      }
      iterations++;
    }
  }

  cout << "Bubble Sort iterations: " << iterations << endl;
}

vector<int> quickSort(vector<int> list){
  if (list.size() < 2) {
    return list;
  }

  auto pivotIndex = list.size()/2;
  auto pivotValue = list[pivotIndex];

  vector<int> lower;
  vector<int> upper;

  for (int i=0 ; i<list.size() ; i++){
    auto current = list[i];

    if (current < pivotValue){
      lower.push_back(current);
    } else {
      upper.push_back(current);
    }
  }

  auto lowerSorted = quickSort(lower);
  auto upperSorted = quickSort(upper);

  lowerSorted.insert(lowerSorted.end(), upperSorted.begin(), upperSorted.end());

  return lowerSorted;
} 

// void quickSort(int* array, int length, int start = 0){
//   //Recursive bottom
//   //List of a single element considered sorted
//   if (length < 2) {
//     return;
//   }

//   auto pivotIndex = start + length / 2;
//   auto pivotValue = array[pivotIndex];


// }

int main() {
  auto length = 10;

  // auto nums1 = getRandomNumberArray(length);


  // bubbleSort(nums1, length);
  // printArray<int>(nums1, length);

  vector<int> list = getRandomNumberVector(length);
  auto listSorted = quickSort(list);
  printList(list);
  cout << endl;
  printList(listSorted);

  // delete[] nums1;

  return 1;
}