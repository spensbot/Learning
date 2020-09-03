#include <iostream>

struct Vector3 {
  int intArray[10];
  long longArray[5];
  float x, y, z;
};

int main(){
  
  //long offset = (long)&((Vector3*)nullptr)->x;

  std::nullptr_t nullPtrVar = nullptr;
  Vector3* vector3Pointer = (Vector3*)nullPtrVar;
  float* memberXPointer = &vector3Pointer->x;
  long memberXPointerLong = (long) memberXPointer;
  long yPointerLong = (long)&vector3Pointer->y;
  long zPointerLong = (long)&vector3Pointer->z;

  std::cout << "nullptr vector3Pointer: " << vector3Pointer << std::endl;
  std::cout << "nullptr memberXPointer: " << memberXPointer << std::endl;
  std::cout << "nullptr memberXPointerLong: " << memberXPointerLong << std::endl;
  std::cout << "nullptr memberYPointerLong: " << yPointerLong << std::endl;
  std::cout << "nullptr memberZPointerLong: " << zPointerLong << std::endl;


  std::cout << std::endl;

  Vector3* vector3Pointer2 = new Vector3();
  float* memberXPointer2 = &vector3Pointer2->x;
  long memberXPointerLong2 = (long) memberXPointer2;
  long yPointerLong2 = (long)&vector3Pointer2->y;
  long zPointerLong2 = (long)&vector3Pointer2->z;

  std::cout << "real vector3Pointer: " << vector3Pointer2 << std::endl;
  std::cout << "real memberXPointer: " << memberXPointer2 << std::endl;
  std::cout << "real memberXPointerLong: " << memberXPointerLong2 << std::endl;
  std::cout << "real memberYPointerLong: " << yPointerLong2 << std::endl;
  std::cout << "real memberZPointerLong: " << zPointerLong2 << std::endl;

  return 1;
}

