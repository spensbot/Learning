#include <iostream>
#include <chrono>
#include <functional>
#include <vector>

class Timer{
public:
  using clock = std::chrono::high_resolution_clock;
  using us = std::chrono::microseconds;
  using timePoint= std::chrono::time_point<std::chrono::high_resolution_clock>;

  inline static size_t testCount = 1000000; // 1 million by default

  Timer(){
    startPoint = std::chrono::high_resolution_clock::now();
  }
  ~Timer(){
    stop();
  }
  void stop(){
    auto endPoint = std::chrono::high_resolution_clock::now();

    auto startUs = std::chrono::time_point_cast<std::chrono::microseconds>(startPoint).time_since_epoch().count();
    auto endUs = std::chrono::time_point_cast<std::chrono::microseconds>(endPoint).time_since_epoch().count();
    auto duration = endUs - startUs;

    std::cout << "us: " << duration << " | ms: " << duration / 1000.0 << std::endl;
  }

  static int64_t usSinceEpoch(timePoint point){
    return std::chrono::time_point_cast<us>(point).time_since_epoch().count();
  }

  static void test(std::function<void()> testFunction){
    auto startPoint = clock::now();

    for (auto i=0 ; i<testCount ; i++){
      testFunction();
    }

    auto endPoint = clock::now();

    auto duration = usSinceEpoch(endPoint) - usSinceEpoch(startPoint);

    if (testCount > 1){
      std::cout << "Testing " << testCount << " times" << std::endl;
    }
    std::cout << "Total Duration: " << duration << "us | " << duration/1000.0 << "ms" << std::endl;
    if (testCount > 1){
      std::cout << "Average duration per call: " << duration/testCount << "us | " << duration/testCount/1000.0 << "ms" << std::endl;
    }
  }

private:
  std::chrono::time_point<std::chrono::high_resolution_clock> startPoint;
};
