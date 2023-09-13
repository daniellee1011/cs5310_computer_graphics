// g++ -std=c++17 vector2.cpp -o prog
#include <iostream>
#include <vector>

int main(int argc, char **argv) {

  // You need to understand what is going on with sizeof.
  // This exercise exists because on average, 25% of folks
  // are going to try to use 'sizeof' and expect to get the number
  // of elements in a vector, rather than the size of the underlying data type.

  std::vector<unsigned char> myVector;

  for (unsigned int i = 0; i < 91; ++i) {
    myVector.push_back(i);
  }

  // The basic unit is a byte, and the output of 'sizeof' can vary based on
  // platform, compiler, and architecture.
  std::cout << "(number of elements) myVector.size() = " << myVector.size()
            << std::endl;
  // This indicates the size of the 'std::vector' object itself.
  std::cout << "(size of data structure) sizeof(myVector) = "
            << sizeof(myVector) << std::endl;
  // This indicates the size of a pointer to the first element of the vector's
  // underlying array.
  std::cout << "(raw array myVector is built on top of) "
               "sizeof(myVector.data()) = "
            << sizeof(myVector.data()) << std::endl;
  std::cout << std::endl;

  int rawArray[100];
  std::cout
      << "locally allocated, (i.e. stack allocated array) sizeof(rawArray) = "
      << sizeof(rawArray) << std::endl;
  std::cout << std::endl;

  int *heapArray = new int[52];
  std::cout << "heap allocated array, (i.e. pointer to a chunk of memory) "
               "sizeof(heapArray) = "
            << sizeof(heapArray) << std::endl;
  std::cout << std::endl;

  delete[] heapArray;

  return 0;
}
