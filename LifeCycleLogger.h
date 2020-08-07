#include <iostream>
#include <string>
#include <stdlib.h>
#include <memory>

namespace stm {

class LifeCycleLogger {
public:
  // Default constructor
  LifeCycleLogger() {
    m_id = ++numConstructions;
    
    heapTest = new char[n];
    for (int i=0 ; i<n-1 ; i++) {
      heapTest[i] = rand()%26 + 97;
    }
    heapTest[n-1] = '\0';
    log("Constructed");
  }

  ~LifeCycleLogger() {
    delete[] heapTest;
    log("Destroyed");
  }

  // Copy constructor
  LifeCycleLogger(LifeCycleLogger& other) {
    numCopies++;
    m_id = ++numConstructions;

    heapTest = new char[n];
    memcpy(heapTest, other.heapTest, n);
    
    if (shouldLog) {
      std::cout << "Copy Constructed " << this->m_id << " from " << other.m_id << std::endl;
    }
  }

  // Move constructor
  LifeCycleLogger(LifeCycleLogger&& other) {
    numMoves++;
    m_id = ++numConstructions;

    heapTest = other.heapTest;
    other.heapTest = nullptr;

    if (shouldLog) {
      std::cout << "Move Constructed " << this->m_id << " from " << other.m_id << std::endl;
    }
  }

  // Copy Assignment Operator
  LifeCycleLogger& operator=(const LifeCycleLogger& other) {
    ++numCopies;
    m_id = ++numConstructions;
    heapTest = new char[n];
    memcpy(heapTest, other.heapTest, n);

    if (shouldLog) {
      std::cout << "Copy Assigned " << this->m_id << " to " << other.m_id << std::endl;
    }
    return *this;
  }

  // Move Assignment Operator
  LifeCycleLogger& operator=(LifeCycleLogger&& other) {
    ++numMoves;
    m_id = ++numConstructions;
    heapTest = other.heapTest;
    other.heapTest=nullptr;

    if (shouldLog) {
      std::cout << "Move Assigned " << this->m_id << " to " << other.m_id << std::endl;
    }
    return *this;
  }

  static void printInfo(){
    std::cout << "Copies: " << numCopies << " Moves: " << numMoves << std::endl;
  }

  friend std::ostream& operator<<(std::ostream&, const LifeCycleLogger&);

  inline static bool shouldLog = true;

private:
  inline static int numConstructions = 0;
  inline static int numCopies = 0;
  inline static int numMoves = 0;
  int m_id;

  char* heapTest = nullptr;
  const int n = 10;

  void log(const std::string& message){
    if (shouldLog) {
      std::cout << message << ": " << m_id << std::endl;
    }
  }
};

std::ostream& operator<<(std::ostream& os, const LifeCycleLogger& lcl) {
  os << lcl.m_id << ": " << lcl.heapTest;
  return os;
}

} // namespace stm