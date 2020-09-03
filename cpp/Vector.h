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
    clear();
    ::operator delete(data, capacity * sizeof(T));
  }

  void push_back(const T& newItem){
    reserve(size + 1);
    data[size] = newItem;
    size++;
  }

  void push_back(T&& newItem){
    reserve(size + 1);
    data[size] = std::move(newItem);
    size++;
  }

  void pop_back() {
    size--;
    data[size].~T();
  }

  void clear() {
    for (int i=0 ; i<size ; i++)
      data[i].~T();
    size = 0;
  }

  template<typename... Args>
  T& emplace_back(Args&&... args){
    reserve(size + 1);

    new(&data[size]) T(std::forward<Args>(args)...);
    return data[size++];
  }

  T& operator[] (size_t index){
    return data[index];
  }

  const T& operator[] (size_t index) const {
    return data[index];
  }

  void reserve(size_t newCapacity){
    if ( capacity < newCapacity ) {
      reAllocate(newCapacity);
    }
  }

  size_t getSize(){return size;}

private:
  T* data = nullptr;
  size_t size = 0;
  size_t capacity = 0;

  void reAllocate(size_t newCapacity){
    std::cout << "Reallocating for size: " << newCapacity << std::endl;

    T* newData = (T*)::operator new(newCapacity * sizeof(T));

    for (int i=0 ; i<size ; i++) {
      newData[i] = std::move(data[i]);
      data[i].~T();
    }

    ::operator delete(data, capacity * sizeof(T));
    data = newData;
    capacity = newCapacity;
  }
};

} // namespace stm