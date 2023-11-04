#include "PPM.hpp"
#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>

// Constructor loads a filename with the .ppm extension
PPM::PPM(std::string fileName) {
  // TODO:    Load and parse a ppm to get its pixel
  //          data stored properly.
  std::cout << "PPM: filename: " << fileName << std::endl;
  std::cout << "PPM: Current path: " << std::filesystem::current_path() << '\n';

  std::ifstream inFile(fileName);
  if (!inFile.is_open()) {
    throw std::invalid_argument("File open failed!");
  }

  std::string line;
  std::getline(inFile, line);
  if (line != "P3") {
    throw std::invalid_argument("Unsupported PPM format or missing P3 header!");
  }

  // Skip comments
  while (std::getline(inFile, line)) {
    if (line[0] == '#')
      continue;
    std::stringstream dimensionsStream(line);
    dimensionsStream >> m_width >> m_height;
    break;
  }

  int m_maxColorValue;
  inFile >> m_maxColorValue;
  if (m_maxColorValue != 255) {
    throw std::invalid_argument("Unsupported max color value!");
  }

  // Read pixel data
  int r, g, b;
  while (inFile >> r >> g >> b) {
    m_PixelData.push_back(static_cast<uint8_t>(r));
    m_PixelData.push_back(static_cast<uint8_t>(g));
    m_PixelData.push_back(static_cast<uint8_t>(b));
  }

  std::cout << "Loaded image dimensions: " << m_width << " x " << m_height
            << std::endl;
  std::cout << "First 10 pixel values: ";
  for (int i = 0; i < 30;
       i++) { // Since RGB is 3 values, we'll get the first 10 pixels
    std::cout << (int)m_PixelData[i] << " ";
  }
  std::cout << std::endl;
}

// Destructor deletes(delete or delete[]) any memory that has been allocated
// or otherwise calls any 'shutdown' or 'destroy' routines for this deletion
// to occur.
PPM::~PPM() {
  if (m_textureID != 0) {
    glDeleteTextures(1, &m_textureID);
  }
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
  for (int i = 0; i < m_PixelData.size(); i += 3) {
    outFile << (int)m_PixelData[i] << " " << (int)m_PixelData[i + 1] << " "
            << (int)m_PixelData[i + 2] << std::endl;
  }
}

// Sets a pixel to a specific R,G,B value
// Note: You do not *have* to use setPixel in your implementation, but
//       it may be useful to implement.
void PPM::setPixel(int x, int y, uint8_t R, uint8_t G, uint8_t B) {
  // TODO: Optional to implement.
  if (x < 0 || x >= 512 || y < 0 || y >= 512) {
    std::cerr << "Error: Invalid cooridnates!" << std::endl;
    return;
  }

  int pos = (x * 512 + y) * 3;
  m_PixelData[pos] = R;
  m_PixelData[pos + 1] = G;
  m_PixelData[pos + 2] = B;
}

bool PPM::createTexture() {
  //   std::cout << "Creating texture from PPM data." << std::endl;

  if (m_textureID == 0 && !m_PixelData.empty()) {
    // std::cout << "PPM::createTexture(): true" << std::endl;
    // glEnable(GL_TEXTURE_2D);
    glGenTextures(1, &m_textureID);
    glBindTexture(GL_TEXTURE_2D, m_textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_width, m_height, 0, GL_RGB,
                 GL_UNSIGNED_BYTE, m_PixelData.data());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    // if (m_textureID != 0) {
    //   std::cout << "Texture created with ID: " << m_textureID << std::endl;
    // } else {
    //   std::cout << "Failed to create texture." << std::endl;
    // }
    GLenum err = glGetError();
    if (err != GL_NO_ERROR) {
      std::cerr << "OpenGL error during texture creation: " << err << std::endl;
      return false; // or handle the error as appropriate
    }
    return true;
  }
  //   std::cout << "PPM::createTexture(): false" << std::endl;
  return false;
}