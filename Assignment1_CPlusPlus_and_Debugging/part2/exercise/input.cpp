// Compile with:
// g++ -std=c++17 input.cpp -o prog
#include <fstream>
#include <iostream>
#include <string>

int main(int argc, char **argv) {

  // Learning how to read in values will be useful for the course.

  std::ifstream inFile;

  // 1st argument: .\prog.exe
  // 2nd argument: ..\..\..\common\objects\bunny_centered.obj
  // Executable file is also counted.
  if (argc != 2) {
    std::cerr << "Oops, exactly two arguments needed.\n";
    std::cerr << "ProgramName file_path.\n";
    return 0;
  }

  inFile.open(argv[1]);

  if (inFile.is_open()) {
    std::string line;
    while (getline(inFile, line)) {
      std::cout << line << std::endl;
    }
  } else {
    std::cout << "Filepath does not exist" << argv[1] << std::endl;
    std::cout << "Make sure the path is relative to where you are executing "
                 "your program"
              << std::endl;
  }

  // Not closing can cause resource limits, data integrity, and locking issues.
  inFile.close();

  return 0;
}
