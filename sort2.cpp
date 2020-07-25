#include <iostream>
#include <cstdlib>
#include <memory>
#include <vector>
#include <functional>
#include <string>
#include "timer.h"

std::unique_ptr<int[]> getRandomList(int length){
  int* list = new int[length];
  for(int i=0 ; i<length ; i++){
    list[i] = rand()%10000;
  }
  return std::unique_ptr<int[]>(list);
}

std::unique_ptr<int[]> copyList(std::unique_ptr<int[]>& list, int length){
  int* newList = new int[length];
  for (int i=0 ; i<length ; i++){
    newList[i] = list[i];
  }
  return std::unique_ptr<int[]>(newList);
}

struct Bounds{
  int start, end;

  void print(){
    std::cout << start << " | " << end << std::endl;
  }

  int length(){
    return end - start + 1;
  }

  int center(){
    return start + length()/2;
  }

  Bounds splitLeftExclude(int centerIndex){
    return {start, centerIndex - 1};
  }

  Bounds splitRightInclude(int centerIndex){
    return {centerIndex , end};
  }

  Bounds splitRightExclude(int centerIndex){
    return {centerIndex + 1 , end};
  }
};

void printList(const std::unique_ptr<int[]>& list, Bounds bounds){
  for (auto i=bounds.start ; i<=bounds.end ; i++){
    std::cout << list[i] << " ";
  }
  std::cout << std::endl;
}

void indent(int n){
  while (n > 0) {
    std::cout << "  ";
    n--;
  }
}

void swap(const std::unique_ptr<int[]>& hello, int index1, int index2){
  int temp = hello[index1];
  hello[index1] = hello[index2];
  hello[index2] = temp;
}

void bubbleSort(const std::unique_ptr<int[]>& list, Bounds bounds){

  bool modified = true;

  int iterations = 0;

  while (modified) {
    modified = false;
    for (auto i=bounds.start ; i < bounds.end ; i++){
      if (list[i+1] < list[i]){
        swap(list, i, i+1);
        modified = true;
      }
      iterations++;
    }
  }
}

void selectionSort(std::unique_ptr<int[]>& list, Bounds bounds){
  auto length = bounds.length();
  int min = 0;
  int sortedIndex = bounds.start;

  while (sortedIndex <= bounds.end) {
    int minIndex = sortedIndex;
    int minValue = list[sortedIndex];
    for (int i=sortedIndex ; i<=bounds.end ; i++){
      if (list[i] < minValue) {
        minIndex = i;
        minValue = list[i];
      }
    }
    if (minIndex != sortedIndex){
      swap(list, minIndex, sortedIndex);
    }
    sortedIndex++;
  }
}

void quickSort(std::unique_ptr<int[]>& list, Bounds bounds){
  if (bounds.start < bounds.end){
    int pi = bounds.start; //Pivot index
    int pivot = list[bounds.end]; //Pivot value

    for (int i=bounds.start ; i<bounds.end ; i++){
      if (list[i] < pivot){
        swap(list, pi, i);
        pi++;
      }
    }

    swap(list, pi, bounds.end);

    quickSort(list, bounds.splitLeftExclude(pi));
    quickSort(list, bounds.splitRightExclude(pi));
  }
}

void mergeSort(std::unique_ptr<int[]>& list, Bounds bounds, int depth = 0){
  if (bounds.start < bounds.end){
    //Create a "centerIndex" to split lists down the middle
    int centerIndex = bounds.center();
    
    //Sort the 2 sub-lists created by center index
    mergeSort(list, bounds.splitLeftExclude(centerIndex), depth + 1);
    mergeSort(list, bounds.splitRightInclude(centerIndex), depth + 1);

    //Combine the sub-lists
    auto merged = std::make_unique<int[]>(bounds.length());
    
    int leftIndex = bounds.start;
    int rightIndex = centerIndex;
    for (int i=0 ; i<bounds.length() ; i++){
      if (leftIndex > centerIndex-1){
        merged[i] = list[rightIndex];
        rightIndex++;
      }
      else if (rightIndex > bounds.end){
        merged[i] = list[leftIndex];
        leftIndex++;
      }
      else if (list[leftIndex] < list[rightIndex]){
        merged[i] = list[leftIndex];
        leftIndex++;
      }
      else {
        merged[i] = list[rightIndex];
        rightIndex++;
      }
      if (merged[i] == 0){
        int a = 10;
        int b = 1;
      }
    }

    int listIndex = bounds.start;
    for (int i=0 ; i<bounds.length() ; i++){
      list[listIndex] = merged[i];
      listIndex++;
    }
  }
}

//Searches a list of integers for a given integer
bool search(std::unique_ptr<int[]>& list, Bounds bounds, int val){
  for (auto i=bounds.start ; i<=bounds.end ; i++){
    if (list[i] == val){
      return true;
    }
  }
  return false;
}

//Searches a sorted list of integers (low to high) for a given integer
bool binarySearch(std::unique_ptr<int[]>& list, Bounds bounds, int val){
  if (val < list[bounds.start] || val > list[bounds.end]){
    return false;
  }
  int centerIndex = bounds.center();
  if (val < list[centerIndex]){
    return binarySearch(list, bounds.splitLeftExclude(centerIndex), val);
  } else if (val > list[centerIndex]){
    return binarySearch(list, bounds.splitRightExclude(centerIndex), val);
  } else {
    return true;
  }
}


int main(){
  int length = 10;
  int testLength = 1000000;
  Bounds bounds = {0, length-1};
  Bounds testBounds = {0, testLength-1};
  auto list = getRandomList(length);
  auto list2 = copyList(list, length);
  auto list3 = copyList(list, length);
  auto list4 = copyList(list, length);
  
  // printList(list, bounds);

  Timer::testCount = 1000;

  // std::cout << std::endl << "Bubble Sort: " << std::endl;
  // bubbleSort(list, bounds);
  // printList(list, bounds);
  // Timer::test([=](){
  //   auto testList = getRandomList(testLength);
  //   bubbleSort(testList, testBounds);
  // });

  // std::cout << std::endl << "Selection Sort: " << std::endl;
  // selectionSort(list4, bounds);
  // printList(list4, bounds);
  // Timer::test([=](){
  //   auto testList = getRandomList(testLength);
  //   selectionSort(testList, testBounds);
  // });

  // std::cout << std::endl << "Merge Sort:" << std::endl;
  // mergeSort(list3, bounds);
  // printList(list3, bounds);
  // Timer::test([=](){
  //   auto testList = getRandomList(testLength);
  //   mergeSort(testList, testBounds);
  // });  

  // std::cout << std::endl << "Quick Sort:" << std::endl;
  // quickSort(list2, bounds);
  // printList(list2, bounds);
  // Timer::test([=](){
  //   auto testList = getRandomList(testLength);
  //   quickSort(testList, testBounds);
  // });
  
  std::cout << std::endl << "Inefficient Search:" << std::endl;
  auto testList = getRandomList(testLength);
  int success = 0;
  Timer::test([&](){
    int testVal = rand()%10000;
    if (search(testList, testBounds, testVal)){
      success++;
    }
  });
  std::cout << (float)success / (float)Timer::testCount << std::endl;

  success = 0;
  std::cout << std::endl << "Binary Search:" << std::endl;
  quickSort(testList, testBounds);
  Timer::test([&](){
    int testVal = rand()%10000;
    if (binarySearch(testList, testBounds, testVal)){
      success++;
    }
  });
  std::cout << (float)success / (float)Timer::testCount << std::endl;

  return 1;
}