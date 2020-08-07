#include <iostream>
#include <memory>

namespace stm {

template <typename T>
class Vector
{
public:
  Vector(){

  }

  ~Vector(){
    delete[] array;
  }

  void push(T newItem){
    ensureCapacity(size + 1);
    array[size] = newItem;
    size++;
  }

  T pop(){
    size--;
    T item = array[size];
    return item;
  }

  T& operator[] (int index){
    return array[index];
  }

  void reserve(size_t neededSize){
    ensureCapacity(neededSize);
  }

  size_t getSize(){return size;}

private:
  T* array = nullptr;
  size_t size = 0;
  size_t allocated = 0;

  void ensureCapacity(size_t neededSize){
    if ( ! (allocated >= neededSize)) {
      auto oldArray = array;
      array = new T[neededSize];

      memcpy(array, oldArray, sizeof(T) * size);

      delete[] oldArray;

      allocated = neededSize;

      std::cout << "Reallocating for size: " << allocated << std::endl;
    }
  }
};

} // namespace stm