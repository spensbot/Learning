#include <iostream>
#include <string>
#include <stdlib.h>
#include <memory>

namespace stm {

class LifeCycleLogger {
public:
  // Default constructor
  // LifeCycleLogger() {
  //   defaultConstructCount++;
  //   setId();

  //   const char* _tag = "default";

  //   tagSize = strlen(_tag);
  //   tag = new char[tagSize + 1];
  //   memcpy(tag, _tag, tagSize);
  //   tag[tagSize] = '\0';

  //   log("Constructed");
  // }

  LifeCycleLogger(const char* _tag) {
    defaultConstructCount++;
    setId();

    tagSize = strlen(_tag);
    tag = new char[tagSize + 1];
    memcpy(tag, _tag, tagSize);
    tag[tagSize] = '\0';

    log("Constructed");
  }

  ~LifeCycleLogger() {
    log("Destroyed");
    delete[] tag;
    if (++destroyCount == constructCount()) 
      std::cout << "All Destroyed" << std::endl;
  }

  // Copy constructor
  LifeCycleLogger(const LifeCycleLogger& other) {
    copyConstructCount++;
    setId();
    copyTag(other);
    logConstruct(other.id);
  }

  // Move constructor
  LifeCycleLogger(LifeCycleLogger&& other) {
    moveConstructCount++;
    setId();
    moveTag(std::move(other));
    logConstruct(other.id, true);
  }

  // Copy Assignment Operator
  LifeCycleLogger& operator=(const LifeCycleLogger& other) {
    // id = other.id;
    copyAssignCount++;
    copyTag(other);
    logAssign(other.id);
    return *this;
  }

  // Move Assignment Operator
  LifeCycleLogger& operator=(LifeCycleLogger&& other) {
    // id = other.id;
    moveAssignCount++;
    moveTag(std::move(other));
    logAssign(other.id, true);
    return *this;
  }

  void print() {
    std::cout << tag << std::endl;
  }

  static void printInfo(){
    std::cout << "Constructions: " << constructCount() << std::endl;
    printCount(defaultConstructCount, "default");
    printCount(copyConstructCount, "copy");
    printCount(moveConstructCount, "move");
    std::cout << "Assigns: " << assignCount() << std::endl;
    printCount(copyAssignCount, "copy");
    printCount(moveAssignCount, "move");
  }

  static int constructCount() {
    return defaultConstructCount + copyConstructCount + moveConstructCount;
  }

  static int assignCount() {
    return copyAssignCount + moveAssignCount;
  }

  static int moveCount() {
    return moveConstructCount + moveAssignCount;
  }

  static int copyCount() {
    return copyConstructCount + copyAssignCount;
  }

  friend std::ostream& operator<<(std::ostream&, const LifeCycleLogger&);

  inline static bool shouldLog = false;

private:
  inline static int defaultConstructCount = 0;
  inline static int copyConstructCount = 0;
  inline static int moveConstructCount = 0;
  inline static int copyAssignCount = 0;
  inline static int moveAssignCount = 0;
  inline static int destroyCount = 0;

  int id;

  char* tag = nullptr;
  int tagSize = 0;

  void setId() {
    id = constructCount();
  }

  void copyTag(const LifeCycleLogger& other) {
    tagSize = other.tagSize;
    tag = new char[tagSize + 1];
    memcpy(tag, other.tag, tagSize);
    tag[tagSize] = '\0';
  }

  void moveTag(LifeCycleLogger&& other) {
    tagSize = other.tagSize;
    tag = other.tag;
    other.tag = nullptr;
    other.tagSize = 0;
  }

  void log(const std::string& message){
    if (shouldLog) {
      std::cout << "  " << message << " " << id << " (" << (tag == nullptr ? "* moved *" : tag) << ")" << std::endl;
    }
  }

  void logConstruct(size_t idOther, bool isMove = false){
    if (shouldLog) {
      std::cout << "  Constructed " << id << " based on " << idOther << (isMove ? " (Move: " : " (Copy: ") << tag << ")" << std::endl;
    }
  }

  void logAssign(size_t idOther, bool isMove = false){
    if (shouldLog) {
      std::cout << "  Assigned " << id << " = " << idOther << (isMove ? " (Move: " : " (Copy: ") << tag << ")" << std::endl;
    }
  }

  static void printCount(int count, std::string type){
    std::cout << "  " << count << " " << type << std::endl;
  }
};

std::ostream& operator<<(std::ostream& os, const LifeCycleLogger& lcl) {
  os << lcl.id << " (" << (lcl.tag == nullptr ? "* moved *" : lcl.tag) << ")";
  return os;
}

} // namespace stm