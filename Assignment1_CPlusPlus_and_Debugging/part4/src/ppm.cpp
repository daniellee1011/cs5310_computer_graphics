#include "PPM.hpp"
#include <fstream>
#include <iostream>
#include <sstream>

// Constructor loads a filename with the .ppm extension
PPM::PPM(std::string fileName) {
  // TODO:    Load and parse a ppm to get its pixel
  //          data stored properly.
  std::cout << fileName << std::endl;
  std::ifstream inFile;

  inFile.open(fileName);

  if (!inFile.is_open()) {
    throw std::invalid_argument("file open is failed!");
  }

  std::string line = "";
  std::getline(inFile, line);
  std::string type = "";
  type = line;
  std::cout << "type: " << type << std::endl;

  std::getline(inFile, line);
  std::getline(inFile, line);
  std::cout << "line: w_h: " << line << std::endl;
  std::stringstream ss(line);
  std::string word;
  std::vector<std::string> words;

  while (ss >> word) {
    words.push_back(word);
  }
  m_width = std::stoi(words[0]);
  m_height = std::stoi(words[1]);
  std::cout << m_width << " " << m_height << std::endl;

  std::getline(inFile, line);
  int max_value = std::stoi(line);
  std::cout << "max_value: " << max_value << std::endl;

  while (std::getline(inFile, line)) {
    if (line.empty())
      continue;

    if (line[0] == '#') {
      std::cout << "Comment: " << line << std::endl;
    } else {
      uint8_t pixelData = static_cast<uint8_t>(std::stoi(line));
      m_PixelData.push_back(pixelData);
    }
  }
}

// Destructor deletes(delete or delete[]) any memory that has been allocated
// or otherwise calls any 'shutdown' or 'destroy' routines for this deletion
// to occur.
PPM::~PPM() {
  // TODO: Reclaim any allocated memory
  //   delete[] m_PixelData;
}

// Saves a PPM Image to a new file.
void PPM::savePPM(std::string outputFileName) const {
  // TODO: Save a PPM image to disk
  std::cout << "outputFilename: " << outputFileName << std::endl;
  std::ofstream outFile;

  outFile.open(outputFileName);

  outFile << "P3" << std::endl;
  outFile << m_width << " " << m_height << std::endl;
  outFile << 255 << std::endl;
  for (int i = 0; i < m_PixelData.size(); i++) {
    outFile << (int)m_PixelData[i] << std::endl;
  }
}

// Darken halves (integer division by 2) each of the red, green
// and blue color components of all of the pixels
// in the PPM. Note that no values may be less than
// 0 in a ppm.
void PPM::darken() {
  // TODO: Output a 'filtered' PPM image.
  for (int i = 0; i < m_PixelData.size(); i++) {
    m_PixelData[i] = m_PixelData[i] / 2;
  }
}

// Lighten doubles (integer multiply by 2) each of the red, green
// and blue color components of all of the pixels
// in the PPM. Note that no values may be greater than
// 255 in a ppm.
void PPM::lighten() {
  // TODO: Output a 'filtered' PPM image.
}

// Sets a pixel to a specific R,G,B value
// Note: You do not *have* to use setPixel in your implementation, but
//       it may be useful to implement.
void PPM::setPixel(int x, int y, uint8_t R, uint8_t G, uint8_t B) {
  // TODO: Optional to implement.
}
