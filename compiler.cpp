/**
 * Note: g++ flags
 * -E : stop after preprocessing (preprocessed code)
 * -S : stop after compiling (assembly code)
 * -c : stop after assembly (non-linked files)
 * -o : preprocess, compile, assemble, and link all files
 * -O3 : release build optimizations
**/

#include "compiler.h"
#include "compiler1.h"
#include "compiler2.h"

void Compiler::print() {
  std::cout << "Compiler, reporting for duty!" << std::endl;
}

int main() 
{
  Compiler comp;
  Compiler1 comp1;
  Compiler2 comp2;

  comp.print();
  comp1.print();
  comp2.print();

  return 1;
}