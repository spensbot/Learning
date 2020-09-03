int main()
{
  int n = 0;
  for (int i  = 0 ; i<100 ; i++) {
    n += 2;
  }
  n = n * 3;
  return n;
}

/**
 * g++ -S -O3 -std=c++17 assembly.cpp
 * g++ -S -std=c++17 assembly.cpp
 **/