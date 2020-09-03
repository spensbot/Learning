#include <iostream>
#include <functional>

class Test
{
public:
  Test(std::function<void(int)>f1, std::function<int()>f2)
    : func1(f1)
    , func2(f2)
  {
    
  }

  void runBoth() {
    func1(func2());
  }

private:
  std::function<void(int)> func1;
  std::function<int()> func2;

};

int main() {
  int x = 2;
  int y = 3;

  auto func = [&x, y](int z) mutable {
    x++;
    y++;
    z++;
    std::cout << "x: " << x << std::endl;
    std::cout << "y: " << y << std::endl;
    std::cout << "z: " << z << std::endl;
  };

  auto func2 = [&](){
    x*=2;
    y*=2;
    return x + y;
  };

  Test test(func, func2);

  test.runBoth();

  std::cout << "x: " << x << std::endl;
  std::cout << "y: " << y << std::endl;

  return 1;
}