#include <iostream>
#include <string>

namespace stm {

class LifeCycleLogger {
public:
  // Default constructor
  LifeCycleLogger() {
    m_id = currentId;
    currentId++;
    log("Constructed");
  }

  ~LifeCycleLogger() {
    log("Destroyed");
  }

  // Copy constructor
  LifeCycleLogger(LifeCycleLogger& other) {
    log("Copied");
  }

  // Move constructor
  LifeCycleLogger(LifeCycleLogger&& other) {
    log("Moved");
  }

  // LifeCycleLogger& operator=(const LifeCycleLogger& other) {

  // }

  // LifeCycleLogger& operator=(LifeCycleLogger&& other) {

  // }

  friend std::ostream& operator<<(std::ostream&, const LifeCycleLogger&);

  inline static bool shouldLog = true;

private:
  void log(const std::string& message){
    if (shouldLog) {
      std::cout << message << ": " << m_id << std::endl;
    }
  }

  inline static int currentId = 0;
  int m_id;
};

std::ostream& operator<<(std::ostream& os, const LifeCycleLogger& lcl) {
  os << "LifeCycleLogger: " << lcl.m_id;
  return os;
}

} // namespace stm