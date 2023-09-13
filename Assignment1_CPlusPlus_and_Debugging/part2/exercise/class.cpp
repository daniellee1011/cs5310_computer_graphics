// g++ -std=c++17 class.cpp -o prog
#include <cstdint>
#include <iostream>
#include <vector>

class PPM {
public:
  PPM(unsigned int width, unsigned int height) {
    pixelData = new unsigned char[width * height * 3];
    dataSize = width * height * 3;
  }

  ~PPM() { delete[] pixelData; }

  void printPixelDataSize() {
    std::cout << "Size of pixelData: " << dataSize << " bytes" << std::endl;
  }

private:
  uint8_t *pixelData;
  unsigned int dataSize;
};

int main(int argc, char **argv) {

  // You know what you need to do... :) Type it out.
  //
  //
  // Here's something I found funny as a reward:
  // https://stackoverflow.blog/2021/04/19/how-often-do-people-actually-copy-and-paste-from-stack-overflow-now-we-know/
  //
  // (Stack overflow is good too--but copying and pasting doesn't help us learn)
  //
  //
  // Anyway, at this point you know most of the C++ you'll need to get you
  // through the course.

  PPM myPPM(64, 128);
  myPPM.printPixelDataSize();

  return 0;
}
