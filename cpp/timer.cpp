#include <iostream>
#include <chrono>
#include <functional>
#include <vector>

struct Entity
{
  Entity(double x, double y, double z) : x(x), y(y), z(z) {}
  double x,y,z;
};

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

  static void Test(std::function<void()> testFunction){
    auto startPoint = clock::now();

    for (auto i=0 ; i<testCount ; i++){
      testFunction();
    }

    auto endPoint = clock::now();

    auto duration = usSinceEpoch(endPoint) - usSinceEpoch(startPoint);

    std::cout << "Total test duration: " << duration << "us | " << duration/1000.0 << "ms" << std::endl;
    std::cout << "Average duration per call: " << duration/testCount << "us | " << duration/testCount/1000.0 << "ms" << std::endl;
  }

private:
  timePoint startPoint;
};


int main() {

  // { // RAII API
  //   int count=0;
  //   Timer timer;

  //   for (auto i=0 ; i<10000000 ; i++){
  //     count += 1;
  //   }
  // }

  // Callback API
  std::cout << "Empty Loop Test:" << std::endl;
  Timer::Test([](){
    return;
  });
  std::cout << std::endl;

  std::cout << "Test Stack Array:" << std::endl;
  Timer::Test([](){
    int hello[10];
  });
  std::cout << std::endl;

  std::cout << "Test Heap Array:" << std::endl;
  Timer::Test([](){
    int* hello = new int[10];
  });
  std::cout << std::endl;

  std::cout << "Test Efficient Vector: " << std::endl;
  Timer::Test([](){
    std::vector<Entity> entities;
    entities.reserve(5);
    entities.emplace_back(1,2,3);
    entities.emplace_back(1,2,3);
    entities.emplace_back(1,2,3);
    entities.emplace_back(1,2,3);
    entities.emplace_back(1,2,3);
  });
  std::cout << std::endl;

  std::cout << "Test Inefficient Vector: " << std::endl;
  Timer::Test([](){
    std::vector<Entity> entities;
    entities.push_back({1,2,3});
    entities.push_back({1,2,3});
    entities.push_back({1,2,3});
    entities.push_back({1,2,3});
    entities.push_back({1,2,3});
  });
  std::cout << std::endl;

  return 1;
}