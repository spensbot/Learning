#include <iostream>
#include <vector>

struct Entity 
{
  float x,y;

  Entity(float x, float y) : x(x), y(y)
  {
    std::cout << "Constructed" << std::endl;
  }

  Entity(const Entity& other) : x(other.x), y(other.y)
  {
    std::cout << "Copied" << std::endl;
  }

  Entity(Entity&& other)
  {
    std::cout << "Moved" << std::endl;
    x = other.x;
    y = other.y;
  }

  void print() const {
    std::cout << "x: " << x << " | y: " << y << std::endl;
  }

};

int main(){
  { 
    std::cout << "EFFICIENT" << std::endl;
    std::vector<Entity> entities;

    //Use reserve to allocate memory for multiple future pushes in a single go
    entities.reserve(4);
    //Use emplace_back to pass constructor arguments to have objects constructed in place
    entities.emplace_back(1, 2);
    entities.emplace_back(3, 4);
    entities.emplace_back(5, 6);

    // Use a reference to prevent copies if using this iterator
    for (const Entity& entity : entities){
      entity.print();
    }
    // The standard iterator does not create copies
    // for (int i=0 ; i<entities.size() ; i++){
    //   entities[i].print();
    // }

    std::cout << std::endl;
  }

  { 
    std::cout << "INEFFICIENT" << std::endl;
    std::vector<Entity> entities;

    entities.push_back({1, 2});
    entities.push_back({3, 4});
    entities.push_back({5, 6});

    for (Entity entity : entities){
      entity.print();
    }
    std::cout << std::endl;
  }

  return 1;
}