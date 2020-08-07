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

  void push(const T& newItem){
    reserve(size + 1);
    array[size] = newItem;
    size++;
  }

  void push(T&& newItem){
    reserve(size + 1);
    array[size] = std::move(newItem);
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

  const T& operator[] (int index) const {
    return array[index];
  }

  void reserve(size_t newCapacity){
    if ( capacity < newCapacity ) {
      reAllocate(newCapacity);
    }
  }

  size_t getSize(){return size;}

private:
  T* array = nullptr;
  size_t size = 0;
  size_t capacity = 0;

  void reAllocate(size_t newCapacity){
    std::cout << std::endl << "Reallocating for size: " << newCapacity << std::endl;

    auto oldArray = array;
    array = new T[newCapacity];

    for (int i=0 ; i<size ; i++){
      array[i] = std::move(oldArray[i]);
      oldArray[i].~T();
    }

    delete[] oldArray;

    capacity = newCapacity;
  }
};

} // namespace stm