// Compile with:
// g++ -std=c++17 string.cpp -o prog
#include <iostream>
#include <string>

int main() {

  // Type out the example.
  // We'll work with std::strings a lot to parse and manipulate data.

  std::string fullName = "Dongwook Lee";

  std::string firstName = fullName.substr(0, 8);
  std::string lastName = fullName.substr(9, 13);

  std::cout << "First: " << firstName << std::endl;
  std::cout << "Last: " << lastName << std::endl;

  unsigned int i = 0;
  while (i < fullName.length()) {
    if (fullName.at(i) == ' ') {
      break;
    }
    std::cout << fullName[i];
    ++i;
  }

  return 0;
}
